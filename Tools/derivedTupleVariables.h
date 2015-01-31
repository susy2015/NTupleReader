#include "NTupleReader.h"

#include "TLorentzVector.h"
#include "Math/VectorUtil.h"

#include <vector>
#include <iostream>

namespace plotterFunctions
{
    void generateWeight(NTupleReader& tr)
    {
        // Calculate PU weight

        // Calculate Z-eff weight
        
        
    }

    void muInfo(NTupleReader& tr)
    {
        const std::vector<int>& genDecayPdgIdVec        = tr.getVec<int>("genDecayPdgIdVec");
        const std::vector<TLorentzVector>& genDecayLVec = tr.getVec<TLorentzVector>("genDecayLVec");
        const std::vector<TLorentzVector>& muonsLVec    = tr.getVec<TLorentzVector>("muonsLVec");
        const std::vector<double>& muonsRelIso          = tr.getVec<double>("muonsRelIso");
        const std::vector<int>& W_emuVec                = tr.getVec<int>("W_emuVec");

        std::vector<int>* genMatchMu = new std::vector<int>();
        std::vector<TLorentzVector>* cutMuVec = new std::vector<TLorentzVector>();

        for(int i = 0; i < muonsLVec.size(); ++i)
        {
            if(muonsRelIso[i] < 0.20 && muonsLVec[i].Pt() > 10)
            {
                cutMuVec->push_back(muonsLVec[i]);
            }
        }

        for(int j = 0; j < cutMuVec->size(); ++j)
        {
            double dRMin = 999.9;

            for(int i = 0; i < genDecayPdgIdVec.size() && i < genDecayLVec.size(); ++i)
            {
                double dR = ROOT::Math::VectorUtil::DeltaR(genDecayLVec[i], (*cutMuVec)[j]);
                if(abs(genDecayPdgIdVec[i]) == 13)
                {
                    if(dR < dRMin)
                    {
                        dRMin = dR;
                    }
                }
         
            }
            if(dRMin < 0.02)
            {
                genMatchMu->push_back(j);
            }
        }

        double genZPt = -999.9, genZEta = -999.9, genZmass = -999.9;
        int nZ = 0;
        for(int j = 0; j <  genDecayPdgIdVec.size(); ++j)
        {
            if(abs(genDecayPdgIdVec[j]) == 23)
            {
                nZ++;
                genZPt = genDecayLVec[j].Pt();
                genZEta = genDecayLVec[j].Eta();
                genZmass = genDecayLVec[j].M();
            }
        }
        if(nZ > 1) std::cout << "!!!WARNING MORE THAN 1 Z FOUND!!!" << std::endl;

        int pdgIdZDec = 0;
        if(W_emuVec.size() == 0) pdgIdZDec = 15;
        else if(W_emuVec.size() == 2)
        {
            if(abs(genDecayPdgIdVec[W_emuVec[0]]) == 11) pdgIdZDec = 11;
            else if(abs(genDecayPdgIdVec[W_emuVec[0]]) == 13) pdgIdZDec = 13;
        }

        tr.registerDerivedVec("cutMuVec", cutMuVec);
        tr.registerDerivedVec("genMatchMu", genMatchMu);
        tr.registerDerivedVar("genZPt", genZPt);
        tr.registerDerivedVar("genZEta", genZEta);
        tr.registerDerivedVar("genZmass", genZmass);
        tr.registerDerivedVar("pdgIdZDec", pdgIdZDec);
    }

    void cleanJets(NTupleReader& tr)
    {
        const std::vector<TLorentzVector>& jetsLVec  = tr.getVec<TLorentzVector>("jetsLVec");
        const std::vector<TLorentzVector>& elesLVec  = tr.getVec<TLorentzVector>("elesLVec");
        const std::vector<TLorentzVector>& muonsLVec = tr.getVec<TLorentzVector>("muonsLVec");
        const std::vector<double>& elesRelIso        = tr.getVec<double>("elesRelIso");
        const std::vector<double>& muonsRelIso       = tr.getVec<double>("muonsRelIso");

        if(elesLVec.size() != elesRelIso.size() || muonsLVec.size() != muonsRelIso.size())
        {
            std::cout << "MISMATCH IN VECTOR SIZE!!!!!" << std::endl;
            return;
        }

        std::vector<const TLorentzVector*>* cleanJetVec = new std::vector<const TLorentzVector*>();

        const double lPtMin = 5.0;
        const double ldBetaMax = 0.2;
        const double jldRMax = 0.3;

        const double HT_jetPtMin = 50;
        const double HT_jetEtaMax = 2.4;

        double HT = 0.0;

        for(auto& jet : jetsLVec)
        {
            double dRmin = 999.0;

            for(int i = 0; i < elesLVec.size() && i < elesRelIso.size(); ++i)
            {
                if(elesRelIso[i] > ldBetaMax || elesLVec[i].Pt() < lPtMin) continue;
                double dR = ROOT::Math::VectorUtil::DeltaR(jet, elesLVec[i]);
                dRmin = std::min(dRmin, dR);
            }

            for(int i = 0; i < muonsLVec.size() && i < muonsRelIso.size(); ++i)
            {
                if(muonsRelIso[i] > ldBetaMax || muonsLVec[i].Pt() < lPtMin) continue;
                double dR = ROOT::Math::VectorUtil::DeltaR(jet, muonsLVec[i]);
                dRmin = std::min(dRmin, dR);
            }

            if(dRmin > jldRMax) 
            {
                cleanJetVec->push_back(&jet);
                if(jet.Pt() > HT_jetPtMin && fabs(jet.Eta()) < HT_jetEtaMax) HT += jet.Pt();
            }
        }

        tr.registerDerivedVar("cleanHt", HT);
        tr.registerDerivedVec("cleanJetVec", cleanJetVec);
    }
    
    void registerFunctions(NTupleReader& tr)
    {
        tr.registerFunction(&cleanJets);
        tr.registerFunction(&generateWeight);
        tr.registerFunction(&muInfo);
    }

    void activateBranches(std::set<std::string>& activeBranches)
    {
        activeBranches.insert("genDecayPdgIdVec");
        activeBranches.insert("genDecayLVec");
        activeBranches.insert("muonsLVec");
        activeBranches.insert("muonsRelIso");
        activeBranches.insert("elesLVec");
        activeBranches.insert("elesRelIso");
        activeBranches.insert("W_emuVec");
    }
}

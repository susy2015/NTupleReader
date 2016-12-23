#ifndef ANABASELINE_BASELINEDEF_H
#define ANABASELINE_BASELINEDEF_H

#include "NTupleReader.h"
#include "customize.h"
#include "EventListFilter.h"

#include "recipeAUX/OxbridgeMT2/interface/Basic_Mt2_332_Calculator.h"
#include "recipeAUX/OxbridgeMT2/interface/ChengHanBisect_Mt2_332_Calculator.h"


#include "Math/VectorUtil.h"

#include <memory>
#include <iostream>

class BaselineVessel
{
private:
    const std::string spec;
    bool isfastsim;
    std::string firstSpec;
    bool printOnce;

    NTupleReader *tr;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TopTagger ~~~~~
    std::shared_ptr<topTagger::type3TopTagger> type3Ptr;
    std::shared_ptr<TopTagger> ttPtr;

    //  container
    TLorentzVector metLVec; 
    std::vector<TLorentzVector> *jetsLVec_forTagger;
    std::vector<double> *recoJetsBtag_forTagger;
    std::vector<double> *qgLikelihood_forTagger;
    std::vector<TLorentzVector> *vTops;
    std::map<int, std::vector<TLorentzVector> > *mTopJets;

public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Member ~~~~~
    int  bToFake;
    bool debug;
    bool incZEROtop;
    bool UseNewTagger;

    std::string jetVecLabel;
    std::string CSVVecLabel;
    std::string METLabel;
    std::string METPhiLabel;
    std::string jetVecLabelAK8;
    std::string muonsFlagIDLabel;
    std::string elesFlagIDLabel;
    std::string qgLikehoodLabel;
    std::string toptaggerCfgFile;
    bool doIsoTrksVeto;
    bool doMuonVeto;
    bool doEleVeto;
    bool doMET;
    bool dodPhis;
    bool passBaseline;
    bool passBaselineNoTagMT2;
    bool passBaselineNoTag;
    bool passBaselineNoLepVeto;


    BaselineVessel(NTupleReader &tr_, const std::string specialization = "", const std::string filterString = "");
    ~BaselineVessel();

    void PassBaseline();
    bool passNoiseEventFilterFunc();
    bool passQCDHighMETFilterFunc();
    bool passFastsimEventFilterFunc();
    bool PredefineSpec();

    void operator()(NTupleReader& tr);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TopTagger ~~~~~
    bool SetupTopTagger(bool UseNewTagger_ = true, std::string CfgFile_ = "TopTagger.cfg");
    bool PassTopTagger();
    bool GetMHT() const;
    bool GetLeptons() const;
    void prepareTopTagger();
    std::shared_ptr<topTagger::type3TopTagger> GetType3Ptr() const {return type3Ptr;};
    std::shared_ptr<TopTagger> GetTopTaggerPtr() const {return ttPtr;};
    int GetnTops() const;
    double CalcMT2() const;
};

inline void passBaselineFunc(NTupleReader &tr, std::string filterstring)
{
  BaselineVessel blv(tr, "", filterstring);
  blv.PassBaseline();
  blv.GetMHT();
}

namespace stopFunctions
{
    class CleanJets
    {
    public:        
        void operator()(NTupleReader& tr) {internalCleanJets(tr);}

        void setMuonIso(const std::string muIsoFlag);
        void setElecIso(const std::string elecIsoFlag);
        void setJetCollection(std::string jetVecLabel);
        void setBTagCollection(std::string bTagLabel);
        void setMuonsFlagID(std::string muonsFlagIDLabel);
        void setElesFlagID(std::string elesFlagIDLabel);
        void setEnergyFractionCollections(std::string chargedEMfrac, std::string neutralEMfrac, std::string chargedHadfrac);
        void setForceDr(bool forceDr);
        void setDisable(bool disable);
        void setRemove(bool remove);
        void setElecPtThresh(double minPt);
        void setMuonPtThresh(double minPt);
        void setDisableElec(bool disable);
        void setDisableMuon(bool disable);
        //This option is used to clean up to 1 jet in the minDr cone around the muon if the jet is lower pt than the muon
        //It is designed only for use with the z->inv background to remove muon related radiation from the event
        void setJecScaleRawToFull(std::string jecScaleRawToFullLabel);
        void setPhotoCleanThresh(double photoCleanThresh);

        //NOTE!!! Must add Hadron and EM fraction vectors here

        CleanJets()
        {
            setMuonIso("mini");
            setElecIso("mini");
            setJetCollection("jetsLVec");
            setBTagCollection("recoJetsBtag_0");
            setMuonsFlagID("muonsFlagMedium");
            setElesFlagID("elesFlagVeto");
            setEnergyFractionCollections("recoJetschargedHadronEnergyFraction", "recoJetsneutralEmEnergyFraction", "recoJetschargedEmEnergyFraction");    
            setForceDr(false);
            setRemove(false);
            setDisable(false);
            setElecPtThresh(0.0);
            setMuonPtThresh(0.0);
            setPhotoCleanThresh(-999.9);
            setJecScaleRawToFull("recoJetsJecScaleRawToFull");
        }
        
    private:
        std::string muIsoStr_, elecIsoStr_, jetVecLabel_, bTagLabel_, chargedEMFracLabel_, neutralEMFracLabel_, chargedHadFracLabel_;
        std::string muonsFlagIDLabel_, elesFlagIDLabel_;
        std::string recoJetsJecScaleRawToFullLabel_;
        AnaConsts::IsoAccRec muIsoReq_;
        AnaConsts::ElecIsoAccRec elecIsoReq_;
        double elecPtThresh_;
        double muonPtThresh_;
        double photoCleanThresh_;
        bool remove_;
        bool disableMuon_, disableElec_;
        bool forceDr_;

        int cleanLeptonFromJet(const TLorentzVector& lep, const int& lepMatchedJetIdx, const std::vector<TLorentzVector>& jetsLVec, const std::vector<double>& jecScaleRawToFull, std::vector<bool>& keepJet, const std::vector<double>& neutralEmEnergyFrac, std::vector<TLorentzVector>* cleanJetVec, const double& jldRMax, const double photoCleanThresh = -999.9);
        void internalCleanJets(NTupleReader& tr);
    };

    inline void cleanJets(NTupleReader& tr)
    {
      CleanJets cjh;
      cjh(tr);
    }
}

#endif

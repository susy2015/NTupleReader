#include "samples.h"

#include <iostream>
#include <cstdio>
#include <cstring>

namespace AnaSamples
{
    void FileSummary::readFileList() const
    {
        if(filelist_.size()) filelist_.clear();
        
        FILE *f = fopen(filePath.c_str(), "r");
        char buff[512];
        if(f)
        {
            while(!feof(f) && fgets(buff, 512, f))
            {
                for(char* k = strchr(buff, '\n'); k != 0; k = strchr(buff, '\n')) *k = '\0';
                filelist_.push_back(buff);
            }
            fclose(f);
        }
        else std::cout << "Filelist file \"" << filePath << "\" not found!!!!!!!" << std::endl;
    }

    void FileSummary::addCollection(std::string colName)
    {
        collections_.insert(colName);
    }

    std::map<std::string, FileSummary>& SampleSet::getMap()
    {
        return sampleSet_;
    }
    
    SampleSet::SampleSet(std::string fDir, double lumi) : fDir_(fDir), lumi_(lumi)
    {
        // ---------------
        // - backgrounds -
        // ---------------

        // branching ratio info from PDG
        double W_Lept_BR = 0.1086*3;
        double TTbar_SingleLept_BR = 0.43930872; // 2*W_Lept_BR*(1-W_Lept_BR)
        double TTbar_DiLept_BR = 0.10614564; // W_Lept_BR^2

//        std::string MCloc = "Spring15_74X_Oct_2015_Ntp_v2X/";
        std::string MCloc = "Spring15_74X_Feb_2016_Ntp_v6X_forMoriond/";
        std::string MCloc2 = "Spring15_74X_Dec_2015_Ntp_v4X/";
        std::string DATAloc = "Spring15_74X_Feb_2016_Ntp_v6X_forMoriond/";
        std::string sigMCloc = "Spring15_74X_Dec_2015_Ntp_v4X/";

        if(fDir.compare("condor") == 0)
        {
            fDir_ = "";
            MCloc = "";
            MCloc2 = "";
            DATAloc = "";
            sigMCloc = "";
        }

        //TTbar samples
        // TTbarInc has LO xsec on McM : 502.20 pb. The NNLO is 831.76 pb. The k-factor for ttbar is: kt = 831.76/502.20 ~ 1.656233
        addSample("TTbarInc", fDir_ + MCloc2 + "TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 831.76,  lumi, 11344206, 1.0, kGreen);
        // 1.61 * kt 
        addSample("TTbar_HT-600to800", fDir_ + MCloc2 + "TTJets_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 2.666535,    lumi, 5119009, 1.0, kGreen);
        // 0.663 * kt
        addSample("TTbar_HT-800to1200", fDir_ + MCloc2 + "TTJets_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",  "stopTreeMaker/AUX", 1.098082,    lumi, 3510897, 1.0, kGreen);
        // 0.12 * kt
        addSample("TTbar_HT-1200to2500", fDir_ + MCloc2 + "TTJets_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.198748,    lumi, 1014678, 1.0, kGreen);
        // 0.00143 * kt
        addSample("TTbar_HT-2500toInf", fDir_ + MCloc2 + "TTJets_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",  "stopTreeMaker/AUX", 0.002368413, lumi, 507842,  1.0, kGreen);


        // Calculated from PDG BRs'. Not from the kt * xSec in McM.
        addSample("TTbarDiLep", fDir_ + MCloc + "TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",             "stopTreeMaker/AUX", 831.76*TTbar_DiLept_BR,         lumi, 30498962, 1.0, kGreen);
        addSample("TTbarSingleLepT", fDir_ + MCloc + "TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",    "stopTreeMaker/AUX", 831.76*0.5*TTbar_SingleLept_BR, lumi, 60144642, 1.0, kGreen);
        addSample("TTbarSingleLepTbar", fDir_ + MCloc + "TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 831.76*0.5*TTbar_SingleLept_BR, lumi, 59816364, 1.0, kGreen);

        // From https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#W_jets, kw = 1.21
        addSample("WJetsToLNu_HT_100to200", fDir_ + MCloc + "WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1345,  lumi, 10152718,  1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_200to400", fDir_ + MCloc + "WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 359.7,  lumi, 5221599,  1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_400to600", fDir_ + MCloc + "WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 48.91,  lumi, 1745914,  1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_600to800", fDir_ + MCloc + "WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 12.05,  lumi, 4041997,  1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_800to1200", fDir_ + MCloc + "WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX",  5.501,   lumi, 1574633,  1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_1200to2500", fDir_ + MCloc + "WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.329,   lumi, 210017,   1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_2500toInf", fDir_ + MCloc + "WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX",  0.03216, lumi, 253036,   1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_600toInf", fDir_ + MCloc + "WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 18.77,  lumi, 1039152,  1.21, kMagenta+1);
        
        //Z -> nunu
        // From https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#DY_Z, kz = 1.23
        addSample("ZJetsToNuNu_HT_100to200", fDir_ + MCloc + "ZJetsToNuNu_HT-100To200_13TeV-madgraph.txt", "stopTreeMaker/AUX", 280.35, lumi, 5154824, 1.23,  kTeal+4);
        addSample("ZJetsToNuNu_HT_200to400", fDir_ + MCloc + "ZJetsToNuNu_HT-200To400_13TeV-madgraph.txt", "stopTreeMaker/AUX", 77.67,  lumi, 24863552, 1.23,  kTeal+4);
        addSample("ZJetsToNuNu_HT_400to600", fDir_ + MCloc + "ZJetsToNuNu_HT-400To600_13TeV-madgraph.txt", "stopTreeMaker/AUX", 10.73,  lumi, 9591908, 1.23,  kTeal+4);
        addSample("ZJetsToNuNu_HT_600toInf", fDir_ + MCloc + "ZJetsToNuNu_HT-600ToInf_13TeV-madgraph.txt", "stopTreeMaker/AUX", 4.116,  lumi, 10202299, 1.23,  kTeal+4);

        //DY->ll
        // kz = 1.23
        addSample("DYJetsToLL_HT_100to200", fDir_ + MCloc + "DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 147.4, lumi, 11079482, 1.23,  kYellow-7);
        addSample("DYJetsToLL_HT_200to400", fDir_ + MCloc + "DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 40.99, lumi, 9580541,  1.23,  kYellow-7);
        addSample("DYJetsToLL_HT_400to600", fDir_ + MCloc + "DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 5.678, lumi, 10420186, 1.23,  kYellow-7);
        addSample("DYJetsToLL_HT_600toInf", fDir_ + MCloc + "DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 2.198, lumi, 4864654,  1.23,  kYellow-7);
        // NNLO
        addSample("DYJetsToLL_Inc", fDir_ + MCloc2 + "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",             "stopTreeMaker/AUX", 6025.2, lumi, 9042031, 1.0,  kYellow-7);

        //QCD
        // Ref. https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#QCD. But numbers are from McM.
        addSample("QCD_HT100to200", fDir_ + MCloc + "QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 27540000, lumi, 81637494, 1.0,  kBlue);
        addSample("QCD_HT200to300", fDir_ + MCloc + "QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 1735000, lumi, 18718905, 1.0,  kBlue);
        addSample("QCD_HT300to500", fDir_ + MCloc + "QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 366800, lumi, 19826197, 1.0,  kBlue);
        addSample("QCD_HT500to700", fDir_ + MCloc + "QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",    "stopTreeMaker/AUX", 29370, lumi, 19575436, 1.0,  kBlue);
        addSample("QCD_HT700to1000", fDir_ + MCloc + "QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 6524, lumi, 15356448, 1.0,  kBlue);
        addSample("QCD_HT1000to1500", fDir_ + MCloc + "QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",  "stopTreeMaker/AUX", 1064, lumi, 4963895, 1.0,  kBlue);
        addSample("QCD_HT1500to2000", fDir_ + MCloc + "QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",  "stopTreeMaker/AUX", 121.5, lumi, 3868886, 1.0,  kBlue);
        addSample("QCD_HT2000toInf", fDir_ + MCloc + "QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 25.42, lumi, 1864992, 1.0,  kBlue);
/*
        addSample("QCD_Pt_10to15", fDir_ + MCloc + "QCD_Pt_10to15_TuneCUETP8M1_13TeV_pythia8.txt",     "stopTreeMaker/AUX", 5887580000, lumi, 3449170, 1.0,  kBlue);
        addSample("QCD_Pt_15to30", fDir_ + MCloc + "QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8.txt",     "stopTreeMaker/AUX", 1837410000, lumi, 4942232, 1.0,  kBlue);
        addSample("QCD_Pt_30to50", fDir_ + MCloc + "QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8.txt",     "stopTreeMaker/AUX", 140932000, lumi, 4957245, 1.0,  kBlue);
        addSample("QCD_Pt_50to80", fDir_ + MCloc + "QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8.txt",     "stopTreeMaker/AUX", 19204300, lumi, 4978425, 1.0,  kBlue);
        addSample("QCD_Pt_80to120", fDir_ + MCloc + "QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8.txt",    "stopTreeMaker/AUX", 2762530, lumi, 3424782, 1.0,  kBlue);
        addSample("QCD_Pt_120to170", fDir_ + MCloc + "QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8.txt",   "stopTreeMaker/AUX", 471100, lumi, 3452896, 1.0,  kBlue);
        addSample("QCD_Pt_170to300", fDir_ + MCloc + "QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8.txt",   "stopTreeMaker/AUX", 117276, lumi, 3364368, 1.0,  kBlue);
        addSample("QCD_Pt_300to470", fDir_ + MCloc + "QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8.txt",   "stopTreeMaker/AUX", 7823, lumi, 2933611, 1.0,  kBlue);
        addSample("QCD_Pt_470to600", fDir_ + MCloc + "QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8.txt",   "stopTreeMaker/AUX", 648.2, lumi, 1936832, 1.0,  kBlue);
        addSample("QCD_Pt_600to800", fDir_ + MCloc + "QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8.txt",   "stopTreeMaker/AUX", 186.9, lumi, 1964128, 1.0,  kBlue);
        addSample("QCD_Pt_800to1000", fDir_ + MCloc + "QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8.txt",  "stopTreeMaker/AUX", 32.293, lumi, 1937216, 1.0,  kBlue);
        addSample("QCD_Pt_1000to1400", fDir_ + MCloc + "QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8.txt", "stopTreeMaker/AUX", 9.4183, lumi, 1487136, 1.0,  kBlue);
        addSample("QCD_Pt_1400to1800", fDir_ + MCloc + "QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8.txt", "stopTreeMaker/AUX", 0.84265, lumi, 197959, 1.0,  kBlue);
        addSample("QCD_Pt_1800to2400", fDir_ + MCloc + "QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8.txt", "stopTreeMaker/AUX", 0.114943, lumi, 193608, 1.0,  kBlue);
        addSample("QCD_Pt_2400to3200", fDir_ + MCloc + "QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8.txt", "stopTreeMaker/AUX", 0.00682981, lumi, 194456, 1.0,  kBlue);
        addSample("QCD_Pt_3200toInf", fDir_ + MCloc + "QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8.txt",  "stopTreeMaker/AUX", 0.000165445, lumi, 192944, 1.0,  kBlue);
*/
        //Other Samples
        // Aprox. NNLO
        addSample("tW_top", fDir_ + MCloc + "ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1.txt", "stopTreeMaker/AUX", 35.6, lumi,  995600,  1.0,  kYellow);
        // Aprox. NNLO
        addSample("tW_antitop", fDir_ + MCloc + "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1.txt", "stopTreeMaker/AUX", 35.6, lumi, 988500,  1.0,  kYellow);

        // NLO --> negative weights!
        // (sign of gen weight) * (lumi*xsec)/(effective number of events): effective number of events = N(evt) with positive weight - N(evt) with negative weight
        addSample("TTZToLLNuNu", fDir_ + MCloc + "TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.2529, lumi, 291495 - 106505,  1.0,  kOrange+2);
        addSample("TTZToQQ", fDir_ + MCloc + "TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt",          "stopTreeMaker/AUX", 0.5297, lumi, 550599 - 199201,  1.0,  kOrange+2);

        // NLO --> negative weights!
        addSample("TTWJetsToLNu", fDir_ + MCloc + "TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8.txt", "stopTreeMaker/AUX", 0.2043, lumi, 191379 - 61529,   1.0,  kSpring+2);
        addSample("TTWJetsToQQ", fDir_ + MCloc + "TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8.txt",  "stopTreeMaker/AUX", 0.4062, lumi, 632147 - 201817,  1.0,  kSpring+2);

        // NLO --> negative weights!  
        addSample("TTGJets", fDir_ + MCloc2 + "TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8.txt", "stopTreeMaker/AUX", 3.697, lumi,  3199309 - 1632921,  1.0,  kOrange+2);

        // ttH --> negative weights!
	addSample("ttHJetTobb", fDir_ + MCloc2 + "ttHJetTobb_M125_13TeV_amcatnloFXFX_madspin_pythia8.txt",             "stopTreeMaker/AUX", 0.2934,  lumi, 1047463 - 565141,   1.0,  kOrange+2);
	addSample("ttHJetToNonbb", fDir_ + MCloc2 + "ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_mWCutfix.txt", "stopTreeMaker/AUX", 0.2151,  lumi, 5206759 - 2818464,  1.0,  kOrange+2);
        
        // Di-boson
	// Ref. https://indico.cern.ch/event/439995/session/0/contribution/6/attachments/1143460/1638648/diboson_final.pdf (NNLO is given)
        addSample("WW", fDir_ + MCloc + "WW_TuneCUETP8M1_13TeV-pythia8.txt", "stopTreeMaker/AUX", 115.0,  lumi, 993640,  1.0,  kViolet+4); 
	// Ref. https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns (NLO from MCFM)
        addSample("WZ", fDir_ + MCloc + "WZ_TuneCUETP8M1_13TeV-pythia8.txt", "stopTreeMaker/AUX", 47.13,  lumi, 978512,  1.0,  kViolet+4);
        addSample("ZZ", fDir_ + MCloc + "ZZ_TuneCUETP8M1_13TeV-pythia8.txt", "stopTreeMaker/AUX", 16.523, lumi, 996944,  1.0,  kViolet+4);

        // Tri-boson: negative weights!
        addSample("WWZ", fDir_ + MCloc + "WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.1651,  lumi, 235734 - 14266,  1.0,  kViolet+2);
        addSample("WZZ", fDir_ + MCloc + "WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.05565, lumi, 234584 - 15416,  1.0,  kViolet+2);
        addSample("ZZZ", fDir_ + MCloc + "ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.01398, lumi, 231925 - 18075,  1.0,  kViolet+2);

        // --------
        // - data -
        // --------
	
	addSample("Data_SingleMuon_2015C", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_SingleMuon-Run2015C-25ns-05Oct2015.txt","stopTreeMaker/AUX", 17.226, 1.0,  kBlack);
	addSample("Data_SingleMuon_2015D_05Oct2015", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_SingleMuon-Run2015D-05Oct2015.txt","stopTreeMaker/AUX", 575.34, 1.0,  kBlack);
	addSample("Data_SingleMuon_2015D_PromptReco", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_SingleMuon-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 1670.38, 1.0,  kBlack);
//	addSample("Data_SingleElectron_2015C", fDir_ + MCloc + "Spring15_74X_Nov_2015_Ntp_v3p1_SingleElectron-Run2015C-25ns-05Oct2015.txt","stopTreeMaker/AUX", 16.345, 1.0,  kBlack);
//	addSample("Data_SingleElectron_2015D_05Oct2015", fDir_ + MCloc + "Spring15_74X_Nov_2015_Ntp_v 3p1_SingleElectron-Run2015D-05Oct2015.txt","stopTreeMaker/AUX", 552.67, 1.0,  kBlack);
//	addSample("Data_SingleElectron_2015D_PromptReco", fDir_ + MCloc + "Spring15_74X_Nov_2015_Ntp_v3p1_SingleElectron-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 993.25, 1.0,  kBlack);

//	addSample("Data_DoubleMuon_2015C", fDir_ + MCloc + "Spring15_74X_Nov_2015_Ntp_v3p1_DoubleMuon-Run2015C-25ns-05Oct2015.txt","stopTreeMaker/AUX", 16.345, 1.0,  kBlack);
//	addSample("Data_DoubleMuon_2015D_05Oct2015", fDir_ + MCloc + "Spring15_74X_Nov_2015_Ntp_v3p1_DoubleMuon-Run2015D-05Oct2015.txt","stopTreeMaker/AUX", 552.67, 1.0,  kBlack);
//	addSample("Data_DoubleMuon_2015D_PromptReco", fDir_ + MCloc + "Spring15_74X_Nov_2015_Ntp_v3p1_DoubleMuon-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 993.72, 1.0,  kBlack);

	addSample("Data_DoubleEG_2015C", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_DoubleEG-Run2015C-25ns-05Oct2015.txt","stopTreeMaker/AUX", 17.226, 1.0,  kBlack);
	addSample("Data_DoubleEG_2015D_05Oct2015", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_DoubleEG-Run2015D-05Oct2015.txt","stopTreeMaker/AUX", 575.34, 1.0,  kBlack);
	addSample("Data_DoubleEG_2015D_PromptReco", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_DoubleEG-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 1669.92, 1.0,  kBlack);

	addSample("Data_HTMHT_2015C", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_HTMHT-Run2015C-25ns-05Oct2015.txt","stopTreeMaker/AUX", 17.226, 1.0,  kBlack);
	addSample("Data_HTMHT_2015D_05Oct2015", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_HTMHT-Run2015D-05Oct2015.txt","stopTreeMaker/AUX", 574.23, 1.0,  kBlack);
	addSample("Data_HTMHT_2015D_PromptReco", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_HTMHT-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 1670.53, 1.0,  kBlack);

        // ----------
	// - signal -
        // ----------

	// To be updated - no T2tt, T2bb Spring15 samples yet (update later)!
        addSample("Signal_T1tttt_mGluino1200_mLSP800", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1200_mLSP-800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.0856418, lumi, 147194, 1.0,  kRed);
        addSample("Signal_T1tttt_mGluino1500_mLSP100", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.0141903, lumi, 103140, 1.0,  kRed);
//        addSample("Signal_T5tttt_mGluino1300_mStop300_mChi280", fDir_ + sigMCloc + "", "stopTreeMaker/AUX", 0.0460525, lumi, 44011,  1.0,  kRed);
//        addSample("Signal_T5tttt_mGluino1300_mStop300_mCh285", fDir_ + sigMCloc + "", "stopTreeMaker/AUX", 0.0460525, lumi, 43818,  1.0,  kRed);
//        addSample("Signal_T1bbbb_mGluino1000_mLSP900", fDir_ + sigMCloc + "T1bbbb_2J_mGl-1000_mLSP-900_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.325388,  lumi, 97134,  1.0,  kRed);
//        addSample("Signal_T1bbbb_mGluino1500_mLSP100", fDir_ + sigMCloc + "T1bbbb_2J_mGl-1500_mLSP-100_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.0141903, lumi, 105149, 1.0,  kRed);
//        addSample("Signal_T2tt_mStop425_mLSP325", fDir_ + sigMCloc + "T2tt_2J_mStop-425_mLSP-325_madgraph-tauola.txt", "stopTreeMaker/AUX", 1.31169,   lumi, 1039030,1.0,  kRed);
        addSample("Signal_T2tt_mStop500_mLSP325", fDir_ + sigMCloc + "SMS-T2tt_mStop-500_mLSP-325_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.51848, lumi, 388207, 1.0,  kRed);
//        addSample("Signal_T2tt_mStop650_mLSP325", fDir_ + sigMCloc + "T2tt_2J_mStop-650_mLSP-325_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.107045,  lumi, 105672, 1.0,  kRed);
        addSample("Signal_T2tt_mStop850_mLSP100", fDir_ + sigMCloc + "SMS-T2tt_mStop-850_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.0189612, lumi, 240685, 1.0,  kRed);
//        addSample("Signal_T2bb_mSbottom600_mLSP580", fDir_ + sigMCloc + "T2bb_2J_mStop-600_mLSP-580_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.174599,  lumi, 107316, 1.0,  kRed);
//        addSample("Signal_T2bb_mSbottom900_mLSP100", fDir_ + sigMCloc + "T2bb_2J_mStop-900_mLSP-100_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.0128895, lumi, 102661, 1.0,  kRed);    
//        addSample("Signal_TTDMDMJets_M600GeV", fDir_ + sigMCloc + "TTDMDMJets_M600GeV.txt", "stopTreeMaker/AUX", 0.1038, lumi, 126547, 1.0,  kRed);    
//        addSample("Signal_TTDMDMJets_M1000GeV", fDir_ + sigMCloc + "TTDMDMJets_M1000GeV.txt", "stopTreeMaker/AUX", 0.01585, lumi, 121817, 1.0,  kRed);    
// Fastsim signal scans
        addSample("SMS-T2tt_mStop-100-125_mLSP-1to50", fDir_ + sigMCloc + "SMS-T2tt_mStop-100-125_mLSP-1to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-150-175_mLSP-1to100", fDir_ + sigMCloc + "SMS-T2tt_mStop-150-175_mLSP-1to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-200_mLSP-1to125", fDir_ + sigMCloc + "SMS-T2tt_mStop-200_mLSP-1to125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-225_mLSP-25to150", fDir_ + sigMCloc + "SMS-T2tt_mStop-225_mLSP-25to150_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-250_mLSP-1to175", fDir_ + sigMCloc + "SMS-T2tt_mStop-250_mLSP-1to175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-275_mLSP-75to200", fDir_ + sigMCloc + "SMS-T2tt_mStop-275_mLSP-75to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-300to375_mLSP-1to300", fDir_ + sigMCloc + "SMS-T2tt_mStop-300to375_mLSP-1to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-400to475_mLSP-1to400", fDir_ + sigMCloc + "SMS-T2tt_mStop-400to475_mLSP-1to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-500-525-550_mLSP-1to425-325to450-1to475", fDir_ + sigMCloc + "SMS-T2tt_mStop-500-525-550_mLSP-1to425-325to450-1to475_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-600-950_mLSP-1to450", fDir_ + sigMCloc + "SMS-T2tt_mStop-600-950_mLSP-1to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);

        addSample("SMS-T2tb_mStop-200to325_0to150", fDir_ + sigMCloc + "SMS-T2tb_mStop-200to325_0to150_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tb_mStop-200to625_50to475", fDir_ + sigMCloc + "SMS-T2tb_mStop-200to625_50to475_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tb_mStop-350to400_0to225", fDir_ + sigMCloc + "SMS-T2tb_mStop-350to400_0to225_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tb_mStop-425to600_0to425", fDir_ + sigMCloc + "SMS-T2tb_mStop-425to600_0to425_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tb_mStop-625to850_0to450", fDir_ + sigMCloc + "SMS-T2tb_mStop-625to850_0to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tb_mStop-875to1125_0to475", fDir_ + sigMCloc + "SMS-T2tb_mStop-875to1125_0to475_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);

        addSample("SMS-T1tttt_mGluino-600to625_mLSP-250to375", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-600to625_mLSP-250to375_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-600_mLSP-1to225", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-600_mLSP-1to225_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-600_mLSP-250to325", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-600_mLSP-250to325_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-625to650_mLSP-200to400", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-625to650_mLSP-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-625_mLSP-275to375", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-625_mLSP-275to375_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-650to675_mLSP-250to425", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-650to675_mLSP-250to425_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-675_mLSP-325to450", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-675_mLSP-325to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-700to750_mLSP-200to500", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-700to750_mLSP-200to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-700_mLSP-1to450", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-700_mLSP-1to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-750to775_mLSP-350to525", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-750to775_mLSP-350to525_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-775_mLSP-475to550", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-775_mLSP-475to550_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-800to825_mLSP-1to575", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-800to825_mLSP-1to575_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-825to850_mLSP-200to600", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-825to850_mLSP-200to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-850to875_mLSP-450to625", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-850to875_mLSP-450to625_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-875to900_mLSP-1to650", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-875to900_mLSP-1to650_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-900to950_mLSP-200to700", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-900to950_mLSP-200to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-950to975_mLSP-350to725", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-950to975_mLSP-350to725_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-975_mLSP-600to750", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-975_mLSP-600to750_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1000to1050_mLSP-1to800", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1000to1050_mLSP-1to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1000_mLSP-1to700", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1000_mLSP-1to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1050to1075_mLSP-650to850", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1050to1075_mLSP-650to850_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1050_mLSP-50to775", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1050_mLSP-50to775_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1100to1125_mLSP-700to900", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1100to1125_mLSP-700to900_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1100_mLSP-1to775", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1100_mLSP-1to775_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1150to1175_mLSP-750to925", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1150to1175_mLSP-750to925_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1150_mLSP-1to800", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1150_mLSP-1to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1175_mLSP-950", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1175_mLSP-950_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1200to1225_mLSP-800to1000", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1200to1225_mLSP-800to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1200_mLSP-1to825", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1200_mLSP-1to825_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1225to1250_mLSP-1to1025", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1225to1250_mLSP-1to1025_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1250to1275_mLSP-700to1050", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1250to1275_mLSP-700to1050_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1275_mLSP-900to975", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1275_mLSP-900to975_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1300to1325_mLSP-700to1100", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1300to1325_mLSP-700to1100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1300_mLSP-1to1075", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1300_mLSP-1to1075_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1325to1350_mLSP-1to1125", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1325to1350_mLSP-1to1125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1350to1375_mLSP-50to1025", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1350to1375_mLSP-50to1025_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1375_mLSP-950to1150", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1375_mLSP-950to1150_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1400to1425_mLSP-50to1100", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1400to1425_mLSP-50to1100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1400_mLSP-1to1175", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1400_mLSP-1to1175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1425to1450_mLSP-1to1200", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1425to1450_mLSP-1to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1450to1475_mLSP-50to1075", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1450to1475_mLSP-50to1075_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1475to1500_mLSP-1to1250", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1475to1500_mLSP-1to1250_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1500to1525_mLSP-50to1125", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1500to1525_mLSP-50to1125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1525to1550_mLSP-1to1300", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1525to1550_mLSP-1to1300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1550to1575_mLSP-500to1175", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1550to1575_mLSP-500to1175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1600to1650_mLSP-1to1350", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1600to1650_mLSP-1to1350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1650to1700_mLSP-1to1400", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1650to1700_mLSP-1to1400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1700to1750_mLSP-1to1450", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1700to1750_mLSP-1to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1750_mLSP-50to1450", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1750_mLSP-50to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1800to1850_mLSP-1to1450", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1800to1850_mLSP-1to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1850to1900_mLSP-1to1450", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1850to1900_mLSP-1to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1900to1950_mLSP-0to1450", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1900to1950_mLSP-0to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1950_mLSP-700to950", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1950_mLSP-700to950_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);

    }

    SampleCollection::SampleCollection(SampleSet& samples)
    {
        //Define sets of samples for later use
        addSampleSet(samples, "TTbar", {"TTbarInc"});
        addSampleSet(samples, "TTbarSingleLep", {"TTbarSingleLepT", "TTbarSingleLepTbar"});
        addSampleSet(samples, "TTbarDiLep", {"TTbarDiLep"});
        addSampleSet(samples, "TTbarHT", {"TTbar_HT-600to800", "TTbar_HT-800to1200", "TTbar_HT-1200to2500", "TTbar_HT-2500toInf"});
        addSampleSet(samples, "TTbarNoHad", {"TTbarSingleLepT", "TTbarSingleLepTbar", "TTbarDiLep"});

        // Only all had. part of TTbarInc
        addSampleSet(samples, "TTbarAll", {"TTbarInc", "TTbarSingleLepT", "TTbarSingleLepTbar", "TTbarDiLep"});

        // Only all had. part of TTbarInc & HT cuts on inclusive samples
        addSampleSet(samples, "TTbarExt", {"TTbarInc", "TTbarSingleLepT", "TTbarSingleLepTbar", "TTbarDiLep", "TTbar_HT-600to800", "TTbar_HT-800to1200", "TTbar_HT-1200to2500", "TTbar_HT-2500toInf"});

//        addSampleSet(samples, "WJetsToLNu_LESS", {"WJetsToLNu_HT_600toInf", "WJetsToLNu_HT_400to600", "WJetsToLNu_HT_200to400", "WJetsToLNu_HT_100to200"});
        addSampleSet(samples, "WJetsToLNu", {"WJetsToLNu_HT_2500toInf", "WJetsToLNu_HT_1200to2500", "WJetsToLNu_HT_800to1200", "WJetsToLNu_HT_600to800", "WJetsToLNu_HT_400to600", "WJetsToLNu_HT_200to400", "WJetsToLNu_HT_100to200"});

        addSampleSet(samples, "ZJetsToNuNu", {"ZJetsToNuNu_HT_600toInf", "ZJetsToNuNu_HT_400to600", "ZJetsToNuNu_HT_200to400", "ZJetsToNuNu_HT_100to200"});
        addSampleSet(samples, "DYJetsToLL", {"DYJetsToLL_HT_600toInf", "DYJetsToLL_HT_400to600", "DYJetsToLL_HT_200to400", "DYJetsToLL_HT_100to200"});
        addSampleSet(samples, "IncDY", {"DYJetsToLL_Inc"});

        addSampleSet(samples, "QCD", {"QCD_HT2000toInf", "QCD_HT1500to2000", "QCD_HT1000to1500", "QCD_HT700to1000", "QCD_HT500to700", "QCD_HT300to500", "QCD_HT200to300", "QCD_HT100to200"});

        addSampleSet(samples, "tW", {"tW_top", "tW_antitop"});

        addSampleSet(samples, "TTZ", {"TTZToLLNuNu", "TTZToQQ"});
        addSampleSet(samples, "TTW", {"TTWJetsToLNu", "TTWJetsToQQ"});

        addSampleSet(samples, "TTG", {"TTGJets"});

//	addSampleSet(samples, "ttHJetTobb", {"ttHJetTobb"});
//	addSampleSet(samples, "ttHJetToNonbb", {"ttHJetToNonbb"});
	addSampleSet(samples, "ttH", {"ttHJetTobb", "ttHJetToNonbb"});

        addSampleSet(samples, "WWZ", {"WWZ"});
        addSampleSet(samples, "WZZ", {"WZZ"});
        addSampleSet(samples, "ZZZ", {"ZZZ"});

        addSampleSet(samples, "Diboson", {"WW", "WZ", "ZZ"});

        addSampleSet(samples, "Triboson", {"WWZ", "WZZ", "ZZZ"});

	addSampleSet(samples, "Rare", {"TTWJetsToLNu", "TTWJetsToQQ", "TTGJets", "WWZ", "WZZ", "ZZZ", "ttHJetTobb", "ttHJetToNonbb"});

        addSampleSet(samples, "Data_SingleMuon", {"Data_SingleMuon_2015C", "Data_SingleMuon_2015D_05Oct2015", "Data_SingleMuon_2015D_PromptReco"});

        addSampleSet(samples, "Data_SingleElectron", {"Data_SingleElectron_2015C", "Data_SingleElectron_2015D_05Oct2015", "Data_SingleElectron_2015D_PromptReco"});

        addSampleSet(samples, "Data_DoubleMuon", {"Data_DoubleMuon_2015C", "Data_DoubleMuon_2015D_05Oct2015", "Data_DoubleMuon_2015D_PromptReco"});

        addSampleSet(samples, "Data_DoubleEG", {"Data_DoubleEG_2015C", "Data_DoubleEG_2015D_05Oct2015", "Data_DoubleEG_2015D_PromptReco"});

        addSampleSet(samples, "Data_HTMHT", {"Data_HTMHT_2015C", "Data_HTMHT_2015D_05Oct2015", "Data_HTMHT_2015D_PromptReco"});

        addSampleSet(samples, "Signal_T1tttt_mGluino1200_mLSP800", {"Signal_T1tttt_mGluino1200_mLSP800"});
        addSampleSet(samples, "Signal_T1tttt_mGluino1500_mLSP100", {"Signal_T1tttt_mGluino1500_mLSP100"});
        addSampleSet(samples, "Signal_T2tt_mStop500_mLSP325", {"Signal_T2tt_mStop500_mLSP325"});
        addSampleSet(samples, "Signal_T2tt_mStop850_mLSP100", {"Signal_T2tt_mStop850_mLSP100"});

        addSampleSet(samples, "Signal_fastsim_T2tt_scan", {"SMS-T2tt_mStop-100-125_mLSP-1to50", "SMS-T2tt_mStop-150-175_mLSP-1to100", "SMS-T2tt_mStop-200_mLSP-1to125", "SMS-T2tt_mStop-225_mLSP-25to150", "SMS-T2tt_mStop-250_mLSP-1to175", "SMS-T2tt_mStop-275_mLSP-75to200", "SMS-T2tt_mStop-300to375_mLSP-1to300", "SMS-T2tt_mStop-400to475_mLSP-1to400", "SMS-T2tt_mStop-500-525-550_mLSP-1to425-325to450-1to475", "SMS-T2tt_mStop-600-950_mLSP-1to450"});

        addSampleSet(samples, "Signal_fastsim_T2tb_scan", {"SMS-T2tb_mStop-350to400_0to225", "SMS-T2tb_mStop-425to600_0to425", "SMS-T2tb_mStop-625to850_0to450", "SMS-T2tb_mStop-875to1125_0to475"});
        addSampleSet(samples, "Signal_fastsim_T1tttt_scan", {"SMS-T1tttt_mGluino-600to625_mLSP-250to375", "SMS-T1tttt_mGluino-600_mLSP-1to225", "SMS-T1tttt_mGluino-600_mLSP-250to325", "SMS-T1tttt_mGluino-625to650_mLSP-200to400", "SMS-T1tttt_mGluino-625_mLSP-275to375", "SMS-T1tttt_mGluino-650to675_mLSP-250to425", "SMS-T1tttt_mGluino-675_mLSP-325to450", "SMS-T1tttt_mGluino-700to750_mLSP-200to500", "SMS-T1tttt_mGluino-700_mLSP-1to450", "SMS-T1tttt_mGluino-750to775_mLSP-350to525", "SMS-T1tttt_mGluino-775_mLSP-475to550", "SMS-T1tttt_mGluino-800to825_mLSP-1to575", "SMS-T1tttt_mGluino-825to850_mLSP-200to600", "SMS-T1tttt_mGluino-850to875_mLSP-450to625", "SMS-T1tttt_mGluino-875to900_mLSP-1to650", "SMS-T1tttt_mGluino-900to950_mLSP-200to700", "SMS-T1tttt_mGluino-950to975_mLSP-350to725", "SMS-T1tttt_mGluino-975_mLSP-600to750", "SMS-T1tttt_mGluino-1000to1050_mLSP-1to800", "SMS-T1tttt_mGluino-1000_mLSP-1to700", "SMS-T1tttt_mGluino-1050to1075_mLSP-650to850", "SMS-T1tttt_mGluino-1050_mLSP-50to775", "SMS-T1tttt_mGluino-1100to1125_mLSP-700to900", "SMS-T1tttt_mGluino-1100_mLSP-1to775", "SMS-T1tttt_mGluino-1150to1175_mLSP-750to925", "SMS-T1tttt_mGluino-1150_mLSP-1to800", "SMS-T1tttt_mGluino-1175_mLSP-950", "SMS-T1tttt_mGluino-1200to1225_mLSP-800to1000", "SMS-T1tttt_mGluino-1200_mLSP-1to825", "SMS-T1tttt_mGluino-1225to1250_mLSP-1to1025", "SMS-T1tttt_mGluino-1250to1275_mLSP-700to1050", "SMS-T1tttt_mGluino-1275_mLSP-900to975", "SMS-T1tttt_mGluino-1300to1325_mLSP-700to1100", "SMS-T1tttt_mGluino-1300_mLSP-1to1075", "SMS-T1tttt_mGluino-1325to1350_mLSP-1to1125", "SMS-T1tttt_mGluino-1350to1375_mLSP-50to1025", "SMS-T1tttt_mGluino-1375_mLSP-950to1150", "SMS-T1tttt_mGluino-1400to1425_mLSP-50to1100", "SMS-T1tttt_mGluino-1400_mLSP-1to1175", "SMS-T1tttt_mGluino-1425to1450_mLSP-1to1200", "SMS-T1tttt_mGluino-1450to1475_mLSP-50to1075", "SMS-T1tttt_mGluino-1475to1500_mLSP-1to1250", "SMS-T1tttt_mGluino-1500to1525_mLSP-50to1125", "SMS-T1tttt_mGluino-1525to1550_mLSP-1to1300", "SMS-T1tttt_mGluino-1550to1575_mLSP-500to1175", "SMS-T1tttt_mGluino-1600to1650_mLSP-1to1350", "SMS-T1tttt_mGluino-1650to1700_mLSP-1to1400", "SMS-T1tttt_mGluino-1700to1750_mLSP-1to1450", "SMS-T1tttt_mGluino-1750_mLSP-50to1450", "SMS-T1tttt_mGluino-1800to1850_mLSP-1to1450", "SMS-T1tttt_mGluino-1850to1900_mLSP-1to1450", "SMS-T1tttt_mGluino-1900to1950_mLSP-0to1450", "SMS-T1tttt_mGluino-1950_mLSP-700to950"});

    }

    void SampleCollection::addSampleSet(SampleSet& samples, std::string name, std::vector<std::string> vss)
    {
        if(vss.size() > 1)
        {
            for(std::string& sn : vss)
            {
                if(sn.compare(name) == 0)
                {
                    std::cout << "You have named a sampleCollection the same as one of its member sampleSets, but it has more than one sampleSet!!!! This is bad!!!  Stop!!! Stop now!!!  This collection will be skipped until it is properly named." << std::endl;
                    return;
                }
            }
        }

        auto& map = samples.getMap();

        for(std::string& sn : vss)
        {
            map[sn].addCollection(name);
            sampleSet_[name].push_back(samples[sn]);
            nameVec_[name].push_back(sn);
            totalLumiMap_[name] += samples[sn].lumi;
        }
    }

    std::vector<std::string>& SampleCollection::getSampleLabels(std::string name)
    {
        return nameVec_[name];
    }

    bool operator< (const FileSummary& lhs, const FileSummary& rhs)
    {
        return lhs.filePath < rhs.filePath || lhs.treePath < rhs.treePath;
    }

    bool operator== (const FileSummary& lhs, const FileSummary& rhs)
    {
        return lhs.filePath == rhs.filePath && lhs.treePath == rhs.treePath && lhs.xsec == rhs.xsec && lhs.lumi == rhs.lumi && lhs.kfactor == rhs.kfactor && lhs.nEvts == rhs.nEvts;
    }

    bool operator!= (const FileSummary& lhs, const FileSummary& rhs)
    {
        return !(lhs == rhs);
    }
}

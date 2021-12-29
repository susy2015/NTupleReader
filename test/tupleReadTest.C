#include "NTupleReader/include/NTupleReader.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>

class GetScaleWeights
{
private:
    NTupleReader trSupp_;

    void getScaleWeights(NTupleReader& tr)
    {
        if(trSupp_.getNextEvent() &&
           tr.getVar<unsigned long long>("event") == trSupp_.getVar<unsigned long long>("event") &&
           tr.getVar<float>("GenMET_pt")          == trSupp_.getVar<float>("GenMET_pt") )
        {
            tr.registerDerivedVec("LHEScaleWeight", new std::vector(trSupp_.getVec<float>("LHEScaleWeight")));
        }
        //else
        //{
        //    std::cout << tr.getVar<unsigned long long>("event") << "\t" << trSupp_.getVar<unsigned long long>("event") << "\t" << tr.getVar<float>("GenMET_pt") << "\t" << trSupp_.getVar<float>("met") << std::endl;
        //    THROW_SATEXCEPTION("ERROR: Event mismatch between master and supplamental file!!!!");
        //}
    }

public:
    GetScaleWeights(TChain *ch) : trSupp_(ch, {"event"}) {}
    GetScaleWeights(GetScaleWeights&) = delete;
    GetScaleWeights(GetScaleWeights&& gsw) : trSupp_(std::move(gsw.trSupp_)) {}

    void operator()(NTupleReader& tr) { getScaleWeights(tr); }
};

int main()
{
    char baseFile[]         = "testFile.root";
    char supplamantalFile[] = "testFile.root";

    TChain *chBase = new TChain("Events");
    chBase->Add(baseFile);

    TChain *chSupp = new TChain("Events");
    chSupp->Add(supplamantalFile);

    try
    {
        NTupleReader tr(chBase, {"nJet"});

        //For NTupleReader users you simply need to register the class with NTupleReader 
        tr.emplaceModule<GetScaleWeights>(chSupp);

        while(tr.getNextEvent())
        {
            const auto& LHEScaleWeight = tr.getVec<float>("LHEScaleWeight");

            std::cout << LHEScaleWeight.size() << std::endl;
        }
    }
    catch(const SATException& e)
    {
        e.print();
    }

    return 0;
}


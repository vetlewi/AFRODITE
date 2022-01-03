#include "AnalyseSims.h"

#include <TH1.h>
#include <TH2.h>
#include <iostream>


AnalyseSims::AnalyseSims(TTree *chain, const size_t &last_bin)
    : fChain( chain )
    , clover{CLOVER_detector_data(0), CLOVER_detector_data(1), CLOVER_detector_data(2), CLOVER_detector_data(3),
             CLOVER_detector_data(4), CLOVER_detector_data(5), CLOVER_detector_data(6), CLOVER_detector_data(7)}
    , ocl_labr{LABR_detector_data("OCLLABR_Energy", 0), LABR_detector_data("OCLLABR_Energy", 1)}
    , fta_labr{LABR_detector_data("FTALABR_Energy", 0), LABR_detector_data("FTALABR_Energy", 1), LABR_detector_data("FTALABR_Energy", 2),
               LABR_detector_data("FTALABR_Energy", 3), LABR_detector_data("FTALABR_Energy", 4), LABR_detector_data("FTALABR_Energy", 5)}
    , clover_tot( new TH1F("clover_tot", "CLOVER total", last_bin, 0, last_bin) )
    , ocl_tot( new TH1F("ocl_tot", "OCL total", last_bin, 0, last_bin) )
    , fta_tot( new TH1F("fta_tot", "FTA total", last_bin, 0, last_bin) )
    , clover_indv( new TH2F("clover_indv", "CLOVER by detector", last_bin, 0, last_bin, NUM_CLOVER+1, 1, NUM_CLOVER+1) )
    , ocl_indv( new TH2F("ocl_indv", "OCL by detector", last_bin, 0, last_bin, NUM_OCL+1, 1, NUM_OCL+1) )
    , fta_indv( new TH2F("fta_indv", "FTA by detector", last_bin, 0, last_bin, NUM_FTA+1, 1, NUM_FTA+1) )
{
    // Initialize
    for ( auto &c : clover ){
        c.SetBranch(fChain);
    }

    for ( auto &l : ocl_labr ){
        l.SetBranch(fChain);
    }

    for ( auto &l : fta_labr ){
        l.SetBranch(fChain);
    }
}

AnalyseSims::~AnalyseSims()
{
    delete clover_tot;
    delete ocl_tot;
    delete fta_tot;

    delete clover_indv;
    delete ocl_indv;
    delete fta_indv;
}

size_t AnalyseSims::loop()
{
    fChain->SetBranchStatus("*",1);
    int num_entries = fChain->GetEntries();

    Long64_t nbytes=0, nb=0;

    for ( int i = 0 ; i < num_entries ; ++i ){
        nbytes += fChain->GetEntry(i);

        for ( auto &c : clover ){
            c.FillSpec(clover_tot, clover_indv);
        }

        for ( auto &l : ocl_labr ){
            l.FillSpec(ocl_tot, ocl_indv);
        }

        for ( auto &l : fta_labr ){
            l.FillSpec(fta_tot, fta_indv);
        }
    }
    std::cout << "Read " << nbytes / 1e6 << " mb" << std::endl;
    return num_entries;
}

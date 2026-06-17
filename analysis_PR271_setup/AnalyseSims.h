#ifndef ANALYSE_SIMS
#define ANALYSE_SIMS

#include <string>
#include <sstream>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>

#include "indicators.hpp"

#define NUM_CLOVER 8
#define NUM_OCL 6
#define NUM_FTA 6

struct CLOVER_crystal_data
{
    Double_t energy;

    CLOVER_crystal_data(const size_t &d_num, const char &c_num) : num( d_num ), cnum( c_num ){}

    void SetBranch(TTree *chain)
    {
        std::stringstream ss;
        ss << "CLOVER_Energy" << num << "_" << cnum;
        std::string name = ss.str();
        chain->SetBranchAddress(name.c_str(), &energy);
    }

private:
    size_t num;
    char cnum;

};

struct CLOVER_detector_data
{
    CLOVER_crystal_data crystalA;
    CLOVER_crystal_data crystalB;
    CLOVER_crystal_data crystalC;
    CLOVER_crystal_data crystalD;
    
    Double_t BGO_energy;

    CLOVER_detector_data(const size_t &dnum)
        : num( dnum )
        , crystalA( dnum, 'A' )
        , crystalB( dnum, 'B' )
        , crystalC( dnum, 'C' )
        , crystalD( dnum, 'D' )
    {}

    void SetBranch(TTree *chain)
    {
        crystalA.SetBranch(chain);
        crystalB.SetBranch(chain);
        crystalC.SetBranch(chain);
        crystalD.SetBranch(chain);
        
        std::stringstream ss;
        ss << "BGO_Energy" << num;
        std::string name = ss.str();
        chain->SetBranchAddress(name.c_str(), &BGO_energy);
    }

    void FillSpec(TH1 *tot, TH2 *indv)
    {
        // We will only fill the total if the BGO energy is less than 1 keV
        if ( BGO_energy > 1.0 )
            return;

        auto etot = crystalA.energy + crystalB.energy + crystalC.energy + crystalD.energy;
        if (etot > 0){
            tot->Fill(etot);
            indv->Fill(etot, num+1);
        }
    }

private:
    size_t num;

};

struct LABR_detector_data
{
    Double_t energy;
    TBranch *b_energy;

    LABR_detector_data(const std::string &b_name, const size_t &dnum) : base_name( b_name ), num( dnum ){}

    void SetBranch(TTree *chain)
    {
        std::stringstream ss;
        ss << base_name << num;
        std::string name = ss.str();
        chain->SetBranchAddress(name.c_str(), &energy);
    }

    void FillSpec(TH1 *tot, TH2 *indv)
    {
        if ( energy > 0 ){
            tot->Fill(energy);
            indv->Fill(energy, num+1);
        }
    }

private:
    std::string base_name;
    size_t num;
};

class AnalyseSims
{

private:

    TTree *fChain;  // Pointer to the analyzed TTree of TChain
    Int_t fCurrent; // Current Tree number in TChain

    CLOVER_detector_data clover[NUM_CLOVER];
    LABR_detector_data ocl_labr[NUM_OCL];
    LABR_detector_data fta_labr[NUM_FTA];

    TH1 *clover_tot, *ocl_tot, *fta_tot;
    TH2 *clover_indv, *ocl_indv, *fta_indv;

public:

    AnalyseSims(TTree *chain, const size_t &last_bin);
    ~AnalyseSims();
    size_t loop();
    size_t loop(indicators::DynamicProgress<indicators::ProgressBar> &bars, const int &n);

    TH1 *GetCloverTot(){ return clover_tot; }
    TH1 *GetOCLTot(){ return ocl_tot; }
    TH1 *GetFTATot(){ return fta_tot; }

    TH2 *GetCloverIndv(){ return clover_indv; }
    TH2 *GetOCLIndv(){ return ocl_indv; }
    TH2 *GetFTAIndv(){ return fta_indv; }

};


#endif // ANALYSE_SIMS
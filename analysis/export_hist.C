

#include "AnalyseSims.C"
#ifdef MAKECINT
    #pragma link C++ defined_in “AnalyseSims.h”;
    #pragma link C++ defined_in “AnalyseSims.C”;
#endif


#include "th1_to_mama.C"
#include "th22mama.C"

#include "indicators.hpp"


void export_hist()
{
    std::vector<string> fnames;
    string fout_name = "Peaks.dat";
    system("xterm -e 'mkdir mama_spectra'"); // create dir for mama spectra, if not already existend
    string outdir = "mama_spectra";

    string fname_tmp;
    system("xterm -e 'find /Volumes/PR271/result/*.root > tmp.txt'");
    ifstream tmpfile("tmp.txt", ios::in);
    while(tmpfile>>fname_tmp){
        fnames.push_back(fname_tmp); //adding data in to the vector
    }

    // Setup the progress bars
    indicators::ProgressBar bar{indicators::option::BarWidth{50}, indicators::option::ForegroundColor{indicators::Color::white},
                                indicators::option::ShowElapsedTime{true}, indicators::option::ShowRemainingTime{true},
                                indicators::option::MaxProgress{fnames.size()}};
    indicators::ProgressBar bar_file{indicators::option::BarWidth{50}, indicators::option::ForegroundColor{indicators::Color::white},
                                indicators::option::ShowElapsedTime{true}, indicators::option::ShowRemainingTime{true}};
    indicators::DynamicProgress<indicators::ProgressBar> bars(bar, bar_file);

    int i = 0;
    for ( auto &fname : fnames ){

        //std::cout << "Working on '" << fname << "'" << std::endl;
        int energy_keV = std::stoi(fname.substr(fname.find('_')+1, fname.rfind('.')-fname.find('_')-4));

        TFile file_root(fname.c_str(), "READ");
        TTree *DataTreeSim;
        file_root.GetObject("DataTreeSim",DataTreeSim);
        AnalyseSims t(DataTreeSim, 22000);
        
        bars[1].set_option(indicators::option::PrefixText{"Processing file '"+fname+"'"});
        auto entries = t.loop(bars);

        std::string outname = outdir + "/" + "energy_clover_" + std::to_string(energy_keV) + "keV_";
        outname += std::to_string(entries) + "entries.m";
        th1_to_mama(t.GetCloverTot(), outname.c_str());

        outname = outdir + "/" + "energy_ocl_" + std::to_string(energy_keV) + "keV_";
        outname += std::to_string(entries) + "entries.m";
        th1_to_mama(t.GetOCLTot(), outname.c_str());

        outname = outdir + "/" + "energy_fta_" + std::to_string(energy_keV) + "keV_";
        outname += std::to_string(entries) + "entries.m";
        th1_to_mama(t.GetFTATot(), outname.c_str());

        bars[0].set_option(indicators::option::PostfixText{std::to_string(++i) + "/" + std::to_string(fnames.size())});
        bars[0].tick();
    }
    bars[0].mark_as_completed();
    bars[1].mark_as_completed();
}
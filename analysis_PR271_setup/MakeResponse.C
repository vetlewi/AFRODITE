
#include <vector>
#include <string>

#include "AnalyseSims.C"
#include "SpectraAnalyzer.C"
#ifdef MAKECINT
    #pragma link C++ defined_in "AnalyseSims.h";
    #pragma link C++ defined_in "AnalyseSims.C";
    #pragma link C++ defined_in "SpectraAnalyzer.h";
    #pragma link C++ defined_in "SpectraAnalyzer.C";
#endif

#include "indicators.hpp"

double cloverFWHM(double e)
{
    return std::sqrt( 3.56365670e+00 + 9.93722358e-04*e + 1.36227220e-06*e*e );
}

double oclFWHM(double e)
{
    return std::sqrt( 60.6499 + 0.458252*e + 0.000265552*e*e );
}

double ftaFWHM(double e)
{
    return std::sqrt( 60.6499 + 0.458252*e + 0.000265552*e*e );
}

void MakeResponse()
{
    //ROOT::EnableImplicitMT();
    std::vector<string> fnames;

    system("xterm -e 'mkdir clover_response'"); // create dir for mama spectra, if not already existend
    system("xterm -e 'mkdir ocl_response'"); // create dir for mama spectra, if not already existend
    system("xterm -e 'mkdir fta_response'"); // create dir for mama spectra, if not already existend

    system("xterm -e 'mkdir ocl45_response'");
    system("xterm -e 'mkdir ocl90_response'");
    system("xterm -e 'mkdir ocl135_response'");

    string fname_tmp;
    system("xterm -e 'find /Volumes/PR271/afrodite6LaBr3/*.root > tmp.txt'");
    ifstream tmpfile("tmp.txt", ios::in);
    while(tmpfile>>fname_tmp){
        fnames.push_back(fname_tmp); //adding data in to the vector
    }

    //fnames.push_back("../build/app/result/energy_1332keV.root");
    //fnames.push_back("../build/app/result/energy_1500keV.root");
    //fnames.push_back("../build/app/result/energy_5000keV.root");

    // Setup the progress bars
    indicators::ProgressBar bar{indicators::option::BarWidth{50}, indicators::option::ForegroundColor{indicators::Color::white},
                                indicators::option::ShowElapsedTime{true}, indicators::option::ShowRemainingTime{true},
                                indicators::option::MaxProgress{fnames.size()}};

    indicators::ProgressBar bar_file{indicators::option::BarWidth{50}, indicators::option::ForegroundColor{indicators::Color::white},
                                      indicators::option::ShowElapsedTime{true}, indicators::option::ShowRemainingTime{true}};
    

    indicators::DynamicProgress<indicators::ProgressBar> bars(bar, bar_file);

    //bars.set_option(indicators::option::HideBarWhenComplete{true});

    int i = 0;

    ResponseFunction clover_response(cloverFWHM);
    ResponseFunction ocl_response(oclFWHM);
    ResponseFunction fta_response(ftaFWHM);

    ResponseFunction ocl45_response(oclFWHM);
    ResponseFunction ocl90_response(oclFWHM);
    ResponseFunction ocl135_response(oclFWHM);

    for ( auto &fname : fnames ){

        bars[0].set_option(indicators::option::PostfixText{std::to_string(++i) + "/" + std::to_string(fnames.size())});
        bars[0].tick();

        int energy_keV = std::stoi(fname.substr(fname.find('_')+1, fname.rfind('.')-fname.find('_')-4));
        /*if ( energy_keV < 1300 || energy_keV > 1400 )
            continue;

        if ( energy_keV > 2000 )
            break;*/
        indicators::ProgressBar bar_file{indicators::option::BarWidth{50}, indicators::option::ForegroundColor{indicators::Color::white},
                                         indicators::option::ShowElapsedTime{true}, indicators::option::ShowRemainingTime{true},
                                         indicators::option::PrefixText{"Processing file '"+fname+"'"}};

        TFile file_root(fname.c_str(), "READ");
        TTree *DataTreeSim;
        file_root.GetObject("DataTreeSim",DataTreeSim);
        AnalyseSims t(DataTreeSim, 22000);
        
        bars[1].set_option(indicators::option::PrefixText{"Processing file '"+fname+"'"});
        auto entries = t.loop(bars, 1);

        auto *hist_indiv = t.GetOCLIndv();
        auto *ocl45hist = t.GetOCLIndv()->ProjectionX("ocl45", 1, 4);
        auto *ocl90hist = t.GetOCLIndv()->ProjectionX("ocl90", 5, 5);
        auto *ocl135hist = t.GetOCLIndv()->ProjectionX("ocl135", 6, 6);


        if ( energy_keV == 1332 ){
            clover_response.set_ref(t.GetCloverTot(), energy_keV, entries);
            ocl_response.set_ref(t.GetOCLTot(), energy_keV, entries);
            fta_response.set_ref(t.GetFTATot(), energy_keV, entries);

            ocl45_response.set_ref(ocl45hist, energy_keV, entries);
            ocl90_response.set_ref(ocl90hist, energy_keV, entries);
            ocl135_response.set_ref(ocl135hist, energy_keV, entries);

        } else {
            clover_response.AddResponse(t.GetCloverTot(), energy_keV, entries);
            ocl_response.AddResponse(t.GetOCLTot(), energy_keV, entries);
            fta_response.AddResponse(t.GetFTATot(), energy_keV, entries);

            ocl45_response.AddResponse(ocl45hist, energy_keV, entries);
            ocl90_response.AddResponse(ocl90hist, energy_keV, entries);
            ocl135_response.AddResponse(ocl135hist, energy_keV, entries);
        }
    }

    clover_response.Export("clover_response");
    ocl_response.Export("ocl_response");
    fta_response.Export("fta_response");

    ocl45_response.Export("ocl45_response");
    ocl90_response.Export("ocl90_response");
    ocl135_response.Export("ocl135_response");

    bars[0].mark_as_completed();
}
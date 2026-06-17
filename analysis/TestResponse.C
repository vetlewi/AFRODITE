

#include "SpectraAnalyzer.C"
#ifdef MAKECINT
    #pragma link C++ defined_in "SpectraAnalyzer.h";
    #pragma link C++ defined_in "SpectraAnalyzer.C";
#endif


double cloverFWHM(double e)
{
    return std::sqrt( 3.56365670e+00 + 9.93722358e-04*e + 1.36227220e-06*e*e );
}


void TestResponse()
{
	TFile file_root("res1332.root", "READ");
    TH1 *hist;
    file_root.GetObject("clover_tot",hist);

    ResponseFunction clover_response(cloverFWHM);

    clover_response.AddResponse(hist, 1332, 3000000);
    clover_response.Export("clover_response");
}
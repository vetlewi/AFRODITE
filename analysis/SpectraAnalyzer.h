#ifndef SPECTRA_ANALYZER
#define SPECTRA_ANALYZER

#include <vector>
#include <functional>

#include <TH1.h>

/*! EnergyResponse is a structure that contains the important information obtained
    from analyzing a simulated energy spectra. It decomposes the spectra into
    counts in the different peaks (full energy, single escape, double escape, 511).
 */
struct EnergyResponse {
    TH1D raw;
    TH1D compton;

    double Einc;                // Energy of the incoming gamma-ray
    double nEvents;             // Total number of simulated events
    double nCounts;             // Total number of counts in the spectrum
    double cntFE;               // Counts in the full energy peak
    double cntSE;               // Counts in the single escape peak
    double cntDE;               // Counts in the double escape peak
    double cnt511;              // Counts in the 511 peak
    double cntRest;             // Number of counts after subtracting peaks
    double cntRestNoThres;      // Effective number of counts?

    EnergyResponse(TH1 *inp, const double &Einc, const long long &nEvents, const int &nBgBins = 3, const double &threshold = 0.);
    ~EnergyResponse();

    double EffTot() const { return nCounts/nEvents; }
    double EffFE() const { return cntFE/nEvents; }
    double EffSE() const { return cntSE/nEvents; }
    double EffDE() const { return cntDE/nEvents; }
    double Eff511() const { return cnt511/nEvents; }
};


class ResponseFunction
{

private:
    std::vector<EnergyResponse> responses;
    std::function<double(double)> fwhm;

    EnergyResponse *ref;

    // Optional stuff
    int nBgBins;
    double threshold;

    void pExport(const std::string &path); // Unsafe, can throw

public:

    ResponseFunction(const std::function<double(double)> &_fwhm, const int &bgbins = 3, const double &thres = 0.) : fwhm( _fwhm ), ref( nullptr ), nBgBins( bgbins ), threshold( thres ){}

    void AddResponse(TH1 *inp, const double &Einc, const long long &nEvents)
    {
        responses.push_back(EnergyResponse(inp, Einc, nEvents, nBgBins, threshold));
    }

    void set_ref(TH1 *inp, const double &Einc, const long long &nEvents)
    {
        responses.push_back(EnergyResponse(inp, Einc, nEvents, nBgBins, threshold));
        ref = &responses.back();
    }

    /*!
        Export all spectra needed for mama/ompy to generate response matrices.

        path: Relative/absolute path to the folder where data should be stored. 
        Returns false if failing to write to disk.
     */
    bool Export(const std::string &path) noexcept;

};


#endif // SPECTRA_ANALYZER
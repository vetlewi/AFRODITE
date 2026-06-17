#include "SpectraAnalyzer.h"

#include "th1_to_mama.C"

#include <iostream>
#include <cmath>
#include <numeric>
#include <algorithm>

template<typename T, typename V>
std::pair<double, double> leastSquares(std::vector<T> x,
                                       std::vector<V> y)
{
    double n = double(x.size());
    double sumx = 0;
    double sumxSq = 0;
    double sumy = 0;
    double sumySq = 0;
    double sumxy = 0;
    for ( size_t i = 0 ; i < x.size() ; ++i ){
        sumx += x[i];
        sumy += y[i];
        sumxy += x[i]*y[i];
        sumxSq += x[i]*x[i];
        sumySq += y[i]*y[i];
    }

    double E = sumxy - sumx*sumy/n;
    double D = sumxSq - sumx*sumx/n;

    double m = E/D;
    double c = sumy/n - m*sumx/n;
    return std::make_pair(m, c);
}

double GetBinWoBG(TH1* h1, double xValueofBin)
{
    TAxis *xaxis = h1->GetXaxis();
    TAxis *yaxis = h1->GetYaxis();
    Int_t binx = xaxis->FindBin(xValueofBin);
    double binvalue = h1->GetBinContent(binx);
    double cntsInPeak = binvalue;

    // We look at bins -20 to -5 and 5 to 20
    std::vector<Int_t> binx_vals;
    for ( int i = -20 ; i < -4 ; ++i ){ binx_vals.push_back(binx+i);}
    for ( int i = 5 ; i < 21 ; ++i ){ binx_vals.push_back(binx+i);}

    std::vector<double> biny_vals;
    for ( auto xbin : binx_vals ){ biny_vals.push_back(h1->GetBinContent(xbin)); }

    auto fit_coef = leastSquares(binx_vals, biny_vals);
    //std::cout << fit_coef.first << " " << fit_coef.second << std::endl;
    double BGcntsInPeak = fit_coef.first * xValueofBin + fit_coef.second;
    //std::cout << binx << " " << std::endl;
    //std::cout << cntsInPeak << " " << BGcntsInPeak << std::endl;


    return cntsInPeak - BGcntsInPeak;

}

double GetBinWoBG(TH1* h1, double xValueofBin, int nChannelsBG)
{
    TAxis *xaxis = h1->GetXaxis();
    TAxis *yaxis = h1->GetYaxis();
    Int_t binx = xaxis->FindBin(xValueofBin);
    double binvalue = h1->GetBinContent(binx);


    double cntsInPeak = binvalue;
    cntsInPeak += h1->GetBinContent(binx+1) + h1->GetBinContent(binx-1);

    // get BG
    Int_t min = binx-nChannelsBG;
    Int_t max = binx + nChannelsBG;
    double sum = 0;
    for(int ch=min ; ch<max+1 ; ch++ ){
        sum += h1->GetBinContent(ch);
    }
    
    // a little messy -- this accounts also for thaat counts can come in the bin below/above
    int nchTot = 2*nChannelsBG + 1;
    int nchBGonly = nchTot - 3;
    double cntbg = (sum - cntsInPeak) / nchBGonly; // bg per channel
    double cntWobg = cntsInPeak - cntbg*3.;
    return cntWobg;
}


TH1D CreateMamaSpectrum(TH1* h, double EgFE, int nChannelsBG=3)
{
    TH1D h1 = TH1D(*(TH1D *)h); // create output spectrum as a clone
    std::string name = "hmama" + std::to_string(int(EgFE));
    h1.SetTitle("hmama");
    h1.SetName(name.c_str());

    vector<double> peaks;
    if(EgFE>=1022.) {
        peaks.insert(peaks.end(), {EgFE, EgFE-511., EgFE-2.*511., 511.});
    } else {
        peaks.insert(peaks.end(), {EgFE});
    }

    TAxis *xaxis = h1.GetXaxis();
    TAxis *yaxis = h1.GetYaxis();

    for(auto Eg : peaks){
        Int_t  binx     = xaxis->FindBin(Eg);
        double binvalue = h1.GetBinContent(binx);

        double cntsInPeak = binvalue;
        cntsInPeak += h1.GetBinContent(binx+1) + h1.GetBinContent(binx-1);

        // get BG
        Int_t min = binx - nChannelsBG;
        Int_t max = binx + nChannelsBG;
    
        // cout << "Subtract BG from " <<  h1->GetXaxis()->GetBinCenter(min)
        // << "to " <<  h1->GetXaxis()->GetBinCenter(max) << endl;
    
        double sum = 0;
        for(int ch=min;ch<max+1;ch++ ){
            sum += h1.GetBinContent(ch);
        }
        
        // a little messy -- this accounts also for thaat counts can come in the bin below/above
        int nchTot = 2*nChannelsBG +1;
        int nchBGonly = nchTot -3;
        double cntbg = (sum - cntsInPeak) / nchBGonly; // bg per channel

        // set all counts equal to bg
        for(int ch=min; ch<max+1 ; ch++ ){
            h1.SetBinContent(ch, cntbg);
        }
    }
    return h1;
}


double CountsAboveThreshold(TH1 *inp, const double &threshold)
{
    double sum = 0; 
    for ( int i = inp->GetXaxis()->FindBin(threshold) ; i < inp->GetXaxis()->GetNbins() ; ++i )
        sum += inp->GetBinContent(i);
    return sum;
}


EnergyResponse::EnergyResponse(TH1 *inp, const double &E_inc, const long long &n_Events, const int &nBgBins, const double &threshold)
    : raw( *(TH1D *)inp )
    , compton( CreateMamaSpectrum(&raw, E_inc, nBgBins) )
    , Einc( E_inc )
    , nEvents( n_Events )
    , nCounts( raw.GetEntries() )
    , cntFE( GetBinWoBG(&raw, Einc) )
    , cntSE( ( Einc > 1022. ) ? GetBinWoBG(&raw, Einc-511.) : 0 )
    , cntDE( ( Einc > 1022. ) ? GetBinWoBG(&raw, Einc-2*511.) : 0 )
    , cnt511( ( Einc > 1022. ) ? GetBinWoBG(&raw, 510.5) : 0 )
    , cntRest( CountsAboveThreshold(&raw, threshold) - cntFE - cntSE - cntDE -cnt511 )
    , cntRestNoThres( compton.GetEffectiveEntries() )
{
    /*cntSE = ( cntSE < 0 ) ? 0 : cntSE;
    cntDE = ( cntDE < 0 ) ? 0 : cntDE;
    cnt511 = ( cnt511 < 0 ) ? 0 : cnt511;
    cntRest = CountsAboveThreshold(&raw, threshold) - cntFE - cntSE - cntDE -cnt511;
    cntRestNoThres = compton.GetEffectiveEntries();*/


}

EnergyResponse::~EnergyResponse()
{
    //delete raw; 
    //delete compton;
}

bool operator<(const EnergyResponse &lhs, const EnergyResponse &rhs){ return lhs.Einc < rhs.Einc; }

void ResponseFunction::pExport(const std::string &path)
{
    // Create the resp.dat file

    std::ofstream datout(path+"/resp.dat"), csvout(path+"/resp.csv");
    // write header
    datout << responses.size() << std::endl;
    datout << "# Eg        FWHM_rel      Eff_tot       FE            SE            DE            c511" << std::endl;
    csvout << "Eg,FWHM_rel_norm,Eff_tot,FE,SE,DE,c511,cntRest,cntRestNoThres,nEvents,nCounts" << std::endl;
    // Ensure we get all energies in order.
    std::sort(responses.begin(), responses.end());

    double ref_eff = ( ref ) ? ref->EffTot() : 1.0;
    /*std::cout << ref_eff << std::endl;
    std::cout << "-------------------------------" << std::endl;
    std::cout << "nEvents: " << ref->nEvents << std::endl;
    std::cout << "nCounts: " << ref->nCounts << std::endl;
    std::cout << "cntFE: " << ref->cntFE << std::endl;
    std::cout << "cntSE: " << ref->cntSE << std::endl;
    std::cout << "cntDE: " << ref->cntDE << std::endl;
    std::cout << "cnt511: " << ref->cnt511 << std::endl;
    std::cout << "cntRest: " << ref->cntRest << std::endl;
    std::cout << "cntRestNoThres: " << ref->cntRestNoThres << std::endl;
    std::cout << "-------------------------------" << std::endl;
    ref->raw.Draw("colz");*/
    double fwhm_ref = ( ref ) ? fwhm(ref->Einc) : 1.0;

    for ( auto &response : responses ){
        datout << response.Einc << "\t";
        csvout << response.Einc << ",";
        datout << fwhm(response.Einc)/fwhm_ref << "\t";
        csvout << fwhm(response.Einc)/fwhm_ref << ",";
        datout << response.EffTot() / ref_eff << "\t";
        csvout << response.EffTot() / ref_eff << ",";
        datout << response.cntFE << "\t";
        csvout << response.cntFE << ",";
        datout << response.cntSE << "\t";
        csvout << response.cntSE << ",";
        datout << response.cntDE << "\t";
        csvout << response.cntDE << ",";
        datout << response.cnt511 << std::endl;
        csvout << response.cnt511 << ",";
        csvout << response.cntRest << ",";
        csvout << response.cntRestNoThres << ",";
        csvout << response.nEvents << "," << response.nCounts << std::endl;

        // Write the spectra to file
        auto spec_path = path+"/cmp"+std::to_string(int(response.Einc));
        //std::cout << response.EffTot() << std::endl;
        th1_to_mama(&response.compton, spec_path.c_str());
    }

}

bool ResponseFunction::Export(const std::string &path) noexcept
{
    try {
        pExport(path);
        return true;
    } catch ( const std::exception& e ){
        std::cerr << "Could not export. Got '" << e.what() << "'" << std::endl;
        return false;
    }
}




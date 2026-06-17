

{
    gROOT->ProcessLine(".L AnalyseSims.C");
    gROOT->ProcessLine(".L SpectraAnalyze.C");
    AnalyseSims t(DataTreeSim, 6000);

    auto nC = t.loop();

    GetBinWoBG(t.GetOCLTot(), 5800-511);
}
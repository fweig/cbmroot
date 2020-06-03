using namespace std;
void draw_study_report()
{
    gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
    loadlibs();
    
    
    std::vector<std::string> fileNames, studyNames;
    std::string outputDir = "results_reports_quartz_1.0gev_3mm_dist/";
    
    fileNames.push_back("/Users/slebedev/Development/cbm/data/sim/rich/richprototype/reco.d.quartz3mm.1.2gev.1.root");
    fileNames.push_back("/Users/slebedev/Development/cbm/data/sim/rich/richprototype/reco.d.quartz3mm.1.2gev.2.root");
    fileNames.push_back("/Users/slebedev/Development/cbm/data/sim/rich/richprototype/reco.d.quartz3mm.1.2gev.3.root");
    fileNames.push_back("/Users/slebedev/Development/cbm/data/sim/rich/richprototype/reco.d.quartz3mm.1.2gev.4.root");
    fileNames.push_back("/Users/slebedev/Development/cbm/data/sim/rich/richprototype/reco.d.quartz3mm.1.2gev.5.root");
    fileNames.push_back("/Users/slebedev/Development/cbm/data/sim/rich/richprototype/reco.d.quartz3mm.1.2gev.6.root");
    
    studyNames.push_back("30 mm");
    studyNames.push_back("40 mm");
    studyNames.push_back("50 mm");
    studyNames.push_back("60 mm");
    studyNames.push_back("70 mm");
    studyNames.push_back("80 mm");
    
    if (outputDir != ""){
        gSystem->mkdir(outputDir.c_str(), true); // create directory if it does not exist
    }
    
    CbmRichSmallPrototypeStudyReport* reportQa = new CbmRichSmallPrototypeStudyReport();
    reportQa->Create(fileNames, studyNames, outputDir);
}

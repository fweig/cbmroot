using namespace std;

void hadd() {
    gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
    loadlibs();
    gSystem->Load("libAnalysis");

    string dir = "/hera/cbm/users/adrian/data/jun15_25gev/jpsi_urqmd/";
    string fileArray = ".auau.25gev.centr.";
    Int_t nofFiles = 1000;
    Int_t nofEventsPerFile = 100;
    Int_t fileSizeLimit = 50000;

    TString script = TString(gSystem->Getenv("SCRIPT"));
    if (script == "yes") {
    	fileArray = string(gSystem->Getenv("LMVM_FILE_ARRAY"));
    	dir = string(gSystem->Getenv("LMVM_MAIN_DIR"));
        nofFiles = TString(gSystem->Getenv("LMVM_NOF_FILES")).Atof();
    }


    CbmHaddBase::AddFilesInDir(dir, fileArray, "analysis", nofFiles, fileSizeLimit, nofEventsPerFile);
    CbmHaddBase::AddFilesInDir(dir, fileArray, "litqa", nofFiles, fileSizeLimit, nofEventsPerFile);

}

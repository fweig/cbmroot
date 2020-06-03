using namespace std;

void hadd(string pattern, string outputFile, int fileSizeLimit = 50000, int nofEvents = 1000) {

    vector<string> files = CbmHaddBase::GetGoodFiles(pattern, fileSizeLimit, nofEvents);

    string commandStr = "hadd -T -f " + outputFile;
    for (int i = 0; i < files.size(); i++) {
        commandStr += (" " + files[i]);
    }

    gSystem->Exec(commandStr.c_str());

    cout << "All done." << endl;
}

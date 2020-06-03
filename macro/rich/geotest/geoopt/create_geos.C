#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <string>
#include <iterator>

using namespace std;

int ImportGdmlExportRoot(const string& gdmlFileName, const string& rootFileName);
void CreateSetupFile(const string& setupFilePath, const string& richGeoTag);

void create_geos()
{
    // we need to use latest root version to work with gdml geometry
    system(string("source /usr/local/Cellar/root/6.16.00/bin/thisroot.sh").c_str());

    string templateFileName = "rich_opt_template.gdml";
    ifstream templateStream(templateFileName);
    stringstream buffer;
    buffer << templateStream.rdbuf();
    string templateStr = buffer.str();

    string regexPattern = "_RICH_OPTIMIZATION_PARAMATERS_";

    string outputDir = "output/";

    system((string("rm -rf ") + outputDir).c_str() );
    system((string("mkdir -p ") + outputDir + "/root/").c_str() );
    system((string("mkdir -p ") + outputDir + "/gdml/").c_str() );
    system((string("mkdir -p ") + outputDir + "/setup/").c_str() );

    std::ofstream paramFile(outputDir + "/rich_geoopt_param.txt", std::ofstream::out);

    vector<double> mirrorTiltVec = {10., 12., 15.};
    vector<double> camTiltVec = {0., 3., 6., 9., 12., 15., 18., 21.};
    vector<double> camYVec = {-100., -75., -50., -25., 0., 25., 50., 75., 100.};
    vector<double> camZVec = {-100., -75., -50., -25., 0., 25., 50., 75., 100.};

    for(auto const& mirrorTilt: mirrorTiltVec) {
        int counter = 0;
        for(auto const& camTilt: camTiltVec) {
        for(auto const& camY: camYVec) {
        for(auto const& camZ: camZVec) {
            counter++;
            std::string paramStr = "<variable name=\"RICH_mirror_tilt_angle\" value=\"" + to_string(mirrorTilt) + "\"/>\n" +
                                   "<variable name=\"RICH_camera_tilt_angle\" value=\"" + to_string(camTilt) + "\"/>\n" +
                                   "<variable name=\"RICH_camera_shift_Y\" value=\"" + to_string(camY) + "\"/>\n" +
                                   "<variable name=\"RICH_camera_shift_Z\" value=\"" + to_string(camZ) + "\"/>";

            string templateStrCopy(templateStr);
            size_t findPos = templateStrCopy.find(regexPattern);
            templateStrCopy.erase(findPos, regexPattern.length());
            templateStrCopy.insert(findPos, paramStr);

            paramFile << counter << " " << mirrorTilt << " " << camTilt << " " << camY << " " << camZ << endl;
            string richGeoTag = "mirror" + to_string((int)mirrorTilt) + "_" + to_string(counter);
            string fileName = "rich_" + richGeoTag;
            string gdmlFileName = outputDir + "/gdml/" + fileName + ".gdml";
            string rootFileName = outputDir + "/root/" + fileName + ".geo.root";
            string setupFileName = outputDir + "/setup/setup_" + richGeoTag + ".C";


            cout << counter << " " << gdmlFileName << " " << rootFileName << " " << setupFileName << endl;

            std::ofstream file(gdmlFileName, std::ofstream::out);
            file << templateStrCopy;
            file.close();
            int nofOverlaps = ImportGdmlExportRoot(gdmlFileName, rootFileName);
            CreateSetupFile(setupFileName, richGeoTag);
        } //camZ
        } //camY
        } //camTilt
    } //mirrorTilt
    paramFile.close();
}

int ImportGdmlExportRoot(const string& gdmlFileName, const string& rootFileName)
{
    TGeoManager *gdml = new TGeoManager("gdml", "FAIRGeom");

    TGDMLParse parser;
    // Define your input GDML file HERE
    TGeoVolume* gdmlTop = parser.GDMLReadFile(gdmlFileName.c_str());
    TGeoVolume* rootTop = new TGeoVolumeAssembly("TOP");

    gGeoManager->SetTopVolume(rootTop);

    TGeoRotation* rot = new TGeoRotation ("rot", 0., 0., 0.);
    TGeoCombiTrans* posrot = new TGeoCombiTrans (0., 0., 0., rot);

    rootTop->AddNode (gdmlTop, 1, posrot);

    gGeoManager->CloseGeometry();
    //gGeoManager->CheckOverlaps();

    int nofOverlaps = 0;//gGeoManager->GetListOfOverlaps()->GetEntriesFast();

    TFile* outfile = new TFile(rootFileName.c_str(), "RECREATE");
    rootTop->Write();
    outfile->Close();

    return nofOverlaps;
}

void CreateSetupFile(const string& setupFilePath, const string& richGeoTag)
{
    std::ofstream file(setupFilePath, std::ofstream::out);

    file << "void setup_" +richGeoTag+ "() {" << endl;
    file << "  CbmSetup* setup = CbmSetup::Instance();" << endl;
    file << "  if ( ! setup->IsEmpty() ) {" << endl;
    file << "    std::cout << \"-W- setup_sis100_electron: overwriting existing setup\" << setup->GetTitle() << std::endl;" << endl;
    file << "    setup->Clear();" << endl;
    file << "  }" << endl;
    file << "  setup->SetTitle(\"Setup "+ richGeoTag+ "\");" << endl;
    file << "  setup->SetModule(kMagnet, \"v18a\");" << endl;
    file << "  setup->SetModule(kPipe, \"v16b_1e\");" << endl;
    file << "  setup->SetModule(kSts, \"v19a\");" << endl;
    file << "  setup->SetModule(kRich, \""+ richGeoTag +"\");" << endl;
    file << "  setup->SetField(\"v18a\", 1., 0., 0., 40.);" << endl;
    file << "}" << endl;

    file.close();
}


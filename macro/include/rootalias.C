#include "TGeoManager.h"
#include "TGeoNode.h"

// Functions needed for CTest runtime dependency
// The only way to introduce such a runtime dependency is via a file
// which is created at the end of the macro if everything worked as
// expected
void Generate_CTest_Dependency_File(TString filename)
{
  TString touchCommand = "touch " + filename;
  gSystem->Exec(touchCommand);
}

TString Remove_CTest_Dependency_File(TString outDir, TString macroName, const char* setup = "")
{
  TString _setup(setup);
  TString testDir  = outDir;
  TString testFile = "";

  if (_setup.EqualTo("")) { testFile = macroName + "_ok"; }
  else {
    testFile = macroName + "_" + _setup + "_ok";
  }
  TString depFile = outDir + "/" + testFile;

  if (gSystem->FindFile(testDir, testFile)) {
    TString rmCommand = "rm " + testFile;
    gSystem->Exec(rmCommand);
  }
  depFile.ReplaceAll("//", "/");
  return depFile;
}


// -----   Check for presence of FairMonitor  ----------------------------------
/** @function Has_Fair_Monitor
 ** @brief Checks presence of the FairMonitor class
 ** @author F. Uhlig <f.uhlig@gsi.de>
 ** @date 16.02.2019 (V. Friese)
 ** @value kTRUE if FairMonitor is available.
 **
 ** FairMonitor is part of FairSoft since v-15.11
 **/
Bool_t Has_Fair_Monitor()
{
  // FairMonitor is part of FairSoft since v-15.11
  TString version = gSystem->GetFromPipe("$FAIRROOTPATH/bin/fairroot-config --version");

  // 2018 and later. FairRoot version name is v[year].[major].[minor].
  if (version(1) != '-') return kTRUE;

  // Before 2017. FairRoot version name is v-[major].[minor]
  Size_t start  = version.First("-") + 1;
  TString major = version(start, 2);
  TString minor = version(start + 3, 2);

  if (major.Atoi() >= 15) {
    if (major.Atoi() > 15) { return kTRUE; }
    else {
      if (minor.Atoi() >= 11) { return kTRUE; }  //? minor version >= 11
    }                                            //? major version = 15
  }                                              //? major version >= 15

  return kFALSE;
}
// -----------------------------------------------------------------------------


/**
 * \file determine setup.C
 * \brief Macro determines detector presence in CBM setup.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2008
 */

/**
 * \function CheckDetectorPresence
 * \brief Determines detector presence in TGeo by its name.
 * \param[in] parFile Name of the parameter file with TGeo.
 * \param[in] name Detector name.
 * \return True if detector presents in the TGeo.
 */
Bool_t CheckDetectorPresence(const TString& parFile, const char* name)
{
  cout << "In CheckDetectorPresence for detector " << name << endl;

  TFile* currentfile = gFile;
  TFile* f           = new TFile(parFile);

  // In fairbase trunk the geometry is moved into a separate container,
  // so we have to check which version of paramtere file we have

  if (!gGeoManager) {
    f->Get("FairBaseParSet");
    TGeoManager* geoMan = gGeoManager;
    if (!geoMan) {
      f->Get("FairGeoParSet");
      geoMan = gGeoManager;
      if (!geoMan) {
        std::cout << "Could not find valid GeoManager. Abort now!" << std::endl;
        exit(1);
      }
    }
  }

  cout << "After getting the geomanager " << name << endl;

  TObjArray* nodes = gGeoManager->GetTopNode()->GetNodes();
  for (Int_t iNode = 0; iNode < nodes->GetEntriesFast(); iNode++) {
    TGeoNode* node = (TGeoNode*) nodes->At(iNode);
    cout << "Node: " << node->GetName() << endl;
    if (TString(node->GetName()).Contains(name)) {
      f->Close();
      delete f;
      gFile = currentfile;
      return true;
    }
  }

  if (std::string(name) == "mvd") {
    TGeoNode* node1 = gGeoManager->GetTopVolume()->FindNode("pipevac1_0");
    if (node1) {  // old ascii geometry for pipe
      nodes = node1->GetNodes();
      for (Int_t iNode = 0; iNode < nodes->GetEntriesFast(); iNode++) {
        TGeoNode* node = (TGeoNode*) nodes->At(iNode);
        cout << "Node: " << node->GetName() << endl;
        if (TString(node->GetName()).Contains(name)) {
          f->Close();
          delete f;
          gFile = currentfile;
          return true;
        }
      }
    }
    else {
      // Find Pipe top node
      TObjArray* nodes = gGeoManager->GetTopNode()->GetNodes();
      for (Int_t iNode = 0; iNode < nodes->GetEntriesFast(); iNode++) {
        TGeoNode* node   = (TGeoNode*) nodes->At(iNode);
        TString nodeName = node->GetName();
        nodeName.ToLower();
        if (nodeName.Contains("pipe")) {
          cout << "Found node: " << node->GetName() << endl;

          // find pipevac1
          TObjArray* nodes2 = node->GetNodes();
          for (Int_t iiNode = 0; iiNode < nodes2->GetEntriesFast(); iiNode++) {
            TGeoNode* node2   = (TGeoNode*) nodes2->At(iiNode);
            TString nodeName2 = node2->GetName();
            nodeName2.ToLower();
            if (nodeName2.Contains("pipevac1")) {
              cout << "Node: " << nodeName2 << endl;
              // check if there is a mvd in the pipevac
              TObjArray* nodes3 = node2->GetNodes();
              for (Int_t iiiNode = 0; iiiNode < nodes3->GetEntriesFast(); iiiNode++) {
                TGeoNode* node3   = (TGeoNode*) nodes3->At(iiiNode);
                TString nodeName3 = node3->GetName();
                nodeName3.ToLower();
                cout << "Node: " << nodeName3 << endl;
                if (nodeName3.Contains("mvd")) {
                  cout << "Node: " << nodeName3 << endl;
                  f->Close();
                  delete f;
                  gFile = currentfile;
                  return true;
                }
              }
            }
          }
        }
      }
    }
  }
  f->Close();
  gFile = currentfile;
  delete f;
  return false;
}

/**
 * \function IsTrd
 * \brief Determines TRD detector presence in TGeo.
 * \param[in] parFile Name of parameter file with TGeo.
 * \return True if TRD exists in TGeo.
 */
Bool_t IsTrd(const TString& parFile) { return CheckDetectorPresence(parFile, "trd"); }

/**
 * \function IsMuch
 * \brief Determines MUCH detector presence in TGeo.
 * \param[in] parFile Name of parameter file with TGeo.
 * \return True if MUCH exists in TGeo.
 */
Bool_t IsMuch(const TString& parFile) { return CheckDetectorPresence(parFile, "much"); }

/**
 * \function IsTof
 * \brief Determines TOF detector presence in TGeo.
 * \param[in] parFile Name of parameter file with TGeo.
 * \return True if TOF exists in TGeo.
 */
Bool_t IsTof(const TString& parFile) { return CheckDetectorPresence(parFile, "tof"); }

/**
 * \function IsRich
 * \brief Determines RICH detector presence in TGeo.
 * \param[in] parFile Name of parameter file with TGeo.
 * \return True if RICH exists in TGeo.
 */
Bool_t IsRich(const TString& parFile) { return CheckDetectorPresence(parFile, "rich"); }

/**
 * \function IsMvd
 * \brief Determines MVD detector presence in TGeo.
 * \param[in] parFile Name of the parameter file with TGeo.
 * \return True if MVD exists in TGeo.
 */
Bool_t IsMvd(const TString& parFile) { return CheckDetectorPresence(parFile, "mvd"); }

/**
 * \ function RemoveGeoManager
 * There are some problems when deleting our geometries. In some cases
 * or combinations of geometries there is a double free of some memory 
 * which results in a crash of ROOT. To avoid this we have patched one
 * ROOT class. With the newest ROOT6 version this isn't done any longer.
 * As a workaround to avoid the crash we delete two TObjArrays ourself 
 * and then call the destructor of the TGeoManager at the end of the 
 * macro. To simplify this one also can use this function.
 */
void RemoveGeoManager()
{
  if (gROOT->GetVersionInt() >= 60602) {
    gGeoManager->GetListOfVolumes()->Delete();
    gGeoManager->GetListOfShapes()->Delete();
    delete gGeoManager;
  }
}

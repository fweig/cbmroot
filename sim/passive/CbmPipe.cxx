/* Generated by Together */
#include "CbmPipe.h"

#include "CbmGeometryUtils.h"

#include <TString.h>

#include "TObjArray.h"

#include "TFile.h"
#include "TGeoManager.h"
#include "TKey.h"


// -----  Default constructor  ------------------------------------------------
CbmPipe::CbmPipe() : FairModule(), fCombiTrans(), fVolumeName("") {}
// ----------------------------------------------------------------------------


// -----  Constructor  --------------------------------------------------------
CbmPipe::CbmPipe(const char* name, const char* title)
  : FairModule(name, title), fCombiTrans(), fVolumeName("") {}
// ----------------------------------------------------------------------------


// -----  Destructor  ---------------------------------------------------------
CbmPipe::~CbmPipe() {}
// ----------------------------------------------------------------------------


// -----  ConstructGeometry  --------------------------------------------------
void CbmPipe::ConstructGeometry() {
  TString fileName = GetGeometryFileName();
  if (fileName.EndsWith(".root")) {
    if (Cbm::GeometryUtils::IsNewGeometryFile(fgeoName)) {
      LOG(info) << "Importing Pipe geometry from ROOT file " << fgeoName.Data();
      Cbm::GeometryUtils::ImportRootGeometry(fgeoName, this);
    } else {
      LOG(info) << "Constructing Pipe geometry from ROOT file "
                << fgeoName.Data();
      FairModule::ConstructRootGeometry();
    }
  } else
    LOG(fatal) << "Geometry format of PIPE file " << fileName.Data()
               << " not supported.";
}
// ----------------------------------------------------------------------------

ClassImp(CbmPipe)

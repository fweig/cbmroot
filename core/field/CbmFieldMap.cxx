// -------------------------------------------------------------------------
// -----                      CbmFieldMap source file                  -----
// -----         Created 12/01/04  by M. Al/Turany (CbmField.cxx)      -----
// -----                Redesign 13/02/06  by V. Friese                -----
// -------------------------------------------------------------------------
#include "CbmFieldMap.h"

#include "CbmFieldMapCreator.h"  // for CbmFieldMapCreator
#include "CbmFieldMapData.h"     // for CbmFieldMapData
#include "CbmFieldPar.h"         // for CbmFieldPar

#include <FairField.h>   // for FairField
#include <FairLogger.h>  // for LOG, Logger

#include <TArrayF.h>  // for TArrayF
#include <TFile.h>    // for TFile, gFile
#include <TMath.h>    // for Nint

#include <assert.h>  // for assert
#include <iomanip>   // for operator<<, setw
#include <iostream>  // for operator<<, basic_ostream, endl, ost...
#include <stdlib.h>  // for div, div_t, getenv, exit

using std::cerr;
using std::cout;
using std::endl;
using std::flush;
using std::right;
using std::setw;
using std::showpoint;


// -------------   Default constructor  ----------------------------------
CbmFieldMap::CbmFieldMap()
  : FairField()
  , fFileName("")
  , fScale(1.)
  , fPosX(0.)
  , fPosY(0.)
  , fPosZ(0.)
  , fXmin(0.)
  , fXmax(0.)
  , fXstep(0.)
  , fYmin(0.)
  , fYmax(0.)
  , fYstep(0.)
  , fZmin(0.)
  , fZmax(0.)
  , fZstep(0.)
  , fNx(0)
  , fNy(0)
  , fNz(0)
  , fBx(nullptr)
  , fBy(nullptr)
  , fBz(nullptr)
  , fBxOrigin(0.)
  , fByOrigin(0.)
  , fBzOrigin(0.) {
  // Initilization of arrays is to my knowledge not
  // possible in member initalization lists
  for (Int_t i = 0; i < 2; i++) {
    fHc[i] = 0;
    for (Int_t j = 0; j < 2; j++) {
      fHb[i][j] = 0;
      for (Int_t k = 0; k < 2; k++) {
        fHa[i][j][k] = 0;
      }
    }
  }
  // Assign values to data members of base classes
  // There is no appropriate constructor of the base
  // class.
  fName = "";
  fType = 1;
}
// ------------------------------------------------------------------------


// -------------   Standard constructor   ---------------------------------
CbmFieldMap::CbmFieldMap(const char* mapName, const char* fileType)
  : FairField()
  , fFileName("")
  , fScale(1.)
  , fPosX(0.)
  , fPosY(0.)
  , fPosZ(0.)
  , fXmin(0.)
  , fXmax(0.)
  , fXstep(0.)
  , fYmin(0.)
  , fYmax(0.)
  , fYstep(0.)
  , fZmin(0.)
  , fZmax(0.)
  , fZstep(0.)
  , fNx(0)
  , fNy(0)
  , fNz(0)
  , fBx(nullptr)
  , fBy(nullptr)
  , fBz(nullptr)
  , fBxOrigin(0.)
  , fByOrigin(0.)
  , fBzOrigin(0.) {
  // Initilization of arrays is to my knowledge not
  // possible in member initalization lists
  for (Int_t i = 0; i < 2; i++) {
    fHc[i] = 0;
    for (Int_t j = 0; j < 2; j++) {
      fHb[i][j] = 0;
      for (Int_t k = 0; k < 2; k++) {
        fHa[i][j][k] = 0;
      }
    }
  }
  // Assign values to data members of base classes
  // There is no appropriate constructor of the base
  // class.
  fName       = mapName;
  fTitle      = "CbmFieldMap";
  TString dir = getenv("VMCWORKDIR");
  fFileName   = dir + "/input/" + mapName;
  if (fileType[0] == 'R') {
    fFileName += ".root";
  } else {
    fFileName += ".dat";
  }
  fType = 1;
  LOG(info) << "Filename is " << fFileName;
}
// ------------------------------------------------------------------------


// ------------   Constructor from CbmFieldPar   --------------------------
CbmFieldMap::CbmFieldMap(CbmFieldPar* fieldPar)
  : FairField()
  , fFileName("")
  , fScale(1.)
  , fPosX(0.)
  , fPosY(0.)
  , fPosZ(0.)
  , fXmin(0.)
  , fXmax(0.)
  , fXstep(0.)
  , fYmin(0.)
  , fYmax(0.)
  , fYstep(0.)
  , fZmin(0.)
  , fZmax(0.)
  , fZstep(0.)
  , fNx(0)
  , fNy(0)
  , fNz(0)
  , fBx(nullptr)
  , fBy(nullptr)
  , fBz(nullptr)
  , fBxOrigin(0.)
  , fByOrigin(0.)
  , fBzOrigin(0.) {
  // Initilization of arrays is to my knowledge not
  // possible in member initalization lists
  for (Int_t i = 0; i < 2; i++) {
    fHc[i] = 0;
    for (Int_t j = 0; j < 2; j++) {
      fHb[i][j] = 0;
      for (Int_t k = 0; k < 2; k++) {
        fHa[i][j][k] = 0;
      }
    }
  }
  // Assign values to data members of base classes
  // There is no appropriate constructor of the base
  // class.
  fName = "";
  fType = 1;
  if (!fieldPar) {
    cerr << "-W- CbmFieldConst::CbmFieldMap: empty parameter container!"
         << endl;
  } else {
    fieldPar->MapName(fName);
    fPosX       = fieldPar->GetPositionX();
    fPosY       = fieldPar->GetPositionY();
    fPosZ       = fieldPar->GetPositionZ();
    fScale      = fieldPar->GetScale();
    TString dir = getenv("VMCWORKDIR");
    fFileName   = dir + "/input/" + fName + ".root";
    fType       = fieldPar->GetType();
  }
}
// ------------------------------------------------------------------------


// ------------  Constructor from CbmFieldMapCreator  ---------------------
CbmFieldMap::CbmFieldMap(CbmFieldMapCreator* creator)
  : FairField()
  , fFileName("")
  , fScale(1.)
  , fPosX(0.)
  , fPosY(0.)
  , fPosZ(0.)
  , fXmin(0.)
  , fXmax(0.)
  , fXstep(0.)
  , fYmin(0.)
  , fYmax(0.)
  , fYstep(0.)
  , fZmin(0.)
  , fZmax(0.)
  , fZstep(0.)
  , fNx(0)
  , fNy(0)
  , fNz(0)
  , fBx(nullptr)
  , fBy(nullptr)
  , fBz(nullptr)
  , fBxOrigin(0.)
  , fByOrigin(0.)
  , fBzOrigin(0.) {
  // Initilization of arrays is to my knowledge not
  // possible in member initalization lists
  for (Int_t i = 0; i < 2; i++) {
    fHc[i] = 0;
    for (Int_t j = 0; j < 2; j++) {
      fHb[i][j] = 0;
      for (Int_t k = 0; k < 2; k++) {
        fHa[i][j][k] = 0;
      }
    }
  }
  // Assign values to data members of base classes
  // There is no appropriate constructor of the base
  // class.
  fName = "";
  fType = 1;
  if (!creator) {
    cerr << "-W- CbmFieldMap: no creator given!" << endl;
  } else {
    fType  = 1;
    fName  = creator->GetMapName();
    fXmin  = creator->GetXmin();
    fXmax  = creator->GetXmax();
    fYmin  = creator->GetYmin();
    fYmax  = creator->GetYmax();
    fZmin  = creator->GetZmin();
    fZmax  = creator->GetZmax();
    fNx    = creator->GetNx();
    fNy    = creator->GetNy();
    fNz    = creator->GetNz();
    fXstep = (fXmax - fXmin) / Double_t(fNx - 1);
    fYstep = (fYmax - fYmin) / Double_t(fNy - 1);
    fZstep = (fZmax - fZmin) / Double_t(fNz - 1);
    fBx    = creator->GetBx();
    fBy    = creator->GetBy();
    fBz    = creator->GetBz();
  }
}

// ------------------------------------------------------------------------


// ------------   Destructor   --------------------------------------------
CbmFieldMap::~CbmFieldMap() {
  if (fBx) delete fBx;
  if (fBy) delete fBy;
  if (fBz) delete fBz;
}
// ------------------------------------------------------------------------


// -----------   Intialisation   ------------------------------------------
void CbmFieldMap::Init() {
  if (fFileName.EndsWith(".root"))
    ReadRootFile(fFileName, fName);
  else if (fFileName.EndsWith(".dat"))
    ReadAsciiFile(fFileName);
  else {
    cerr << "-E- CbmFieldMap::Init: No proper file name defined! (" << fFileName
         << ")" << endl;
    Fatal("Init", "No proper file name");
  }
  // Fill values needed in the Print() function. This is needed to allow
  // a constant Print() function.
  fBxOrigin = GetBx(0., 0., 0.);
  fByOrigin = GetBy(0., 0., 0.);
  fBzOrigin = GetBz(0., 0., 0.);

  Print();
}
// ------------------------------------------------------------------------


// ----- Initialisation from arrays   -------------------------------------
void CbmFieldMap::Init(Int_t nX,
                       Double_t xMin,
                       Double_t xMax,
                       Int_t nY,
                       Double_t yMin,
                       Double_t yMax,
                       Int_t nZ,
                       Double_t zMin,
                       Double_t zMax,
                       TArrayF* bx,
                       TArrayF* by,
                       TArrayF* bz) {

  Reset();
  fNx           = nX;
  fXmin         = xMin;
  fXmax         = xMax;
  fNy           = nY;
  fYmin         = yMin;
  fYmax         = yMax;
  fNz           = nZ;
  fZmin         = zMin;
  fZmax         = zMax;
  Int_t nPoints = nX * nY * nZ;
  assert(bx->GetSize() == by->GetSize());
  assert(bz->GetSize() == bx->GetSize());
  if (bx->GetSize() != nPoints) {
    LOG(error) << "CbmFieldMap: array size " << bx->GetSize()
               << " does not match number of grid points.";
    return;
  }
  fBx         = new TArrayF(nPoints);
  fBy         = new TArrayF(nPoints);
  fBz         = new TArrayF(nPoints);
  Int_t index = 0;
  for (Int_t ix = 0; ix < fNx; ix++) {
    for (Int_t iy = 0; iy < fNy; iy++) {
      for (Int_t iz = 0; iz < fNz; iz++) {
        index         = ix * fNy * fNz + iy * fNz + iz;
        (*fBx)[index] = (*bx)[index];
        (*fBy)[index] = (*by)[index];
        (*fBz)[index] = (*bz)[index];
      }  //# iz
    }    //# iy
  }      //# ix

  fXstep    = (fXmax - fXmin) / Double_t(fNx - 1);
  fYstep    = (fYmax - fYmin) / Double_t(fNy - 1);
  fZstep    = (fZmax - fZmin) / Double_t(fNz - 1);
  fBxOrigin = GetBx(0., 0., 0.);
  fByOrigin = GetBy(0., 0., 0.);
  fBzOrigin = GetBz(0., 0., 0.);

  LOG(info) << "CbmFieldMap: Initialised from " << nPoints << " grid points";
  Print();
}
// ------------------------------------------------------------------------


// -----------   Get x component of the field   ---------------------------
Double_t CbmFieldMap::GetBx(Double_t x, Double_t y, Double_t z) {

  Int_t ix    = 0;
  Int_t iy    = 0;
  Int_t iz    = 0;
  Double_t dx = 0.;
  Double_t dy = 0.;
  Double_t dz = 0.;

  if (IsInside(x, y, z, ix, iy, iz, dx, dy, dz)) {

    // Get Bx field values at grid cell corners
    fHa[0][0][0] = fBx->At(ix * fNy * fNz + iy * fNz + iz);
    fHa[1][0][0] = fBx->At((ix + 1) * fNy * fNz + iy * fNz + iz);
    fHa[0][1][0] = fBx->At(ix * fNy * fNz + (iy + 1) * fNz + iz);
    fHa[1][1][0] = fBx->At((ix + 1) * fNy * fNz + (iy + 1) * fNz + iz);
    fHa[0][0][1] = fBx->At(ix * fNy * fNz + iy * fNz + (iz + 1));
    fHa[1][0][1] = fBx->At((ix + 1) * fNy * fNz + iy * fNz + (iz + 1));
    fHa[0][1][1] = fBx->At(ix * fNy * fNz + (iy + 1) * fNz + (iz + 1));
    fHa[1][1][1] = fBx->At((ix + 1) * fNy * fNz + (iy + 1) * fNz + (iz + 1));

    // Return interpolated field value
    return Interpolate(dx, dy, dz);
  }

  return 0.;
}
// ------------------------------------------------------------------------


// -----------   Get y component of the field   ---------------------------
Double_t CbmFieldMap::GetBy(Double_t x, Double_t y, Double_t z) {

  Int_t ix    = 0;
  Int_t iy    = 0;
  Int_t iz    = 0;
  Double_t dx = 0.;
  Double_t dy = 0.;
  Double_t dz = 0.;

  if (IsInside(x, y, z, ix, iy, iz, dx, dy, dz)) {

    // Get By field values at grid cell corners
    fHa[0][0][0] = fBy->At(ix * fNy * fNz + iy * fNz + iz);
    fHa[1][0][0] = fBy->At((ix + 1) * fNy * fNz + iy * fNz + iz);
    fHa[0][1][0] = fBy->At(ix * fNy * fNz + (iy + 1) * fNz + iz);
    fHa[1][1][0] = fBy->At((ix + 1) * fNy * fNz + (iy + 1) * fNz + iz);
    fHa[0][0][1] = fBy->At(ix * fNy * fNz + iy * fNz + (iz + 1));
    fHa[1][0][1] = fBy->At((ix + 1) * fNy * fNz + iy * fNz + (iz + 1));
    fHa[0][1][1] = fBy->At(ix * fNy * fNz + (iy + 1) * fNz + (iz + 1));
    fHa[1][1][1] = fBy->At((ix + 1) * fNy * fNz + (iy + 1) * fNz + (iz + 1));

    // Return interpolated field value
    return Interpolate(dx, dy, dz);
  }

  return 0.;
}
// ------------------------------------------------------------------------


// -----------   Get z component of the field   ---------------------------
Double_t CbmFieldMap::GetBz(Double_t x, Double_t y, Double_t z) {

  Int_t ix    = 0;
  Int_t iy    = 0;
  Int_t iz    = 0;
  Double_t dx = 0.;
  Double_t dy = 0.;
  Double_t dz = 0.;

  if (IsInside(x, y, z, ix, iy, iz, dx, dy, dz)) {

    // Get Bz field values at grid cell corners
    fHa[0][0][0] = fBz->At(ix * fNy * fNz + iy * fNz + iz);
    fHa[1][0][0] = fBz->At((ix + 1) * fNy * fNz + iy * fNz + iz);
    fHa[0][1][0] = fBz->At(ix * fNy * fNz + (iy + 1) * fNz + iz);
    fHa[1][1][0] = fBz->At((ix + 1) * fNy * fNz + (iy + 1) * fNz + iz);
    fHa[0][0][1] = fBz->At(ix * fNy * fNz + iy * fNz + (iz + 1));
    fHa[1][0][1] = fBz->At((ix + 1) * fNy * fNz + iy * fNz + (iz + 1));
    fHa[0][1][1] = fBz->At(ix * fNy * fNz + (iy + 1) * fNz + (iz + 1));
    fHa[1][1][1] = fBz->At((ix + 1) * fNy * fNz + (iy + 1) * fNz + (iz + 1));

    // Return interpolated field value
    return Interpolate(dx, dy, dz);
  }

  return 0.;
}
// ------------------------------------------------------------------------


// -----------   Check whether a point is inside the map   ----------------
Bool_t CbmFieldMap::IsInside(Double_t x,
                             Double_t y,
                             Double_t z,
                             Int_t& ix,
                             Int_t& iy,
                             Int_t& iz,
                             Double_t& dx,
                             Double_t& dy,
                             Double_t& dz) {

  // --- Transform into local coordinate system
  Double_t xl = x - fPosX;
  Double_t yl = y - fPosY;
  Double_t zl = z - fPosZ;

  // ---  Check for being outside the map range
  if (!(xl >= fXmin && xl < fXmax && yl >= fYmin && yl < fYmax && zl >= fZmin
        && zl < fZmax)) {
    ix = iy = iz = 0;
    dx = dy = dz = 0.;
    return kFALSE;
  }

  // --- Determine grid cell
  ix = Int_t((xl - fXmin) / fXstep);
  iy = Int_t((yl - fYmin) / fYstep);
  iz = Int_t((zl - fZmin) / fZstep);


  // Relative distance from grid point (in units of cell size)
  dx = (xl - fXmin) / fXstep - Double_t(ix);
  dy = (yl - fYmin) / fYstep - Double_t(iy);
  dz = (zl - fZmin) / fZstep - Double_t(iz);

  return kTRUE;
}
// ------------------------------------------------------------------------


// ----------   Write the map to an ASCII file   --------------------------
void CbmFieldMap::WriteAsciiFile(const char* fileName) {

  // Open file
  LOG(info) << "Writing field map to ASCII file " << fileName;
  std::ofstream mapFile(fileName);
  if (!mapFile.is_open()) {
    LOG(error) << "Could not open file! ";
    return;
  }

  // Write field map grid parameters
  mapFile.precision(6);
  mapFile << showpoint;
  if (fType == 1) mapFile << "nosym" << endl;
  if (fType == 2) mapFile << "sym2" << endl;
  if (fType == 3) mapFile << "sym3" << endl;
  mapFile << fXmin << " " << fXmax << " " << fNx << endl;
  mapFile << fYmin << " " << fYmax << " " << fNy << endl;
  mapFile << fZmin << " " << fZmax << " " << fNz << endl;

  // Write field values
  Double_t factor = 10. * fScale;  // Takes out scaling and converts kG->T
  cout << right;
  Int_t nTot = fNx * fNy * fNz;
  cout << "-I- CbmFieldMap: " << fNx * fNy * fNz << " entries to write... "
       << setw(3) << 0 << " % ";
  Int_t index = 0;
  div_t modul;
  Int_t iDiv = TMath::Nint(nTot / 100.);
  for (Int_t ix = 0; ix < fNx; ix++) {
    for (Int_t iy = 0; iy < fNy; iy++) {
      for (Int_t iz = 0; iz < fNz; iz++) {
        index = ix * fNy * fNz + iy * fNz + iz;
        modul = div(index, iDiv);
        if (modul.rem == 0) {
          Double_t perc = TMath::Nint(100. * index / nTot);
          cout << "\b\b\b\b\b\b" << setw(3) << perc << " % " << flush;
        }
        mapFile << fBx->At(index) / factor << " " << fBy->At(index) / factor
                << " " << fBz->At(index) / factor << endl;
      }  // z-Loop
    }    // y-Loop
  }      // x-Loop
  cout << "   " << index + 1 << " written" << endl;
  mapFile.close();
}
// ------------------------------------------------------------------------


// -------   Write field map to a ROOT file   -----------------------------
void CbmFieldMap::WriteRootFile(const char* fileName, const char* mapName) {

  CbmFieldMapData* data = new CbmFieldMapData(mapName, *this);

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* file           = new TFile(fileName, "RECREATE");
  data->Write();
  file->Close();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}
// ------------------------------------------------------------------------


// -----  Set the position of the field centre in global coordinates  -----
void CbmFieldMap::SetPosition(Double_t x, Double_t y, Double_t z) {
  fPosX = x;
  fPosY = y;
  fPosZ = z;
}
// ------------------------------------------------------------------------


// ---------   Screen output   --------------------------------------------
void CbmFieldMap::Print(Option_t*) const {
  TString type = "Map";
  if (fType == 2) type = "Map sym2";
  if (fType == 3) type = "Map sym3";
  cout << "======================================================" << endl;
  cout.precision(4);
  cout << showpoint;
  cout << "----  " << fTitle << " : " << fName << endl;
  cout << "----" << endl;
  cout << "----  Field type     : " << type << endl;
  cout << "----" << endl;
  cout << "----  Field map grid : " << endl;
  cout << "----  x = " << setw(4) << fXmin << " to " << setw(4) << fXmax
       << " cm, " << fNx << " grid points, dx = " << fXstep << " cm" << endl;
  cout << "----  y = " << setw(4) << fYmin << " to " << setw(4) << fYmax
       << " cm, " << fNy << " grid points, dy = " << fYstep << " cm" << endl;
  cout << "----  z = " << setw(4) << fZmin << " to " << setw(4) << fZmax
       << " cm, " << fNz << " grid points, dz = " << fZstep << " cm" << endl;
  cout << endl;
  cout << "----  Field centre position: ( " << setw(6) << fPosX << ", "
       << setw(6) << fPosY << ", " << setw(6) << fPosZ << ") cm" << endl;
  cout << "----  Field scaling factor: " << fScale << endl;
  cout << "----" << endl;
  cout << "----  Field at origin is ( " << setw(6) << fBxOrigin << ", "
       << setw(6) << fByOrigin << ", " << setw(6) << fBzOrigin << ") kG"
       << endl;

  cout << "======================================================" << endl;
}
// ------------------------------------------------------------------------


// ---------    Reset parameters and data (private)  ----------------------
void CbmFieldMap::Reset() {
  fPosX = fPosY = fPosZ = 0.;
  fXmin = fYmin = fZmin = 0.;
  fXmax = fYmax = fZmax = 0.;
  fXstep = fYstep = fZstep = 0.;
  fNx = fNy = fNz = 0;
  fScale          = 1.;
  if (fBx) {
    delete fBx;
    fBx = nullptr;
  }
  if (fBy) {
    delete fBy;
    fBy = nullptr;
  }
  if (fBz) {
    delete fBz;
    fBz = nullptr;
  }
}
// ------------------------------------------------------------------------


// -----   Read field map from ASCII file (private)   ---------------------
void CbmFieldMap::ReadAsciiFile(const char* fileName) {

  Double_t bx = 0., by = 0., bz = 0.;

  // Open file
  LOG(info) << "CbmFieldMap: Reading field map from ASCII file " << fileName;
  std::ifstream mapFile(fileName);
  if (!mapFile.is_open()) {
    LOG(error) << "CbmFieldMap:ReadAsciiFile: Could not open file!";
    LOG(fatal) << "CbmFieldMap:ReadAsciiFile: Could not open file!";
  }

  // Read map type
  TString type;
  mapFile >> type;
  Int_t iType = 0;
  if (type == "nosym") iType = 1;
  if (type == "sym2") iType = 2;
  if (type == "sym3") iType = 3;
  if (fType != iType) {
    cout << "-E- CbmFieldMap::ReadAsciiFile: Incompatible map types!" << endl;
    cout << "    Field map is of type " << fType
         << " but map on file is of type " << iType << endl;
    Fatal("ReadAsciiFile", "Incompatible map types");
  }

  // Read grid parameters
  mapFile >> fXmin >> fXmax >> fNx;
  mapFile >> fYmin >> fYmax >> fNy;
  mapFile >> fZmin >> fZmax >> fNz;
  fXstep = (fXmax - fXmin) / Double_t(fNx - 1);
  fYstep = (fYmax - fYmin) / Double_t(fNy - 1);
  fZstep = (fZmax - fZmin) / Double_t(fNz - 1);

  // Create field arrays
  fBx = new TArrayF(fNx * fNy * fNz);
  fBy = new TArrayF(fNx * fNy * fNz);
  fBz = new TArrayF(fNx * fNy * fNz);

  // Read the field values
  Double_t factor = fScale * 10.;  // Factor 10 for T -> kG
  cout << right;
  Int_t nTot = fNx * fNy * fNz;
  cout << "-I- CbmFieldMap: " << nTot << " entries to read... " << setw(3) << 0
       << " % ";
  Int_t index = 0;
  div_t modul;
  Int_t iDiv = TMath::Nint(nTot / 100.);
  for (Int_t ix = 0; ix < fNx; ix++) {
    for (Int_t iy = 0; iy < fNy; iy++) {
      for (Int_t iz = 0; iz < fNz; iz++) {
        if (!mapFile.good())
          cerr << "-E- CbmFieldMap::ReadAsciiFile: "
               << "I/O Error at " << ix << " " << iy << " " << iz << endl;
        index = ix * fNy * fNz + iy * fNz + iz;
        modul = div(index, iDiv);
        if (modul.rem == 0) {
          Double_t perc = TMath::Nint(100. * index / nTot);
          cout << "\b\b\b\b\b\b" << setw(3) << perc << " % " << flush;
        }
        mapFile >> bx >> by >> bz;
        fBx->AddAt(factor * bx, index);
        fBy->AddAt(factor * by, index);
        fBz->AddAt(factor * bz, index);
        if (mapFile.eof()) {
          cerr << endl
               << "-E- CbmFieldMap::ReadAsciiFile: EOF"
               << " reached at " << ix << " " << iy << " " << iz << endl;
          mapFile.close();
          break;
        }
      }  // z-Loop
    }    // y-Loop0)
  }      // x-Loop

  cout << "   " << index + 1 << " read" << endl;

  mapFile.close();
}
// ------------------------------------------------------------------------


// -------------   Read field map from ROOT file (private)  ---------------
void CbmFieldMap::ReadRootFile(const char* fileName, const char* mapName) {
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  // Open root file
  LOG(info) << "CbmFieldMap: Reading field map from ROOT file " << fileName;
  TFile* file = new TFile(fileName, "READ");
  if (!(file->IsOpen())) {
    LOG(error) << "CbmFieldMap:ReadRootFile: Could not open file!";
    LOG(fatal) << "CbmFieldMap:ReadRootFile: Could not open file!";
  }

  // Get the field data object
  CbmFieldMapData* data = nullptr;
  file->GetObject(mapName, data);
  if (!data) {
    cout << "-E- CbmFieldMap::ReadRootFile: data object " << fileName
         << " not found in file! " << endl;
    exit(-1);
  }

  // Get the field parameters
  SetField(data);

  // Close the root file and delete the data object
  file->Close();
  delete data;

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}
// ------------------------------------------------------------------------


// ------------   Set field parameters and data (private)  ----------------
void CbmFieldMap::SetField(const CbmFieldMapData* data) {

  // Check compatibility
  if (data->GetType() != fType) {
    if (!((data->GetType() == 3) && (fType == 5)))  // E.Litvinenko
    {
      cout << "-E- CbmFieldMap::SetField: Incompatible map types!" << endl;
      cout << "    Field map is of type " << fType
           << " but map on file is of type " << data->GetType() << endl;
      Fatal("SetField", "Incompatible map types");
    } else
      cout << "   CbmFieldMap::SetField: Warning:  You are using PosDepScaled "
              "map (original map type = 3)"
           << endl;
  }


  fXmin  = data->GetXmin();
  fYmin  = data->GetYmin();
  fZmin  = data->GetZmin();
  fXmax  = data->GetXmax();
  fYmax  = data->GetYmax();
  fZmax  = data->GetZmax();
  fNx    = data->GetNx();
  fNy    = data->GetNy();
  fNz    = data->GetNz();
  fXstep = (fXmax - fXmin) / Double_t(fNx - 1);
  fYstep = (fYmax - fYmin) / Double_t(fNy - 1);
  fZstep = (fZmax - fZmin) / Double_t(fNz - 1);
  if (fBx) delete fBx;
  if (fBy) delete fBy;
  if (fBz) delete fBz;
  fBx = new TArrayF(*(data->GetBx()));
  fBy = new TArrayF(*(data->GetBy()));
  fBz = new TArrayF(*(data->GetBz()));

  // Scale and convert from T to kG
  Double_t factor = fScale * 10.;
  Int_t index     = 0;
  for (Int_t ix = 0; ix < fNx; ix++) {
    for (Int_t iy = 0; iy < fNy; iy++) {
      for (Int_t iz = 0; iz < fNz; iz++) {
        index = ix * fNy * fNz + iy * fNz + iz;
        if (fBx) (*fBx)[index] = (*fBx)[index] * factor;
        if (fBy) (*fBy)[index] = (*fBy)[index] * factor;
        if (fBz) (*fBz)[index] = (*fBz)[index] * factor;
      }
    }
  }
}
// ------------------------------------------------------------------------


// ------------   Interpolation in a grid cell (private)  -----------------
Double_t CbmFieldMap::Interpolate(Double_t dx, Double_t dy, Double_t dz) {

  // Interpolate in x coordinate
  fHb[0][0] = fHa[0][0][0] + (fHa[1][0][0] - fHa[0][0][0]) * dx;
  fHb[1][0] = fHa[0][1][0] + (fHa[1][1][0] - fHa[0][1][0]) * dx;
  fHb[0][1] = fHa[0][0][1] + (fHa[1][0][1] - fHa[0][0][1]) * dx;
  fHb[1][1] = fHa[0][1][1] + (fHa[1][1][1] - fHa[0][1][1]) * dx;

  // Interpolate in y coordinate
  fHc[0] = fHb[0][0] + (fHb[1][0] - fHb[0][0]) * dy;
  fHc[1] = fHb[0][1] + (fHb[1][1] - fHb[0][1]) * dy;

  // Interpolate in z coordinate
  return fHc[0] + (fHc[1] - fHc[0]) * dz;
}
// ------------------------------------------------------------------------


ClassImp(CbmFieldMap)

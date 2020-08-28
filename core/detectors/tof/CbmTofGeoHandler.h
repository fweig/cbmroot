// -------------------------------------------------------------------------
// -----                 CbmTofGeoHandler header file                  -----
// -----                 Created 20/11/12  by F. Uhlig                 -----
// -------------------------------------------------------------------------


/** CbmTofGeoHandler.h
 ** Helper class to extract information from the GeoManager which is
 ** needed in many other TOF classes. This helper class should be a
 ** single place to hold all these functions.
 ** @author F. Uhlig <f.uhlig@gsi.de>
 **/

#ifndef CBMTOFGEOHANDLER_H
#define CBMTOFGEOHANDLER_H 1

enum TofGeometryVersions { k07a, k12a, k12b, k14a, k21a };

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Float_t, Bool_t, kFALSE, Double_t
#include <TObject.h>     // for TObject
#include <TString.h>     // for TString

#include "CbmTofDetectorId.h"  // for CbmTofDetectorId (ptr only), CbmTofDet...

class TGeoBBox;
class TGeoHMatrix;
class TGeoVolume;

class CbmTofGeoHandler : public TObject {
public:
  /** Constructor **/
  CbmTofGeoHandler();


  /** Destructor **/
  ~CbmTofGeoHandler() {};

  Int_t GetUniqueDetectorId();
  Int_t GetUniqueCounterId();
  Int_t GetUniqueDetectorId(TString volName);

  Int_t GetCellId(Int_t uniqueId);

  Int_t GetGeoVersion() { return fGeoVersion; }

  Int_t Init(Bool_t isSimulation = kFALSE);

  Int_t GetDetSystemId(Int_t uniqueId);
  Int_t GetSMType(Int_t uniqueId);
  Int_t GetSModule(Int_t uniqueId);
  Int_t GetCounter(Int_t uniqueId);
  Int_t GetGap(Int_t uniqueId);
  Int_t GetCell(Int_t uniqueId);
  Int_t GetRegion(Int_t uniqueId);

  Float_t GetSizeX(TString volName);
  Float_t GetSizeY(TString volName);
  Float_t GetSizeZ(TString volName);
  Float_t GetX(TString volName);
  Float_t GetY(TString volName);
  Float_t GetZ(TString volName);

  void FillDetectorInfoArray(Int_t uniqueId);
  void NavigateTo(TString volName);

  // Implement Interface functions to the TGeoManager to be
  // the same as for the VMC
  Int_t CurrentVolOffID(Int_t off, Int_t& copy) const;
  Int_t CurrentVolID(Int_t& copy) const;
  Int_t VolId(const Text_t* name) const;
  Int_t VolIdGeo(const char* name) const;
  const char* CurrentVolName() const;
  const char* CurrentVolOffName(Int_t off) const;
  const char* CurrentNodeName() const;
  const char* CurrentNodeOffName(Int_t off) const;

  Int_t CheckGeometryVersion();

  CbmTofDetectorId* GetDetIdPointer() { return fTofId; }

private:
  CbmTofDetectorId* fTofId;  //!
  Int_t fGeoVersion;         //!

  Bool_t fIsSimulation;  //!
  Int_t fMCVersion;      //!
  Bool_t fUseNodeName;   //!

  Int_t fLastUsedDetectorID;              //!
  CbmTofDetectorInfo fDetectorInfoArray;  //!
  UInt_t fGeoPathHash;                    //!
  TGeoVolume* fCurrentVolume;             //!
  TGeoBBox* fVolumeShape;                 //!
  Double_t fGlobal[3];                    //! Global centre of volume
  TGeoHMatrix* fGlobalMatrix;             //!

  CbmTofGeoHandler(const CbmTofGeoHandler&);
  CbmTofGeoHandler operator=(const CbmTofGeoHandler&);

  ClassDef(CbmTofGeoHandler, 0)
};


#endif  //CBMTOFGEOHANDLER_H

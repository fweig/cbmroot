/** @file CbmMuchGeoScheme.h
  * @copyright Copyright (C) 2008-2021 St. Petersburg Polytechnic University, St. Petersburg
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig, Evgeny Kryshen, Mikhail Ryzhinskiy [committer], David Emschermann, Vikas Singhal **/

// -------------------------------------------------------------------------
// -----                   CbmMuchGeoScheme header file                -----
// -----                  Created 18/02/08  by E. Kryshen
// -----                  Modified 18/10/2017 by Omveer Singh          -----
// -------------------------------------------------------------------------

/** CbmMuchGeoScheme
 *@author Evgeny Kryshen <e.kryshen@gsi.de>
 *@since 18.02.08
 *@version 1.0
 *
 */

#ifndef CbmMuchGeoScheme_H
#define CbmMuchGeoScheme_H 1

#include "CbmMuchAddress.h"  // for CbmMuchAddress, kMuchLayer, kMuchLayerSide

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t, Bool_t, Float_t, Char_t
#include <TArrayC.h>     // for TArrayC
#include <TArrayD.h>     // for TArrayD
#include <TArrayI.h>     // for TArrayI
#include <TGeoBBox.h>    // for TGeoBBox
#include <TGeoCone.h>    // for TGeoCone
#include <TObjArray.h>   // for TObjArray
#include <TObject.h>     // for TObject
#include <TString.h>     // for TString

#include <map>     // for map
#include <vector>  // for vector

class CbmMuchLayer;
class CbmMuchLayerSide;
class CbmMuchModule;
class CbmMuchModuleGem;
class CbmMuchStation;
class TGeoHMatrix;
class TGeoNode;
class TGeoTrap;
class TGeoVolume;

class CbmMuchGeoScheme : public TObject {

public:
  /** Destructor.     */
  ~CbmMuchGeoScheme();
  //void Init(Bool_t isSimulation = kFALSE);
  static CbmMuchGeoScheme* Instance();
  /** Gets whether the geometry scheme is initialized. */
  Bool_t IsInitialized() { return fInitialized; }

  // Get geometry objects by indices
  CbmMuchStation* GetStation(Int_t iStation) const;
  //Int_t GetStation(const TString& path);
  CbmMuchLayer* GetLayer(Int_t iStation, Int_t iLayer) const;
  CbmMuchLayerSide* GetLayerSide(Int_t iStation, Int_t iLayer, Bool_t iSide) const;
  CbmMuchModule* GetModule(Int_t iStation, Int_t iLayer, Bool_t iSide, Int_t iModule) const;

  // Get geometry objects by detector id
  CbmMuchStation* GetStationByDetId(Int_t detId) const;
  CbmMuchLayer* GetLayerByDetId(Int_t detId) const;
  CbmMuchLayerSide* GetLayerSideByDetId(Int_t detId) const;
  CbmMuchModule* GetModuleByDetId(Int_t detId) const;

  static Int_t GetStationIndex(Int_t address) { return CbmMuchAddress::GetElementId(address, kMuchStation); }
  static Int_t GetLayerIndex(Int_t address) { return CbmMuchAddress::GetElementId(address, kMuchLayer); }
  static Int_t GetLayerSideIndex(Int_t address) { return CbmMuchAddress::GetElementId(address, kMuchLayerSide); }

  //  fStations->GetEntriesFast();
  Int_t GetNStations() const { return fStations->GetEntriesFast(); }
  Int_t GetNAbsorbers() const { return fNabs; }
  TObjArray* GetStations() const { return fStations; }
  TObjArray* GetAbsorbers() const { return fAbsorbers; }
  TGeoCone* GetMuchCave() const { return fMuchCave; }
  Double_t GetMuchCaveZ0() const { return fMuchZ1 + fMuchCave->GetDz(); }
  //  Double_t   GetAbsorberZ0(Int_t i) const {return fMuchZ1+fAbsorberZ1[i]+((TGeoCone*) fAbsorbers->At(i))->GetDz();}
  Char_t GetAbsorberMat(Int_t i) const { return fAbsorberMat[i]; }
  Double_t GetAbsorberZ0(Int_t i)
  {
    if (i == 0) return fMuchZ1 + fAbsorberZ1[i] + ((TGeoBBox*) fAbsorbers->At(i))->GetDZ();
    //return fMuchZ1+fAbsorberZ1[i]+((TGeoCone*)fAbsorbers->At(i))->GetDz();

    else
      return fMuchZ1 + fAbsorberZ1[i] + ((TGeoBBox*) fAbsorbers->At(i))->GetDZ();
  }


  Double_t GetSizeX(const TString& path);
  Double_t GetSizeY(const TString& path);
  Double_t GetSizeZ(const TString& path);
  Double_t GetX(const TString& path);
  Double_t GetY(const TString& path);
  Double_t GetZ(const TString& path);


  Double_t GetModuleDZ(const TString& path);
  Double_t GetModuleX(const TString& path);
  Double_t GetModuleY(const TString& path);
  Double_t GetModuleZ(const TString& path);
  Double_t GetModulePhi(const TString& path);
  Double_t GetModuleH1(const TString& path);
  Double_t GetModuleBl1(const TString& path);
  Double_t GetModuleTl1(const TString& path);


  Double_t GetActiveLx() const { return fActiveLx; }
  Double_t GetActiveLy() const { return fActiveLy; }
  Double_t GetActiveLz() const { return fActiveLz; }
  Double_t GetSpacerLx() const { return fSpacerLx; }
  Double_t GetSpacerLy() const { return fSpacerLy; }
  Double_t GetSpacerR() const { return fSpacerR; }
  Double_t GetSpacerPhi() const { return fSpacerPhi; }
  Double_t GetLayersDz(Int_t st) const { return fLayersDz[st]; }

  void Init(TObjArray* stations, Int_t flag);
  void Init(TString digiFileName, Int_t flag);
  void CreatePointArrays();
  void CreateHitArrays();
  void CreateClusterArrays();
  void ClearPointArrays();
  void ClearHitArrays();
  void ClearClusterArrays();
  std::vector<CbmMuchModule*> GetModules() const;
  std::vector<CbmMuchModuleGem*> GetGemModules() const;
  std::vector<CbmMuchModule*> GetModules(Int_t iStation) const;
  std::vector<CbmMuchLayerSide*> GetLayerSides(Int_t iStation) const;
  Int_t GetLayerSideNr(Int_t detId) const;

  void ExtractGeoParameter(TGeoNode* muchNode, const char* volumeName);
  void StationNode(TGeoNode* MuchObjNode, TString MuchObjPath);
  void LayerNode(TGeoNode* StNode, Int_t iStation, TString StPath);
  void ModuleNode(TGeoNode* layerNode, Int_t iStation, Int_t iLayer, TString layerPath);
  void ActiveModuleNode(TGeoNode* moduleNode, Int_t iStation, Int_t iLayer, Int_t iModule, TString modulePath);


private:
  CbmMuchGeoScheme();
  void NavigateTo(const TString& path);
  void NavigateModule(const TString& path);
  void InitModules();
  Int_t Intersect(Float_t x, Float_t y, Float_t dx, Float_t dy, Float_t r);

  UInt_t fGeoPathHash;         //!
  TGeoVolume* fCurrentVolume;  //!
  TGeoBBox* fVolumeBoxShape;   //!
  TGeoTrap* fVolumeTrapShape;
  Double_t fGlobal[3];
  Double_t fGlobalTrap[3];     //! Global center of volume
  TGeoHMatrix* fGlobalMatrix;  //!

  // for backward compatibility
  Int_t fStation;  //! StationTypeID, 1..3
  Int_t fLayer;    //! LayerID within station, 1..4
  Int_t fActive;   //! ModuleCopyID with module type
  Int_t fGeoID;    // Id to distinguish geometry (mcbm/cbm)

  static CbmMuchGeoScheme* fInstance;
  static Bool_t fInitialized;         // Defines whether the instance was initialized
  static Bool_t fModulesInitialized;  // Defines whether grid of the instance was initialized

  std::vector<std::vector<CbmMuchModule*>> fModules;   //!
  std::vector<std::vector<CbmMuchLayerSide*>> fSides;  //!
  std::map<Int_t, Int_t> fMapSides;

  TObjArray* fStations;   //!
  TObjArray* fAbsorbers;  //!
  TGeoCone* fMuchCave;    //!

  // Input parameters
  Double_t fMuchZ1;            // MuchCave Zin position [cm]
  Double_t fAcceptanceTanMin;  // Acceptance tangent min
  Double_t fAcceptanceTanMax;  // Acceptance tangent max
  Int_t fNabs;                 // Number of absorbers
  Int_t fNst;                  // Number of stations

  // GEM module parameters
  Double_t fActiveLx;  // Active volume lx [cm]
  Double_t fActiveLy;  // Active volume ly [cm]
  Double_t fActiveLz;  // Active volume lz [cm]
  Double_t fSpacerLx;  // Spacer lx [cm]
  Double_t fSpacerLy;  // Spacer ly [cm]
  Double_t fOverlapY;  // Overlap along y axis [cm]

  // Straw module parameters
  Double_t fStrawLz;

  // Sector-type module parameters
  TArrayI fNSectorsPerLayer;  // Number of sectors per layer
  Double_t fActiveLzSector;   // Active volume thickness [cm]
  Double_t fSpacerR;          // Spacer width in R [cm]
  Double_t fSpacerPhi;        // Spacer width in Phi [cm]
  Double_t fOverlapR;         // Overlap in R direction [cm]

  TArrayD fAbsorberZ1;   // Absorber Zin position [cm] in the cave reference frame
  TArrayD fAbsorberLz;   // Absorber thickness [cm]
  TArrayC fAbsorberMat;  // Absorber material
  TArrayD fStationZ0;    // Station Zceneter [cm] in  the cave reference frame
  TArrayI fNlayers;      // Number of layers
  TArrayI fDetType;      // Detector type
  TArrayD fLayersDz;     // Distance between layers [cm]
  TArrayD fSupportLz;    // Support thickness [cm]
  TArrayI fModuleDesign; /* 1 - detailed design (modules at two sides)
     * 0 - simple design (1 module per layer)     */

  CbmMuchStation* muchSt;
  CbmMuchLayer* muchLy;
  CbmMuchLayerSide* muchLySd;
  Double_t Rmin;
  Double_t Rmax;
  Double_t Dx2;

  CbmMuchGeoScheme(const CbmMuchGeoScheme&);
  CbmMuchGeoScheme& operator=(const CbmMuchGeoScheme&);

  ClassDef(CbmMuchGeoScheme, 1);
};

#endif

/* Copyright (C) 2010-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev, David Emschermann, Florian Uhlig [committer] */

/**
 * \file CbmTrdGeoHandler.h
 * \brief Helper class to extract information from the GeoManager.
 * \author Florian Uhlig <f.uhlig@gsi.de>
 * \date 13/08/2010
 *
 * Updated 20/05/2013 by Andrey Lebedev <andrey.lebedev@gsi.de>
 *
 * Helper class to extract information from the GeoManager which is
 * needed in many other TRD classes. This helper class should be a
 * single place to hold all these functions.
 */

#ifndef CBMTRDGEOHANDLER_H_
#define CBMTRDGEOHANDLER_H_ 1

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t, Bool_t, kFALSE, UInt_t
#include <TObject.h>     // for TObject
#include <TString.h>     // for TString

#include <map>  // for map

class TGeoBBox;
class TGeoHMatrix;
class TGeoPhysicalNode;
class TGeoVolume;

class CbmTrdGeoHandler : public TObject {
public:
  /**
   * \brief Constructor.
   **/
  CbmTrdGeoHandler();

  /**
   * \brief Destructor.
   **/
  ~CbmTrdGeoHandler();

  void Init(Bool_t isSimulation = kFALSE);

  /**
   * \brief Return module address calculated based on the current node in the TGeoManager.
   */
  Int_t GetModuleAddress();

  /**
   * \brief Navigate to node and return module address.
   */
  Int_t GetModuleAddress(const TString& path);

  //   /**
  //   * \brief Return pad orientation of the current node in the TGeoManager.
  //   */
  //   Int_t GetModuleOrientation();

  /**
   * \brief Navigate to node and return pad orientation.
   */
  Int_t GetModuleOrientation(const TString& path);
  // Path has to contain information down to module part,
  // otherwise retrieved information can be wrong.
  // Path examples:
  // /cave_1/trd_v13x_0/layer01_1101/module1_10100101/gas_1
  // /cave_1/trd_v13x_0/layer10_3210/module8_35411098/gas_1


  /**
   * \brief Fill map with information of the gas volumes for each detector
   */
  std::map<Int_t, TGeoPhysicalNode*> FillModuleMap();

  Double_t GetSizeX(const TString& path);
  Double_t GetSizeY(const TString& path);
  Double_t GetSizeZ(const TString& path);
  Double_t GetX(const TString& path);
  Double_t GetY(const TString& path);
  Double_t GetZ(const TString& path);
  Int_t GetModuleType(const TString& path);
  /** \brief Navigate to node and return the radiator type build in the geometry based on the naming convention.
   * @return radiator type ID or -1 if the radiator is not installed
   */
  Int_t GetRadiatorType(const TString& path);

  // for backward compatibility
  Int_t GetStation(const TString& path);
  Int_t GetLayer(const TString& path);
  Int_t GetModuleCopyNr(const TString& path);

private:
  void NavigateTo(const TString& path);

  Bool_t fIsSimulation;  //!

  UInt_t fGeoPathHash;         //!
  TGeoVolume* fCurrentVolume;  //!
  TGeoBBox* fVolumeShape;      //!
  Double_t fGlobal[3];         //! Global center of volume
  TGeoHMatrix* fGlobalMatrix;  //!
  Int_t fLayerId;              //!
  Int_t fModuleId;             //!
  Int_t fModuleType;           //!
  Int_t fRadiatorType;         //! radiator + chamber entrance window type 
  Int_t fRotation;             //! rotation angle 0,1,2,3

  // for backward compatibility
  Int_t fStation;     //! StationTypeID, 1..3
  Int_t fLayer;       //! LayerID within station, 1..4
  Int_t fModuleCopy;  //! ModuleCopyID with module type

  CbmTrdGeoHandler(const CbmTrdGeoHandler&);
  CbmTrdGeoHandler operator=(const CbmTrdGeoHandler&);

  ClassDef(CbmTrdGeoHandler, 5)
};

#endif  //CBMTRDGEOHANDLER_H

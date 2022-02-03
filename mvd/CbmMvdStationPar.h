/* Copyright (C) 2014-2017 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                      CbmMvdStationPar header file             -----
// -----                  Created 28/10/14 by P.Sitzmann               -----
// -------------------------------------------------------------------------


/** CbmMvdStationPar.h
 *@author P.Sitzmann <p.sitzmann@gsi.de>
 **
 ** Parameter class for the CbmMvdDetector class.
 ** It holds the parameters necessary for tracking.
 **/


#ifndef CBMMVDSTATIONPAR_H
#define CBMMVDSTATIONPAR_H 1


#include "TNamed.h"

#include <map>

class CbmMvdStationPar : public TNamed {

public:
  /** Default constructor **/
  CbmMvdStationPar();

  /** Destructor **/
  virtual ~CbmMvdStationPar();

  void Init(Int_t nrOfStations);

  /** Accessors **/
  Int_t GetStationCount() const { return fStationCount; };
  Double_t GetZPosition(Int_t stationNumber) const;
  Double_t GetZThickness(Int_t stationNumber) const;
  Double_t GetHeight(Int_t stationNumber) const;
  Double_t GetWidth(Int_t stationNumber) const;
  Double_t GetXRes(Int_t stationNumber) const;
  Double_t GetYRes(Int_t stationNumber) const;
  Double_t GetZRadThickness(Int_t stationNumber) const;
  Double_t GetBeamHeight(Int_t stationNumber) const;
  Double_t GetBeamWidth(Int_t stationNumber) const;

  /** Data interface */
  void AddZPosition(Int_t stationNumber, Double_t z, Double_t zThickness);
  void AddHeight(Int_t stationNumber, Double_t height);
  void AddWidth(Int_t stationNumber, Double_t width);
  void AddXRes(Int_t stationNumber, Double_t xres);
  void AddYRes(Int_t stationNumber, Double_t yres);
  void AddZRadThickness(Int_t stationNumber, Double_t length);
  void AddBeamHeight(Int_t stationNumber, Double_t beamheight);
  void AddBeamWidth(Int_t stationNumber, Double_t beamwidth);

  /** Output to screen **/
  void Print(Option_t* opt = "") const;

private:
  Double_t GetParameter(const std::vector<Double_t>& parArray, Int_t iStation) const;
  void SetParameterMax(std::vector<Double_t>& parArray, Int_t iStation, Double_t value);
  void SetParameterMin(std::vector<Double_t>& parArray, Int_t iStation, Double_t value);

  Int_t fStationCount {-1};  // Number of Stations, station numbering starts at 0!!!

  std::vector<Double_t> fZPositions {};     // map of the z positions of all Stations
  std::vector<Double_t> fZPositionMin {};   // a helper array for filling the fZPositions
  std::vector<Double_t> fZPositionMax {};   // a helper array for filling the fZPositions
  std::vector<Double_t> fZThicknesses {};   // in cm
  std::vector<Double_t> fHeights {};        // in cm
  std::vector<Double_t> fWidths {};         // in cm
  std::vector<Double_t> fXResolutions {};   // in mu m
  std::vector<Double_t> fYResolutions {};   // in mu m
  std::vector<Double_t> fZRadThickness {};  // Z thickness [in radiation lengths]
  std::vector<Double_t> fBeamHeights {};    // in cm
  std::vector<Double_t> fBeamWidths {};     // in cm

  ClassDef(CbmMvdStationPar, 2);
};


#endif

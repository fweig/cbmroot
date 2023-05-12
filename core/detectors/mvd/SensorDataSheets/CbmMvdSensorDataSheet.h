/* Copyright (C) 2014-2015 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Michael Deveaux, Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                      CbmMvdSensorDataSheet header file                -----
// -----                  Created 02/12/08  by M. Deveaux             -----
// -------------------------------------------------------------------------


/** CbmMvdSensorDataSheet.h
 ** @author M.Deveaux <deveaux@physik.uni-frankfurt.de>
 **
 ** Data base for different MVD sensors
 ** Base class to be modified 
 **/


#ifndef CBMMVDSENSORDATASHEET_H
#define CBMMVDSENSORDATASHEET_H 1

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, Float_t, Bool_t, Option_t
#include <TMath.h>       // for TMath::Power
#include <TNamed.h>      // for TNamed
#include <TString.h>     // for TString

#include <string>  // for string
class TBuffer;
class TClass;
class TMemberInspector;

class CbmMvdSensorDataSheet : public TNamed {

public:
  /** Default constructor **/
  CbmMvdSensorDataSheet() = default;
  virtual void Print(Option_t* opt = "") const;
  std::string ToString() const;

  /** Destructor **/
  ~CbmMvdSensorDataSheet() = default;

protected:
  /** Technical data of the sensor */
  TString fMimosaName = "DefaulSensor"; // Clear name of the simulated sensor
  Double_t fPixelPitchX = 18.4e-4;      // Pixel pitch of this sensor
  Double_t fPixelPitchY = 18.4e-4;      // Pixel pitch of this sensor
  Int_t fNPixelsX = 0;                  // Number of pixels in row
  Int_t fNPixelsY = 0;                  // Number of pixels in col
  Int_t fNPixels = 0;                   // Number of pixels in sensor
  Int_t fPixelSignX = 1;                // Direction of the pixel count X.
    // Defined according to pixelNumberX=fPixelSign * const * x
  Int_t fPixelSignY = 1;                // Direction of the pixel count Y.
    // Defined according to pixelNumberY=fPixelSign * const * y
  Int_t fShutterSign = 0;               // Direction of the rolling shutter.
                              // 1 => parallel to y-axis, -1 => anti-parallel
  Double_t fIntegrationTime = 50e3;  // Integration time of the pixels
  Double_t fEpiTh = 14e-4;            // Thickness of the epitaxial layer

  Double_t fChargeThreshold = 0.;
  Double_t fSignalRiseTime = nan("NotSet");
  Double_t fSignalFallTime = nan("NotSet");


  /** Description of the sensor for the digitizer **/
  Double_t fNoise = 0.;        // Noise of the sensor
  Double_t fLandauMPV = 8.62131e+02;    // Landau for digitizer
  Double_t fLandauSigma = 2.e+02;  // Landau for digitizer
  Double_t fLandauGain = 1.56;   // Landau for digitizer
  Double_t fLorentzPar0 = 4.12073e+02;  // Lorentz for digitizer
  Double_t fLorentzPar1 = 0.8e+00;  // Lorentz for digitizer
  Double_t fLorentzPar2 = 0.1;  // Lorentz for digitizer

  /** ADC description, relevant for sensors with analog readout (not present in the CbmMvd) **/

  Int_t fAdcDynamic = 150;
  Int_t fAdcOffset = 0;
  Int_t fAdcBits = 1;
  Int_t fAdcSteps = TMath::Power(2, fAdcBits);
  Float_t fAdcStepSize = fAdcDynamic / fAdcSteps;

  /** Sensor substructure, relevant for MIMOSA-26 (Prototype of the CbmMvd) **/
  Int_t fStatesPerBank = 0;
  Int_t fStatesPerLine = 0;
  Int_t fStatesPerFrame = 0;
  Int_t fPixelsPerBank = 0;

  /** Threshold of the in-pixel discriminator. Relevant for MIMOSIS **/
  Int_t fAnalogThreshold = -1;


  /** Self-organizsation **/

  Bool_t fValidData = kFALSE;


public:
  virtual TString* GetSensorName() { return &fMimosaName; };
  virtual Double_t GetPixelPitchX() { return fPixelPitchX; };
  virtual Double_t GetPixelPitchY() { return fPixelPitchY; };
  virtual Int_t GetNPixelsX() { return fNPixelsX; };
  virtual Int_t GetNPixelsY() { return fNPixelsY; };
  virtual Int_t GetNPixels() { return fNPixels; };
  virtual Int_t GetPixelSignX() { return fPixelSignX; };
  virtual Int_t GetPixelSignY() { return fPixelSignY; };
  virtual Int_t GetShutterSign() { return fShutterSign; };
  virtual Double_t GetIntegrationTime() { return fIntegrationTime; };
  virtual Double_t GetEpiThickness() { return fEpiTh; }
  virtual Double_t GetNoise() { return fNoise; };
  virtual Double_t GetSignalRiseTime() { return fSignalRiseTime; };
  virtual Double_t GetSignalFallTime() { return fSignalFallTime; };


  /** Description of the sensor for the digitizer **/
  virtual Double_t GetLandauMPV() { return fLandauMPV; };      // Landau for digitizer
  virtual Double_t GetLandauSigma() { return fLandauSigma; };  // Landau for digitizer
  virtual Double_t GetLandauGain() { return fLandauGain; };    // Landau for digitizer
  virtual Double_t GetLorentzPar0() { return fLorentzPar0; };  // Lorentz for digitizer
  virtual Double_t GetLorentzPar1() { return fLorentzPar1; };  // Lorentz for digitizer
  virtual Double_t GetLorentzPar2() { return fLorentzPar2; };  // Lorentz for digitizer


  virtual Double_t GetChargeThreshold() { return fChargeThreshold; };
  virtual Int_t GetAnalogThreshold() { return fAnalogThreshold; };


  /** ADC description **/

  virtual Int_t GetAdcDynamic() { return fAdcDynamic; };
  virtual Int_t GetAdcOffset() { return fAdcOffset; };
  virtual Int_t GetAdcBits() { return fAdcBits; };
  virtual Int_t GetAdcSteps() { return fAdcSteps; };
  virtual Float_t GetAdcStepSize() { return fAdcStepSize; };


  ClassDef(CbmMvdSensorDataSheet, 1);
};


#endif

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
#include <TNamed.h>      // for TNamed
#include <TString.h>     // for TString

#include <string>  // for string
class TBuffer;
class TClass;
class TMemberInspector;

class CbmMvdSensorDataSheet : public TNamed {

public:
  /** Default constructor **/
  CbmMvdSensorDataSheet();
  virtual void Print(Option_t* opt = "") const;
  std::string ToString() const;

  /** Destructor **/
  ~CbmMvdSensorDataSheet();

protected:
  /** Technical data of the sensor */


  TString fMimosaName;    // Clear name of the simulated sensor
  Double_t fPixelPitchX;  // Pixel pitch of this sensor
  Double_t fPixelPitchY;  // Pixel pitch of this sensor
  Int_t fNPixelsX;        // Number of pixels in row
  Int_t fNPixelsY;        // Number of pixels in col
  Int_t fNPixels;         // Number of pixels in sensor
  Int_t fPixelSignX;      // Direction of the pixel count X.
    // Defined according to pixelNumberX=fPixelSign * const * x
  Int_t fPixelSignY;  // Direction of the pixel count Y.
    // Defined according to pixelNumberY=fPixelSign * const * y
  Int_t fShutterSign;         // Direction of the rolling shutter.
                              // 1 => parallel to y-axis, -1 => anti-parallel
  Double_t fIntegrationTime;  // Integration time of the pixels
  Double_t fEpiTh;            // Thickness of the epitaxial layer

  Double_t fChargeThreshold;
  Double_t fSignalRiseTime;
  Double_t fSignalFallTime;


  /** Description of the sensor for the digitizer **/
  Double_t fNoise;        // Noise of the sensor
  Double_t fLandauMPV;    // Landau for digitizer
  Double_t fLandauSigma;  // Landau for digitizer
  Double_t fLandauGain;   // Landau for digitizer
  Double_t fLorentzPar0;  // Lorentz for digitizer
  Double_t fLorentzPar1;  // Lorentz for digitizer
  Double_t fLorentzPar2;  // Lorentz for digitizer

  /** ADC description, relevant for sensors with analog readout (not present in the CbmMvd) **/

  Int_t fAdcDynamic;
  Int_t fAdcOffset;
  Int_t fAdcBits;
  Int_t fAdcSteps;
  Float_t fAdcStepSize;

  /** Sensor substructure, relevant for MIMOSA-26 (Prototype of the CbmMvd) **/
  Int_t fStatesPerBank;
  Int_t fStatesPerLine;
  Int_t fStatesPerFrame;
  Int_t fPixelsPerBank;

  /** Threshold of the in-pixel discriminator. Relevant for MIMOSIS **/
  Int_t fAnalogThreshold;


  /** Self-organizsation **/

  Bool_t fValidData;


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

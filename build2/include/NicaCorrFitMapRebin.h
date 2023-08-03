/*
 * NicaCorrFitMapRebin.h
 *
 *  Created on: 8 gru 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFITMAPREBIN_H_
#define NICACORRFITMAPREBIN_H_

#include <TObject.h>

#include "NicaArray.h"

/**
 * class for compression of multidimensional maps
 */

class TFile;
class TTree;
class NicaCorrFitInfo;
class NicaDividedHisto1D;
class NicaDividedHisto3D;
class NicaCorrFitMapRebin : public TObject {
  TFile* fInFile;
  TFile* fOutFile;
  TTree* fInTree;
  TTree* fOutTree;
  NicaCorrFitInfo* fInfo;
  TString fInFileName;
  TString fOutFileName;
  Int_t fRebinX, fRebinY, fRebinZ;
  Bool_t Rebin1D();
  Bool_t Rebin3D();

public:
  NicaCorrFitMapRebin();
  /**
   *
   * @param inFile name of the input file
   * @param nbins rebinning value (the same for all axes)
   */
  NicaCorrFitMapRebin(TString inFile, Int_t nbins);
  /** set rebin separatelly for each axis **/
  void SetRebin3D(Int_t binsX, Int_t binsY, Int_t binsZ) {
    fRebinX = binsX;
    fRebinY = binsY;
    fRebinZ = binsZ;
  }
  /**
   * compress map and write to file
   * @param outFile file with compressed map
   * @return status of compression (false if some problem occured)
   */
  Bool_t Compress(TString outFile);
  virtual ~NicaCorrFitMapRebin();
  NicaCorrFitMapRebin& operator=(const NicaCorrFitMapRebin& other) = delete;
  ClassDef(NicaCorrFitMapRebin, 1)
};


#endif /* NICACORRFITMAPREBIN_H_ */

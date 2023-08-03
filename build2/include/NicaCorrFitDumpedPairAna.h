/*
 * NicaFemtoNDimMap.h
 *
 *  Created on: 24 paź 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTONDIMMAP_H_
#define NICAFEMTONDIMMAP_H_

#include <TObject.h>
#include <TString.h>

#include "NicaFemtoFreezoutGenerator.h"

/**
 * basic class for generation of multidim maps
 */


class NicaCorrFitDatabase;
class NicaFemto1DCF;
class NicaFemto3DCF;
class NicaFemtoCorrFunc;
class NicaFemtoMicroPair;
class NicaFemtoPair;
class NicaFemtoSHCF;
class NicaFemtoWeightGenerator;
class TFile;
class TTree;
class TClonesArray;
/**
 * analysis of pairs created by NicaFemtoDumpPairAna
 */

class NicaCorrFitDumpedPairAna : public TObject {
private:
  TString fPairFile;
  TFile* fFile;
  TTree* fTree;
  Int_t fJobId;
  Int_t fMultiply;
  Bool_t fIgnoreSing;
  NicaCorrFitDatabase* fDB;
  NicaFemtoCorrFunc* fCF;
  TClonesArray* fPairsSignal;
  TClonesArray* fPairsBackground;
  NicaFemtoPair* fPair;
  NicaFemtoMicroPair* fMiniPair;
  NicaFemtoFreezoutGenerator* fGenerator;
  NicaFemtoWeightGenerator* fWeight;
  enum eCalcMode { kPerf = 0, kMixed = 1 };
  eCalcMode fMode;
  /** export CF to root tree, this is used for compression of data, you can set
   * value of bin to -1, then such bin will not be used **/
  void RootExport1D(NicaFemto1DCF* cf);
  void RootExport3D(NicaFemto3DCF* cf);
  void RootExportSH(NicaFemtoSHCF* cf);

public:
  NicaCorrFitDumpedPairAna(Int_t jobid = -1);
  void SetGenerator(const NicaFemtoFreezoutGenerator& gen) { fGenerator = gen.MakeCopy(); };
  void SetCorrFunc(const NicaFemtoCorrFunc& func);
  void SetWeightGenerator(const NicaFemtoWeightGenerator& weight);
  void UseMixed() { fMode = kMixed; };
  Bool_t Init();
  void Run(Int_t maxEvents = -1);
  void Finish();
  void IgnoreSign() { fIgnoreSing = kTRUE; };
  void SetMultiplyFactor(Int_t m) { fMultiply = m; };
  void SetJobID(Int_t jobId) { fJobId = jobId; };
  void SetPairFile(TString pairFile) { fPairFile = pairFile; };
  virtual void Print(Option_t* option = "") const;
  virtual void PreprocessPair() {};
  virtual void PreprocessMixedPair() {};
  virtual ~NicaCorrFitDumpedPairAna();
  ClassDef(NicaCorrFitDumpedPairAna, 1)
};

#endif /* NICAFEMTO_ANALYSIS_FEMTO_CORRFIT_MAPGENERATORS_NICAFEMTONDIMMAP_H_                                                     \
        */

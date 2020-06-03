#ifndef CBMFASTSIM_H
#define CBMFASTSIM_H 1


#include "FairTask.h"
#include "TVector3.h"
#include "TMatrixD.h"
#include <string>
#include <list>
#include "TString.h"
#include "TClonesArray.h"
#include "TH3.h"
#include "TH2.h"
#include "TH1.h"

class TObjectArray;
class TRandom3;
class TLorentzVector;
class TParticle;
class TF1;
class TH2;
class TProfile3D;
class THnBase;
class TDatabasePDG;
class PairAnalysisFunction;

class CbmFastSim : public FairTask
{

 public:
  enum EParticleType {
    kEle = 0,
    kPio,
    kKao,
    kPro,
    kMuo,
    kGam
  };

  enum EEfficiencyMethod {
    kIgnoreFluct = 0,
    kInterpolate,
    kAverage,
    kFactorize,
    kLastDim
  };

  /** Default constructor **/
  CbmFastSim(bool persist=true);

  /** Destructor **/
  ~CbmFastSim();

  void  Register();

  /** Virtual method Init **/
  virtual InitStatus Init();
  virtual void InitTask();

  virtual void Finish();


  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  void SetLookupEfficiency(  THnBase *nom, THnBase *denom, EParticleType part);
  void SetLookupResolutionP( TH2F *obj, EParticleType part) { fRFp[part] = obj; }
  void SetEfficiencyMethod( EEfficiencyMethod method)       { fMethod = method; }

  void SetSeed(unsigned int seed=65539);

 private:

  TRandom3  *fRand;
  TDatabasePDG* fdbPdg;
  static const Int_t fgkNParts = 8;      // numer of different particle species
  EEfficiencyMethod fMethod = kIgnoreFluct;  // efficiency caluclations

  /** function for selection and smearing **/
  Bool_t PassEfficiencyFilter(Int_t pdg, Double_t *vals, Int_t *coord);
  Double_t GetEfficiency( Int_t idx, Double_t *vals, Int_t *coord);

  Bool_t Smearing(TLorentzVector *p4, Int_t pdg);
  Double_t AnalyzeMap(TH2F *map, Double_t refValue);

  virtual Bool_t IsSelected(TObject *sel, Int_t opt);

  /** Output array of Candidates **/
  TClonesArray* fFastTracks = NULL;

  /** input objects **/
  THnBase    *fEFF[fgkNParts]        ;         // efficiency(nominator) map
  THnBase    *fEFFdenom[fgkNParts]   ;         // denominator map
  TH1        *fEFFproj[fgkNParts][10];         // projections (method dependent)

  TH2F       *fRFp[fgkNParts]        ;         // smearing matrix

  /** Get parameter containers **/
  virtual void SetParContainers();

  CbmFastSim(const CbmFastSim &obj);
  CbmFastSim& operator = (const CbmFastSim &obj);

  ClassDef(CbmFastSim,1);

};

#endif

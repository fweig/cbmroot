#ifndef PAIRANALYSISSIGNALMC_H
#define PAIRANALYSISSIGNALMC_H

/* Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

#include <TNamed.h>
#include <TMCProcess.h>
#include <TSpline.h>
//#include <TGraph.h>
//#include <TF1.h>


//__________________________________________________________________
class PairAnalysisSignalMC : public TNamed {
  
 public:
  enum EBranchRelation {kUndefined=0, kSame, kDifferent};
  enum ESource {kDontCare=0, kPrimary, kSecondary, kFinalState, kDirect, kSecondaryFromWeakDecay, kSecondaryFromMaterial};
  enum EDalitz {      kWhoCares=0, kIsDalitz, kIsNotDalitz};
  enum EDefinedSignal {kInclJpsi=0, kNonRadJpsi, kRadJpsi, kPsi2S,
		       kConversion,
		       kRho0, kOmegaDalitz, kOmega, kPhi,
		       kEtaDalitz, kEta,
		       kPi0Dalitz, kPi0Gamma, kPi0,
		       kK0Short, kLambda,
		       kInclElePM,
		       kDeltaElectron,
		       kPrimElectron, kPrimMuon, kPrimPion, kPrimKaon, kPrimProton,
		       kDeuteron, kTriton, kHe3, kAlpha,
		       kNSignals};
  
  PairAnalysisSignalMC();
  PairAnalysisSignalMC(const Char_t* name, const Char_t* title);
  PairAnalysisSignalMC(EDefinedSignal defaultSignal);
  virtual ~PairAnalysisSignalMC();
  
  void SetLegPDGs(Int_t pdg1, Int_t pdg2, Bool_t exclude1=kFALSE, Bool_t exclude2=kFALSE)
    {fLeg1 = pdg1; fLeg2 = pdg2; fLeg1Exclude=exclude1; fLeg2Exclude=exclude2;}
  void SetMotherPDGs(Int_t pdg1, Int_t pdg2, Bool_t exclude1=kFALSE, Bool_t exclude2=kFALSE)
    {fMother1 = pdg1; fMother2 = pdg2; fMother1Exclude=exclude1; fMother2Exclude=exclude2;}
  void SetGrandMotherPDGs(Int_t pdg1, Int_t pdg2, Bool_t exclude1=kFALSE, Bool_t exclude2=kFALSE)
    {fGrandMother1 = pdg1; fGrandMother2 = pdg2; fGrandMother1Exclude=exclude1; fGrandMother2Exclude=exclude2;}
  void SetGreatGrandMotherPDGs(Int_t pdg1, Int_t pdg2, Bool_t exclude1=kFALSE, Bool_t exclude2=kFALSE)
    {fGreatGrandMother1 = pdg1; fGreatGrandMother2 = pdg2; fGreatGrandMother1Exclude=exclude1; fGreatGrandMother2Exclude=exclude2;}
  void SetLegSources(ESource s1, ESource s2)                       {fLeg1Source = s1;                      fLeg2Source = s2;}
  void SetMotherSources(ESource s1, ESource s2)                    {fMother1Source = s1;                   fMother2Source = s2;}
  void SetGrandMotherSources(ESource s1, ESource s2)               {fGrandMother1Source = s1;              fGrandMother2Source = s2;}
  void SetCheckBothChargesLegs(Bool_t flag1, Bool_t flag2)         {fCheckBothChargesLeg1 = flag1;         fCheckBothChargesLeg2 = flag2;}
  void SetCheckBothChargesMothers(Bool_t flag1, Bool_t flag2)      {fCheckBothChargesMother1 = flag1;      fCheckBothChargesMother2 = flag2;}
  void SetCheckBothChargesGrandMothers(Bool_t flag1, Bool_t flag2) {fCheckBothChargesGrandMother1 = flag1; fCheckBothChargesGrandMother2 = flag2;}
  void SetMothersRelation(EBranchRelation relation)                {fMothersRelation = relation;}
  void SetGEANTProcess(TMCProcess processID, Bool_t check=kTRUE)  {fGEANTProcess = processID; fCheckGEANTProcess=check;}
  //  void SetWeight(TSpline3 *wghts, UInt_t var)                      { if(&fWeights) delete &fWeights; fWeights = *wghts; fType=var; }
  void SetWeight(TSpline3 *wghts, UInt_t var)                      { if(fWeights) delete fWeights; fWeights = wghts; fType=var; }
  void SetWeight(Double_t wght)                                    {fWeight=wght;}
  void SetFillPureMCStep(Bool_t fill=kTRUE)                        {fFillPureMCStep = fill;}
  void SetIsSingleParticle(Bool_t fill=kTRUE)                      {fIsSingleParticle = fill;}

  Int_t GetLegPDG(Int_t branch)                        const {return (branch==1 ? fLeg1 : fLeg2);}
  Int_t GetMotherPDG(Int_t branch)                     const {return (branch==1 ? fMother1 : fMother2);}
  Int_t GetGrandMotherPDG(Int_t branch)                const {return (branch==1 ? fGrandMother1 : fGrandMother2);}
  Int_t GetGreatGrandMotherPDG(Int_t branch)                const {return (branch==1 ? fGreatGrandMother1 : fGreatGrandMother2);}
  Bool_t GetLegPDGexclude(Int_t branch)                const {return (branch==1 ? fLeg1Exclude : fLeg2Exclude);}
  Bool_t GetMotherPDGexclude(Int_t branch)             const {return (branch==1 ? fMother1Exclude : fMother2Exclude);}
  Bool_t GetGrandMotherPDGexclude(Int_t branch)        const {return (branch==1 ? fGrandMother1Exclude : fGrandMother2Exclude);}
  Bool_t GetGreatGrandMotherPDGexclude(Int_t branch)        const {return (branch==1 ? fGreatGrandMother1Exclude : fGreatGrandMother2Exclude);}
  ESource GetLegSource(Int_t branch)                   const {return (branch==1 ? fLeg1Source : fLeg2Source);}
  ESource GetMotherSource(Int_t branch)                const {return (branch==1 ? fMother1Source : fMother2Source);}
  ESource GetGrandMotherSource(Int_t branch)           const {return (branch==1 ? fGrandMother1Source : fGrandMother2Source);}
  Bool_t GetCheckBothChargesLegs(Int_t branch)         const {return (branch==1 ? fCheckBothChargesLeg1 : fCheckBothChargesLeg2);}
  Bool_t GetCheckBothChargesMothers(Int_t branch)      const {return (branch==1 ? fCheckBothChargesMother1 : fCheckBothChargesMother2);}
  Bool_t GetCheckBothChargesGrandMothers(Int_t branch) const {return (branch==1 ? fCheckBothChargesGrandMother1 : fCheckBothChargesGrandMother2);}
  Bool_t GetCheckBothChargesGreatGrandMothers(Int_t branch) const {return (branch==1 ? fCheckBothChargesGreatGrandMother1 : fCheckBothChargesGreatGrandMother2);}
  EBranchRelation GetMothersRelation()                 const {return fMothersRelation;}
  TMCProcess GetGEANTProcess()                         const {return fGEANTProcess;}
  Bool_t GetCheckGEANTProcess()                        const {return fCheckGEANTProcess;}
  Double_t GetWeight()                                 const {return fWeight;}
  //  Double_t GetWeight(Double_t *const values)           const {return (&fWeights ? fWeights.Eval(values[fType]) : fWeight); }
  //  Double_t GetWeight(Double_t *const values)           const {return (fType>0 ? fWeights->Eval(values[fType]) : fWeight); }
  // TODO: workaround
  Double_t GetWeight(Double_t *const values)           const;// {return (fType>0 ? fWeights->Eval(values[fType]) : fWeight); }
  Bool_t GetFillPureMCStep()                           const {return fFillPureMCStep;}
  Bool_t IsSingleParticle()                            const {return fIsSingleParticle; }

  void SetIsDalitz(EDalitz dal, Int_t pdg=0) { fDalitz=dal; fDalitzPdg=pdg;   }
  EDalitz GetDalitz() const   { return fDalitz; }
  Int_t   GetDalitzPdg()   const   { return fDalitzPdg; }

  static const char* fgkSignals[kNSignals][2];  //default signal names+titles

  void Print(Option_t * option = "") const;

 private:
  // PDG codes for legs, mothers and grand-mothers
  Int_t fLeg1                = 0;                        // leg 1 PDG
  Int_t fLeg2                = 0;                        // leg 2 PDG
  Int_t fMother1             = 0;                     // mother 1 PDG
  Int_t fMother2             = 0;                     // mother 2 PDG
  Int_t fGrandMother1        = 0;                // grandmother 1 PDG
  Int_t fGrandMother2        = 0;                // grandmother 2 PDG
  Int_t fGreatGrandMother1   = 0;          // great grandmother 1 PDG
  Int_t fGreatGrandMother2   = 0;          // great grandmother 2 PDG

  // Toggle on/off the use of the PDG codes as inclusion or exclusion
  // Example: if fLeg1=211 and fLeg1Exclude=kTRUE than all codes will be accepted for leg 1 with
  //          the exception of 211 (pions)
  Bool_t fLeg1Exclude               = kFALSE;                // leg 1
  Bool_t fLeg2Exclude               = kFALSE;                // leg 2
  Bool_t fMother1Exclude            = kFALSE;             // mother 1
  Bool_t fMother2Exclude            = kFALSE;             // mother 2
  Bool_t fGrandMother1Exclude       = kFALSE;        // grandmother 1
  Bool_t fGrandMother2Exclude       = kFALSE;        // grandmother 2
  Bool_t fGreatGrandMother1Exclude  = kFALSE;  // great grandmother 1
  Bool_t fGreatGrandMother2Exclude  = kFALSE;  // great grandmother 2

  // Particle sources
  ESource fLeg1Source               = kDontCare;                // leg 1 source
  ESource fLeg2Source               = kDontCare;                // leg 2 source
  ESource fMother1Source            = kDontCare;             // mother 1 source
  ESource fMother2Source            = kDontCare;             // mother 2 source
  ESource fGrandMother1Source       = kDontCare;        // grandmother 1 source
  ESource fGrandMother2Source       = kDontCare;        // grandmother 2 source

  // Flaggs whether to check both charges of a given PDG code
  Bool_t fCheckBothChargesLeg1              = kFALSE; // check both charges of the legs pdg
  Bool_t fCheckBothChargesLeg2              = kFALSE; //                leg2
  Bool_t fCheckBothChargesMother1           = kFALSE; //                mother 1
  Bool_t fCheckBothChargesMother2           = kFALSE; //                mother 2
  Bool_t fCheckBothChargesGrandMother1      = kFALSE; //              grand mother 1
  Bool_t fCheckBothChargesGrandMother2      = kFALSE; //              grand mother 2
  Bool_t fCheckBothChargesGreatGrandMother1 = kFALSE; //         great grand mother 1
  Bool_t fCheckBothChargesGreatGrandMother2 = kFALSE; //         great grand mother 2
  Bool_t fCheckGEANTProcess                 = kFALSE; //              GEANT process

  EBranchRelation fMothersRelation          = kUndefined;   // mother 1&2 relation (same, different or whatever)
  TMCProcess fGEANTProcess                  = kPPrimary;    // GEANT process ID (see roots TMCProcess)
  Double_t fWeight                          = 1.;           // weighting factor
  TSpline3   *fWeights                      = NULL;         //> weighting factors
  UInt_t   fType                            = 0;            // lookup variable for weighting factors

  // dalitz decays
  EDalitz fDalitz                           = kWhoCares;    // check for dalitz decay
  Int_t fDalitzPdg                          = 0;            // dalitz PDG

  Bool_t fFillPureMCStep                    = kFALSE;       // check and fill the pure MC step
  Bool_t fIsSingleParticle                  = kFALSE;       // single particle MC signal such as e,pi,K,p

  PairAnalysisSignalMC(const PairAnalysisSignalMC &c);
  PairAnalysisSignalMC &operator=(const PairAnalysisSignalMC &c);
  
  ClassDef(PairAnalysisSignalMC,5); // MC signal defintions
};

#endif

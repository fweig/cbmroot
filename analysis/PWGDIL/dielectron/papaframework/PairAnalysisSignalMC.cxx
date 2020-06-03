///////////////////////////////////////////////////////////////////////////
//                MC signal description container
//                                               
// Authors:                                                              
//   * Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
//   Julian Book   <Julian.Book@cern.ch>                                 
/*
 A container to describe the decay of a two (three) body process
 
 TODO: update and add default signals, single particle signals, geantid!

 Monte Carlo signal definition for a two body decay:
      Leg #1  <-- Mother #1  <--  Grandmother #1  <--  GreatGrandmother #1
                      |
    ( Leg #2  <-- Mother #2  <--  Grandmother #2  <--  GreatGrandmother #2 ) 
  
   For every leg, mother, grand-mother or great grand-mother a PDG code and a source can be specified.
     SetLegPDGs, SetMotherPDGs, SetGrandMotherPDGs, SetGreatGrandMotherPDGs
     
   1.) For the PDG codes, the PYTHIA standard is used. 
   A few non-existent PYTHIA codes are used to select more than one PYTHIA code. All these are described below
   and implemented in PairAnalysisMC::ComparePDG() function:
      0 - default, accepts all PYTHIA codes
    100 - light unflavoured mesons in the code range 100-199
    200 -        --"--                               200-299
    300 - strange mesons in the code range           300-399
    400 - charmed mesons in the code range           400-499
    401 - open charm mesons (all D and D* mesons)    400-439
    402 - open charm mesons and baryons together     400-439, 4000-4399
    403 - all charm hadrons (mesons and baryons)     400-499, 4000-4999
    500 - beauty mesons in the code range            500-599
    501 - open beauty mesons                         500-549
    502 - open beauty mesons and baryons             500-549, 5000-5499
    503 - all beauty hadrons                         500-599, 5000-5999
    902 - all open charm open beauty mesons+baryons  400-439, 500-549, 4000-4399, 5000-5499
   1000 - light unflavoured baryons in the code range 1000-1999
   2000 -        --"--                                2000-2999
   3000 - strange baryons in the code range           3000-3999
   4000 - charmed baryons in the code range           4000-4999
   4001 - open charm baryons                          4000-4399
   5000 - beauty baryons in the code range            5000-5999
   5001 - open beauty baryons                         5000-5499

   2.) If the exclusion flags are turned ON then the PDG codes required and the conventional codes described above
       are used to exclude the selected particles. 

   3.) If the selection of both charges is switched ON then the PDG codes act on both particles and anti-particles.
         SetCheckBothChargesLegs, SetCheckBothChargesMothers, SetCheckBothChargesGrandMothers

   4.) Particles sources implemented:
       TOADD: geant process id via TMCProcess

   5.) The 2 legs can originate from the same or different mother particles. This can be specified via 
       the SetMothersRelation() method call.

   6.) The filling of the pure MC step can be switched on using SetFillPureMCStep() method call. This should be used
       with care since at the pure MC information level there is no cut applied and for abundant particles the 
       combinatorics can be very high.

 */
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include "PairAnalysisSignalMC.h"
#include "PairAnalysisHelper.h"

ClassImp(PairAnalysisSignalMC)

const char* PairAnalysisSignalMC::fgkSignals[kNSignals][2]= {  //default signal names+titles
  {"InclJpsi",       "J/#psi (incl.)"},
  {"NonRadJpsi",     "J/#psi"},
  {"RadJpsi",        "J/#psi (rad.)"},
  {"Psi2S",          "#psi(2S)"},
  {"Conversion",     "#gamma#rightarrow e^{+}e^{-}"},
  {"Rho0",           "#rho^{0}"},
  {"OmegaDalitz",    "#omega_{Dalitz}"},
  {"Omega",          "#omega"},
  {"Phi",            "#phi"},
  {"EtaDalitz",      "#eta_{Dalitz}"},
  {"Eta",            "#eta"},
  {"Pi0Dalitz",      "#pi^{0}_{Dalitz}"},
  {"Pi0Gamma",       "#pi^{0}"},
  {"Pi0",            "#pi^{0}"},
  {"K0Short",       "K^{0}_{S}"},
  {"Lambda",         "#Lambda"},
  {"InclElePM",      "e^{+}e^{-} (incl.)"},
  {"DeltaElectron",  "#delta rays"},
  {"PrimElectron",   "e (prim.)"},
  {"PrimMuon",       "#mu (prim.)"},
  {"PrimPion",       "#pi (prim.)"},
  {"PrimKaon",       "K (prim.)"},
  {"PrimProton",     "p (prim.)"},
  {"Deuteron",       "d"},
  {"Triton",         "t"},
  {"Helion",         "^{3}He"},
  {"Alpha",          "^{4}He"}
};

//_________________________________________________________________________
PairAnalysisSignalMC::PairAnalysisSignalMC() :
  PairAnalysisSignalMC("PairAnalysisSignalMC", "PairAnalysisSignalMC")
{

  //
  // Default constructor
  //
}


//_________________________________________________________________________
PairAnalysisSignalMC::PairAnalysisSignalMC(const Char_t* name, const Char_t* title) :
  TNamed(name, title)
{

  //
  // Named constructor
  //
}

//_________________________________________________________________________
PairAnalysisSignalMC::PairAnalysisSignalMC(EDefinedSignal defaultSignal) :
  PairAnalysisSignalMC()
{

  //
  // Constructor with default signals
  //
  TString title="";
  switch(defaultSignal) {
  case kInclJpsi:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;  fLeg2=-11;
    fMother1=443; fMother2=443;
    fMothersRelation=kSame;
    break;
  case kRadJpsi:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;  fLeg2=-11;
    fMother1=443; fMother2=443;
    fMothersRelation=kSame;
    fDalitz=kIsDalitz; fDalitzPdg=22;
    break;
  case kNonRadJpsi:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;  fLeg2=-11;
    fMother1=443; fMother2=443;
    fMothersRelation=kSame;
    fDalitz=kIsNotDalitz; fDalitzPdg=22;
    break;
  case kPsi2S:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;  fLeg2=-11;
    fMother1=100443; fMother2=100443;
    fMothersRelation=kSame;
    break;
  case kConversion:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;    fLeg2=-11;
    fMother1=22; fMother2=22;
    fMother1Source=kSecondary; fMother2Source=kSecondary;
    fMothersRelation=kSame;
    // fGrandMother1=111; fGrandMother2=111;
    // fGrandMother1Exclude=kTRUE; fGrandMother1Exclude=111;
    SetGEANTProcess(kPPair);
    break;
  case kRho0:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;    fLeg2=-11;
    fMother1=113; fMother2=113;
    fMothersRelation=kSame;
    SetGEANTProcess(kPPrimary); //pluto
    break;
  case kOmega:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;    fLeg2=-11;
    fMother1=223; fMother2=223;
    fMothersRelation=kSame;
    fDalitz=kIsNotDalitz; fDalitzPdg=111;
    SetGEANTProcess(kPPrimary); //pluto
    break;
  case kOmegaDalitz:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;    fLeg2=-11;
    fMother1=223; fMother2=223;
    fMothersRelation=kSame;
    fDalitz=kIsDalitz; fDalitzPdg=111;
    SetGEANTProcess(kPPrimary); //pluto
    break;
  case kPhi:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;    fLeg2=-11;
    fMother1=333; fMother2=333;
    fMothersRelation=kSame;
    SetGEANTProcess(kPPrimary); //pluto
    break;
  case kEtaDalitz:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;    fLeg2=-11;   fCheckBothChargesLeg1=kTRUE;    fCheckBothChargesLeg2=kTRUE;
    fMother1=221; fMother2=221;
    fMothersRelation=kSame;
    fDalitz=kIsDalitz; fDalitzPdg=22;
    //    SetGEANTProcess(kPPrimary); //pluto
    break;
  case kEta:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;    fLeg2=-11;
    fMother1=221; fMother2=221;
    fMothersRelation=kSame;
    //    SetGEANTProcess(kPUserDefined);
    break;
  case kPi0:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;    fLeg2=-11;
    fMother1=111; fMother2=111;
    fMothersRelation=kSame;
    //    SetGEANTProcess(kPUserDefined);
    break;
  case kPi0Gamma:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;    fLeg2=-11;
    fMother1=22; fMother2=22;
    fMothersRelation=kSame;
    fGrandMother1=111; fGrandMother2=111;
    //    SetGEANTProcess(kPUserDefined);
    break;
  case kPi0Dalitz:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;    fLeg2=-11;
    fMother1=111; fMother2=111;
    fMothersRelation=kSame;
    fDalitz=kIsDalitz; fDalitzPdg=22;
    // SetGEANTProcess(kPPrimary); //pluto
    break;
  case kK0Short:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=211;    fLeg2=-211;
    fMother1=310; fMother2=310; fCheckBothChargesMother1=kTRUE; fCheckBothChargesMother2=kTRUE;
    fMothersRelation=kSame;
    break;
  case kLambda:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=211;     fLeg2=2212;    fCheckBothChargesLeg1=kTRUE; fCheckBothChargesLeg2=kTRUE;
    fMother1=3122; fMother2=3122; fCheckBothChargesMother1=kTRUE; fCheckBothChargesMother2=kTRUE;
    fMothersRelation=kSame;
    break;
  case kInclElePM:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;    fLeg2=-11;   fCheckBothChargesLeg1=kTRUE;    fCheckBothChargesLeg2=kTRUE;
    fMother1Source=kPrimary; fMother2Source=kPrimary;
    //    fMothersRelation=kSame;
    break;
  case kDeltaElectron: //single delta ray electrons
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;  fLeg2=1; fCheckBothChargesLeg1=kTRUE;
    SetGEANTProcess(kPDeltaRay);
    fIsSingleParticle=kTRUE;
    //    fMother1Source=kPrimary; fMother2Source=kDontCare;
    break;
  case kPrimElectron: //single electrons
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=11;  fLeg2=1; fCheckBothChargesLeg1=kTRUE;
    //fMother1Source=kPrimary;
    SetGEANTProcess(kPPrimary);
    fIsSingleParticle=kTRUE;
    break;
  case kPrimMuon: //single muoons
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=13;  fLeg2=1; fCheckBothChargesLeg1=kTRUE;
    //fMother1Source=kPrimary;
    SetGEANTProcess(kPPrimary);
    fIsSingleParticle=kTRUE;
    break;
  case kPrimPion: //single pions
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=211;  fLeg2=1; fCheckBothChargesLeg1=kTRUE;
    //fMother1Source=kPrimary;
    SetGEANTProcess(kPPrimary);
    fIsSingleParticle=kTRUE;
    break;
  case kPrimKaon: //single kaons
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=321;  fLeg2=1; fCheckBothChargesLeg1=kTRUE;
    //fMother1Source=kPrimary;
    SetGEANTProcess(kPPrimary);
    fIsSingleParticle=kTRUE;
    break;
  case kPrimProton: //single protons
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=2212;  fLeg2=1; fCheckBothChargesLeg1=kTRUE;
    //fMother1Source=kPrimary;
    SetGEANTProcess(kPPrimary);
    fIsSingleParticle=kTRUE;
    break;
  case kDeuteron:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=1000010020;;  fLeg2=1; fCheckBothChargesLeg1=kTRUE;
    //fMother1Source=kPrimary;
    //SetGEANTProcess(kPPrimary);
    fIsSingleParticle=kTRUE;
    break;
  case kTriton:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=1000010030;;  fLeg2=1; fCheckBothChargesLeg1=kTRUE;
    //fMother1Source=kPrimary;
    //SetGEANTProcess(kPPrimary);
    fIsSingleParticle=kTRUE;
    break;
  case kHe3:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=1000020030;;  fLeg2=1; fCheckBothChargesLeg1=kTRUE;
    //fMother1Source=kPrimary;
    //SetGEANTProcess(kPPrimary);
    fIsSingleParticle=kTRUE;
    break;
  case kAlpha:
    SetNameTitle(fgkSignals[defaultSignal][0],fgkSignals[defaultSignal][1]);
    fLeg1=1000020040;;  fLeg2=1; fCheckBothChargesLeg1=kTRUE;
    //fMother1Source=kPrimary;
    //SetGEANTProcess(kPPrimary);
    fIsSingleParticle=kTRUE;
    break;
  default:
    Error("PairAnalysisSignalMC","Signal NOT predefined! Either request it or configure on your own.");
    return;
    break;
  }
  //  Info("PairAnalysisSignalMC","Use predefined signal selected: %s\n",GetName());
  //  Print();

}

void PairAnalysisSignalMC::Print(Option_t* /*option*/) const
{
  //
  //
  //
  Printf("MC Signal: %s with title %s ", GetName(), GetTitle());
  printf("\tPDGs leg1:  %s%s%d%s ",(fLeg1Exclude?"!":""),(fCheckBothChargesLeg1?"|":""),fLeg1,(fCheckBothChargesLeg1?"|":""));
  if(fMother1!=0)
    printf("<--- %s%s%d%s ",(fMother1Exclude?"!":""),(fCheckBothChargesMother1?"|":""),fMother1,(fCheckBothChargesMother1?"|":""));
  if(fGrandMother1!=0)
    printf("<--- %s%s%d%s ",(fGrandMother1Exclude?"!":""),(fCheckBothChargesGrandMother1?"|":""),fGrandMother1,(fCheckBothChargesGrandMother1?"|":""));
  if(fGreatGrandMother1!=0)
    printf("<--- %s%s%d%s",(fGreatGrandMother1Exclude?"!":""),(fCheckBothChargesGreatGrandMother1?"|":""),fGreatGrandMother1,(fCheckBothChargesGreatGrandMother1?"|":""));
  printf("\n");
  if(!fIsSingleParticle) {
    //    Printf("\t                     \t %s",(fDalitz)
    //    Printf("\t                     \t %s",(fMothersRelation)
  printf("\tPDGs leg2:  %s%s%d%s ",(fLeg2Exclude?"!":""),(fCheckBothChargesLeg2?"|":""),fLeg2,(fCheckBothChargesLeg2?"|":""));
  if(fMother2!=0)
    printf("<--- %s%s%d%s ",(fMother2Exclude?"!":""),(fCheckBothChargesMother2?"|":""),fMother2,(fCheckBothChargesMother2?"|":""));
  if(fGrandMother2!=0)
    printf("<--- %s%s%d%s ",(fGrandMother2Exclude?"!":""),(fCheckBothChargesGrandMother2?"|":""),fGrandMother2,(fCheckBothChargesGrandMother2?"|":""));
  if(fGreatGrandMother2!=0)
    printf("<--- %s%s%d%s",(fGreatGrandMother2Exclude?"!":""),(fCheckBothChargesGreatGrandMother2?"|":""),fGreatGrandMother2,(fCheckBothChargesGreatGrandMother2?"|":""));
  printf("\n");
  }

}


//_________________________________________________________________________
PairAnalysisSignalMC::~PairAnalysisSignalMC() {
  //
  //  Destructor
  //
}

Double_t PairAnalysisSignalMC::GetWeight(Double_t *const values) const{
  // TODO: workaround to avoid zero weights
  if(fType==0) return fWeight;

  // kind of binary search
  Int_t knot = fWeights->FindX(values[fType]);

  Double_t x=0.;
  Double_t y=0.;
  fWeights->GetKnot(knot,x,y);
  return y;
}

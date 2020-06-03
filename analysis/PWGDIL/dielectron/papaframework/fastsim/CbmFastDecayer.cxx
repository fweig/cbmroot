#include "FairPrimaryGenerator.h"

#include "CbmStack.h"
#include "CbmFastDecayer.h"

#include "TSystem.h"
#include "TVirtualMC.h"
#include "TMCProcess.h"
#include <TParticle.h>

#include <iostream>     // std::cout
#include <algorithm>    // std::find
#include <vector>       // std::vector
#include <sstream>      // stream

using std::string;
using std::stringstream;

#include <TVirtualMCDecayer.h>

ClassImp(CbmFastDecayer)
///////////////////////////////////////////////////////////////////////////
CbmFastDecayer::CbmFastDecayer()
  :CbmFastDecayer("fastDecayer","fastDecayer")
{
  //
  // Default Construction
  //
}

///////////////////////////////////////////////////////////////////////////////////////////
CbmFastDecayer::CbmFastDecayer(const char* fileName, TString particle)
  : FairGenerator(),
    fDecayPdgCodes(0),
    fGeantPdgCodes(0)
{
  //
  // Standrad Construction
  //
  particle+=fileName; // dummy against unsed variables
}

///////////////////////////////////////////////////////////////////////////////////////////
CbmFastDecayer::~CbmFastDecayer() 
{
  //
  // Standard Destructor
  //
  if(fStack) {delete fStack;}
  fStack = 0; 
  if(fDecayer) {delete fDecayer;}
  fDecayer = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
Bool_t CbmFastDecayer::Init() 
{
  //
  // Standard CbmGenerator Initializer - no input 
  // 1) initialize decayer with default decay and particle table
  // 2) set the decay mode to force particle 
  // 3) set a user decay table if defined
  //
  if(!fDecayer)
    {
      Fatal("Init","CbmFastDecayer has no external decayer!!!");
    }
  //  fDecayer = new CbmDecayerEvtGen();

  /// TODO: really needed? should be initialized before
  //  fDecayer->Init(); //read the default decay table DECAY.DEC and particle table

  //if is set a decay mode: default decay mode is kAll  
  //  fDecayer->SetForceDecay(fForceDecay);
  //  fDecayer->ForceDecay();

  // //if is defined a user decay table
  // if(fUserDecay) 
  //   {
  //     fDecayer->SetDecayTablePath(fUserDecayTablePath);
  //     fDecayer->ReadDecayTable();
  //   }
  return kTRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
Bool_t CbmFastDecayer::ReadEvent(FairPrimaryGenerator* primGen)
{
  //
  //Generate method - Input none - Output none
  //For each event:
  //1)return the stack of the previous generator and select particles to be decayed by EvtGen
  //2)decay particles selected and put the decay products on the stack
  //
  //

  // ---> Check for primary generator
  if ( ! primGen ) {
    Error("ReadEvent","No PrimaryGenerator!");
    return kFALSE;
  }

  // ---> Check for particle stack
  fStack = dynamic_cast<CbmStack*>(gMC->GetStack());
  if(!fStack) {
    Error("ReadEvent","Error: No stack found!");
    return kFALSE;
  }

  Float_t polar[3]= {0,0,0};  // Polarisation of daughter particles
  Float_t origin0[3];         // Origin of the parent particle
  Float_t pc[3], och[3];      // Momentum and origin of the children particles from EvtGen
  Int_t nt;
  Float_t tof;
  Int_t nPrimStack;
  Int_t nTrackStack;
  TLorentzVector *mom=new TLorentzVector();

  // create array with new particles/ deacy products
  static TClonesArray *particles;
  if(!particles) particles = new TClonesArray("TParticle",1000);

  nPrimStack  = fStack->GetNprimary();
  nTrackStack = fStack->GetNtrack();
  Info("ReadEvent","nPrimaries = %d  and ntracks = %d before fast decayer",nPrimStack,nTrackStack);

  /// allow tracking of decay products
  primGen->DoTracking(kTRUE);
  fStack->DoTracking(kTRUE);

  /// selection for geant and decay pdgs
  std::vector<int>::iterator it;

  // loop over all tracks need to decay
  for (Int_t iTrack = 0; iTrack < nTrackStack; ++iTrack) {

    // get particle
    TParticle *part = fStack->GetParticle(iTrack);
    Int_t pdg=part->GetPdgCode();

    /// select certain particles for decay
    it = std::find(fDecayPdgCodes.begin(), fDecayPdgCodes.end(), TMath::Abs(pdg));
    if(it==fDecayPdgCodes.end()) continue;

    //check if particle is already decayed
    /// check for dalitz pi0, gamma, etc.
    //TODO: how?
    if(/*part->GetStatusCode() != 1 ||*/ part->GetNDaughters()>0)  {
      Info("ReadEvent","Attention: particle %d is already decayed (code:%d,daughters:%d)!",
			    pdg,part->GetStatusCode(),part->GetNDaughters());
      continue;
    }

    /// TODO: check
    part->SetStatusCode(11); //Set particle as decayed : change the status code

    /// create lorentz vector
    mom->SetPxPyPzE(part->Px(),part->Py(),part->Pz(),part->Energy());
    Int_t np;

    /// decay
    do{
      fDecayer->Decay(pdg,mom);
      np = fDecayer->ImportParticles(particles); // into array
    }while(np<0);

    Int_t* trackIt   = new Int_t[np];
    Int_t* pParent   = new Int_t[np];

    Info("ReadEvent","number of products: np = %d for mother: %d",np-1,pdg);
    //    mom->Print(); // debug

    for (int i = 0; i < np; i++) {
      pParent[i]   = -1;
      trackIt[i]   =  0;
    }

    //select trackable particles (decay products)
    if (np>1) {
      TParticle* iparticle =  0;

      for (int i = 1; i<np ; i++) {
	iparticle = (TParticle*) particles->At(i);
	Int_t ks = iparticle->GetStatusCode();
	Int_t pdgd = iparticle->GetPdgCode();

	//track last decay products
	if(ks==1) {

	  /// select certain particles for decay
	  it = std::find(fGeantPdgCodes.begin(), fGeantPdgCodes.end(), TMath::Abs(pdgd));
	  if(it==fGeantPdgCodes.end()) trackIt[i]=1;
	  else             continue;
	}

      }//decay particles loop

    }// if decay products

    /// mother origin
    origin0[0]=part->Vx(); //[cm]
    origin0[1]=part->Vy(); //[cm]
    origin0[2]=part->Vz(); //[cm]

    /// only debug
    //    TParticle* mother = (TParticle *) particles->At(0);
    //    TLorentzVector motherL;
    //    motherL.SetPxPyPzE(mother->Px(),mother->Py(),mother->Pz(),mother->Energy());
    //    motherL.Print();

    //
    // Put decay products on the stack
    //
    for (int i = 1; i < np; i++) {
      TParticle* iparticle = (TParticle *) particles->At(i);
      Int_t kf   = iparticle->GetPdgCode();
      Int_t ksc  = iparticle->GetStatusCode();
      Int_t jpa  = iparticle->GetFirstMother()-1;         //jpa = 0 for daughters ofmother particles

      /// NOTE: the first decay generation points to the generated particles (by e.g. UrQMD),
      ///       further generations (cascade daughters) have jpa indices > 0
      ///       and need to point to the correct previously added mother (not grandmother) -> (i-jpa)
      Int_t iparent = (jpa > 0) ? fStack->GetNtrack()-(i-jpa) : iTrack;

      Info("ReadEvent","FirstMother = %d (iparent %d),  indicePart = %d,  pdg = %d ",jpa,iparent,i,kf);

      /// position and momentum of daughters
      och[0] = origin0[0]+iparticle->Vx(); //[cm]
      och[1] = origin0[1]+iparticle->Vy(); //[cm]
      och[2] = origin0[2]+iparticle->Vz(); //[cm]
      pc[0]  = iparticle->Px(); //[GeV/c]
      pc[1]  = iparticle->Py(); //[GeV/c]
      pc[2]  = iparticle->Pz(); //[GeV/c]
      tof = part->T()+iparticle->T();
      Double_t ee = iparticle->Energy();


      /// Give track to PrimaryGenerator
      /// NOTE: parent index need to be reduced by the stack size, because the index is shifted by this number in FairPrimaryGenerator::AddTrack!
      Info("ReadEvent"," Add track %d to primary Generator: pdg=%d with parent=%d and do tracking %d (stack primaries: %d, tracks: %d)",
	   i,kf, iparent,trackIt[i],fStack->GetNprimary(),fStack->GetNtrack());

      primGen->AddTrack(kf, pc[0], pc[1], pc[2], och[0], och[1], och[2], iparent -nTrackStack, trackIt[i], ee);

    }// Particle loop

    // clean up
    particles->Clear();
    if (trackIt)    delete[] trackIt;
    if (pParent)    delete[] pParent;
  }

  /// deny tracking of decay products
  primGen->DoTracking(kFALSE);
  fStack->DoTracking(kFALSE);

  return kTRUE;
}

void CbmFastDecayer::SetParticlesForDecay(char const *pdgs)
{

  stringstream ss(pdgs);
  int n;
  char ch;

  while(ss >> n) {
    if(ss >> ch)      fDecayPdgCodes.push_back(n);
    else              fDecayPdgCodes.push_back(n);
  }

  // for (auto i = fDecayPdgCodes.begin(); i != fDecayPdgCodes.end(); ++i)
  //   std::cout << *i << ' ';

}

void CbmFastDecayer::SetParticlesForGeant(char const *pdgs)
{

  stringstream ss(pdgs);
  int n;
  char ch;

  while(ss >> n) {
    if(ss >> ch)      fGeantPdgCodes.push_back(n);
    else              fGeantPdgCodes.push_back(n);
  }

  // for (auto i = fGeantPdgCodes.begin(); i != fGeantPdgCodes.end(); ++i)
  //   std::cout << *i << ' ';

}


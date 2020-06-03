/////////////////////////////////////////////////////////////
//
//  FastSim
//
////
///////////////////////////////////////////////////////////////

//C++ class headers
#include <string>
#include <fstream>

//Fair class headers
#include "FairLogger.h"
#include "FairRootManager.h"
//#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairRun.h"

//CBM class headers
#include "CbmMCTrack.h"
#include "CbmStack.h"
#include "PairAnalysisHelper.h"

//ROOT class headers
#include <TClonesArray.h>
#include <TVector3.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include <TParticle.h>
#include <TRandom3.h>
#include <TDatabasePDG.h>
#include <TParticlePDG.h>
#include <TPDGCode.h>
#include <TVirtualMC.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TProfile3D.h>
#include <THnBase.h>
#include <THnSparse.h>
#include <TMath.h>


#include "CbmFastSim.h"


using std::cout;
using std::endl;
using std::string;
using std::ifstream;


// -----   Default constructor   -------------------------------------------
CbmFastSim::CbmFastSim(bool persist) 
  : FairTask("Cbm Fast Simulation"),
    fRand(new TRandom3(0)),
    fdbPdg(TDatabasePDG::Instance())
{
  ///
  /// default constructor
  ///
  for(Int_t idx=0; idx<fgkNParts; idx++) {
    fEFF[idx]         = NULL;
    fEFFdenom[idx]    = NULL;
    for(Int_t j=0;j<10;j++)
      fEFFproj[idx][j] = NULL;

    fRFp[idx]         = NULL;
  }
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmFastSim::~CbmFastSim()
{
  ///
  /// destructor
  ///
  FairRootManager *fManager =FairRootManager::Instance();
  fManager->Write();

  if (fFastTracks) {fFastTracks->Delete(); delete fFastTracks;}
  if (fRand) delete fRand;
}
// -------------------------------------------------------------------------


void CbmFastSim::Register() {
  ///
  /// register output
  ///
  fFastTracks = new TClonesArray("TParticle");
  FairRootManager::Instance()->Register("FastTrack","FastSim", fFastTracks, kTRUE);

}



// -----   Public method Init   --------------------------------------------
void CbmFastSim::InitTask() {
  Init();
}

// -----   Public method Init   --------------------------------------------
InitStatus CbmFastSim::Init()
{

  /// loop over all particle species
  for(Int_t idx=0; idx<fgkNParts; idx++) { ///TODO: include muons

    if(fEFF[idx] && fEFFdenom[idx]) {
      Info("Init","Prepare efficiency map and projections for index %d and method %d",idx,fMethod);
      //      fEFF[idx]->Print("a");

      // first get projections and calculate integrated effiencies
      // NOTE: if statistic are bad these are used
      for (Int_t j = 0; j < fEFF[idx]->GetNdimensions(); j++) {

	/// method dependent scaling, normalisation, ...
	switch(fMethod) {
	case kIgnoreFluct:
	  /// NOTE: do not fill projections
	  continue;
	  break;
	case kLastDim:
	  /// NOTE: only last dimension is used for efficiency
	  if( j<(fEFF[idx]->GetNdimensions()-1) ) continue;
	case kAverage:
	  /// NOTE:  use all ndim 1D-histograms
	case kFactorize: {
	  /// NOTE:  use all ndim 1D-histograms, but dim=j>0 are scaled to max=1
	  fEFFproj[idx][j] = fEFF[idx]->Projection(j,"E");
	  fEFFproj[idx][j]->SetName(Form("NOMidx%dj%d",idx,j));

	  TH1 *den=fEFFdenom[idx]->Projection(j,"E");
	  den->SetName(Form("DENidx%dj%d",idx,j));
	  fEFFproj[idx][j]->Divide( den );

	  // factoring using last dimension!=1 histograms
	  if(fMethod==kFactorize && j>0)	fEFFproj[idx][j]->Scale( 1./fEFFproj[idx][j]->GetBinContent( fEFFproj[idx][j]->GetMaximumBin() ) );

	  if(den) delete den;
	}
	  break;
	case kInterpolate: {
	  /// NOTE: prepare a single 3D-histogram
	  if(j>0)  continue;
	  fEFFproj[idx][j] = fEFF[idx]->Projection(j,1,2,"E");
	  fEFFproj[idx][j]->SetName(Form("NOMidx%dj%d12",idx,j));

	  TH1 *den=fEFFdenom[idx]->Projection(j,1,2,"E");
	  den->SetName(Form("DENidx%dj%d12",idx,j));
	  fEFFproj[idx][j]->Divide( den );

	  if(den) delete den;
	}
	  break;
	default:
	  break;
	}
      }

      // calculate main effiency map
      fEFF[idx]->Divide(fEFFdenom[idx]);

    } // endif: nominator


    /// prepare histograms for smearing
    if(fRFp[idx]) {
      Info("Init","Momentum smearing map found for index %d",idx);
      PairAnalysisHelper::CumulateSlicesX( fRFp[idx], kFALSE, kTRUE );
    }

  } //end: particle species

  // Create and register output array
  Register();

  return kSUCCESS;
}

void CbmFastSim::SetParContainers() {

  // Get run and runtime database
  FairRun* run = FairRun::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* db = run->GetRuntimeDb();
  if ( ! db ) Fatal("SetParContainers", "No runtime database");


}
// -------------------------------------------------------------------------

void CbmFastSim::SetSeed(unsigned int seed)
{
  ///
  /// set random seed by hand
  ///
  fRand->SetSeed(seed);
}

// -----   Public method Finish   --------------------------------------------
void CbmFastSim::Finish()
{
  ///
  /// finish 
  ///

}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void CbmFastSim::Exec(Option_t* opt)
{
  ///
  /// exec, executed once per event
  ///

  /// get mc stack of generated particles
  CbmStack *fStack=(CbmStack*)gMC->GetStack();
  Int_t nTracks=fStack->GetNtrack();
  //  Info("Exec","number of tracks **** %d",Tracks);

  /// Reset output array
  if (fFastTracks->GetEntriesFast() != 0)  fFastTracks->Clear("C");

  ///
  TClonesArray &chrgCandidates = *fFastTracks;  // Charged Candidates

  Int_t iKeep=0;
  Int_t iConv=0;

  /// array of variables and coordinates used for look-up tables
  Double_t *vals = new Double_t[4];//fEFF->GetNdimensions()];
  Int_t* coord   = new Int_t[4];

  /// loop over the full mc stack
  for (Int_t iTrack=0; iTrack<nTracks; iTrack++) {

    /// get generated mc particle
    TParticle *t = fStack->GetParticle(iTrack);
    int pdg      = t->GetPdgCode();
    //    t->Print();

    /// process only known and charged particles
    if( !fdbPdg->GetParticle(pdg) )                        continue;
    if( TMath::Abs(fdbPdg->GetParticle(pdg)->Charge()) < 3) continue;

    /// collect mc truth variables
    /// NOTE: TVector3 & TLorentzVector return phi from -pi to pi, while TPartilce (base of CbmMCTrack) in 0 to 2pi
    vals[0] = t->Pt();
    vals[1] = t->Theta();
    vals[2] = TVector2::Phi_0_2pi( t->Phi() );
    vals[3] = t->Vz();

    /// apply efficiency cuts based on mc truth quantities for pt, theta, phi
    /// NOTE: special case for efficiency map of conversion electrons
    if(TMath::Abs(pdg)==11 && fStack->GetParticle(t->GetMother(0)) && fStack->GetParticle(t->GetMother(0))->GetPdgCode()==22) {
      if( !PassEfficiencyFilter(22, vals, coord) ) continue;
      iConv++;
    }
    else {
      if( !PassEfficiencyFilter(pdg, vals, coord) ) continue;
    }

    /// smear the momentum components according to the resolution (diff. btw. gen. & rec.)
    TLorentzVector p4(t->Px(),t->Py(),t->Pz(),t->Energy());

    /// start vertex
    //    TVector3 stvtx(t->Vx(),t->Vy(),t->Vz());

    /// smearing in p
    if(  !Smearing(&p4, pdg) ) { /*ERROR*/ continue;  }

    /// Add smeared track to output tree
    /// IMPORTANT NOTE: set MC index as FirstMother
    new ((*fFastTracks)[iKeep]) TParticle(pdg,t->GetStatusCode(),
					  iTrack,0,0,0,
					  p4.Px(),p4.Py(),p4.Pz(),p4.Energy(),
					  t->Vx(),t->Vy(),t->Vz(),0 );

    iKeep++;

  }//trackloop

  // cleanup
  delete [] vals;
  delete [] coord;

  Info("Exec","stack size: \t %d \n",fStack->GetNtrack());
  Info("Exec","kept tracks: \t %d \n",iKeep);
  Info("Exec","kept converison tracks: \t %d \n",iConv);

}

Bool_t CbmFastSim::PassEfficiencyFilter(Int_t pdg, Double_t *vals, Int_t *coord)
{
  //
  // check if partilce passed the reconstruction cuts
  //
  Int_t idx=-1;
  switch(TMath::Abs(pdg))
    {
    case kGamma:      idx = CbmFastSim::kGam; break;
    case kElectron:   idx = CbmFastSim::kEle; break;
    case kMuonMinus:  idx = CbmFastSim::kMuo; break;
    case kPiPlus:     idx = CbmFastSim::kPio; break;
    case kKPlus:      idx = CbmFastSim::kKao; break;
    case kProton:     idx = CbmFastSim::kPro; break;
    default :         /*Warning("PassEfficiencyFilter","Pdg code %d not supported, particle will not be processed",pdg);*/ return kFALSE;
    }

  /// no efficiency map added?
  if(!fEFF[idx])  { Warning("PassEfficiencyFilter","No filter for pdg code %d",pdg); return kFALSE; }

  /// debug
  //  Info("PassEfficiencyFilter","Efficiency for pt:%.2e theta:%.2e phi:%.2e", vals[0],vals[1],vals[2]);

  /// find bin for mc pt, theta, phi
  Long_t bin = fEFF[idx]->GetBin(vals,kFALSE);

  /// lookup efficiency and check for statistical error beeing less than 30%
  Double_t eff = 0.0;
  Double_t err = 0.0;
  if(bin>0) {
    eff=fEFF[idx]->GetBinContent(bin,coord); // write bin coordinates into coord

    /// check statistical fluctuations
    if(fMethod!=kIgnoreFluct) {

      err=fEFF[idx]->GetBinError(bin);
      //    if (eff>0. && err/eff>=0.60 ) return kFALSE;
      if (eff>0. && err/eff>=0.10 ) {
	Double_t effm= GetEfficiency(idx, vals, coord); // vals was NULL
	//      Info("PassEfficiencyFilter","statistical error %f too small (eff:%.2e, new:%.2e)",(eff>0.?err/eff:0.),eff,effm);
	eff=effm;
      }
    }

  }
  //  else {
  //    eff= GetEfficiency(idx, vals, NULL);
  //    Info("PassEfficiencyFilter","No entry in map, caculate via projections: %.2e (mean eff.)",eff);
  //  }

  Double_t rndm = fRand->Rndm();

  if(rndm>eff) return kFALSE;
  else         return kTRUE;

}

Bool_t CbmFastSim::Smearing(TLorentzVector *p4, Int_t pdg)
{
  ///
  /// smear the momentum components of the tlorentzvector
  /// according to a TH2 disttributions
  ///

  Int_t idx=-1;
  switch(TMath::Abs(pdg))
    {
    case kElectron:   idx = CbmFastSim::kEle; break;
    case kMuonMinus:  idx = CbmFastSim::kMuo; break;
    case kPiPlus:     idx = CbmFastSim::kPio; break;
    case kKPlus:      idx = CbmFastSim::kKao; break;
    case kProton:     idx = CbmFastSim::kPro; break;
    default:          return kFALSE;
    }

  /// references
  Double_t tP        = p4->P();
  Double_t tTheta    = p4->Theta();
  Double_t tPhi      = TVector2::Phi_0_2pi( p4->Phi() );
  Double_t mass      = fdbPdg->GetParticle(pdg)->Mass();

  /// get smeared components
  Double_t sP        =  AnalyzeMap(fRFp[idx],     tP);
  Double_t sTheta    =  tTheta;
  Double_t sPhi      =  tPhi;

  /// calculate vector components
  Double_t sPx       = sP * TMath::Sin( sTheta ) * TMath::Cos( sPhi );
  Double_t sPy       = sP * TMath::Sin( sTheta ) * TMath::Sin( sPhi );
  Double_t sPz       = sP * TMath::Cos( sTheta );

  Double_t sPt       = TMath::Sqrt( sPx*sPx + sPy*sPy );
  Double_t eta       = -TMath::Log( TMath::Tan(sTheta/2) );


  //  p4->Print();
  p4->SetPtEtaPhiM(sPt, eta  ,sPhi, mass);
  // p4->Print();
  // printf("\n\n");

  return kTRUE;
}

Double_t CbmFastSim::AnalyzeMap(TH2F *map, Double_t refValue)
{
  ///
  /// smear the refValue
  /// according to a TH2 map
  ///

  /// check smearing map
  if(!map) return refValue;

  /// get axes
  TAxis *xRec = map->GetXaxis();
  TAxis *yGen = map->GetYaxis();

  /// find y-bin for mc variable
  Int_t binGen=yGen->FindBin( refValue );

  /// for under- or overflow, apply gaussian smearing with 2% resolution
  /// by this electrons are handled without bremsstrahlung (TODO: add crystal ball smearing)
  /// NOTE: in general this should not happen often, you have to provide smearing maps
  ///       with large momentum ranges
  if(binGen<1 || binGen>yGen->GetNbins()) {
    return ( fRand->Gaus(refValue,refValue*0.02) );
  }

  /// get histogram array and calculate offset
  Float_t *arrayh = map->GetArray();
  Int_t offset    = (yGen->GetNbins()+2)*(binGen)+1;

  /// get randomly smeared value: y-axis translates into x-axis (generated into reconstructed)
  Float_t r1    = fRand->Rndm();
  Int_t binRec  = TMath::BinarySearch(xRec->GetNbins()+2, arrayh+offset-1, r1);

  Double_t smearedValue = xRec->GetBinLowEdge(binRec+1);
  if (r1 > arrayh[offset-1+binRec])
    smearedValue += xRec->GetBinWidth(binRec+1)*(r1-arrayh[offset-1+binRec])/(arrayh[offset-1+binRec+1] - arrayh[offset-1+binRec]);

  // printf("binGen: %d, binRec: %d , offset: %d , \t r1: %f \t in: %f --> out: %f \n",
  // 	 binGen,binRec,offset, r1,refValue,smearedValue);

  return smearedValue;
}


void CbmFastSim::SetLookupEfficiency(      THnBase    *nom, THnBase *denom, EParticleType part)
{
  ///
  /// calculate lookup efficiencies
  ///

  if(!nom || !denom) Fatal("SetLookupEfficiency","Either nominator or denominator is NULL ");
  fEFF[part]      = nom;
  fEFFdenom[part] = denom;
  /*
  // first get projections and calculate integrated effiencies
  // NOTE: if statistic are bad these are used
  for (Int_t j = 0; j < nom->GetNdimensions(); ++j) {

    /// method dependent scaling, normalisation, ...
    switch(fMethod) {
    case kIgnoreFluct:
      /// NOTE: do not fill projections
      continue;
      break;
    case kLastDim:
      /// NOTE: only last dimension is used for efficiency
      if( j<(nom->GetNdimensions()-1) ) continue;
      break;
    case kAverage:
      /// NOTE:  use all ndim 1D-histograms
    case kFactorize: {
      /// NOTE:  use all ndim 1D-histograms, but dim=j>0 are scaled to max=1
      fEFFproj[part][j] = nom->Projection(j,"E");
      fEFFproj[part][j]->SetName(Form("NOMidx%dj%d",part,j));

      TH1 *den=denom->Projection(j,"E");
      den->SetName(Form("DENidx%dj%d",part,j));
      fEFFproj[part][j]->Divide( den );

      // factoring using last dimension!=1 histograms
      if(fMethod==kFactorize && j>0)	fEFFproj[part][j]->Scale( 1./fEFFproj[part][j]->GetBinContent( fEFFproj[part][j]->GetMaximumBin() ) );

      if(den) delete den;
    }
      break;
    case kInterpolate: {
      /// NOTE: prepare a single 3D-histogram
      if(j>0 && j<3)  continue;
      fEFFproj[part][j] = nom->Projection(j,1,2,"E");
      fEFFproj[part][j]->SetName(Form("NOMidx%dj%d12",part,j));

      TH1 *den=denom->Projection(j,1,2,"E");
      den->SetName(Form("DENidx%dj%d12",part,j));
      fEFFproj[part][j]->Divide( den );

      if(den) delete den;
    }
      break;
    default:
      break;
    }


  }

  // calculate main effiency map
  nom->Divide(denom);
  */
  
}

Double_t CbmFastSim::GetEfficiency(Int_t idx, Double_t *vals, Int_t *coord)
{
  ///
  /// Calculate the efficieny from the integrated projections of the map
  ///

  if(fMethod==kIgnoreFluct) return 0.;


  Int_t    ndim    = 0;
  Double_t meanEff = ( fMethod==kFactorize ? 1. : 0. );
  Double_t eff     = 0.;
  //  for (Int_t j = 0; j < 10; ++j) {
  for (Int_t j = 10; j >= 0; j--) {

    if(!fEFFproj[idx][j]) continue;

    /// find bin or use coordinates
    if(coord) eff = fEFFproj[idx][j]->GetBinContent( coord[j] );
    else      eff = fEFFproj[idx][j]->GetBinContent( fEFFproj[idx][j]->FindBin(vals[j]) );
    //    Info("GetEfficiency","int eff for j:%d and idx:%d and value: %.2e is %.2e!",j,idx,vals[j],eff);

    /// method dependent
    switch(fMethod) {
    case kIgnoreFluct: return 0.;
    case kInterpolate: {
      if(coord) {
	/// protect against first(last) bin
	/// NOTE: interpolation cannot be done for values smaller(larger) bin center
	for (Int_t c=0; c<3; c++) {
	  if(coord[c]==0)                                   coord[c]+=2;
	  if(coord[c]==1)                                   coord[c]+=1;
	  if(coord[c]==fEFF[idx]->GetAxis(c)->GetNbins())   coord[c]-=2;
	  if(coord[c]==fEFF[idx]->GetAxis(c)->GetNbins()-1) coord[c]-=1;
	}
	eff = fEFFproj[idx][j]->Interpolate(
					    fEFFproj[idx][j]->GetXaxis()->GetBinCenter(coord[0]),
					    fEFFproj[idx][j]->GetYaxis()->GetBinCenter(coord[1]),
					    fEFFproj[idx][j]->GetZaxis()->GetBinCenter(coord[2])
					    );
      }
      else
	eff = fEFFproj[idx][j]->Interpolate( vals[0], vals[1], vals[2]);
      return eff;
    }
      break;
    case kAverage:
    case kLastDim:
      if( eff < 1.e-10 )  return 0.; /// efficiency is 0. for this particle (e.g. outside acceptance)
      meanEff += eff;
      ndim++;
      break;
    case kFactorize:
      if( eff < 1.e-10 )  return 0.; /// efficiency is 0. for this particle (e.g. outside acceptance)
      meanEff *= eff;
      ndim=1; //      ndim++;
      break;
    }

  }

  // protect against division by 0
  if(!ndim) { Error("GetEfficiency","number of dimensions for idx:%d is zero!",idx); return 0.; }

  // return mean efficiency
  return (meanEff/ndim);

}

Bool_t CbmFastSim::IsSelected(TObject *sel, Int_t opt)
{
  //
  // check if partilce passed the cuts
  //

  Int_t pdg=opt;
  TVector3 *mom = dynamic_cast<TVector3*>(sel);

  // Info("IsSelected","Compare pdg code %d to e(%d),mu(%d),pi(%d),K(%d),p(%d)",
  //      TMath::Abs(pdg),kElectron,kMuonMinus,kPiPlus,kKPlus,kProton);

  Int_t idx=-1;
  switch(TMath::Abs(pdg))
    {
    case kGamma:      idx = CbmFastSim::kGam; break;
    case kElectron:   idx = CbmFastSim::kEle; break;
    case kMuonMinus:  idx = CbmFastSim::kMuo; break;
    case kPiPlus:     idx = CbmFastSim::kPio; break;
    case kKPlus:      idx = CbmFastSim::kKao; break;
    case kProton:     idx = CbmFastSim::kPro; break;
    default :         /*Warning("IsSelected","Pdg code %d not supported, particle will not be processed",pdg);*/ return kFALSE;
    }

  // Double_t rndm2 = fRand->Rndm();
  // if(rndm2<0.1) return kTRUE;
  // else         return kFALSE;

  /// no efficiency map added?
  if(!fEFF[idx])  { Warning("IsSelected","No filter for pdg code %d",pdg); return kTRUE; }

  /// array of variables used for look-up table
  Double_t *vals = new Double_t[3];

  /// collect mc truth variables
  /// NOTE: TVector3 & TLorentzVector return phi from -pi to pi, while TPartilce (base of CbmMCTrack) in 0 to 2pi
  vals[0] = mom->Pt();
  vals[1] = mom->Theta();
  vals[2] = TVector2::Phi_0_2pi( mom->Phi() );

  /// find bin for mc pt, theta, phi
  Long_t bin = fEFF[idx]->GetBin(vals,kFALSE);

  /// clean up
  delete vals;

  Double_t eff = 0.0;
  if(bin>0) eff=fEFF[idx]->GetBinContent(bin);
  Double_t rndm = fRand->Rndm();


  if(rndm>eff) return kFALSE;
  else         return kTRUE;

}

ClassImp(CbmFastSim)

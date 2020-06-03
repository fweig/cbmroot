/*
Macro for generation of di-lepton continuum distribution from open charm

Produces open charms randomly with one pair (DD-bar) per event using parametrized pT and rapidity distributions and forced decay via semi-leptonic decay channels 


Two semi-leptonic decay modes with 3 & 4 body channels; they are weighted via relative branching ratios


opt = 1 for muonic channel and opt=2 for electronic channel 

Author: Dr. Partha Pratim Bhaduri (partha.bhaduri@vecc.gov.in)


*/




void OpenCharm_prod_DDbar(int nEvents=1000, int opt=1)

{



 const Int_t kmaxHit = 1000000; 
 Double_t Px[kmaxHit],Py[kmaxHit],Pz[kmaxHit],En[kmaxHit];
 TLorentzVector P1[kmaxHit];

 Int_t Run=0, Event=0;
 Double_t b=0.,px=0.,py=0.,pz=0.,E=0.,pT=0.,p=0.;
 TLorentzVector P;
 
 Int_t trackNo, nTrack=2;

  TRandom3*ran=new TRandom3();

  TStopwatch timer;
  long seed=timer.RealTime()*gRandom->Gaus(0,timer.CpuTime());
  ran->SetSeed(seed);

  TFile *f = new TFile("opencharm.DDbar.muon.root","recreate");
  TTree *Tree = new TTree("Tree","test friend trees");
  Tree->Branch("Run",&Run,"Run/I");
  Tree->Branch("Event",&Event,"Event/I");
  Tree->Branch("trackNo",  &trackNo, "trackNo/I");
  Tree->Branch("Px",       Px,       "Px[trackNo]/D");
  Tree->Branch("Py",       Py,       "Py[trackNo]/D");
  Tree->Branch("Pz",       Pz,       "Pz[trackNo]/D");
  Tree->Branch("En",        En,        "En[trackNo]/D");

 // Tree->Branch("P1", P1, "Px[trackno]/D:Py[trackno]:Pz[trackno]:E[trackno]");
 
  const  double T=0.15,a=6;
  const double pi=acos(-1.0), pi_mass=0.139, D_mass=1.896, lambda_mass=2.286;
  const double mp=0.938,mmu=0.106, me=0.51E-3;

  double part_mass1=0.,part_mass2=0.;
  double Eb = 25; // specify beam energy
  double Ecm=TMath::Sqrt(2.*mp*mp+2.*mp*Eb); // convert cm energy

  double pbeam=TMath::Sqrt(Eb*Eb-mp*mp);
  double beta_CM=pbeam/(Eb+mp);
  double gamma_CM= 1. /TMath::Sqrt(1. - beta_CM*beta_CM );
  
  double Y_CM=0.5*TMath::Log((1+beta_CM)/(1-beta_CM)); // frame rapidity
  
// Rapidity distribution set in the same way as in  SPS signal generator GENESIS. Get width of pion rapidity distribution from Landau formula. For other particles set the width by proper scaling with maxm. possible rapidities

  double sigma_pi=TMath::Sqrt(log(Ecm/(2.0*mp)));  
  double ymax_mass=TMath::Log(Ecm/D_mass);
  double ymax_pi=TMath::Log(Ecm/pi_mass);
  double sigma_y=sigma_pi*(ymax_mass/ymax_pi);
  
  int nDecay=0,mode=0;
  

  int count=0.0;

 float decay_prob[2]={0.55,0.45}; // set according to the relative branching ratios in semi-leptonic modes

 //neutrion mass taken to bbe zero

 Double_t masses1[3] = {0.494, 0.106, 0.0} ; // 3 body channel
 Double_t masses2[4] = {0.494, 0.106, 0.139,0.0} ; // 4 body channel

 if (opt ==2){
 Double_t masses1[3] = {0.494, 0.51E-3, 0.0} ;
 Double_t masses2[4] = {0.494, 0.51E-3, 0.139,0.0} ;
 }
 TF1 *f1 = new TF1("f1","x*TMath::Exp(-[0]*x*x)",0.0,5.0);

 f1->SetParameter(0,1); // b=1 GeV^-2


  for (int i=0;i<nEvents;i++)

    {
      Event = i;
      
      int npart=0;

       //Now Randomly generate 2 D mesons  

      for (int iTrack=0;iTrack<nTrack;iTrack++){

	trackNo=iTrack+1;


    
      Double_t rDecay=gRandom->Uniform(0.,1.);
	
	for(int ii=0;ii<2;ii++){
	  if (rDecay < decay_prob[ii]) break;
	  rDecay-=decay_prob[ii];
	}


	if (ii==0) nDecay=3;
	if (ii==1) nDecay=4;

      //Generate the  mesons

      //pT & rapidity
      double pT1=f1->GetRandom();
      double mT1=TMath::Sqrt(pT1**2+D_mass**2);
      double ycm1=ran->Gaus(0,sigma_y);
    
      double pzcm1=mT1*sinh(ycm1);
      double phicm1=ran->Uniform(0,2.0*pi);
      double pxcm1=pT1*cos(phicm1);
      double pycm1=pT1*sin(phicm1);

  // Lorentztransformation to lab

 //We can either use generic formulae or take advantage of rapidity variable
 
      double ylab1=ycm1+Y_CM;
      double E1=mT1*cosh(ylab1);
      double pz1=mT1*sinh(ylab1); // Z  comp. of D meson momentum in lab frame

      // X & Y components are same in CM and lab frame

      // Now decay the D meson using ROOT class TPhaseSpace Decay     

      TLorentzVector D1(pxcm1,pycm1,pz1,E1);
      TGenPhaseSpace event1;
 
     if (nDecay==3) event1.SetDecay(D1, 3, masses1);
 
     if (nDecay==4) event1.SetDecay(D1, 4, masses2);

      Double_t weight1 = event1.Generate();
      TLorentzVector *pMu1 = event1.GetDecay(1);

      Px[iTrack]=pMu1->Px();
      Py[iTrack]=pMu1->Py();
      Pz[iTrack]=pMu1->Pz();
      En[iTrack]=pMu1->E();
      
      }
   if(trackNo !=0)  Tree->Fill();

 count++;
    }
  f->cd();
   Tree->Write();
   f->Close() ;

  cout<<" Macro complete"<<endl; 
}


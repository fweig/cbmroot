/*
MC code for generation of Drell-Yan dimuons and their decay into single muons

Di-muons are generated randomy following paramtrized mass & rapidity distributions, exponential mT distribution

Decay is performed following anisotropic angular distributions

*/

void DrellYan_SIS100()

{
  TRandom3*ran=new TRandom3();
  TStopwatch timer;
  long seed=timer.RealTime();//*gRandom->Gaus(0,timer.CpuTime());
  gRandom->SetSeed(seed);
  // ran->SetSeed(seed);

  TH1F*h1=new TH1F("mass","",100,0,5.0);
  TH1F*h2=new TH1F("mass reco","",100,0.,5.);

  const  double mass_min=1.0,mass_max=4.0,dmass=0.01;
  const  double T=0.15,a=6;
  const double pi=acos(-1.0), pi_mass=0.139;

  const double mp=0.938,mmu=0.106;

  double Eb = 8;  // Speciy the beam energy in the lab frame
  double Ecm=sqrt(2.*mp*mp+2.*mp*Eb); //Get cm energy of the collision

  double pbeam=TMath::Sqrt(Eb*Eb-mp*mp); // beam momentum
  double beta_CM=pbeam/(Eb+mp);
  double gamma_CM= 1. / TMath::Sqrt(1. - beta_CM*beta_CM );
  
  double Y_CM=0.5*TMath::Log((1+beta_CM)/(1-beta_CM));
  const double sigma_pi=TMath::Sqrt(log(Ecm/(2.0*mp)));
  double ymax_pi=log(Ecm/0.14);

  int Nevt=10000; // specify the number of events 

  for (int i=0;i<Nevt;i++)

    {
      // long seed=timer.RealTime();//*gRandom->Gaus(0,timer.CpuTime());  
      // ran->SetSeed(seed);

      // Generate randomly the drell yan di-muon mass following parameterized distribution (dN/dM ~ a1*M^-5/2*exp(-M^1.8/a2)

      TF1 *f1 = new TF1("f1","([0]/(x**2.5))*exp(-(x**1.8)/[1])",1.0,4.0);
      f1->SetParameters(7465,0.678);
      double mass = f1->GetRandom(1.0,4.0);
     
      h1->Fill(mass);    

      //get the pT distribution

      TF1 *f2 = new TF1("f2","x*TMath::Sqrt(x*x+[1]*[1])*TMath::BesselK1([0]*TMath::Sqrt(x*x+[1]*[1]))",0.0,5.0);

      f2->SetParameter(0,7.69); //6.67 corresponds to 150 MeV 5.88 to 170 MeV 7.69 to 130 MeV
      f2->SetParameter(1,mass);

      double pT=f2->GetRandom();

      //h1->Fill(pT);

      double mT=TMath::Sqrt(pT**2+mass**2);

      //Get the rapidity distribution
      /*
      double ymax_mass=TMath::Log(Ecm/mass);
      double ymax_pi=TMath::Log(Ecm/pi_mass);
      double sigma_y=sigma_pi*(ymax_mass/ymax_pi);
      double ycm=ran->Gaus(0,sigma_y);
      */

      double ymax_mass=TMath::Log(Ecm/mass);
      double ycm=ran->Gaus(0.0,0.684);
     
      if (ycm > ymax_mass) ycm=ymax_mass;

      double pzcm=mT*sinh(ycm);
      double phicm=ran->Uniform(0,2.0*pi);
      double pxcm=pT*cos(phicm);
      double pycm=pT*sin(phicm);

   // Lorentztransformation of mother into lab
     //  Double_t ener0  = TMath::Sqrt(pxcm*pxcm + pycm*pycm + pzcm*pzcm + m0*m0);
     //  pz0 = fGammaCm * (pz0 + fBetaCm*ener0);
   

  // ---> Perform two-particle decay  
  // Generate decay angles in rest frame of mother
      // polar angle
  //Double_t rTheta = gRandom->Rndm();
      // Double_t theta  = TMath::ACos(1.-2.*rTheta); 

      TF1 *f3 = new TF1("f3","1+cos(x)**2",-1.0,1.0);
      //      h2->Fill(f3->GetRandom());
      Double_t theta  = TMath::ACos(f3->GetRandom());      
      Double_t phi    = gRandom->Uniform(0., 2.*TMath::Pi());  // azimuth


  // Decay the mother particle in its rest frame
  Double_t pDecay = 0.5 * TMath::Sqrt( mass*mass - 4.*mmu*mmu);  // Decay momentum
  Double_t px1 = pDecay * TMath::Sin(theta) * TMath::Cos(phi);
  Double_t py1 = pDecay * TMath::Sin(theta) * TMath::Sin(phi);
  Double_t pz1 = pDecay * TMath::Cos(theta);
  Double_t e1  = TMath::Sqrt(px1*px1 + py1*py1 + pz1*pz1 + mmu*mmu);

  //p1, p2 are 4-momentum of the two muons
  TLorentzVector p1(px1, py1, pz1, e1);
  TLorentzVector p2(-px1, -py1, -pz1, e1);


  // Lorentztransformation to lab
  //We can either use generic formulae or take advantage of rapidity variable

  //  double pz= gamma_CM * (pzcm + beta_CM*Ecm);
  //  double E=(pT**2+pz**2+mass**2);

  double ylab=ycm+Y_CM;
  double pz=mT*sinh(ylab);
  double E=mT*cosh(ylab);
  
  
  TLorentzRotation lt(pxcm/E, pycm/E, pz/E);  //Define Lorentz matix for the decay muons to boost tehm from mother rest frame to lab frame

  p1 = lt * p1; //L.T to lab frame
  p2 = lt * p2; //L.Tto lab frame
  
 
  /*
TLorentzRotation lt(pxcm/E, pycm/E, pzcm/E); //Define Lorentz matix for the decay muons to boost tehm from mother rest frame to N-N CMS frame

 p1 = lt * p1; //L.T to NN CMS frame
 p2 = lt * p2; //L.Tto NN CMS frame
  */
 TLorentzVector p=p1+p2;

 h2->Fill(p.Mag()); // gives invariant mass
 // h2->Fill(p.CosTheta()); // gives invariant mass

     
    }
  h2->Draw();
  // h2->SetLineColor(kRed);
  //  h2->Draw("same");

}



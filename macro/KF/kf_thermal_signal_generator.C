//---------------------------------------------------------------------------------
// @author  M. Zyzak
// @version 1.0
// @since   15.08.14
//
// macro to generate signal events for KFParticleFinder
//_________________________________________________________________________________

void kf_thermal_signal_generator(Int_t iParticle    = 0,
                                 Int_t NEvents      = 1000,
                                 TString outputName = "./Signal.root") {
  const double kProtonMass = 0.938272321;  // Proton mass in GeV

  KFPartEfficiencies eff;

  const double kSignalMass = eff.partMass[iParticle];  //  mass in GeV
  int kSignalPDG           = eff.partPDG[iParticle];

  if (kSignalPDG == 7003112) kSignalPDG = 3112;
  if (kSignalPDG == -7003112) kSignalPDG = -3112;
  if (kSignalPDG == 7003222) kSignalPDG = 3222;
  if (kSignalPDG == -7003222) kSignalPDG = -3222;

  double fSlope    = 0.2;
  double fRapSigma = 0.448;
  double eBeam     = 10.;
  double pBeam     = TMath::Sqrt(eBeam * eBeam - kProtonMass * kProtonMass);
  double fYcm      = 0.25 * TMath::Log((eBeam + pBeam) / (eBeam - pBeam));

  TF1* fThermal =
    new TF1("thermal", "x*exp(-1.*sqrt(x*x+[1]*[1]) / [0])", 0., 10.);
  fThermal->SetParameter(0, fSlope);
  fThermal->SetParameter(1, kSignalMass);
  TRandom* fRandGen = new TRandom(0);

  URun* header =
    new URun("Thermal signal", "", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NEvents);
  UEvent* event = new UEvent;
  TFile* output = new TFile(outputName.Data(), "recreate");
  TTree* tree   = new TTree("events", "signal");
  header->Write();
  tree->Branch("event", "UEvent", event);

  int ParticlesPerEvent = 1;
  for (int i = 0; i < NEvents; i++) {
    event->Clear();
    event->SetParameters(i, 0, 0, 0, 0, 0);

    for (int j = 0; j < ParticlesPerEvent; ++j) {
      double yD   = fRandGen->Gaus(fYcm, fRapSigma);
      double ptD  = fThermal->GetRandom();
      double phiD = fRandGen->Uniform(0., 2. * TMath::Pi());

      // Calculate momentum, energy, beta and gamma
      double px = ptD * TMath::Cos(phiD);
      double py = ptD * TMath::Sin(phiD);
      double mt = TMath::Sqrt(kSignalMass * kSignalMass + ptD * ptD);
      double pz = mt * TMath::SinH(yD);
      double energy =
        sqrt(kSignalMass * kSignalMass + px * px + py * py + pz * pz);

      Int_t child[2] = {0, 0};
      event->AddParticle(j,
                         kSignalPDG,
                         0,
                         -1,
                         -1,
                         -1,
                         -1,
                         child,
                         px,
                         py,
                         pz,
                         energy,
                         0,
                         0,
                         0,
                         0,
                         1);
    }

    tree->Fill();
  }

  tree->Write();
  output->Close();

  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}

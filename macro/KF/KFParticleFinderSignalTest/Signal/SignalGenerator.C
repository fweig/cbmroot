//---------------------------------------------------------------------------------
// @author  M. Zyzak
// @version 1.0
// @since   15.08.14
//
// macro to generate signal events for KFParticleFinder
//_________________________________________________________________________________

void SignalGenerator(Int_t iParticle = 0, Int_t NEvent = 1000) {
  const double kProtonMass = 0.938272321;  // Proton mass in GeV

  KFPartEfficiencies eff;

  const double kSignalMass = eff.partMass[iParticle];  //  mass in GeV
  const int kSignalID      = eff.partPDG[iParticle];

  const double kTwoPi = 2. * TMath::Pi();  // 2*pi
  double fSlope       = 0.2;
  double fRapSigma    = 0.448;
  double eBeam        = 10.;
  double pBeam        = TMath::Sqrt(eBeam * eBeam - kProtonMass * kProtonMass);
  double fYcm         = 0.25 * TMath::Log((eBeam + pBeam) / (eBeam - pBeam));

  TF1* fThermal =
    new TF1("thermal", "x*exp(-1.*sqrt(x*x+[1]*[1]) / [0])", 0., 10.);
  fThermal->SetParameter(0, fSlope);
  fThermal->SetParameter(1, kSignalMass);
  TRandom* fRandGen = new TRandom(0);

  // Open output file
  ofstream outputfile;
  outputfile.open("Signal.txt");
  if (!outputfile.is_open()) {
    cout << "-E Signal (generator): Cannot open output file " << endl;
    return;
  }

  int PionsPerEvent = 1;
  for (int i = 0; i < NEvent; i++) {
    // Generate rapidity, pt and azimuth
    outputfile << PionsPerEvent << "   " << i + 1 << "  " << 0. << "  " << 0.
               << "  " << 0. << endl;
    for (int j = 0; j < PionsPerEvent; ++j) {
      double yD   = fRandGen->Gaus(fYcm, fRapSigma);
      double ptD  = fThermal->GetRandom();
      double phiD = fRandGen->Uniform(0., kTwoPi);

      // Calculate momentum, energy, beta and gamma
      double pxD = ptD * TMath::Cos(phiD);
      double pyD = ptD * TMath::Sin(phiD);
      double mtD = TMath::Sqrt(kSignalMass * kSignalMass + ptD * ptD);
      double pzD = mtD * TMath::SinH(yD);

      outputfile << kSignalID << "  " << pxD << "  " << pyD << "  " << pzD
                 << endl;
    }
  }

  outputfile.close();
}

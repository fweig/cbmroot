#include <iostream>
#include <string>

using namespace std;

void run_ascii_generator_cosy17(Int_t nEvents = 100000) {

  string outputAsciiFile = "proton_1.25gevc.ascii.dat";
  double minMomentum     = 1.25;
  double maxMomentum     = 1.25;

  // Open output file
  ofstream file(outputAsciiFile.c_str());

  // Initialize RN generator
  gRandom->SetSeed(10);

  for (Int_t iev = 0; iev < nEvents; iev++) {
    // Generate vertex coordinates
    double vz = -30;
    // Numbers are taken from https://cbm-wiki.gsi.de/foswiki/bin/view/Rich/BeamParameters
    double vx = gRandom->Gaus(0., 0.471);
    double vy = gRandom->Gaus(0., 0.441);

    // Write out event header
    file << 1 << " " << (iev + 1) << " " << vx << " " << vy << " " << vz
         << endl;

    int pdgProton = 2212;

    // Generate momentum
    double p = gRandom->Uniform(minMomentum, maxMomentum);
    // Generate polar angle
    double theta = TMath::Abs(gRandom->Uniform(0., 0.));
    // Generate azimuthal angle
    double phi = gRandom->Uniform(0., 360.);
    // Calculate momentum components
    double pt = p * TMath::Sin(theta);
    double px = pt * TMath::Cos(phi);
    double py = pt * TMath::Sin(phi);
    double pz = p * TMath::Cos(theta);

    file << pdgProton << " " << px << " " << py << " " << pz << endl;

    // Status output
    if (0 == (iev % 10000)) { cout << iev << "   " << pdgProton << endl; }
  }

  file.close();
}

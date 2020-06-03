#include <iostream>
#include <string>

using namespace std;

void run_ascii_generator(Int_t nEvents = 100000)
{
	
   	string asciiFile = "/home/aghoehne/Documents/CbmRoot/trunkNew/macro/rich/prototype/smallprototype/beamdistribution.ascii.dat";
    	//string asciiFile = "/Users/slebedev/Development/cbm/trunk/cbmroot/macro/rich/prototype/smallprototype/beamdistribution.ascii.dat";
   	double minMomentum = 0.3;
   	double maxMomentum = 5.;

   	// Open output file
   	ofstream file(asciiFile.c_str());

   	// Initialize RN generator
   	gRandom->SetSeed(10);

   	for(Int_t iev = 0; iev < nEvents; iev++) 
		{
    		 // Generate vertex coordinates
      		double vz = -30;
      		double vx = gRandom->Gaus(0., 0.3);
      		double vy = gRandom->Gaus(0., 0.3);

     		 // Write out event header
      		file << 1 << " " << (iev+1) << " " << vx << " " << vy << " " << vz << endl;

      		int pdgProton = 2212;
		int pdgPionPlus = 211;
		int pdgKaonPlus = 321;

     		 // Generate momentum
      		double p = gRandom->Uniform(minMomentum, maxMomentum);
     		 // Generate polar angle
      
      		double theta = TMath::Abs(gRandom->Uniform(0., 0.));
	 		 // Generate azimuthal angle
      
      		double phi = gRandom->Uniform(0., 360.);
	 		 // Calculate momentum components
      		double pt = p*TMath::Sin(theta);
      		double px = pt*TMath::Cos(phi);
      		double py = pt*TMath::Sin(phi);
      		double pz = p*TMath::Cos(theta);

     		 // Write out particles information

		int random = gRandom->Uniform(0,0);
		
		
		if (random == 0){
      			file << pdgProton << " " << px << " " << py << " " << pz << endl;
		}else if(random ==1){
			file << pdgPionPlus << " " << px << " " << py << " " << pz << endl;
		}else{	
			file << pdgKaonPlus << " " << px << " " << py << " " << pz << endl;
		}
     		 // Status output
  			if(0 == (iev%10000)) 
				{	
					cout << iev << "   " << pdgProton << endl;
				}  
 		}	 // events

  			 // Close output file
   	file.close();
}




/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

///// generating electron/positron track over full acceptance. Filling them step by step in acceptance.
void generator_input_file()
{
  ofstream* file = new ofstream("my_first_generator.dat");
  gRandom->SetSeed(1);

  Int_t nXsteps   = 49;   //100
  Int_t nYsteps   = 10;   //80
  Double_t xRange = 0.8;  //0.65
  Double_t idy    = 0;
  Double_t idx    = 0;
  Double_t ymin   = 0.0;  // 0.05
  Double_t ymax   = 0.1;  // 0.5
  Int_t sign;

  const Int_t nev = 10000;  // 50000
  Float_t vx, vy, vz;
  Float_t type;
  Int_t pdg;
  Int_t xPos, yPos, xsign;
  Double_t x, y, z, sqr;
  Float_t p, theta, phi, pt, px, py, pz;

  for (Int_t iev = 0; iev < nev; iev++) {
    // Generate vertex coordinates
    vz = 0;
    vx = 0;
    vy = 0;
    // Write out event header
    (*file) << 1 << " " << (iev + 1) << " " << vx << " " << vy << " " << vz << endl;

    // Select electron or positron
    type = gRandom->Uniform(0., 1.);
    if (type < 0.5) { pdg = 11; }
    else {
      pdg = -11;
    }

    // Generate momentum
    p = gRandom->Uniform(2.0, 2.0);  // (2.,2.)

    // xPos  = gRandom->Integer(nXsteps);
    // yPos  = gRandom->Integer(nYsteps);
    // xsign = gRandom->Integer(2);
    //x = xRange/nXsteps*xPos*pow(-1,xsign);
    //y = (ymax-ymin)/nYsteps*yPos+ymin;
    //z = 1;

    /***************** fill circles step by step ()  ************************/
    x   = -xRange + idx;
    y   = ymin + idy;
    z   = 1;
    idx = idx + xRange / nXsteps;
    if (x > (xRange)) {
      idy = idy + (ymax - ymin) / nYsteps;
      idx = 0;
    }
    if (y > (ymax - (ymax - ymin) / nYsteps) && x > (xRange - xRange / nXsteps)) idy = 0;
    /**************************************************************************/

    // x=0.3;
    // y=0.3;
    // z=1;

    sqr = sqrt(x * x + y * y + z * z);
    px  = p * x / sqr;
    py  = p * y / sqr;
    pz  = p * z / sqr;

    // Write out particles information
    (*file) << pdg << " " << px << " " << py << " " << pz << endl;

    // cout << "x = " << x << "; y = " << y << "; z = " << z << "; sqr = " << sqr <<"; p = " << p << "; px = " << px << "; py = " << py << "; pz = " << pz << endl;


    // Status output
    if (0 == (iev % 2000)) cout << iev << "   " << pdg << endl;
  }

  // Close output file
  file->close();
}


// {
//     ofstream *file = new ofstream("my_first_generator.dat");
//     gRandom->SetSeed(1);

//     Int_t nXsteps = 5; //100
//     Int_t nYsteps = 5; //80
//     Double_t xRange = 0.7; //0.65
//     Double_t idy = 0;
//     Double_t idx = 0;
//     Double_t xmin = -xRange; // -xRange
//     Double_t xmax = xRange;  // xRange
//     Double_t ymin = 0.05; // 0.05
//     Double_t ymax = 0.5; // 0.5
//     Int_t sign;

//     const Int_t nev = 1000; // 50000
//     Float_t vx, vy, vz ;
//     Float_t type;
//     Int_t pdg;
//     Int_t xPos, yPos, xsign;
//     Double_t x, y, z, sqr;
//     Float_t p, theta, phi, pt, px, py, pz;

//     for(Int_t iev = 0; iev < nev; iev++) {
//         // Generate vertex coordinates
// 		vz = 0;
// 		vx = 0;
// 		vy = 0;
//         // Write out event header
// 	   (*file) << 1 << " " << (iev+1) << " " << vx << " " << vy << " " << vz << endl;

//         // Select electron or positron
//     	type = gRandom->Uniform(0., 1.);
//     	if(type < 0.5) {
//     	    pdg = 11;
//     	} else {
//             pdg = -11;
//     	}

//         // Generate momentum
//     	p     = gRandom->Uniform(2.0, 2.0);   // (2.,2.)

//         // xPos  = gRandom->Integer(nXsteps);
//         // yPos  = gRandom->Integer(nYsteps);
//         // xsign = gRandom->Integer(2);
//         //x = xRange/nXsteps*xPos*pow(-1,xsign);
//         //y = (ymax-ymin)/nYsteps*yPos+ymin;
//         //z = 1;

//         /***************** fill circles step by step ()  ************************/
//         x = -xRange+idx;
//         y = ymin+idy;
//         z = 1;
//         idx=idx+xRange/nXsteps;
//         if (x > (xRange)) {
//         	idy = idy+(ymax-ymin)/nYsteps;
//         	idx = 0;
//         	}
//         if (y > (ymax-(ymax-ymin)/nYsteps) && x > (xRange-xRange/nXsteps)) idy = 0;
//         /**************************************************************************/

//         // x=0.3;
//         // y=0.3;
//         // z=1;

//     	sqr = sqrt(x*x + y*y + z*z);
//     	px = p*x/sqr;
//     	py = p*y/sqr;
//     	pz = p*z/sqr;

//             // Write out particles information
//     	(*file) << pdg << " " << px << " " << py << " " << pz << endl;

//         // cout << "x = " << x << "; y = " << y << "; z = " << z << "; sqr = " << sqr <<"; p = " << p << "; px = " << px << "; py = " << py << "; pz = " << pz << endl;


//             // Status output
//     	if(0 == (iev%2000)) cout << iev << "   " << pdg << endl;
//     }

//     // Close output file
//     file->close();
// }



  // -----   Define beam pipe sections   --------------------------------------
  TString pipe1name = "pipe1 - vacuum chamber";
  const Int_t nSects1 = 6;
  Double_t z1[nSects1]    = { -50.,  -5.,   -5.,  230.17, 230.17, 230.87 }; // mm
  Double_t rin1[nSects1]  = {  25.,  25.,  400.,  400.,   110.,   110.   };
  Double_t rout1[nSects1] = {  25.7, 25.7, 400.7, 400.7,  400.7,  130.7  };
  TString pipe2name = "pipe2 - first window @ 220mm, h=0.7mm, R=600mm";
  const Int_t nSects2 = 7;
  Double_t z2[nSects2]    = { 220., 220.7, 221.45, 223.71, 227.49, 230.17, 230.87 }; // mm
  Double_t rin2[nSects2]  = {  18.,  18.,   30.,    60.,    90.,   105.86, 110.   };
  Double_t rout2[nSects2] = {  18.,  28.69, 39.3,   65.55,  94.14, 110.,   110.   };
  TString pipevac1name = "pipevac1";
  const Int_t nSects01 = 10;
  Double_t z01[nSects01]    = { -50., -5.,  -5., 220., 220., 220.7,  221.45, 223.71, 227.49, 230.17 }; // mm
  Double_t rin01[nSects01]  = {   0.,  0.,   0.,   0.,  18.,  28.69,  39.3,   65.55,  94.14, 110.   };
  Double_t rout01[nSects01] = {  25., 25., 400., 400., 400., 400.,   400.,   400.,   400.,   400.   };

  TString pipe3name = "pipe3 - STS section";
  const Int_t nSects3 = 4;
  Double_t z3[nSects3]    = { 220.,  500.,  1250.,   1700.  }; // mm
  Double_t rout3[nSects3] = {  18.,   18.,    55.,     74.2 };
  Double_t rin3[nSects3]; 
  TString pipevac2name = "pipevac2";
  const Int_t nSects02 = nSects3;
  Double_t z02[nSects02]    = { 220.,  500.,  1250.,   1700. }; // mm
  Double_t rin02[nSects02]  = {   0.,    0.,     0.,      0. };
  Double_t rout02[nSects02]; 

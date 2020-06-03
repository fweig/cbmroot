{
}

void rootlogon_nh()
{
  pTime=new TDatime();
  cout << " Executing rootlogon.C (nh) at " <<pTime->GetDate() << ", "<<pTime->GetTime() <<endl;
  gStyle->SetOptStat(111);
  gStyle->SetLineWidth(2.);
  gStyle->SetFrameLineWidth(2.);
  gStyle->SetTitleOffset(1.01, "x");
  gStyle->SetTitleOffset(0.9, "y");
  gStyle->SetTitleSize(0.08, "x"); // axis labels 
  gStyle->SetTitleSize(0.08, "y");
  gStyle->SetHistLineWidth(2.);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadLeftMargin(0.2);
  gStyle->SetPadBottomMargin(0.2);
  gStyle->SetTitleFontSize(0.07); // histogram title
  gStyle->SetLabelSize(0.07, "x"); //numbers
  gStyle->SetLabelSize(0.07, "y"); //numbers
  gStyle->SetLabelSize(0.05, "z"); //numbers
  gStyle->SetLabelOffset(0.02, "x"); //numbers
  gStyle->SetLabelOffset(0.02, "y"); //numbers
  gStyle->SetLabelOffset(0.02, "z"); //numbers
  gStyle->SetTextSize(0.3);
  gStyle->SetNdivisions(505, "x");
  gStyle->SetNdivisions(505, "y");
  gStyle->SetNdivisions(505, "z");
  gStyle->SetTickLength(0.06, "x");
  gStyle->SetTickLength(0.03, "y");
  gStyle->SetTickLength(0.06, "z");
  gStyle->SetPalette(1,0);
  //  gStyle->SetOptDate(23);
  //  gStyle->SetDateX(0.5);
  // gStyle->SetDateY(0.5);

  gStyle->SetLineScalePS(1.0);

  gSystem->AddIncludePath(Form("-I%s/include",gSystem->Getenv("FAIRROOTPATH")));

  gSystem->AddIncludePath(Form("-I%s/roclight",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/data",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/data/tof",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/base ",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/hadaq",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/tdc",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/tdc/v1290",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/tdc/vftx",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/tdc/trb",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/tdc/get4",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/scalers",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/scalers/triglog",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/scalers/scalormu",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/scalers/scal2014",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/scalers/orgen",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/unpMoni",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/calib",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/calib/tdc",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/calib/scaler",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/mapping",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/output",gSystem->Getenv("VMCWORKDIR")));
  gSystem->AddIncludePath(Form("-I%s/beamtime/tof/display",gSystem->Getenv("VMCWORKDIR")));

}

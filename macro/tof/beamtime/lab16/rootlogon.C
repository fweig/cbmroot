{
  cout << " Executing rootlogon.C (nh)" << endl;
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
}

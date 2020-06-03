#include <iomanip>
#include <vector>
#include "/u/jbendar/cbmroot5/CBMSRC/base/draw/CbmDrawHist.h"

void Draw_Proj()
{
	TString vmc = TString(gSystem->Getenv("VMCWORKDIR"));
//	cout << "vmc: " << vmc << endl;
	gROOT->LoadMacro(vmc + "/macro/littrack/loadlibs.C");
	loadlibs();
	gROOT->LoadMacro(vmc + "/macro/littrack/cbmrootlibs2.C");
	cbmrootlibs2();

	TH3D *Histo;
	TString Histo_Name_1 = "fhRingTrackDistVsXYTruematchPi";
	TString Histo_Name_2 = "fhRingTrackDistVsXYTruematchPrimel";

	TString Path_Histo = "/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Misaligned_5mrad_Full/merge.root";
	TFile* Histograms = new TFile(Path_Histo, "READ");
	// cout << Histograms << endl;
	Histo_1 = (TH3D*) gDirectory->Get(Histo_Name_1);
	Histo_2 = (TH3D*) gDirectory->Get(Histo_Name_2);

	Draw(Histo_1);
	Draw(Histo_2);
}

void Draw(TH3D* Histo)
{
	TCanvas *Can = new TCanvas("Can","Can", 10, 10, 1010, 510);
	Can->Divide(2,1);
	Can->cd(1);
	Histo->Draw();
	TH2D *Proj;
	Proj = DrawH3Profile(Histo, false, false, 0., 1.);
	Can->cd(2);
	Proj->Draw("colz");
}

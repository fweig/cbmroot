#include <iomanip>
#include <vector>

void DrawHistograms_New()
{
    TH1F *Ref;
    TH1F *RotD_2;
    TH1F *RotD_4;
    TH1F *RotD_4_Trig;
    TString Histo_Name = "fhBoverAEllipse";

    TString Path_Ref = "/home/jordan/Documents/CbmRoot/Output_Files/Scint_Fing/Ref/Ref.root";
    TFile* Histograms = new TFile(Path_Ref, "READ");
    // cout << HistosFile << endl;
    Ref = (TH1F*) gDirectory->Get(Histo_Name);

    /*TString Path_RotD_2 = "/home/jordan/Documents/CbmRoot/Output_Files/Scint_Fing/RotD/Two_mrad_B/RotD_2mradB.root";
    TFile* Histograms = new TFile(Path_RotD_2, "READ");
    RotD_2 = (TH1F*) gDirectory->Get(Histo_Name);*/

    TString Path_RotD_4 = "/home/jordan/Documents/CbmRoot/Output_Files/Scint_Fing/RotX/Four_mrad_F/RotX_4mradF.root";
    TFile* Histograms = new TFile(Path_RotD_4, "READ");
    RotD_4 = (TH1F*) gDirectory->Get(Histo_Name);

    TString Path_RotD_4_Trig = "/home/jordan/Documents/CbmRoot/Output_Files/Cuts/RotX_4mradF_AaxisCut/w_Trigger/RotX_4mradF_cuts_over.root";
    TFile* Histograms = new TFile(Path_RotD_4_Trig, "READ");
    RotD_4_Trig = (TH1F*) gDirectory->Get(Histo_Name);

    /*gStyle->SetOptDate(0);
     gStyle->SetOptTitle(0);
     gStyle->SetOptStat(111111);
     gStyle->SetPadBorderMode(0);
     gStyle->SetCanvasColor(10);*/
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadBottomMargin(0.15);
    //gStyle->SetPalette(1);


    // -------------------- DEFINE CANVAS -------------------- //

    TCanvas *Can = new TCanvas("Can","Can", 10, 10, 510, 510);
    TPad *cpad = new TPad("cpad","Pad",0.1,0.1,0.9,0.9);
    gPad->SetLogy(0);
    gPad->SetGridx(1);
    //gPad->SetBorderMode(1); // Change the border color
    gPad->SetBorderSize(2);
    gPad->SetGridy(1);
    gPad->SetRightMargin(0.07);//

    TH1F* frame = cpad->DrawFrame(0.65,0.,1.,14.); // Set x and y scales
    frame->SetLabelSize(0.03,"X");
    frame->SetTitleSize(0.04,"X");
    frame->SetTitleOffset(0.75,"X");
    frame->SetXTitle("B over A");
    frame->SetLabelSize(0.03,"Y");
    frame->SetTitleSize(0.04,"Y");
    frame->SetTitleOffset(1.3,"Y");
    frame->SetYTitle("A.U.");
    frame->GetXaxis()->SetNdivisions(505, kTRUE);
    frame->GetYaxis()->SetNdivisions(511, kTRUE);
    frame->Draw("mMrRuo");

    TLegend* LEG= new TLegend(0.2,0.6,0.55,0.88); // Set legend position
    LEG->SetBorderSize(1);
    LEG->SetFillColor(0);
    LEG->SetMargin(0.2);
    LEG->SetTextSize(0.03);
    LEG->SetHeader("B over A histogram");

    // -------------------- NORMALIZE HISTOGRAMS -------------------- //
    Double_t norm = 1;
    Ref->Scale(norm/Ref->Integral("width"));
    //RotD_2->Scale(norm/RotD_2->Integral("width"));
    RotD_4->Scale(norm/RotD_4->Integral("width"));
    RotD_4_Trig->Scale(norm/RotD_4_Trig->Integral("width"));

    // -------------------- DRAW HISTOGRAMS & LEGENDS -------------------- //

    int colorind = 1;
    char leg[128];

    Ref->SetLineColor(colorind);
    Ref->SetLineWidth(2);
    //Ref->Rebin(2);
    Ref->Draw("same");
    Ref->Draw("same");
    sprintf(leg, "Ref");
    LEG->AddEntry(Ref, leg, "l");
    colorind++;
    /*RotD_2->SetLineColor(colorind);
    RotD_2->SetLineWidth(2);
    //RotD_2->Rebin(2);
    RotD_2->Draw("same");
    sprintf(leg, "RotD_2mradB");
    LEG->AddEntry(RotD_2, leg, "l");
    colorind++;*/
    RotD_4->SetLineColor(colorind);
    RotD_4->SetLineWidth(2);
    //RotD_4->Rebin(2);
    RotD_4->Draw("same");
    sprintf(leg, "RotX_4mradF");
    LEG->AddEntry(RotD_4, leg, "l");
    colorind++;
    RotD_4_Trig->SetLineColor(colorind);
    RotD_4_Trig->SetLineWidth(2);
    //RotD_4_Trig->Rebin(2);
    RotD_4_Trig->Draw("same");
    sprintf(leg, "RotX_4mradF_cuts");
    LEG->AddEntry(RotD_4_Trig, leg, "l");

    LEG->Draw();

}

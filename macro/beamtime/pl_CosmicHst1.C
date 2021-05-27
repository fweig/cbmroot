void pl_CosmicHst1(Int_t iNSt = 6, Int_t iOpt = 0, Int_t iSel = 0, Int_t i1D = 0, Double_t dYmax = 0.)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 400, 1000);
  can->Divide(1, 6);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kFALSE);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH1* h1;
  TH2* h2;
  TH3* h3;
  TH3* h3f;

  TString hname;
  TString cOpt;
  TString c2D;
  TString c1D;
  TString chopt;

  const Int_t ColMap[10]  = {1, 2, 3, 4, 6, 7, 8, 9, 41, 46};
  const Int_t iPadLoc[10] = {1, 6, 3, 2, 4, 5, 7, 3, 6, 9};
  //const Int_t iPadLoc[10]={1,2,3,4,5,6,7,3,6,9};
  const Int_t SmT[10]  = {9, 9, 9, 9, 9, 9, 1, 1, 1, 1};
  const Int_t iSm[10]  = {0, 0, 1, 1, 2, 2, 2, 0, 1, 2};
  const Int_t iRpc[10] = {0, 1, 0, 1, 0, 1, 0, 2, 2, 2};

  for (Int_t iSt = 0; iSt < iNSt; iSt++) {
    switch (iOpt) {
      case 0:
        chopt = "hPullX_Station";
        hname = Form("%s_%d", chopt.Data(), iSt);
        break;
      case 1:
        chopt = "hPullY_Station";
        hname = Form("%s_%d", chopt.Data(), iSt);
        break;
      case 2:
        chopt = "hPullZ_Station";
        hname = Form("%s_%d", chopt.Data(), iSt);
        break;
      case 3:
        chopt = "hPullT_Station";
        hname = Form("%s_%d", chopt.Data(), iSt);
        break;
      case 4:
        chopt = "hPullTB_Station";
        hname = Form("%s_%d", chopt.Data(), iSt);
        break;

      case 10:
        chopt = "Efficiency";
        hname = Form("%s_%d", chopt.Data(), iSt);
        break;
      case 11:
        chopt = "Total";
        hname = Form("%s_%d", chopt.Data(), iSt);
        break;
      case 12:
        chopt = "Missed";
        hname = Form("%s_%d", chopt.Data(), iSt);
        break;
      case 13:
        chopt = "Acc";
        hname = Form("%s_%d", chopt.Data(), iSt);
        break;
      case 14: hname = Form("hXY_AllTracks_%d", iSt); break;


      case 20: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Size", SmT[iSt], iSm[iSt], iRpc[iSt]); break;
      case 21: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Pos", SmT[iSt], iSm[iSt], iRpc[iSt]); break;
      case 22: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Pos_py", SmT[iSt], iSm[iSt], iRpc[iSt]); break;
      case 23: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Tot", SmT[iSt], iSm[iSt], iRpc[iSt]); break;
      case 24: hname = Form("cl_SmT%d_sm%03d_rpc%03d_AvWalk", SmT[iSt], iSm[iSt], iRpc[iSt]); break;
      case 25: hname = Form("cl_SmT%d_sm%03d_rpc%03d_AvLnWalk", SmT[iSt], iSm[iSt], iRpc[iSt]); break;
      case 26: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Mul", SmT[iSt], iSm[iSt], iRpc[iSt]); break;
      case 27: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Trms", SmT[iSt], iSm[iSt], iRpc[iSt]); break;
      case 28: hname = Form("cl_SmT%d_sm%03d_rpc%03d_DelPos", SmT[iSt], iSm[iSt], iRpc[iSt]); break;
      case 29: hname = Form("cl_SmT%d_sm%03d_rpc%03d_DelTOff", SmT[iSt], iSm[iSt], iRpc[iSt]); break;

      case 30: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_Size", SmT[iSt], iSm[iSt], iRpc[iSt], iSel); break;
      case 31: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_Pos", SmT[iSt], iSm[iSt], iRpc[iSt], iSel); break;
      case 32: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_Pos_py", SmT[iSt], iSm[iSt], iRpc[iSt], iSel); break;
      case 33: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_TOff", SmT[iSt], iSm[iSt], iRpc[iSt], iSel); break;
      case 34: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_TOff_pfx", SmT[iSt], iSm[iSt], iRpc[iSt], iSel); break;
      case 35: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_Tot", SmT[iSt], iSm[iSt], iRpc[iSt], iSel); break;
      case 36: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_AvWalk", SmT[iSt], iSm[iSt], iRpc[iSt], iSel); break;
      case 37: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_Mul", SmT[iSt], iSm[iSt], iRpc[iSt], iSel); break;
      case 38: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_DelTof", SmT[iSt], iSm[iSt], iRpc[iSt], iSel); break;
      case 39: hname = Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_dXdY", SmT[iSt], iSm[iSt], iRpc[iSt], iSel); break;

      case 40: hname = Form("hXY_DX_%d_pyx_pfy", iSt); break;
      case 41: hname = Form("hXY_DY_%d_pyx_pfy", iSt); break;
      case 42: hname = Form("hXY_DT_%d_pyx_pfy", iSt); break;
      case 43: hname = Form("hXY_TOT_%d_pyx_pfy", iSt); break;
      case 44: hname = Form("hXY_CSZ_%d_pyx_pfy", iSt); break;

      default: cout << "iOpt mode not implemented " << endl; return;
    }


    can->cd(iPadLoc[iSt]);
    h2 = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      switch (i1D) {
        case 0: h2->Draw("colz"); break;
        case 1:
          h2->Draw("colz");
          gPad->SetLogz();
          break;
        case 11: h2->Draw(); break;
        case 12:
          h2->Draw();
          gPad->SetLogy();
          break;
        case 13: h2->Draw("same"); break;
      }
      if (dYmax > 0.) {
        h2->SetMinimum(-dYmax);
        h2->SetMaximum(dYmax);
      }
    }
    else
      cout << hname << " not found" << endl;
  }

  can->SaveAs("pl_CosmicHst1.pdf");
}

void inspect_sync_histo(TString hFile        = "data/HistosMonitorTofSync.root",
                        Double_t MaxMismatch = 1.) {
  using namespace std;

  TFile* fHistFile = TFile::Open(hFile, "READ");
  TString hname;
  TH1* h1;
  TH1* h23;
  TH1* h24;
  TH2* h2;
  const Int_t NGdpb       = 12;
  const Int_t NGbTx       = 6;
  Int_t iInit             = 0;
  Int_t NMissingGbtx      = 0;
  Bool_t bSectorFW[NGdpb] = {NGdpb * kFALSE};
  // reinitialize Gbtx
  if (1)
    for (Int_t iGdpb = 0; iGdpb < NGdpb; iGdpb++) {
      hname = Form("hGdpbGet4MessType_%02d", iGdpb);
      h2    = (TH2*) fHistFile->FindObjectAny(hname);
      if (NULL == h2) {
        cout << " Histo " << hname << " not found " << endl;
        continue;
      }
      h1          = h2->ProjectionX("_px", 3, 3);  // get scm line
      Int_t NGet4 = 0;
      for (Int_t iGbtx = 0; iGbtx < NGbTx; iGbtx++) {
        Int_t iStart      = iGbtx * 40;
        Int_t NGet4_begin = NGet4;
        for (Int_t iGet4 = iStart; iGet4 < iStart + 32; iGet4++) {
          if (h1->GetBinContent(iGet4) > 0) NGet4++;
        }
        if (NGet4 == NGet4_begin) NMissingGbtx++;
      }

      TDatime* pTime = new TDatime();
      cout << Form("%d,%d", pTime->GetDate(), pTime->GetTime()) << ": AFCK "
           << iGdpb << " has " << NGet4 << " active GET4s" << endl;
      if (NGet4 < 130) {  // max: 6*24 = 144
        Int_t iSec = iGdpb + 13;
        gSystem->Exec(Form("/home/cbm/bin/prog_etof_sector.sh %d > "
                           "/home/cbm/starsoft/dpbcontrols/LOG/PROG%d ",
                           iSec,
                           iSec));
        bSectorFW[iGdpb] = kTRUE;
      } else {
        ;
      }
    }

  hname = Form("hGdpbEpochFlags");
  h2    = (TH2*) fHistFile->FindObjectAny(hname);
  if (NULL == h2) {
    cout << " Histo " << hname << " not found " << endl;
    return;
  }

  h24 = h2->ProjectionX("_px", 4, 4);  // get mismatch line
  cout << "<I> stop fles " << endl;
  gSystem->Exec(
    Form("cd /home/cbm/starsoft/startup_scripts; ./DisableDataOut.sh > "
         "/dev/null; ./stop_fles.sh > /dev/null "));

  Int_t NBad = 0;
  for (Int_t iGdpb = 0; iGdpb < NGdpb; iGdpb++) {
    Int_t iSec = iGdpb + 13;
    if (h24->GetBinContent(iGdpb + 1) > MaxMismatch || bSectorFW[iGdpb]) {
      TDatime* pTime = new TDatime();
      cout << Form("%d,%d", pTime->GetDate(), pTime->GetTime())
           << Form(": Init Gdpb %d, sector %d with %d mismatches",
                   iGdpb,
                   iSec,
                   (Int_t) h24->GetBinContent(iGdpb + 1))
           << endl;

      gSystem->Exec(Form("cd /home/cbm/starsoft/dpbcontrols; nohup "
                         "./start_etof_sector.sh %d  > ./LOG/EtofSector%d & ",
                         iSec,
                         iSec));

      NBad++;
    } else {
      gSystem->Exec(
        Form("cd /home/cbm/starsoft/dpbcontrols; rm ./LOG/BAD%d &> /dev/null",
             iSec));
    }
  }
  if (NBad == 0) {
    gSystem->Exec("rm /tmp/NoSync");  // done
  } else {
    cout << " Found " << NBad << " bad sectors " << endl;
  }

  gSystem->Exec(Form("echo %d > /tmp/MissingGbtx",
                     NMissingGbtx));  // communicate status to shell script

  gSystem->Exec(Form("/home/cbm/starsoft/dpbcontrols/"
                     "afck_wait_configured.sh "));  // wait for completion
}

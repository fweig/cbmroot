#include "CbmTrbCalibrator.h"

#include "CbmRichTrbDefines.h"

#include "FairLogger.h"
#include "TFile.h"
#include "TCanvas.h"
#include "CbmDrawHist.h"

CbmTrbCalibrator* CbmTrbCalibrator::fInstance = 0;

// By default the calibration tables are imported from the file
CbmTrbCalibrator::CbmTrbCalibrator()
  : TObject(),
    fCalibMode(etn_NOCALIB),
    fTablesCreated(kFALSE),
    fCalibrationPeriod(10000),
    fTRBroot(NULL),
    fInputFilename(""),
    fInputCorrFilename(""),
    fCalibrationDoneHisto(),
    fhLeadingFineBuffer(),
    fhLeadingFine(),
    fhCalcBinWidth(),
    fhCalBinTime(),
    fFTcounter(),
    fhFTcounter()
{
	for (Int_t i=0; i<32*4; i++) fCorrections[i] = 0.;
}

CbmTrbCalibrator::~CbmTrbCalibrator()
{
}

CbmTrbCalibrator* CbmTrbCalibrator::Instance()
{
   if (!fInstance) fInstance = new CbmTrbCalibrator();
   return fInstance;
}

void CbmTrbCalibrator::Export(const char* filename)
{
/*
   TH1I* calibrationEntries = new TH1I("abc", "abc", TRB_TDC3_CHANNELS*TRB_TDC3_NUMTDC*TRB_TDC3_NUMBOARDS, 0, TRB_TDC3_CHANNELS*TRB_TDC3_NUMTDC*TRB_TDC3_NUMBOARDS);
   for (Int_t b=0; b<TRB_TDC3_NUMBOARDS; ++b) {
      for (Int_t t=0; t<TRB_TDC3_NUMTDC; ++t) {
         for (Int_t i=0; i<TRB_TDC3_CHANNELS; ++i) {
            calibrationEntries->SetBinContent(b*TRB_TDC3_NUMTDC*TRB_TDC3_CHANNELS + t*TRB_TDC3_CHANNELS + i, fFTcounter[b][t][i]);
         }
      }
   }
   DrawH1(calibrationEntries);
*/

   TDirectory* current = gDirectory;
   TFile* old = gFile;
   TFile* file = new TFile(filename, "RECREATE");

   if (file == NULL || !(file->IsOpen()))
   {
      // error
   } else {
      file->cd();

      if (fTRBroot) {
         //gROOT->GetRootFolder()->ls();
         fTRBroot->Write();
      }

      file->Close();
      current->cd();
   }

   delete file;

   gFile = old;
   gDirectory = current;
}

// This method will overwrite the existing tables
void CbmTrbCalibrator::Import(/*const char* filename*/)
{
   LOG(info) << "Importing calibration tables from " << fInputFilename;

   TDirectory* current = gDirectory;
   TFile* old = gFile;
   TFile* file = new TFile(fInputFilename, "READ");

   if (file == NULL || !(file->IsOpen()))
   {
      // error
   } else {

      TString obname;
      TFolder* TRBfolder = (TFolder*)file->Get("TRB");

      for (Int_t b=0; b<TRB_TDC3_NUMBOARDS; ++b) {
         obname.Form("TRB%02d", b+1);
         TFolder* perTRBfolder = (TFolder*)(TRBfolder->FindObject(obname));

         for (Int_t t=0; t<TRB_TDC3_NUMTDC; ++t) {
            obname.Form("TDC%d", t);
            TFolder* perTDCfolder = (TFolder*)(perTRBfolder->FindObject(obname));

            for (Int_t i=0; i<TRB_TDC3_CHANNELS; ++i) {
               obname.Form("FineTimeBuffer_%02d_%d_%02d", b+1, t, i);
               TH1D* curHisto = (TH1D*)(perTDCfolder->FindObject(obname));
               fhLeadingFineBuffer[b][t][i] = curHisto;

               obname.Form("FineTime_%02d_%d_%02d", b+1, t, i);
               curHisto = (TH1D*)(perTDCfolder->FindObject(obname));
               fhLeadingFine[b][t][i] = curHisto;

               obname.Form("BinWidth_%02d_%d_%02d", b+1, t, i);
               curHisto = (TH1D*)(perTDCfolder->FindObject(obname));
               fhCalcBinWidth[b][t][i] = curHisto;

               obname.Form("CalTableBinTime_%02d_%d_%02d", b+1, t, i);
               curHisto = (TH1D*)(perTDCfolder->FindObject(obname));
               fhCalBinTime[b][t][i] = curHisto;
            }

            obname.Form("CalibrationDone_%02d_%d", b+1, t);
            TH1C* curHisto2 = (TH1C*)(perTRBfolder->FindObject(obname));
            fCalibrationDoneHisto[b][t] = curHisto2;
         }
      }

   }

   delete file;

   gFile = old;
   gDirectory = current;

   fTablesCreated = kTRUE;

   LOG(info) << "Calibration tables imported.";

   this->ImportCorrections();
}

void CbmTrbCalibrator::ImportCorrections()
{
   FILE* f = fopen (fInputCorrFilename.Data(), "r");
   if (f == NULL) {
      LOG(error) << "Failed to open corrections table. Using zero corrections.";
      return;
   }
   for (int tdc=0; tdc<16*4; tdc++) {
      for (int ch=0; ch<32; ch+=2) {
            int read = fscanf (f, "%lf\t%lf\n", &fCorrections[tdc*32+ch], &fCorrections[tdc*32+ch+1]);
            if ( read != 2 ) LOG(error) << "Failed to read corrections for tdc "
                << tdc << " ch " << ch;
      }
   }
   fclose(f);
   LOG(info) << "Correction tables imported.";
}

// inTDCid - raw id, looks like 0x****    TDC - from 0 to 3 incl.
// inCHid - raw id of the channel - the same as CH - number of the channel from 0 to 32 incl.
void CbmTrbCalibrator::AddFineTime(UShort_t inTDCid, UShort_t inCHid, UShort_t fineCnt)
{
   // for etn_NOCALIB and etn_IDEAL this method is useless

   //printf ("       CbmTrbCalibrator::AddFineTime      TDC %x ch %d\n", inTDCid, inCHid);

   if (fCalibMode == etn_ONLINE)
   {
      if (!fTablesCreated) GenHistos();

      UShort_t TRB = ((inTDCid >> 4) & 0x00FF) - 1;
      UShort_t TDC = (inTDCid & 0x000F);
      UShort_t CH = inCHid;

      LOG(debug3) << "AddFineTime: TDC" << std::hex << inTDCid << std::dec << " CH" << CH;

      fhLeadingFineBuffer[TRB][TDC][CH]->Fill(fineCnt);
      fhLeadingFine[TRB][TDC][CH]->Fill(fineCnt);

      fFTcounter[TRB][TDC][CH]++;
      fhFTcounter[TRB]->SetBinContent(CH+1, TDC+1, fhFTcounter[TRB]->GetBinContent(CH+1,TDC+1)+1); //FIXME

      if (fFTcounter[TRB][TDC][CH] >= fCalibrationPeriod
         && fCalibrationDoneHisto[TRB][TDC]->GetBinContent(CH+1) == 0)
      {
         DoCalibrate(inTDCid, CH);
         fFTcounter[TRB][TDC][CH] = 0; //TODO maybe comment?
      }
   }
}

void CbmTrbCalibrator::ForceCalibration()
{
   LOG(info) << "Force calibration of all the channels of all TDCs which have at least one entry!";
   UShort_t TDCid;
   for (UInt_t TRB=0; TRB<TRB_TDC3_NUMBOARDS; TRB++) {
      for (UShort_t TDC=0; TDC<=3; TDC++) {     // from 0x0??0 to 0x0??3
         for (UShort_t CH=0; CH<=32; CH++) {
            TDCid = (TRB+1)*16+TDC;
            if (fFTcounter[TRB][TDC][CH] > 0) {
               DoCalibrate(TDCid, CH);
               fFTcounter[TRB][TDC][CH] = 0;
            }
         }
      }
   }
}

void CbmTrbCalibrator::DoCalibrate(UShort_t inTDCid, UShort_t inCHid)
{
   LOG(debug) << "DoCalibrate: TDC" << std::hex << inTDCid << std::dec << " CH" << inCHid;

   UShort_t TRB = ((inTDCid >> 4) & 0x00FF) - 1;
   UShort_t TDC = (inTDCid & 0x000F);
   UShort_t CH = inCHid;

   TH1* hfinecal = 0;
   UInt_t sum;
   Double_t binwidth;
   Double_t binhits;

   hfinecal = fhLeadingFineBuffer[TRB][TDC][CH];
   sum = hfinecal->GetEntries();

   for (Int_t fb=0; fb<TRB_TDC3_FINEBINS; ++fb)
   {
      binwidth = 1.;
      binhits = hfinecal->GetBinContent(fb + 1);
      if (sum)
         binwidth = (TRB_TDC3_COARSEUNIT * binhits) / (Double_t)sum;
      fhCalcBinWidth[TRB][TDC][CH]->SetBinContent(fb + 1, binwidth);

      Double_t calbintime = 1.;
      if (fb == 0)
         calbintime = fhCalcBinWidth[TRB][TDC][CH]->GetBinContent(fb + 1) / 2;
      else
         calbintime = fhCalBinTime[TRB][TDC][CH]->GetBinContent(fb) +
                     (fhCalcBinWidth[TRB][TDC][CH]->GetBinContent(fb) + fhCalcBinWidth[TRB][TDC][CH]->GetBinContent(fb + 1)) / 2;

      fhCalBinTime[TRB][TDC][CH]->SetBinContent(fb + 1, calbintime);
   }

   fhLeadingFineBuffer[TRB][TDC][CH]->Reset();

   fCalibrationDoneHisto[TRB][TDC]->SetBinContent(CH+1, 1);
}

Double_t CbmTrbCalibrator::GetFineTimeCalibrated(UShort_t inTDCid, UShort_t inCHid, UShort_t fineCnt)
{
   // printf ("TDC %x CH %d fine %x\n", inTDCid, inCHid, fineCnt);

   // hard-code for special channel number 0 which is a reference channel.
   // for this channel the ideal calibration is used
   if (inCHid == 0) return GetAlmostLinearCalibratedFT(fineCnt);

   UShort_t TRB = ((inTDCid >> 4) & 0x00FF) - 1;
   UShort_t TDC = (inTDCid & 0x000F);

   switch (fCalibMode) {
      case etn_IMPORT:
         if (!fTablesCreated) this->Import();
         if (fCalibrationDoneHisto[TRB][TDC]->GetBinContent(inCHid+1) == 1)
            return GetRealCalibratedFT(inTDCid, inCHid, fineCnt);
         else
            return GetAlmostLinearCalibratedFT(fineCnt); // if the channel is not calibrated return time as if it was ideally calibrated
         break;
      case etn_ONLINE:
         if (!fTablesCreated) this->GenHistos();
         if (fCalibrationDoneHisto[TRB][TDC]->GetBinContent(inCHid+1) == 1)
            return GetRealCalibratedFT(inTDCid, inCHid, fineCnt);
         else
            return GetAlmostLinearCalibratedFT(fineCnt); // if the channel is not calibrated return time as if it was ideally calibrated
         break;
      case etn_NOCALIB:
         return GetLinearCalibratedFT(fineCnt);
         break;
      case etn_IDEAL:
         return GetAlmostLinearCalibratedFT(fineCnt);
         break;
      case etn_NOFINE:
         return 0.;
         break;
      default:
        return 0.;
        break;
   }
}

// Per-channel corrections are introduced here
Double_t CbmTrbCalibrator::GetFullTime(UInt_t inTDCid, UInt_t inCHid, UInt_t epoch, UInt_t coarse, UInt_t fine)
{
   Double_t fullTimeCalibrated = epoch * TRB_TDC3_COARSEUNIT * TRB_TDC3_COARSEBINS + coarse * TRB_TDC3_COARSEUNIT -
                                 this->GetFineTimeCalibrated(inTDCid, inCHid, fine);

   UInt_t TDCindex = ((((inTDCid >> 4) & 0x00ff) - 1) * 4 + (inTDCid & 0x000f));
   Double_t correction = 0.;
   if (inCHid != 0) {
      correction = fCorrections[TDCindex*32+inCHid-1];
   }

   return fullTimeCalibrated + correction;
}

// etn_NOCALIB
Double_t CbmTrbCalibrator::GetLinearCalibratedFT(UShort_t fineCnt)
{
   return (Double_t)fineCnt * 5. / 512.; // counter * 5 ns / 512.  512 because there are 10 bits for fine time counter.
}

// etn_IDEAL
Double_t CbmTrbCalibrator::GetAlmostLinearCalibratedFT(UShort_t fineCnt)
{
   UShort_t tailSize = 30;

   if (fineCnt < tailSize) {    // Left tail - 0 ns
      return 0.;
   } else if (fineCnt > 512-tailSize) {     // Right tail - 5 ns
      return 5.;
   } else {                   // Middle - linear function
      return (Double_t)(fineCnt-tailSize) * 5. / (512. - 2.*tailSize);
   }
}

// etn_IMPORT || etn_ONLINE
Double_t CbmTrbCalibrator::GetRealCalibratedFT(UShort_t inTDCid, UShort_t inCHid, UShort_t fineCnt)
{
   UShort_t TRB = ((inTDCid >> 4) & 0x00FF) - 1;
   UShort_t TDC = (inTDCid & 0x000F);
   return fhCalBinTime[TRB][TDC][inCHid]->GetBinContent(fineCnt+1);
}

void CbmTrbCalibrator::GenHistos()
{
   LOG(debug2) << "CbmTrbCalibrator::GenHistos";

   TH1::AddDirectory(kFALSE); // Magic line which helps from crashes on new OS/gcc/fairsoft

   TString obname;
   TString obtitle;
   TString dirname;

   UInt_t tbins = TRB_TDC3_FINEBINS;
   Int_t trange = TRB_TDC3_FINEBINS;

   fTRBroot = gROOT->GetRootFolder()->AddFolder("TRB", "TRB root folder");
   gROOT->GetListOfBrowsables()->Add(fTRBroot, "TRB");

   TFolder* TRBfolders[TRB_TDC3_NUMBOARDS];
   TFolder* TDCfolders[TRB_TDC3_NUMBOARDS][TRB_TDC3_NUMTDC];

   // TRBs are numerated starting from 1, but the arrays of histos are from 0 (as in any C++ code)
   // TDCs are numerated from 0. One TRB has TDCs {0,1,2,3}.
   // Channels are numerated from 0 and there are total 33 channels for each TDC. 0-th channel is the reference channel.

   for (Int_t b=0; b<TRB_TDC3_NUMBOARDS; ++b) {

      obname.Form("TRB%02d", b+1);
      obtitle.Form("TRB%02d", b+1);
      TRBfolders[b] = fTRBroot->AddFolder(obname, obtitle);

      obname.Form("fhFTcounter_%02d", b+1);
      obtitle.Form("Counter of entries for all the channels of all the TDCs of TRB %d", b+1);
      fhFTcounter[b] = new TH2D(obname.Data(), obtitle.Data(), TRB_TDC3_CHANNELS, 0, TRB_TDC3_CHANNELS, TRB_TDC3_NUMTDC, 0, TRB_TDC3_NUMTDC);
      TRBfolders[b]->Add(fhFTcounter[b]);


      for (Int_t t=0; t<TRB_TDC3_NUMTDC; ++t) {

         obname.Form("TDC%d", t);
         obtitle.Form("TDC%d", t);
         TDCfolders[b][t] = TRBfolders[b]->AddFolder(obname, obtitle);

         for (Int_t i=0; i<TRB_TDC3_CHANNELS; ++i) {

            obname.Form("FineTimeBuffer_%02d_%d_%02d", b+1, t, i);
            obtitle.Form("Fine time Leading edge TRB %d TDC %d Channel %02d (calibration buffer)", b+1, t, i);
            fhLeadingFineBuffer[b][t][i] = new TH1D(obname.Data(), obtitle.Data(), tbins, 0, trange);
            TDCfolders[b][t]->Add(fhLeadingFineBuffer[b][t][i]);

            obname.Form("FineTime_%02d_%d_%02d", b+1, t, i);
            obtitle.Form("Fine time Leading edge TRB %02d TDC %02d Channel %02d", b+1, t, i);
            fhLeadingFine[b][t][i] = new TH1D(obname.Data(), obtitle.Data(), tbins, 0, trange);
            TDCfolders[b][t]->Add(fhLeadingFine[b][t][i]);

            obname.Form("BinWidth_%02d_%d_%02d", b+1, t, i);
            obtitle.Form("Binwidth Histogram of TRB %02d TDC %d Channel %02d", b+1, t, i);
            fhCalcBinWidth[b][t][i] = new TH1D(obname.Data(), obtitle.Data(), TRB_TDC3_FINEBINS, 0, TRB_TDC3_FINEBINS);

            //! Reset time bin correction arrays
            for (int fb=0; fb<TRB_TDC3_FINEBINS; ++fb)
               fhCalcBinWidth[b][t][i]->SetBinContent(fb + 1, 1.0);

            TDCfolders[b][t]->Add(fhCalcBinWidth[b][t][i]);

            obname.Form("CalTableBinTime_%02d_%d_%02d", b+1, t, i);
            obtitle.Form("Calibration Table of TRB %02d TDC %d Channel %02d", b+1, t, i);
            fhCalBinTime[b][t][i] = new TH1D(obname.Data(), obtitle.Data(), TRB_TDC3_FINEBINS, 0, TRB_TDC3_FINEBINS);

            //! Reset time bin correction arrays
            for (int fb=0; fb<TRB_TDC3_FINEBINS; ++fb)
               fhCalBinTime[b][t][i]->SetBinContent(fb + 1, (Double_t)(TRB_TDC3_COARSEUNIT * fb) / (Double_t)TRB_TDC3_FINEBINS);

            TDCfolders[b][t]->Add(fhCalBinTime[b][t][i]);

         } // Channels

         obname.Form("CalibrationDone_%02d_%d", b+1, t);
         obtitle.Form("Flag indicating that calibration is done for TRB %02d TDC %d", b+1, t);
         fCalibrationDoneHisto[b][t] = new TH1C(obname.Data(), obtitle.Data(), TRB_TDC3_CHANNELS, 0, TRB_TDC3_CHANNELS);

         TRBfolders[b]->Add(fCalibrationDoneHisto[b][t]);

      } // TDCs
   } // TRBs

   LOG(info) << "Histos created.";
   fTablesCreated = kTRUE;

} // end of method

//TODO draw all 17, not 16 tables
void CbmTrbCalibrator::Draw(Option_t*)
{
   TCanvas* c[4];
   for (int t=0; t<4; t++)
   {
      TString obname;
      obname.Form("Channels calibration flags of TDC%d of all TRBs", t);
      c[t] = new TCanvas(obname, obname, 1000, 1000);
      c[t]->Divide(4, 4);
      for (int b=0; b<16; b++) {
         c[t]->cd(b+1);
         DrawH1(fCalibrationDoneHisto[b][t]);
      }
   }
}

ClassImp(CbmTrbCalibrator)

#include "CbmRichTrbUnpackQa.h"
#include "FairRootManager.h"
#include "FairLogger.h"

#include "CbmRichTrbDefines.h"
#include "CbmRichTrbDigi.h"
#include "CbmRichTrbParam.h"

#include "CbmRichHit.h"
#include "CbmRichHitInfo.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TF1.h"
#include "TClonesArray.h"

#include "CbmUtils.h"

// Uncomment if you want to have excessive printout (do not execute on many events, may produce Gb's of output)
//#define DEBUGPRINT

CbmRichTrbUnpackQa::CbmRichTrbUnpackQa()
  : FairTask(),
	fRichTrbDigi(NULL),
	fRichHits(NULL),
	fRichHitInfos(NULL),
	fEventNum(0),
	fRunTitle(""),
	fOutputDir(""),
	fDrawHist(kFALSE),
	fHM(NULL),
	numOfPairs(0),
	numOfSingleEdges(0),
	numOfSingleEdgesSync(0),
	lastBaseTimestamp(0.)
{
}

CbmRichTrbUnpackQa::~CbmRichTrbUnpackQa()
{
}

InitStatus CbmRichTrbUnpackQa::Init()
{
	FairRootManager* manager = FairRootManager::Instance();
	fRichTrbDigi = (TClonesArray*)manager->GetObject("CbmRichTrbDigi");
	if (NULL == fRichTrbDigi) { Fatal("CbmRichTrbUnpackQa::Init","No CbmRichTrbDigi array!"); }

	fRichHits = new TClonesArray("CbmRichHit", 10);
	manager->Register("RichHit", "RICH", fRichHits, IsOutputBranchPersistent("RichHit"));

	fRichHitInfos = new TClonesArray("CbmRichHitInfo", 10);
	manager->Register("RichHitInfo","RICH", fRichHitInfos, IsOutputBranchPersistent("RichHitInfo"));

	InitHist();

	return kSUCCESS;
}

void CbmRichTrbUnpackQa::InitHist()
{
   if (fDrawHist)
   {
	   fHM = new CbmHistManager();

	   fHM->Create1<TH1D>("fhNofDigisInEvent", "fhNofDigisInEvent;Number of digis in event;Entries", 2000, 0, 2000);
	   fHM->Create1<TH1D>("fhTDCmessages", "fhTDCmessages;Number of messages for a given TDC;Entries", 105, -5, 100);

       Double_t K = 1.;

	   fHM->Create1<TH1D>("fhTOT_channel_1-2",   "fhTOT_channel_1-2;Time-over-threshold channels 1-2;Entries",     20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);
	   fHM->Create1<TH1D>("fhTOT_channel_3-4",   "fhTOT_channel_3-4;Time-over-threshold channels 3-4;Entries",     20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);
	   fHM->Create1<TH1D>("fhTOT_channel_5-6",   "fhTOT_channel_5-6;Time-over-threshold channels 5-6;Entries",     20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);
	   fHM->Create1<TH1D>("fhTOT_channel_7-8",   "fhTOT_channel_7-8;Time-over-threshold channels 7-8;Entries",     20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);
	   fHM->Create1<TH1D>("fhTOT_channel_9-10",  "fhTOT_channel_9-10;Time-over-threshold channels 9-10;Entries",   20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);
	   fHM->Create1<TH1D>("fhTOT_channel_11-12", "fhTOT_channel_11-12;Time-over-threshold channels 11-12;Entries", 20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);
	   fHM->Create1<TH1D>("fhTOT_channel_13-14", "fhTOT_channel_13-14;Time-over-threshold channels 13-14;Entries", 20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);
	   fHM->Create1<TH1D>("fhTOT_channel_15-16", "fhTOT_channel_15-16;Time-over-threshold channels 15-16;Entries", 20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);
	   fHM->Create1<TH1D>("fhTOT_channel_17-18", "fhTOT_channel_17-18;Time-over-threshold channels 17-18;Entries", 20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);
	   fHM->Create1<TH1D>("fhTOT_channel_19-20", "fhTOT_channel_19-20;Time-over-threshold channels 19-20;Entries", 20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);
	   fHM->Create1<TH1D>("fhTOT_channel_21-22", "fhTOT_channel_21-22;Time-over-threshold channels 21-22;Entries", 20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);
	   fHM->Create1<TH1D>("fhTOT_channel_23-24", "fhTOT_channel_23-24;Time-over-threshold channels 23-24;Entries", 20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);
	   fHM->Create1<TH1D>("fhTOT_channel_25-26", "fhTOT_channel_25-26;Time-over-threshold channels 25-26;Entries", 20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);
	   fHM->Create1<TH1D>("fhTOT_channel_27-28", "fhTOT_channel_27-28;Time-over-threshold channels 27-28;Entries", 20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);
	   fHM->Create1<TH1D>("fhTOT_channel_29-30", "fhTOT_channel_29-30;Time-over-threshold channels 29-30;Entries", 20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);
	   fHM->Create1<TH1D>("fhTOT_channel_31-32", "fhTOT_channel_31-32;Time-over-threshold channels 31-32;Entries", 20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -K*NEGATIVEPAIRWINDOW, K*POSITIVEPAIRWINDOW);

	   Double_t minWin = -1000.;
	   Double_t maxWin = 1000.;

	   fHM->Create1<TH1D>("fhDelay_channel_1",  "fhDelay_channel_1;Delay of the PMT front from the 'base' front - channels 1;Entries",  5*(maxWin-minWin), minWin, maxWin);
	   fHM->Create1<TH1D>("fhDelay_channel_3",  "fhDelay_channel_3;Delay of the PMT front from the 'base' front - channels 3;Entries",  5*(maxWin-minWin), minWin, maxWin);
	   fHM->Create1<TH1D>("fhDelay_channel_5",  "fhDelay_channel_5;Delay of the PMT front from the 'base' front - channels 5;Entries",  5*(maxWin-minWin), minWin, maxWin);
	   fHM->Create1<TH1D>("fhDelay_channel_7",  "fhDelay_channel_7;Delay of the PMT front from the 'base' front - channels 7;Entries",  5*(maxWin-minWin), minWin, maxWin);
	   fHM->Create1<TH1D>("fhDelay_channel_9",  "fhDelay_channel_9;Delay of the PMT front from the 'base' front - channels 9;Entries",  5*(maxWin-minWin), minWin, maxWin);
	   fHM->Create1<TH1D>("fhDelay_channel_11", "fhDelay_channel_11;Delay of the PMT front from the 'base' front - channels 11;Entries", 5*(maxWin-minWin), minWin, maxWin);
	   fHM->Create1<TH1D>("fhDelay_channel_13", "fhDelay_channel_13;Delay of the PMT front from the 'base' front - channels 13;Entries", 5*(maxWin-minWin), minWin, maxWin);
	   fHM->Create1<TH1D>("fhDelay_channel_15", "fhDelay_channel_15;Delay of the PMT front from the 'base' front - channels 15;Entries", 5*(maxWin-minWin), minWin, maxWin);
	   fHM->Create1<TH1D>("fhDelay_channel_17", "fhDelay_channel_17;Delay of the PMT front from the 'base' front - channels 17;Entries", 5*(maxWin-minWin), minWin, maxWin);
	   fHM->Create1<TH1D>("fhDelay_channel_19", "fhDelay_channel_19;Delay of the PMT front from the 'base' front - channels 19;Entries", 5*(maxWin-minWin), minWin, maxWin);
	   fHM->Create1<TH1D>("fhDelay_channel_21", "fhDelay_channel_21;Delay of the PMT front from the 'base' front - channels 21;Entries", 5*(maxWin-minWin), minWin, maxWin);
	   fHM->Create1<TH1D>("fhDelay_channel_23", "fhDelay_channel_23;Delay of the PMT front from the 'base' front - channels 23;Entries", 5*(maxWin-minWin), minWin, maxWin);
	   fHM->Create1<TH1D>("fhDelay_channel_25", "fhDelay_channel_25;Delay of the PMT front from the 'base' front - channels 25;Entries", 5*(maxWin-minWin), minWin, maxWin);
	   fHM->Create1<TH1D>("fhDelay_channel_27", "fhDelay_channel_27;Delay of the PMT front from the 'base' front - channels 27;Entries", 5*(maxWin-minWin), minWin, maxWin);
	   fHM->Create1<TH1D>("fhDelay_channel_29", "fhDelay_channel_29;Delay of the PMT front from the 'base' front - channels 29;Entries", 5*(maxWin-minWin), minWin, maxWin);
	   fHM->Create1<TH1D>("fhDelay_channel_31", "fhDelay_channel_31;Delay of the PMT front from the 'base' front - channels 31;Entries", 5*(maxWin-minWin), minWin, maxWin);

	   fHM->Create1<TH1D>("fhHitRelativeTime", "fhHitRelativeTime;Timestamp of the hit relative to the base timestamp;Entries", 200, -1500000, 1500000);

   }
}

void CbmRichTrbUnpackQa::DrawHist()
{
   if (fDrawHist) {
	   //TCanvas* c = new TCanvas(fRunTitle + "", fRunTitle + "_", 600, 600);
	   //DrawH1(fHM->H1("fhNofDigisInEvent"));
	   //FitGaussAndDrawResults(fHM->H1("fhNofHitsInEvent"));
	   //Cbm::SaveCanvasAsImage(c, string(fOutputDir.Data()), "png");

	   TCanvas* cTDC = new TCanvas(fRunTitle + "_TDCmessages", fRunTitle + "_TDCmessages", 800, 800);
	   DrawH1(fHM->H1("fhTDCmessages"));
   //	Cbm::SaveCanvasAsImage(cTDC, string(fOutputDir.Data()), "png");

	   TCanvas* cHitRelativeTime = new TCanvas(fRunTitle + "_HitRelativeTime", fRunTitle + "_HitRelativeTime", 800, 800);
	   DrawH1(fHM->H1("fhHitRelativeTime"));

   // TOT histograms

	   TCanvas* cTOT1 = new TCanvas(fRunTitle + "_TOT_1-8", fRunTitle + "_TOT_1-8", 1400, 1400);
	   cTOT1->Divide(2, 2);
	   cTOT1->cd(1);	DrawH1(fHM->H1("fhTOT_channel_1-2"));
	   cTOT1->cd(2);	DrawH1(fHM->H1("fhTOT_channel_3-4"));
	   cTOT1->cd(3);	DrawH1(fHM->H1("fhTOT_channel_5-6"));
	   cTOT1->cd(4);	DrawH1(fHM->H1("fhTOT_channel_7-8"));
   //	Cbm::SaveCanvasAsImage(cTOT1, string(fOutputDir.Data()), "png");

	   TCanvas* cTOT2 = new TCanvas(fRunTitle + "_TOT_9-16", fRunTitle + "_TOT_9-16", 1400, 1400);
	   cTOT2->Divide(2, 2);
	   cTOT2->cd(1);	DrawH1(fHM->H1("fhTOT_channel_9-10"));
	   cTOT2->cd(2);	DrawH1(fHM->H1("fhTOT_channel_11-12"));
	   cTOT2->cd(3);	DrawH1(fHM->H1("fhTOT_channel_13-14"));
	   cTOT2->cd(4);	DrawH1(fHM->H1("fhTOT_channel_15-16"));
   //	Cbm::SaveCanvasAsImage(cTOT2, string(fOutputDir.Data()), "png");

	   TCanvas* cTOT3 = new TCanvas(fRunTitle + "_TOT_17-24", fRunTitle + "_TOT_17-24", 1400, 1400);
	   cTOT3->Divide(2, 2);
	   cTOT3->cd(1);	DrawH1(fHM->H1("fhTOT_channel_17-18"));
	   cTOT3->cd(2);	DrawH1(fHM->H1("fhTOT_channel_19-20"));
	   cTOT3->cd(3);	DrawH1(fHM->H1("fhTOT_channel_21-22"));
	   cTOT3->cd(4);	DrawH1(fHM->H1("fhTOT_channel_23-24"));
   //	Cbm::SaveCanvasAsImage(cTOT3, string(fOutputDir.Data()), "png");

	   TCanvas* cTOT4 = new TCanvas(fRunTitle + "_TOT_25-32", fRunTitle + "_TOT_25-32", 1400, 1400);
	   cTOT4->Divide(2, 2);
	   cTOT4->cd(1);	DrawH1(fHM->H1("fhTOT_channel_25-26"));
	   cTOT4->cd(2);	DrawH1(fHM->H1("fhTOT_channel_27-28"));
	   cTOT4->cd(3);	DrawH1(fHM->H1("fhTOT_channel_29-30"));
	   cTOT4->cd(4);	DrawH1(fHM->H1("fhTOT_channel_31-32"));
   //	Cbm::SaveCanvasAsImage(cTOT4, string(fOutputDir.Data()), "png");


   // Delay of the PMT front from the LED sync front histograms

	   TCanvas* cDelay1 = new TCanvas(fRunTitle + "_LEDdelay_1,3,5,7", fRunTitle + "_LEDdelay_1,3,5,7", 1400, 1400);
	   cDelay1->Divide(2, 2);
	   cDelay1->cd(1);	DrawH1(fHM->H1("fhDelay_channel_1"));
	   cDelay1->cd(2);	DrawH1(fHM->H1("fhDelay_channel_3"));
	   cDelay1->cd(3);	DrawH1(fHM->H1("fhDelay_channel_5"));
	   cDelay1->cd(4);	DrawH1(fHM->H1("fhDelay_channel_7"));
   //	Cbm::SaveCanvasAsImage(cDelay1, string(fOutputDir.Data()), "png");

	   TCanvas* cDelay2 = new TCanvas(fRunTitle + "_LEDdelay_9,11,13,15", fRunTitle + "_LEDdelay_9,11,13,15", 1400, 1400);
	   cDelay2->Divide(2, 2);
	   cDelay2->cd(1);	DrawH1(fHM->H1("fhDelay_channel_9"));
	   cDelay2->cd(2);	DrawH1(fHM->H1("fhDelay_channel_11"));
	   cDelay2->cd(3);	DrawH1(fHM->H1("fhDelay_channel_13"));
	   cDelay2->cd(4);	DrawH1(fHM->H1("fhDelay_channel_15"));
   //	Cbm::SaveCanvasAsImage(cDelay2, string(fOutputDir.Data()), "png");

	   TCanvas* cDelay3 = new TCanvas(fRunTitle + "_LEDdelay_17,19,21,23", fRunTitle + "_LEDdelay_17,19,21,23", 1400, 1400);
	   cDelay3->Divide(2, 2);
	   cDelay3->cd(1);	DrawH1(fHM->H1("fhDelay_channel_17"));
	   cDelay3->cd(2);	DrawH1(fHM->H1("fhDelay_channel_19"));
	   cDelay3->cd(3);	DrawH1(fHM->H1("fhDelay_channel_21"));
	   cDelay3->cd(4);	DrawH1(fHM->H1("fhDelay_channel_23"));
   //	Cbm::SaveCanvasAsImage(cDelay3, string(fOutputDir.Data()), "png");

	   TCanvas* cDelay4 = new TCanvas(fRunTitle + "_LEDdelay_25,27,29,31", fRunTitle + "_LEDdelay_25,27,29,31", 1400, 1400);
	   cDelay4->Divide(2, 2);
	   cDelay4->cd(1);	DrawH1(fHM->H1("fhDelay_channel_25"));
	   cDelay4->cd(2);	DrawH1(fHM->H1("fhDelay_channel_27"));
	   cDelay4->cd(3);	DrawH1(fHM->H1("fhDelay_channel_29"));
	   cDelay4->cd(4);	DrawH1(fHM->H1("fhDelay_channel_31"));
   //	Cbm::SaveCanvasAsImage(cDelay4, string(fOutputDir.Data()), "png");

   }
}

void CbmRichTrbUnpackQa::Exec(Option_t* /*option*/)
{
   CbmRichTrbParam* param = CbmRichTrbParam::Instance();

   Int_t nofDigisInEvent = fRichTrbDigi->GetEntries();
   if (fDrawHist) {
	   fHM->H1("fhNofDigisInEvent")->Fill(nofDigisInEvent);
   }

   fRichHits->Clear();
   fRichHitInfos->Clear();

   TString curHistoName;

#ifdef DEBUGPRINT
	printf ("EVENT %d -------------------------------------------------------------------------------------------------\n", fEventNum);
#endif

	lastBaseTimestamp = 0.;

	for (Int_t i = 0; i < nofDigisInEvent; i++) {
		CbmRichTrbDigi* theDigit = static_cast<CbmRichTrbDigi*>(fRichTrbDigi->At(i));

		if (theDigit->GetTDCid() == 0x0110 && theDigit->GetLeadingEdgeChannel() == 11) {
			lastBaseTimestamp = theDigit->GetLeadingEdgeTimeStamp();
		}

		if (theDigit->GetHasLeadingEdge() && theDigit->GetHasTrailingEdge()) {
			numOfPairs++;

#ifdef DEBUGPRINT
			printf ("pair:");
			printf ("      LEAD  - tdc 0x%04x  ch %d = %f\t(%f)\n",
					theDigit->GetTDCid(), theDigit->GetLeadingEdgeChannel(), theDigit->GetLeadingEdgeTimeStamp(), theDigit->GetLeadingEdgeTimeStamp());
			printf ("     ");
			printf ("      TRAIL - tdc 0x%04x  ch %d = %f\t(%f)\n",
					theDigit->GetTDCid(), theDigit->GetTrailingEdgeChannel(), theDigit->GetTrailingEdgeTimeStamp(), theDigit->GetTrailingEdgeTimeStamp());
#endif

			UInt_t curLch = theDigit->GetLeadingEdgeChannel();

			if (fDrawHist) {
            	fHM->H1("fhTDCmessages")->Fill(param->TDCidToInteger(theDigit->GetTDCid()));

			   if (theDigit->GetTDCid() == 0x0030) {
				   curHistoName.Form("fhTOT_channel_%d-%d", curLch, curLch+1);
				   fHM->H1(curHistoName.Data())->Fill(theDigit->GetTOT());
			   }

			   if (param->IsPmtTdc(theDigit->GetTDCid())) {
			   //if (theDigit->GetTDCid() == 0x0030) {
				   curHistoName.Form("fhDelay_channel_%d", curLch);
				   fHM->H1(curHistoName.Data())->Fill(theDigit->GetLeadingEdgeTimeStamp() - lastBaseTimestamp);
			   }
            }

		   if (param->IsPmtTdc(theDigit->GetTDCid())) {
			   Double_t hitRelativeTime = theDigit->GetLeadingEdgeTimeStamp() - lastBaseTimestamp;

			   if (fDrawHist) {
				   if (lastBaseTimestamp != 0.)
					   fHM->H1("fhHitRelativeTime")->Fill(hitRelativeTime);
			   }
/*
			   if (hitRelativeTime >= -105. && hitRelativeTime <= -65.) {

				   CbmRichHitInfo* hitData = param->GetRichHitInfo(theDigit->GetTDCid(), curLch);
				   new( (*fRichHits)[fRichHits->GetEntriesFast()] ) CbmRichHit(hitData->GetX(), hitData->GetY());

				   LOG(info) << "Create a hit";

				   UInt_t counter2 = fRichHitInfos->GetEntries();
				   new((*fRichHitInfos)[counter2]) CbmRichHitInfo();
				   CbmRichHitInfo* newHitInfo = static_cast<CbmRichHitInfo*>(fRichHitInfos->At(counter2));
				   newHitInfo->Copy(hitData);

			   }
*/
		   }

		}
		else {
			if (theDigit->GetHasLeadingEdge() && !(theDigit->GetHasTrailingEdge()) && param->IsSyncChannel(theDigit->GetLeadingEdgeChannel())) {
				numOfSingleEdgesSync++;

#ifdef DEBUGPRINT
				printf ("sync:");
				printf ("      SYNC  - tdc 0x%04x  ch %d = %f\t(%f)\n",
						theDigit->GetTDCid(), theDigit->GetLeadingEdgeChannel(), theDigit->GetLeadingEdgeTimeStamp(), theDigit->GetLeadingEdgeTimeStamp());
#endif
			} else {
#ifdef DEBUGPRINT
			   if (theDigit->GetHasLeadingEdge()) {
			      printf ("sing:      LEAD  - tdc 0x%04x  ch %d = %f\t(%f)\n",
			    		  theDigit->GetTDCid(), theDigit->GetLeadingEdgeChannel(), theDigit->GetLeadingEdgeTimeStamp(), theDigit->GetLeadingEdgeTimeStamp());
			   } else {
			      printf ("sing:      TRAIL - tdc 0x%04x  ch %d = %f\t(%f)\n",
			    		  theDigit->GetTDCid(), theDigit->GetTrailingEdgeChannel(), theDigit->GetTrailingEdgeTimeStamp(), theDigit->GetTrailingEdgeTimeStamp());
			   }
#endif
			   numOfSingleEdges++;
			}
		}
	}

	if (fEventNum % 1000 == 0)
		LOG(info) << "CbmRichTrbUnpackQa : Event #" << fEventNum;
	fEventNum++;

}

void CbmRichTrbUnpackQa::Finish()
{
	UInt_t totalMessages = numOfPairs*2 + numOfSingleEdges + numOfSingleEdgesSync;

	LOG(info) << "[CbmRichTrbUnpackQa] " << numOfPairs << " pairs; " << numOfSingleEdges << " single edges (not incl.) "
		<< numOfSingleEdgesSync  << " sync messages; Total " << totalMessages << " messages";

	LOG(info) << 100.*(Double_t)numOfPairs*2. / (Double_t)totalMessages << "%, "
		  << 100.*(Double_t)numOfSingleEdges / (Double_t)totalMessages << "%, "
		  << 100.*(Double_t)numOfSingleEdgesSync / (Double_t)totalMessages << "%";

   if (fDrawHist) {
		DrawHist();
	   TFile* curHistoFile = new TFile(fOutHistoFile, "CREATE");
	   LOG(info) << "Opening file " << fOutHistoFile << " for histograms";
	   fHM->WriteToFile();
	   curHistoFile->Close();
   }
}

void CbmRichTrbUnpackQa::FitGaussAndDrawResults(TH1* h)
{
	h->Fit("gaus", "Q");
	//h->Fit("gaus");
	TF1* fit = h->GetFunction("gaus");

	Double_t sigma = (NULL != fit) ? fit->GetParameter(2) : 0.;
	Double_t mean = (NULL != fit) ? fit->GetParameter(1) : 0.;
	TString str;
	str.Form("m=%.2f  #sigma=%.2f", mean, sigma);

	h->SetTitle(str.Data());
	fit->SetLineColor(kBlack);
}

ClassImp(CbmRichTrbUnpackQa)

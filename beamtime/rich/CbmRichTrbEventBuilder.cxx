#include "CbmRichTrbEventBuilder.h"
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
#include <TFile.h>

#include "CbmUtils.h"

// Uncomment if you want to have excessive printout (do not execute on many events, may produce Gb's of output)
//#define DEBUGPRINT

#define TESTEDTDC 0x0010

CbmRichTrbEventBuilder::CbmRichTrbEventBuilder()
  : FairTask(),
    fRichTrbDigi(NULL),
    fRichHits(NULL),
    fRichHitInfos(NULL),
    numOfPairs(0),
    numOfSingleEdges(0),
    numOfSingleEdgesSync(0),
    fHM(NULL),
    fDrawHist(kFALSE),
    fEventNum(0),
    fRunTitle(""),
    fOutputDir(""),
    fOutHistoFile(""),
    fWlsState(""),
    lastTriggerTimestamp(0.),
    lastRefTime(0.)
{
}

CbmRichTrbEventBuilder::~CbmRichTrbEventBuilder()
{
}

InitStatus CbmRichTrbEventBuilder::Init()
{
	FairRootManager* manager = FairRootManager::Instance();
	fRichTrbDigi = (TClonesArray*)manager->GetObject("CbmRichTrbDigi");
	if (NULL == fRichTrbDigi) { Fatal("CbmRichTrbEventBuilder::Init","No CbmRichTrbDigi array!"); }

	fRichHits = new TClonesArray("CbmRichHit", 10);
	manager->Register("RichHit", "RICH", fRichHits, IsOutputBranchPersistent("RichHit"));

	fRichHitInfos = new TClonesArray("CbmRichHitInfo", 10);
	manager->Register("RichHitInfo","RICH", fRichHitInfos, IsOutputBranchPersistent("RichHitInfo"));

	InitHist();

	return kSUCCESS;
}

void CbmRichTrbEventBuilder::InitHist()
{
	if (fDrawHist)
	{
		fHM = new CbmHistManager();
//		Double_t K, minWin, maxWin; (FU) not used

		TString histoName;
		TString histoTitle;
/*  (FU) not used
		K = 100;	// 1000/W=K, where W - desired bin width in ps.
		minWin = -50.;
		maxWin = 50.;
*/
/*
      histoName.Form("hRefChannel");
      histoTitle.Form("hRefChannel");
      fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 256, 0, 256);

      histoName.Form("hMeasuredChannel");
      histoTitle.Form("hMeasuredChannel");
      fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 256, 0, 256);

      histoName.Form("ExponentaBudetTut_laser");
      histoTitle.Form("ExponentaBudetTut_laser");
      fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), K*(maxWin-minWin), minWin, maxWin);
*/
/*
	   TString tdcIDs[16];
	   tdcIDs[0] = "10";	tdcIDs[1] = "11";	tdcIDs[2] = "12";	tdcIDs[3] = "13";
	   tdcIDs[4] = "20";	tdcIDs[5] = "21";	tdcIDs[6] = "22";	tdcIDs[7] = "23";
	   tdcIDs[8] = "50";	tdcIDs[9] = "51";	tdcIDs[10] = "52";	tdcIDs[11] = "53";
	   tdcIDs[12] = "60";	tdcIDs[13] = "61";	tdcIDs[14] = "62";	tdcIDs[15] = "63";

	   for (Int_t tdc1J=0; tdc1J<=15; tdc1J++) {
		   Int_t tdc2J = tdc1J;
		   for (Int_t ch1=0; ch1<=15; ch1++) {
			   for (Int_t ch2=ch1; ch2<=15; ch2++) {
				   histoName.Form("LeadingEdgeDiff_TDC%s_ch%d_TDC%s_ch%d", tdcIDs[tdc1J].Data(), ch1, tdcIDs[tdc2J].Data(), ch2);
				   histoTitle.Form("LeadingEdgeDiff_TDC%s_ch%d_TDC%s_ch%d", tdcIDs[tdc1J].Data(), ch1, tdcIDs[tdc2J].Data(), ch2);
				   fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), K*(maxWin-minWin), minWin, maxWin);
				   //printf ("%s\n", histoName.Data());
			   }
		   }
		   for (tdc2J=tdc1J+1; tdc2J<=15; tdc2J++) {
			   for (Int_t ch1=0; ch1<=15; ch1++) {
				   for (Int_t ch2=0; ch2<=15; ch2++) {
					   histoName.Form("LeadingEdgeDiff_TDC%s_ch%d_TDC%s_ch%d", tdcIDs[tdc1J].Data(), ch1, tdcIDs[tdc2J].Data(), ch2);
					   histoTitle.Form("LeadingEdgeDiff_TDC%s_ch%d_TDC%s_ch%d", tdcIDs[tdc1J].Data(), ch1, tdcIDs[tdc2J].Data(), ch2);
					   fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), K*(maxWin-minWin), minWin, maxWin);
					   //printf ("%s\n", histoName.Data());
				   }
			   }
		   }
	   }
*/

      histoName.Form("fNumOfDigisInEvent");
      histoTitle.Form("Number of digits in event");
      fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 200, 0., 200.);

      histoName.Form("fNumOfDigisInEvent_pairOnly");
      histoTitle.Form("Number of digits (constructed as pait of hits) in event");
      fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 200, 0., 200.);

      for (unsigned int pixel=0; pixel<64; pixel++)
      {
	      histoName.Form("fTriggCorrelation_pixel%d", pixel);
	      histoTitle.Form("Correlation with the trigger in pixel%d", pixel);
	      fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 1200, -200., 1000.);

	      histoName.Form("fTriggCorrelation_inWin_pixel%d", pixel);
	      histoTitle.Form("Correlation with the trigger inside trigger window in pixel%d", pixel);
	      fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 1200, -200., 1000.);

	      histoName.Form("fTriggCorrelation_wide_pixel%d", pixel);
	      histoTitle.Form("Correlation with the trigger in bigger window in pixel%d", pixel);
	      fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 848, -2000., 210000.);

	      histoName.Form("fTriggCorrelation_inNoiseWin_pixel%d", pixel);
	      histoTitle.Form("Correlation with the trigger inside noise trigger window in pixel%d", pixel);
	      fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 848, -2000., 210000.);
	  }

      histoName.Form("fTriggCorrelation_allPixels");
      histoTitle.Form("Correlation with the trigger in all pixels");
      fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 1200, -200., 1000.);

      histoName.Form("fTriggCorrelation_inWin_allPixels");
      histoTitle.Form("Correlation with the trigger inside trigger window in all pixels");
      fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 1200, -200., 1000.);

      histoName.Form("fTriggCorrelation_wide_allPixels");
      histoTitle.Form("Correlation with the trigger in bigger window in all pixels");
      fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 848, -2000., 210000.);

      histoName.Form("fTriggCorrelation_inNoiseWin_allPixels");
      histoTitle.Form("Correlation with the trigger inside noise trigger window in all pixels");
      fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 848, -2000., 210000.);

      histoName.Form("fNumOfLightHitsInEvent");
      histoTitle.Form("Number of light hits in event");
      fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 20, 0., 20.);

      // Bin number 257 is for the number of events needed for normalisation
      histoName.Form("fTotalNumOfLightHitsInPixel");
      histoTitle.Form("Total number of light hits in pixel");
      fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 257, 0., 257.);

	}
}

void CbmRichTrbEventBuilder::DrawHist()
{
}

void CbmRichTrbEventBuilder::Exec(Option_t* /*option*/)
{
	// Get the parameters object
	CbmRichTrbParam* param = CbmRichTrbParam::Instance();

	// Save the number of messages coming into this task in this event
	Int_t nofDigisInEvent = fRichTrbDigi->GetEntries();

	// Empty output TClonesArray
	fRichHits->Clear();
	fRichHitInfos->Clear();

	// Just a string
	TString curHistoName;

#ifdef DEBUGPRINT
	printf ("CbmRichTrbEventBuilder:EVENT %d - %d digis\n", fEventNum, nofDigisInEvent);
#endif

	// Number of pairs local for the current event
	Int_t nOfPairsInEvent = 0;

	UInt_t hitTDCid1;
	UInt_t hitChannel1;
	Double_t hitTimestamp1;

	// Detect trigger timestamp here
	//lastTriggerTimestamp = 0.;
	// Loop over all the incoming digi and find the trigger message
	for (Int_t i = 0; i < nofDigisInEvent; i++)
	{
		CbmRichTrbDigi* theDigit = static_cast<CbmRichTrbDigi*>(fRichTrbDigi->At(i));
		if (theDigit->GetTDCid() == TESTEDTDC && theDigit->GetLeadingEdgeChannel() == 0) {
			lastTriggerTimestamp = theDigit->GetLeadingEdgeTimeStamp();
		}
	}

	// Just to count
	for (Int_t i=0; i<nofDigisInEvent; i++) {
		CbmRichTrbDigi* theDigit = static_cast<CbmRichTrbDigi*>(fRichTrbDigi->At(i));
		if (theDigit->GetHasLeadingEdge() && theDigit->GetHasTrailingEdge()) {
			numOfPairs++;			// Global - per run
			nOfPairsInEvent++;		// Local - per event
		} else {
			if (theDigit->GetHasLeadingEdge() && !(theDigit->GetHasTrailingEdge()) && param->IsSyncChannel(theDigit->GetLeadingEdgeChannel())) {
				numOfSingleEdgesSync++;
			} else {
				numOfSingleEdges++;
			}
		}
	}

   fHM->H1("fNumOfDigisInEvent")->Fill(nofDigisInEvent);
   fHM->H1("fNumOfDigisInEvent_pairOnly")->Fill(nOfPairsInEvent);

   UInt_t counterOfLightHits=0;
   for (Int_t i=0; i<nofDigisInEvent; i++) {
      CbmRichTrbDigi* theDigit = static_cast<CbmRichTrbDigi*>(fRichTrbDigi->At(i));
      hitTDCid1 = theDigit->GetTDCid();
      hitChannel1 = theDigit->GetLeadingEdgeChannel();
      hitTimestamp1 = theDigit->GetLeadingEdgeTimeStamp();

      // First check that the hit is coming from one of the studied TDCs
      if (!param->isStudiedTDC(hitTDCid1)) {
         continue;
      }

/*
      // Check that the hit is made of two edges (not a single edge) - thus skip sync messages
      if (!(theDigit->GetHasLeadingEdge() && theDigit->GetHasTrailingEdge())) {
         continue;
      }
*/

      if (param->IsSyncChannel(hitChannel1)) {
         continue;
      }
      if (!(theDigit->GetHasLeadingEdge())) {
         continue;
      }

      UInt_t universalPixelID1 = (param->TDCidToInteger(hitTDCid1)*32 + hitChannel1)/2; //FIXME
      fHM->H1("fTotalNumOfLightHitsInPixel")->Fill(universalPixelID1);

      //printf ("TDC %d ch %d id %d\n", hitTDCid1, hitChannel1, universalPixelID1);

      // Correlation with the trigger (SYNC message in TESTEDTDC)
      TString hNam;

      hNam.Form("fTriggCorrelation_pixel%d", universalPixelID1);
      fHM->H1(hNam.Data())->Fill(hitTimestamp1 - lastTriggerTimestamp);

      hNam.Form("fTriggCorrelation_wide_pixel%d", universalPixelID1);
      fHM->H1(hNam.Data())->Fill(hitTimestamp1 - lastTriggerTimestamp);

      hNam.Form("fTriggCorrelation_allPixels");
      fHM->H1(hNam.Data())->Fill(hitTimestamp1 - lastTriggerTimestamp);

      hNam.Form("fTriggCorrelation_wide_allPixels");
      fHM->H1(hNam.Data())->Fill(hitTimestamp1 - lastTriggerTimestamp);

      // Here we cut hits using some window around trigger signal
      Double_t distToTrig = hitTimestamp1 - lastTriggerTimestamp;

      if (distToTrig > -80. && distToTrig < 0.) {
      	hNam.Form("fTriggCorrelation_inWin_pixel%d", universalPixelID1);
      	fHM->H1(hNam.Data())->Fill(hitTimestamp1 - lastTriggerTimestamp);
      	hNam.Form("fTriggCorrelation_inWin_allPixels");
      	fHM->H1(hNam.Data())->Fill(hitTimestamp1 - lastTriggerTimestamp);
         // Count the number of light hits
         counterOfLightHits++;
      }

      if (distToTrig > 50000. && distToTrig < 150000.) {
      	hNam.Form("fTriggCorrelation_inNoiseWin_pixel%d", universalPixelID1);
      	fHM->H1(hNam.Data())->Fill(hitTimestamp1 - lastTriggerTimestamp);
      	hNam.Form("fTriggCorrelation_inNoiseWin_allPixels");
      	fHM->H1(hNam.Data())->Fill(hitTimestamp1 - lastTriggerTimestamp);
      }

   }

   fHM->H1("fNumOfLightHitsInEvent")->Fill(counterOfLightHits);
   fHM->H1("fTotalNumOfLightHitsInPixel")->Fill(256.5);

/*
	// Find the trigger
	Bool_t hodoFound = kFALSE;
	Bool_t laserFound = kFALSE;
	Bool_t ledFound = kFALSE;
	for (Int_t i=0; i<nofDigisInEvent; i++) {
		CbmRichTrbDigi* theDigit = static_cast<CbmRichTrbDigi*>(fRichTrbDigi->At(i));

		if (theDigit->GetTDCid() == 0x0110 && theDigit->GetLeadingEdgeChannel() == 5) {
			hodoFound = kTRUE;
			//LOG(info) << "hodoFound = kTRUE";
		}
		if (theDigit->GetTDCid() == 0x0110 && theDigit->GetLeadingEdgeChannel() == 15) {
			laserFound = kTRUE;
			//LOG(info) << "laserFound = kTRUE";
		}
		if (theDigit->GetTDCid() == 0x0110 && theDigit->GetLeadingEdgeChannel() == 7) {
			ledFound = kTRUE;
			//LOG(info) << "ledFound = kTRUE";
		}
	}

	//TODO choose which trigger to use
	if (laserFound && !hodoFound && !ledFound) {
		// go on
	} else {
		return;
	}

	// Find first hit in the laser flash - reference
	Int_t hitTDCidRef;
	Int_t hitChannelRef;
	Double_t hitTimestampRef;
	Int_t firstHitIndex = -1;
	Int_t minTDCidRef;
	Int_t minChannelRef;
	Double_t minTimestampRef = 0.;
	Bool_t refFound = kFALSE;

   for (Int_t i=0; i<nofDigisInEvent; i++)
   {
		CbmRichTrbDigi* theDigitRef = static_cast<CbmRichTrbDigi*>(fRichTrbDigi->At(i));
      hitTDCidRef = theDigitRef->GetTDCid();
      hitChannelRef = theDigitRef->GetLeadingEdgeChannel();
      hitTimestampRef = theDigitRef->GetLeadingEdgeTimeStamp();

      // First check that the hit is coming from one of the studied TDCs
      if (!param->isStudiedTDC(hitTDCidRef)) {
         continue;
      }
		// Check that the hit is made of two edges (not a single edge)
		if (!(theDigitRef->GetHasLeadingEdge() && theDigitRef->GetHasTrailingEdge())) {
			continue;
		}
      // Then check that the pixel is not bad
      if (!param->isNormalPixel(hitTDCidRef, hitChannelRef)) {
         continue;
      }
      // Additional cut
      if (hitTDCidRef==0x0013 && (hitChannelRef==11 || hitChannelRef==15))
         continue;

      if (firstHitIndex == -1) {
         firstHitIndex = i;
         minTDCidRef = hitTDCidRef;
         minChannelRef = hitChannelRef;
         minTimestampRef = hitTimestampRef;
         refFound = kTRUE;
      } else {
         if (hitTimestampRef < minTimestampRef) {
            firstHitIndex = i;
            minTDCidRef = hitTDCidRef;
            minChannelRef = hitChannelRef;
            minTimestampRef = hitTimestampRef;
            refFound = kTRUE;
         }
      }
   }

   // Fill only if the reference hit is found
   if (refFound) {
      for (Int_t i=0; i<nofDigisInEvent; i++) {
         CbmRichTrbDigi* theDigit1 = static_cast<CbmRichTrbDigi*>(fRichTrbDigi->At(i));
         hitTDCid1 = theDigit1->GetTDCid();
         hitChannel1 = theDigit1->GetLeadingEdgeChannel();
         hitTimestamp1 = theDigit1->GetLeadingEdgeTimeStamp();

         // First check that the hit is coming from one of the studied TDCs
         if (!param->isStudiedTDC(hitTDCid1)) {
            continue;
         }
         // Check that the hit is made of two edges (not a single edge)
         if (!(theDigit1->GetHasLeadingEdge() && theDigit1->GetHasTrailingEdge())) {
            continue;
         }
         // Then check that the pixel is not bad
         if (!param->isNormalPixel(hitTDCid1, hitChannel1)) {
            continue;
         }
         //FIXME Additional cut - tune according to needs
         if (hitTDCid1==0x0013 && (hitChannel1==11 || hitChannel1==15)) {
            continue;
         }

         // Skip hit which is the same as reference
         // Skip double hit - different hits coming from the same channel in the same event
         if ((i == firstHitIndex) || (minTDCidRef==hitTDCid1 && minChannelRef==hitChannel1)) {
            continue;
         }

         fHM->H1("ExponentaBudetTut_laser")->Fill(hitTimestamp1 - minTimestampRef);

         if (hitTimestamp1 - minTimestampRef > 10.) {
            fHM->H1("hRefChannel")->Fill(param->TDCidToInteger(minTDCidRef)*32+minChannelRef);
            fHM->H1("hMeasuredChannel")->Fill(param->TDCidToInteger(hitTDCid1)*32+hitChannel1);
            //printf ("ref: %x %d\t\tmeasured: %x %d\n", minTDCidRef, minChannelRef, hitTDCid1, hitChannel1);
         }

      }
   }

	TString histoName;
	UInt_t hitTDCid2;
	UInt_t hitChannel2;

	// Loop over the list of hits to extract the first hit for the pair
	// -1 as we don't need to consider the last hit - there is no pair for the last hit anyway
	for (Int_t i=0; i<nofDigisInEvent-1; i++) {
		CbmRichTrbDigi* theDigit1 = static_cast<CbmRichTrbDigi*>(fRichTrbDigi->At(i));

		// Check that the hit is made of two edges (not a single edge)
		if (!(theDigit1->GetHasLeadingEdge() && theDigit1->GetHasTrailingEdge())) {
			continue;
		}

		// Inner loop over the list of hits (not full!) to extract the second hit for the pair
		for (Int_t j=i+1; j<nofDigisInEvent; j++) {
			CbmRichTrbDigi* theDigit2 = static_cast<CbmRichTrbDigi*>(fRichTrbDigi->At(j));

			// Check that the hit is made of two edges (not a single edge)
			if (!(theDigit2->GetHasLeadingEdge() && theDigit2->GetHasTrailingEdge())) {
				continue;
			}

			hitTDCid1 = theDigit1->GetTDCid();
			hitTDCid2 = theDigit2->GetTDCid();
			hitChannel1 = theDigit1->GetLeadingEdgeChannel();
			hitChannel2 = theDigit2->GetLeadingEdgeChannel();

			//printf("considering pair TDC%04x ch%d TDC%04x ch%d\n", hitTDCid1, hitChannel1, hitTDCid2, hitChannel2);
			if (param->isStudiedTDC(hitTDCid1) == kTRUE && param->isStudiedTDC(hitTDCid2) == kTRUE)
			{
				TString tdcID1;	tdcID1.Form("%x", hitTDCid1);
				TString tdcID2;	tdcID2.Form("%x", hitTDCid2);

				UInt_t universalPixelID1 = param->TDCidToInteger(hitTDCid1)*32 + hitChannel1;
				UInt_t universalPixelID2 = param->TDCidToInteger(hitTDCid2)*32 + hitChannel2;
				if (universalPixelID1 < universalPixelID2) {
					histoName.Form("LeadingEdgeDiff_TDC%s_ch%d_TDC%s_ch%d", tdcID1.Data(), hitChannel1/2, tdcID2.Data(), hitChannel2/2);
					//printf ("%s\n", histoName.Data());
					fHM->H1(histoName.Data())->Fill(theDigit1->GetLeadingEdgeTimeStamp() - theDigit2->GetLeadingEdgeTimeStamp());
				} else {
					histoName.Form("LeadingEdgeDiff_TDC%s_ch%d_TDC%s_ch%d", tdcID2.Data(), hitChannel2/2, tdcID1.Data(), hitChannel1/2);
					//printf ("%s\n", histoName.Data());
					fHM->H1(histoName.Data())->Fill(theDigit2->GetLeadingEdgeTimeStamp() - theDigit1->GetLeadingEdgeTimeStamp());
				}
			}
		}
	}
*/

	fEventNum++;
}

void CbmRichTrbEventBuilder::Finish()
{

  //	Double_t k = (Double_t)numOfSingleEdgesSync;

	UInt_t totalMessages = numOfPairs*2 + numOfSingleEdges + numOfSingleEdgesSync;

	LOG(info) << "[CbmRichTrbEventBuilder] " << numOfPairs << " pairs; " << numOfSingleEdges << " single edges (not incl.) "
		<< numOfSingleEdgesSync  << " sync messages; Total " << totalMessages << " messages";

	LOG(info) << 100.*(Double_t)numOfPairs*2. / (Double_t)totalMessages << "%, "
		  << 100.*(Double_t)numOfSingleEdges / (Double_t)totalMessages << "%, "
		  << 100.*(Double_t)numOfSingleEdgesSync / (Double_t)totalMessages << "%";

	if (fDrawHist) {
		DrawHist();
		TFile* curHistoFile = new TFile(fOutHistoFile, "UPDATE");
		LOG(info) << "Opening file " << fOutHistoFile << " for histograms";
		fHM->WriteToFile();
		curHistoFile->Close();
	}
}

ClassImp(CbmRichTrbEventBuilder)

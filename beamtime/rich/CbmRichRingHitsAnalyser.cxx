/*
 * CbmRichRingHitsAnalyser.cxx
 *
 *  Created on: Apr 15, 2015
 *      Author: evovch
 */

#include "CbmRichRingHitsAnalyser.h"

#include "FairLogger.h"

#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include <TFile.h>

#include "CbmRichRing.h"
#include "CbmRichHit.h"
#include "CbmRichHitInfo.h"
#include "CbmRichTrbParam.h"

#include <vector>
#include <fstream>
#include <math.h>

CbmRichRingHitsAnalyser::CbmRichRingHitsAnalyser()
	: FairTask(),
	  fRichHits(NULL),
	  fRichRings(NULL),
	  fRichHitInfos(NULL),
	  fWlsState(""),
	  fInputFileBasename(""),
	  fRingCutsFile(""),
	  fOutHistoFile(""),
	  fConditionsMap(),
	  fRingCondition(),
	  fApplyRingCcut(kFALSE),
	  fApplyRingRcut(kFALSE),
	  fHM(NULL)
{
	fRingCondition.cxmin = 0.;
	fRingCondition.cxmax = 21.;
	fRingCondition.cymin = 0.;
	fRingCondition.cymax = 21.;
	fRingCondition.rmin = 0.;
	fRingCondition.rmax = 10.;
}

CbmRichRingHitsAnalyser::~CbmRichRingHitsAnalyser()
{
}

InitStatus CbmRichRingHitsAnalyser::Init()
{
	FairRootManager* manager = FairRootManager::Instance();

	fRichHits = (TClonesArray*)manager->GetObject("RichHit");
	if (NULL == fRichHits) { Fatal("CbmRichRingHitsAnalyser::Init","No RichHit array!"); }

	fRichRings = (TClonesArray*)manager->GetObject("RichRing");
	if (NULL == fRichRings) { Fatal("CbmRichRingHitsAnalyser::Init","No RichRing array!"); }

	fRichHitInfos = (TClonesArray*)manager->GetObject("RichHitInfo");
	if (NULL == fRichHitInfos) { Fatal("CbmRichRingHitsAnalyser::Init","No RichHitInfo array!"); }

	fHM = new CbmHistManager();


	TString histoName;
	TString histoTitle;

	Double_t K, minWin, maxWin;

	K = 100.;	// 1000/W=K, where W - desired bin width in ps.
	minWin = -50.;
	maxWin = 50.;

/*
	Int_t tdcNum;
	for (tdcNum=1; tdcNum<=16; tdcNum++) {
		for (Int_t i=0; i<16; i++) {
			for (Int_t j=i; j<16; j++) {
				histoName.Form("LeadingEdgeDiff_TDC%x0_%d_%d", tdcNum, i, j);
				histoTitle.Form("LeadingEdgeDiff_TDC%x0_%d_%d;ns;Entries", tdcNum, i, j);
				fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), K*(maxWin-minWin), minWin, maxWin);
				histoName.Form("LeadingEdgeDiff_TDC%x1_%d_%d", tdcNum, i, j);
				histoTitle.Form("LeadingEdgeDiff_TDC%x1_%d_%d;ns;Entries", tdcNum, i, j);
				fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), K*(maxWin-minWin), minWin, maxWin);
				histoName.Form("LeadingEdgeDiff_TDC%x2_%d_%d", tdcNum, i, j);
				histoTitle.Form("LeadingEdgeDiff_TDC%x2_%d_%d;ns;Entries", tdcNum, i, j);
				fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), K*(maxWin-minWin), minWin, maxWin);
				histoName.Form("LeadingEdgeDiff_TDC%x3_%d_%d", tdcNum, i, j);
				histoTitle.Form("LeadingEdgeDiff_TDC%x3_%d_%d;ns;Entries", tdcNum, i, j);
				fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), K*(maxWin-minWin), minWin, maxWin);
			}
		}
	}
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

	histoName.Form("hDistBetwDoubleRings");
	histoTitle.Form("hDistBetwDoubleRings");
	fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 200, 0, 100.);

	histoName.Form("hDeltaRdoubleRings");
	histoTitle.Form("hDeltaRdoubleRings");
	fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 200, 0, 100.);


	histoName.Form("hNumOfRingInEvent");
	histoTitle.Form("hNumOfRingInEvent");
	fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 100, 0, 100);

	histoName.Form("hNumOfHitsOnRing");
	histoTitle.Form("hNumOfHitsOnRing");
	fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 100, 0, 100);
/*
	histoName.Form("hNumOfHitsOnRing_PMT1");
	histoTitle.Form("hNumOfHitsOnRing_PMT1");
	fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 100, 0, 100);
	histoName.Form("hNumOfHitsOnRing_PMT2");
	histoTitle.Form("hNumOfHitsOnRing_PMT2");
	fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 100, 0, 100);
	histoName.Form("hNumOfHitsOnRing_PMT5");
	histoTitle.Form("hNumOfHitsOnRing_PMT5");
	fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 100, 0, 100);
	histoName.Form("hNumOfHitsOnRing_PMT6");
	histoTitle.Form("hNumOfHitsOnRing_PMT6");
	fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 100, 0, 100);
*/
	histoName.Form("FirstPixelInRing");
	histoTitle.Form("FirstPixelInRing");
	fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 512, 0, 512);

	histoName.Form("ExponentaBudetTut");
	histoTitle.Form("ExponentaBudetTut");
	fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), K*(maxWin-minWin), minWin, maxWin);

	// Histogram for rings centers
	histoName.Form("RingsCenters1");
	histoTitle.Form("Centers of the found rings 1");
	fHM->Create2<TH2D>(histoName.Data(), histoTitle.Data(), 200, 0., 50., 200, 0., 50.);

	histoName.Form("RingsCenters2");
	histoTitle.Form("Centers of the found rings 2");
	fHM->Create2<TH2D>(histoName.Data(), histoTitle.Data(), 200, 0., 50., 200, 0., 50.);

	histoName.Form("RingsRadii");
	histoTitle.Form("Radii of the found rings");
	fHM->Create1<TH1D>(histoName.Data(), histoTitle.Data(), 200, 0., 10.);

	if (fApplyRingCcut || fApplyRingRcut)
	{
		// Read the full set of conditions
		ifstream ifstr (fRingCutsFile.Data(), std::ios::in);
		string curInfile;
		TString root_CurInfile;
		Double_t v_cxmin, v_cxmax, v_cymin, v_cymax, v_rmin, v_rmax;
		while (!ifstr.eof()) {
			ifstr >> curInfile >> v_cxmin >> v_cxmax >> v_cymin >> v_cymax >> v_rmin >> v_rmax;
			root_CurInfile = curInfile;
			ringCondition v_curCond;
			v_curCond.cxmin = v_cxmin;
			v_curCond.cxmax = v_cxmax;
			v_curCond.cymin = v_cymin;
			v_curCond.cymax = v_cymax;
			v_curCond.rmin = v_rmin;
			v_curCond.rmax = v_rmax;
			fConditionsMap.insert(std::pair<TString, ringCondition>(root_CurInfile, v_curCond));
			//////printf ("Read: %s\t-\t%f\t%f\t%f\t%f\t%f\t%f\n", root_CurInfile.Data(), v_cxmin, v_cxmax, v_cymin, v_cymax, v_rmin, v_rmax);
		 }

		// Find the actual condition and use it
		std::map<TString, ringCondition>::iterator iter;
		iter = fConditionsMap.find(fInputFileBasename);
		if (iter != fConditionsMap.end())
		{
			fRingCondition = iter->second;
			printf ("found condition for the current file.\n");
			printf ("\t%s\t-\t%f\t%f\t%f\t%f\t%f\t%f\n", iter->first.Data(),
				fRingCondition.cxmin, fRingCondition.cxmax, fRingCondition.cymin, fRingCondition.cymax, fRingCondition.rmin, fRingCondition.rmax);
		}
		else
		{
			LOG(info) << "Can not find ring conditions for the current input file.";
		}
	}

	return kSUCCESS;
}

void CbmRichRingHitsAnalyser::Exec(Option_t* /*option*/)
{
	// Get the parameters object 
	CbmRichTrbParam* param = CbmRichTrbParam::Instance();

	Int_t numOfRings = fRichRings->GetEntriesFast();
//	Int_t numOfHits = fRichHits->GetEntriesFast();

	fHM->H1("hNumOfRingInEvent")->Fill(numOfRings);

	std::vector< std::pair< CbmRichHit*, CbmRichHitInfo* > > listOfRingHitInfos;

	if (numOfRings == 1) {
		CbmRichRing* theRing = static_cast<CbmRichRing*>(fRichRings->At(0));
		Int_t numOfHitsOnRing = theRing->GetNofHits();

//		printf("Ring. nHits=%d\tx=%.4f\ty=%.4f\tr=%.4f\n", numOfHitsOnRing, theRing->GetCenterX(), theRing->GetCenterY(), theRing->GetRadius());

		// Cut on the ring center
		if (fApplyRingCcut) {
			if ((theRing->GetCenterX() < fRingCondition.cxmin || theRing->GetCenterX() > fRingCondition.cxmax) ||
				(theRing->GetCenterY() < fRingCondition.cymin || theRing->GetCenterY() > fRingCondition.cymax))
				return;
		}

		// Cut on the ring radius
		if (fApplyRingRcut) {
			if (theRing->GetRadius() < fRingCondition.rmin ||theRing->GetRadius() > fRingCondition.rmax)
				return;
		}

		// Fill the histos
		if (theRing->GetRadius() < 4.85)
			fHM->H2("RingsCenters1")->Fill(theRing->GetCenterX(), theRing->GetCenterY());
		else
			fHM->H2("RingsCenters2")->Fill(theRing->GetCenterX(), theRing->GetCenterY());

		fHM->H1("RingsRadii")->Fill(theRing->GetRadius());
		fHM->H1("hNumOfHitsOnRing")->Fill(numOfHitsOnRing);

		// Start analysis of pairs of hits
		Int_t hitIndex1;
		CbmRichHit* curHit1;
		CbmRichHitInfo* curHitInfo1;
		Int_t hitTDCid1;
		Int_t hitChannel1;

//		Int_t hitIndex2;
//		CbmRichHit* curHit2;
//		CbmRichHitInfo* curHitInfo2;
//		Int_t hitTDCid2;
//		Int_t hitChannel2;

		TString histoName;

		// Get the first in time hit of the ring
		Int_t firstHitIndex = -1;
		Double_t minTimestamp = 0.;
/*
		UInt_t num_PMT1=0;
		UInt_t num_PMT2=0;
		UInt_t num_PMT5=0;
		UInt_t num_PMT6=0;
*/
		for (Int_t i=0; i<numOfHitsOnRing; i++) {
			hitIndex1 = theRing->GetHit(i);
			curHit1 = static_cast<CbmRichHit*>(fRichHits->At(hitIndex1));
			curHitInfo1 = static_cast<CbmRichHitInfo*>(fRichHitInfos->At(hitIndex1));
			hitTDCid1 = curHitInfo1->GetTdcId();
			hitChannel1 = curHitInfo1->GetLeadingChannel();

			if (!param->isNormalPixel(hitTDCid1, hitChannel1))
				continue;

			if (param->isStudiedTDC(hitTDCid1))
			{
/*
				if (hitTDCid1==0x0010 || hitTDCid1==0x0011 || hitTDCid1==0x0012 || hitTDCid1==0x0013) num_PMT1++;
				if (hitTDCid1==0x0020 || hitTDCid1==0x0021 || hitTDCid1==0x0022 || hitTDCid1==0x0023) num_PMT2++;
				if (hitTDCid1==0x0050 || hitTDCid1==0x0051 || hitTDCid1==0x0052 || hitTDCid1==0x0053) num_PMT5++;
				if (hitTDCid1==0x0060 || hitTDCid1==0x0061 || hitTDCid1==0x0062 || hitTDCid1==0x0063) num_PMT6++;
*/
				if (firstHitIndex == -1) {
					firstHitIndex = hitIndex1;
					minTimestamp = curHit1->GetTime();
				} else {
					if (curHit1->GetTime() < minTimestamp) {
						firstHitIndex = hitIndex1;
						minTimestamp = curHit1->GetTime();
					}
				}
			}
		}

		if (firstHitIndex == -1) return;
/*
		fHM->H1("hNumOfHitsOnRing_PMT1")->Fill(num_PMT1);
		fHM->H1("hNumOfHitsOnRing_PMT2")->Fill(num_PMT2);
		fHM->H1("hNumOfHitsOnRing_PMT5")->Fill(num_PMT5);
		fHM->H1("hNumOfHitsOnRing_PMT6")->Fill(num_PMT6);
*/
		curHit1 = static_cast<CbmRichHit*>(fRichHits->At(firstHitIndex));
		curHitInfo1 = static_cast<CbmRichHitInfo*>(fRichHitInfos->At(firstHitIndex));
		hitTDCid1 = curHitInfo1->GetTdcId();
		hitChannel1 = curHitInfo1->GetLeadingChannel();
		fHM->H1("FirstPixelInRing")->Fill(param->TDCidToInteger(hitTDCid1)*16 + hitChannel1/2);

		for (Int_t i=0; i<numOfHitsOnRing; i++) {
			// Get information about the first hit
			hitIndex1 = theRing->GetHit(i);
			curHit1 = static_cast<CbmRichHit*>(fRichHits->At(hitIndex1));
			curHitInfo1 = static_cast<CbmRichHitInfo*>(fRichHitInfos->At(hitIndex1));
			hitTDCid1 = curHitInfo1->GetTdcId();
			hitChannel1 = curHitInfo1->GetLeadingChannel();

			if (hitIndex1 != firstHitIndex && param->isStudiedTDC(hitTDCid1)) {

				if (param->isNormalPixel(hitTDCid1, hitChannel1))
					fHM->H1("ExponentaBudetTut")->Fill(curHit1->GetTime()-minTimestamp);

			}
		}
/*
		for (Int_t i=0; i<numOfHitsOnRing-1; i++) {

			// Get information about the first hit
			hitIndex1 = theRing->GetHit(i);
			curHit1 = static_cast<CbmRichHit*>(fRichHits->At(hitIndex1));
			curHitInfo1 = static_cast<CbmRichHitInfo*>(fRichHitInfos->At(hitIndex1));
			hitTDCid1 = curHitInfo1->GetTdcId();
			hitChannel1 = curHitInfo1->GetLeadingChannel();

			for (Int_t j=i+1; j<numOfHitsOnRing; j++) {

				// Get information about the second hit
				hitIndex2 = theRing->GetHit(j);
				curHit2 = static_cast<CbmRichHit*>(fRichHits->At(hitIndex2));
				curHitInfo2 = static_cast<CbmRichHitInfo*>(fRichHitInfos->At(hitIndex2));
				hitTDCid2 = curHitInfo2->GetTdcId();
				hitChannel2 = curHitInfo2->GetLeadingChannel();

				//printf ("Considering pair of hits: tdc %04x ch %d time %f\t\ttdc %04x ch %d time %f\n",
				//	hitTDCid1, hitChannel1, curHit1->GetTimestamp(), hitTDCid2, hitChannel2, curHit2->GetTimestamp());

				// Build the histograms for a certain TDC
				// if (hitTDCid1 == hitTDCid2)
				// {
					// if (hitChannel1 <= hitChannel2) {
						// histoName.Form("LeadingEdgeDiff_TDC%2x_%d_%d", hitTDCid1, hitChannel1/2, hitChannel2/2);
						// fHM->H1(histoName.Data())->Fill(curHit1->GetTimestamp() - curHit2->GetTimestamp());
					// } else {
						// //if (hitChannel1 == hitChannel2) {
						// //} else {
							// histoName.Form("LeadingEdgeDiff_TDC%2x_%d_%d", hitTDCid1, hitChannel2/2, hitChannel1/2);
							// fHM->H1(histoName.Data())->Fill(curHit2->GetTimestamp() - curHit1->GetTimestamp());
						// //}
					// }
				// }

				if (param->isStudiedTDC(hitTDCid1) == kTRUE && param->isStudiedTDC(hitTDCid2) == kTRUE)
				{
					TString tdcID1;	tdcID1.Form("%x", hitTDCid1);
					TString tdcID2;	tdcID2.Form("%x", hitTDCid2);

					UInt_t universalPixelID1 = param->TDCidToInteger(hitTDCid1)*32 + hitChannel1;
					UInt_t universalPixelID2 = param->TDCidToInteger(hitTDCid2)*32 + hitChannel2;

					if (universalPixelID1 < universalPixelID2) {
						histoName.Form("LeadingEdgeDiff_TDC%s_ch%d_TDC%s_ch%d", tdcID1.Data(), hitChannel1/2, tdcID2.Data(), hitChannel2/2);
						fHM->H1(histoName.Data())->Fill(curHit1->GetTimestamp() - curHit2->GetTimestamp());
					} else {
						histoName.Form("LeadingEdgeDiff_TDC%s_ch%d_TDC%s_ch%d", tdcID2.Data(), hitChannel2/2, tdcID1.Data(), hitChannel1/2);
						fHM->H1(histoName.Data())->Fill(curHit2->GetTimestamp() - curHit1->GetTimestamp());
					}

				}

			}
		}
*/

	} else if (numOfRings == 2) {
		CbmRichRing* theRing1 = static_cast<CbmRichRing*>(fRichRings->At(0));
		CbmRichRing* theRing2 = static_cast<CbmRichRing*>(fRichRings->At(1));

		Double_t c1x = theRing1->GetCenterX();
		Double_t c1y = theRing1->GetCenterY();
		Double_t c2x = theRing2->GetCenterX();
		Double_t c2y = theRing2->GetCenterY();
		Double_t r1 = theRing1->GetRadius();
		Double_t r2 = theRing2->GetRadius();

		Double_t distBetwCenters = sqrt( (c1x-c2x)*(c1x-c2x) + (c1y-c2y)*(c1y-c2y) );

		fHM->H1("hDistBetwDoubleRings")->Fill(distBetwCenters);
		fHM->H1("hDeltaRdoubleRings")->Fill(abs(r1-r2));
	} else if (numOfRings > 2) {
		LOG(info) << "CbmRichRingHitsAnalyser::Exec   More than two rings found. Skipping.";
		//return;
	} else {
		LOG(debug) << "CbmRichRingHitsAnalyser::Exec   No rings found";
		//return;
	}
}

void CbmRichRingHitsAnalyser::Finish()
{
	TFile* curHistoFile = new TFile(fOutHistoFile, "UPDATE");
	LOG(info) << "Opening file " << fOutHistoFile << " for histograms";
	fHM->WriteToFile();
	curHistoFile->Close();
}

ClassImp(CbmRichRingHitsAnalyser)

#include "CbmRichEventDebug.h"

#include "FairLogger.h"

#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include <TFile.h>
#include "CbmRichHit.h"

CbmRichEventDebug::CbmRichEventDebug()
  : FairTask(),
    fRichHits(NULL),
    fRichHitInfos(NULL),
    fSingleEventFilled(kFALSE),
    fOutHistoFile(""),
    fHM(NULL)
{

}

CbmRichEventDebug::~CbmRichEventDebug()
{

}

InitStatus CbmRichEventDebug::Init()
{
	FairRootManager* manager = FairRootManager::Instance();

	fRichHits = (TClonesArray*)manager->GetObject("RichHit");
	if (NULL == fRichHits) { Fatal("CbmRichEventDebug::Init","No RichHit array!"); }

	fRichHitInfos = (TClonesArray*)manager->GetObject("RichHitInfo");
	if (NULL == fRichHitInfos) { Fatal("CbmRichEventDebug::Init","No RichHitInfo array!"); }

	fHM = new CbmHistManager();
	fHM->Create1<TH1D>("fhNumOfHitsInEvent", "Number of hits in the event", 1000, 0, 1000);
	fHM->Create2<TH2D>("fhSingleEvent", "Single event", 32, 0., 32., 32, 0., 32.);

	return kSUCCESS;
}

void CbmRichEventDebug::Exec(Option_t* /*option*/)
{
	UInt_t numOfHits = fRichHits->GetEntriesFast();
	if (numOfHits > 0) {
		fHM->H1("fhNumOfHitsInEvent")->Fill(numOfHits);

		LOG(info) << numOfHits << " hits.";

		//if (!fSingleEventFilled && numOfHits == 12)
		{
			for (UInt_t i=0; i<numOfHits; i++) {
				CbmRichHit* theHit = static_cast<CbmRichHit*>(fRichHits->At(i));
				fHM->H2("fhSingleEvent")->Fill(theHit->GetX(), theHit->GetY());
			}
			fSingleEventFilled = kTRUE;
		}

	}
}

void CbmRichEventDebug::Finish()
{
	new TCanvas("cNumOfHitsInEvent", "cNumOfHitsInEvent", 800, 800);
	DrawH1(fHM->H1("fhNumOfHitsInEvent"));

	if (fSingleEventFilled) {
		new TCanvas("cSingleEvent", "cSingleEvent", 800, 800);
		DrawH2(fHM->H2("fhSingleEvent"));
	}

	TFile* curHistoFile = new TFile(fOutHistoFile, "CREATE");
	LOG(info) << "Opening file " << fOutHistoFile << " for histograms";
	fHM->WriteToFile();
	curHistoFile->Close();
}

ClassImp(CbmRichEventDebug)

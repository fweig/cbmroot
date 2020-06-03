#include "CbmMuchRawHistos.h"

#include <FairRootManager.h>      // for FairRootManager
#include <FairTask.h>             // for InitStatus, kSUCCESS, FairTask, kERROR
#include <TH2.h>                  // for TH2F, TH2
#include <FairLogger.h>           // for Logger, LOG
#include <stddef.h>               // for NULL
#include <set>                    // for set
#include "CbmHistManager.h"       // for CbmHistManager
#include "CbmMuchBeamTimeDigi.h"  // for CbmMuchBeamTimeDigi
#include <TClonesArray.h>         // for TClonesArray
#include <TString.h>              // for TString, operator+
#include "CbmDigiManager.h"

using std::set;

// ---- Default constructor -------------------------------------------
CbmMuchRawHistos::CbmMuchRawHistos()
  : FairTask("CbmMuchRawHistos"),
    fHM(new CbmHistManager()),
    fNofEvents(0)
{
}

// ---- Destructor ----------------------------------------------------
CbmMuchRawHistos::~CbmMuchRawHistos()
{
}

// ----  Initialisation  ----------------------------------------------
void CbmMuchRawHistos::SetParContainers()
{
}

// ---- Init ----------------------------------------------------------
InitStatus CbmMuchRawHistos::Init()
{
	LOG(info) << GetName() << ": Initialising...";

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

  // Get the input digis
  fDigiVec = ioman->InitObjectAs<std::vector<CbmMuchBeamTimeDigi> const *>("MuchDigi");
  if ( ! fDigiVec ) {
    fDigiArr = dynamic_cast<TClonesArray*>(ioman->GetObject("MuchDigi"));
    if ( ! fDigiArr ) {
      LOG(error) << "No branch MuchDigi found!";
      LOG(error) << "Task CbmMuchRawHistos will be inactive";
      return kERROR;
    }
  }

  CreateHistograms();

  return kSUCCESS;

}

// -----   Create histograms   ---------------------------------------------
void CbmMuchRawHistos::CreateHistograms()
{

	TString name;
	TString title;

	name = "hits_much";
	title = name + ";padX;padY";
	fHM->Add(name.Data(), new TH2F(name, title, 23, -0.5, 22.5, 78, -0.5, 77.5));

	name = "MUCH ADC vs channel";
	title = name + ";channel;ADC";
	fHM->Add(name.Data(), new TH2F(name, title, 128, -0.5, 127.5, 4096, -0.5, 4095.5));
}
// -------------------------------------------------------------------------


// ---- ReInit  -------------------------------------------------------
InitStatus CbmMuchRawHistos::ReInit()
{
  return kSUCCESS;
}

// ---- Exec ----------------------------------------------------------
void CbmMuchRawHistos::Exec(Option_t* /*option*/)
{


	const CbmMuchBeamTimeDigi* digi = nullptr;
/*	Int_t adc = -1;*/

	Int_t nDigis = (fDigiVec ? fDigiVec->size() :
	    (fDigiArr ? fDigiArr->GetEntriesFast() : 0));
	for (Int_t iDigi = 0; iDigi < nDigis; iDigi++) {
	  if ( fDigiVec ) digi = &(fDigiVec->at(iDigi));
	  else if ( fDigiArr ) digi = dynamic_cast<CbmMuchBeamTimeDigi*>(fDigiArr->At(iDigi));
		if ( ! digi ) continue;
		fHM->H2("hits_much")->Fill(Double_t(digi->GetPadX()),
				                       Double_t(digi->GetPadY()));
	}

	fNofEvents++;
	if ( ! (fNofEvents%1000) )
		LOG(info) << GetName() << ": event " << fNofEvents << ", digis "
		          << nDigis;
	else
		LOG(info) << GetName() << ": event " << fNofEvents << ", digis "
	             << nDigis;
  
}

// ---- Finish --------------------------------------------------------
void CbmMuchRawHistos::Finish()
{

  fHM->H2("hits_much")->Write();
}

ClassImp(CbmMuchRawHistos)

#ifndef CBMSTSDIGITIZEQA_H_
#define CBMSTSDIGITIZEQA_H_

#include "FairTask.h"
#include "CbmStsDigi.h"
#include "CbmHistManager.h"
#include "CbmMCDataArray.h"
#include <iostream>
#include <fstream>
#include <vector>
class TClonesArray;
class CbmDigiManager;
class CbmStsDigitize;
class CbmStsParAsic;
class CbmStsParSim;
class CbmStsParSetModule;
class CbmStsSetup;

class CbmStsDigitizeQa : public FairTask
{
    public:
	CbmStsDigitizeQa(CbmStsDigitize * digitizer=NULL);

	virtual ~CbmStsDigitizeQa();

	virtual InitStatus Init();

	virtual void Exec(Option_t* opt);

        virtual void SetParContainers();
        
	virtual void Finish();

	void SetOutputDir(const std::string& outputDir) { fOutputDir = outputDir; }

	void CreateHistograms();

	void CreateNofObjectsHistograms();

	void CreateDigiHistograms();

	void ProcessDigisAndPoints(const TClonesArray * points);
	void ProcessAngles();

    private:
	void ReadDataBranches();

	CbmHistManager* fHM;
	CbmDigiManager* fDigiManager;
	std::string fOutputDir;
	TClonesArray* fStsPoints;
	CbmStsSetup * fSetup;
	Int_t fNofStation;
	const CbmStsParSim* fSettings = nullptr;
	CbmStsParSetModule* fModuleParSet = nullptr;
	const CbmStsParAsic* fAsicPar = nullptr;

	Int_t fMaxScale;
	std::ofstream fOutFile;
	std::vector < std::vector <std::vector <std::vector < std::vector <Int_t>>>>> fnOfDigisChip;

	CbmStsDigitizeQa(const CbmStsDigitizeQa&);
	CbmStsDigitizeQa& operator=(const CbmStsDigitizeQa&);

	ClassDef(CbmStsDigitizeQa, 1);
};

#endif

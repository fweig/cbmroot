// -------------------------------------------------------------------------
// -----              CbmMvdClusterAna  header file                    -----
// -----              Created 27/04/15  by S. Amar-Youcef              -----
// -------------------------------------------------------------------------


/**  CbmMvdClusterAna.h
 *@author S.Amar-Youcef <s.amar@gsi.de>
 *
 *  Cluster Analysis
 *
 **/

#ifndef CBMMVDCLUSTERANA_H
#define CBMMVDCLUSTERANA_H

#include "FairTask.h"

class TClonesArray;
class TH1F;
class TH2F;
class TProfile;

class CbmMvdClusterAna : public FairTask
{
public:
	CbmMvdClusterAna();
	CbmMvdClusterAna(const char* name, Int_t iVerbose = 0);
	
	~CbmMvdClusterAna();
	
	InitStatus	Init();
	void			Exec(Option_t* opt); 
	void 			Finish();
	
private:
	TClonesArray* fMcPoints;
	TClonesArray* fMvdDigis;
	TClonesArray* fMvdClusters;
	TClonesArray* fMvdHits;
	
	TClonesArray* fMvdDigisMatch;	
	TClonesArray* fMvdClustersMatch;
	TClonesArray* fMvdHitsMatch;
	
	TClonesArray* fListMCTracks;
	TClonesArray* fStsTrackArray;
	TClonesArray* fStsTrackMatches;
	
	TH1F *fMvdHisto1[50];
	TH2F *fMvdHisto2[50];
	TProfile *fProf[50]; 

	int fNrMcPointsAll;
	int fNrHitsAll;
	
	Int_t	fMcperDigi[6];
	Int_t	fMcperHit[11];
	
	Double_t fPixelpitch[2];

        CbmMvdClusterAna (const CbmMvdClusterAna&);
        CbmMvdClusterAna operator=(const CbmMvdClusterAna&);
  
 ClassDef(CbmMvdClusterAna,1);
};


#endif

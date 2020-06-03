/**
 * \file CbmRichTrackExtrapolationLittrack
 *
 * \author Semen Lebedev
 * \date 2016
 **/
#include "CbmRichTrackExtrapolationLittrack.h"

#include "propagation/CbmLitTGeoTrackPropagator.h"
#include "cbm/utils/CbmLitConverterFairTrackParam.h"
#include "cbm/base/CbmLitToolFactory.h"

#include "FairTrackParam.h"
#include "CbmStsTrack.h"
#include "CbmGlobalTrack.h"
#include "FairTrackParam.h"
#include "FairRootManager.h"

#include "TClonesArray.h"

#include <iostream>

using std::cout;
using std::endl;

CbmRichTrackExtrapolationLittrack::CbmRichTrackExtrapolationLittrack()
 : CbmRichTrackExtrapolationBase(),
   fStsTracks(0),
   fLitPropagator()
{
}

CbmRichTrackExtrapolationLittrack::~CbmRichTrackExtrapolationLittrack()
{
}

void CbmRichTrackExtrapolationLittrack::Init()
{
    FairRootManager* ioman = FairRootManager::Instance();
    if ( NULL == ioman) { Fatal("CbmRichTrackExtrapolationLittrack::Init", "RootManager not instantised!");}
    
    fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
    if ( NULL == fStsTracks) {Fatal("CbmRichTrackExtrapolationLittrack::Init", "No StsTrack array!");}
    
    fLitPropagator = CbmLitToolFactory::CreateTrackPropagator("lit");
}

void CbmRichTrackExtrapolationLittrack::DoExtrapolation(
                                                        TClonesArray* globalTracks,
                                                        TClonesArray* extrapolatedTrackParams,
                                                        double z)
{
    cout <<"CbmRichTrackExtrapolationLittrack::DoExtrapolation" << endl;
    if ( NULL == extrapolatedTrackParams ) {
        cout << "-E- CbmRichTrackExtrapolationLittrack::DoExtrapolate: TrackParamArray missing!" << endl;
        return;
    }
    
    if ( NULL == globalTracks ) {
        cout << "-E- CbmRichTrackExtrapolationLittrack::DoExtrapolate: Track Array missing!" << endl;
        return;
    }
    
    Int_t nTracks = globalTracks->GetEntriesFast();
    for (Int_t iTrack=0; iTrack < nTracks; iTrack++){
        CbmGlobalTrack* gTrack = (CbmGlobalTrack*)globalTracks->At(iTrack);
        new((*extrapolatedTrackParams)[iTrack]) FairTrackParam();
        Int_t idSTS = gTrack->GetStsTrackIndex();
        if (idSTS < 0 ) continue;
        CbmStsTrack* pSTStr = (CbmStsTrack*) fStsTracks->At(idSTS);
        if ( NULL == pSTStr ) continue;
        
        CbmLitTrackParam litInParam, litOutParam;
        CbmLitConverterFairTrackParam::FairTrackParamToCbmLitTrackParam(pSTStr->GetParamLast(), &litInParam);
        std::vector<litfloat> F(25);
        litfloat length = 0;
        
        fLitPropagator->Propagate(&litInParam, &litOutParam, z, 11, &F, &length);
        
        FairTrackParam outParam;
        CbmLitConverterFairTrackParam::CbmLitTrackParamToFairTrackParam(&litOutParam, &outParam);
        
        *(FairTrackParam*)(extrapolatedTrackParams->At(iTrack)) = outParam;
    }
}

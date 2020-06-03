#include "CbmRichMCbmQaReal.h"

#include "TH1D.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TF1.h"
#include "TStyle.h"
#include "TEllipse.h"
#include "TLine.h"
#include "TMarker.h"
#include "TGeoNode.h"
#include "TGeoManager.h"
#include "TGeoBBox.h"
#include "TMath.h"
#include <TFile.h>
#include <TLegend.h>
#include <TBox.h>

#include "CbmRichDigi.h"
#include "CbmRichHit.h"
#include "CbmRichRingFinderHoughImpl.h"
#include "TLatex.h"
#include "CbmDrawHist.h"
#include "CbmTrackMatchNew.h"
#include "CbmRichRing.h"
#include "CbmRichHit.h"
#include "CbmMatchRecoToMC.h"
#include "CbmRichGeoManager.h"
#include "CbmRichPoint.h"
#include "CbmRichDraw.h"
#include "CbmGlobalTrack.h"
#include "CbmTrdTrack.h"
#include "CbmTofHit.h"
#include "CbmTofDigi.h"
#include "CbmStsDigi.h"
#include "CbmEvent.h"
#include "CbmTofTracklet.h"
#include "CbmRichUtil.h"

#include "CbmRichConverter.h"

#include "CbmUtils.h"
#include "CbmHistManager.h"
#include "CbmDigiManager.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <boost/assign/list_of.hpp>
#include <sstream>
#include <cmath>

using namespace std;
using boost::assign::list_of;

#define RichZPos 348.

CbmRichMCbmQaReal::CbmRichMCbmQaReal() :
                        FairTask("CbmRichMCbmQaReal"),
                        fT0Digis(nullptr),
                        fRichHits(nullptr),
                        fRichRings(nullptr),
                        fTofHits(nullptr),
                        fTofTracks(nullptr),
                        fCbmEvent(nullptr),
                        fHM(nullptr),
                        fXOffsetHisto(0.),
                        fEventNum(0),
                        fNofDrawnRings(0),
                        fNofDrawnRichTofEv(0),
                        fNofDrawnEvents(0),
                        fOutputDir("result")
{

}

InitStatus CbmRichMCbmQaReal::Init()
{
    cout << "CbmRichMCbmQaReal::Init" << endl;

    FairRootManager* ioman = FairRootManager::Instance();
    if (nullptr == ioman) { Fatal("CbmRichMCbmQaReal::Init","RootManager not instantised!"); }

    fDigiMan = CbmDigiManager::Instance();
    fDigiMan->Init();

    if ( ! fDigiMan->IsPresent(ECbmModuleId::kRich) )
      Fatal("CbmRichMCbmQaReal::Init", "No Rich Digis!");

    if ( ! fDigiMan->IsPresent(ECbmModuleId::kTof) )
      Fatal("CbmRichMCbmQaReal::Init", "No Tof Digis!");


    fRichHits =(TClonesArray*) ioman->GetObject("RichHit");
    if (nullptr == fRichHits) { Fatal("CbmRichMCbmQaReal::Init", "No Rich Hits!");}
    
    fRichRings =(TClonesArray*) ioman->GetObject("RichRing");
    if (nullptr == fRichRings) { Fatal("CbmRichMCbmQaReal::Init", "No Rich Rings!");}
    
    fTofHits =(TClonesArray*) ioman->GetObject("TofHit");
    if (nullptr == fTofHits) { Fatal("CbmRichMCbmQaReal::Init", "No Tof Hits!");}
    
    fTofTracks =(TClonesArray*) ioman->GetObject("TofTracks");
    if (nullptr == fTofTracks) { Fatal("CbmRichMCbmQaReal::Init", "No Tof Tracks!");}    

//     fT0Digis =(TClonesArray*) ioman->GetObject("CbmT0Digi");
//     if (nullptr == fT0Digis) { Fatal("CbmRichMCbmQaReal::Init", "No T0 Digis!");}

    fCbmEvent =(TClonesArray*) ioman->GetObject("CbmEvent");
    if (nullptr == fCbmEvent) { Fatal("CbmRichMCbmQaReal::Init", "No Event!");}
    
    InitHistograms();

    return kSUCCESS;
}

void CbmRichMCbmQaReal::InitHistograms()
{
    fHM = new CbmHistManager();

    fHM->Create1<TH1D>("fhNofEvents","fhNofEvents;Entries", 1, 0.5, 1.5);
    fHM->Create1<TH1D>("fhNofCbmEvents","fhNofCbmEvents;Entries", 1, 0.5, 1.5);
    fHM->Create1<TH1D>("fhNofCbmEventsRing","fhNofCbmEventsRing;Entries", 1, 0.5, 1.5);
    
    fHM->Create1<TH1D>("fhHitsInTimeslice","fhHitsInTimeslice;Timeslice;#Hits", 200, 1, 200);

    // nof objects per timeslice
    fHM->Create1<TH1D>("fhNofRichDigisInTimeslice","fhNofRichDigisInTimeslice;# RICH digis / timeslice;Entries", 100, -0.5, 999.5);
    fHM->Create1<TH1D>("fhNofRichHitsInTimeslice","fhNofRichHitsInTimeslice;# RICH hits / timeslice;Entries", 100, -0.5, 999.5);
    fHM->Create1<TH1D>("fhNofRichRingsInTimeslice","fhNofRichRingsInTimeslice;# RICH rings / timeslice;Entries", 10, -0.5, 9.5);

    // RICH hits
    fHM->Create2<TH2D>("fhRichHitXY","fhRichHitXY;RICH hit X [cm];RICH hit Y [cm];Entries", 67, -20.1 + fXOffsetHisto, 20.1 + fXOffsetHisto,  84, -25.2, 25.2);

    // RICH digis, the limits of log histograms are set in Exec method
    fHM->Create1<TH1D>("fhRichDigisTimeLog", "fhNofRichDigisTimeLog;Time [ns];Entries", 400, 0., 0.);
    fHM->Create1<TH1D>("fhRichDigisTimeLogZoom", "fhNofRichDigisTimeLogZoom;Time [ns];Entries", 400, 0., 0.);
    fHM->Create1<TH1D>("fhRichDigisTimeLogZoom2", "fhNofRichDigisTimeLogZoom2;Time [ns];Entries", 400, 0., 0.);
    
    fHM->Create1<TH1D>("fhRichRingsTimeLog", "fhNofRichRingsTimeLog;Time [ns];Entries", 400, 0., 0.);
    fHM->Create1<TH1D>("fhRichRingsTimeLogZoom", "fhNofRichRingsTimeLogZoom;Time [ns];Entries", 400, 0., 0.);
    fHM->Create1<TH1D>("fhRichRingsTimeLogZoom2", "fhNofRichRingsTimeLogZoom2;Time [ns];Entries", 400, 0., 0.);

    fHM->Create1<TH1D>("fhTofDigisTimeLog","fhTofDigisTimeLog;Time [ns];Entries", 400, 0., 0.);
    fHM->Create1<TH1D>("fhTofDigisTimeLogZoom","fhTofDigisTimeLogZoom;Time [ns];Entries", 400, 0., 0.);
    fHM->Create1<TH1D>("fhTofDigisTimeLogZoom2", "fhTofDigisTimeLogZoom2;Time [ns];Entries", 400, 0., 0.);

    fHM->Create1<TH1D>("fhStsDigisTimeLog","fhStsDigisTimeLog;Time [ns];Entries", 400, 0., 0.);
    fHM->Create1<TH1D>("fhStsDigisTimeLogZoom","fhStsDigisTimeLogZoom;Time [ns];Entries", 400, 0., 0.);
    fHM->Create1<TH1D>("fhStsDigisTimeLogZoom2", "fhStsDigisTimeLogZoom2;Time [ns];Entries", 400, 0., 0.);

    fHM->Create1<TH1D>("fhT0DigisTimeLog","fhT0DigisTimeLog;Time [ns];Entries", 400, 0., 0.);
    fHM->Create1<TH1D>("fhT0DigisTimeLogZoom","fhT0DigisTimeLogZoom;Time [ns];Entries", 400, 0., 0.);
    fHM->Create1<TH1D>("fhT0DigisTimeLogZoom2", "fhT0DigisTimeLogZoom2;Time [ns];Entries", 400, 0., 0.);
    
    //ToT
    fHM->Create1<TH1D>("fhRichDigisToT","fhRichDigisToT;ToT [ns];Entries", 601, 9.975, 40.025);
    fHM->Create1<TH1D>("fhRichHitToT","fhRichHitToT;ToT [ns];Entries", 601, 9.975, 40.025);

    // RICH rings
    fHM->Create2<TH2D>("fhRichRingXY","fhRichRingXY;Ring center X [cm];Ring center Y [cm];Entries", 100, -20 + fXOffsetHisto, 20 + fXOffsetHisto,  100, -20, 20);
    fHM->Create1<TH1D>("fhRichRingRadius","fhRichRingRadius;Ring radius [cm];Entries", 100, 0., 7.);
    fHM->Create1<TH1D>("fhNofHitsInRing","fhNofHitsInRing;# hits in ring;Entries", 50, -0.5, 49.5);
    
    //Tof Rich correlation
    fHM->Create2<TH2D>("fhTofRichX","fhTofRichX;Rich Hit X [cm];TofHit X [cm];Entries", 67, -20.1 + fXOffsetHisto, 20.1 + fXOffsetHisto,  400, -50, 110);
    fHM->Create2<TH2D>("fhTofRichY","fhTofRichY;Rich Hit Y [cm];TofHit Y [cm];Entries", 84, -25.2, 25.2,  200, -80, 80);
    
    //Tof Rich correlation
    fHM->Create2<TH2D>("fhTofRichX_zoomed","fhTofRichX_zoomed;Rich Hit X [cm];TofHit X [cm];Entries", 27, -8.1 + fXOffsetHisto, 8.1 + fXOffsetHisto,  180, -15, 75);
    fHM->Create2<TH2D>("fhTofRichY_zoomed","fhTofRichY_zoomed;Rich Hit Y [cm];TofHit Y [cm];Entries", 14, 7.8, 16.2,  30, -5., 25);
    
    fHM->Create2<TH2D>("fhClosTrackRingX","fhClosTrackRingX;Rich Ring center X [cm];Tof Track X [cm];Entries", 67, -20.1 + fXOffsetHisto, 20.1 + fXOffsetHisto,  400, -50, 110);
    fHM->Create2<TH2D>("fhClosTrackRingY","fhClosTrackRingY;Rich Ring center Y [cm];Tof Track Y [cm];Entries", 84, -25.2, 25.2,  200, -80, 80);
    
    fHM->Create2<TH2D>("fhTofRichRingX","fhTofRichRingX;Rich Ring Center X [cm];TofHit X [cm];Entries", 100, -20 + fXOffsetHisto, 20 + fXOffsetHisto,  400, -50, 110);
    fHM->Create2<TH2D>("fhTofRichRingY","fhTofRichRingY;Ring Ring Center Y [cm];TofHit Y [cm];Entries", 125, -25, 25,  200, -80, 80);
    fHM->Create2<TH2D>("fhTofRichRingXZ","fhTofRichXZ; Z [cm];Hit/Ring X [cm];Entries", 140, 230, 370,  400, -50, 110);
    
    fHM->Create2<TH2D>("fhTofTrackRichRingXY","fhTofTrackRichRingXY; X [cm]; Y [cm];Entries", 100, -20 + fXOffsetHisto, 20 + fXOffsetHisto,  120, -10, 20); //1bin == 2mm
    fHM->Create2<TH2D>("fhTofClosTrackRichRingXY","fhTofClosTrackRichRingXY; X [cm]; Y [cm];Entries", 100, -20 + fXOffsetHisto, 20 + fXOffsetHisto,  120, -10, 20); //1bin == 2mm
    
    fHM->Create3<TH3D>("fhTofXYZ","fhTofXYZ;Tof Hit X [cm];TofHit Z [cm];Tof Hit Y [cm];Entries", 100, -20, 20,  141,  230.,  370.,  100, -20, 20);
    fHM->Create1<TH1D>("fhTofHitsZ","fhTofHitsZ;Z [cm];Entries", 350, -0.5, 349.5);
    fHM->Create2<TH2D>("fhTofHitsXZ","fhTofHitsXZ;Z [cm];X [cm];Entries", 350, -0.5, 349.5, 400, -50, 110);
    //Tof Tracks
    fHM->Create1<TH1D>("fhTofTracksPerEvent","fhTofTracksPerEvent;NofTracks/RichEvent;Entries", 20, -0.5, 19.5);
    fHM->Create2<TH2D>("fhTofTracksXY","fhTofTracksXY;X[cm];Y[cm];NofTracks/cm^2",50 , -20 + fXOffsetHisto, 30 + fXOffsetHisto, 180,-90,90); // projected in RICH Plane
    fHM->Create2<TH2D>("fhTofTracksXYRICH","fhTofTracksXYRICH;X[cm];Y[cm];NofTracks/cm^2", 50 , -20 + fXOffsetHisto, 30 + fXOffsetHisto, 180,-90,90); // projected in RICH Plane
    fHM->Create1<TH1D>("fhNofTofTracks","fhNofTofTracks;X[cm];Y[cm];NofTracks", 4,0,4); // 1: All 2: left; 3: right; 4: RICH
    
    fHM->Create1<TH1D>("fhTrackRingDistance","fhTrackRingDistance;TrackRingDistance [cm];Entries", 31, -0.5, 30.5);
    fHM->Create2<TH2D>("fhTrackRingDistanceVSRingradius","fhTrackRingDistanceVSRingradius;TrackRingDistance [cm];RingRadius [cm];Entries", 81, -0.5, 80.5, 100, 0., 7.);

    fHM->Create1<TH1D>("fhTofBetaTracksWithHitsNoRing","fhTofBetaTracksWithHitsNoRing; \\beta;Entries", 111, -0.005, 1.105);
    fHM->Create1<TH1D>("fhTofBetaTracksWithHits","fhTofBetaTracksWithHits; \\beta;Entries", 111, -0.005, 1.105);
    fHM->Create1<TH1D>("fhTofBetaTracksNoRing","fhTofBetaTracksNoRing; \\beta;Entries", 111, -0.005, 1.105);
    fHM->Create1<TH1D>("fhTofBetaTrackswithClosestRingInRange","fhTofBetaTrackswithClosestRingInRange; \\beta;Entries", 111, -0.005, 1.105);
    
    fHM->Create1<TH1D>("fhTofBetaRing","fhTofBetaRing; \\beta;Entries", 111, -0.005, 1.105);
    fHM->Create1<TH1D>("fhTofBetaAll","fhTofBetaAll; \\beta;Entries", 111, -0.005, 1.105);
    fHM->Create2<TH2D>("fhTofBetaVsRadius","fhTofBetaVsRadius; \\beta;ring radius [cm];Entries", 102, -0.005, 1.015, 100, 0., 7.);
    fHM->Create2<TH2D>("fhTofBetaRingDist","fhTofBetaRingDist; \\beta;ring Dist [cm];Entries", 102, -0.005, 1.015, 100, 0., 20.);
    fHM->Create1<TH1D>("fhTofBetaAllFullAcc","fhTofBetaAllFullAcc; \\beta;Entries", 221, -1.105, 1.105);
    
    fHM->Create1<TH1D>("fhRingDeltaTime","fhRingDeltaTime; \\Delta Time/ns;Entries", 101, -10.1, 10.1);
    fHM->Create1<TH1D>("fhRingToTs","fhRingToTs; ToT/ns;Entries", 601, 9.975, 40.025);
    fHM->Create1<TH1D>("fhRingLE","fhRingLE;LE/ns;Entries",201,-0.5,200.5);
    fHM->Create1<TH1D>("fhGoodRingLE","fhGoodRingLE;LE/ns;Entries",201,-0.5,200.5);
    fHM->Create1<TH1D>("fhRingNoClTrackLE","fhRingNoClTrackLE;LE/ns;Entries",201,-0.5,200.5);
    fHM->Create1<TH1D>("fhRingClTrackFarAwayLE","fhRingClTrackFarAwayLE;LE/ns;Entries",201,-0.5,200.5);
    fHM->Create2<TH2D>("fhRingLEvsToT","fhRingLEvsToT;LE/ns;ToT/ns;Entries",201, -0.5, 200.5, 601, 9.975, 40.025);
    
    fHM->Create1<TH1D>("fhInnerRingDeltaTime","fhInnerRingDeltaTime; \\Delta Time/ns;Entries", 101, -10.1, 10.1);
    fHM->Create1<TH1D>("fhInnerRingToTs","fhInnerRingToTs; ToT/ns;Entries", 601, 9.975, 40.025);
    fHM->Create1<TH1D>("fhInnerRingLE","fhInnerRingLE;LE/ns;Entries",201,-0.5,200.5);
    fHM->Create1<TH1D>("fhInnerGoodRingLE","fhInnerGoodRingLE;LE/ns;Entries",201,-0.5,200.5);
    fHM->Create1<TH1D>("fhInnerRingNoClTrackLE","fhInnerRingNoClTrackLE;LE/ns;Entries",201,-0.5,200.5);
    fHM->Create1<TH1D>("fhInnerRingClTrackFarAwayLE","fhInnerRingClTrackFarAwayLE;LE/ns;Entries",201,-0.5,200.5);
    fHM->Create1<TH1D>("fhInnerRingFlag","fhInnerRingFlag;Has|HasNot;Entries",2,-0.5,1.5);
    fHM->Create1<TH1D>("fhNofInnerHits","fhNofInnerHits;#Hits;Entries",31,-0.5,30.5);
    
    fHM->Create1<TH1D>("fhDiRICHsInRegion","fhNofInnerHits;#Hits;DiRICH",4096,28672,32767);
}


void CbmRichMCbmQaReal::Exec(Option_t* /*option*/)
{
    fEventNum++;
    fHM->H1("fhNofEvents")->Fill(1);
    cout << "CbmRichMCbmQaReal, event No. " <<  fEventNum << endl;

    if (fEventNum == 2) {
        double minTime = std::numeric_limits<double>::max();
        for (int i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kRich); i++) {
            const CbmRichDigi* richDigi = fDigiMan->Get<CbmRichDigi>(i);
           // fHM->H1("fhRichDigisToT")->Fill(richDigi->GetToT());
            if (richDigi->GetTime() < minTime) minTime = richDigi->GetTime();
        }

        double dT = 40e9;
        double dTZoom1 = 0.8e9;
        double dTZoom2 = 0.008e9;
        fHM->H1("fhRichDigisTimeLog")->GetXaxis()->SetLimits(minTime, minTime+dT);
        fHM->H1("fhRichDigisTimeLogZoom")->GetXaxis()->SetLimits(minTime, minTime+dTZoom1);
        fHM->H1("fhRichDigisTimeLogZoom2")->GetXaxis()->SetLimits(minTime, minTime + dTZoom2);

        fHM->H1("fhRichRingsTimeLog")->GetXaxis()->SetLimits(minTime, minTime + dT);
        fHM->H1("fhRichRingsTimeLogZoom")->GetXaxis()->SetLimits(minTime, minTime + dTZoom1);
        fHM->H1("fhRichRingsTimeLogZoom2")->GetXaxis()->SetLimits(minTime, minTime + dTZoom2);
        
        fHM->H1("fhTofDigisTimeLog")->GetXaxis()->SetLimits(minTime, minTime+dT);
        fHM->H1("fhTofDigisTimeLogZoom")->GetXaxis()->SetLimits(minTime, minTime+dTZoom1);
        fHM->H1("fhTofDigisTimeLogZoom2")->GetXaxis()->SetLimits(minTime, minTime + dTZoom2);

        fHM->H1("fhStsDigisTimeLog")->GetXaxis()->SetLimits(minTime, minTime+dT);
        fHM->H1("fhStsDigisTimeLogZoom")->GetXaxis()->SetLimits(minTime, minTime+dTZoom1);
        fHM->H1("fhStsDigisTimeLogZoom2")->GetXaxis()->SetLimits(minTime, minTime + dTZoom2);

        fHM->H1("fhT0DigisTimeLog")->GetXaxis()->SetLimits(minTime, minTime+dT);
        fHM->H1("fhT0DigisTimeLogZoom")->GetXaxis()->SetLimits(minTime, minTime+dTZoom1);
        fHM->H1("fhT0DigisTimeLogZoom2")->GetXaxis()->SetLimits(minTime, minTime + dTZoom2);
    }

    {
        for (int i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kRich); i++) {
            const CbmRichDigi* richDigi = fDigiMan->Get<CbmRichDigi>(i);
            fHM->H1("fhRichDigisToT")->Fill(richDigi->GetToT());
        }
    }
    
    int nofRichDigis = fDigiMan->GetNofDigis(ECbmModuleId::kRich);
    fHM->H1("fhNofRichDigisInTimeslice")->Fill(nofRichDigis);
    for (int i = 0; i < nofRichDigis; i++) {
        const CbmRichDigi* digi = fDigiMan->Get<CbmRichDigi>(i);
        fHM->H1("fhRichDigisTimeLog")->Fill(digi->GetTime() );
        fHM->H1("fhRichDigisTimeLogZoom")->Fill(digi->GetTime() );
        fHM->H1("fhRichDigisTimeLogZoom2")->Fill(digi->GetTime());
    }

    int nofRichRings= fRichRings->GetEntries();
    for (int i = 0; i < nofRichRings; i++)
    {
        CbmRichRing *ring = static_cast<CbmRichRing *>(fRichRings->At(i));
        fHM->H1("fhRichRingsTimeLog")->Fill(ring->GetTime());
        fHM->H1("fhRichRingsTimeLogZoom")->Fill(ring->GetTime());
        fHM->H1("fhRichRingsTimeLogZoom2")->Fill(ring->GetTime());
    }
    
    int nofTofDigis = fDigiMan->GetNofDigis(ECbmModuleId::kTof);
    for (int i = 0; i < nofTofDigis; i++) {
        const CbmTofDigi* digi = fDigiMan->Get<CbmTofDigi>(i);
        fHM->H1("fhTofDigisTimeLog")->Fill(digi->GetTime() );
        fHM->H1("fhTofDigisTimeLogZoom")->Fill(digi->GetTime() );
        fHM->H1("fhTofDigisTimeLogZoom2")->Fill(digi->GetTime());
    }

    if (fDigiMan->IsPresent(ECbmModuleId::kSts)){
        int nofStsDigis = fDigiMan->GetNofDigis(ECbmModuleId::kSts);
        for (int i = 0; i < nofStsDigis; i++) {
            const CbmStsDigi* digi = fDigiMan->Get<CbmStsDigi>(i);
            fHM->H1("fhStsDigisTimeLog")->Fill(digi->GetTime() );
            fHM->H1("fhStsDigisTimeLogZoom")->Fill(digi->GetTime() );
            fHM->H1("fhStsDigisTimeLogZoom2")->Fill(digi->GetTime());
        }
    }

//     int nofT0Digis = fT0Digis->GetEntries();
//     for (int i = 0; i < nofT0Digis; i++) {
//         CbmDigi* digi = static_cast<CbmDigi*>(fT0Digis->At(i));
//         fHM->H1("fhT0DigisTimeLog")->Fill(digi->GetTime() );
//         fHM->H1("fhT0DigisTimeLogZoom")->Fill(digi->GetTime() );
//         fHM->H1("fhT0DigisTimeLogZoom2")->Fill(digi->GetTime());
//     }


    int nofRichHits = fRichHits->GetEntries();
    fHM->H1("fhNofRichHitsInTimeslice")->Fill(nofRichHits);
    fHM->H1("fhHitsInTimeslice")->Fill(fEventNum,nofRichHits);
    for (int iH = 0; iH < nofRichHits; iH++) {
        CbmRichHit* richHit = static_cast<CbmRichHit*>(fRichHits->At(iH));
        fHM->H2("fhRichHitXY")->Fill(richHit->GetX(), richHit->GetY());
        fHM->H1("fhRichHitToT")->Fill(richHit->GetToT());
        //printf("HitToT: %f \n", richHit->GetToT());
    }

    
    //CBMEVENT
    auto fNCbmEvent = fCbmEvent->GetEntriesFast();
    
    for (int i=0;i<fNCbmEvent;i++){
        fHM->H1("fhNofCbmEvents")->Fill(1);
        CbmEvent* ev = static_cast<CbmEvent*>(fCbmEvent->At(i));
        std::vector<int> ringIndx;
        std::vector<int> evRichHitIndx;
        

        for (int j=0;j<ev->GetNofData(ECbmDataType::kRichHit);j++){
            auto iRichHit = ev->GetIndex(ECbmDataType::kRichHit, j);
            evRichHitIndx.push_back(iRichHit);
            CbmRichHit* richHit = static_cast<CbmRichHit*>(fRichHits->At(iRichHit));
            //std::cout<<"\t\t *  "<<i<<". Event, Hit "<< j <<": "<< iRichHit <<"\t " << std::fixed << std::setprecision(5) << richHit->GetTime() <<std::endl;
            
            int nofRichRings2= fRichRings->GetEntries();
            for (int l = 0; l < nofRichRings2; l++)
            {
                CbmRichRing *ring = static_cast<CbmRichRing *>(fRichRings->At(l));
                auto NofRingHits = ring->GetNofHits();
                for (int m = 0; m < NofRingHits; m++)
                {
                    auto RingHitIndx = ring->GetHit(m);
                    if (RingHitIndx == iRichHit) {
                        Bool_t used = false;
                        for (auto check : ringIndx) {
                             if (check == l){ used = true; break;}
                        }
                        if (used == false) ringIndx.push_back(l);
                        break;
                    }  
                }
            }
            
            for (int k=0;k<ev->GetNofData(ECbmDataType::kTofHit);k++){
                auto iTofHit = ev->GetIndex(ECbmDataType::kTofHit, k);
                CbmTofHit* tofHit = static_cast<CbmTofHit*>(fTofHits->At(iTofHit));
                if (tofHit->GetZ() < 2.) continue; // Cut T0 away!
                fHM->H2("fhTofRichX")->Fill(richHit->GetX(),tofHit->GetX());
                fHM->H2("fhTofRichY")->Fill(richHit->GetY(),tofHit->GetY());
                fHM->H2("fhTofRichX_zoomed")->Fill(richHit->GetX(),tofHit->GetX());
                fHM->H2("fhTofRichY_zoomed")->Fill(richHit->GetY(),tofHit->GetY()); 
            }
        }
        
        
       
       DrawEvent(ev, ringIndx, 1); 
       
//       std::cout<<DrawCbmEvent<<std::endl;
       // for (int j=0;j<ev->GetNofData(ECbmDataType::kRichDigi);j++){
       //     auto iRichDigi = ev->GetIndex(ECbmDataType::kRichDigi, j);
       //     CbmRichDigi* richDigi = static_cast<CbmRichDigi*>(fRichDigis->At(iRichDigi));
       //       std::cout<<"\t\t *  "<<i<<". Event, Digi "<< j <<": "<< iRichDigi <<"\t " << std::fixed << std::setprecision(5) <<  richDigi->GetTime()<<std::endl;
       // }
        
        //std::vector<int> evTofTrack;
        
        //*** Tracks in CbmEvent -> Here Trigger is on! This is seen in Track Position
        //std::cout<<"TRACKS in CbmEvent: "<<ev->GetNofData(ECbmDataType::kTofTrack)<<std::endl;
        for (int j=0;j<ev->GetNofData(ECbmDataType::kTofTrack);j++){
            auto iTofTrack = ev->GetIndex(ECbmDataType::kTofTrack, j);
            CbmTofTracklet* tTrack = static_cast<CbmTofTracklet*>(fTofTracks->At(iTofTrack));
           
            fHM->H2("fhTofTracksXY")->Fill(tTrack->GetFitX(RichZPos),tTrack->GetFitY(RichZPos));
            fHM->H1("fhNofTofTracks")->Fill(0.5); // 1: All 2: left; 3: right; 4: RICH
            fHM->H1("fhTofBetaAllFullAcc")->Fill(getBeta(tTrack));
            //std::cout<<"beta Track "<< j <<": "<<getBeta(tTrack)<<std::endl;
            if (isAccmRICH(tTrack)){
                if (RestrictToFullAcc(tTrack)){
                    fHM->H2("fhTofTracksXYRICH")->Fill(tTrack->GetFitX(RichZPos),tTrack->GetFitY(RichZPos)); // projected in RICH Plane
                    fHM->H1("fhNofTofTracks")->Fill(3.5); // 1: All 2: left; 3: right; 4: RICH
                    std::vector<int> evTofTrack;
                    evTofTrack.push_back(iTofTrack);
                    DrawRichTofEv(evRichHitIndx,evTofTrack);
                    
                    if (ringIndx.size() == 0 && evRichHitIndx.size() > 0) fHM->H1("fhTofBetaTracksWithHitsNoRing")->Fill(getBeta(tTrack)); // no Ring in CbmEvent found
                    if (evRichHitIndx.size() > 0)fHM->H1("fhTofBetaTracksWithHits")->Fill(getBeta(tTrack)); 
                    if (ringIndx.size() == 0)fHM->H1("fhTofBetaTracksNoRing")->Fill(getBeta(tTrack));
                    if (FindClosestRing(tTrack,ringIndx).first > -1) fHM->H1("fhTofBetaTrackswithClosestRingInRange")->Fill(getBeta(tTrack));
                    //if (ringIndx.size() > 0)fHM->H1("fhTofBetaTrackswithRing")->Fill(getBeta(tTrack)); // ring is somewehere in Acc
                    fHM->H1("fhTofBetaAll")->Fill(getBeta(tTrack));
                }
            }
            if (tTrack->GetFitX(RichZPos) > 30) { // right
                fHM->H1("fhNofTofTracks")->Fill(2.5); // 1: All 2: left; 3: right; 4: RICH
            } else { //left
                fHM->H1("fhNofTofTracks")->Fill(1.5);
            }
        } 
        
        
        std::vector<TVector3> RichTofEv;
        
        for (int j=0;j<ev->GetNofData(ECbmDataType::kTofHit);j++){
            auto iTofHit = ev->GetIndex(ECbmDataType::kTofHit, j);
            CbmTofHit* tofHit = static_cast<CbmTofHit*>(fTofHits->At(iTofHit));
            if (tofHit->GetZ() < 2.) continue; // Cut T0 away!
            
            fHM->H1("fhTofHitsZ")->Fill(tofHit->GetZ());
            fHM->H1("fhTofHitsXZ")->Fill(tofHit->GetZ(),tofHit->GetX());
            fHM->H3("fhTofXYZ")->Fill(tofHit->GetX(),tofHit->GetZ(),tofHit->GetY());
            
            if (ringIndx.size() > 0) {
                //fHM->H2("fhTofRichRingXZ")->Fill(tofHit->GetZ(),tofHit->GetX());
                //RichTofEv.emplace_back(tofHit->GetX(),tofHit->GetY(),tofHit->GetZ());
                for (unsigned int rings=0; rings < ringIndx.size();rings++) {
                    CbmRichRing *ring = static_cast<CbmRichRing *>(fRichRings->At(ringIndx[rings])); 
                    fHM->H2("fhTofRichRingX")->Fill(ring->GetCenterX(),tofHit->GetX());
                    fHM->H2("fhTofRichRingY")->Fill(ring->GetCenterY(),tofHit->GetY());
                }
            }
        }
        
        if (ringIndx.size() > 0) { // Ring in CbmEvent
            std::vector<CbmTofTracklet*> TracksOfEvnt;
            //TRACKS
            auto nofTofTracks = ev->GetNofData(ECbmDataType::kTofTrack);
            fHM->H1("fhTofTracksPerEvent")->Fill(nofTofTracks);
            for (int j=0;j<nofTofTracks;j++){
                auto iTofTrack = ev->GetIndex(ECbmDataType::kTofTrack, j);
                CbmTofTracklet *track = static_cast<CbmTofTracklet *>(fTofTracks->At(iTofTrack));
                fHM->H2("fhTofTrackRichRingXY")->Fill(track->GetFitX(RichZPos),track->GetFitY(RichZPos));
                TracksOfEvnt.emplace_back(track);
            } 
            
            //rings
            for (unsigned int rings=0; rings < ringIndx.size();rings++) {
                CbmRichRing *ring = static_cast<CbmRichRing *>(fRichRings->At(ringIndx[rings])); 
                
                //DrawRing(ring,TracksOfEvnt,true); // here normally; now only Rings with high BETA
                auto clTrack = FindClosestTrack(ring,TracksOfEvnt); // has no cut on distance
                analyseRing(ring,ev,clTrack);
                if (clTrack.first > -1){ 
                    // first: TrackIndex;  second: Distance;
                    //FIXME
                    //if (getBeta(TracksOfEvnt[clTrack.first]) > 0.9) DrawRing(ring,TracksOfEvnt,true);
                    fHM->H1("fhTrackRingDistance")->Fill(clTrack.second);
                    fHM->H2("fhTrackRingDistanceVSRingradius")->Fill(clTrack.second,ring->GetRadius());
                    fHM->H2("fhClosTrackRingX")->Fill(ring->GetCenterX(),TracksOfEvnt[clTrack.first]->GetFitX(RichZPos));
                    fHM->H2("fhClosTrackRingY")->Fill(ring->GetCenterY(),TracksOfEvnt[clTrack.first]->GetFitY(RichZPos));
                    fHM->H2("fhTofClosTrackRichRingXY")->Fill(TracksOfEvnt[clTrack.first]->GetFitX(RichZPos),TracksOfEvnt[clTrack.first]->GetFitY(RichZPos));
                    if (cutDistance(clTrack) && cutRadius(ring)) { //Good Ring
                        fHM->H1("fhTofBetaRing")->Fill(getBeta(TracksOfEvnt[clTrack.first]));
                        fHM->H2("fhTofBetaRingDist")->Fill(getBeta(TracksOfEvnt[clTrack.first]),clTrack.second);
                        fHM->H2("fhTofBetaVsRadius")->Fill(getBeta(TracksOfEvnt[clTrack.first]),ring->GetRadius());
                    }
                }
                
                fHM->H2("fhTofRichRingXZ")->Fill(RichZPos,ring->GetCenterX()); //Z Axis by Hand because ring has no Z component
                RichTofEv.emplace_back(ring->GetCenterX(),ring->GetCenterY(),RichZPos);  
                // Draw XY position of center of rings; later add the Tracks in Tof
                fHM->H2("fhTofTrackRichRingXY")->Fill(ring->GetCenterX(),ring->GetCenterY(),3); // 3 to change color for Rings
            }

            //DrawRichTofEv(RichTofEv);
            fHM->H1("fhNofCbmEventsRing")->Fill(1);
        }
        
        
//         std::cout<< "TOF Digis:\t" << ev->GetNofData(ECbmDataType::kTofDigi) <<std::endl;
//         std::cout<< "TOF Hits:\t" << ev->GetNofData(ECbmDataType::kTofHit) <<std::endl;
//         std::cout<< "TOF Tracks:\t" << ev->GetNofData(ECbmDataType::kTofTrack) <<std::endl;
        
     
    } //End CbmEvent loop
    

    RichRings();
    
}

void CbmRichMCbmQaReal::RichRings()
{
    int nofRichRings = fRichRings->GetEntries();
    fHM->H1("fhNofRichRingsInTimeslice")->Fill(nofRichRings);
    for (int i = 0; i < nofRichRings; i++) {
        CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(i));
        if (ring == nullptr) continue;
        //DrawRing(ring);
        fHM->H2("fhRichRingXY")->Fill(ring->GetCenterX(), ring->GetCenterY());
        fHM->H1("fhRichRingRadius")->Fill(ring->GetRadius());
        fHM->H1("fhNofHitsInRing")->Fill(ring->GetNofHits());
    }
}

std::pair<int, double> CbmRichMCbmQaReal::FindClosestTrack(const CbmRichRing* ring, const std::vector<CbmTofTracklet*> track){
    int ringX = ring->GetCenterX();
    int ringY = ring->GetCenterY();

    int closTrack   = -1;
    double closDist = -999999.99;

    for (unsigned int indx = 0; indx < track.size(); ++indx){
        
        //Calc if Track is in Ring (+20% )
        const double xDist = (track[indx]->GetFitX(RichZPos) - ringX);
        const double yDist = (track[indx]->GetFitY(RichZPos) - ringY);
        const double rDist = std::sqrt(xDist*xDist + yDist*yDist);        
        const double RadiusFactor = 1.2; // Factor of how big radius of acceptance should       

        if (rDist < ring->GetRadius()*RadiusFactor) {
            std::cout<<"Track in defined Ring range ("<<ring->GetRadius()*RadiusFactor<<"cm) (RingRadius: "<<ring->GetRadius()<<"cm).  ";
        }
        if (indx== 0) {
            closDist  = rDist;
            closTrack = indx;
        } else {
            if ( closDist > rDist ) {
                closDist  = rDist;
                closTrack = indx;
            } 
        }
    }

    std::pair<int, double> p;
    p.first  = closTrack;
    p.second = closDist;

    return p;
}

std::pair<int, double> CbmRichMCbmQaReal::FindClosestRing(CbmTofTracklet* track, std::vector<int> &ringIndx){
    // Closest Ring to Track in +20% Ring Radius!
    const double x_track = track->GetFitX(RichZPos);
    const double y_track = track->GetFitY(RichZPos);

    int closTrack   = -1;
    double closDist = -999999.99;

    for (unsigned int indx = 0; indx < ringIndx.size(); ++indx){
        CbmRichRing *ring = static_cast<CbmRichRing *>(fRichRings->At(ringIndx[indx]));
        
        int ringX = ring->GetCenterX();
        int ringY = ring->GetCenterY();
        
        //Calc if Track is in Ring (+20% )
        const double xDist = (x_track - ringX);
        const double yDist = (y_track - ringY);
        const double rDist = std::sqrt(xDist*xDist + yDist*yDist);        
        const double RadiusFactor = 1.2; // Factor of how big radius of acceptance should       

        if (rDist < ring->GetRadius()*RadiusFactor) {
            //std::cout<<"Track in defined Ring range ("<<ring->GetRadius()*RadiusFactor<<"cm) (RingRadius: "<<ring->GetRadius()<<"cm).  ";

            if (indx== 0) {
                closDist  = rDist;
                closTrack = indx;
            } else {
                if ( closDist > rDist ) {
                    closDist  = rDist;
                    closTrack = indx;
                } 
            }
        }
    }  
    std::pair<int, double> p;
    p.first  = closTrack;
    p.second = closDist;

    return p;
}

void CbmRichMCbmQaReal::DrawHist()
{
       cout.precision(4);
  
     //SetDefaultDrawStyle();
     double nofEvents = fHM->H1("fhNofCbmEvents")->GetEntries();
     fHM->ScaleByPattern("fh_.*", 1./nofEvents);

    {
        fHM->CreateCanvas("rich_mcbm_fhNofCbmEvents","rich_mcbm_fhNofCbmEvents", 600 , 600);
        DrawH1(fHM->H1("fhNofCbmEvents"));
    }
    
    {
        fHM->CreateCanvas("rich_mcbm_fhNofCbmEventsRing","rich_mcbm_fhNofCbmEventsRing", 600 , 600);
        DrawH1(fHM->H1("fhNofCbmEventsRing"));
    }    
    
    {
        fHM->CreateCanvas("rich_mcbm_fhNofEvents","rich_mcbm_fhNofEvents", 600 , 600);
        DrawH1(fHM->H1("fhNofEvents"));
    }
    
    {
        fHM->CreateCanvas("HitsInTimeslice","HitsInTimeslice", 600 , 600);
        DrawH1(fHM->H1("fhHitsInTimeslice"));
    }
 
    {
        TCanvas* c = fHM->CreateCanvas("rich_mcbm_nofObjectsInTimeslice","rich_mcbm_nofObjectsInTimeslice", 1500 , 500);
        c->Divide(3,1);
        c->cd(1);
        DrawH1(fHM->H1("fhNofRichDigisInTimeslice"), kLinear, kLog);
        c->cd(2);
        DrawH1(fHM->H1("fhNofRichHitsInTimeslice"), kLinear, kLog);
        c->cd(3);
        DrawH1(fHM->H1("fhNofRichRingsInTimeslice"), kLinear, kLog);
    }

    {
        TCanvas* c = fHM->CreateCanvas("rich_mcbm_XY","rich_mcbm_XY", 1200 , 600);
        c->Divide(2,1);
        c->cd(1);
        DrawH2(fHM->H2("fhRichHitXY"));
        c->cd(2);
        DrawH2(fHM->H2("fhRichRingXY"));
    }

    
    {
        TCanvas* c = fHM->CreateCanvas("rich_tof_XY","rich_tof_XY", 1200 , 600);
        c->Divide(2,1);
        c->cd(1);
        DrawH2(fHM->H2("fhTofRichX"));
        c->cd(2);
        DrawH2(fHM->H2("fhTofRichY"));
    }
    

    {
        TCanvas* c = fHM->CreateCanvas("rich_tof_XY_zoomed","rich_tof_XY_zoomed", 1200 , 600);
        c->Divide(2,1);
        c->cd(1);
        DrawH2(fHM->H2("fhTofRichX_zoomed"));
        c->cd(2);
        DrawH2(fHM->H2("fhTofRichY_zoomed"));
    }
    
    
    {
        TCanvas* c = fHM->CreateCanvas("rich_mcbm_richDigisTimeLog","rich_mcbm_richDigisTimeLog", 1200 , 1200);
        c->Divide(1,2);
        c->cd(1);
        DrawH1({fHM->H1("fhStsDigisTimeLog"), fHM->H1("fhTofDigisTimeLog"), fHM->H1("fhT0DigisTimeLog"), fHM->H1("fhRichDigisTimeLog")},
                {"STS", "TOF", "T0", "RICH"}, kLinear, kLog, true, 0.87, 0.75, 0.99, 0.99);
        gPad->SetLeftMargin(0.1);
        gPad->SetRightMargin(0.10);
        fHM->H1("fhStsDigisTimeLog")->GetYaxis()->SetTitleOffset(0.7);
        fHM->H1("fhStsDigisTimeLog")->SetMinimum(0.9);
        c->cd(2);
        DrawH1({fHM->H1("fhStsDigisTimeLogZoom"), fHM->H1("fhTofDigisTimeLogZoom"),   fHM->H1("fhT0DigisTimeLogZoom"), fHM->H1("fhRichDigisTimeLogZoom")},
                {"STS", "TOF", "T0", "RICH"}, kLinear, kLog, true, 0.87, 0.75, 0.99, 0.99);
        gPad->SetLeftMargin(0.1);
        gPad->SetRightMargin(0.1);
        fHM->H1("fhStsDigisTimeLogZoom")->GetYaxis()->SetTitleOffset(0.7);
        fHM->H1("fhStsDigisTimeLogZoom")->SetMinimum(0.9);
    }

    {
        fHM->CreateCanvas("rich_mcbm_richDigisTimeLog2", "rich_mcbm_richDigisTimeLog2", 1200, 600);
        DrawH1({fHM->H1("fhStsDigisTimeLogZoom2"), fHM->H1("fhTofDigisTimeLogZoom2"), fHM->H1("fhT0DigisTimeLogZoom2"), fHM->H1("fhRichDigisTimeLogZoom2")},
               {"STS", "TOF", "T0", "RICH"}, kLinear, kLog, true, 0.87, 0.75, 0.99, 0.99);
        gPad->SetLeftMargin(0.1);
        gPad->SetRightMargin(0.1);
        fHM->H1("fhStsDigisTimeLogZoom2")->GetYaxis()->SetTitleOffset(0.7);
        fHM->H1("fhStsDigisTimeLogZoom2")->SetMinimum(0.9);

    }

    {

        TCanvas *c = fHM->CreateCanvas("rich_mcbm_richRingsTimeLog", "rich_mcbm_richRingsTimeLog", 1200, 1200);
        c->Divide(1, 2);
        c->cd(1);
        DrawH1({fHM->H1("fhRichDigisTimeLog"), fHM->H1("fhRichRingsTimeLog")}, {"Digis", "Rings"}, kLinear, kLog, true, 0.87, 0.75, 0.99, 0.99);
        gPad->SetLeftMargin(0.1);
        gPad->SetRightMargin(0.10);
        fHM->H1("fhRichDigisTimeLog")->GetYaxis()->SetTitleOffset(0.7);
        fHM->H1("fhRichDigisTimeLog")->SetMinimum(0.9);
        c->cd(2);
        DrawH1({fHM->H1("fhRichDigisTimeLogZoom"), fHM->H1("fhRichRingsTimeLogZoom")}, {"Digis", "Rings"}, kLinear, kLog, true, 0.87, 0.75, 0.99, 0.99);
        gPad->SetLeftMargin(0.1);
        gPad->SetRightMargin(0.1);
        fHM->H1("fhRichDigisTimeLogZoom")->GetYaxis()->SetTitleOffset(0.7);
        fHM->H1("fhRichDigisTimeLogZoom")->SetMinimum(0.9);
    }

    {
        fHM->CreateCanvas("rich_mcbm_richRingsTimeLog2", "rich_mcbm_richRingsTimeLog2", 1200, 600);
        DrawH1({fHM->H1("fhRichDigisTimeLogZoom2"), fHM->H1("fhRichRingsTimeLogZoom2")}, {"Digis", "Rings"}, kLinear, kLog, true, 0.87, 0.75, 0.99, 0.99);
        gPad->SetLeftMargin(0.1);
        gPad->SetRightMargin(0.1);
        fHM->H1("fhRichDigisTimeLogZoom2")->GetYaxis()->SetTitleOffset(0.7);
        fHM->H1("fhRichDigisTimeLogZoom2")->SetMinimum(0.9);
    }

    
    
    {
        TCanvas* c = fHM->CreateCanvas("rich_ToT","rich_ToT", 1200 , 600);
        c->Divide(2,1);
        c->cd(1);
        DrawH1(fHM->H1("fhRichDigisToT"));
        c->cd(2);
        DrawH1(fHM->H1("fhRichHitToT"));
    }

    {
        fHM->CreateCanvas("ToF_XYZ","ToF_XYZ", 1200 , 1200);
        fHM->H3("fhTofXYZ")->Draw();
    }
    
    
    {
        TCanvas* c = fHM->CreateCanvas("rich_mcbm_rings","rich_mcbm_rings", 1200 , 600);
        c->Divide(2,1);
        c->cd(1);
        DrawH1(fHM->H1("fhRichRingRadius"));
        c->cd(2);
        DrawH1(fHM->H1("fhNofHitsInRing"));
    }
    
    {
        fHM->CreateCanvas("TofHitsZ","TofHitsZ", 1200 , 1200);
        fHM->H1("fhTofHitsZ")->Draw();
    }
    
    {
        fHM->CreateCanvas("TofTracksPerEvent","TofTracksPerEvent", 1200 , 1200);
        fHM->H1("fhTofTracksPerEvent")->Draw();
    }
    
    {
        TCanvas* c = fHM->CreateCanvas("TofRichRingX","TofRichRingX", 1200 , 1200);
        c->Divide(2,1);
        c->cd(1);
        DrawH1(fHM->H1("fhTofRichRingX"));
        c->cd(2);
        DrawH1(fHM->H1("fhTofRichRingY"));
    }
    
    {
        fHM->CreateCanvas("TofRichRingXZ","TofRichRingXZ", 1200 , 1200);

        DrawH2(fHM->H2("fhTofRichRingXZ"));
    }
    
    {
        fHM->CreateCanvas("TofHitsXZ","TofHitsXZ", 1200 , 1200);

        DrawH2(fHM->H2("fhTofHitsXZ"));
    }
    
    {
        fHM->CreateCanvas("TofTrackRichRingXY","TofTrackRichRingXY", 1200 , 1200);

        DrawH2(fHM->H2("fhTofTrackRichRingXY"));
    }

    {
        fHM->CreateCanvas("TofClosTrackRichRingXY","TofClosTrackRichRingXY", 1200 , 1200);

        DrawH2(fHM->H2("fhTofClosTrackRichRingXY"));
    }
    
    {
        TCanvas* c = fHM->CreateCanvas("TrackRingDist","TrackRingDist", 1200 , 800);
        c->Divide(2,1);
        c->cd(1);
        DrawH1(fHM->H1("fhTrackRingDistance"));
        c->cd(2);
        DrawH2(fHM->H2("fhTrackRingDistanceVSRingradius"));
    }
    
    {
        TCanvas* c = fHM->CreateCanvas("ClosTrackRingXY","ClosTrackRingXY", 1200 , 800);
        c->Divide(2,1);
        c->cd(1);
        DrawH2(fHM->H2("fhClosTrackRingX"));
        c->cd(2);
        DrawH2(fHM->H2("fhClosTrackRingY"));
    }

    {
        TCanvas* c = fHM->CreateCanvas("TofTracksXY","TofTracksXY", 1200 , 800);
        c->Divide(2,1);
        c->cd(1);
        DrawH2(fHM->H2("fhTofTracksXY"));
        c->cd(2);
        DrawH2(fHM->H2("fhTofTracksXYRICH"));
    }
    
    {
        fHM->CreateCanvas("TofBeta","TofBeta", 800 , 800);
        gStyle->SetOptStat(0);
        fHM->H1("fhTofBetaAll")->Draw("HIST");
        fHM->H1("fhTofBetaTracksWithHitsNoRing")->SetLineColorAlpha(kGreen, 1);
        fHM->H1("fhTofBetaTracksWithHitsNoRing")->Draw("HIST SAME");
        
        //fHM->H1("fhTofBetaTracksNoRing")->SetLineColorAlpha(8, 1);
        //fHM->H1("fhTofBetaTracksNoRing")->Draw("HIST SAME");
        
        //fHM->H1("fhTofBetaTracksWithHits")->SetLineColorAlpha(44, 1);
        //fHM->H1("fhTofBetaTracksWithHits")->Draw("HIST SAME");
        
        fHM->H1("fhTofBetaTrackswithClosestRingInRange")->SetLineColorAlpha(44, 1);
        fHM->H1("fhTofBetaTrackswithClosestRingInRange")->Draw("HIST SAME");
        
        fHM->H1("fhTofBetaRing")->SetLineColorAlpha(kRed, 1);
        fHM->H1("fhTofBetaRing")->Draw("HIST SAME");      
        
        auto legend = new TLegend(0.1,0.75,0.4,0.9);
        legend->AddEntry(fHM->H1("fhTofBetaTracksWithHitsNoRing"),"Tracks with RichHits and no Ring","l");
        //legend->AddEntry(fHM->H1("fhTofBetaTracksWithHits"),"Tracks with RichHits","l");
        //legend->AddEntry(fHM->H1("fhTofBetaTracksNoRing"),"Tracks with no Ring","l");
        legend->AddEntry(fHM->H1("fhTofBetaTrackswithClosestRingInRange"),"Tracks with clos. Ring in +20% Radius","l");
        legend->AddEntry(fHM->H1("fhTofBetaRing"),"Tracks in good ring","l");
        if (fRestrictToAcc) {
            legend->AddEntry(fHM->H1("fhTofBetaAll"),"All Tracks in mRICH Acc.","l");
        } else if (fRestrictToFullAcc) {
            legend->AddEntry(fHM->H1("fhTofBetaAll"),"All Tracks in full mRICH Acc.","l");
        } else {
            legend->AddEntry(fHM->H1("fhTofBetaAll"),"All Tracks","l");
        }
        legend->Draw();
    }
    
    {
        TCanvas* c = fHM->CreateCanvas("TofBetaLog","TofBetaLog", 800 , 800);
        c->SetLogy();
        fHM->H1("fhTofBetaAll")->Draw("HIST");
        fHM->H1("fhTofBetaTracksWithHitsNoRing")->SetLineColorAlpha(kGreen, 1);
        fHM->H1("fhTofBetaTracksWithHitsNoRing")->Draw("HIST SAME");
        
        //fHM->H1("fhTofBetaTracksNoRing")->SetLineColorAlpha(8, 1);
        //fHM->H1("fhTofBetaTracksNoRing")->Draw("HIST SAME");
        
        //fHM->H1("fhTofBetaTracksWithHits")->SetLineColorAlpha(44, 1);
        //fHM->H1("fhTofBetaTracksWithHits")->Draw("HIST SAME");
        
        fHM->H1("fhTofBetaTrackswithClosestRingInRange")->SetLineColorAlpha(44, 1);
        fHM->H1("fhTofBetaTrackswithClosestRingInRange")->Draw("HIST SAME");
        
        fHM->H1("fhTofBetaRing")->SetLineColorAlpha(kRed, 1);
        fHM->H1("fhTofBetaRing")->Draw("HIST SAME");      
        
        auto legend = new TLegend(0.1,0.75,0.4,0.9);
        legend->AddEntry(fHM->H1("fhTofBetaTracksWithHitsNoRing"),"Tracks with RichHits and no Ring","l");
        //legend->AddEntry(fHM->H1("fhTofBetaTracksWithHits"),"Tracks with RichHits","l");
        //legend->AddEntry(fHM->H1("fhTofBetaTracksNoRing"),"Tracks with no Ring","l");
        legend->AddEntry(fHM->H1("fhTofBetaTrackswithClosestRingInRange"),"Tracks with clos. Ring in +20% Radius","l");
        legend->AddEntry(fHM->H1("fhTofBetaRing"),"Tracks in good ring","l");
        if (fRestrictToAcc) {
            legend->AddEntry(fHM->H1("fhTofBetaAll"),"All Tracks in mRICH Acc.","l");
        } else if (fRestrictToFullAcc) {
            legend->AddEntry(fHM->H1("fhTofBetaAll"),"All Tracks in full mRICH Acc.","l");
        } else {
            legend->AddEntry(fHM->H1("fhTofBetaAll"),"All Tracks","l");
        }
        legend->Draw();
    }
    
    {
        fHM->CreateCanvas("TofBetaVsRadius","TofBetaVsRadius", 800 , 800);
        DrawH2(fHM->H2("fhTofBetaVsRadius"));
    }
    
    {
        fHM->CreateCanvas("TofBetaRingDist","TofBetaRingDist", 800 , 800);
        DrawH2(fHM->H2("fhTofBetaRingDist"));
    }
    
    {
        fHM->CreateCanvas("TofBetaAllFullAcc","TofBetaAllFullAcc", 800 , 800);
        DrawH1(fHM->H1("fhTofBetaAllFullAcc"));
    }
    
    {
        TCanvas* c = fHM->CreateCanvas("TofBetaAllFullAccLog","TofBetaAllFullAccLog", 800 , 800);
        c->SetLogy();
        DrawH1(fHM->H1("fhTofBetaAllFullAcc"));
    }
    
    {
        TCanvas* c = fHM->CreateCanvas("RingAnalysis","RingAnalysis", 1000 , 1000);
        //c->SetLogy();
        c->Divide(2,2);
        c->cd(1);
        DrawH1(fHM->H1("fhRingDeltaTime"));
        c->cd(2);
        DrawH1(fHM->H1("fhRingToTs"));
        c->cd(3);
        DrawH1({fHM->H1("fhRingLE"), fHM->H1("fhRingNoClTrackLE"), fHM->H1("fhRingClTrackFarAwayLE"), fHM->H1("fhGoodRingLE")},
                {"all Rings", "Rings w/o closest Track","Rings w/ closest Track >5cm", "Good Rings"}, kLinear, kLinear, true, 0.70, 0.75, 0.95, 0.99);
        c->cd(4);
        DrawH2(fHM->H2("fhRingLEvsToT"));
    }    
    
    {
        TCanvas* c = fHM->CreateCanvas("InnerRingAnalysis","InnerRingAnalysis", 1000 , 1200);
        //c->SetLogy();
        c->Divide(2,3);
        c->cd(1);
        DrawH1(fHM->H1("fhInnerRingDeltaTime"));
        c->cd(2);
        DrawH1(fHM->H1("fhInnerRingToTs"));
        c->cd(3);
        DrawH1({fHM->H1("fhInnerRingLE"), fHM->H1("fhInnerRingNoClTrackLE"), fHM->H1("fhInnerRingClTrackFarAwayLE"), fHM->H1("fhInnerGoodRingLE")},
                {"all Rings", "Rings w/o closest Track","Rings w/ closest Track >5cm", "Good Rings"}, kLinear, kLinear, true, 0.70, 0.75, 0.95, 0.99);
        c->cd(4);
        DrawH1(fHM->H1("fhInnerRingFlag"));
        c->cd(5);
        DrawH1(fHM->H1("fhNofInnerHits"));
    }    
    
    {
        fHM->CreateCanvas("DiRICHInRegion","DiRICHInRegion", 800 , 800);
        //c->SetLogy();
        DrawH1(fHM->H1("fhDiRICHsInRegion"));
    }    
}

void CbmRichMCbmQaReal::DrawRing(
                              CbmRichRing* ring
                                )
{
    std::vector<CbmTofTracklet*> track;
    this->DrawRing(ring,track);
}

void CbmRichMCbmQaReal::DrawRing(
                              CbmRichRing* ring,
                              std::vector<CbmTofTracklet*> track,
                              bool full
                                )
{
    std::cout<<"#!#DRAW!!!"<<std::endl;
    if (fNofDrawnRings > 20) return;
    fNofDrawnRings++;
    stringstream ss;
    ss << "Event" << fNofDrawnRings;
    //fNofDrawnRings++;
    TCanvas *c = nullptr;
    if (full == true) {
        c = fHM->CreateCanvas(ss.str().c_str(), ss.str().c_str(), 800, 800);
    } else {
        c = fHM->CreateCanvas(ss.str().c_str(), ss.str().c_str(), 500, 500);
    }
    c->SetGrid(true, true);
    TH2D* pad = nullptr;
    if (full == true){
        pad = new TH2D(ss.str().c_str(), (ss.str() + ";X [cm];Y [cm]").c_str(), 1, -15., 10., 1, -5., 20);
    } else {
        pad = new TH2D(ss.str().c_str(), (ss.str() + ";X [cm];Y [cm]").c_str(), 1, -5., 5., 1, -5., 5);
    }
    
    pad->SetStats(false);
    pad->Draw();
    
    if (full == true){
        //rough Drawing of RichDetectorAcceptance
        TLine *line0 = new TLine(-6.25,8,-6.25,15.9);
        line0->Draw();
        TLine *line1 = new TLine(-6.25,15.9,-1.05,15.9);
        line1->Draw();
        TLine *line2 = new TLine(-1.05,15.9,-1.05,13.2);
        line2->Draw();
        TLine *line3 = new TLine(-1.05,13.2,+4.25,13.2);
        line3->Draw();    
        TLine *line4 = new TLine(4.25,13.2,+4.25,8);
        line4->Draw();
        TLine *line5 = new TLine(4.25,8,-6.25,8);
        line5->Draw();  
    }

    // find min and max x and y positions of the hits
    // in order to shift drawing
    double xCur = 0.;
    double yCur = 0.;
    
    if (full == false){
        double xmin = 99999., xmax = -99999., ymin = 99999., ymax = -99999.;
        for (int i = 0; i < ring->GetNofHits(); i++){
            Int_t hitInd = ring->GetHit(i);
            CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
            if (nullptr == hit) continue;
            if (xmin > hit->GetX()) xmin = hit->GetX();
            if (xmax < hit->GetX()) xmax = hit->GetX();
            if (ymin > hit->GetY()) ymin = hit->GetY();
            if (ymax < hit->GetY()) ymax = hit->GetY();
        }
        xCur = (xmin + xmax) / 2.;
        yCur = (ymin + ymax) / 2.;
    }

    //Draw circle and center
    TEllipse* circle = new TEllipse(ring->GetCenterX() - xCur, ring->GetCenterY() - yCur, ring->GetRadius());
    circle->SetFillStyle(0);
    circle->SetLineWidth(3);
    circle->Draw();
    TEllipse* center = new TEllipse(ring->GetCenterX() - xCur, ring->GetCenterY() - yCur, .1);
    center->Draw();

    
    double hitZ = 0;
    uint nofDrawHits = 0;
    
    // Draw hits
    for (int i = 0; i < ring->GetNofHits(); i++){
        Int_t hitInd = ring->GetHit(i);
        CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
        if (nullptr == hit) continue;
        TEllipse* hitDr = new TEllipse(hit->GetX() - xCur, hit->GetY() - yCur, .25);
        if (doToT(hit)) { // Good ToT selection
          hitDr->SetFillColor(kRed);
        } else {
          hitDr->SetFillColor(kBlue);
        }
        hitZ += hit->GetZ();
        nofDrawHits++;
        hitDr->Draw();
    }
    hitZ /= nofDrawHits;
    
    int Tc = 0;

    //Draw Tracks
    if (track.size() > 0 ) {
        stringstream ss3;
        std::string sTrackLabel;
        double dist = -99999.999;
        for (auto trackInd : track){
            TEllipse* hitDr = new TEllipse(trackInd->GetFitX(hitZ) - xCur, trackInd->GetFitY(hitZ) - yCur, .25);
            hitDr->SetFillColor(kGreen);
            hitDr->Draw();
            //ss3 << "\\beta : "<< getBeta(trackInd); //inVel <<"x10^7 m/s";
            if (trackInd->GetFitX(hitZ) < 30.0) {// Track on correct side of ToF
                Tc++;
                double tmp_dist = std::sqrt(trackInd->GetFitX(hitZ)*trackInd->GetFitX(hitZ) + trackInd->GetFitY(hitZ)*trackInd->GetFitY(hitZ));
                if (dist < -99999.0){
                    dist = tmp_dist;
                    sTrackLabel = "\\beta : " + std::to_string((double)getBeta(trackInd));
                } else if (dist > tmp_dist) {
                    dist = tmp_dist;
                    sTrackLabel = "\\beta : " + std::to_string((double)getBeta(trackInd));
                }
            }
        }
        ss3 << sTrackLabel;
        TLatex* latex1 = new TLatex(-4., 0.5, ss3.str().c_str());
        latex1->Draw();
    } else {
        std::cout<<"No Tracks to Draw."<<std::endl;
    }

    
    //Draw information
    stringstream ss2;
    if (full == false){
        ss2 << "(x,y,r,n)=(" << setprecision(3) << ring->GetCenterX() << ", " << ring->GetCenterY()<< ", " 
            << ring->GetRadius() << ", " << ring->GetNofHits()<<")";
    } else {
        ss2 << "(x,y,r,n,T,Tc)=(" << setprecision(3) << ring->GetCenterX() << ", " << ring->GetCenterY()<< ", "
            << ring->GetRadius() << ", " << ring->GetNofHits()<<", " << track.size() <<", " << Tc <<")";
    }
    TLatex* latex = nullptr;
    if (full == true){
        latex = new TLatex(ring->GetCenterX()-13., ring->GetCenterY()+5., ss2.str().c_str());
    } else {
        latex = new TLatex(-4., 4., ss2.str().c_str());
    }
    latex->Draw();
}

void CbmRichMCbmQaReal::DrawRichTofEv(
                               const std::vector<int> richHitIndx,
                               const std::vector<int> tofTrackIndx)
{
    if (richHitIndx.size() < 1 || tofTrackIndx.size() < 1 ) return;
    if (fNofDrawnRichTofEv > 30) return;
    fNofDrawnRichTofEv++;
    stringstream ss;
    ss << "TREv/TofRichEvent" << fNofDrawnRichTofEv;
    TCanvas *c = nullptr;
    c = fHM->CreateCanvas(ss.str().c_str(), ss.str().c_str(), 800, 800);
    c->SetGrid(true, true);
    
    TH2D* pad = new TH2D(ss.str().c_str(), (ss.str() + ";X [cm];Y [cm]").c_str(), 1, -15., 10., 1, -5., 20);
    
    pad->SetStats(false);
    pad->Draw();
    
    //rough Drawing of RichDetectorAcceptance
    TLine *line0 = new TLine(-6.25,8,-6.25,15.9);
    line0->Draw();
    TLine *line1 = new TLine(-6.25,15.9,-1.05,15.9);
    line1->Draw();
    TLine *line2 = new TLine(-1.05,15.9,-1.05,13.2);
    line2->Draw();
    TLine *line3 = new TLine(-1.05,13.2,+4.25,13.2);
    line3->Draw();    
    TLine *line4 = new TLine(4.25,13.2,+4.25,8);
    line4->Draw();
    TLine *line5 = new TLine(4.25,8,-6.25,8);
    line5->Draw();  
    
    int nofDrawHits = 0;
    double hitZ = 0.;
    
    for (unsigned int i = 0; i < richHitIndx.size(); i++){
        CbmRichHit* hit = (CbmRichHit*) fRichHits->At(richHitIndx[i]);
        if (nullptr == hit) continue;
        TEllipse* hitDr = new TEllipse(hit->GetX(), hit->GetY(), .25);
        if (doToT(hit)) { // Good ToT selection
          hitDr->SetFillColor(kRed);
        } else {
          hitDr->SetFillColor(kBlue);
        }
        hitZ += hit->GetZ();
        nofDrawHits++;
        hitDr->Draw();
    }
    if ( nofDrawHits != 0) {
        hitZ /= nofDrawHits;
    }else {
        hitZ = RichZPos;
    }
    
    stringstream ss2;

    for (unsigned int i = 0; i < tofTrackIndx.size(); i++){
        CbmTofTracklet* track = (CbmTofTracklet*) fTofTracks->At(tofTrackIndx[i]);
        TEllipse* trDr = new TEllipse(track->GetFitX(hitZ), track->GetFitY(hitZ), .25);
        trDr->SetFillColor(kGreen);
        trDr->Draw();
        
        ss2<< "\\beta: "<<getBeta(track);
    }
    
    TLatex* latex = new TLatex(-4., 4., ss2.str().c_str());
    
    latex->Draw();

}



void CbmRichMCbmQaReal::DrawEvent(CbmEvent *ev, std::vector<int> &ringIndx, bool full=true){
 
   
    double pmtWidth  = 5.20;
    double pmtHeight = 5.20;
    double pmtGap    = 0.10;
    
    double left   = 0.0;
    //double right  = 0.0;
    double top    = 0.0;
    //double bottom = 0.0;
    if (full == true){
        left   = -16.85;
        //right  =   4.25;
        top    =  23.8;
        //bottom = -23.8;
    }
    
    
    if (fNofDrawnEvents > 50) return;
    if ((ev->GetNofData(ECbmDataType::kRichHit) <= 4) /*|| ringIndx.size() == 0*/) return;
    fNofDrawnEvents++;
    stringstream ss;
    ss << "Ev/CbmEvent" << fNofDrawnEvents;
    TCanvas *c = nullptr;
    if (full == true) {
        c = fHM->CreateCanvas(ss.str().c_str(), ss.str().c_str(), 600, 1000);
    } else {
        c = fHM->CreateCanvas(ss.str().c_str(), ss.str().c_str(), 800, 800);
    }
    c->SetGrid(true, true);
    TH2D* pad = nullptr;
    if (full == true){
        pad = new TH2D(ss.str().c_str(), (ss.str() + ";X [cm];Y [cm]").c_str(), 1, -18., 5., 1, -26., 26.);
    } else {
        pad = new TH2D(ss.str().c_str(), (ss.str() + ";X [cm];Y [cm]").c_str(), 1, -15., 10., 1, -5., 20);
    }
    
    pad->SetStats(false);
    pad->Draw();
    
    if (full == true){
        for(unsigned int x=0;x<4;++x){
            for(unsigned int y=0;y<9;++y){
                double pmtLeft = left+(pmtWidth+pmtGap)*x;
                double pmtTop  = top-(pmtHeight+pmtGap)*y;
                TBox *box = new TBox( pmtLeft,
                                      pmtTop,
                                      pmtLeft + pmtWidth,
                                      pmtTop-pmtHeight
                                    );
                //box->SetFillColorAlpha(8,0.2);
                box->Draw();
                
                pmtLeft += 0.175;
                pmtTop  -= 0.175;
                for (unsigned int pX=0; pX<8; ++pX){
                    for (unsigned int pY=0; pY<8; ++pY){
                        double xStart = 0.0;
                        double xEnd   = 0.0;
                        double yStart = 0.0;
                        double yEnd   = 0.0;
                        if (pX == 0){
                            xStart = pmtLeft;
                            xEnd   = pmtLeft + 0.625;
                        } else if (pX < 7){
                            xStart = pmtLeft + 0.625 + 0.6*(pX-1);
                            xEnd   = pmtLeft + 0.625 + 0.6*(pX);
                        } else if (pX == 7){
                            xStart = pmtLeft + 0.625 + 0.6*6;
                            xEnd   = pmtLeft + 0.625*2 + 0.6*6;
                        }
                        
                        if (pY == 0){
                            yStart = pmtTop;
                            yEnd   = pmtTop - 0.625;
                        } else if (pY < 7){
                            yStart = pmtTop - 0.625 - 0.6*(pY-1);
                            yEnd   = pmtTop - 0.625 - 0.6*(pY);
                        } else if (pY == 7){
                            yStart = pmtTop - 0.625 - 0.6*6;
                            yEnd   = pmtTop - 0.625*2 - 0.6*6;
                        }

                        TBox *box1 = new TBox( xStart,
                                               yStart,
                                               xEnd,
                                               yEnd
                                             );
                        box1->SetLineWidth(1.);
                        //box1->SetFillColorAlpha(30.,0.1);
                        //box1->SetLineColorAlpha(30.,0.5);
                        
                        box1->Draw("l");
                    }
                }
            }
        }
    }

    if (full == true){
        //rough Drawing of RichDetectorAcceptance
/*        TLine *line0 = new TLine( left ,    top, right,    top);
        line0->Draw();
        TLine *line1 = new TLine( right,    top, right, bottom);
        line1->Draw();
        TLine *line2 = new TLine( right, bottom,  left, bottom);
        line2->Draw();
        TLine *line3 = new TLine( left , bottom,  left,    top);
        line3->Draw(); */   
    }

    // find min and max x and y positions of the hits
    // in order to shift drawing


    double hitZ = 0;
    uint nofDrawHits = 0;
    
    for (int j=0;j<ev->GetNofData(ECbmDataType::kRichHit);j++){
        auto iRichHit = ev->GetIndex(ECbmDataType::kRichHit, j);
        CbmRichHit* hit = static_cast<CbmRichHit*>(fRichHits->At(iRichHit));
        if (nullptr == hit) continue;
        TEllipse* hitDr = new TEllipse(hit->GetX(), hit->GetY(), .25);
        if (doToT(hit)) { // Good ToT selection
          hitDr->SetFillColor(kCyan);
        } else {
          hitDr->SetFillColor(kBlue);
        }
        hitZ += hit->GetZ();
        nofDrawHits++;
        hitDr->Draw();
    }
    
    //Draw circle and center
    for (unsigned int rings=0; rings < ringIndx.size();rings++) {
        CbmRichRing *ring = static_cast<CbmRichRing *>(fRichRings->At(ringIndx[rings]));
        
        TEllipse* circle = new TEllipse(ring->GetCenterX(), ring->GetCenterY(), ring->GetRadius());
        circle->SetFillStyle(0);
        circle->SetLineWidth(3);
        circle->Draw();
        TEllipse* center = new TEllipse(ring->GetCenterX(), ring->GetCenterY(), .1);
        center->Draw();

        // Draw hits
        for (int i = 0; i < ring->GetNofHits(); i++){
            Int_t hitInd = ring->GetHit(i);
            CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
            if (nullptr == hit) continue;
            TEllipse* hitDr = new TEllipse(hit->GetX(), hit->GetY(), .125);
            if (doToT(hit)) { // Good ToT selection
                hitDr->SetFillColor(kMagenta);
            } else {
                hitDr->SetFillColor(kRed);
            }
            //hitZ += hit->GetZ();
            //nofDrawHits++;
            hitDr->Draw();
        }
    }
    hitZ /= nofDrawHits;

    //Draw Tracks
    auto nofTofTracks = ev->GetNofData(ECbmDataType::kTofTrack);
    for (int j=0;j<nofTofTracks;j++){
        auto iTofTrack = ev->GetIndex(ECbmDataType::kTofTrack, j);
        CbmTofTracklet *track = static_cast<CbmTofTracklet *>(fTofTracks->At(iTofTrack));
    
        TEllipse* hitDr = new TEllipse(track->GetFitX(hitZ), track->GetFitY(hitZ), .25);
        hitDr->SetFillColor(kGreen);
        hitDr->Draw();
    }
    
}



void CbmRichMCbmQaReal::Finish()
{   
    //std::cout<<"Tracks:  "<< fTofTracks->GetEntriesFast() <<std::endl;
    std::cout<<"Drawing Hists...";
    DrawHist();
    std::cout<<"DONE!"<<std::endl;
    
    if (this->fDoDrawCanvas){
        fHM->SaveCanvasToImage(fOutputDir,"png");
        std::cout<<"Canvas saved to Images!"<<std::endl;
    }
    
    if (this->fDoWriteHistToFile){
        TDirectory * oldir = gDirectory;
        std::string s = fOutputDir + "/RecoHists.root";
        TFile *outFile = new TFile(s.c_str(),"RECREATE");
        if (outFile->IsOpen()) {
            fHM->WriteToFile();
            std::cout<<"Written to Root-file \""<< s << "\"  ...";
            outFile->Close();
            std::cout<<"Done!"<<std::endl;
        }
        gDirectory->cd( oldir->GetPath() );
    }
}


void CbmRichMCbmQaReal::DrawFromFile(
        const string& fileName,
        const string& outputDir)
{
    fOutputDir = outputDir;

    if (fHM != nullptr) delete fHM;

    fHM = new CbmHistManager();
    TFile* file = new TFile(fileName.c_str());
    fHM->ReadFromFile(file);
    DrawHist();

    fHM->SaveCanvasToImage(fOutputDir);
}

bool CbmRichMCbmQaReal::isAccmRICH(CbmTofTracklet *track){
    //check if Track is in mRICH acceptance
    
    double x = track->GetFitX(RichZPos);
    double y = track->GetFitY(RichZPos);
    bool inside = false;
    if (!fRestrictToAcc) return true;
    if ( x >= -6.25 && x <= -1.05 ){
        // left part of mRICH
        if ( y >8 && y < 15.9) {
            inside = true;
        }
    } else if (x>-1.05 && x< 4.25){
        //right part
        if ( y >8 && y < 13.2) {
            inside = true;
        }
    }
    
    return inside;
}

bool CbmRichMCbmQaReal::doToT(CbmRichHit * hit){
    bool check = false;
    if ((hit->GetToT() > 23.7) && (hit->GetToT() < 30.0)) check = true;
    
    return check;
}

Double_t CbmRichMCbmQaReal::getBeta(CbmTofTracklet *track){
    Double_t inVel = 1e7/(track->GetTt()); // in m/s
    Double_t beta  = inVel/TMath::C();
    
    return beta;
}

bool CbmRichMCbmQaReal::RestrictToFullAcc(CbmTofTracklet *track){
    //check if Track is in mRICH acceptance
    
    double x = track->GetFitX(RichZPos);
    double y = track->GetFitY(RichZPos);
    return this->RestrictToFullAcc(x, y);
}

bool CbmRichMCbmQaReal::RestrictToFullAcc(TVector3 &pos){
    Double_t x = pos.X();
    Double_t y = pos.Y();
    
    return this->RestrictToFullAcc(x, y);
}

bool CbmRichMCbmQaReal::RestrictToFullAcc(Double_t x, Double_t y)
{   //check if Track is in mRICH acceptance
    if (fRestrictToFullAcc == false) return true;
    bool inside = false;
    if ( x >= -16.85 && x <= 4.25 ){ //TODO:check values
        // left part of mRICH
        if ( y >=-23.8 && y <= 23.8) {
            inside = true;
        }
    }
    
    return inside;
}

void CbmRichMCbmQaReal::analyseRing(CbmRichRing *ring, CbmEvent *ev,std::pair<int, double> &clTrack){
 
    std::cout<<"analyse a Ring"<<std::endl;
    
    Double_t meanTime = 0.;
    unsigned int hitCnt = 0;
    Double_t minRHit2 = std::numeric_limits<Double_t>::max();
    for (int i = 0; i < ring->GetNofHits(); i++){
       Int_t hitInd = ring->GetHit(i);
       CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
       if (nullptr == hit) continue;
       
       meanTime += hit->GetTime();
       hitCnt++; 
       
       const Float_t diffX = hit->GetX() - ring->GetCenterX();
       const Float_t diffY = hit->GetY() - ring->GetCenterY();
       const Float_t tmpHitRadius2 = (diffX*diffX+diffY*diffY);
       
       if (tmpHitRadius2 < minRHit2) {
           minRHit2 = tmpHitRadius2;
       }
    }
    meanTime = meanTime / hitCnt;
    
    //std::cout<<"mean: "<<meanTime<<std::endl;
    for (int i = 0; i < ring->GetNofHits(); i++){
       Int_t hitInd = ring->GetHit(i);
       CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
       if (nullptr == hit) continue;
       //std::cout<<"DeltatTime: "<< meanTime - hit->GetTime()<<std::endl;
       fHM->H1("fhRingDeltaTime")->Fill(static_cast<Double_t>(meanTime - hit->GetTime()));
    
       fHM->H1("fhRingToTs")->Fill(hit->GetToT());
       fHM->H1("fhRingLE")->Fill(static_cast<Double_t>(hit->GetTime()-ev->GetStartTime()));
       fHM->H2("fhRingLEvsToT")->Fill(static_cast<Double_t>(hit->GetTime()-ev->GetStartTime()),hit->GetToT());
       //std::vector<int> tmpRingIndx;
       //tmpRingIndx.push_back(ring->GetIndex);
       const Double_t Tdiff_ring = (hit->GetTime()-ev->GetStartTime());
       if ((Tdiff_ring > 20.) && (Tdiff_ring < 30.)){ 
          std::cout<<ev->GetNumber()<<" Address_ring: "<<std::hex<< CbmRichUtil::GetDirichId(hit->GetAddress())<<std::dec<<"  "<< CbmRichUtil::GetDirichChannel(hit->GetAddress()) <<"  "<< hit->GetToT()<<"  "<<ring->GetRadius()<<std::endl;
          //fHM->H1("fhDiRICHsInRegion")->Fill(CbmRichUtil::GetDirichId(hit->GetAddress()));
        }
       
       if (clTrack.first == -1) fHM->H1("fhRingNoClTrackLE")->Fill(static_cast<Double_t>(hit->GetTime()-ev->GetStartTime()));
       if ((clTrack.first >= 0) && !(clTrack.second < 5.)) fHM->H1("fhRingClTrackFarAwayLE")->Fill(static_cast<Double_t>(hit->GetTime()-ev->GetStartTime()));
       if (cutDistance(clTrack) && cutRadius(ring)){ //Good Ring
           fHM->H1("fhGoodRingLE")->Fill(static_cast<Double_t>(hit->GetTime()-ev->GetStartTime()));
       }
    }
    
    int InnerHitCnt = 0;
    int InnerHitCnt_cut = 0;
    for (int j=0;j<ev->GetNofData(ECbmDataType::kRichHit);j++){
       auto iRichHit = ev->GetIndex(ECbmDataType::kRichHit, j);
       CbmRichHit* richHit = static_cast<CbmRichHit*>(fRichHits->At(iRichHit));
       if (nullptr == richHit) continue;
       const Float_t diffX = richHit->GetX() - ring->GetCenterX();
       const Float_t diffY = richHit->GetY() - ring->GetCenterY();
       //select inner Part of Ring
       if ( diffX*diffX+diffY*diffY < minRHit2 ) {
          InnerHitCnt++;
          const Double_t Tdiff_inner = (richHit->GetTime()-ev->GetStartTime());
          if ((Tdiff_inner > 20.) && (Tdiff_inner < 30.)){ 
            InnerHitCnt_cut++;
            if (InnerHitCnt_cut == 1) {DrawRing(ring);} 
            std::cout<<ev->GetNumber()<<" Address_inner: "<<std::hex<< CbmRichUtil::GetDirichId(richHit->GetAddress())<<std::dec<<"  "<< CbmRichUtil::GetDirichChannel(richHit->GetAddress()) <<"  "<< richHit->GetToT()<<"  "<<ring->GetRadius()<<std::endl;
            fHM->H1("fhDiRICHsInRegion")->Fill(CbmRichUtil::GetDirichId(richHit->GetAddress()));
          }
          
          fHM->H1("fhInnerRingDeltaTime")->Fill(static_cast<Double_t>(meanTime - richHit->GetTime()));
          fHM->H1("fhInnerRingToTs")->Fill(richHit->GetToT());
          fHM->H1("fhInnerRingLE")->Fill(static_cast<Double_t>(richHit->GetTime()-ev->GetStartTime()));
          if (clTrack.first == -1) fHM->H1("fhInnerRingNoClTrackLE")->Fill(static_cast<Double_t>(richHit->GetTime()-ev->GetStartTime()));
          if ((clTrack.first >= 0) && !(clTrack.second < 5.)) fHM->H1("fhInnerRingClTrackFarAwayLE")->Fill(static_cast<Double_t>(richHit->GetTime()-ev->GetStartTime()));
          if (cutDistance(clTrack) && cutRadius(ring)){  //Good Ring
             fHM->H1("fhInnerGoodRingLE")->Fill(static_cast<Double_t>(richHit->GetTime()-ev->GetStartTime()));
          }
       }
    }
    if (InnerHitCnt == 0) {
        fHM->H1("fhInnerRingFlag")->Fill(1);
    } else {
        fHM->H1("fhInnerRingFlag")->Fill(0);
    }
    fHM->H1("fhNofInnerHits")->Fill(InnerHitCnt);
}


Bool_t CbmRichMCbmQaReal::cutRadius(CbmRichRing *ring){
    if (ring->GetRadius() > 2. && ring->GetRadius() < 4.2 ) return true;    
    
    return false;
}


Bool_t CbmRichMCbmQaReal::cutDistance(std::pair<int, double> &clTrack){
    if ( (clTrack.first >= 0) && (clTrack.second < 5.)) return true;
    
    return false;
}

ClassImp(CbmRichMCbmQaReal)


#include "CbmPsdMCbmQaReal.h"

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
#include "TSystem.h"
#include <TFile.h>


#include "CbmPsdDigi.h"
#include "CbmPsdMCbmHit.h"
#include "TLatex.h"
#include "CbmDrawHist.h"
#include "CbmTrackMatchNew.h"
#include "CbmMatchRecoToMC.h"
#include "CbmGlobalTrack.h"
#include "CbmTrdTrack.h"
#include "CbmTofHit.h"
#include "CbmTofDigi.h"
#include "CbmStsDigi.h"
#include "CbmEvent.h"
#include "CbmTofTracklet.h"

#include "CbmUtils.h"
#include "CbmHistManager.h"
#include "CbmDigiManager.h"

#include <iostream>
#include <string>
#include <boost/assign/list_of.hpp>
#include <sstream>
#include <cmath>

using namespace std;
using boost::assign::list_of;

#define PsdZPos 348.

CbmPsdMCbmQaReal::CbmPsdMCbmQaReal() :
                        FairTask("CbmPsdMCbmQaReal"),
                        fT0Digis(nullptr),
                        fPsdHits(nullptr),
                        fTofHits(nullptr),
                        fTofTracks(nullptr),
                        fCbmEvent(nullptr),
                        fHM(nullptr),
                        fEntryNum(0),
                        fOutputDir("result")
{

}

InitStatus CbmPsdMCbmQaReal::Init()
{
    cout << "CbmPsdMCbmQaReal::Init" << endl;

    FairRootManager* ioman = FairRootManager::Instance();
    if (nullptr == ioman) { Fatal("CbmPsdMCbmQaReal::Init","RootManager not instantised!"); }

    fDigiMan = CbmDigiManager::Instance();
    fDigiMan->Init();

    if ( ! fDigiMan->IsPresent(ECbmModuleId::kPsd) )
      Fatal("CbmPsdMCbmQaReal::Init", "No Psd Digis!");

    if ( ! fDigiMan->IsPresent(ECbmModuleId::kTof) )
      Fatal("CbmPsdMCbmQaReal::Init", "No Tof Digis!");


    fPsdHits =(TClonesArray*) ioman->GetObject("PsdHit");
    if (nullptr == fPsdHits) { Fatal("CbmPsdMCbmQaReal::Init", "No Psd Hits!");}
      
    fTofHits =(TClonesArray*) ioman->GetObject("TofHit");
    if (nullptr == fTofHits) { Fatal("CbmPsdMCbmQaReal::Init", "No Tof Hits!");}
    
    fTofTracks =(TClonesArray*) ioman->GetObject("TofTracks");
    if (nullptr == fTofTracks) { Fatal("CbmPsdMCbmQaReal::Init", "No Tof Tracks!");}    

//     fT0Digis =(TClonesArray*) ioman->GetObject("CbmT0Digi");
//     if (nullptr == fT0Digis) { Fatal("CbmPsdMCbmQaReal::Init", "No T0 Digis!");}

    fCbmEvent =(TClonesArray*) ioman->GetObject("CbmEvent");
    if (nullptr == fCbmEvent) { Fatal("CbmPsdMCbmQaReal::Init", "No Event!");}
    
    InitHistograms();

    return kSUCCESS;
}

void CbmPsdMCbmQaReal::InitHistograms()
{
    fHM = new CbmHistManager();

    fHM->Create1<TH1D>("fhNofEntries","fhNofEntries; Counts", 1, 0.5, 1.5);
    fHM->Create1<TH1D>("fhNofCbmEvents","fhNofCbmEvents;Counts", 1, 0.5, 1.5);
    
    fHM->Create1<TH1D>("fhHitsInTimeslice","fhHitsInTimeslice;Timeslice;#Hits", 200, 1, 200);

    // nof objects per timeslice
    fHM->Create1<TH1D>("fhNofPsdDigisInTimeslice","fhNofPsdDigisInTimeslice;# PSD digis / timeslice;Counts", 100, 0, 2000);
    fHM->Create1<TH1D>("fhNofPsdHitsInTimeslice","fhNofPsdHitsInTimeslice;# PSD hits / timeslice;Counts", 100, 0, 2000);

    // PSD hits
    fHM->Create2<TH2D>("fhPsdHitPos","fhPsdHitPos;PSD module id [];PSD section id [];Counts", 20, 0, 20, 20, 0, 20);
    fHM->Create1<TH1D>("fhPsdHitsTimeLog", "fhPsdHitsTimeLog;Time [ns];Counts", 400, 0., 0.);


    // PSD digis, the limits of log histograms are set in Exec method
    fHM->Create1<TH1D>("fhPsdDigisTimeLog", "fhNofPsdDigisTimeLog;Time [ns];Counts", 400, 0., 0.);
    fHM->Create1<TH1D>("fhTofDigisTimeLog","fhTofDigisTimeLog;Time [ns];Counts", 400, 0., 0.);
    fHM->Create1<TH1D>("fhStsDigisTimeLog","fhStsDigisTimeLog;Time [ns];Counts", 400, 0., 0.);
    fHM->Create1<TH1D>("fhT0DigisTimeLog","fhT0DigisTimeLog;Time [ns];Counts", 400, 0., 0.);

    //Edep
    fHM->Create1<TH1D>("fhPsdDigisEdep","fhPsdDigisEdep;Edep [adc counts];Counts", 500, 0, 50000);
    fHM->Create1<TH1D>("fhPsdHitEdep","fhPsdHitEdep;Edep [adc counts];Counts", 500, 0, 50000);
    fHM->Create1<TH1D>("fhPsdEdepInEvent","fhPsdEdepInEvent; Edep [adc counts]; Counts", 500, 0, 300000);
   
    //Tof Psd correlation
    fHM->Create2<TH2D>("fhTofTrackMultPsdEdep","fhTofTrackMultPsdEdep;PSD Edep [adc counts];Tof track Mult [];Counts", 500, 0, 400000,  20, 0, 20);
    fHM->Create2<TH2D>("fhTofHitMultPsdEdep","fhTofHitMultPsdEdep;PSD Edep [adc counts];Tof hit Mult [];Counts", 500, 0, 400000,  50, 0, 50);
  
    //Tof Hits  
    fHM->Create3<TH3D>("fhTofXYZ","fhTofXYZ;Tof Hit X [cm];TofHit Z [cm];Tof Hit Y [cm];Counts", 100, -20, 20,  141,  230.,  370.,  100, -20, 20);
    fHM->Create1<TH1D>("fhTofHitsZ","fhTofHitsZ;Z [cm];Counts", 350, -0.5, 349.5);
    fHM->Create2<TH2D>("fhTofHitsXZ","fhTofHitsXZ;Z [cm];X [cm];Counts", 600, -150, 450, 500, -50, 450);

    //Tof Tracks
    fHM->Create1<TH1D>("fhTofTracksPerEvent","fhTofTracksPerEvent;NofTracks/Event;Counts", 20, -5, 25);
    fHM->Create2<TH2D>("fhTofTracksXY","fhTofTracksXY;X[cm];Y[cm];NofTracks/cm^2", 250, -100, 150, 300,-150,150);

    
}


void CbmPsdMCbmQaReal::Exec(Option_t* /*option*/)
{
    fEntryNum++;
    fHM->H1("fhNofEntries")->Fill(1);
    cout << "CbmPsdMCbmQaReal, entry No. " <<  fEntryNum << endl;

    if (fDigiHitsInitialized == false)
    {
         if ((fDigiMan->GetNofDigis(ECbmModuleId::kPsd) > 0) ||
             (fDigiMan->GetNofDigis(ECbmModuleId::kSts) > 0) ||
             (fDigiMan->GetNofDigis(ECbmModuleId::kTof) > 0)
            )
         {

            double minTime = std::numeric_limits<double>::max();
            for (int i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kPsd); i++) {
                const CbmPsdDigi* psdDigi = fDigiMan->Get<CbmPsdDigi>(i);
                // fHM->H1("fhRichDigisToT")->Fill(richDigi->GetToT());
                if (psdDigi->GetTime() < minTime) minTime = psdDigi->GetTime();
            }

            double dT = 40e9;
            fHM->H1("fhPsdHitsTimeLog")->GetXaxis()->SetLimits(minTime, minTime + dT);
            fHM->H1("fhPsdDigisTimeLog")->GetXaxis()->SetLimits(minTime, minTime+dT);      
            fHM->H1("fhTofDigisTimeLog")->GetXaxis()->SetLimits(minTime, minTime+dT);
            fHM->H1("fhStsDigisTimeLog")->GetXaxis()->SetLimits(minTime, minTime+dT);
            fHM->H1("fhT0DigisTimeLog")->GetXaxis()->SetLimits(minTime, minTime+dT);


            fDigiHitsInitialized = true;
        }
    } // if (fDigiHitsInitialized == false)
    
    if (fDigiHitsInitialized == true) 
    {

        int nofPsdDigis = fDigiMan->GetNofDigis(ECbmModuleId::kPsd);
        fHM->H1("fhNofPsdDigisInTimeslice")->Fill(nofPsdDigis);
        for (int i = 0; i < nofPsdDigis; i++) {
            const CbmPsdDigi* digi = fDigiMan->Get<CbmPsdDigi>(i);
            fHM->H1("fhPsdDigisTimeLog")->Fill(digi->GetTime() );
            fHM->H1("fhPsdDigisEdep")->Fill(digi->GetEdep());
        }
  
        int nofTofDigis = fDigiMan->GetNofDigis(ECbmModuleId::kTof);
        for (int i = 0; i < nofTofDigis; i++) {
            const CbmTofDigi* digi = fDigiMan->Get<CbmTofDigi>(i);
            fHM->H1("fhTofDigisTimeLog")->Fill(digi->GetTime() );

        }

        if (fDigiMan->IsPresent(ECbmModuleId::kSts)){
            int nofStsDigis = fDigiMan->GetNofDigis(ECbmModuleId::kSts);
            for (int i = 0; i < nofStsDigis; i++) {
                const CbmStsDigi* digi = fDigiMan->Get<CbmStsDigi>(i);
                fHM->H1("fhStsDigisTimeLog")->Fill(digi->GetTime() );

            }
        }

//     int nofT0Digis = fT0Digis->GetEntries();
//     for (int i = 0; i < nofT0Digis; i++) {
//         CbmDigi* digi = static_cast<CbmDigi*>(fT0Digis->At(i));
//         fHM->H1("fhT0DigisTimeLog")->Fill(digi->GetTime() );
//         fHM->H1("fhT0DigisTimeLogZoom")->Fill(digi->GetTime() );
//         fHM->H1("fhT0DigisTimeLogZoom2")->Fill(digi->GetTime());
//     }
    }

    int nofPsdHits = fPsdHits->GetEntries();
    fHM->H1("fhNofPsdHitsInTimeslice")->Fill(nofPsdHits);
    fHM->H1("fhHitsInTimeslice")->Fill(fEntryNum,nofPsdHits);
    for (int iH = 0; iH < nofPsdHits; iH++) {
        CbmPsdMCbmHit* psdHit = static_cast<CbmPsdMCbmHit*>(fPsdHits->At(iH));
        fHM->H2("fhPsdHitPos")->Fill(psdHit->GetModuleID(), psdHit->GetSectionID());
        fHM->H1("fhPsdHitEdep")->Fill(psdHit->GetEdep());
    }

    //CBMEVENT
    auto fNCbmEvent = fCbmEvent->GetEntriesFast();
    
    for (int i=0;i<fNCbmEvent;i++)
    {
        fHM->H1("fhNofCbmEvents")->Fill(1);
        CbmEvent* ev = static_cast<CbmEvent*>(fCbmEvent->At(i));
        std::vector<int> evPsdHitIndx;

        // Scan Event to find first Digi that triggered.
        std::cout<<"Sts Digis:"<< ev->GetNofData(ECbmDataType::kStsDigi)<<std::endl;
        std::cout<<"Much Digis:"<< ev->GetNofData(ECbmDataType::kMuchDigi)<<std::endl;
        std::cout<<"Tof Digis:"<< ev->GetNofData(ECbmDataType::kTofDigi)<<std::endl;
        std::cout<<"Rich Digis:"<< ev->GetNofData(ECbmDataType::kRichDigi)<<std::endl;
        std::cout<<"Psd Digis:"<< ev->GetNofData(ECbmDataType::kPsdDigi)<<std::endl;

	Int_t nofTofTracksInEvent = ev->GetNofData(ECbmDataType::kTofTrack);
	Int_t nofPsdHitsInEvent = ev->GetNofData(ECbmDataType::kPsdHit);
	Int_t nofTofHitsInEvent = ev->GetNofData(ECbmDataType::kTofHit);
	Double_t PsdEdepInEvent = 0.;
        for (int j = 0; j < nofPsdHitsInEvent; j++)
        {
            auto iPsdHit = ev->GetIndex(ECbmDataType::kPsdHit, j);
            evPsdHitIndx.push_back(iPsdHit);
            CbmPsdMCbmHit* psdHit = static_cast<CbmPsdMCbmHit*>(fPsdHits->At(iPsdHit));
            PsdEdepInEvent += psdHit->GetEdep();
        }
        fHM->H1("fhPsdEdepInEvent")->Fill(PsdEdepInEvent);
        fHM->H2("fhTofTrackMultPsdEdep")->Fill(PsdEdepInEvent, nofTofTracksInEvent);
        fHM->H2("fhTofHitMultPsdEdep")->Fill(PsdEdepInEvent, nofTofHitsInEvent);

        for (int j=0;j<nofTofHitsInEvent;j++)
        {
            auto iTofHit = ev->GetIndex(ECbmDataType::kTofHit, j);
            CbmTofHit* tofHit = static_cast<CbmTofHit*>(fTofHits->At(iTofHit));
            //if (tofHit->GetZ() < 2.) continue; // Cut T0 away!
            
            fHM->H1("fhTofHitsZ")->Fill(tofHit->GetZ());
            fHM->H1("fhTofHitsXZ")->Fill(tofHit->GetZ(),tofHit->GetX());
            fHM->H3("fhTofXYZ")->Fill(tofHit->GetX(),tofHit->GetZ(),tofHit->GetY());
        }

        fHM->H1("fhTofTracksPerEvent")->Fill(nofTofTracksInEvent);
        for (int j = 0; j < nofTofTracksInEvent; j++) 
        {
            auto iTofTrack = ev->GetIndex(ECbmDataType::kTofTrack, j);
            CbmTofTracklet* tTrack = static_cast<CbmTofTracklet*>(fTofTracks->At(iTofTrack));
          
            fHM->H2("fhTofTracksXY")->Fill(tTrack->GetFitX(PsdZPos),tTrack->GetFitY(PsdZPos));
        }

    } //End CbmEvent loop
      
}


void CbmPsdMCbmQaReal::DrawHist()
{
       cout.precision(4);
  
     //SetDefaultDrawStyle();
     //gStyle->SetOptStat(1111);
     double nofEvents = fHM->H1("fhNofCbmEvents")->GetEntries();
     fHM->ScaleByPattern("fh_.*", 1./nofEvents);

    {
        fHM->CreateCanvas("psd_mcbm_fhNofEntries","psd_mcbm_fhNofEntries", 600 , 600);
        DrawH1(fHM->H1("fhNofEntries"));
    }
   
    {
        fHM->CreateCanvas("psd_mcbm_fhNofCbmEvents","psd_mcbm_fhNofCbmEvents", 600 , 600);
        DrawH1(fHM->H1("fhNofCbmEvents"));
    }

    {
        fHM->CreateCanvas("HitsInTimeslice","HitsInTimeslice", 600 , 600);
        DrawH1(fHM->H1("fhHitsInTimeslice"));
    }
    

    {
        TCanvas* c = fHM->CreateCanvas("psd_mcbm_nofObjectsInTimeslice","psd_mcbm_nofObjectsInTimeslice", 1500 , 500);
        c->Divide(2,1);
        c->cd(1);
        DrawH1(fHM->H1("fhNofPsdDigisInTimeslice"), kLinear, kLog);
        c->cd(2);
        DrawH1(fHM->H1("fhNofPsdHitsInTimeslice"), kLinear, kLog);
    }

    {
        fHM->CreateCanvas("psd_mcbm_XY","psd_mcbm_XY", 1200 , 600);
        DrawH2(fHM->H2("fhPsdHitPos"));
    }

    {
        fHM->CreateCanvas("psd_mcbm_DigisTimeLog","psd_mcbm_DigisTimeLog", 1200 , 1200);
        DrawH1({fHM->H1("fhStsDigisTimeLog"), fHM->H1("fhTofDigisTimeLog"), fHM->H1("fhT0DigisTimeLog"), fHM->H1("fhPsdDigisTimeLog")},
                {"STS", "TOF", "T0", "PSD"}, kLinear, kLog, true, 0.87, 0.75, 0.99, 0.99);
        gPad->SetLeftMargin(0.1);
        gPad->SetRightMargin(0.10);
        fHM->H1("fhStsDigisTimeLog")->GetYaxis()->SetTitleOffset(0.7);
        fHM->H1("fhStsDigisTimeLog")->SetMinimum(0.9);
    }

    {
        fHM->CreateCanvas("psd_mcbm_fhPsdDigisEdep","psd_mcbm_fhPsdDigisEdep", 600 , 600);
        DrawH1(fHM->H1("fhPsdDigisEdep"));
    } 

    {
        fHM->CreateCanvas("psd_mcbm_fhPsdHitEdep","psd_mcbm_fhPsdHitEdep", 600 , 600);
        DrawH1(fHM->H1("fhPsdHitEdep"));
    } 

    {
        fHM->CreateCanvas("psd_mcbm_fhPsdEdepInEvent","psd_mcbm_fhPsdEdepInEvent", 600 , 600);
        DrawH1(fHM->H1("fhPsdEdepInEvent"));
    } 

    {
        fHM->CreateCanvas("psd_mcbm_fhTofTrackMultPsdEdep","psd_mcbm_fhTofTrackMultPsdEdep", 1200 , 1200);
        DrawH2(fHM->H2("fhTofTrackMultPsdEdep"));
    }

    {
        fHM->CreateCanvas("psd_mcbm_fhTofHitMultPsdEdep","psd_mcbm_fhTofHitMultPsdEdep", 1200 , 1200);
        DrawH2(fHM->H2("fhTofHitMultPsdEdep"));
    }

    {
        fHM->CreateCanvas("TofHitsZ","TofHitsZ", 1200 , 1200);
        DrawH1(fHM->H1("fhTofHitsZ"));
    }

    {
        fHM->CreateCanvas("TofHitsXZ","TofHitsXZ", 1200 , 1200);
        DrawH2(fHM->H2("fhTofHitsXZ"));
    }

    {
        fHM->CreateCanvas("ToF_XYZ","ToF_XYZ", 1200 , 1200);
        fHM->H3("fhTofXYZ")->Draw();
    }

    {
        TCanvas* c = fHM->CreateCanvas("TofTracksXY","TofTracksXY", 1200 , 800);
        c->Divide(2,1);
        c->cd(1);
        DrawH2(fHM->H2("fhTofTracksXY"));
        //c->cd(2);
        //DrawH2(fHM->H2("fhTofTracksXYRICH"));
    }
 
}




void CbmPsdMCbmQaReal::Finish()
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


void CbmPsdMCbmQaReal::DrawFromFile(
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





ClassImp(CbmPsdMCbmQaReal)


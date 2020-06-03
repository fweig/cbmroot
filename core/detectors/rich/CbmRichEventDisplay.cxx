/**
* \file CbmRichEventDisplay.cxx
*
* \author Supriya Das
* \date 2006
**/
#include "CbmRichEventDisplay.h"

#include "CbmDrawHist.h"        // for DrawH2, SetDefaultDrawStyle
#include "CbmHistManager.h"     // for CbmHistManager
#include "CbmRichHit.h"         // for CbmRichHit
#include "CbmRichPoint.h"       // for CbmRichPoint
#include "CbmRichRing.h"        // for CbmRichRing

#include <FairTask.h>           // for FairTask, InitStatus, kSUCCESS
#include <FairRootManager.h>    // for FairRootManager
#include <FairTrackParam.h>     // for FairTrackParam

#include <TAxis.h>              // for TAxis
#include <TGenericClassInfo.h>  // for TGenericClassInfo
#include <TH2.h>                // for TH2D
#include <TVirtualPad.h>        // for TVirtualPad, gPad
#include <TCanvas.h>            // for TCanvas
#include <TClonesArray.h>       // for TClonesArray
#include <TEllipse.h>           // for TEllipse
#include <TMarker.h>            // for TMarker

#include <iostream>             // for string, stringstream, operator<<, bas...
#include <string>               // for operator==, basic_string, char_traits

using namespace std;

CbmRichEventDisplay::CbmRichEventDisplay():
   FairTask("CbmRichEventDisplay"),
   fRichRings(nullptr),
   fRichHits(nullptr),
   fRichPoints(nullptr),
   fRichMatches(nullptr),
   fRichProjections(nullptr),
   fMcTracks(nullptr),
   fOutputDir(""),
   fHM(nullptr),
   fEventNum(0),
   fDrawRings(true),
   fDrawHits(true),
   fDrawPoints(true),
   fDrawProjections(true)
{
   SetDefaultDrawStyle();
}

CbmRichEventDisplay::~CbmRichEventDisplay()
{
}


InitStatus CbmRichEventDisplay::Init()
{
   FairRootManager* ioman = FairRootManager::Instance();
   if (nullptr == ioman) {Fatal("CbmRichEventDisplay::Init", "RootManager not instantiated!");}

   fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
   if (nullptr == fRichHits) {Fatal("CbmRichEventDisplay::Init", "No RichHit array!");}

   fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
   if (nullptr == fRichRings) { Fatal("CbmRichEventDisplay::Init", "No RichRing array!");}

   fRichPoints = (TClonesArray*) ioman->GetObject("RichPoint");
   if (nullptr == fRichPoints) {Fatal("CbmRichEventDisplay::Init", "No RichPoint array!");}

   fRichMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
   if (nullptr == fRichMatches) {Fatal("CbmRichEventDisplay::Init", "No RichRingMatch array!");}

   fRichProjections = (TClonesArray*) ioman->GetObject("RichProjection");
   if (nullptr == fRichProjections) {Fatal("CbmRichEventDisplay::Init", "No RichProjection array!");}

   fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
   if (nullptr == fMcTracks ) {Fatal("CbmRichEventDisplay::Init", "No MCTrack array!");}

   fHM = new CbmHistManager();

   return kSUCCESS;
}

void CbmRichEventDisplay::Exec(
      Option_t* /*opt*/)
{
   fEventNum++;
   SetDefaultDrawStyle();
   DrawOneEvent();
}

void CbmRichEventDisplay::DrawOneEvent()
{
   stringstream ss;
   ss << "rich_event_display_event_"<< fEventNum;
   TCanvas *c = fHM->CreateCanvas(ss.str().c_str(), ss.str().c_str(), 800, 800);
   c->Divide(1, 2);
   c->cd(1);
   TH2D* padU = new TH2D("padU", ";x [cm];y [cm]", 1, -120., 120., 1, 120., 210);
   DrawH2(padU);
   padU->GetYaxis()->SetTitleOffset(0.75);
   gPad->SetLeftMargin(0.1);
   gPad->SetRightMargin(0.05);
   DrawOnePmtPlane("up");

   c->cd(2);
   TH2D* padD = new TH2D("padD", ";x [cm];y [cm]", 1, -120., 120., 1, -210., -120.);
   DrawH2(padD);
   padD->GetYaxis()->SetTitleOffset(0.75);
   gPad->SetLeftMargin(0.1);
   gPad->SetRightMargin(0.05);
   DrawOnePmtPlane("down");
}

void CbmRichEventDisplay::DrawOnePmtPlane(
      const string& plane)
{
   //Draw Track projections
   if (fDrawProjections) {
      int nofProjections = fRichProjections->GetEntriesFast();
      for (int iP = 0; iP < nofProjections; iP++){
         FairTrackParam* pr = (FairTrackParam*) fRichProjections->At(iP);
         if (nullptr == pr) continue;
         if ( (plane == "up" && pr->GetY() >= 0.) ||
              (plane == "down" && pr->GetY() < 0.) ){
            TMarker* m = new TMarker(pr->GetX(), pr->GetY(), 3.);
            m->SetMarkerSize(0.7);
            m->SetMarkerColor(kGreen+3);
            m->Draw();
         }
      }
   }

   // Draw hits
   if (fDrawHits){
      int nofHits = fRichHits->GetEntriesFast();
      for (int iH = 0; iH < nofHits; iH++){
         CbmRichHit* hit = (CbmRichHit*) fRichHits->At(iH);
         if (nullptr == hit) continue;
         if ( (plane == "up" && hit->GetY() >= 0.) ||
              (plane == "down" && hit->GetY() < 0.) ){

            TEllipse* hitDr = new TEllipse(hit->GetX(), hit->GetY(), 0.6);
            hitDr->SetFillColor(kRed);
            hitDr->SetLineColor(kRed);
            hitDr->Draw();
         }
      }
   }

   // Draw rings
   if (fDrawRings){
      int nofRings = fRichRings->GetEntriesFast();
      for (int iR = 0; iR < nofRings; iR++){
         CbmRichRing* ring = (CbmRichRing*) fRichRings->At(iR);
         if (nullptr == ring) continue;
         if ( (plane == "up" && ring->GetCenterY() >= 0.) ||
              (plane == "down" && ring->GetCenterY() < 0.) ){
            DrawCircle(ring);
         }
      }
   }

   // Draw RICH MC Points
   if (fDrawPoints) {
      int nofPoints = fRichPoints->GetEntriesFast();
      for (int iP = 0; iP < nofPoints; iP++){
         CbmRichPoint* point = (CbmRichPoint*) fRichPoints->At(iP);
         if (nullptr == point) continue;
         if ( (plane == "up" && point->GetY() >= 0.) ||
              (plane == "down" && point->GetY() < 0.) ){
            TEllipse* pointDr = new TEllipse(point->GetX(), point->GetY(), 0.4);
            pointDr->Draw();
         }
      }
   }


}

void CbmRichEventDisplay::DrawCircle(
      CbmRichRing* ring)
{
   TEllipse* circle = new TEllipse(ring->GetCenterX(), ring->GetCenterY(), ring->GetRadius());
   circle->SetFillStyle(0);
   circle->SetLineWidth(2);
   circle->SetLineColor(kBlue);
   circle->Draw();
   TMarker* center = new TMarker(ring->GetCenterX(), ring->GetCenterY(), 2);
   center->SetMarkerColor(kBlue);
   center->SetMarkerSize(0.4);
   center->Draw();
}

void CbmRichEventDisplay::Finish()
{
	fHM->SaveCanvasToImage(fOutputDir);
}

ClassImp(CbmRichEventDisplay)

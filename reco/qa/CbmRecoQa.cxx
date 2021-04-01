/** @file CbmRecoQa.cxx
 ** @author Tim Fauerbach
 ** @since Jun 2019
 **/

#include "CbmRecoQa.h"

#include "CbmHit.h"
#include "CbmLink.h"
#include "CbmMCDataArray.h"
#include "CbmMatch.h"
#include "CbmMuchPixelHit.h"
#include "CbmMuchPoint.h"
#include "CbmMvdHit.h"
#include "CbmMvdPoint.h"
#include "CbmStsDigi.h"
#include "CbmStsHit.h"
#include "CbmStsPoint.h"
#include "CbmTofHit.h"
#include "CbmTofPoint.h"
#include "CbmTrdHit.h"
#include "CbmTrdPoint.h"

#include "FairDetector.h"
#include "FairMCPoint.h"
#include "FairTask.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TList.h"
#include "TROOT.h"
#include "TString.h"
#include "TTask.h"
#include "TVector3.h"

#include <iostream>
#include <string>
#include <vector>

CbmRecoQa* CbmRecoQa::instance = 0;

/**
 ** Costructor
 ** @param decNames Custom Struct { NameOfDetector, { Range for Pull Histogramms, Range for x Residual Histogramms, Range for y Residual Histogramms, Range for t Residual Histogramms}}
 ** @param outName Name of the Outputfile
 ** @param verbose_l Verbosity Level of Output
 **/
CbmRecoQa::CbmRecoQa(
  std::vector<std::pair<std::string, std::array<int, 4>>> decNames,
  std::string outName,
  int verbose_l)
  : FairTask("CbmRecoQa")
  , pullresfile(nullptr)
  , verbosity(verbose_l)
  , detectors(decNames)
  , hists(
      std::vector<std::vector<TH1F*>>(detectors.size(), std::vector<TH1F*>(6)))
  , eventList(nullptr)
  , fManager(nullptr)
  , mcManager(nullptr)
  , outname(outName) {
  if (!instance) { instance = this; };
};

// --- Destructor
CbmRecoQa::~CbmRecoQa() {};

// --- Init and Re-Init
InitStatus CbmRecoQa::ReInit() { return kSUCCESS; };

InitStatus CbmRecoQa::Init() {

  fManager  = FairRootManager::Instance();
  mcManager = (CbmMCDataManager*) fManager->GetObject("MCDataManager");
  eventList = (CbmMCEventList*) fManager->GetObject("MCEventList.");

  if (!mcManager) {
    LOG(error) << "Could not initialise MCDataManager" << std::endl;
    return kERROR;
  }
  if (!fManager) {
    LOG(warn) << "Could not initialise FairRootManager" << std::endl;
    return kERROR;
  }

  for (unsigned int i = 0; i < detectors.size(); i++) {

    // Histogram Setup
    std::string decName = detectors[i].first;
    std::string px      = "Px_" + decName;
    std::string py      = "Py_" + decName;
    std::string pt      = "Pt_" + decName;
    std::string rx      = "x_" + decName;
    std::string ry      = "y_" + decName;
    std::string rt      = "t_" + decName;
    std::string pullx   = decName + " Pull x";
    std::string pully   = decName + " Pull y";
    std::string pullt   = decName + " Pull t";
    std::string resx    = decName + " Residual x";
    std::string resy    = decName + " Residual y";
    std::string rest    = decName + " Residual t";

    TH1F* pullxHist = new TH1F(px.c_str(),
                               pullx.c_str(),
                               100,
                               -1 * detectors[i].second[0],
                               detectors[i].second[0]);
    TH1F* pullyHist = new TH1F(py.c_str(),
                               pully.c_str(),
                               100,
                               -1 * detectors[i].second[0],
                               detectors[i].second[0]);
    TH1F* pulltHist = new TH1F(pt.c_str(),
                               pullt.c_str(),
                               100,
                               -1 * detectors[i].second[0],
                               detectors[i].second[0]);

    TH1F* residualxHist = new TH1F(rx.c_str(),
                                   resx.c_str(),
                                   100,
                                   -1 * detectors[i].second[1],
                                   detectors[i].second[1]);
    TH1F* residualyHist = new TH1F(ry.c_str(),
                                   resy.c_str(),
                                   100,
                                   -1 * detectors[i].second[2],
                                   detectors[i].second[2]);
    TH1F* residualtHist = new TH1F(rt.c_str(),
                                   rest.c_str(),
                                   100,
                                   -1 * detectors[i].second[3],
                                   detectors[i].second[3]);

    //    pullxHist->SetCanExtend(TH1::kAllAxes);
    //    pullyHist->SetCanExtend(TH1::kAllAxes);
    //    pulltHist->SetCanExtend(TH1::kAllAxes);
    //    residualxHist->SetCanExtend(TH1::kAllAxes);
    //    residualyHist->SetCanExtend(TH1::kAllAxes);
    //    residualtHist->SetCanExtend(TH1::kAllAxes);

    pullxHist->GetXaxis()->SetTitle("Pull x");
    pullyHist->GetXaxis()->SetTitle("Pull y");
    pulltHist->GetXaxis()->SetTitle("Pull t");
    residualxHist->GetXaxis()->SetTitle("Residual x");
    residualyHist->GetXaxis()->SetTitle("Residual y");
    residualtHist->GetXaxis()->SetTitle("Residual t");

    hists[i][0] = pullxHist;
    hists[i][1] = pullyHist;
    hists[i][2] = pulltHist;
    hists[i][3] = residualxHist;
    hists[i][4] = residualyHist;
    hists[i][5] = residualtHist;

    if (verbosity > 1)
      LOG(info) << "CbmRecoQa Success Initiliasing Histograms for: " << decName;
  }

  return kSUCCESS;
}

// --- Finish Event
// Record all Data for Detectors defined
void CbmRecoQa::FinishEvent() {

  static int event = 0;
  LOG(info) << "CbmRecoQa for Event " << event++;
  for (unsigned int k = 0; k < detectors.size(); k++) {
    instance->record(detectors[k].first, k);
  }
}

// --- Finish Task
// Save Data in File
void CbmRecoQa::FinishTask() {
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  std::string filename = outname + ".qa.hists.root";
  pullresfile          = new TFile(filename.c_str(), "Recreate");

  for (unsigned int i = 0; i < hists.size(); i++) {
    gDirectory->mkdir(detectors[i].first.c_str());
    gDirectory->cd(detectors[i].first.c_str());
    for (unsigned int k = 0; k < hists[i].size(); k++) {
      if (verbosity > 2)
        LOG(info) << "CbmRecoQa Histogramm " << hists[i][k]->GetName()
                  << " Entries " << hists[i][k]->GetEntries();
      if (hists[i][k]->GetEntries() > 0) hists[i][k]->Write();
    }
    gDirectory->cd("..");
  }

  pullresfile->Close();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

// --- Write Data in Historgrams, depending on detectorw
void CbmRecoQa::record(std::string decName, int decNum) {

  if (verbosity > 1) LOG(info) << "CbmRecoQa Record called for: " << decName;

  // Data aquasition

  TClonesArray* listHits       = nullptr;
  TClonesArray* listHitMatches = nullptr;
  //  TClonesArray* listDigiMatches = nullptr;
  CbmMCDataArray* listPoints = nullptr;
  //  TClonesArray* listClusters = nullptr; (FU) unused
  TClonesArray* listClusterMatches = nullptr;

  if (decName == "sts") {
    listHits       = (TClonesArray*) (fManager->GetObject("StsHit"));
    listHitMatches = (TClonesArray*) (fManager->GetObject("StsHitMatch"));
    //    listClusters= (TClonesArray*)(fManager->GetObject("StsCluster")); (FU) unused
    listClusterMatches =
      (TClonesArray*) (fManager->GetObject("StsClusterMatch"));
    listPoints = mcManager->InitBranch("StsPoint");
    if (listPoints == nullptr) { LOG(warn) << "No StsPoint data!"; }


    if (listHits && listHitMatches) {

      TVector3 hitPos(.0, .0, .0);
      TVector3 mcPos(.0, .0, .0);
      TVector3 hitErr(.0, .0, .0);

      int nEnt = listHits->GetEntriesFast();
      if (verbosity > 2)
        LOG(info) << "CbmRecoQa for " << decName << " found " << nEnt
                  << " Hit Entries";
      for (int j = 0; j < nEnt; j++) {

        float bestWeight  = 0.f;
        float totalWeight = 0.f;
        //        int iMCPoint = -1; (FU) unused
        CbmLink link;

        CbmStsHit* curr_hit     = dynamic_cast<CbmStsHit*>(listHits->At(j));
        CbmStsPoint* curr_point = 0;

        if (listClusterMatches) {

          const CbmMatch* front_match = static_cast<const CbmMatch*>(
            listClusterMatches->At(curr_hit->GetFrontClusterId()));
          const CbmMatch* back_match = static_cast<const CbmMatch*>(
            listClusterMatches->At(curr_hit->GetBackClusterId()));
          CbmMatch curr_match;
          if (verbosity > 3)
            LOG(info) << "Front Match: " << front_match->ToString()
                      << " Back Match: " << back_match->ToString();

          for (int frontlink_c = 0; frontlink_c < front_match->GetNofLinks();
               frontlink_c++) {

            const CbmLink& frontLink = front_match->GetLink(frontlink_c);

            for (int backlink_c = 0; backlink_c < back_match->GetNofLinks();
                 backlink_c++) {

              const CbmLink& backLink = back_match->GetLink(backlink_c);
              if (verbosity > 3)
                LOG(info) << "FrontLink: " << frontLink.ToString()
                          << " BackLink: " << backLink.ToString();
              if (frontLink == backLink) {
                curr_match.AddLink(frontLink);
                curr_match.AddLink(backLink);
              }
            }
          }

          //LOG(info) << curr_match.ToString();

          for (int iLink = 0; iLink < curr_match.GetNofLinks(); iLink++) {
            float tmpweight = curr_match.GetLink(iLink).GetWeight();
            totalWeight += tmpweight;
            //LOG(info) << " Match Link Nr.: " << iLink;
            if (tmpweight > bestWeight) {
              bestWeight = tmpweight;
              //              iMCPoint = curr_match.GetLink(iLink).GetIndex(); (FU) unused
              link = curr_match.GetLink(iLink);
              if (verbosity > 3) LOG(info) << "Found Link for current HIT";
            }
          }

          curr_point = (CbmStsPoint*) (listPoints->Get(
            link.GetFile(), link.GetEntry(), link.GetIndex()));

          if (curr_point == 0) continue;
          double mcTime =
            curr_point->GetTime()
            + eventList->GetEventTime(link.GetEntry(), link.GetFile());

          curr_hit->Position(hitPos);
          curr_hit->PositionError(hitErr);
          if (verbosity > 3) LOG(info) << "Calculated Position Error";

          mcPos.SetX(curr_point->GetX(hitPos.Z()));
          mcPos.SetY(curr_point->GetY(hitPos.Z()));
          mcPos.SetZ(hitPos.Z());
          if (verbosity > 3) LOG(info) << "Calculated MCPos";


          if (hitErr.X() != 0)
            hists[decNum][0]->Fill((hitPos.X() - mcPos.X())
                                   / curr_hit->GetDx());
          if (hitErr.Y() != 0)
            hists[decNum][1]->Fill((hitPos.Y() - mcPos.Y())
                                   / curr_hit->GetDy());
          hists[decNum][2]->Fill((curr_hit->GetTime() - mcTime)
                                 / curr_hit->GetTimeError());

          hists[decNum][3]->Fill((hitPos.X() - mcPos.X()) * 10 * 1000);
          hists[decNum][4]->Fill((hitPos.Y() - mcPos.Y()) * 10 * 1000);
          hists[decNum][5]->Fill(curr_hit->GetTime() - mcTime);
        } else {
          LOG(warn) << "CBMRECOQA WARNING :-- No Sts Cluster Matches found!"
                    << std::endl;
        }
      }
    } else {
      LOG(warn) << "CBMRECOQA WARNING :-- NO Data for Reco QA found! "
                << "Detector: " << decName << std::endl;
    }
  } else if (decName == "mvd") {
    listHits       = (TClonesArray*) (fManager->GetObject("MvdHit"));
    listHitMatches = (TClonesArray*) (fManager->GetObject("MvdHitMatch"));
    //listClusters= (TClonesArray*)(fManager->GetObject("MvdCluster"));
    //listClusterMatches = (TClonesArray*)(fManager->GetObject("MvdClusterMatch"));
    TClonesArray* listMvdPoints =
      (TClonesArray*) (fManager->GetObject("MvdPoint"));

    if (listHits && listHitMatches) {

      TVector3 hitPos(.0, .0, .0);
      TVector3 mcPos(.0, .0, .0);
      TVector3 hitErr(.0, .0, .0);

      int nEnt = listHits->GetEntriesFast();
      if (verbosity > 2)
        LOG(info) << "CbmRecoQa for " << decName << " found " << nEnt
                  << " Hit Entries";
      for (int j = 0; j < nEnt; j++) {

        //        float bestWeight = 0.f;
        //        float totalWeight = 0.f;
        int iMC = -1;

        CbmMvdHit* curr_hit  = dynamic_cast<CbmMvdHit*>(listHits->At(j));
        CbmMatch* curr_match = dynamic_cast<CbmMatch*>(listHitMatches->At(j));
        CbmMvdPoint* curr_point = 0;

        if (curr_match->GetNofLinks() > 0)
          iMC = curr_match->GetLink(0).GetIndex();
        if (iMC < 0) continue;

        curr_point = (CbmMvdPoint*) (listMvdPoints->At(iMC));

        if (curr_point == 0) continue;

        curr_hit->Position(hitPos);
        curr_hit->PositionError(hitErr);
        if (verbosity > 3) LOG(info) << "Calculated Position Error";

        mcPos.SetX((curr_point->GetX() + curr_point->GetXOut()) / 2.);
        mcPos.SetY((curr_point->GetY() + curr_point->GetYOut()) / 2.);
        mcPos.SetZ(hitPos.Z());
        if (verbosity > 3) LOG(info) << "Calculated MCPos";


        if (hitErr.X() != 0)
          hists[decNum][0]->Fill((hitPos.X() - mcPos.X()) / curr_hit->GetDx());
        if (hitErr.Y() != 0)
          hists[decNum][1]->Fill((hitPos.Y() - mcPos.Y()) / curr_hit->GetDy());

        hists[decNum][3]->Fill((hitPos.X() - mcPos.X()) * 10 * 1000);
        hists[decNum][4]->Fill((hitPos.Y() - mcPos.Y()) * 10 * 1000);
      }
    } else {
      LOG(warn) << "CBMRECOQA WARNING :-- NO Data for Reco QA found! "
                << "Detector: " << decName << std::endl;
    }
  } else if (decName == "much") {

    listHits       = (TClonesArray*) (fManager->GetObject("MuchPixelHit"));
    listHitMatches = (TClonesArray*) (fManager->GetObject("MuchPixelHitMatch"));
    listPoints     = mcManager->InitBranch("MuchPoint");
    if (listPoints == nullptr) { LOG(warn) << "No MuchPoint data!"; }

    if (listHits && listHitMatches) {

      TVector3 hitPos(.0, .0, .0);
      TVector3 mcPos(.0, .0, .0);
      TVector3 hitErr(.0, .0, .0);

      int nEnt = listHits->GetEntriesFast();
      if (verbosity > 2)
        LOG(info) << "CbmRecoQa for " << decName << " found " << nEnt
                  << " Hit Entries";
      for (int j = 0; j < nEnt; j++) {

        float bestWeight  = 0.f;
        float totalWeight = 0.f;
        int iMC           = -1;
        CbmLink link;

        CbmMuchPixelHit* curr_hit =
          dynamic_cast<CbmMuchPixelHit*>(listHits->At(j));
        CbmMatch* curr_match = dynamic_cast<CbmMatch*>(listHitMatches->At(j));

        for (int iLink = 0; iLink < curr_match->GetNofLinks(); iLink++) {
          float tmpweight = curr_match->GetLink(iLink).GetWeight();
          totalWeight += tmpweight;
          //LOG(info) << " Match Link Nr.: " << iLink;
          if (tmpweight > bestWeight) {
            bestWeight = tmpweight;
            iMC        = curr_match->GetLink(iLink).GetIndex();
            link       = curr_match->GetLink(iLink);
            if (verbosity > 3) LOG(info) << "Found Link for current HIT";
          }
        }

        if (iMC < 0) continue;

        CbmMuchPoint* curr_point = (CbmMuchPoint*) (listPoints->Get(
          link.GetFile(), link.GetEntry(), link.GetEntry()));
        double mcTime =
          curr_point->GetTime()
          + eventList->GetEventTime(link.GetEntry(), link.GetFile());
        //LOG(info) << "Much Hit " << j << " Time: " << mcTime << " " << curr_hit->GetTime() << " " << curr_hit->GetTimeError();

        if (curr_point == 0) continue;

        curr_hit->Position(hitPos);
        curr_hit->PositionError(hitErr);
        if (verbosity > 3) LOG(info) << "Calculated Position Error";

        mcPos.SetX((curr_point->GetXIn() + curr_point->GetXOut()) / 2.);
        mcPos.SetY((curr_point->GetYIn() + curr_point->GetYOut()) / 2.);
        mcPos.SetZ(hitPos.Z());
        if (verbosity > 3) LOG(info) << "Calculated MCPos";

        curr_hit->Position(hitPos);

        if (hitErr.X() != 0)
          hists[decNum][0]->Fill((hitPos.X() - mcPos.X()) / curr_hit->GetDx());
        if (hitErr.Y() != 0)
          hists[decNum][1]->Fill((hitPos.Y() - mcPos.Y()) / curr_hit->GetDy());
        if (hitErr.Y() != 0)
          hists[decNum][2]->Fill(
            ((curr_hit->GetTime() - mcTime) / curr_hit->GetTimeError()));

        hists[decNum][3]->Fill((hitPos.X() - mcPos.X()));
        hists[decNum][4]->Fill((hitPos.Y() - mcPos.Y()));
        hists[decNum][5]->Fill(curr_hit->GetTime() - mcTime);
      }
    } else {
      LOG(warn) << "CBMRECOQA WARNING :-- NO Data for Reco QA found! "
                << "Detector: " << decName << std::endl;
    }

  } else {
    LOG(warn) << "CBMRECOQA WARNING :--  NO matching Detector found ! "
              << std::endl;
  }
}
ClassImp(CbmRecoQa)

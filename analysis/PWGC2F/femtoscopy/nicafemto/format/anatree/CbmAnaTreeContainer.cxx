/*
 * NicaDstAnaTreeContainer.cxx
 *
 *  Created on: 17 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmAnaTreeContainer.h"
#include "AnalysisTree/Matching.hpp"
#include <TChain.h>

Bool_t CbmAnaTreeRecoSourceContainer::ConnectToTree(TChain* tree) {
  fEvent     = new AnalysisTree::EventHeader();
  fVtxTracks = new AnalysisTree::TrackDetector();
  fTofHits   = new AnalysisTree::HitDetector();
  fVtx2Tof   = new AnalysisTree::Matching();
  fVtx2Mc    = new AnalysisTree::Matching();
  if (tree->GetBranch("VtxTracks") == nullptr) return kFALSE;
  if (tree->GetBranch("RecEventHeader") == nullptr) return kFALSE;
  if (tree->GetBranch("TofHits") == nullptr) return kFALSE;
  if (tree->GetBranch("VtxTracks2TofHits") == nullptr) return kFALSE;
  tree->SetBranchAddress("VtxTracks", &fVtxTracks);
  tree->SetBranchAddress("RecEventHeader", &fEvent);
  tree->SetBranchAddress("TofHits", &fTofHits);
  tree->SetBranchAddress("VtxTracks2TofHits", &fVtx2Tof);
  tree->SetBranchStatus("VtxTracks", 1);
  tree->SetBranchStatus("RecEventHeader", 1);
  tree->SetBranchStatus("TofHits", 1);
  tree->SetBranchStatus("VtxTracks2TofHits", 1);
  if (tree->GetBranch("VtxTracks2SimParticles")) {
    tree->SetBranchAddress("VtxTracks2SimParticles", &fVtx2Mc);
    tree->SetBranchStatus("VtxTracks2SimParticles", 1);
  }
  return kTRUE;
}

Bool_t CbmAnaTreeMcSourceContainer::ConnectToTree(TChain* tree) {
  fEvent     = new AnalysisTree::EventHeader();
  fParticles = new AnalysisTree::Particles();
  if (tree->GetBranch("SimEventHeader") == nullptr) return kFALSE;
  if (tree->GetBranch("SimParticles") == nullptr) return kFALSE;
  tree->SetBranchAddress("SimEventHeader", &fEvent);
  tree->SetBranchAddress("SimParticles", &fParticles);
  tree->SetBranchStatus("SimEventHeader", 1);
  tree->SetBranchStatus("SimParticles", 1);
  return kTRUE;
}

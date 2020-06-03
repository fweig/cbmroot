/**
 * \file CbmLitAcceptanceFunction.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2014
 * \brief Global function to define the track acceptance. Used in QA.
 */

#ifndef CBMLITACCEPTANCEFUNCTION_H_
#define CBMLITACCEPTANCEFUNCTION_H_

#include "CbmMCTrack.h"
#include "CbmGlobalTrack.h"
#include "CbmTrackMatchNew.h"
#include "TClonesArray.h"
#include "TMath.h"
#include "TDatabasePDG.h"
#include "TMCProcess.h"
#include "CbmMCDataArray.h"
//#include <cmath>
//using std::abs;


class CbmLitAcceptanceFunction
{
public:

   static Bool_t AllTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      return true;
   }

   static Bool_t NegativeTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      const TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle(mcTrack->GetPdgCode());
      if (particle == NULL) return false;
      return particle->Charge() < 0;
   }

   static Bool_t PositiveTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      const TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle(mcTrack->GetPdgCode());
      if (particle == NULL) return false;
      return particle->Charge() > 0;
   }

   static Bool_t PrimaryTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return (mcTrack->GetMotherId() == -1);
      //return (mcTrack->GetGeantProcessId() == kPPrimary);
   }

   static Bool_t ReferenceTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return (mcTrack->GetMotherId() == -1) && (mcTrack->GetP() > 1.);
   }

   static Bool_t SecondaryTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return (mcTrack->GetMotherId() != -1);
   }

   static Bool_t PrimaryElectronTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      //return (TMath::Abs(mcTrack->GetPdgCode()) == 11) && (mcTrack->GetMotherId() == -1);
      return (TMath::Abs(mcTrack->GetPdgCode()) == 11) && (mcTrack->GetGeantProcessId() == kPPrimary);
   }

   static Bool_t PrimaryMuonTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      //return (TMath::Abs(mcTrack->GetPdgCode()) == 13) && (mcTrack->GetMotherId() == -1);
      return (TMath::Abs(mcTrack->GetPdgCode()) == 13) && (mcTrack->GetGeantProcessId() == kPPrimary);
   }

   static Bool_t ElectronTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return TMath::Abs(mcTrack->GetPdgCode()) == 11;
   }

   static Bool_t MuonTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return TMath::Abs(mcTrack->GetPdgCode()) == 13;
   }

   static Bool_t ProtonTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return (TMath::Abs(mcTrack->GetPdgCode()) == 2212);
   }

   static Bool_t AntiProtonTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return mcTrack->GetPdgCode() == -2212;
   }

   static Bool_t PionTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return (TMath::Abs(mcTrack->GetPdgCode()) == 211);
   }

   static Bool_t PionPlusTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return (mcTrack->GetPdgCode() == 211);
   }

   static Bool_t PionMinusTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return (mcTrack->GetPdgCode() == -211);
   }

   static Bool_t KaonTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return (TMath::Abs(mcTrack->GetPdgCode()) == 321);
   }

   static Bool_t KaonPlusTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return (mcTrack->GetPdgCode() == 321);
   }

   static Bool_t KaonMinusTrackAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return (mcTrack->GetPdgCode() == -321);
   }

   static Bool_t AllRingAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index,
         Int_t nofHitsInRing)
   {
      return true;
   }

   static Bool_t AllReferenceRingAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index,
         Int_t nofHitsInRing)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return (mcTrack->GetMotherId() == -1) && (nofHitsInRing >= 15);
   }

   static Bool_t PrimaryElectronRingAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index,
         Int_t nofHitsInRing)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      //return (mcTrack->GetMotherId() == -1) && (TMath::Abs(mcTrack->GetPdgCode()) == 11);
      return (mcTrack->GetGeantProcessId() == kPPrimary) && (TMath::Abs(mcTrack->GetPdgCode()) == 11);
   }

   static Bool_t PrimaryElectronReferenceRingAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index,
         Int_t nofHitsInRing)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
     // return (mcTrack->GetMotherId() == -1) && (TMath::Abs(mcTrack->GetPdgCode()) == 11) && (nofHitsInRing >= 15);
      return (mcTrack->GetGeantProcessId() == kPPrimary) && (TMath::Abs(mcTrack->GetPdgCode()) == 11) && (nofHitsInRing >= 15);
   }

   static Bool_t PionRingAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index,
         Int_t nofHitsInRing)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return (TMath::Abs(mcTrack->GetPdgCode()) == 211);
   }

   static Bool_t PionReferenceRingAcceptanceFunction(
         CbmMCDataArray* mcTracks,
         Int_t eventNo,
         Int_t index,
         Int_t nofHitsInRing)
   {
      if (index < 0) return false;
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventNo, index));
      return (TMath::Abs(mcTrack->GetPdgCode()) == 211) && (nofHitsInRing >= 15);
   }

   static Bool_t AllPiSuppAcceptanceFunction(
         const TClonesArray* globalTracks,
         const TClonesArray* stsMatches,
         const TClonesArray* richMatches,
         Int_t index)
   {
      return true;
   }

   static Bool_t TrueMatchPiSuppAcceptanceFunction(
         const TClonesArray* globalTracks,
         const TClonesArray* stsMatches,
         const TClonesArray* richMatches,
         Int_t index)
   {
      const CbmGlobalTrack* gTrack = static_cast<const CbmGlobalTrack*>(globalTracks->At(index));
      Int_t stsInd = gTrack->GetStsTrackIndex();
      Int_t richInd = gTrack->GetRichRingIndex();
      if (stsInd == -1 || richInd == -1) return false;
      const CbmTrackMatchNew* stsMatch = static_cast<const CbmTrackMatchNew*>(stsMatches->At(stsInd));
      const CbmTrackMatchNew* richMatch = static_cast<const CbmTrackMatchNew*>(richMatches->At(richInd));
      if (NULL == stsMatch || NULL == richMatch) return false;

      if (stsMatch->GetMatchedLink().GetIndex() == richMatch->GetMatchedLink().GetIndex()) return true;
      return false;
   }

   static Bool_t WrongMatchPiSuppAcceptanceFunction(
         const TClonesArray* globalTracks,
         const TClonesArray* stsMatches,
         const TClonesArray* richMatches,
         Int_t index)
   {
      return !TrueMatchPiSuppAcceptanceFunction(globalTracks, stsMatches, richMatches, index);
   }

};

#endif /* CBMLITACCEPTANCEFUNCTION_H_ */

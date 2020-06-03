#include "CbmTrbEdgeMatcher.h"

//TODO check which headers are really needed
#include <cstdlib>

#include <TClonesArray.h>
#include <TH1D.h>
#include <TCanvas.h>
#include "FairRootManager.h"
#include "FairLogger.h"

#include "CbmRichTrbParam.h"
#include "CbmRichTrbDigi.h"
#include "CbmTrbRawMessage.h"
#include "CbmTrbCalibrator.h"

// Uncomment if you want to have excessive printout (do not execute on many events, may produce Gb's of output)
//#define DEBUGPRINT

CbmTrbEdgeMatcher::CbmTrbEdgeMatcher() 
  : FairTask(),
    fTrbRawHits(NULL), 
    fRichTrbDigi(new TClonesArray("CbmRichTrbDigi", 10)),
    tdcIdToStoredEdges(),
    fTrBuf(),
    fMultiCounter(0),
    fDrawHist(kFALSE),
    fhTtimeMinusLtime(),
    fhTtimeMinusLtimeCH(),
    fhMultiDist()
{
}

CbmTrbEdgeMatcher::~CbmTrbEdgeMatcher()
{
}

InitStatus CbmTrbEdgeMatcher::Init()
{
   FairRootManager* manager = FairRootManager::Instance();
   fTrbRawHits = (TClonesArray*)manager->GetObject("CbmTrbRawMessage");
   if (NULL == fTrbRawHits) { Fatal("CbmTrbEdgeMatcher","No CbmTrbRawMessage array!"); }

   manager->Register("CbmRichTrbDigi", "RICH TRB leading-trailing edge pairs", fRichTrbDigi, IsOutputBranchPersistent("CbmRichTrbDigi"));

   if (fDrawHist)
   {
      CbmRichTrbParam* param = CbmRichTrbParam::Instance();

      TString histoName;
      TString histoTitle;

      for (UInt_t i=0; i<68; i++) {
         histoName.Form("fhTtimeMinusLtime - TDC %04x", param->IntegerToTDCid(i));
         histoTitle.Form("(Trailing time - Leading time) for TDC %04x", param->IntegerToTDCid(i));
         fhTtimeMinusLtime[i] = new TH1D(histoName, histoTitle, 4000, -50., 50.);
      }

      for (UInt_t i=0; i<16; i++) {
         histoName.Form("fhTtimeMinusLtime - TDC %04x CH%d - CH%d", DEBUGTDCID, i*2+2, i*2+1);
         histoTitle.Form("(Trailing time - Leading time) for TDC %04x CH%d - CH%d", DEBUGTDCID, i*2+2, i*2+1);
         fhTtimeMinusLtimeCH[i] = new TH1D(histoName, histoTitle, 4000, -50., 50.);
      }
   }

   fhMultiDist = new TH1D("123", "234", 20*(POSITIVEPAIRWINDOW+NEGATIVEPAIRWINDOW), -NEGATIVEPAIRWINDOW, POSITIVEPAIRWINDOW);

   return kSUCCESS;
}

void CbmTrbEdgeMatcher::Exec(Option_t* /*option*/)
{
   CbmRichTrbParam* param = CbmRichTrbParam::Instance();

   for (Int_t i=0; i<fTrbRawHits->GetEntries(); i++) { // for loop over the input raw hits
      CbmTrbRawMessage* curTrbRawHit = static_cast<CbmTrbRawMessage*>(fTrbRawHits->At(i));

      UInt_t tdcId = curTrbRawHit->GetSourceAddress();
      UInt_t channel = curTrbRawHit->GetChannelID();
      UInt_t fine = curTrbRawHit->GetTDCfine();
      UInt_t coarse = curTrbRawHit->GetTDCcoarse();
      UInt_t epoch = curTrbRawHit->GetEpochMarker();
//      Double_t corr = curTrbRawHit->GetCorr();

      if (param->IsSyncChannel(channel)) {                   // SYNCH MESSAGE PROCESSING

         //TODO implement

         Double_t timestamp = GetFullTime(tdcId, channel, epoch, coarse, fine) /*+ corr*/;

         // This is channel zero, we suppose that here only leading edge is coming, so write an 'only-leading-edge' RichTrbDigi.
         // Set channel to 0xffffffff if the edge is unavailable

         // Create leading-edge-only sync digi
         new( (*fRichTrbDigi)[fRichTrbDigi->GetEntriesFast()] )
            CbmRichTrbDigi(tdcId, kTRUE, kFALSE, channel, 0xffffffff, timestamp, 0.0);

      } else if (param->IsLeadingEdgeChannel(channel)) {     // LEADING EDGE PROCESSING

#ifdef DEBUGPRINT
         Double_t timestamp = GetFullTime(tdcId, channel, epoch, coarse, fine) /*+ corr*/;
         printf ("Processing leading edge. tdc 0x00%x  ch %d  %f\n", tdcId, channel, timestamp);
#endif

         //FIXME
/*
         if (tdcId == 0x0110 || tdcId == 0x0111) {
            this->CreateLeadingEdgeOnlyDigi(*curTrbRawHit);
         }
*/
         this->AddPossibleLeadingEdge(*curTrbRawHit);

      } else {                                              // TRAILING EDGE PROCESSING

         // LEADING-TRAILING edges matching

#ifdef DEBUGPRINT
         Double_t timestamp = GetFullTime(tdcId, channel, epoch, coarse, fine) /*+ corr*/;
         printf ("Processing trailing edge. tdc 0x00%x  ch %d  %f\n", tdcId, channel, timestamp);
#endif

         //FIXME
/*
         if (tdcId == 0x0110 || tdcId == 0x0111) {
            // Create trailing-edge-only digi
            new( (*fRichTrbDigi)[fRichTrbDigi->GetEntriesFast()] )
               CbmRichTrbDigi(tdcId, kFALSE, kTRUE, 0xffffffff, channel, 0.0, timestamp);
         }
*/

         UInt_t searchedLchannel = param->GetCorrespondingLeadingEdgeChannel(channel, tdcId);
         std::pair<UInt_t, CbmTrbRawMessage> foundLedge = this->FindLeadingEdge(tdcId, searchedLchannel, *curTrbRawHit);

         if (foundLedge.first != 0xffffffff)  // corresponding leading edge found
         {

            UInt_t lchannel = foundLedge.second.GetChannelID();
            UInt_t lfine = foundLedge.second.GetTDCfine();
            UInt_t lcoarse = foundLedge.second.GetTDCcoarse();
//            Double_t lcorr = foundLedge.second.GetCorr();

            Double_t tfullTime = GetFullTime(tdcId, channel, epoch, coarse, fine) /*+ corr*/;
            Double_t lfullTime = GetFullTime(tdcId, lchannel, foundLedge.first, lcoarse, lfine) /*+ lcorr*/;

            if (fDrawHist) {
               fhTtimeMinusLtime[param->TDCidToInteger(tdcId)]->Fill(tfullTime - lfullTime);
               if (tdcId == DEBUGTDCID) fhTtimeMinusLtimeCH[(channel/2)-1]->Fill(tfullTime - lfullTime);
            }

            if (tfullTime - lfullTime < 0.) {
               // negative time-over-threshold in the found pair
            }

            // Create both-edges digi
            new( (*fRichTrbDigi)[fRichTrbDigi->GetEntriesFast()] )
               CbmRichTrbDigi(tdcId, kTRUE, kTRUE, lchannel, channel, lfullTime, tfullTime);

#ifdef DEBUGPRINT
            printf("PAIR\t\tToT=%f\n", tfullTime - lfullTime);
            printf("   --- LEAD  - tdc %x ch %d epoch %08x coarse %08x fine %08x\n", tdcId, lchannel, foundLedge.first, lcoarse, lfine);
            printf("   --- TRAIL - tdc %x ch %d epoch %08x coarse %08x fine %08x\n", tdcId, channel, epoch, coarse, fine);
#endif

         } else {  // corresponding leading edge not found

            fTrBuf.push_back(curTrbRawHit);

         }

      } // TYPE OF THE EDGE

   } // for loop over the input raw hits

   for (auto iter : fTrBuf) {

      UInt_t tdcId = iter->GetSourceAddress();
      UInt_t channel = iter->GetChannelID();
      UInt_t fine = iter->GetTDCfine();
      UInt_t coarse = iter->GetTDCcoarse();
      UInt_t epoch = iter->GetEpochMarker();
      //Double_t corr = iter->GetCorr();

//	  Double_t fullT = GetFullTime(tdcId, channel, epoch, coarse, fine) /*+ corr*/;

      //printf ("POSTPROCESSING EVENT: tdc 0x00%x  ch %d time=%f\n", tdcId, channel, fullT);

      // Try to find the leading edge again - now in he buffer, which was formed during the first run
      UInt_t searchedLchannel = param->GetCorrespondingLeadingEdgeChannel(channel, tdcId);
      std::pair<UInt_t, CbmTrbRawMessage> foundLedge = this->FindLeadingEdge(tdcId, searchedLchannel, *iter);

      if (foundLedge.first != 0xffffffff)  // corresponding leading edge found
      {
         //printf("FOUND!!!\n");

         UInt_t lchannel = foundLedge.second.GetChannelID();
         UInt_t lfine = foundLedge.second.GetTDCfine();
         UInt_t lcoarse = foundLedge.second.GetTDCcoarse();
//         Double_t lcorr = foundLedge.second.GetCorr();

         Double_t tfullTime = GetFullTime(tdcId, channel, epoch, coarse, fine) /*+ corr*/;
         Double_t lfullTime = GetFullTime(tdcId, lchannel, foundLedge.first, lcoarse, lfine) /*+ lcorr*/;

         if (fDrawHist) {
            fhTtimeMinusLtime[param->TDCidToInteger(tdcId)]->Fill(tfullTime - lfullTime);
            if (tdcId == DEBUGTDCID) fhTtimeMinusLtimeCH[(channel/2)-1]->Fill(tfullTime - lfullTime);
         }

         // Create both-edges digi
         new( (*fRichTrbDigi)[fRichTrbDigi->GetEntriesFast()] )
            CbmRichTrbDigi(tdcId, kTRUE, kTRUE, lchannel, channel, lfullTime, tfullTime);

#ifdef DEBUGPRINT
         printf("PAIR\t\tToT=%f\n", tfullTime - lfullTime);
         printf("   --- LEAD  - tdc %x ch %d epoch %08x coarse %08x fine %08x\n", tdcId, lchannel, foundLedge.first, lcoarse, lfine);
         printf("   --- TRAIL - tdc %x ch %d epoch %08x coarse %08x fine %08x\n", tdcId, channel, epoch, coarse, fine);
#endif

      } else {
         //printf("EDGE NOT FOUND!!! %x\tch %d\n", tdcId, channel);

         Double_t timestamp = GetFullTime(tdcId, channel, epoch, coarse, fine); //+ corr

         // Create trailing-edge-only digi
         new( (*fRichTrbDigi)[fRichTrbDigi->GetEntriesFast()] )
            CbmRichTrbDigi(tdcId, kFALSE, kTRUE, 0xffffffff, channel, 0.0, timestamp);

#ifdef DEBUGPRINT
         printf("TRAILING ONLY\n");
         printf("   --- TRAIL - tdc %x ch %d epoch %08x coarse %08x fine %08x\n", tdcId, channel, epoch, coarse, fine);
#endif

      }

   }

   fTrBuf.clear();

}

void CbmTrbEdgeMatcher::FinishEvent()
{
   fRichTrbDigi->Clear();
}

void CbmTrbEdgeMatcher::FinishTask()
{
   LOG(info) << fMultiCounter << " cases with multiple leading edges within time window.";

	UInt_t v_numOfStoredEdges=0;

	for (UInt_t tdcId=0x0010; tdcId<=0x0013; tdcId++) {

		std::map< UInt_t, std::vector< std::pair< UInt_t, CbmTrbRawMessage > >* >::iterator tdcIdToStoredEdgesI;
		tdcIdToStoredEdgesI = tdcIdToStoredEdges.find(tdcId);

		if (tdcIdToStoredEdgesI != tdcIdToStoredEdges.end()) {
			std::vector< std::pair< UInt_t, CbmTrbRawMessage > >* PerTDCbuffer;
			PerTDCbuffer = tdcIdToStoredEdgesI->second;
			for (unsigned int v_channel=0; v_channel<=32; v_channel++) {
				v_numOfStoredEdges += PerTDCbuffer[v_channel].size();
			}
		}

	}
	LOG(info) << "Leftovers in the pair-matching buffers: " << v_numOfStoredEdges;

   //fhMultiDist->Draw();

   this->DrawDebugHistos();
}

void CbmTrbEdgeMatcher::AddPossibleLeadingEdge(CbmTrbRawMessage rawMessage)
{
   UInt_t tdcId = rawMessage.GetSourceAddress();
   UInt_t lChannel = rawMessage.GetChannelID();
   UInt_t lEpoch = rawMessage.GetEpochMarker();

   // Find the buffer for the given channel.
   // If no yet - create it.
   std::map< UInt_t, std::vector< std::pair< UInt_t, CbmTrbRawMessage > >* >::iterator tdcIdToStoredEdgesI;
   tdcIdToStoredEdgesI = tdcIdToStoredEdges.find(tdcId);
   if (tdcIdToStoredEdgesI == tdcIdToStoredEdges.end())
   {
      tdcIdToStoredEdges.insert( std::pair< UInt_t, std::vector< std::pair< UInt_t, CbmTrbRawMessage > >* >(tdcId, new std::vector< std::pair< UInt_t, CbmTrbRawMessage > >[33]) );
      tdcIdToStoredEdgesI = tdcIdToStoredEdges.find(tdcId);
   }

   // Select the sub-buffer corresponding to the channel and add an entry there.
   std::vector< std::pair< UInt_t, CbmTrbRawMessage > >* PerTDCbuffer;
   PerTDCbuffer = tdcIdToStoredEdgesI->second;
   PerTDCbuffer[lChannel].push_back(std::pair<UInt_t, CbmTrbRawMessage>(lEpoch, rawMessage));

#ifdef DEBUGPRINT
   printf ("Adding leading edge tdc %x ch %d\n", tdcId, lChannel);
#endif
}

// return found leading edge which corresponds to the given falling edge, the found instance is cleared
std::pair<UInt_t, CbmTrbRawMessage> CbmTrbEdgeMatcher::FindLeadingEdge(UInt_t tdcId, UInt_t lChannel, CbmTrbRawMessage tRawMessage)
{
   std::pair<UInt_t, CbmTrbRawMessage> foundResult;

   UInt_t tChannel = tRawMessage.GetChannelID();
   UInt_t tFine = tRawMessage.GetTDCfine();
   UInt_t tCoarse = tRawMessage.GetTDCcoarse();
   UInt_t tEpoch = tRawMessage.GetEpochMarker();
//   Double_t tCorr = tRawMessage.GetCorr();

   Double_t tFullTime = GetFullTime(tdcId, tChannel, tEpoch, tCoarse, tFine) /*+ tCorr*/;

   std::map< UInt_t, std::vector< std::pair< UInt_t, CbmTrbRawMessage > >* >::iterator tdcIdToStoredEdgesI;
   tdcIdToStoredEdgesI = tdcIdToStoredEdges.find(tdcId);

   // no entries for the given tdc is found - no need to create a 'leading-edge-only' digi. (The trailing-edge-only digi is created outside of this method.)
   if (tdcIdToStoredEdgesI == tdcIdToStoredEdges.end()) {
      foundResult.first = 0xffffffff; /*foundResult.second = 0xffffffff;*/
      return foundResult;
   } else {

      UInt_t foundEpoch = 0xffffffff;
      CbmTrbRawMessage foundWord;

      std::vector< std::pair< UInt_t, CbmTrbRawMessage > >* PerTDCbuffer;
      PerTDCbuffer = tdcIdToStoredEdgesI->second;

      UInt_t numOfStoredEdges = PerTDCbuffer[lChannel].size();

      if (numOfStoredEdges == 0) // nothing found for the given channel - no need to create a 'leading-edge-only' digi. (The trailing-edge-only digi is created outside of this method.)
      {
#ifdef DEBUGPRINT
         printf("         numOfStoredEdges == 0\n");
#endif
         foundResult.first = 0xffffffff; /*foundResult.second = 0xffffffff;*/
         return foundResult;
      }
      else if (numOfStoredEdges == 1) // only one edge is stored for the given channel. If ToT allowed - return pair, otherwise postpone until the end of the event
      {

#ifdef DEBUGPRINT
         printf("         numOfStoredEdges == 1\n");
#endif

         std::pair< UInt_t, CbmTrbRawMessage > foundPair;

         foundPair = PerTDCbuffer[lChannel].back();

/*
         //FIXME hardcoded case for the LED - create a pair no matter what TOT is.
         if (tdcId == 0x0110 && lChannel == 7) { return foundPair; }
         //FIXME hardcoded case for the Laser - create a pair no matter what TOT is.
         if (tdcId == 0x0110 && lChannel == 15) { return foundPair; }
*/

         //FIXME
         if (tdcId == 0x0110 || tdcId == 0x0111 || tdcId == 0x0113) { return foundPair; }

         foundEpoch = foundPair.first;
         foundWord =  foundPair.second;

         UInt_t foundLcoarse = foundWord.GetTDCcoarse();
         UInt_t foundLfine = foundWord.GetTDCfine();
//         Double_t foundCorr = foundWord.GetCorr();
         Double_t lFullTime = GetFullTime(tdcId, lChannel, foundEpoch, foundLcoarse, foundLfine) /*+ foundCorr*/;

         if ((tFullTime - lFullTime) < -NEGATIVEPAIRWINDOW || (tFullTime - lFullTime) > POSITIVEPAIRWINDOW) {
#ifdef DEBUGPRINT
            printf("only one edge is stored for the given channel. Restricted ToT. tdc 0x00%x  ch %d l=%f t=%f (TOT=%f)\n", tdcId, lChannel, lFullTime, tFullTime, (tFullTime - lFullTime));
#endif
            //// Create a digi with leading edge only (a digi with trailing edge only is created outside of this method).
            //this->CreateLeadingEdgeOnlyDigi(foundWord);
            foundResult.first = 0xffffffff; /*foundResult.second = 0xffffffff;*/
            return foundResult;
         } else {
            PerTDCbuffer[lChannel].pop_back();
            return foundPair;
         }

      }
      else // more than one word found
      {
         std::vector< std::pair< UInt_t, CbmTrbRawMessage > >::iterator PerTDCbufferI;
         std::vector< std::pair< UInt_t, CbmTrbRawMessage > >::iterator foundWordIndex;

/*
         // Flush all the leading edges which were earlier than fallingEdge-NEGATIVEPAIRWINDOW
         for (PerTDCbufferI = PerTDCbuffer[lChannel].begin(); PerTDCbufferI != PerTDCbuffer[lChannel].end(); ) {
               foundEpoch = PerTDCbufferI->first;
               foundWord =  PerTDCbufferI->second;
               UInt_t foundLcoarse = foundWord.GetTDCcoarse();
               UInt_t foundLfine = foundWord.GetTDCfine();
               Double_t foundLcorr = foundWord.GetCorr();
               Double_t lFullTime = GetFullTime(tdcId, lChannel, foundEpoch, foundLcoarse, foundLfine); //+ foundLcorr;

               if (lFullTime < tFullTime - POSITIVEPAIRWINDOW) {
#ifdef DEBUGPRINT
                  printf ("Creating leading-edge-only digi lFullTime=%f < tFullTime=%f\n", lFullTime, tFullTime);
#endif
                  this->CreateLeadingEdgeOnlyDigi(foundWord);
                  PerTDCbufferI = PerTDCbuffer[lChannel].erase(PerTDCbufferI);
               } else {
                  ++PerTDCbufferI;
               }
         }
*/

         UInt_t numWithinWindow = 0;
         Double_t storedTOT = 0.;

         // Count how many leading edges in the buffer are in the allowed time window
         for (PerTDCbufferI = PerTDCbuffer[lChannel].begin(); PerTDCbufferI != PerTDCbuffer[lChannel].end(); ++PerTDCbufferI) {
            foundEpoch = PerTDCbufferI->first;
            foundWord =  PerTDCbufferI->second;
            UInt_t foundLcoarse = foundWord.GetTDCcoarse();
            UInt_t foundLfine = foundWord.GetTDCfine();
//            Double_t foundLcorr = foundWord.GetCorr();
            Double_t lFullTime = GetFullTime(tdcId, lChannel, foundEpoch, foundLcoarse, foundLfine) /*+ foundLcorr*/;

            if ((tFullTime - lFullTime) >= -NEGATIVEPAIRWINDOW && (tFullTime - lFullTime) <= POSITIVEPAIRWINDOW) {
               numWithinWindow++;                  // count
               foundWordIndex = PerTDCbufferI;     // store
               storedTOT = tFullTime - lFullTime;  // store
            }

         }

#ifdef DEBUGPRINT
         printf("         numWithinWindow == %d\n", numWithinWindow);
#endif

         // If there are no leading edges within allowed time window - ("clear" everything before the current trailing edge)
         // Current trailing edge and all leading edges before this trailing edge will be sent to output as separate edges.
         if (numWithinWindow == 0) {
            foundResult.first = 0xffffffff; /*foundResult.second = 0xffffffff;*/
            return foundResult;

         // Only one leading edge within the allowed time window is found and a) TOT>0 - Perfect case, build a pair. b) TOT<0 - allowed, build a pair
         } else if (numWithinWindow == 1) {

               foundEpoch = foundWordIndex->first;
               foundWord = foundWordIndex->second;
               if (storedTOT >= 0.) {
               #ifdef DEBUGPRINT
                  printf ("One edge in the window. Allowed positive TOT\n");
               #endif
               } else {
               #ifdef DEBUGPRINT
                  printf ("One edge in the window. Allowed negative TOT\n");
               #endif
               }
               PerTDCbuffer[lChannel].erase(foundWordIndex);
               // Return epoch and data word (a pair is created outside of this method).
               foundResult.first = foundEpoch; foundResult.second = foundWord;
               return foundResult;
         } else {

//TODO implement

//#ifdef DEBUGPRINT
               printf ("Multiple edges in the window.\t\tTDC %x\t\tTrailing edge channel=%d\n", tdcId, tChannel);
//#endif
               fMultiCounter++;

/*
               for (UInt_t i=0; i<BUFSIZE; i++) {
                  if (tdcIdToStoredEdgesI->second[lChannel*BUFSIZE*2 + i*2 + 1] != 0xffffffff) {
                     foundEpoch = tdcIdToStoredEdgesI->second[lChannel*BUFSIZE*2 + i*2 + 0];  // store epoch
                     foundWord =  tdcIdToStoredEdgesI->second[lChannel*BUFSIZE*2 + i*2 + 1];  // store data word

					UInt_t foundLcoarse = foundWord.GetTDCcoarse();
					UInt_t foundLfine = foundWord.GetTDCfine();
					Double_t foundLcorr = foundWord.GetCorr();
					Double_t lFullTime = GetFullTime(tdcId, lChannel, foundEpoch, foundLcoarse, foundLfine) + foundLcorr;

                     fhMultiDist->Fill(tFullTime - lFullTime);

                  }
               }
*/
               foundResult.first = 0xffffffff; //foundResult.second = 0xffffffff;
               return foundResult;
         }

      }

   }

}

void CbmTrbEdgeMatcher::CreateLeadingEdgeOnlyDigi(CbmTrbRawMessage rawMessage)
{
   // Create a digi with leading edge only

   UInt_t tdcId = rawMessage.GetSourceAddress();
   UInt_t lchannel = rawMessage.GetChannelID();
   UInt_t lepoch = rawMessage.GetEpochMarker();
   UInt_t lcoarse = rawMessage.GetTDCcoarse();
   UInt_t lfine = rawMessage.GetTDCfine();
//   Double_t lcorr = rawMessage.GetCorr();

   Double_t timestamp = GetFullTime(tdcId, lchannel, lepoch, lcoarse, lfine) /*+ lcorr*/;

   new( (*fRichTrbDigi)[fRichTrbDigi->GetEntriesFast()] )
      CbmRichTrbDigi(tdcId, kTRUE, kFALSE, lchannel, 0xffffffff, timestamp, 0.0);

#ifdef DEBUGPRINT
   printf("LEADING ONLY\n");
   printf("   --- LEAD - tdc %x ch %d epoch %08x coarse %08x fine %08x corr %f\n", tdcId, lchannel, lepoch, lcoarse, lfine, lcorr);
#endif
}

Double_t CbmTrbEdgeMatcher::GetFullTime(UInt_t tdcId, UInt_t channel, UInt_t epoch, UInt_t coarse, UInt_t fine)
{
   return CbmTrbCalibrator::Instance()->GetFullTime(tdcId, channel, epoch, coarse, fine);
}

void CbmTrbEdgeMatcher::DrawDebugHistos()
{
   if (!fDrawHist) return;

   CbmRichTrbParam* param = CbmRichTrbParam::Instance();

   TCanvas* c[17];
   for (UInt_t i=1; i<18; i++) {

      TString canvasName;
      TString canvasTitle;
      canvasName.Form("fhTtimeMinusLtime - TDCs %04x, %04x, %04x, %04x", param->IntegerToTDCid(i*4+0),
                                                                         param->IntegerToTDCid(i*4+1),
                                                                         param->IntegerToTDCid(i*4+2),
                                                                         param->IntegerToTDCid(i*4+3));
      canvasTitle.Form("(Trailing time - Leading time) for TDCs %04x, %04x, %04x, %04x",
                                                                         param->IntegerToTDCid(i*4+0),
                                                                         param->IntegerToTDCid(i*4+1),
                                                                         param->IntegerToTDCid(i*4+2),
                                                                         param->IntegerToTDCid(i*4+3));

      c[i-1] = new TCanvas(canvasName, canvasTitle, 800, 800);
      c[i-1]->Divide(2, 2);
      c[i-1]->cd(1);
      c[i-1]->GetPad(1)->SetLogy(1);
      fhTtimeMinusLtime[(i-1)*4+0]->Draw();
      c[i-1]->cd(2);
      c[i-1]->GetPad(2)->SetLogy(1);
      fhTtimeMinusLtime[(i-1)*4+1]->Draw();
      c[i-1]->cd(3);
      c[i-1]->GetPad(3)->SetLogy(1);
      fhTtimeMinusLtime[(i-1)*4+2]->Draw();
      c[i-1]->cd(4);
      c[i-1]->GetPad(4)->SetLogy(1);
      fhTtimeMinusLtime[(i-1)*4+3]->Draw();
   }

/*  (FU) not used
   TCanvas* c2[16];
   TCanvas* c3;
*/

   TString canvasName;
   TString canvasTitle;

   canvasName.Form("fhTtimeMinusLtime - TDC %04x", DEBUGTDCID);
   canvasTitle.Form("(Trailing time - Leading time) for TDC %04x", DEBUGTDCID);
   new TCanvas(canvasName, canvasTitle, 800, 800); 
   gPad->SetLogy(1);
   fhTtimeMinusLtime[param->TDCidToInteger(DEBUGTDCID)]->Draw();

   for (UInt_t i=0; i<16; i++) {
      canvasName.Form("fhTtimeMinusLtime - TDC %04x CH%d - CH%d", DEBUGTDCID, i*2+2, i*2+1);
      canvasTitle.Form("(Trailing time - Leading time) for TDC %04x CH%d - CH%d", DEBUGTDCID, i*2+2, i*2+1);

      new TCanvas(canvasName, canvasTitle, 800, 800);
      gPad->SetLogy(1);
      fhTtimeMinusLtimeCH[i]->Draw();

   }
}

ClassImp(CbmTrbEdgeMatcher)

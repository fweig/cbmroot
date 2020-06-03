// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmUnpackTofStar2018                       -----
// -----               Created 08.12.2017 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmUnpackTofStar2018_H
#define CbmUnpackTofStar2018_H

#include "Timeslice.hpp"
#include "rocMess_wGet4v1.h"

#include "CbmTSUnpack.h"
#include "CbmHistManager.h"
#include "CbmTofDigi.h"
#include "CbmTbDaqBuffer.h"

#include "TClonesArray.h"

#include <vector>
#include <map>

class CbmTofUnpackPar;

class CbmUnpackTofStar2018 : public CbmTSUnpack
{
public:

   CbmUnpackTofStar2018( UInt_t uNbGdpb = 1 );
   virtual ~CbmUnpackTofStar2018();

   virtual Bool_t Init();

   virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

   virtual void Reset();

   virtual void Finish();

   void SetParContainers();

   Bool_t InitContainers();

   Bool_t ReInitContainers();


   void FillOutput(CbmDigi* digi);

   void SetMsLimitLevel( size_t uAcceptBoundaryPct = 100 ) { fuMsAcceptsPercent = uAcceptBoundaryPct; }
   size_t GetMsLimitLevel( ) { return fuMsAcceptsPercent; }

   void SetMsOverlap(size_t uOverlapMsNb = 1) { fuOverlapMsNb = uOverlapMsNb; }
   size_t GetMsOverlap()                      { return fuOverlapMsNb; }

   inline void SetTShiftRef(Double_t val) {fdTShiftRef = val;}

private:

   size_t fuMsAcceptsPercent; /** Reject Ms with index inside TS above this, assumes 100 MS per TS **/
   size_t fuOverlapMsNb;      /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
   UInt_t fuMinNbGdpb;
   UInt_t fuCurrNbGdpb;

   UInt_t fuNrOfGdpbs;           // Total number of GDPBs in the system
   UInt_t fuNrOfFebsPerGdpb;     // Number of FEBs per GDPB
   UInt_t fuNrOfGet4PerFeb;      // Number of GET4s per FEB
   UInt_t fuNrOfChannelsPerGet4; // Number of channels in each GET4

   UInt_t fuNrOfChannelsPerFeet; // Number of channels in each FEET
   UInt_t fuNrOfGet4;            // Total number of Get4 chips in the system
   UInt_t fuNrOfGet4PerGdpb;     // Number of GET4s per GDPB
   UInt_t fuNrOfChannelsPerGdpb; // Number of channels per GDPB

   std::vector<int> fMsgCounter;
   std::map<UInt_t, UInt_t> fGdpbIdIndexMap;

   UInt_t fuGdpbId; // Id (hex number) of the GDPB for current message
   UInt_t fuGdpbNr; // running number (0 to fNrOfGdpbs) of the GDPB for current message
   UInt_t fuGet4Id; // running number (0 to fNrOfGet4PerGdpb) of the Get4 chip of a unique GDPB for current message
   UInt_t fuGet4Nr; // running number (0 to fNrOfGet4) of the Get4 chip in the system for current message

   CbmHistManager* fHM;  ///< Histogram manager

   /** Current epoch marker for each GDPB and GET4
     * (first epoch in the stream initializes the map item)
     * pointer points to an array of size fNrOfGdpbs * fNrOfGet4PerGdpb
     * The correct array index is calculated using the function
     * GetArrayIndex(gdpbId, get4Id)
     **/
   std::vector< ULong64_t > fvulCurrentEpoch; //!
   std::vector< Bool_t >    fvbFirstEpochSeen; //!

   Int_t fNofEpochs;              /** Current epoch marker for each ROC **/
   ULong64_t fulCurrentEpochTime;     /** Time stamp of current epoch **/

   Int_t fEquipmentId;
   Double_t fdMsIndex;
   Double_t fdTShiftRef;

   TClonesArray* fTofDigi;
   CbmTofDigi* fDigi;

   CbmTbDaqBuffer* fBuffer;

   CbmTofUnpackPar* fUnpackPar;      //!

   void CreateHistograms();

   // Variables used for histo filling
   Double_t fdRefTime;
   Double_t fdLastDigiTime;
   Double_t fdFirstDigiTimeDif;
   Double_t fdEvTime0;
   TH1* fhRawTDigEvT0;
   TH1* fhRawTDigRef0;
   TH1* fhRawTDigRef;
   TH1* fhRawTRefDig0;
   TH1* fhRawTRefDig1;
   TH1* fhRawDigiLastDigi;
   std::vector< TH2* > fhRawTotCh;
   std::vector< TH1* > fhChCount;
   std::vector< Bool_t > fvbChanThere;
   std::vector< TH2* > fhChanCoinc;
   Bool_t fbDetChanThere[64];
   TH2*   fhDetChanCoinc;

   std::vector< std::vector < ngdpb::Message > > fvmEpSupprBuffer;

   void FillHitInfo(ngdpb::Message);
   void FillStarTrigInfo(ngdpb::Message);
   void FillEpochInfo(ngdpb::Message);
   void PrintSlcInfo(ngdpb::Message);
   void PrintSysInfo(ngdpb::Message);
   void PrintGenInfo(ngdpb::Message);

   inline Int_t GetArrayIndex(Int_t gdpbId, Int_t get4Id)
   {
      return gdpbId * fuNrOfGet4PerGdpb + get4Id;
   }

   CbmUnpackTofStar2018(const CbmUnpackTofStar2018&);
   CbmUnpackTofStar2018 operator=(const CbmUnpackTofStar2018&);

   ClassDef(CbmUnpackTofStar2018, 1)
};

#endif

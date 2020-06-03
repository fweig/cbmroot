#// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTSMonitorTof                            -----
// -----               Created 11.11.2016 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmTSMonitorTofLegacy_H
#define CbmTSMonitorTofLegacy_H

#include "Timeslice.hpp"
#include "rocMess_wGet4v1.h"

#include "CbmTSUnpack.h"
#include "CbmHistManager.h"

#include "TClonesArray.h"
#include "Rtypes.h"

#include <vector>
#include <map>

class CbmTofUnpackPar;

class CbmTSMonitorTofLegacy : public CbmTSUnpack
{
public:

   CbmTSMonitorTofLegacy();
   virtual ~CbmTSMonitorTofLegacy();

   virtual Bool_t Init();

   virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

   virtual void Reset();

   virtual void Finish();

   void SetParContainers();

   Bool_t InitContainers();

   Bool_t ReInitContainers();


   void FillOutput(boost::any);

   void SetMsLimitLevel( size_t uAcceptBoundaryPct = 100 ) { fuMsAcceptsPercent = uAcceptBoundaryPct; }
   size_t GetMsLimitLevel( ) { return fuMsAcceptsPercent; }

   void SetDiamondChannels( UInt_t uGdpb = 0, UInt_t uFeet = 2, UInt_t uChannelA = 78,
                            UInt_t uChannelB = 70, UInt_t uChannelC = 94, UInt_t uChannelD = 86 );

   void ResetAllHistos();

private:

   size_t fuMsAcceptsPercent; /** Reject Ms with index inside TS above this, assumes 100 MS per TS **/
   UInt_t fuMinNbGdpb;
   UInt_t fuCurrNbGdpb;

   UInt_t fNrOfGdpbs;           // Total number of GDPBs in the system
   UInt_t fNrOfFebsPerGdpb;     // Number of FEBs per GDPB
   UInt_t fNrOfGet4PerFeb;      // Number of GET4s per FEB
   UInt_t fNrOfChannelsPerGet4; // Number of channels in each GET4

   UInt_t fNrOfGet4;            // Total number of Get4 chips in the system
   UInt_t fNrOfGet4PerGdpb;     // Number of GET4s per GDPB

   UInt_t fDiamondGdpb;
   UInt_t fDiamondFeet;
   UInt_t fDiamondChanA;
   UInt_t fDiamondChanB;
   UInt_t fDiamondChanC;
   UInt_t fDiamondChanD;
   UInt_t fDiamondTimeLastReset;

   std::vector<int> fMsgCounter;

   std::map<UInt_t, UInt_t> fGdpbIdIndexMap;

   CbmHistManager* fHM;  ///< Histogram manager

   /** Current epoch marker for each ROC and GET4 (first epoch in the stream initialises the map item) **/
   std::map<Int_t, std::map<Int_t, UInt_t> > fCurrentEpoch;
   Int_t fNofEpochs; /** Current epoch marker for each ROC **/
   ULong_t fCurrentEpochTime;     /** Time stamp of current epoch **/
   Double_t fdStartTime; /** Time of first valid hit (epoch available), used as reference for evolution plots**/

   /** Used only if the channel rate plots are enabled **/
   /** Last Hit time for each ROC/GET4/Channel (first hit in the stream initialises the map item) **/
   std::map<Int_t, std::map<Int_t, std::map<Int_t, Double_t> > > fTsLastHit;    // * 6.25 ns

   Int_t fEquipmentId;

   CbmTofUnpackPar* fUnpackPar;      //!

   void CreateHistograms();

   void FillHitInfo(ngdpb::Message, std::vector<TH2*>,
                    std::vector<TH1*>, std::vector<TH2*>,
                    std::vector<TH1*>, TH2*
                   );
   void FillEpochInfo(ngdpb::Message, TH2*);
   void PrintSlcInfo(ngdpb::Message);
   void PrintSysInfo(ngdpb::Message);
   void PrintGenInfo(ngdpb::Message);

   CbmTSMonitorTofLegacy(const CbmTSMonitorTofLegacy&);
   CbmTSMonitorTofLegacy operator=(const CbmTSMonitorTofLegacy&);

   ClassDef(CbmTSMonitorTofLegacy, 1)
};

#endif

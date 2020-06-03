// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTsMonitorSts                            -----
// -----                 Created 08.03.2017 by P.-A. Loizeau               -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmTsMonitorSts_H
#define CbmTsMonitorSts_H

#include "Timeslice.hpp"
#include "rocMess_wGet4v1.h"

#include "CbmTSUnpack.h"
#include "CbmHistManager.h"

#include <vector>
#include <map>

class CbmStsUnpackPar; // Unpacker Class which will read the StsUnpackPar.par

class CbmTsMonitorSts: public CbmTSUnpack
{
public:
  
   CbmTsMonitorSts();
   virtual ~CbmTsMonitorSts();
    
   virtual Bool_t Init();

   virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

   virtual void Reset();

   virtual void Finish();

   void SetParContainers();

   Bool_t InitContainers();

   Bool_t ReInitContainers();

   void FillOutput(boost::any);

   void SetRunStart( Int_t dateIn, Int_t timeIn, Int_t iBinSize = 5 );

   void ResetAllHistos();
    
   void SaveAllHistos();
    
   void UpdateAdcStatHistos();

private:
   static const Int_t kiNbChanFebF = 128;

   Int_t fNrOfNdpbs;           // Total number of NDPBs in the system
   Int_t fNrOfNdpbsA;           // Total number of NDPBs for GEM A
   Int_t fNrOfNdpbsB;           // Total number of NDPBs for GEM B
   Int_t fNrOfFebsPerNdpb;     // Number of FEBs per NDPB
   
   std::vector<int> fMsgCounter;
   std::map<UInt_t, UInt_t> fNdpbIdIndexMap;

   CbmHistManager* fHM;  ///< Histogram manager

   std::map<Int_t, std::map<Int_t, UInt_t> > fCurrentEpoch; // Current epoch (first epoch in the stream initialises the map item)
   Int_t fNofEpochs; /** Current epoch marker for each ROC **/
   Double_t fCurrentEpochTime;     /** Time stamp of current epoch **/
   Double_t fdStartTime; /** Time of first valid hit (epoch available), used as reference for evolution plots**/
   Double_t fdStartTimeMsSz; /** Time of first microslice, used as reference for evolution plots**/
   TCanvas* fcMsSizeAll;

   Int_t fEquipmentId;

   CbmStsUnpackPar* fUnpackPar;    //!
    
   std::vector<TH1*> fFebRateDate_nDPB;
   std::vector<TH2*> fHitDtDate_nDPB;
   std::vector<TH1*> fHitDt_nDPB;
   std::vector<Double_t> fdLastHitTime_nDPB;
   Int_t             fiRunStartDateTimeSec;
   Int_t             fiBinSizeDatePlots;

   TH1* fHistMessType;
   TH1* fHistSysMessType;
   
   std::vector<TH1*> fChan_Counts_Sts;
   std::vector<TH2*> fRaw_ADC_Sts;
   std::vector<TH1*> fFebRate;
   std::vector<TH1*> fHitMissEvo;
  
   std::vector<TH1*> fADC_Mean_Sts;
   std::vector<TH1*> fADC_Rms_Sts;
   std::vector<TH1*> fADC_Skew_Sts;
   std::vector<TH1*> fADC_Kurt_Sts;
  
   static const Int_t kiMaxNbFlibLinks = 16;
   TH1*      fhMsSz[kiMaxNbFlibLinks];
   TProfile* fhMsSzTime[kiMaxNbFlibLinks];
  
   void CreateHistograms();

   void FillHitInfo(ngdpb::Message, UInt_t uNdpbIdx, UInt_t uFebBase);
   void FillEpochInfo(ngdpb::Message, UInt_t uNdpbIdx, UInt_t uFebBase);

   Int_t CreateAddress(Int_t febBase, Int_t febId, Int_t stationId, Int_t layerId, Int_t sideId,
		  Int_t moduleId, Int_t channelId);

   CbmTsMonitorSts(const CbmTsMonitorSts&);
   CbmTsMonitorSts operator=(const CbmTsMonitorSts&);

   ClassDef(CbmTsMonitorSts, 1)
};

#endif

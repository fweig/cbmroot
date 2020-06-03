// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTSMonitorMuch                            -----
// -----                 Created 11.11.2016 by V. Singhal and A. Kumar     -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmTSMonitorMuch_H
#define CbmTSMonitorMuch_H

#include "Timeslice.hpp"
#include "rocMess_wGet4v1.h"

#include "CbmTSUnpack.h"
#include "CbmHistManager.h"

#include <vector>
#include <map>

class CbmMuchUnpackPar; // Unpaker Class which will read the MuchUnpackPar.par

class CbmTSMonitorMuch: public CbmTSUnpack
{
public:
  
  CbmTSMonitorMuch();
  virtual ~CbmTSMonitorMuch();
    
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

  void SetMsOverlap(size_t uOverlapMsNb = 1) { fuOverlapMsNb = uOverlapMsNb; }
  size_t GetMsOverlap()                      { return fuOverlapMsNb; }

private:
  size_t fuOverlapMsNb;      /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
  
   Int_t fNrOfNdpbs;           // Total number of NDPBs in the system
   Int_t fNrOfNdpbsA;           // Total number of NDPBs for GEM A
   Int_t fNrOfNdpbsB;           // Total number of NDPBs for GEM B
   Int_t fNrOfFebsPerNdpb;     // Number of FEBs per NDPB
   
  std::vector<int> fMsgCounter;
   std::map<UInt_t, UInt_t> fNdpbIdIndexMap;

  Int_t fMuchStationMapX[16][128]; //fMuchStationMap for map of row number via getX
  Int_t fMuchStationMapY[16][128]; // For map of column number via getY>

  CbmHistManager* fHM;  ///< Histogram manager

  std::map<Int_t, std::map<Int_t, UInt_t> > fCurrentEpoch; // Current epoch (first epoch in the stream initialises the map item)
  Int_t fNofEpochs; /** Current epoch marker for each ROC **/
  Double_t fCurrentEpochTime;     /** Time stamp of current epoch **/
  Double_t fdStartTime; /** Time of first valid hit (epoch available), used as reference for evolution plots**/
  Double_t fdStartTimeMsSz; /** Time of first microslice, used as reference for evolution plots**/
  TCanvas* fcMsSizeAll;

  Int_t fEquipmentId;

  CbmMuchUnpackPar* fUnpackPar;    //!
    
  std::vector<TH1*> fFebRateDate_nDPB;
  std::vector<TH2*> fHitDtDate_nDPB;
  std::vector<Double_t> fdLastHitTime_nDPB;
  Int_t             fiRunStartDateTimeSec;
  Int_t             fiBinSizeDatePlots;

  TH1* fHistMessType;
  TH1* fHistSysMessType;
   
  std::vector<TH1*> fChan_Counts_Much;
  std::vector<TH2*> fRaw_ADC_Much;
  std::vector<TH1*> fFebRate;
  std::vector<TH1*> fHitMissEvo;
  
  TH2* fHistPadDistr;
  
  static const Int_t kiMaxNbFlibLinks = 16;
  TH1*      fhMsSz[kiMaxNbFlibLinks];
  TProfile* fhMsSzTime[kiMaxNbFlibLinks];
  
  void CreateHistograms();

  void FillHitInfo(ngdpb::Message, UInt_t uNdpbIdx, UInt_t uFebBase);
  void FillEpochInfo(ngdpb::Message, UInt_t uNdpbIdx, UInt_t uFebBase);

  Int_t CreateAddress(Int_t febBase, Int_t febId, Int_t stationId, Int_t layerId, Int_t sideId,
		  Int_t moduleId, Int_t channelId);

  CbmTSMonitorMuch(const CbmTSMonitorMuch&);
  CbmTSMonitorMuch operator=(const CbmTSMonitorMuch&);

  ClassDef(CbmTSMonitorMuch, 1)
};

#endif

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           CbmTSUnpackSpadic20OnlineMonitor        -----
// -----                    Created 20.12.2016 by C. Bergmann              -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMTSUNPACKSPADIC20DESYONLINEMONITOR_H
#define CBMTSUNPACKSPADIC20DESYONLINEMONITOR_H

#include "Timeslice.hpp"
#include "Message.hpp"

#include <map>
#include "CbmTSUnpack.h"
#include "CbmBeamDefaults.h"

#include "CbmHistManager.h"
#include "TClonesArray.h"

class CbmTSUnpackSpadic20DesyOnlineMonitor : public CbmTSUnpack
{
 public:

  CbmTSUnpackSpadic20DesyOnlineMonitor(Bool_t highPerformancen = true );
  virtual ~CbmTSUnpackSpadic20DesyOnlineMonitor();

  virtual Bool_t Init();

  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
  void print_message(const spadic::Message& m);

  virtual void Reset();

  virtual void Finish();
  virtual void FinishEvent();
  void SetParContainers() {;}

  // protected:
  //  virtual void Register();

  virtual void FillOutput(boost::any){;}
  
 private:
    
  TClonesArray* fSpadicRaw;

  Bool_t fHighPerformance;
 
  
  std::map<std::pair<int,int>,int>fEpochMarkerArray;
  //Array to store the previous Epoch counter
  std::map<std::pair<int,int>,int>fPreviousEpochMarkerArray;
  //Suppress multiple Epoch Messages in duplicated Microslices. NOTE:Currently Buggy
  const Bool_t SuppressMultipliedEpochMessages = false;
  std::map<std::pair<int,int>,int>fSuperEpochArray;

  static Int_t   GetSpadicID(Int_t address);
//  static Int_t   GetAfckID(Int_t AfckAddress);
  inline TString GetSpadicName(Int_t link,Int_t address);
  Int_t GetChannelOnPadPlane(Int_t SpadicChannel, Int_t groupId);
  Int_t fEpochMarker;
  Int_t fSuperEpoch;
  Int_t fNrExtraneousSamples;
  Int_t  startTime = 0; // should be ULong_t as soon as used with a GetFullTime lateron
  Bool_t first = true; // for the "Sync" histo : is it the very first entry into this histo? 
  Bool_t syncHistFinished = false; // declare explicitly, if the "Sync" histo is finished.
  Int_t  maxTime = 0; // should be ULong_t as soon as used with a GetFullTime lateron
  Int_t lastTsCh7 = 0;
  void GetEpochInfo(Int_t link, Int_t addr);
  void FillEpochInfo(Int_t link, Int_t addr, Int_t epoch_count);
  CbmHistManager* fHM;
  TCanvas* fcB;
  TCanvas* fcM;//[(iLink)*(NrOfHalfSpadics/2)];
  TCanvas* fcH;
  TCanvas* fcL;
  TCanvas* fcE;
  TCanvas* fcO;
  TCanvas* fcS;
  TCanvas* fcI;
  TCanvas* fcTS;
  TCanvas* fcF;
  TCanvas* fcSp;
  TCanvas* fcPS[(NrOfAfcks)*(NrOfSpadics)];
  TCanvas* fcMS;
  TCanvas* fcSy;
  TCanvas* fcNS;
  TCanvas* fcTH;
  TCanvas* fcAS;
  TCanvas* fcA;
  TCanvas* fcES;
  TCanvas* fcHC;
  void InitHistos();
  void InitCanvas();
  void UpdateCanvas();
  TH2I* fBaseline[(NrOfAfcks)*(NrOfSpadics)];
  TH2I* fmaxADCmaxTimeBin[(NrOfAfcks)*(NrOfSpadics)];
  TH2I* fHit[(NrOfAfcks)*(NrOfSpadics)];
  TH2I* fHitChannel[(NrOfAfcks)*(NrOfSpadics)];
  TH2I* fLost[(NrOfAfcks)*(NrOfSpadics)];
  TH2I* fEpoch[(NrOfAfcks)*(NrOfSpadics)];
  TH2I* fOutOfSync[(NrOfAfcks)*(NrOfSpadics)];
  TH2I* fStrange[(NrOfAfcks)*(NrOfSpadics)];
  TH2I* fInfo[(NrOfAfcks)*(NrOfSpadics)];
  TH1I* fHitTimeA[(NrOfAfcks)*(NrOfSpadics)];
  TH1I* fHitTimeB[(NrOfAfcks)*(NrOfSpadics)];
  TH1I* fHitFrequency[(NrOfAfcks)*(NrOfSpadics)];
  TH1I* fSpectrum[(NrOfAfcks)*(NrOfSpadics)];
  TH2I* fPulseShape[(NrOfAfcks)*(NrOfSpadics)*32];
  TH1I* fMessageStatistic[(NrOfAfcks)*(NrOfSpadics)];
  TH2I* fHitSync[(NrOfAfcks)*(NrOfSpadics)];
  TH2I* fNrSamples[(NrOfAfcks)*(NrOfSpadics)];
  TH1D* fTimeBetweenHits[(NrOfAfcks)*(NrOfSpadics)];
  TH1I* fADCmaxSum[(NrOfAfcks)*(NrOfSpadics)];
  //  TH1I* fDeltaT[1];
  TGraph* fTSGraph[(NrOfAfcks)*(NrOfSpadics)];
  Int_t fLastSuperEpochA[(NrOfAfcks)*(NrOfSpadics)];
  Int_t fLastSuperEpochB[(NrOfAfcks)*(NrOfSpadics)];
  ULong_t fLastFullTime[NrOfAfcks][NrOfSpadics][32];
  ULong_t fHitsTrd0[16777216][2];
  ULong_t fHitsTrd1[16777216][2];
  ULong_t fHitsTrd1n[16777216][2];
  ULong_t fHitsTrd2[16777216][2];
  ULong_t fLastTimeGlobal;
  ULong_t fThisTime;
  ULong_t fAlignmentCounter0;
  ULong_t fAlignmentCounter1;
  ULong_t fAlignmentCompareCounter;
  ULong_t fAlignmentCompareCounter1;
  ULong_t fLastTimes[(NrOfAfcks)*(NrOfSpadics)];
  TString fMessageTypes[7];
  TString fTriggerTypes[4];
  Int_t fClusterSwitch[(NrOfAfcks)*(NrOfSpadics)];
  Int_t fMaxSum[(NrOfAfcks)*(NrOfSpadics)];
  Int_t fLastTrigger[(NrOfAfcks)*(NrOfSpadics)];
  Int_t fLastChannel[(NrOfAfcks)*(NrOfSpadics)];
  TString fStopTypes[6];
  TString fInfoTypes[7];
  TString fAlignments[3];
  TString fChambers[4];
  Int_t fAFCKs[4];
  Int_t fSource;
  Int_t fLastSource;
  ULong_t fFillCounter;
  //Int_t fSumHitMessages[(NrOfAfcks)*(NrOfSpadics)];
  CbmTSUnpackSpadic20DesyOnlineMonitor(const CbmTSUnpackSpadic20DesyOnlineMonitor&);
  CbmTSUnpackSpadic20DesyOnlineMonitor operator=(const CbmTSUnpackSpadic20DesyOnlineMonitor&);

  TH2I* fSyncChambers[1];
  TH1I* fEventSelection[1];
  TH2I* fAlignment[3];
  TH1D* fMaxADC[32*(NrOfAfcks)*(NrOfSpadics)];

  ClassDef(CbmTSUnpackSpadic20DesyOnlineMonitor, 2)

 };

#endif

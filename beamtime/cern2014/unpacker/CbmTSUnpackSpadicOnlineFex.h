// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           CbmTSUnpackSpadicOnlineFex        -----
// -----                    Created 10.12.2016 by C. Bergmann              -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMTSUNPACKSPADICONLINEFEX_H
#define CBMTSUNPACKSPADICONLINEFEX_H

#include "Timeslice.hpp"
#include "Message.hpp"


#include "CbmTSUnpack.h"
#include "CbmBeamDefaults.h"

#include "CbmHistManager.h"
#include "TClonesArray.h"

class CbmTSUnpackSpadicOnlineFex : public CbmTSUnpack
{
 public:
  
  CbmTSUnpackSpadicOnlineFex(Bool_t highPerformancen = true );
  virtual ~CbmTSUnpackSpadicOnlineFex();
    
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
  
  Int_t fEpochMarkerArray[NrOfSyscores][NrOfHalfSpadics];
  //Array to store the previous Epoch counter
  Int_t fPreviousEpochMarkerArray[NrOfSyscores][NrOfHalfSpadics];
  //Suppress multiple Epoch Messages in duplicated Microslices. NOTE:Currently Buggy
  const Bool_t SuppressMultipliedEpochMessages = false;
  Int_t fSuperEpochArray[NrOfSyscores][NrOfHalfSpadics];
  
  static Int_t   GetSpadicID(Int_t address);
  static Int_t   GetSyscoreID(Int_t link);
  inline TString GetSpadicName(Int_t link,Int_t address);
  Int_t GetChannelOnPadPlane(Int_t SpadicChannel, Int_t groupId);
  Int_t fEpochMarker;
  Int_t fSuperEpoch;
  Int_t fNrExtraneousSamples;
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
  TCanvas* fcPS[(NrOfSyscores)*(NrOfSpadics)];
  TCanvas* fcMS;
  void InitHistos();
  void InitCanvas();
  void UpdateCanvas();
  TH2I* fBaseline[(NrOfSyscores)*(NrOfSpadics)];
  TH2I* fmaxADCmaxTimeBin[(NrOfSyscores)*(NrOfSpadics)];
  TH2I* fHit[(NrOfSyscores)*(NrOfSpadics)];
  TH2I* fLost[(NrOfSyscores)*(NrOfSpadics)];
  TH2I* fEpoch[(NrOfSyscores)*(NrOfSpadics)];
  TH2I* fOutOfSync[(NrOfSyscores)*(NrOfSpadics)];
  TH2I* fStrange[(NrOfSyscores)*(NrOfSpadics)];
  TH2I* fInfo[(NrOfSyscores)*(NrOfSpadics)];
  TH1I* fHitTimeA[(NrOfSyscores)*(NrOfSpadics)];
  TH1I* fHitTimeB[(NrOfSyscores)*(NrOfSpadics)];
  TH1I* fHitFrequency[(NrOfSyscores)*(NrOfSpadics)];
  TH1I* fSpectrum[(NrOfSyscores)*(NrOfSpadics)];
  TH2I* fPulseShape[(NrOfSyscores)*(NrOfSpadics)*32];
  TH1I* fMessageStatistic[(NrOfSyscores)*(NrOfSpadics)];
  TGraph* fTSGraph[(NrOfSyscores)*(NrOfSpadics)];
  Int_t fLastSuperEpochA[(NrOfSyscores)*(NrOfSpadics)];
  Int_t fLastSuperEpochB[(NrOfSyscores)*(NrOfSpadics)];
  ULong_t fLastFullTime[NrOfSyscores][NrOfSpadics][32];
  TString fMessageTypes[7];
  TString fTriggerTypes[4];
  TString fStopTypes[6];
  TString fInfoTypes[7];
  //Int_t fSumHitMessages[(NrOfSyscores)*(NrOfSpadics)];
  CbmTSUnpackSpadicOnlineFex(const CbmTSUnpackSpadicOnlineFex&);
  CbmTSUnpackSpadicOnlineFex operator=(const CbmTSUnpackSpadicOnlineFex&);

  ClassDef(CbmTSUnpackSpadicOnlineFex, 2)
    };

#endif


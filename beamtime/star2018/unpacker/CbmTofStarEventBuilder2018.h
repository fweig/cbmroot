// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTofStarEventBuilder2018                 -----
// -----               Created 08.12.2017 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmTofStarEventBuilder2018_H
#define CbmTofStarEventBuilder2018_H

#include "Timeslice.hpp"
#include "rocMess_wGet4v2.h"
#include "CbmTofStarData.h"
#include "CbmTofStarData2018.h"

#include "CbmTSUnpack.h"
#include "CbmTbDaqBuffer.h"

#include "TClonesArray.h"

#include <vector>
#include <map>
#include <chrono>

class CbmTofStar2018Par;

class TCanvas;
class TH1;
class TH2;
class TProfile;

#ifdef STAR_SUBEVT_BUILDER
   /*
    ** Function to send sub-event block to the STAR DAQ system
    *       trg_word received is packed as:
    *
    *       trg_cmd|daq_cmd|tkn_hi|tkn_mid|tkn_lo
   */
   extern "C" int star_rhicf_write(unsigned int trg_word, void *dta, int bytes);
#endif // STAR_SUBEVT_BUILDER

class CbmTofStarEventBuilder2018 : public CbmTSUnpack
{
public:

   CbmTofStarEventBuilder2018( UInt_t uNbGdpb = 1 );
   virtual ~CbmTofStarEventBuilder2018();

   virtual Bool_t Init();
   virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
   virtual void Reset();

   virtual void Finish();

   void SetParContainers();

   Bool_t InitContainers();

   Bool_t ReInitContainers();

   void FillOutput(boost::any);

   // Setting change methods
   void SetMsLimitLevel( size_t uAcceptBoundaryPct = 100 ) { fuMsAcceptsPercent = uAcceptBoundaryPct; }
   size_t GetMsLimitLevel( ) { return fuMsAcceptsPercent; }

   void SetEventBuildingMode( Bool_t bEventBuildingMode = kFALSE );
   void SetTimeSortOutput( Bool_t bTimeSort = kTRUE );
   void SetEventDumpEnable( Bool_t bDumpEna = kTRUE );

   inline void SetHistoryHistoSize( UInt_t inHistorySizeSec = 1800 ) { fuHistoryHistoSize = inHistorySizeSec; }
   inline void SetHistoryHistoSizeLong( UInt_t inHistorySizeMin = 1800 ) { fuHistoryHistoSizeLong = inHistorySizeMin; }
   inline void SetPrintoutInterval( Double_t inIntervalInSec = 30.0 ) { fdMoniOutIntervalSec = inIntervalInSec; }
   inline void SetHistSaveToPrintRatio( UInt_t inSavePrintRatio = 4 ) { fuPrintToSaveRatio = inSavePrintRatio; }


   // Output control methods
   void SaveAllHistos( TString sFileName = "" );
   void ResetAllHistos();
   void ResetEvolutionHistograms();
   void ResetLongEvolutionHistograms();

private:

   size_t   fuMsAcceptsPercent; /** Reject Ms with index inside TS above this, assumes 100 MS per TS **/
   size_t   fuTotalMsNb;        /** Total nb of MS per link in timeslice **/
   size_t   fuOverlapMsNb;      /** Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
   size_t   fuCoreMs;           /** Number of non overlap MS at beginning of TS **/
   Double_t fdMsSizeInNs;
   Double_t fdTsCoreSizeInNs;
   UInt_t fuMinNbGdpb;
   UInt_t fuCurrNbGdpb;

   /** Settings from parameter file **/
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

   std::vector< Double_t >  fdStarTriggerDeadtime;
   std::vector< Double_t >  fdStarTriggerDelay;
   std::vector< Double_t >  fdStarTriggerWinSize;
   Double_t                 fdTsDeadtimePeriod;

   /** Running indices **/
   uint64_t fulCurrentTsIndex;  // Idx of the current TS
   size_t fuCurrentMs; // Idx of the current MS in TS (0 to fuTotalMsNb)
   UInt_t fuGdpbId;    // Id (hex number) of the GDPB for current message
   UInt_t fuGdpbNr;    // running number (0 to fNrOfGdpbs) of the GDPB for current message
   UInt_t fuGet4Id;    // running number (0 to fNrOfGet4PerGdpb) of the Get4 chip of a unique GDPB for current message
   UInt_t fuGet4Nr;    // running number (0 to fNrOfGet4) of the Get4 chip in the system for current message

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
   Double_t fdStartTime; /** Time of first valid hit (epoch available), used as reference for evolution plots**/
   Double_t fdStartTimeLong; /** Time of first valid hit (epoch available), used as reference for evolution plots**/
   Double_t fdStartTimeMsSz; /** Time of first microslice, used as reference for evolution plots**/
   TCanvas* fcMsSizeAll;

   Int_t fEquipmentId;
   Double_t fdMsIndex;

   CbmTofStar2018Par* fUnpackPar;      //!

   void CreateHistograms();

   /// Variables used for histo filling
   UInt_t            fuHistoryHistoSize;
   UInt_t            fuHistoryHistoSizeLong;

   /// Variables used for monitoring output control
   Double_t          fdMoniOutIntervalSec;
   UInt_t            fuPrintToSaveRatio;
   UInt_t            fuPrintToSaveCount;

   ///* STAR TRIGGER detection *///
   std::vector< ULong64_t > fulGdpbTsMsb;
   std::vector< ULong64_t > fulGdpbTsLsb;
   std::vector< ULong64_t > fulStarTsMsb;
   std::vector< ULong64_t > fulStarTsMid;
   std::vector< ULong64_t > fulGdpbTsFullLast;
   std::vector< ULong64_t > fulStarTsFullLast;
   std::vector< UInt_t    > fuStarTokenLast;
   std::vector< UInt_t    > fuStarDaqCmdLast;
   std::vector< UInt_t    > fuStarTrigCmdLast;
   std::vector< ULong64_t > fulGdpbTsFullLastCore;
   std::vector< ULong64_t > fulStarTsFullLastCore;
   std::vector< UInt_t    > fuStarTokenLastCore;
   std::vector< UInt_t    > fuStarDaqCmdLastCore;
   std::vector< UInt_t    > fuStarTrigCmdLastCore;
   std::vector< TH1 *     > fhTokenMsgType;
   std::vector< TH1 *     > fhTriggerRate;
   std::vector< TH1 *     > fhTriggerRateLong;
   std::vector< TH2 *     > fhCmdDaqVsTrig;
   std::vector< TH2 *     > fhStarTokenEvo;
   std::vector< TProfile * > fhStarTrigGdpbTsEvo;
   std::vector< TProfile * > fhStarTrigStarTsEvo;

   ///* STAR event building *///
   Bool_t fbEventBuilding; //! If false => build subevents in each MS => M1, if true => store data for full TS, then build subevents in TS => M2
   Bool_t fbTimeSortOutput;
   CbmTofStarSubevent2018 fStarSubEvent;
   ULong64_t fulNbBuiltSubEvent;
   ULong64_t fulNbStarSubEvent;
   ULong64_t fulNbBuiltSubEventLastPrintout;
   ULong64_t fulNbStarSubEventLastPrintout;
   std::chrono::time_point<std::chrono::system_clock> fTimeLastPrintoutNbStarEvent;
   Double_t fdCurrentMsStartTime;                          //! M1, Used in case of single link per subevent: test mode in 2018 S1, sector mode in 2018 S2
   Double_t fdCurrentMsEndTime;                            //! M1, Used in case of single link per subevent: test mode in 2018 S1, sector mode in 2018 S2
   std::vector< gdpb::FullMessage > fvmCurrentLinkBuffer;  //! M1, Used in case of single link per subevent: test mode in 2018 S1, sector mode in 2018 S2
   std::vector< CbmTofStarTrigger2018  > fvtCurrentLinkBuffer; //! M1, Used in case of single link per subevent: test mode in 2018 S1, sector mode in 2018 S2
   Double_t fdCurrentTsStartTime;                                      //! M2, Used in case of all links in same subevent: Sector mode in 2018 S1, Full eTOF mode in 2018 S2
   Double_t fdCurrentTsCoreEndTime;                                    //! M2, Used in case of all links in same subevent: Sector mode in 2018 S1, Full eTOF mode in 2018 S2
   std::vector< std::vector < gdpb::FullMessage > >  fvmTsLinksBuffer; //! M2, Used in case of all links in same subevent: Sector mode in 2018 S1, Full eTOF mode in 2018 S2
   std::vector< std::vector < CbmTofStarTrigger2018  > > fvtTsLinksBuffer; //! M2, Used in case of all links in same subevent: Sector mode in 2018 S1, Full eTOF mode in 2018 S2
   std::vector< std::vector < gdpb::FullMessage > >  fvmTsOverLinksBuffer; //! M2, Used in case of all links in same subevent: Sector mode in 2018 S1, Full eTOF mode in 2018 S2
   std::vector< std::vector < CbmTofStarTrigger2018  > > fvtTsOverLinksBuffer; //! M2, Used in case of all links in same subevent: Sector mode in 2018 S1, Full eTOF mode in 2018 S2
   std::vector<TH1*> fhStarHitToTrigAll_gDPB;
   std::vector<TH1*> fhStarHitToTrigWin_gDPB;
   std::vector<TH2*> fhStarHitToTrigAllTime_gDPB;
   std::vector<TH2*> fhStarHitToTrigWinTime_gDPB;
   std::vector<TH2*> fhStarHitToTrigAllTimeLong_gDPB;
   std::vector<TH2*> fhStarHitToTrigWinTimeLong_gDPB;
   std::vector<TH1*> fhStarEventSize_gDPB;
   std::vector<TH2*> fhStarEventSizeTime_gDPB;
   std::vector<TH2*> fhStarEventSizeTimeLong_gDPB;
   std::vector<TH1*> fhStarTrigTimeToMeanTrig_gDPB;
   TH1 * fhStarEventSize;
   TH2 * fhStarEventSizeTime;
   TH2 * fhStarEventSizeTimeLong;

   std::chrono::time_point<std::chrono::system_clock> fStartTimeProcessingLastTs;
   TH2 * fhStarTsProcessTime;
   TH2 * fhStarTsProcessTimeShort;
   TH1 * fhStarTsInterprocessTime;

   std::vector< std::vector < gdpb::Message > > fvmEpSupprBuffer;

   ///* Event dump to binary file *///
   Bool_t fbEventDumpEna;
   std::fstream * fpBinDumpFile;
   const UInt_t   kuBinDumpBegWord = 0xFEEDBEAF;
   const UInt_t   kuBinDumpEndWord = 0xFAEBDEEF;

   void FillHitInfo(gdpb::Message);
   void FillStarTrigInfo(gdpb::Message);
   void FillEpochInfo(gdpb::Message);
   void PrintSlcInfo(gdpb::Message);
   void PrintSysInfo(gdpb::Message);
   void PrintGenInfo(gdpb::Message);


   inline Int_t GetArrayIndex(Int_t gdpbId, Int_t get4Id)
   {
      return gdpbId * fuNrOfGet4PerGdpb + get4Id;
   }

   void BuildStarEventsSingleLink();
   void BuildStarEventsAllLinks();

   CbmTofStarEventBuilder2018(const CbmTofStarEventBuilder2018&);
   CbmTofStarEventBuilder2018 operator=(const CbmTofStarEventBuilder2018&);

   ClassDef(CbmTofStarEventBuilder2018, 1)
};

#endif

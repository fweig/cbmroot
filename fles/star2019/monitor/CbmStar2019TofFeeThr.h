// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmStar2019TofFeeThr                          -----
// -----               Created 10.07.2018 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmStar2019TofFeeThr_H
#define CbmStar2019TofFeeThr_H

#include "Timeslice.hpp"
#include "gDpbMessv100.h"
//#include "CbmTofStarData.h"
//#include "CbmTofStarData2018.h"

#include "CbmMcbmUnpack.h"

#include "TClonesArray.h"
#include "Rtypes.h"

#include <vector>
#include <map>
#include <chrono>

class CbmStar2019TofPar;

class TCanvas;
class TH1;
class TH2;
class TProfile;
class TProfile2D;

class CbmStar2019TofFeeThr: public CbmMcbmUnpack {
   public:

      CbmStar2019TofFeeThr();
      virtual ~CbmStar2019TofFeeThr();

      virtual Bool_t Init();

      virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

      virtual void Reset();

      virtual void Finish();

      void SetParContainers();

      Bool_t InitContainers();

      Bool_t ReInitContainers();

      void SetMsLimitLevel(size_t uAcceptBoundaryPct = 100) { fuMsAcceptsPercent = uAcceptBoundaryPct; }
      size_t GetMsLimitLevel() { return fuMsAcceptsPercent; }

      virtual void AddMsComponentToList( size_t component, UShort_t usDetectorId );
      virtual void SetNbMsInTs( size_t uCoreMsNb, size_t uOverlapMsNb );
      void SetIgnoreMsOverlap( Bool_t bEnaFlag = kTRUE ) { fbIgnoreOverlapMs = bEnaFlag; }
      void SetMsOverlap(size_t uOverlapMsNb = 1) { fuOverlapMsNb = uOverlapMsNb; }
      size_t GetMsOverlap() { return fuOverlapMsNb; }

      void SaveAllHistos( TString sFileName = "" );
      void ResetAllHistos();

      void SetHistoFileName( TString sFileName = "data/HistosTofFeeTest.root" ) { fsHistoFileFullname = sFileName; }

      void UseDaqBuffer(Bool_t) {};
   private:
      /// FLES containers
      std::vector< size_t >    fvMsComponentsList; //!
      size_t                   fuNbCoreMsPerTs; //!
      size_t                   fuNbOverMsPerTs; //!
      Bool_t                   fbIgnoreOverlapMs; //! /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/

      /// Histo File name and path
      TString fsHistoFileFullname;

      /// OLD, to be cleaned out !!!!!
      size_t   fuMsAcceptsPercent; /** Reject Ms with index inside TS above this, assumes 100 MS per TS **/
      size_t   fuTotalMsNb;        /** Total nb of MS per link in timeslice **/
      size_t   fuOverlapMsNb;      /** Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
      size_t   fuCoreMs;           /** Number of non overlap MS at beginning of TS **/
      Double_t fdMsSizeInNs;
      Double_t fdTsCoreSizeInNs;
      UInt_t   fuMinNbGdpb;
      UInt_t   fuCurrNbGdpb;

      /** Settings from parameter file **/
      CbmStar2019TofPar* fUnpackPar;      //!
      UInt_t fuNrOfGdpbs;           // Total number of GDPBs in the system
      UInt_t fuNrOfFeePerGdpb;      // Number of FEBs per GDPB
      UInt_t fuNrOfGet4PerFee;      // Number of GET4s per FEE
      UInt_t fuNrOfChannelsPerGet4; // Number of channels in each GET4

      UInt_t fuNrOfChannelsPerFee;  // Number of channels in each FEE
      UInt_t fuNrOfGet4;            // Total number of Get4 chips in the system
      UInt_t fuNrOfGet4PerGdpb;     // Number of GET4s per GDPB
      UInt_t fuNrOfChannelsPerGdpb; // Number of channels per GDPB

      UInt_t fuNrOfGbtx;

      const UInt_t kuNbFeePerGbtx  = 5;
      const UInt_t kuNbGbtxPerGdpb = 6;

      /// Map of ID to index for the gDPBs
      std::map<UInt_t, UInt_t> fGdpbIdIndexMap;

      /** Running indices **/
      uint64_t fulCurrentTsIndex;  // Idx of the current TS
      size_t   fuCurrentMs; // Idx of the current MS in TS (0 to fuTotalMsNb)
      size_t   fuCurrentMsSysId; // SysId of the current MS in TS (0 to fuTotalMsNb)
      Double_t fdMsIndex;   // Time in ns of current MS from its index
      UInt_t   fuGdpbId;    // Id (hex number) of the GDPB for current message
      UInt_t   fuGdpbNr;    // running number (0 to fuNrOfGdpbs) of the GDPB for current message
      UInt_t   fuGet4Id;    // running number (0 to fuNrOfGet4PerGdpb) of the Get4 chip of a unique GDPB for current message
      UInt_t   fuGet4Nr;    // running number (0 to fuNrOfGet4) of the Get4 chip in the system for current message
      Int_t    fiEquipmentId;
      std::vector<int> fviMsgCounter;

      /// Histograms
      TH2 * fhGdpbAsicSpiCounts; //!

      void CreateHistograms();

      void PrintSlcInfo(      gdpbv100::Message );

      inline Int_t GetArrayIndex(Int_t gdpbId, Int_t get4Id)
      {
         return gdpbId * fuNrOfGet4PerGdpb + get4Id;
      }

      ///* PADI channel to GET4 channel mapping and reverse *///
      std::vector< UInt_t > fvuPadiToGet4;
      std::vector< UInt_t > fvuGet4ToPadi;

      ///* GET4 to eLink mapping and reverse *///
      static const UInt_t kuNbGet4PerGbtx = 5 * 8; /// 5 FEE with 8 GET4 each
      std::vector< UInt_t > fvuElinkToGet4;
      std::vector< UInt_t > fvuGet4ToElink;
      inline UInt_t ConvertElinkToGet4( UInt_t uElinkIdx )
      {
         return fvuElinkToGet4[ uElinkIdx % kuNbGet4PerGbtx ] + kuNbGet4PerGbtx * (uElinkIdx / kuNbGet4PerGbtx);
      }
      inline UInt_t ConvertGet4ToElink( UInt_t uGet4Idx )
      {
         return fvuGet4ToElink[ uGet4Idx % kuNbGet4PerGbtx ] + kuNbGet4PerGbtx * (uGet4Idx / kuNbGet4PerGbtx);
      }

      CbmStar2019TofFeeThr(const CbmStar2019TofFeeThr&);
      CbmStar2019TofFeeThr operator=(const CbmStar2019TofFeeThr&);

   ClassDef(CbmStar2019TofFeeThr, 1)
};

#endif

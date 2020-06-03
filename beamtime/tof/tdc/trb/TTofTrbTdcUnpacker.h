// -----------------------------------------------------------------------------
// ----- TTofTrbTdcUnpacker header file                                    -----
// -----                                                                   -----
// ----- created by C. Simon on 2014-04-02                                 -----
// -----                                                                   -----
// ----- based on TTofVftxUnpacker by P.-A. Loizeau                        -----
// ----- https://subversion.gsi.de/fairroot/cbmroot/development/ploizeau/  -----
// -----   main/unpack/tof/tdc/vftx/TTofVftxUnpacker.h                     -----
// ----- revision 20754, 2013-07-17                                        -----
// -----------------------------------------------------------------------------

#ifndef TTOFTRBTDCUNPACKER_H_
#define TTOFTRBTDCUNPACKER_H_

#include "TObject.h"

#include <vector>
#include <map>

class TMbsUnpackTofPar;
class TMbsCalibTofPar;

namespace hadaq
{
  struct RawSubevent;
}
/*
namespace trbtdc
{
  enum TrbTdcProcessStatus;
}
*/

class TClonesArray;
class TH1I;
class TH1D;
class TH2I;
class TDirectory;

class TTofTrbTdcUnpacker : public TObject
{
   public:
      TTofTrbTdcUnpacker( TMbsUnpackTofPar * parUnpackIn, TMbsCalibTofPar * parCalibIn );
      ~TTofTrbTdcUnpacker();

      // unsure if needed
      // never called in the unpacker
      void Clear(Option_t *option);

      Int_t ProcessData( hadaq::RawSubevent* tSubevent, UInt_t uStartIndex );

      void SetCalibTrigger( UInt_t uTriggerType ) { fbCalibTrigger = (0xd == uTriggerType); };
      static void SetInspection( Bool_t bInspection ) { fbInspection = bInspection;}

      void CreateHistos();
      void FillHistos();
      void WriteHistos( TDirectory* inDir);
      void DeleteHistos();

   private:   
      // no default Copy constructor and = OP as class not meant to be copied
      TTofTrbTdcUnpacker(const TTofTrbTdcUnpacker&);
      TTofTrbTdcUnpacker& operator=(const TTofTrbTdcUnpacker&);

      TMbsUnpackTofPar * fParUnpack;
      TMbsCalibTofPar * fParCalib;
      UInt_t   fuNbActiveTrbTdc;
      Bool_t fbCalibTrigger;
      static Bool_t fbInspection;
      Bool_t fbJointEdgesChannel;
      TClonesArray * fTrbTdcBoardCollection;

      // TDC histograms
      TH1I* fTrbTdcRingBufferOverflow;
      static std::map<Int_t,Bool_t> fmRingBufferOverflow;
      static Int_t fiAcceptedHits;
      static Int_t fiAvailableHits;

      std::vector<TH1D*> fLeadingOnlyShares;
      std::vector<TH1D*> fTrailingOnlyShares;
      std::vector<TH1D*> fUnequalEdgesShares;

      std::vector< std::vector<Int_t> > fiLeadingOnlyBuffers;
      std::vector< std::vector<Int_t> > fiTrailingOnlyBuffers;
      std::vector< std::vector<Int_t> > fiUnequalEdgesBuffers;
      std::vector< std::vector<Int_t> > fiAllNonEmptyBuffers;

      std::vector<TH2I*> fTrbTdcBoardTot;
      std::vector<TH2I*> fTrbTdcLeadingPosition;
      std::vector<TH2I*> fTrbTdcTrailingPosition;
      TH2I* fTrbTdcRefChannelFineTime;
      std::vector<TH2I*> fTrbTdcBoardFineTime;

      // TDC board histograms
      // channel occupancy distribution
      std::vector<TH1I*> fTrbTdcChannelOccupancy;

/*
      // channel fine time overshoot distribution
      std::vector<TH1I*> fTrbTdcChannelFineTimeOvershoot;
      // channel undetected hits distribution
      std::vector<TH1I*> fTrbTdcChannelUnprocessedHits;
*/

      // TDC channel histograms
      // channel fine time distribution
      std::vector< std::vector<TH1I*> > fTrbTdcChannelFineTime;

/*
      // channel coarse time distribution
      std::vector< std::vector<TH1I*> > fTrbTdcChannelCoarseTime;
*/
      std::vector< std::vector<TH1I*> > fTrbTdcChannelToT;

      UInt_t fuEventIndex;
      
   ClassDef(TTofTrbTdcUnpacker, 1)
};

#endif

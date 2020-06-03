// -------------------------------------------------------------------------
// -----                 CbmFHodoUnpackPar header file                 -----
// -----                 Created 02/11/16  by F. Uhlig                 -----
// -------------------------------------------------------------------------

#ifndef CBMTOFUNPACKPAR_H
#define CBMTOFUNPACKPAR_H

#include "FairParGenericSet.h"

#include "TArrayI.h"
#include "TArrayD.h"

class FairParIo;
class FairParamList;


class CbmTofUnpackPar : public FairParGenericSet 
{

 public:

  /** Standard constructor **/
  CbmTofUnpackPar(const char* name      = "CbmTofUnpackPar",
		    const char* title   = "Tof unpacker parameters",
		    const char* context = "Default");


  /** Destructor **/
  virtual ~CbmTofUnpackPar();
  
  /** Reset all parameters **/
  virtual void clear();

  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);

  inline Int_t GetNrOfRocs() { return fNrOfGdpb; }
  inline Int_t GetRocId(Int_t i) { return fGdpbIdArray[i]; }
  inline Int_t GetNrOfFebsPerGdpb() { return fNrOfFebsPerGdpb; }
  inline Int_t GetNrOfGet4PerFeb() {return fNrOfGet4PerFeb;}
  inline Int_t GetNrOfChannelsPerGet4() {return fNrOfChannelsPerGet4;}
  inline Int_t GetNumberOfChannels() {return fNrOfChannels;}
  inline Int_t GetChannelToDetUIdMap(Int_t channel) {return fChannelToDetUIdMap[channel];}
  inline Bool_t IsChannelRateEnabled() {return (1 == fPlotChannelRate);}
  inline Bool_t IsStarSortAndCutEnabled() {return (1 == fiStarSortAndCut);}
  inline UInt_t GetStarActiveMask(Int_t gdpb) { return (IsStarSortAndCutEnabled() ? static_cast<UInt_t>(fiStarActiveMasksArray[gdpb]) : 0); }
  inline Double_t GetStarTriggDelay(Int_t gdpb) { return (IsStarSortAndCutEnabled() ? fdStarTriggerDelay[gdpb] : 0.0);}
  inline Double_t GetStarTriggWinSize(Int_t gdpb) { return (IsStarSortAndCutEnabled() ? fdStarTriggerWinSize[gdpb] : 0.0);}

 private:

  Int_t   fNrOfGdpb; // Total number of GDPBs
  TArrayI fGdpbIdArray; // Array to hold the unique IDs for all Tof GDPBs

  Int_t   fNrOfFebsPerGdpb; // Number of FEBs which are connected to one GDPB
  Int_t   fNrOfGet4PerFeb;  // Number of GET4 chips which are connected to one FEB
  Int_t   fNrOfChannelsPerGet4;  // Number of channels per GET4

  Int_t   fNrOfChannels; // Total number of Channels
  TArrayI fChannelToDetUIdMap;// Array which stores the corresponding UIDs for each channel
  
  Int_t   fPlotChannelRate; // Flag controlling wether rate plots for individual channels are filled
  
  Int_t   fiStarSortAndCut; // STAR: Flag controlling wether data are time sorted and cut relative to a received STAR trigger
  TArrayI fiStarActiveMasksArray; // STAR: Array to hold the GET4s masks for each gDPB, used to wait for epoch index
  TArrayD fdStarTriggerDelay; // STAR: Array to hold for each gDPB the Delay in us to subtract when looking for beginning of coincidence of data with trigger window
  TArrayD fdStarTriggerWinSize; // STAR: Array to hold for each gDPB the Size of the trigger window in us

  ClassDef(CbmTofUnpackPar,1);
};
#endif

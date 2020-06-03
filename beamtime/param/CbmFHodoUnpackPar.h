// -------------------------------------------------------------------------
// -----                 CbmFHodoUnpackPar header file                 -----
// -----                 Created 02/11/16  by F. Uhlig                 -----
// -------------------------------------------------------------------------

#ifndef CBMFHODOUNPACKPAR_H
#define CBMFHODOUNPACKPAR_H

#include "FairParGenericSet.h"

#include "TArrayI.h"

class FairParIo;
class FairParamList;


class CbmFHodoUnpackPar : public FairParGenericSet 
{

 public:

  /** Standard constructor **/
  CbmFHodoUnpackPar(const char* name    = "CbmFHodoUnpackPar",
		    const char* title   = "FHodo unpacker parameters",
		    const char* context = "Default");


  /** Destructor **/
  virtual ~CbmFHodoUnpackPar();
  
  /** Reset all parameters **/
  virtual void clear();

  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);

  Int_t GetNrOfRocs() { return fNrOfRocs; }
  Int_t GetRocId(Int_t i) { return fRocIdArray[i]; }
  Int_t GetNumberOfChannels() {return fNrOfChannels;}
  Int_t GetChannelToFiberMap(Int_t channel) {return fChannelToFiberMap[channel];}
  Int_t GetChannelToPixelMap(Int_t channel) {return fChannelToPixelMap[channel];}
  Int_t GetChannelToPlaneMap(Int_t channel) {return fChannelToPlaneMap[channel];}
 private:

  Int_t fNrOfRocs; // Total number of Rocs
  TArrayI fRocIdArray; // Array to hold the unique IDs for all FHodo Rocs

  Int_t fNrOfChannels;
  TArrayI fChannelToFiberMap; // Array which stores the corresponding fiber for each channel
  TArrayI fChannelToPixelMap; // Array which stores the corresponding pixel for each channel
  TArrayI fChannelToPlaneMap; // Array which stores the corresponding side for each channel

  ClassDef(CbmFHodoUnpackPar,1);
};
#endif

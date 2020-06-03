#ifndef CBMTRDTESTBEAMTOOLS2017DESY_H
#define CBMTRDTESTBEAMTOOLS2017DESY_H

#include "CbmTrdTestBeamTools.h"
#include "FairLogger.h"

class CbmTrdTestBeamTools2017DESY: public CbmTrdTestBeamTools
{
  public:
 CbmTrdTestBeamTools2017DESY();
  // CbmTrdTestBeamTools2017DESY():CbmTrdTestBeamTools2017DESY("CbmTrdTestBeamTools2017DESY"){};
 ~CbmTrdTestBeamTools2017DESY(){};

 Float_t GetSamplingTime(){
    /*
     * Get inverse sampling frequency in nano seconds.
     */
   return 1E3/16;
  };
  Int_t GetShapingTime(){
    /*
     * Get shaping time in nano seconds.
     */
    return 240;
  };
  virtual Int_t GetNrLayers(){
    return 8;
  };
  virtual Int_t GetNrRows(Int_t Layer=0){
    //Get the Number of PadRows in the corresponding Layer
    if (Layer <4)
      return 6;
    else if(Layer <6)
      return 2;
    else
      return 1;
  };
  virtual Int_t GetNrColumns(Int_t Layer=0){
    //Get the Number of active Columns in the corresponding Layer
    if (Layer <4)
      return 16*8;
    else if(Layer <6)
      return 16;
    else
      return 1;
  }
  virtual Int_t GetNrRobs(){
    //Get the Number of active ROBS in the Setup;
    return 4;
  };
  virtual Int_t GetNrSpadics(Int_t = 0){
    return 2;
  }
  virtual Int_t GetRowID(CbmSpadicRawMessage* raw){
    /*Get row of the active channel.
     *Upper row is row 1.
     */
    Int_t Offset=0;
    Int_t SpadicId=GetSpadicID(raw);
    if(SpadicId/2==0)
      {
	Offset=2;
      }
    else if(GetLayerID(raw)>5)
      {
	return 0;
      }
    return GetChannelOnPadPlane(raw)/16+Offset;
  };
  virtual Int_t GetRobID(CbmSpadicRawMessage* raw){
    return raw->GetEquipmentID();    //blubb
  }
  virtual Int_t GetSpadicID(CbmSpadicRawMessage* raw){
    return raw->GetSourceAddress(); //blubb
  }
  virtual Int_t GetColumnID(CbmSpadicRawMessage* raw){
    /*Get column of the active channel.
     *With view towards the target, column 0 is on the left side.
     *Lower numbered colums are towards the left side of the module.
     */
    Int_t Offset=0;
    if (GetSpadicID(raw)/2==0)
      {
	Offset=4*16;
      }
    Int_t ColID=(GetChannelOnPadPlane(raw))%16+Offset;
    return ColID;
  };
  virtual Int_t GetLayerID(CbmSpadicRawMessage* raw)
  {
    Int_t SpadicId =GetSpadicID(raw);
    Int_t AfckId=GetRobID(raw);
    if (SpadicId/2==0)
      return AfckId;
    else{
      switch(AfckId){
      case 0: return 0;
      case 1: return 5;
      case 2: break; //return 6;
      case 3: return 6;
      }
    }
    LOG (ERROR) << this->GetName()<< ": Unknown SPADIC on Afck "<< AfckId << "SPADICID "<< SpadicId<<". Unable to find layer.";
    return -1;
  };
  
  CbmTrdTestBeamTools2017DESY(const CbmTrdTestBeamTools2017DESY&);
  CbmTrdTestBeamTools2017DESY operator=(const CbmTrdTestBeamTools2017DESY&);
 public:
  
  ClassDef(CbmTrdTestBeamTools2017DESY,1);
};

#endif

#ifndef CBMTRDTESTBEAMTOOLS2017GIF_H
#define CBMTRDTESTBEAMTOOLS2017GIF_H

#include "CbmTrdTestBeamTools.h"
#include "FairLogger.h"

class CbmTrdTestBeamTools2017GIF: public CbmTrdTestBeamTools
{
  public:
 CbmTrdTestBeamTools2017GIF();
  // CbmTrdTestBeamTools2017GIF():CbmTrdTestBeamTools2017GIF("CbmTrdTestBeamTools2017GIF"){};
 ~CbmTrdTestBeamTools2017GIF(){};

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
    return 3;
  };
  virtual Int_t GetNrRows(Int_t Layer=0){
    //Get the Number of PadRows in the corresponding Layer
    if (Layer <2)
      return 1;
    else
      return 2;
  };
  virtual Int_t GetNrColumns(Int_t Layer=0){
    //Get the Number of active Columns in the corresponding Layer
    if (Layer <2)
          return 1;
        else
          return 4*16;
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
//    Int_t SpadicId=GetSpadicID(raw);
    if(GetLayerID(raw)<2)
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
    if(GetSpadicID(raw)>1)
      return 0;
    Offset=GetRobID(raw)*16;
    Int_t ColID=(GetChannelOnPadPlane(raw))%16+Offset;
    return ColID;
  };
  virtual Int_t GetLayerID(CbmSpadicRawMessage* raw)
  {
    Int_t SpadicId =GetSpadicID(raw);
    Int_t AfckId=GetRobID(raw);
    Int_t ChannelId=GetChannelOnPadPlane(raw);
    if (SpadicId/2==0)
      return 2;
    else{
      switch(ChannelId){
      case 31: return 0;
      case 27: return 1;
      }
    }
    LOG (ERROR) << this->GetName()<< ": Unknown SPADIC on Afck "<< AfckId << "SPADICID "<< SpadicId<<". Unable to find layer.";
    return -1;
  };
  
  CbmTrdTestBeamTools2017GIF(const CbmTrdTestBeamTools2017GIF&);
  CbmTrdTestBeamTools2017GIF operator=(const CbmTrdTestBeamTools2017GIF&);
 public:
  
  ClassDef(CbmTrdTestBeamTools2017GIF,1);
};

#endif

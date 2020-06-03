#ifndef CBMTRDTESTBEAMTOOLS2018GIF_H
#define CBMTRDTESTBEAMTOOLS2018GIF_H

#include "CbmTrdTestBeamTools.h"
#include "FairLogger.h"

class CbmTrdTestBeamTools2018GIF: public CbmTrdTestBeamTools
{
  public:
 CbmTrdTestBeamTools2018GIF();
  // CbmTrdTestBeamTools2018GIF():CbmTrdTestBeamTools2018GIF("CbmTrdTestBeamTools2018GIF"){};
 ~CbmTrdTestBeamTools2018GIF(){};

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
  virtual Int_t GetNrRows(Int_t /*Layer*/=0){
    //Get the Number of PadRows in the corresponding Layer
    return 2;
  };
  virtual Int_t GetNrColumns(Int_t /*Layer*/=0){
    //Get the Number of active Columns in the corresponding Layer
    return 16;
  }
  virtual Int_t GetNrRobs(){
    //Get the Number of active ROBS in the Setup;
    return 1;
  };
  virtual Int_t GetNrSpadics(Int_t = 0){
    return 3;
  }
  virtual Int_t GetRowID(CbmSpadicRawMessage* raw){
    /*Get row of the active channel.
     *Upper row is row 1.
     */
    Int_t Offset=0;
//    Int_t SpadicId=GetSpadicID(raw);
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
    Int_t ColID=(GetChannelOnPadPlane(raw))%16+Offset;
    return ColID;
  };
  /**Get layer of current message. 
   *
   * Returns the layer of the current message. In GIF2018, the third 
   * Spadic was a Lemo FEB with scintillator / coincidence signals 
   * connected. Here, all signals of this Spadic are assigned to one 
   * layer. 
   */
  virtual Int_t GetLayerID(CbmSpadicRawMessage* raw)
  {
    Int_t SpadicId =GetSpadicID(raw);
    //    Int_t AfckId=GetRobID(raw);
    //    Int_t ChannelId=GetChannelOnPadPlane(raw);
    return SpadicId/2;
    //    LOG (error) << this->GetName()<< ": Unknown SPADIC on Afck "<< AfckId << "SPADICID "<< SpadicId<<". Unable to find layer.";
    //    return -1;
  };
  
  CbmTrdTestBeamTools2018GIF(const CbmTrdTestBeamTools2018GIF&);
  CbmTrdTestBeamTools2018GIF operator=(const CbmTrdTestBeamTools2018GIF&);
 public:
  
  ClassDef(CbmTrdTestBeamTools2018GIF,1);
};

#endif

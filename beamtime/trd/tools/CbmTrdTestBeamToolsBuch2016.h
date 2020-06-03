#ifndef CBMTRDLABTOOLSBUCH2016_H
#define CBMTRDLABTOOLSBUCH2016_H

#include "CbmTrdTestBeamTools.h"

class CbmTrdTestBeamToolsBuch2016: public CbmTrdTestBeamTools
{
  public:
	CbmTrdTestBeamToolsBuch2016();
  // CbmTrdLabTools():CbmTrdLabTools("CbmTrdBeamtimeTools"){};
 ~CbmTrdTestBeamToolsBuch2016(){};

 //virtual Int_t GetBaseEquipmentID(){
 //  //Get the BaseEquipmentID
 //  return 0xE001;
 // };

  virtual Int_t GetNrRobs(){
    return 1;
  };

 /* virtual Int_t GetNrRows(){
     //Get the Number of active Rows in the corresponding Layer

	 return 1;
   };

  virtual Int_t GetColumnID(CbmSpadicRawMessage* raw){
	  // of course we have 32 columns
	  Int_t ColID=GetChannelOnPadPlane(raw);
	      return ColID;
  };

  virtual Int_t GetRowID(CbmSpadicRawMessage* raw){
	  //for the Bucharest Padplane there is only one row
	 return 0;
  };*/

  virtual Int_t GetNrSpadics(Int_t = 0){
    return 1;
  }
  virtual Int_t GetLayerID(CbmSpadicRawMessage* /*raw*/)
  {
    return 0;
  };
  
  virtual std::vector<Int_t> GetChannelMap(CbmSpadicRawMessage* /*raw*/){
    /*
     * Get the channel mapping that is valid for the spadic where raw originated.
     */
    std::vector<Int_t> map = {{31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,
    		15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0}};
    return map;
  };

  CbmTrdTestBeamToolsBuch2016(const CbmTrdTestBeamToolsBuch2016&);
  CbmTrdTestBeamToolsBuch2016 operator=(const CbmTrdTestBeamToolsBuch2016&);
 public:
  
  ClassDef(CbmTrdTestBeamToolsBuch2016,1);
};

#endif

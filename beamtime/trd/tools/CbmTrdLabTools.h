#ifndef CBMTRDLABTOOLS_H
#define CBMTRDLABTOOLS_H

#include "CbmTrdTestBeamTools.h"

class CbmTrdLabTools: public CbmTrdTestBeamTools
{
  public:
 CbmTrdLabTools();
  // CbmTrdLabTools():CbmTrdLabTools("CbmTrdBeamtimeTools"){};
 ~CbmTrdLabTools(){};


  virtual Int_t GetNrRobs(){
    return 1;
  };
  virtual Int_t GetNrSpadics(Int_t = 0){
    return 1;
  }
  virtual Int_t GetLayerID(CbmSpadicRawMessage* /*raw*/)
  {
    return 0;
  };
  
  CbmTrdLabTools(const CbmTrdLabTools&);
  CbmTrdLabTools operator=(const CbmTrdLabTools&);
 public:
  
  ClassDef(CbmTrdLabTools,1);
};

#endif

// ------------------------------------------------------------------------
// -----                  CbmMvdSensorTask header file              -----
// -----                   Created 02/02/12  by M. Deveaux            -----
// ------------------------------------------------------------------------

/**  CbmMvdSensorReadoutTask.h
 **
 **
 ** class for the readout plugins of the MVD sensor
 **
 **/

#ifndef CBMMVDSENSORREADOUTTASK_H
#define CBMMVDSENSORREADOUTTASK_H 1

#include "TObject.h"
#include "CbmMvdSensorTask.h"
#include "CbmMvdDigi.h"
#include <iostream>


class TClonesArray;
class CbmMvdSensor;

class CbmMvdSensorReadoutTask : public CbmMvdSensorTask
{

 public:

  /** Default constructor **/
     CbmMvdSensorReadoutTask();
     CbmMvdSensorReadoutTask(const char* name);

  /** Destructor **/
  virtual ~CbmMvdSensorReadoutTask();

    /** fill buffer **/
  void SetInputArray (TClonesArray* inputStream);
  void SetInputDigi (CbmMvdDigi* digi);

  virtual TClonesArray* GetOutputArray() {return fOutputBuffer;};

  void		InitTask(CbmMvdSensor* mySensor);

  void 		Exec();
  void 		Finish();
  void          ExecChain();

  void          Reset();

  private:

  static const Int_t maxBanks = 64;

  TClonesArray* fInputBuffer;     // Buffer of background events
  TClonesArray* fOutputBuffer;
  CbmMvdSensor* fSensor;

  Int_t fSensorBanks[maxBanks];
  const Int_t fPixelsPerBank = 22;

  Int_t GetBankNumber(const Int_t &yPixelNr) const;

  CbmMvdSensorReadoutTask(const CbmMvdSensorReadoutTask&);
  CbmMvdSensorReadoutTask operator=(const CbmMvdSensorReadoutTask&);
    
  ClassDef(CbmMvdSensorReadoutTask,1);

};


#endif

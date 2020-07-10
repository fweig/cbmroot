/**
 * CbmDeviceStsLocalReco.h
 *
 * @since 2019-08-28
 * @author F. Uhlig
 */

#ifndef CBMDEVICESTSLOCALRECO_H_
#define CBMDEVICESTSLOCALRECO_H_

#include "FairMQDevice.h"

#include "TMessage.h"
#include <vector>

class CbmDeviceStsLocalReco : public FairMQDevice {
public:
  CbmDeviceStsLocalReco();
  virtual ~CbmDeviceStsLocalReco();

protected:
  virtual void InitTask();
  bool HandleData(FairMQMessagePtr&, int);

private:
  uint64_t fMaxTimeslices;
  uint64_t fNumMessages;
};

// special class to expose protected TMessage constructor
class CbmMQTMessage : public TMessage {
public:
  CbmMQTMessage(void* buf, Int_t len) : TMessage(buf, len) {
    ResetBit(kIsOwner);
  }
};

#endif /* CBMDEVICESTSLOCALRECO_H_ */

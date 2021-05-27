// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmStsHitProducerIdealAlgo                    -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef Cbm_StsHitProducerIdealAlgo_H
#define Cbm_StsHitProducerIdealAlgo_H

#include "CbmAlgo.h"

// Data
#include "CbmStsHit.h"
#include "CbmStsPoint.h"

// C/C++
#include <vector>

class CbmTrdParSetGas;

class CbmStsHitProducerIdealAlgo : public CbmAlgo<CbmStsPoint, CbmStsHit> {
public:
  CbmStsHitProducerIdealAlgo();
  ~CbmStsHitProducerIdealAlgo();
  CbmStsHitProducerIdealAlgo(const CbmStsHitProducerIdealAlgo&) = delete;
  CbmStsHitProducerIdealAlgo& operator=(const CbmStsHitProducerIdealAlgo&) = delete;

  virtual Bool_t Init();
  virtual void Reset();
  virtual void Finish();

  Bool_t InitContainers();
  Bool_t ReInitContainers();
  TList* GetParList();

  Bool_t InitParameters();

  virtual std::vector<CbmStsHit> ProcessInputData(const std::vector<CbmStsPoint>&);

private:
  /// Settings from parameter file
  CbmTrdParSetGas* fTrdGasPar {nullptr};
};

#endif

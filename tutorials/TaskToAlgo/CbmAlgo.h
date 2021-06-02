/** @file CbmAlgo.h
  * @copyright Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                         CbmAlgo                                   -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmAlgo_H
#define CbmAlgo_H

#include "Rtypes.h"

#include <vector>

class TList;

template<class Input, class Output>
class CbmAlgo {
public:
  CbmAlgo()               = default;
  virtual ~CbmAlgo()      = default;
  CbmAlgo(const CbmAlgo&) = delete;
  CbmAlgo& operator=(const CbmAlgo&) = delete;

  virtual Bool_t Init()             = 0;
  virtual void Reset()              = 0;
  virtual void Finish()             = 0;
  virtual Bool_t InitContainers()   = 0;
  virtual Bool_t ReInitContainers() = 0;
  virtual TList* GetParList()       = 0;

  virtual std::vector<Output> ProcessInputData(const std::vector<Input>&) = 0;

protected:
  /// Parameter management
  TList* fParCList {nullptr};

private:
};

#endif

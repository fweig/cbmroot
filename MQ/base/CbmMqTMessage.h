/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#ifndef CBMMQTMESSAGE_H_
#define CBMMQTMESSAGE_H_

#include "TMessage.h"

// special class to expose protected TMessage constructor
class CbmMqTMessage : public TMessage {
public:
  CbmMqTMessage(void* buf, Int_t len) : TMessage(buf, len) { ResetBit(kIsOwner); }
};

#endif /* CBMMQTMESSAGE_H_ */

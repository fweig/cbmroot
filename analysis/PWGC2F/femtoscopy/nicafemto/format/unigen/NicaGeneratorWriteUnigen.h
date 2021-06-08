/* Copyright (C) 2015-2019 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * NicaGeneratorWriteUnigen.h
 *
 *  Created on: 13 sie 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef _NICAGENERATORWRITEUNIGEN_H_
#define _NICAGENERATORWRITEUNIGEN_H_

#include "NicaGeneratorWrite.h"
#include "UEvent.h"
#include "UParticle.h"
/**
 * default class for saving data in unigen format
 */
class NicaGeneratorWriteUnigen : public NicaGeneratorWrite {
  UEvent* fEvent;

public:
  NicaGeneratorWriteUnigen();
  virtual InitStatus Init();
  virtual void Exec(Option_t* /*opt*/) {};
  virtual void ClearEvent();
  void AddEvent();
  void AddParticle();
  virtual ~NicaGeneratorWriteUnigen();
  ClassDef(NicaGeneratorWriteUnigen, 1)
};

#endif /* _NICAGENERATORWRITEUNIGEN_H_ */

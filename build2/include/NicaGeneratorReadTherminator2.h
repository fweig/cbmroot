/*
 * NicaGeneratorReadTherminator2.h
 *
 *  Created on: 6 lut 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAGENERATORREADTHERMINATOR2_H_
#define NICAGENERATORREADTHERMINATOR2_H_

#include "NicaGeneratorReadTree.h"

/**
 * basic class for converring standard output from default therminator2 files
 */
class NicaGeneratorReadTherminator2 : public NicaGeneratorReadTree {
  struct particle {
    Float_t mass;
    Float_t t;
    Float_t x;
    Float_t y;
    Float_t z;
    Float_t e;
    Float_t px;
    Float_t py;
    Float_t pz;
    Int_t decayed;
    Int_t pid;
    Int_t fatherpid;
    Int_t rootpid;
    Int_t eid;
    Int_t fathereid;
    UInt_t eventid;
  };

  particle fParticle;
  Int_t fTrackCounter;
  Int_t fX_Id, fY_Id, fZ_Id, fT_Id;
  Int_t fStatus_Id, fMother_Id;

 protected:
  virtual Int_t ReadEvent();
  virtual Int_t Reopen();
  virtual void CheckParameters();

 public:
  NicaGeneratorReadTherminator2(TString file, Bool_t multi_mode);
  virtual ~NicaGeneratorReadTherminator2();
  ClassDef(NicaGeneratorReadTherminator2, 1)
};

#endif /* NICAGENERATORREADTHERMINATOR2_H_ */

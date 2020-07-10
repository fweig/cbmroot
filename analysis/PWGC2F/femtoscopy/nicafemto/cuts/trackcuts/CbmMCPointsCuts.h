/*
 * CbmMCPointsCuts.h
 *
 *  Created on: 18 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMMCPOINTSCUTS_H_
#define CBMMCPOINTSCUTS_H_

#include "NicaCbmTrack.h"
#include "NicaTrackCut.h"

class NicaCbmMCPointsCut : public NicaTrackCut {
public:
  NicaCbmMCPointsCut(Int_t params);
  virtual Bool_t Init(Int_t format_id);
  virtual ~NicaCbmMCPointsCut() {};
  ClassDef(NicaCbmMCPointsCut, 1);
};

class NicaCbmStsPointsCut : public NicaCbmMCPointsCut {
public:
  NicaCbmStsPointsCut();
  Bool_t Pass(NicaTrack* track);
  virtual ~NicaCbmStsPointsCut() {};
  ClassDef(NicaCbmStsPointsCut, 1);
};

class NicaCbmMvdPointsCut : public NicaCbmMCPointsCut {
public:
  NicaCbmMvdPointsCut();
  Bool_t Pass(NicaTrack* track);
  virtual ~NicaCbmMvdPointsCut() {};
  ClassDef(NicaCbmMvdPointsCut, 1);
};

class NicaCbmRichPointsCut : public NicaCbmMCPointsCut {
public:
  NicaCbmRichPointsCut();
  Bool_t Pass(NicaTrack* track);
  virtual ~NicaCbmRichPointsCut() {};
  ClassDef(NicaCbmRichPointsCut, 1);
};

class NicaCbmMuchPointsCut : public NicaCbmMCPointsCut {
public:
  NicaCbmMuchPointsCut();
  Bool_t Pass(NicaTrack* track);
  virtual ~NicaCbmMuchPointsCut() {};
  ClassDef(NicaCbmMuchPointsCut, 1);
};

class NicaCbmTrdPointsCut : public NicaCbmMCPointsCut {
public:
  NicaCbmTrdPointsCut();
  Bool_t Pass(NicaTrack* track);
  virtual ~NicaCbmTrdPointsCut() {};
  ClassDef(NicaCbmTrdPointsCut, 1);
};

class NicaCbmTofPointsCut : public NicaCbmMCPointsCut {
public:
  NicaCbmTofPointsCut();
  Bool_t Pass(NicaTrack* track);
  virtual ~NicaCbmTofPointsCut() {};
  ClassDef(NicaCbmTofPointsCut, 1);
};

class NicaCbmEcalPointsCut : public NicaCbmMCPointsCut {
public:
  NicaCbmEcalPointsCut();
  Bool_t Pass(NicaTrack* track);
  virtual ~NicaCbmEcalPointsCut() {};
  ClassDef(NicaCbmEcalPointsCut, 1);
};

class NicaCbmPsdPointsCut : public NicaCbmMCPointsCut {
public:
  NicaCbmPsdPointsCut();
  Bool_t Pass(NicaTrack* track);
  virtual ~NicaCbmPsdPointsCut() {};
  ClassDef(NicaCbmPsdPointsCut, 1);
};


#endif /* NICA_CBM_CUTS_TRACKCUTS_CBMMCPOINTSCUTS_H_ */

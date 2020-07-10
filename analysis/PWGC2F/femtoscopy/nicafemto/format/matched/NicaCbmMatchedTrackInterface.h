/*
 * NicaCbmMatchedTrackInterface.h
 *
 *  Created on: 12 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMMATCHEDTRACKINTERFACE_H_
#define NICACBMMATCHEDTRACKINTERFACE_H_

#include "CbmTrackMatchNew.h"
#include "NicaCbmTrackInterface.h"

class NicaCbmMatchedTrackInterface : public NicaCbmTrackInterface {
  CbmTrackMatchNew* fStsLink;
  CbmTrackMatchNew* fTofLink;
  CbmTrackMatchNew* fTrdLink;
  CbmTrackMatchNew* fRichLink;
  CbmTrackMatchNew* fMuchLink;

public:
  NicaCbmMatchedTrackInterface();
  void SetLinkSts(CbmTrackMatchNew* link) { fStsLink = link; }
  void SetLinkTof(CbmTrackMatchNew* link) { fTofLink = link; };
  void SetLinkTrd(CbmTrackMatchNew* link) { fTrdLink = link; };
  void SetLinkMuch(CbmTrackMatchNew* link) { fMuchLink = link; };
  void SetLinkRich(CbmTrackMatchNew* link) { fRichLink = link; };
  CbmTrackMatchNew* GetLinkSts() const { return fStsLink; }
  CbmTrackMatchNew* GetLinkTof() const { return fTofLink; };
  CbmTrackMatchNew* GetLinkTrd() const { return fTrdLink; };
  CbmTrackMatchNew* GetLinkMuch() const { return fMuchLink; };
  CbmTrackMatchNew* GetLinkRich() const { return fRichLink; };
  virtual ~NicaCbmMatchedTrackInterface();
  ClassDef(NicaCbmMatchedTrackInterface, 1)
};

#endif /* NICA_CBM_FORMAT_MATCHED_NICACBMMATCHEDTRACKINTERFACE_H_ */

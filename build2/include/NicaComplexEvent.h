/*
 * NicaEventCombined.h
 *
 *  Created on: 4 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACOMPLEXEVENT_H_
#define NICACOMPLEXEVENT_H_

#include <TString.h>

#include "NicaEvent.h"
#include "NicaStd.h"

/**
 * class for storing two formats, Real event presents current data whereas
 * "imaginary event" is hidden NOTE - compression of source here doesn't work
 * corretly so use uncompressed event or only don't use raw tracks
 */
class NicaComplexEvent : public NicaEvent {
protected:
  static Int_t* fgIndexMap;       //!
  static Int_t* fgSumMap;         //!
  static Int_t fgIndexArraySize;  //!
  static Int_t fgSumMapSize;      //!
  NicaEvent* fRealEvent;
  NicaEvent* fImgEvent;
  void CalculateCompressionMapImg(Int_t* map, Int_t map_size, NicaComplexEvent* event);
  void ShallowCopyTracks(NicaEvent* event);
  void ShallowCopyCompressTracks(NicaEvent* event, Int_t* map, Int_t* mapID, Int_t map_size);
  void ShallowCopyEvent(NicaEvent* event);
  NicaComplexEvent(TString track_class, NicaEvent* real, NicaEvent* img);

public:
  NicaComplexEvent(NicaEvent* real = NULL, NicaEvent* img = NULL);
  NicaComplexEvent(const NicaComplexEvent& other);
  void CreateSource();
  virtual void ClearSource(Bool_t del = kTRUE);
  virtual void Clear(Option_t* opt = " ");
  virtual void Update();
  virtual void LinkWithTree();
  virtual void CopyHiddenSettings(const NicaEvent* event);
  virtual Bool_t HasHiddenSettings() const;
  virtual Bool_t ExistInTree() const;
  virtual Bool_t IsCompatible(const NicaEvent* non_buffered) const;
  virtual Float_t GetFieldVal(Int_t fieldID) const;
  virtual TString GetFieldName(Int_t fieldID) const;
  virtual ENicaFormatType GetFormatType() const { return ENicaFormatType::kComplexReco; };
  NicaEvent* GetImgEvent() const { return fImgEvent; };
  NicaEvent* GetRealEvent() const { return fRealEvent; };
  NicaTrack* GetNewImgTrack() const { return fImgEvent->GetNewTrack(); };
  NicaTrack* GetNewRealTrack() const { return fRealEvent->GetNewTrack(); };
  virtual ~NicaComplexEvent();
  ClassDef(NicaComplexEvent, 1)
};

#endif /* NICACOMPLEXEVENT_H_ */

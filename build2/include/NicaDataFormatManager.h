/*
 * NicaDataFormatManager.h
 *
 *  Created on: 23-06-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICADATAFORMATMANAGER_H_
#define NICADATAFORMATMANAGER_H_

#include <TObject.h>

#include "NicaStd.h"
#include "NicaTrack.h"

class NicaEvent;

/**
 * manager of formats
 */
class NicaEventAna;

class NicaDataFormatManager : public TObject {
  friend class NicaEventAna;
  Int_t fRegisteredFormats;
  NicaEvent** fDataFormatBuffered;     //[fRegisteredFormats]
  NicaEvent** fDataFormatNonBuffered;  //[fRegisteredFormats]
  TObjArray* fAutoSupportedFormats;
  static NicaDataFormatManager* fgInstance;

 public:
  /**
   * default constructor
   */
  NicaDataFormatManager();
  /**
   * regiter new format
   * @return id of newly registered format
   */
  Int_t RegisterFormat();
  /**
   * add new kind of supported format to automatical formats
   * @param event instabce of class that is used for new format
   */
  void AddAutoSupportedFormat(NicaEvent* event);
  /**
   * remove all stored formats and events
   */
  void Reset();
  /**
   * try to find anty kind of supported events in tree
   * @return pointer to supported format
   */
  NicaEvent* FindAutoSupportedFormat();
  /**
   * force overwrite format at given position
   * @param format event that represent new format
   * @param task_id id of overwritten format
   * @param silent - silent overwriting (don't display warning
   */
  void SetFormat(NicaEvent* format, Int_t task_id,
                 ENicaFormatDepth depth = ENicaFormatDepth::kAll, Bool_t silent = kFALSE);
  /**
   *
   * @param task_id id of used format
   * @return event that point to event at current entry in tree in proper format
   */
  NicaEvent* GetEventFromTree(Int_t task_id);
  /**
   * return allocated representatnio of NicaEvent used with given format
   * @param task_id format/task id
   * @return newly allocated event
   */
  const NicaEvent* GetFormat(Int_t task_id,
                             ENicaFormatDepth format_depth = ENicaFormatDepth::kAll) const;
  /**
   *
   * @param task_id format id
   * @return event optimized for given format of data
   */
  NicaEvent* GetNewEvent(Int_t task_id,
                         ENicaFormatDepth format_depth = ENicaFormatDepth::kAll) const;
  /**
   * create pointer to track with proper format
   * @param task_id format id
   * @return pointer to track in proper format
   */
  NicaTrack* GetNewTrack(Int_t task_id,
                         ENicaFormatDepth format_depth = ENicaFormatDepth::kAll) const;
  /**
   *
   * @param task_id format id
   * @return name of format with given id
   */
  TString GetFormatName(Int_t task_id,
                        ENicaFormatDepth format_depth = ENicaFormatDepth::kAll) const;
  /**
   *
   * @param task_id format id
   * @return true if format has been registered and set
   */
  Bool_t FormatExist(Int_t task_id, ENicaFormatDepth format_depth = ENicaFormatDepth::kAll) const;
  /**
   *
   * @return instance of this
   */
  static NicaDataFormatManager* Instance();
  virtual ~NicaDataFormatManager();
  ClassDef(NicaDataFormatManager, 1)
};

#endif /* NICADATAFORMATMANAGER_H_ */

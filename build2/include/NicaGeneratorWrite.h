/*
 * NicaGeneratorWrite.h
 *
 *  Created on: 13 sie 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAGENERATORWRITE_H_
#define NICAGENERATORWRITE_H_

#include <FairTask.h>
#include <TString.h>

#include "NicaStd.h"

class NicaGeneratorParametersArray;
class NicaXMLFile;

/**
 * basci class for writing data from NicaGeneratorRead
 */
class NicaGeneratorWrite : public FairTask {
private:
  Int_t fTrackParCounter[3];
  Int_t fEventParCounter[3];
  Int_t fEventPatternsNo;
  Int_t fTrackPatternsNo;
  TString* fEventPatterns;  //[fEventPatternsNo]
  TString* fEventLabels;    //[fEventPatternsNo]
  Char_t* fEventTypes;      //[fEventPatternsNo]
  TString* fTrackPatterns;  //[fTrackPatternsNo]
  TString* fTrackLabels;    //[fTrackPatternsNo]
  Char_t* fTrackTypes;      //[fTrackPatternsNo]
  NicaXMLFile* fXMLParser;

protected:
  /**
   * array of event parameters
   */
  NicaGeneratorParametersArray* fEventParameters;
  /**
   * array of track parameterss
   */
  NicaGeneratorParametersArray* fTrackParameters;
  /**
   * register parameter with given pattern
   * @param pattern patter of given parameters
   */
  void Register(TString pattern);
  /**
   * register group of parameters
   * @param no number of parameters to registration
   */
  void Register(Int_t no, ...);

public:
  /**
   * default ct-or
   * @param event_par_i number of integer parameters in event
   * @param event_par_d number of double parameters in event
   * @param event_par_s number of string parameters in event
   * @param event_par_u number of ulong64  parameters in event
   * @param track_par_i number of track parameters (integers)
   * @param track_par_d number of track parameters (doubles)
   * @param track_par_s number of string parameters (strings)
   * 	@param track_par_u number of string parameters (ulong64)
   */
  NicaGeneratorWrite(Int_t event_par_i,
                     Int_t event_par_d,
                     Int_t event_par_s,
                     Int_t event_par_u,
                     Int_t track_par_i,
                     Int_t track_par_d,
                     Int_t track_par_s,
                     Int_t track_par_u);
  /**
   *
   * @param update update ratio - event or track
   * @return parameter array
   */
  NicaGeneratorParametersArray* GetParams(ENicaCutUpdate update) const {
    if (update == ENicaCutUpdate::kTrackUpdate) return fTrackParameters;
    return fEventParameters;
  }
  /**
   * add particle to event and fill particle wtih parameters
   */
  virtual void AddParticle() {};
  /**
   * fill event header with data
   */
  virtual void AddEvent() {};
  /**
   * init method
   * @return init status
   */
  virtual InitStatus Init();
  /**
   * exec method
   * @param opt option
   */
  virtual void Exec(Option_t* /*opt*/ = "") {};
  /**
   * clear event
   */
  virtual void ClearEvent();
  /**
   * clear task - not used
   */
  virtual void Clear(Option_t* opt = "");
  /**
   *
   * @param pattern parameter name
   * @return parameter description
   */
  TString GetParameterDescription(TString pattern);
  /**
   *
   * @param pattern parameter name
   * @return parameter type
   */
  Char_t GetParameterType(TString pattern);
  virtual ~NicaGeneratorWrite();
  ClassDef(NicaGeneratorWrite, 1)
};

#endif /* NICAGENERATORWRITE_H_ */

/*
 * NicaEventAna.h
 *
 *  Created on: 05-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAEVENTANA_H_
#define NICAEVENTANA_H_

#include <FairTask.h>
#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>

#include "NicaCut.h"
#include "NicaCutMonitor.h"
#include "NicaStd.h"

class NicaEvent;
class TDatabasePDG;

class NicaCutsAndMonitors;
class NicaDataFormatManager;
class NicaMemoryMapManager;

// s#define _NICA_CLEAR_BUFFER_ // realy need to clear bufferes?
class NicaEventAnaChain;
/**
 * Base class for physical analysis
 */
class NicaEventAna : public FairTask {
  friend NicaEventAnaChain;

protected:
  Bool_t fCompressEvents;
  Bool_t fDirectAcces;
  Bool_t fKeepSource;
  Bool_t fDisableFormatChecking;
  /**
   * number of processed events
   */
  UInt_t fProcessedEvents;
  /**
   * size of buffer for mixing (not used by event and track analysis
   */
  Int_t fMixSize;
  /**
   * total number of event collections
   */
  Int_t fEventCollectionsNo;
  /**
   * point to current event collection number
   */
  Int_t fCurrentEventCollectionID;
  /**
   * number of tiers in NicaCutContainer, used during creating first cut, if
   * number is too small some cut's will not be added and code my crash
   */
  ENicaCutUpdate fTiers;
  /**
   * pointer to instance of  TDatabasePDG
   */
  TDatabasePDG* fPDG;
  /**
   * hold information about all cuts and cut monitors
   */
  NicaCutContainer* fCutContainer;
  /**
   * hold all events and tracks
   */
  NicaMemoryMapManager* fMemoryMap;
  /**
   * currently processed event
   */
  NicaEvent* fCurrentEvent;
  /**
   * comment about this analysis
   */
  TString fComment;
  /**
   * true if object has been initialized
   */
  Int_t fInit;
  /**
   *
   * @return name of input file
   */
  TString GetInputFileName() const { return fInFileName; };
  /**
   * set input file name
   * @param name
   */
  void SetInputFileName(TString name) { fInFileName = name; };
  /**
   * initialize memory map manager
   */
  virtual void InitMemoryMap();

private:
  /**
   * true if this task is in chain
   */
  Bool_t fInChain;
  TList* fTagList;
  Int_t fTaskID;
  TString fInFileName;
  NicaDataFormatManager* fDataFormatManager;  //->
  static Double_t fgMagField;
  static Bool_t fgUseForcedMagfiield;
#ifdef _NICA_CLEAR_BUFFER_
  Bool_t fIsLastTask;
#endif

protected:
  /**
   *  true if should be used MagFiled from FairRunAna
   */
  Bool_t UseFairRunField() const { return !fgUseForcedMagfiield; };
  /**
   *
   * @return magnetic field provied by user
   */
  Double_t GetUserMagField() const { return fgMagField; };
  /**
   *
   * @return true if taks in chain
   */
  Bool_t IsInChain() const { return fInChain; };
  /**
   * mark taks as "in chain"
   */
  void MarkAsInChain() { fInChain = kTRUE; };
  /**
   * go to subdirectory in output file
   * @param name path to directory
   */
  void GoToDir(TString name);
  /**
   * add object to existing metadata list
   * @param main_pack pointer to package created by Report method
   * @param obj object to add
   */
  void AddToAnaMetadata(NicaPackage* main_pack, TObject* obj) const;
  /**
   * initialize cut container
   * @return return kFATAL if something is wrong with cut containers
   */
  InitStatus InitCutContainer();
  /**
   * check if format set in analysis is ok
   * @return
   */
  virtual InitStatus CheckFormat();
  /**
   *
   * @return ID of task (and usally andformat) used in this task (each task have
   * own ID even if use the same format
   */
  inline Int_t GetTaskID() const { return fTaskID; };
  /**
   * link collections
   */
  virtual void LinkCollections();
  /**
   * process event, can be be overwritten in derived classes
   */
  virtual void ProcessEvent() {};
  /**
   * init new cut container if current is NULL (may happen if user don't add any
   * cut,  should be overwritten in derived classes
   */
  virtual void InitNewCutContainer();
  /**
   * checking cut containers.  should be overwritten in derived classes
   */
  virtual void CheckCutContainerCollections();
  /**
   * create report that will be saved, derived classes should call this method
   * for base class and add own objects
   * @return report from analysis
   */
  virtual NicaPackage* Report() const;
#ifdef _NICA_CLEAR_BUFFER_
  /**
   * force clean memory after last analysis
   */
  void ClearMemory();
#endif
  /**
   * initialize task,  should aslo call parent Init (or at leas
   * NicaEventAna::Init)
   * @return status of initialization
   */
  virtual InitStatus Init();

public:
  enum EFormatOption { kCompress, kNoCompress, kKeepSource, kNoKeepSource, kDirectAccess, kNoDirectAcces };
  /**
   * basic constructor
   */
  NicaEventAna();
  /**
   * copy constructor
   * @param ana
   */
  NicaEventAna(const NicaEventAna& ana);
  /**
   * assignement operator
   * @param other
   * @return
   */
  NicaEventAna& operator=(const NicaEventAna& other);
  /**
   * add tags
   * @param tag tags seperated by space
   */
  virtual void AddTags(TString tag);
  /**
   * add cut object
   * @param cut
   * @param opt  can be "fast", with multiplying factor "{AxB}" (where A is
   * number of added cuts, and B is jump in collection value), like "{A}" (then
   * cut is added with collection number A), also mixed versions like
   * "fast+{AxB}" and "fast+{A}" can be used. For two track analysis option
   * "sig" add cut only for signal pairs and "bckg" only for background pairs.
   * It's also possible to add in this way pure real or pure imaginary cuts. For
   * example by adding NicaTrackXCut with "im" option
   * NicaTrackComplexCut(NicaTrackVirtualCut, NicaTrackXCut) is added.
   */
  virtual void AddCut(const NicaCut& cut, Option_t* opt = "");
  /**
   * add cut monitor object
   * @param mon
   * @param opt option of adding cut monitor. Following options are allowed:
   * "bckg" to add only to background, "sig" to add only in singal (both flags
   * are valid only for analysis where signal and background is present). Other
   * options are like "{AxB}" where A is number of cut monitors, and B is jumb
   * in collection number, eg. {2x3} mean adding cut with collection N, N+3.
   * User might also set option like "{A}" where A is collection number.
   * Combination of those options are allowed. Its's also possible to monitor
   * pure "real"  cuts or pure imaginary cuts but setting option "+re" or "+im"
   * respectively
   *
   */
  virtual void AddCutMonitor(const NicaCutMonitor& mon, Option_t* opt = "");
  /**
   * add collection of predefined cuts and cut monitors
   * @param monCuts
   */
  virtual void AddCutsAndMonitors(const NicaCutsAndMonitors& monCuts);
  /**
   * called for each event
   * @param opt not used
   */
  virtual void Exec(Option_t* opt);
  /**
   * for setting options, not used in NicaEventAna
   * @param opt
   */
  virtual void SetOption(Option_t* opt);
  /**
   * set format option
   *  - kCompress - compress buffered events
   *  - kDirectAcces - read preprocesed event from tree (that inherits from
   * NicaEvent)
   *  - kKeepSource - keep original structure in memory
   * @param option
   */
  void SetFormatOption(EFormatOption option);
  /**
   * add owno comment
   * @param comment comment
   */
  virtual void SetComment(TString comment);
  /**
   * set format
   * @param format any class that inherit from NIcaEventAna and ican be read
   * from currently processed file
   * @param depth - format type - use buffered to setup different format for
   * buffered events
   */
  virtual void SetFormat(NicaEvent* format, ENicaFormatDepth depth = ENicaFormatDepth::kAll);
  /**
   * called at the end of analysis save resutls
   */
  virtual void FinishTask();
  /**
   * default dtor
   */
  virtual ~NicaEventAna();
  /**
   * set magnetic field used by helix, magnetic field from FairRunAna will not
   * be used anymore
   * @param field
   */
  static void SetMagField(Double_t field);
  /**
   * skip format checking, should not be used in typical analysis
   */
  void DisableFormatChecking() { fDisableFormatChecking = kTRUE; };
  ClassDef(NicaEventAna, 1)
};

#endif /* NICAEVENTANA_H_ */

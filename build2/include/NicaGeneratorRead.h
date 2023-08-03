/*
 * NicaGeneratorRead.h
 *
 *  Created on: 13 sie 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAGENERATORREAD_H_
#define NICAGENERATORREAD_H_

#include <FairTask.h>
#include <TString.h>
#include <map>

#include "NicaGeneratorWrite.h"

/**
 * basci generator for reading files
 */
class NicaGeneratorRead : public FairTask {
 private:
  std::map<Int_t, Int_t> fParticleTable;  //! table with conversion data
  const TString fConversionTableName;
  const Int_t fMaxFilesNo;
  /**
   * get event, called for each exec
   * @param return true if suceeded
   */
  Int_t TryGetEvent();
  Int_t TryGetFile();
  Int_t fStatus;
  NicaGeneratorWrite *fWriteGenerator;

 protected:
  /**
   * px parameter index
   */
  Int_t fPx_Id;
  /**
   * py parametere index
   */
  Int_t fPy_Id;
  /**
   * pz parameter index
   */
  Int_t fPz_Id;
  /**
   * energy parameter index
   */
  Int_t fE_Id;
  /**
   * pdg code parameter index
   */
  Int_t fPdg_Id;
  /**
   * particle index index
   */
  Int_t fIndex_Id;
  /**
   * event number  paramter index
   */
  Int_t fEventNr_Id;
  /**
   * impact parameter index
   */
  Int_t fB_Id;
  /**
   * reaction plane angle parameter index
   */
  Int_t fPhi_Id;
  /**
   * check if parameter can be saved
   * @param pattern parameter name
   * @return paramter index or 0 if parameter cannot be stored
   */
  Int_t CheckParameter(TString pattern);
  /**
   * array of event parameters
   */
  NicaGeneratorParametersArray *fEventParameters;
  /**
   * array of track parameters
   */
  NicaGeneratorParametersArray *fTrackParameters;
  /**
   * array with names of files
   */
  TString *fFileName;
  /**
   * array with number of events in files
   */
  Int_t *fEventsNo;
  /**
   * number of proceseed events in opened file
   */
  Int_t fProcessedEvents;
  /**
   * number of files
   */
  Int_t fFilesNo;
  /**
   * number of processed files
   */
  Int_t fProcessedFiles;
  /**
   * total number of processed events
   */
  Int_t fTotalProcessedEvents;
  /**
   * event number
   */
  Int_t fNr;
  /**
   * read input event and fill fEvent
   */
  virtual Int_t ReadEvent();
  /**
   * close one input file and open next
   */
  virtual Int_t Reopen();
  /**
   * close input file
   */
  virtual void Close(){};
  /**
   * check if parameters can be registered and set parameter numbers
   */
  virtual void CheckParameters();
  /**
   * read table that convert generator code into the pdg code
   */
  void ReadConversionTable();
  /**
   * convert generator code into pdg code
   * @param pid generator code
   * @return pdg code
   */
  Int_t GetPdgCode(Int_t pid);
  /**
   * constructor with pdg-table
   * @param filename input file (or list of files)
   * @param mulit_mode if true then mulit_mode is used
   * @param table_file name of file with PDG conversion table
   */
  NicaGeneratorRead(TString filename, Bool_t mulit_mode, TString table_file);
  /**
   * copy parameters into event header, should be called for fill event
   */
  inline void WriteEventHeader() { fWriteGenerator->AddEvent(); };
  /**
   * copy track parameters and add track, should be called for each particle
   */
  inline void WriteParticle() { fWriteGenerator->AddParticle(); };

 public:
  /**
   * default constructor
   * @param filename name of file
   * @param multi_mode if true then treat filename like a file with list of
   * files
   */
  NicaGeneratorRead(TString filename = "", Bool_t multi_mode = kFALSE);
  /**
   * init method
   * @param write one of NicaGeneratorWrite
   * @return
   */
  virtual InitStatus InitGen(NicaGeneratorWrite *write);
  /**
   * exec method
   * @param opt not used
   * @return false if something bad happened
   */
  void Exec(Option_t *opt = "");
  /**
   * finish simulation
   */
  virtual void FinishTask();
  /**
   *
   * @return status of event
   */
  virtual Int_t GetStatus() { return fStatus; };
  /**
   *
   * @return status flag for good event
   */
  static Int_t EventOk() { return 0; };
  /**
   *
   * @return status flag if event have bad format
   */
  static Int_t EventBad() { return 1; };
  /**
   *
   * @return status flag if file is OK
   */
  static Int_t FileOk() { return 2; };
  /**
   *
   * @return status flag if there is problem with reading file
   */
  static Int_t FileBad() { return 3; };
  /**
   *
   * @return status flag if there is no more files to open
   */
  static Int_t NoMoreFiles() { return 4; };
  virtual ~NicaGeneratorRead();
  ClassDef(NicaGeneratorRead, 1)
};

#endif /* NICAGENERATORREAD_H_ */

/*
 * NicaGeneratorReadTxt.h
 *
 *  Created on: 13 sie 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef _NICAGENERATORREADTXT_H_
#define _NICAGENERATORREADTXT_H_

#include <fstream>

#include "NicaGeneratorRead.h"
#ifndef __CINT__
#include <zlib.h>
#endif
/**
 * basic abstract generator for reading txt files
 */
class NicaGeneratorReadTxt : public NicaGeneratorRead {
 private:
  Int_t fInputMode;

 protected:
  /**
   * input gz file
   */
#ifndef __CINT__
  gzFile fGzFile;  //! gz file
#endif
  /**
   * input txt file
   */
  std::ifstream fInputFile;
  /**
   * true if can process txt files
   */
  Bool_t fTxtSupported;
  /**
   * true if can process gz files
   */
  Bool_t fGzSupported;
  /**
   * read event int gz file
   */
  virtual Int_t ReadEventGz();
  /**
   * read event int txt file
   */
  virtual Int_t ReadEventTxt();
  /**
   * open gz file
   */
  virtual Int_t ReopenGz();
  /**
   * open txt file
   */
  virtual Int_t ReopenTxt();
  virtual void CheckParameters();
  Int_t ReadEvent();
  Int_t Reopen();
  /**
   * constructor with pdg-table
   * @param filename input file (or list of files)
   * @param multi_mode if true then multi mode is used
   * @param table_file name of file with PDG conversion table
   */
  NicaGeneratorReadTxt(TString filename, Bool_t multi_mode, TString table_file);

 public:
  /**
   * default constructor
   * @param filename name of input file,
   * @param multi_mode if true then input should contain list of flles and list
   * of events in each file, such files will be saved into one file
   */
  NicaGeneratorReadTxt(TString filename, Bool_t multi_mode = kFALSE);
  virtual InitStatus InitGen(NicaGeneratorWrite *writer);
  virtual void FinishTask();
  virtual ~NicaGeneratorReadTxt();
  ClassDef(NicaGeneratorReadTxt, 1)
};

#endif /* _NICAGENERATORREADTXT_H_ */

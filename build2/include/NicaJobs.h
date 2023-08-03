/*
 * NicaJobs.h
 *
 *  Created on: 6 maj 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO_FEATURES_NICAJOBS_H_
#define NICAFEMTO_FEATURES_NICAJOBS_H_

#include <TObject.h>
#include <TString.h>
#include <vector>


class NicaJobs : public TObject {
  Bool_t fArray;
  TString fFile;
  TString fSubmitCommand;
  Int_t fStartJob, fEndJob;
  TString fShell;
  std::vector<TString> fCommands;
  std::vector<TString> fOptions;
  static Int_t GetNVariablesTxt(TString textfile);
  static Int_t GetNVariablesXML(TString xmlfile);
  static TString GetParameterTxt(TString textfile, Int_t job, Int_t var);
  static TString GetParameterXml(TString xmlfile, Int_t job, Int_t var);

public:
  NicaJobs(TString name);
  void CreateJobs();
  void SubmitJobs();
  /**
   * create dummy text file that can be used as pattern with parameters passed
   * to job
   * @param textfile
   * @param jobs
   * @param vars
   */
  static void CreateDummyTxtFile(TString textfile, Int_t jobs, Int_t vars);
  /**
   * create dummy text file that can be used as pattern with parameters passed
   * to job
   * @param textfile
   * @param jobs
   * @param vars
   */
  static void CreateDummyXMLFile(TString xmlfile, Int_t jobs, Int_t vars);
  /**
   *
   * @param textfile file with parameters
   * @return number of parameters in text with parameters
   */
  static Int_t GetNVariables(TString textfile);
  /**
   *
   * @param textfile file with job parameters
   * @param job_no number of job
   * @param var_no number of variable
   * @return return given variable from given job
   */
  static TString GetParameter(TString textfile, Int_t job_no, Int_t var_no);
  virtual ~NicaJobs();
  ClassDef(NicaJobs, 1)
};

#endif /* NICAFEMTO_FEATURES_NICAJOBS_H_ */

/*
 * NicaCorrFitDatabase.h
 *
 *  Created on: 22 sty 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFITDATABASE_H_
#define NICACORRFITDATABASE_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>

#include "NicaArray.h"

class TSQLServer;
class NicaCorrFitDumpedPairAna;

/**
 * store information atou parameters of actual job
 */
class NicaCorrFitDbParams : public TObject {
  Int_t fSize;
  Double_t* fParams;  //[fSize]
  TString* fNames;    //[fSize]
public:
  NicaCorrFitDbParams(Int_t params = 1);
  NicaCorrFitDbParams(const NicaCorrFitDbParams& other);
  NicaCorrFitDbParams& operator=(const NicaCorrFitDbParams& other);
  TString GetParName(Int_t par) const;
  Double_t GetParVal(Int_t par) const;
  Double_t GetParVal(TString name) const;
  Int_t GetNParams() const { return fSize; };
  void SetParam(Int_t no, Double_t val, TString name);
  virtual ~NicaCorrFitDbParams();
  ClassDef(NicaCorrFitDbParams, 1)
};

/**
 * store information about number of parameters combination strored in
 * database.
 * This is used to fill JOB_PAR table
 */

class NicaCorrFitDbParamsSetup : public TObject {
  Int_t fSize;
  NicaArray_1<Double_t> fMin;
  NicaArray_1<Double_t> fMax;
  NicaArray_1<Int_t> fNpoints;
  TString* fNames;  //[fSize]
public:
  NicaCorrFitDbParamsSetup(Int_t params = 1);
  NicaCorrFitDbParamsSetup(const NicaCorrFitDbParamsSetup& other);
  NicaCorrFitDbParamsSetup& operator=(const NicaCorrFitDbParamsSetup& rhs);
  virtual void Print(Option_t* opt = "") const;
  void SetParameter(Int_t par_id, Double_t min, Double_t max, Int_t points, TString name);
  Int_t GetNParams() const { return fSize; }
  Int_t GetNSteps(Int_t par_id) const { return fNpoints.Get(par_id) - 1; };
  Int_t GetNPoints(Int_t par_id) const { return fNpoints.Get(par_id); };
  Double_t GetMin(Int_t par_id) const { return fMin.Get(par_id); };
  Double_t GetMax(Int_t par_id) const { return fMax.Get(par_id); };
  Double_t GetStepSize(Int_t par_id) const;
  TString GetName(Int_t par_id) const { return fNames[par_id]; };
  virtual ~NicaCorrFitDbParamsSetup() { delete[] fNames; }
  ClassDef(NicaCorrFitDbParamsSetup, 2)
};

/**
 * main manager of DB
 */

class NicaCorrFitDatabase : public TObject {
  TSQLServer* fSQL;
  NicaCorrFitDbParamsSetup fSetup;
  Int_t fNMerges;
  Int_t fNCommands;
  TString* fCommand;  //[fNCommands]
  TString fParFile;
  void Connect();


public:
  enum eJobStatus { kCompleted = 1, kNotCompleted = 0 };
  NicaCorrFitDatabase();
  /**
   * configure NicaCorrFitDumpedPairAna according to xml file,
   * if job is completed then exit the macro
   * @param ana analysis to setup
   * @param jobID jobID if negative then JOB_ID_NICA is used
   */
  void ConfigureDumpedPairAna(NicaCorrFitDumpedPairAna* ana, Int_t jobID = -1);
  void LoadConfXML();
  void FillDB();
  /**
   * set all job status to 0 (not completed)
   */
  void CleanJobs();
  static void PrepareTemplate();
  void MarkJobCompleted(Int_t job_id);
  static NicaCorrFitDbParams GetParams(Int_t job_id);
  static Bool_t IsJobCompleted(Int_t job_id);
  Bool_t GetJobStatus(Int_t job_id);
  static Bool_t AreAllJobsCompleted();
  Int_t GetNJobs() const;
  NicaCorrFitDbParamsSetup GetSetup() const { return fSetup; };
  virtual ~NicaCorrFitDatabase();
  ClassDef(NicaCorrFitDatabase, 1)
};

#endif /* NICAFEMTO_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_NICACORRFITDATABASE_H_                                          \
        */

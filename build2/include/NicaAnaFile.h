/*
 * NicaPackageExtractor.h
 *
 *  Created on: 23-08-2013
 *  renamed to NicaAnaFile 21-01-2019
 *
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAANAFILE_H_
#define NICAANAFILE_H_

#include <TString.h>
#include <TObject.h>

#include "NicaStd.h"

class NicaPackage;
class TFile;

/**
 * class for extracting data from reports
 */
class NicaAnaFile : public TObject{
  Int_t fHTMLCounter;
  Int_t fCurrentPackID;
  Int_t fMaxPackID;
  TObjArray* fMainPackageArray;
  TObjArray* fCutContainerArray;
  NicaPackage* fMetaDataPack;
  TFile* fFile;
  TList* fPhysKeys;
  TString fFilename;
  Bool_t FindCutContainer(Int_t pack_no);
  void PrintMonitorCutList(TList* list) const;
  void CreateMonitorHTML(Int_t i) const;
  void CreateCutHTML(Int_t i) const;
  void PrintCut(NicaPackage* pack) const;
  void PrintCutsInPackage(NicaPackage* pack) const;
  NicaPackage* GetCutCollection(ENicaCutUpdate update, Int_t no) const;
  NicaPackage* GetMainPackage() const;
  NicaPackage* GetCutContainer() const;

 public:
  /**
   * constructor
   * @param filename name of file
   * @param packname name of package (if not specified first will be taken,
   * other packages will not be read - SwitchPackage will not work)
   */
  NicaAnaFile(TString filename, TString packname);

  /**
   * constructor
   * @param filename name of file
   * @param packID - package numer  (if not specified first will be taken, other
   * packages will not be red - SwitchPackage will not work)
   */
  NicaAnaFile(TString filename = " ", Int_t id = -1);
  /**
   *
   * @return kes from NicaPhysics director
   */
  TList* GetKeys() const { return fPhysKeys; };
  /**
   * switch package by name
   * @param name name of package
   * @return true if package found, false otherwise
   */
  Bool_t SwitchPackage(TString name);
  /**
   * switch package by number
   * @param no number of package
   * @return true if package found, false otherwise
   */
  Bool_t SwitchPackage(Int_t no);
  /**
   *  return entrire analysis package
   * @param name name of package
   * @return package with report from analysis
   */
  NicaPackage* GetPackage(TString name) const;
  /**
   *
   * @return global data (or local if global wasn't found)
   */
  NicaPackage* GetMetaData() const;
  /**
   * return object from current physical analysis package
   * in most cases first two objects contain analysis settings
   * @param i object number
   * @return object from package
   */
  TObject* GetMainObject(Int_t i = 0) const;
  /**
   * return object from current physical analysis package in most cases
   * first two main objects contain information about analysis settings
   * @param name name of object
   * @param index object index (if more objects exist)
   * @return object from package
   */
  TObject* GetMainObject(TString name, Int_t index = 0) const;
  /**
   *
   * @param update cut monitor update ratio/tier
   * @param collection cut monitor collection
   * @param no cut monitor number
   * @return histogram with passed values from cut monitor
   */
  TH1* GetHistogramPassed(ENicaCutUpdate update, Int_t collection,
                          Int_t no) const;
  /**
   *
   * @param update cut monitor update ratio/tier
   * @param collection cut monitor collection
   * @param no cut monitor number
   * @return histogram with failed values from cut monitor
   */
  TH1* GetHistogramFailed(ENicaCutUpdate update, Int_t collection,
                          Int_t no) const;
  /**
   *
   * @return number of merged files into this one
   */
  Int_t GetMerged() const;
  /**
   *
   * @param update tier/update ratio
   * @return number of collections at given level
   */
  Int_t GetCollectionsNo(ENicaCutUpdate update) const;
  /**
   *
   * @return number of objects with current package with analysis
   */
  Int_t GetMainObjectsNo() const;
  /**
   *
   * @param update
   * @param collection
   * @param opt - flag of collections "fast" for only fast collection number,
   * "slow" for only slow collection number, otherwise retur total number of
   * cuts
   * @return number of cuts in given collection
   *
   */
  Int_t GetCutsNo(ENicaCutUpdate update, Int_t collection,
                  TString opt = "all") const;
  /**
   *
   * @param update
   * @param collection
   * @param opt - flag  - might be 1d, 2d or 3d
   * @return number of cut monitors in given collection
   *
   */
  Int_t GetCuMonitorsNo(ENicaCutUpdate update, Int_t collection,
                        TString opt = "all") const;
  /**
   * print list of packages with physical analysis in current file
   * @param filename name of file
   */
  static void GetPackageList(TString filename);
  /**
   *
   * @param update update ratio
   * @param collection collection no
   * @param opt option "fast" for getting entries from "fast" cuts
   * @return number of passed objects in given collection
   */
  ULong64_t GetPassedNo(ENicaCutUpdate update, Int_t collection,
                        Option_t* opt = "slow") const;
  /**
   *
   * @param update update ratio
   * @param collection collection no
   * @param opt option "fast" for getting entries from "fast" cuts
   * @return number of failed objects in given collection
   */
  ULong64_t GetFailedNo(ENicaCutUpdate update, Int_t collection,
                        Option_t* opt = " slow") const;
  /**
   * export file to HTML
   * @param dirname path to place where report will be created
   * @param task_id if -1 all packages exported, otherwise package with given id
   */
  void ExportHTML(TString dirname = "extr_html", Int_t task_id = -1) const;
  /**
   * print cuts settings
   * @param update - update
   * @param col - collection ID (If -1 print all colllections)
   */
  void PrintCuts(ENicaCutUpdate update = ENicaCutUpdate::kNoUpdate,
                 Int_t col = -1) const;
  /**
   *
   * @param update
   * @param collection
   * @param cutNo
   * @param type - type of cut (all, slow or fast - for "all" option slow cuts
   * number is shifted by slow cuts number
   * @return package with report from given cut
   */
  NicaPackage* GetCutPackage(ENicaCutUpdate update, Int_t collection,
                             Int_t cutNo, Option_t* type = "all") const;
  virtual ~NicaAnaFile();
  ClassDef(NicaAnaFile, 1)
};

#endif /* NICAANAFILE_H_ */

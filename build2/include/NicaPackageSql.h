/*
 * NicaPackageSql.h
 *
 *  Created on: 28-08-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAPACKAGESQL_H_
#define NICAPACKAGESQL_H_

#include <TObject.h>
#include <TString.h>

class NicaAnaFile;
class NicaPackage;
class TSQLServer;

/**
 * class for put reports in database
 */
class NicaPackageSql : public TObject {
  TSQLServer *fSQL;
  Int_t fID;
  TString fFilename;
  NicaPackage *fRunInfo;
  TString RemoveDir(TString path);
  TString RemoveDoubleDot(TString path);
  Bool_t HasDir(TString path);
  TString GetPath(TString filename);
  void ExportTask(NicaAnaFile *extr, Int_t file_id, Int_t task_id);

 public:
  /**
   *
   * @param filename file to export
   */
  NicaPackageSql(TString filename);
  virtual ~NicaPackageSql();
  ClassDef(NicaPackageSql, 1)
};

#endif /* NICAPACKAGESQL_H_ */

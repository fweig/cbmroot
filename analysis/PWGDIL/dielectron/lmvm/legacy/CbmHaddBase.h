/* Copyright (C) 2015-2018 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev, Elena Lebedeva [committer] */

#ifndef CBM_HADD_BASE_H
#define CBM_HADD_BASE_H

#include "TObject.h"

#include <string>
#include <vector>

class TFile;
class TList;
class TDirectory;

using namespace std;

class CbmHaddBase : public TObject {

public:
  /*
	 *\brief Hadd all files in a dir with a specified fileTemplate.
	 *\param dir Path to the directory with files
	 *\param fileTemplate [addString].auau.25gev.[00000][.root] [] - will be add to the file name automatically
	 *\param addString analysis or litqa
	 *\param nofFiles number Of files to add
	 */
  static void AddFilesInDir(const std::string& dir, const std::string& fileTemplate, const std::string& addString,
                            Int_t nofFiles, Int_t fileSizeLimit = 50000, Int_t nofEvents = 1000);


  static vector<string> GetFilesByPattern(const string& pattern);

  static vector<string> GetGoodFiles(const string& pattern, Int_t fileSizeLimit, Int_t nofEvents);

private:
  static TFile* CreateAndMergeTempTargetFile(const std::string& dir, const std::string& addString, Int_t targetFileNum,
                                             TList* fileList);

  /*
	 * \brief Close and delete files in the list.
	 */
  static void CloseFilesFromList(TList* fileList);

  /*
	 * \brief Check that file is not NULL and file size is more then a size limit.
	 */
  static Bool_t CheckFileSize(TFile* file, Int_t fileSizeLimit = 50000);

  /*
	 * \brief Check that file is not NULL, file size is more than a limit, number of events is equal to nofEvents
	 */
  static Bool_t CheckFile(TFile* file, Int_t fileSizeLimit = 50000, Int_t nofEvents = 1000);

  /*
	 * \brief Merge root file into one. This function was taken from hadd.C macro.
	 */
  static void MergeRootfile(TDirectory* target, TList* sourcelist);

  ClassDef(CbmHaddBase, 1);
};


#endif

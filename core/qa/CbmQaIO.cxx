/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaIO.cxx
/// @brief  Module for ROOT objects IO interface (implementation)
/// @author S.Zharko <s.zharko@gsi.de>
/// @since  29.03.2023

#include "CbmQaIO.h"

// ---------------------------------------------------------------------------------------------------------------------
//
CbmQaIO::CbmQaIO(const char* folderName, const char* prefixName, TFolder* pParentFolder)
  : fsRootFolderName(folderName)
  , fsPrefix(prefixName)
  , fpParentFolder(pParentFolder)
{
  if (fpParentFolder) { fpFolderRoot = fpParentFolder->AddFolder(fsRootFolderName, fsRootFolderName); }
  else {
    fpFolderRoot =
      new TFolder(fsRootFolderName, fsRootFolderName);  // The name of the folder follows the name of the task
    fpFolderRoot->SetOwner(true);                       // When true, TFolder owns all added objects
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
CbmQaIO::~CbmQaIO()
{
  // Free memory for fpFolderRoot
  if (fpFolderRoot && !fpParentFolder) {
    delete fpFolderRoot;
    fpFolderRoot = nullptr;
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmQaIO::SetHistoProperties(TH1* pHist)
{
  pHist->SetStats(true);
  pHist->Sumw2();
}

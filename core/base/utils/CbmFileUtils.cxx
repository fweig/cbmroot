/** @file CbmFileUtils.cxx
  * @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#include "CbmFileUtils.h"

#include "FairLogger.h"  // for LOG, info and error

#include "TArchiveFile.h"  // for TArchiveFile
#include "TFile.h"         // for TFile
#include "TObjArray.h"

#include <string>  // for string, find, substr

#include <sys/stat.h>  // for stcuct stat


namespace Cbm
{
  namespace File
  {
    bool IsRootFile(std::string filename)
    {
      // Currently plain root files and root files stored in a zip file are supported.
      // The destiction between the two is a "#" in the filename string  which separates the
      // name of the zip file from the name of the root file which is inside the zip file.
      // In case the filename contains a hash (e.g. multi.zip#file.root) the hash
      // separates the name of a zipfile (multi.zip) which contains the real root file
      // (file.root).
      // This nameing convention (e.g. multi.zip#file.root) is sed by ROOT.

      // If a filename string contains a hash "#"
      // split the string at the # in the name of the zipfile and
      // the name of the contained root file.
      std::string checkFilename {""};
      std::string membername {""};
      std::size_t found = filename.find("#");
      if (found != std::string::npos) {
        checkFilename = filename.substr(0, found);
        membername    = filename.substr(found + 1);
      }
      else {
        checkFilename = filename;
      }

      bool wasfound = kFALSE;

      // Check if the file exist
      // In case of a root file contained in a zip archive check if the zip file
      // exist
      struct stat buffer;
      if (stat(checkFilename.c_str(), &buffer) == 0) { wasfound = kTRUE; }
      else {
        wasfound = kFALSE;
        LOG(error) << "Input File " << checkFilename << " not found";
      }

      // In case of a zip archive check if the archive contains the root file
      if (membername.compare("") != 0) {
        TFile* fzip = TFile::Open(checkFilename.c_str());
        if (fzip->IsOpen()) {
          TArchiveFile* archive = fzip->GetArchive();
          if (archive) {
            TObjArray* members = archive->GetMembers();
            if (members->FindObject(membername.c_str()) == 0) {
              LOG(error) << "File " << membername << " not found in zipfile " << checkFilename;
              wasfound = kFALSE;
            }
            else {
              LOG(info) << "File " << membername << " found in zipfile " << checkFilename;
              wasfound = kTRUE;
            }
          }
          else {
            LOG(error) << "Zipfile " << checkFilename << " does not contain an archive";
            wasfound = kFALSE;
          }
          fzip->Close();
          delete fzip;
        }
        else {
          LOG(error) << "Could not open zipfile " << checkFilename;
          wasfound = kFALSE;
        }
      }
      else {
        TFile* rootfile = TFile::Open(checkFilename.c_str());
        if (rootfile->IsOpen()) {
          LOG(info) << "File " << checkFilename << " is a ROOT file.";
          wasfound = kTRUE;
        }
        else {
          LOG(error) << "File " << checkFilename << " is no ROOT file.";
          wasfound = kFALSE;
        }
        rootfile->Close();
        delete rootfile;
      }

      return wasfound;
    }
  }  // namespace File
}  // namespace Cbm

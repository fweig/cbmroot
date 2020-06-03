/************************************************************
 * The Data Acquisition Backbone Core (DABC)                *
 ************************************************************
 * Copyright (C) 2009 -                                     *
 * GSI Helmholtzzentrum fuer Schwerionenforschung GmbH      *
 * Planckstr. 1, 64291 Darmstadt, Germany                   *
 * Contact:  http://dabc.gsi.de                             *
 * Contact:  http://dabc.gsi.de                             *
 ************************************************************
 * This software can be used under the GPL license          *
 * agreements as stated in LICENSE.txt file                 *
 * which is part of the distribution.                       *
 ************************************************************/

#include "mbs/LmdInput.h"

#include <string.h>
#include <stdlib.h>

/* *** PAL edit *** 
#include "dabc/logging.h"
#include "dabc/Buffer.h"
#include "dabc/FileIO.h"
#include "dabc/Manager.h"
#include "dabc/Port.h"
   ***************** */
#include "FairLogger.h"

/* *** PAL: for wildcard support *** */
#include "TRegexp.h"
#include "TSystem.h"
/* ***************** */

#include "mbs/MbsTypeDefs.h"

mbs::LmdInput::LmdInput(const char* fname, uint32_t bufsize) :
//   dabc::DataInput(), *** PAL edit *** 
   fFileName(fname ? fname : ""),
   fBufferSize(bufsize),
//   fFilesList(0),
   fInputFileList(),  // *** PAL edit, from CbmSourceLmdNew ***
   fFile(),
   fCurrentFileName(),
   fCurrentRead(0)
{
}

mbs::LmdInput::~LmdInput()
{
   // FIXME: cleanup should be done much earlier
   CloseFile();
/* *** PAL edit *** 
   if (fFilesList) {
      dabc::Object::Destroy(fFilesList);
      fFilesList = 0;
   }
   ***************** */
   fInputFileList.Delete();
}

/* *** PAL edit: We don't use the xml config in CbmRoot *** 
bool mbs::LmdInput::Read_Init(const dabc::WorkerRef& wrk, const dabc::Command& cmd)
{
   fFileName = wrk.Cfg(mbs::xmlFileName, cmd).AsStdStr(fFileName);
   fBufferSize = wrk.Cfg(dabc::xmlBufferSize, cmd).AsInt(fBufferSize);

   // DOUT1(("BufferSize = %d", fBufferSize));

   return Init();
}
   ***************** */

bool mbs::LmdInput::Init()
{
/* *** PAL edit: Avoid using DABC data types, use ROOT ones instead *** 
   if (fFileName.length()==0) return false;

   if (fFilesList!=0) {
      EOUT(("Files list already exists"));
      return false;
   }

   if (fBufferSize==0) {
      EOUT(("Buffer size not specified !!!!"));
      return false;
   }

   if (strpbrk(fFileName.c_str(),"*?")!=0)
      fFilesList = dabc::mgr()->ListMatchFiles("", fFileName.c_str());
   else {
      fFilesList = new dabc::Object(0, "FilesList", true);
      new dabc::Object(fFilesList, fFileName.c_str());
   }
   ***************** */
   
/* *** PAL edit: From root v5.34 method: Int_t TChain::Add(const char* name, Long64_t nentries )
 * ***           Attempt to get Wildcard support in a OS independent way
 */ 
   TString basename = fFileName.c_str();
   // case with one single file
   if ( kFALSE == basename.MaybeWildcard()) {
      fInputFileList.Add(new TObjString(basename)); 
   } // if ( kFALSE == basename.MaybeWildcard())
   else
   {
      TString suffix = ""; // PAL => In principle not suffix in our case as we use only local files
      
      // wildcarding used in name
      Int_t slashpos = basename.Last('/');
      TString directory;
      if (slashpos>=0) {
         directory = basename(0,slashpos); // Copy the directory name
         basename.Remove(0,slashpos+1);      // and remove it from basename
      } else {
         directory = gSystem->UnixPathName(gSystem->WorkingDirectory()); // PAL: Convert UNIX path to local path
      }

      const char *file;
      const char *epath = gSystem->ExpandPathName(directory.Data());
      void *dir = gSystem->OpenDirectory(epath);
      delete [] epath;
      if (dir) {
         //create a TList to store the file names (not yet sorted)
         TList l;
         TRegexp re(basename,kTRUE);
         while ((file = gSystem->GetDirEntry(dir))) {
            if (!strcmp(file,".") || !strcmp(file,"..")) continue;
            TString s = file;
            if ( (basename!=file) && s.Index(re) == kNPOS) continue;
            l.Add(new TObjString(file));
         }
         gSystem->FreeDirectory(dir);
         //sort the files in alphanumeric order
         l.Sort();
         TIter next(&l);
         TObjString *obj;
         while ((obj = (TObjString*)next())) {
            file = obj->GetName();
            fInputFileList.Add(new TObjString( TString::Format("%s/%s%s",directory.Data(),file,suffix.Data()) )); 
         }
         l.Delete();
      }
   } // else of if ( kFALSE == basename.MaybeWildcard())
/*
   ***************** */
   
//   fInputFileList.Add(new TObjString(fFileName.c_str())); // *** PAL edit, from CbmSourceLmdNew, not support for wildcard ***

   PrintFileList(); /* *** PAL edit: Add a bit of info at startup *** */
         
   return OpenNextFile();
}

void mbs::LmdInput::PrintFileList()
{
   LOG(info) <<  "mbs::LmdInput::PrintFileList => Following files will be open/read next:";
   
   TIter next(&fInputFileList);
   TObjString *obj;
   while ((obj = (TObjString*)next())) {
      LOG(info) <<  "                                "
                << obj->GetString();
   } // while ((obj = (TObjString*)next()))
   LOG(info) <<  "mbs::LmdInput::PrintFileList => If the order is not what is planned,"
             <<  " please do not use wildcards and add files individually";
}

bool mbs::LmdInput::OpenNextFile()
{
   CloseFile();

/* *** PAL edit: Avoid using DABC data types, use ROOT ones instead *** 
   if ((fFilesList==0) || (fFilesList->NumChilds()==0)) return false;

   const char* nextfilename = fFilesList->GetChild(0)->GetName();

   bool res = fFile.OpenRead(nextfilename);
   ***************** */
   if ( 0 == fInputFileList.GetSize() ) return false;// *** PAL edit, from CbmSourceLmdNew,
   TObjString* tmp = dynamic_cast<TObjString*>(fInputFileList.At(0));
   TString str = tmp->GetString();
   const char* nextfilename = str.Data();

   LOG(info) <<  "mbs::LmdInput::OpenNextFile => "
             << " -- " << Form("%s", nextfilename);
   LOG(debug1) <<  "mbs::LmdInput::OpenNextFile => " << tmp
            << " -- " << tmp->GetString()
            << " -- " << nextfilename
            << " -- " << Form("%s", str.Data());

   bool res = fFile.OpenRead( nextfilename );

   if (!res)
      LOG(error) << Form("Cannot open file %s for reading, errcode:%u", nextfilename, fFile.LastError());
   else {
      fCurrentFileName = nextfilename;
      LOG(debug) << Form("Open lmd file %s for reading", fCurrentFileName.c_str());
   }

/* *** PAL edit: Avoid using DABC data types, use ROOT ones instead *** 
   fFilesList->DeleteChild(0);
   ***************** */
   fInputFileList.Remove( fInputFileList.	First() );// *** PAL edit, from CbmSourceLmdNew amd ROOT Website

   return res;
}


bool mbs::LmdInput::CloseFile()
{
   fFile.Close();
   fCurrentFileName = "";
   fCurrentRead = 0;
   return true;
}

/* *** PAL edit: Run offline with LMD file and without DABC *** 
unsigned mbs::LmdInput::Read_Size()
{
   // get size of the buffer which should be read from the file

   if (!fFile.IsReadMode())
      if (!OpenNextFile()) return dabc::di_EndOfStream;

   return fBufferSize;
}
   ***************** */

/* *** PAL edit: Run offline with LMD file and without DABC *** 
unsigned mbs::LmdInput::Read_Complete(dabc::Buffer& buf)
{
   unsigned numev = 0;
   uint32_t bufsize = 0;

   do {

       if (!fFile.IsReadMode()) return dabc::di_Error;

       // TODO: read into segmented buffer
       bufsize = buf.SegmentSize(0);

       numev = fFile.ReadBuffer(buf.SegmentPtr(0), bufsize);

       if (numev==0) {
          DOUT3(("File %s return 0 numev for buffer %u - end of file", fCurrentFileName.c_str(), buf.GetTotalSize()));
          if (!OpenNextFile()) return dabc::di_EndOfStream;
       }

   } while (numev==0);

   fCurrentRead += bufsize;
   buf.SetTotalSize(bufsize);
   buf.SetTypeId(mbs::mbt_MbsEvents);

   return dabc::di_Ok;
}
   ***************** */

mbs::EventHeader* mbs::LmdInput::ReadEvent()
{
   while (true) {
       if (!fFile.IsReadMode()) return 0;

       mbs::EventHeader* hdr = fFile.ReadEvent();
       if (hdr!=0) return hdr;

       LOG(debug) << Form("File %s return 0 - end of file", fCurrentFileName.c_str());
       if (!OpenNextFile()) return 0;
   }

   return 0;
}

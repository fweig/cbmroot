/************************************************************
 * The Data Acquisition Backbone Core (DABC)                *
 ************************************************************
 * Copyright (C) 2009 -                                     *
 * GSI Helmholtzzentrum fuer Schwerionenforschung GmbH      *
 * Planckstr. 1, 64291 Darmstadt, Germany                   *
 * Contact:  http://dabc.gsi.de                             *
 ************************************************************
 * This software can be used under the GPL license          *
 * agreements as stated in LICENSE.txt file                 *
 * which is part of the distribution.                       *
 ************************************************************/

#ifndef MBS_LmdInput
#define MBS_LmdInput

/* *** PAL edit: Avoid using DABC data types, use ROOT ones instead *** 
#ifndef DABC_DataIO
#include "dabc/DataIO.h"
#endif

#ifndef DABC_Object
#include "dabc/Object.h"
#endif
   ***************** */
#include "TList.h"      // *** PAL edit, from CbmSourceLmdNew ***
#include "TString.h"    // *** PAL edit, from CbmSourceLmdNew ***
#include "TObjString.h" // *** PAL edit, from CbmSourceLmdNew ***

#ifndef MBS_LmdFile
#include "mbs/LmdFile.h"
#endif

namespace mbs {

//   class LmdInput : public dabc::DataInput {
   class LmdInput {
      public:
         LmdInput(const char* fname = 0, uint32_t bufsize = 0x10000);
         virtual ~LmdInput();

/* *** PAL edit: We don't use the xml config in CbmRoot *** 
         virtual bool Read_Init(const dabc::WorkerRef& wrk, const dabc::Command& cmd);
         virtual bool Read_Init();
   ***************** */

         bool Init();
         
         void PrintFileList(); // *** PAL edit, new info method ***

/* *** PAL edit: Run offline with LMD file and without DABC *** 
         virtual unsigned Read_Size();
         virtual unsigned Read_Complete(dabc::Buffer& buf); 
   ***************** */

         // alternative way to read mbs events from LmdInput - no any dabc buffer are used
         mbs::EventHeader* ReadEvent();

      protected:
         bool CloseFile();

         bool OpenNextFile();

         std::string         fFileName;
         uint32_t            fBufferSize;

//         dabc::Object*       fFilesList; *** PAL edit *** 
         TList    fInputFileList;    // *** PAL edit, from CbmSourceLmdNew ***

         mbs::LmdFile        fFile;
         std::string         fCurrentFileName;
         uint64_t            fCurrentRead;
   };

}

#endif

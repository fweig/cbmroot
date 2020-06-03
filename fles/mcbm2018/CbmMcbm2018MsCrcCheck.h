// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          CbmMcbm2018MsCrcCheck                    -----
// -----                    Created 02.02.2019 by P.-A. Loizeau            -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMMCBM2018MSCRCCHECK_H
#define CBMMCBM2018MSCRCCHECK_H

#include "TString.h"
#include "TList.h"
#include "TObjString.h"

#include "TimesliceSubscriber.hpp"

class CbmMcbm2018MsCrcCheck
{
  public:
    CbmMcbm2018MsCrcCheck();
    virtual ~CbmMcbm2018MsCrcCheck();

    void SetFileName(TString name) { fFileName = name; fInputFileList.Add(new TObjString(name));}
    void AddFile(const char * name) {
      fInputFileList.Add(new TObjString(name));
    }

    void Run();

  private:

    TString fFileName;
    TList   fInputFileList;    ///< List of input files
    Int_t   fFileCounter;

    UInt_t fTSNumber;
    UInt_t fTSCounter;

    fles::TimesliceSource* fSource; //!

    Bool_t OpenNextFile();

    CbmMcbm2018MsCrcCheck(const CbmMcbm2018MsCrcCheck& source);
    CbmMcbm2018MsCrcCheck operator=(const CbmMcbm2018MsCrcCheck&);

    ClassDef(CbmMcbm2018MsCrcCheck, 1)
};


#endif

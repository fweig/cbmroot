// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                             CbmROCUnpack                          -----
// -----                    Created 07.11.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
#include "CbmROCUnpack.h"
#include <TObject.h>            // for TObject

CbmROCUnpack::CbmROCUnpack()
  : TObject(),
    fPersistence(kTRUE)
{
}

CbmROCUnpack::~CbmROCUnpack()
{
}

ClassImp(CbmROCUnpack)

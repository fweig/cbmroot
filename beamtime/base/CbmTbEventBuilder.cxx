// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          CbmTbEventBuilder                        -----
// -----                    Created 14.12.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
#include "CbmTbEventBuilder.h"
#include <TObject.h>            // for TObject
#include <stddef.h>             // for NULL

CbmTbEventBuilder::CbmTbEventBuilder()
  : TObject(),
    fCurrentEvent(NULL)
{
}

CbmTbEventBuilder::~CbmTbEventBuilder()
{
}

ClassImp(CbmTbEventBuilder)

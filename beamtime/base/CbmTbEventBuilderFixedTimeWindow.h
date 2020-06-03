// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmTbEventBuilderFixedTimeWindow                -----
// -----                    Created 14.12.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMTBEVENTBUILDERFIXEDTIMEWINDOW_H
#define CBMTBEVENTBUILDERFIXEDTIMEWINDOW_H

#include <Rtypes.h>             // for ClassDef
#include <RtypesCore.h>         // for Bool_t, Double_t
#include "CbmTbEvent.h"         // for CbmTbEvent
#include "CbmTbEventBuilder.h"  // for CbmTbEventBuilder
#include <boost/any.hpp>

class CbmTbEventBuilderFixedTimeWindow : public CbmTbEventBuilder
{
 public:
  
  CbmTbEventBuilderFixedTimeWindow();
  virtual ~CbmTbEventBuilderFixedTimeWindow();
    
  virtual Bool_t Init();
  virtual Bool_t IsInEvent(boost::any);
  virtual void Reset();
  virtual void PrintCurrentEvent()
  { fCurrentEvent->Print(); }

  virtual void BuildNextEvent(){;}
  
  void SetEventTimeWindow(Double_t window) { fEventTimeWindow = window; }

 private:
        
  Double_t fEventTimeWindow;
  
  ClassDef(CbmTbEventBuilderFixedTimeWindow, 1)
};

#endif

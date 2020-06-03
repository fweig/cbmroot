// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           CbmTbEventBuilder                       -----
// -----                    Created 14.12.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMTBEVENTBUILDER_H
#define CBMTBEVENTBUILDER_H

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Bool_t
#include <TObject.h>     // for TObject
#include <boost/any.hpp>
class CbmTbEvent;

class CbmTbEventBuilder : public TObject
{
 public:
  
  CbmTbEventBuilder();
  virtual ~CbmTbEventBuilder();
    
  virtual Bool_t Init() = 0;
  virtual Bool_t IsInEvent(boost::any digi) = 0;
  virtual void Reset() = 0;
  virtual void PrintCurrentEvent() = 0;

  virtual void BuildNextEvent() = 0;

 protected:
   // --- Event status
   CbmTbEvent* fCurrentEvent;    ///< Pointer to current event
         
   private:
      CbmTbEventBuilder(const CbmTbEventBuilder&);
      CbmTbEventBuilder& operator=(const CbmTbEventBuilder&);
        
   ClassDef(CbmTbEventBuilder, 0)
};

#endif

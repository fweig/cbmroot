#ifndef CBMPIXELHITSETDRAW_H_
#define CBMPIXELHITSETDRAW_H_

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Color_t, Int_t, Style_t

#include <FairPointSetDraw.h>  // for FairPointSetDraw

class TObject;
class TVector3;

class CbmPixelHitSetDraw : public FairPointSetDraw {
public:
  CbmPixelHitSetDraw();
  CbmPixelHitSetDraw(const char* name,
                     Color_t color,
                     Style_t mstyle,
                     Int_t iVerbose = 1)
    : FairPointSetDraw(name, color, mstyle, iVerbose) {};
  virtual ~CbmPixelHitSetDraw();

protected:
  TVector3 GetVector(TObject* obj);

  ClassDef(CbmPixelHitSetDraw, 1);
};

#endif /* CBMPIXELHITSETDRAW_H_ */

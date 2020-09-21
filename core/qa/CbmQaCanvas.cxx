/// \file   CbmQaCanvas.cxx
/// \brief  Implementation of the CbmQaCanvas class
/// \author Sergey Gorbunov <se.gorbunov@gsi.de>
/// \date   17.09.2020

#include "CbmQaCanvas.h"
#include "TBuffer.h"
#include "TVirtualPad.h"

ClassImp(CbmQaCanvas);

/// The Streamer is declared by ClassDef() macro
/// Stream an object of class CbmQaCanvas
///
void CbmQaCanvas::Streamer(TBuffer& R__b) {

  // Save global gPad pointer,
  // because it will be modified by TCanvas streamer
  auto store = gPad;
  if (R__b.IsReading()) {
    R__b.ReadClassBuffer(CbmQaCanvas::Class(), this);
  } else {
    R__b.WriteClassBuffer(CbmQaCanvas::Class(), this);
  }
  // restore the global pointer
  gPad = store;
}

void CbmQaCanvas::Divide2D(int nPads) {
  if (nPads < 1) nPads = 1;
  int rows = (int) sqrt(nPads);
  int cols = nPads / rows;
  if (cols * rows < nPads) cols++;
  TCanvas::Divide(cols, rows);
}

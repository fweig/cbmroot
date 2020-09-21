/// \file   CbmQaCanvas.h
/// \brief  Definition of the CbmQaCanvas class
/// \author Sergey Gorbunov <se.gorbunov@gsi.de>
/// \date   17.09.2020

#ifndef CbmQaCanvas_H
#define CbmQaCanvas_H

#include "TCanvas.h"
#include "TROOT.h"

/// A modification of TCanvas which is helpful for storing canvases in a root
/// file together with other objects.
///
/// 1. When ROOT reads a file which contains canvases, for no reason it sets
/// global gPad to the last canvas in the file, thus deactivating the browser
/// window. CbmQaCanvas modifies the standard TCanvas::Streamer() to suppress
/// this feature.
///
/// 2. The CbmQaCanvas is created in the batch mode, i.e. it is not
/// automatically displayed.
///
/// 3. Please use DrawCopy() methods instead of Draw(), to draw objects in the
/// canvas. Otherwise the same object can be stored several times in the file;
/// later this will cause crashes in the ROOT Browser for whatever reason.
///
class CbmQaCanvas : public TCanvas {
public:
  /// Default constructor needed by the ROOT streamer
  CbmQaCanvas() : TCanvas() {}

  /// Reimplementation of any existing TCanvas constructor
  /// It sets the batch mode ON and then calls the constructor
  template<typename... Types>
  CbmQaCanvas(Types... args) : CbmQaCanvas(SetBatchModeOn(), args...) {}

  /// Destructor
  virtual ~CbmQaCanvas() {}

  /// Divide canvas into nPads in 2D in a nice way
  void Divide2D(int nPads);

private:
  /// Use a specific type name in order to avoid ambiguities when unrolling
  /// templates
  enum MyBoolean : Bool_t;

  /// Constructor which calls TCanvas constructor and restores the bach mode
  template<typename... Types>
  CbmQaCanvas(MyBoolean oldBatchMode, Types... args) : TCanvas(args...) {
    gROOT->SetBatch((Bool_t) oldBatchMode);
  }

  /// Set batch mode ON and return its old value
  static MyBoolean SetBatchModeOn() {
    MyBoolean oldBatchMode = (MyBoolean) gROOT->IsBatch();
    gROOT->SetBatch(kTRUE);
    return oldBatchMode;
  }

  ClassDef(CbmQaCanvas, 1);
};

#endif

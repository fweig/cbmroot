/*
 * NicaFemto3DCF.h
 *
 *  Created on: 12-03-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAFEMTO3DCF_H_
#define NICAFEMTO3DCF_H_

#include <TString.h>

#include "NicaArray.h"
#include "NicaDividedHisto.h"
#include "NicaFemtoConst.h"

class NicaCorrFit3DCF;
/**
 * basic class for storing 3-dim correlation functions
 */
class NicaFemto3DCF : public NicaDividedHisto3D {
  /**
   * frame of pairs in CF
   */
  const ENicaFemtoKinematics fFrame;
  /**
   * set assumed names of axes
   * @param h - adress of numerator or denominator
   */
  virtual void SetAxisNames(TH1* h);
  /**
   * extract draw range pattern
   * @param pattern
   * @param min
   * @param max
   * @return
   */
  Bool_t ExtrDraw(TString& pattern, Double_t& min, Double_t& max) const;
  virtual TH1D* Projection3DTo1D(Double_t min1, Double_t max1, Double_t min2, Double_t max2, Option_t* opt) const;

public:
  /**
   * constructor for streamer
   */
  NicaFemto3DCF() : NicaDividedHisto3D(), fFrame(ENicaFemtoKinematics::kLCMS) {};
  /**
   * default constructor
   * @param name name of CF's
   * @param frame frame of pairs
   */
  NicaFemto3DCF(TString name, ENicaFemtoKinematics frame = ENicaFemtoKinematics::kLCMS);
  NicaFemto3DCF(TString name,
                Int_t binsX,
                Double_t minX,
                Double_t maxX,
                Int_t binsY,
                Double_t minY,
                Double_t maxY,
                Int_t binsZ,
                Double_t minZ,
                Double_t maxZ,
                ENicaFemtoKinematics frame = ENicaFemtoKinematics::kLCMS);
  NicaFemto3DCF(const NicaFemto3DCF& other);
  /**
   *
   * @return pair kinematics
   */
  ENicaFemtoKinematics GetFrame() const { return fFrame; };
  virtual void FillNumObj(TObject* ob);
  virtual void FillDenObj(TObject* obj);
  virtual void AddNum(TH1* h, Option_t* opt = "");
  virtual void AddDen(TH1* h, Option_t* opt = "");
  /**
   * fit this function
   */
  void Fit(NicaCorrFit3DCF* fit);
  /**
   * make "dummy fit" (allow to draw function )
   * does not perform any fit
   * @param fit
   */
  void FitDummy(NicaCorrFit3DCF* fit);
  virtual void Browse(TBrowser* b);
  /**
   * draw this object
   * @param opt if "num" then only numerator is drawn, if "den" only denominator
   * is drawn,  , if "all"  then draw everything (numerator, denominator, cf)
   * current pad is divided and numertor, if "cf" or not speciified
   * theon only projections of cf's are drawn, user can also specify additional
   * option for TH1->Draw like "num+same" denominator and CF are drawn,
   * otherwise only divided histogram is drawn
   *
   * Standard option of draw is "cf" this option works also with option "norm" (draw normalized) "rgb" (each projection have
   * different color
   * Option "diag1" lub "diag2" draw diagonal cf's,  option "norm" is also available for such type of drawing
   */
  virtual void Draw(Option_t* opt = "cf+rgb+norm");
  /**
   * draw scaled projection of CF
   * @param scale scale (norm)
   * @param opt option like for TH1
   */
  virtual void DrawScaled(Double_t scale, Option_t* opt);
  virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
  /**
   *
   * @return numerator exported as flat array
   */
  virtual NicaArray_1<Float_t>* ExportToFlatNum() const;
  virtual TH1D** GetDiagProj(Option_t* opt = "diag1", Bool_t normalized = kTRUE) const;
  virtual void Print(Option_t* opt = "") const;
  virtual ~NicaFemto3DCF();
  ClassDef(NicaFemto3DCF, 3)
};

#endif /* NICAFEMTO3DCF_H_ */

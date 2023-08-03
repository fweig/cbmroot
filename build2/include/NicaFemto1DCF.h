/*
 * NicaFemto1DCF.h
 *
 *  Created on: 12-03-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 *		Experimental class
 */

#ifndef NICAFEMTO1DCF_H_
#define NICAFEMTO1DCF_H_

#include <TString.h>

#include "NicaArray.h"
#include "NicaDividedHisto.h"
#include "NicaFemtoConst.h"

class NicaCorrFit1DCF;
/**
 * basic class for storing 1-dim correlation functions
 */
class NicaFemto1DCF : public NicaDividedHisto1D {
  const ENicaFemtoKinematics fFrame;

protected:
  virtual TString GetPic() const;
  /**
   * set assumed names of axes
   * @param h - adress of numerator or denominator
   */
  virtual void SetAxisNames(TH1* h);

public:
  NicaFemto1DCF();
  /**
   * default ctor
   * @param name name of function
   * @param frame frame
   */
  NicaFemto1DCF(TString name, ENicaFemtoKinematics frame = ENicaFemtoKinematics::kLCMS);
  /**
   * another constructor
   * @param name
   * @param bins
   * @param min
   * @param max
   * @param frame
   */
  NicaFemto1DCF(TString name, Int_t bins, Double_t min, Double_t max, ENicaFemtoKinematics frame = ENicaFemtoKinematics::kLCMS);
  /**
   * copy constructor
   * @param other
   */
  NicaFemto1DCF(const NicaFemto1DCF& other);
  /**
   *
   * @return kinematics mode
   */
  ENicaFemtoKinematics GetFrame() const { return fFrame; };
  virtual void AddNum(TH1* h, Option_t* opt = "");
  virtual void AddDen(TH1* h, Option_t* opt = "");
  void FillNumObj(TObject* obj);
  void FillDenObj(TObject* obj);
  /**
   * fit this function
   */
  void Fit(NicaCorrFit1DCF* fit);
  /**
   * make "dummy fit" (allow to draw function )
   * does not perform any fit
   * @param fit
   */
  void FitDummy(NicaCorrFit1DCF* fit);
  virtual void Browse(TBrowser* b);
  /**
   * draw scaled C-f
   * @param scale scale (norm)
   * @param opt option like for TH1
   */
  virtual void DrawScaled(Double_t scale, Option_t* opt);
  /**
   * extract this CF into web page
   * @param counter number of this object
   * @param dir directory where extract object
   * @return url to extracted object
   */
  virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
  /**
   *
   * @return numerator exported as flat array
   */
  virtual NicaArray_1<Float_t>* ExportToFlatNum() const;
  virtual void Print(Option_t* opt = "") const;
  virtual ~NicaFemto1DCF();
  ClassDef(NicaFemto1DCF, 3)
};

#endif /* NICAFEMTO1DCF_H_ */

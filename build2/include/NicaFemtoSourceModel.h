/*
 * NicaFemtoSourceModel.h
 *
 *  Created on: 26-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAFEMTOSOURCEMODEL_H_
#define NICAFEMTOSOURCEMODEL_H_

#include <TRandom.h>
#include <TString.h>


class NicaFemtoPair;
class NicaPackage;

/**
 * basic class for all source models
 */
class NicaFemtoSourceModel : public TObject {
private:
  /**
   * numer of parameters
   */
  const Int_t fParametersNo;
  Double_t* fParams;  //[fParametersNo]
  /**
   * array of parameter names
   */
  TString* fParameterNames;  //[fParametersNo]
protected:
  /**
   * used for generating random coordinates
   */
  TRandom* fRandom;
  /**
   * current out-radius
   */
  Double_t fRout;
  /**
   * current side radius
   */
  Double_t fRside;
  /**
   * current long radius
   */
  Double_t fRlong;
  /**
   * name of this model
   */
  TString fModelName;
  void SetParName(TString name, Int_t no) { fParameterNames[no] = name; };
  // NicaFemtoSourceModel();
public:
  /**
   * default ctor
   */
  NicaFemtoSourceModel(Int_t nparams);
  /**
   * copy ctor
   * @param model
   */
  NicaFemtoSourceModel(const NicaFemtoSourceModel& model);
  /**
   *
   * @return  generated out radius
   */
  inline Double_t GetROut() const { return fRout; };
  /**
   *
   * @return generated  out radius
   */
  inline Double_t GetRSide() const { return fRside; };
  /**
   *
   * @return generated long out radius
   */
  inline Double_t GetRLong() const { return fRlong; };
  /**
   * set generator seed
   * @param seed
   */
  void SetSeed(UInt_t seed) { fRandom->SetSeed(seed); };
  /**
   * se
   * @param par_val value of parameter
   * @param par_no parameter number
   */
  inline void SetParameter(Double_t par_val, Int_t par_no) { fParams[par_no] = par_val; };
  /**
   * set value of parameter
   * @param par
   * @param name
   */
  void SetParameterByName(Double_t par, TString name);
  /**
   * calculate coordinates
   */
  virtual void GenerateCoordinates(NicaFemtoPair* Pair) = 0;
  /**
   * set 1-dim radius, if source is 3D that each radii componet will be scaled
   * by 1/sqrt(3) (e.g. r_out = radii/sqrt(3)
   * @param radii
   */
  virtual void SetRadius(Double_t /*radii*/) {};
  /**
   *
   * @return number of parameters
   */
  Int_t GetNpar() const { return fParametersNo; };
  /**
   *
   * @param n
   * @return name of n-th parameter
   */
  TString GetParamName(Int_t n) const;
  /**
   * return value of parameter with numer N
   * @param n
   * @return
   */
  Double_t GetParameter(Int_t n) const { return fParams[n]; };
  virtual void Print(Option_t* option = "") const;
  /**
   *
   * @return copy of this
   */
  virtual NicaFemtoSourceModel* MakeCopy() const = 0;
  /**
   * create package with report
   * @return package with report
   */
  virtual NicaPackage* Report() const;
  virtual ~NicaFemtoSourceModel();
  ClassDef(NicaFemtoSourceModel, 1)
};
/**
 * class for generating of coordinates for 1Dim sources
 */
class NicaFemtoSourceModel1D : public NicaFemtoSourceModel {
protected:
  NicaFemtoSourceModel1D(Int_t par);

public:
  NicaFemtoSourceModel1D();
  /**
   * copy ctor
   * @param model
   */
  NicaFemtoSourceModel1D(const NicaFemtoSourceModel1D& model);
  /**
   * set generated radiius
   * @param radii
   */
  void SetRadius(Double_t radii) { SetParameter(radii, 0); };
  virtual NicaFemtoSourceModel* MakeCopy() const = 0;
  virtual ~NicaFemtoSourceModel1D();
  ClassDef(NicaFemtoSourceModel1D, 1)
};
/**
 * abstract class for all 3-dim sources
 */
class NicaFemtoSourceModel3D : public NicaFemtoSourceModel {
protected:
  NicaFemtoSourceModel3D(Int_t no);

public:
  NicaFemtoSourceModel3D();
  /**
   * copy ctor
   * @param model
   */
  NicaFemtoSourceModel3D(const NicaFemtoSourceModel3D& model);
  /**
   * set out radius
   * @param out radius
   */
  void SetOutRadius(Double_t out) { SetParameter(out, 0); };
  /**
   * set side radius
   * @param side radius
   */
  void SetSideRadius(Double_t side) { SetParameter(side, 1); };
  /**
   * set long radius
   * @param longr radius
   */
  void SetLongRadius(Double_t longr) { SetParameter(longr, 2); };
  /**
   * set out long and side radii where R-out/side/long = radii/sqrt(3)
   * @param radii
   */
  virtual void SetRadius(Double_t radii);
  virtual NicaFemtoSourceModel* MakeCopy() const = 0;
  virtual ~NicaFemtoSourceModel3D();
  ClassDef(NicaFemtoSourceModel3D, 1)
};

#endif /* NICAFEMTOSOURCEMODEL_H_ */

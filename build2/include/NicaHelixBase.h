/*
 * NicaHelixBase.h
 *
 *  Created on: 7 wrz 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAHELIXBASE_H_
#define NICAHELIXBASE_H_

#include <TObject.h>
#include <TVector3.h>
/**
 * representation of helix in magnetic field with constatn Bz
 * based on MpdHelix class from MpdROOT
 */
class NicaHelixBase : public TObject {
private:
  static Double_t fgHelixBz;
  Double_t fX0, fY0, fZ0;
  Double_t fPhi0,fPhiCos,fPhiSin;
  Double_t fLambda,fLambdaCos,fLambdaSin;
  Double_t fCurv;
  Double_t fXc, fYc;
  Double_t fH;
  static Double_t GetBz();
  Double_t FudgePathLenght(const TVector3 vec) const;
  Double_t Distance(const TVector3 p, Bool_t scanPeriods = kTRUE) const;
protected:
  /**
   *
   * @param x X start postion
   * @param y Y start position
   * @param z Z start position
   * @param px px-momentum
   * @param py py momentum
   * @param pz pz momentum
   * @param charge
   * @param conversion
   */
  NicaHelixBase(Double_t x,
                Double_t y,
                Double_t z,
                Double_t px,
                Double_t py,
                Double_t pz,
                Double_t charge,
                Double_t conversion = 0.);
  /**
   * calculate path lenghts for intersection of helix and circle with center x,y
   * @param r circle radii
   * @param x x-circle center
   * @param y y-circle center
   * @param s1 first path
   * @param s2 second path
   */
  void BasePathLength(Double_t r, Double_t x, Double_t y, Double_t& s1, Double_t& s2) const;
  /**
   *
   * @param x start position
   * @param mom momentum at X
   * @param charge charge of particle
   * @param conversion not used
   */
  void BaseSetParams(TVector3& x, TVector3& mom, Double_t charge, Double_t conversion = 0.);
  /**
   *
   * @return x-center
   */
  inline Double_t BaseGetXcenter() const { return fXc; };
  /**
   *
   * @return y-center
   */
  inline Double_t BaseGetYcenter() const { return fYc; };
  /**
   *
   * @return start X
   */
  inline Double_t BaseGetStartX() const { return fX0; };
  /**
   *
   * @return start Y
   */
  inline Double_t BaseGetStartY() const { return fY0; };
  /**
   *
   * @return start Z
   */
  inline Double_t BaseGetStartZ() const { return fZ0; };
  /**
   *
   * @param point
   * @param scanPeriods true if scan perdios
   * @return path at DCA between helix and point
   */
  Double_t BasePathLength(TVector3 point, Bool_t scanPeriods = kTRUE) const;
  /**
   *
   * @param x point x-coordiante
   * @param y point y-coordinate
   * @return path lenght at shortest distance between point and helix at xy
   * plane
   */
  Double_t BasePathLength(Double_t x, Double_t y) const;
  /**
   *
   * @param s
   * @return helix corrdinates at lengith s
   */
  TVector3 BasePosition(Double_t s) const;
  /**
   *
   * @return star point of helix
   */
  TVector3 BaseGetStartPoint() const;
  /**
   * calculate momentum at given point
   * @return
   */
  TVector3 BaseMomentum(Double_t s) const;
  /**
   * calculate two points where circle intersect with cilider at given R
   * @param R
   * @param x1
   * @param x2
   * @return number of solutions
   */
  Int_t BaseIntersection(Double_t R, TVector3& x1, TVector3& x2) const;
  void BasePrint() const;
  /**
   * calculate path lenght at fixed radii perpendicular to magnetic field
   * @param r radii
   * @param s1 first path
   * @param s2 second path
   */
  void BasePathLength(Double_t r, Double_t& s1, Double_t& s2) const;
  /**
   * shift this vector by given coordinates
   * @param x
   * @param y
   * @param z
   */
  void BaseShift(Double_t x, Double_t y, Double_t z);
  /**
   * calculate the momentum and position at given s
   * @param s
   * @param mom
   * @param pos
   */
  void BaseFullEval(Double_t s, TVector3 &mom, TVector3 &pos)const;

public:
  NicaHelixBase();
  NicaHelixBase(const NicaHelixBase& t);
  /**
   *
   * @param x initial position
   * @param mom momentum at x
   * @param charge electric charge in e
   * @param conversion not used
   */
  NicaHelixBase(TVector3 x, TVector3 mom, Double_t charge, Double_t conversion = 0.);
  /**
   *
   * @return curvature of helix
   */
  inline Double_t GetCurv() const { return fCurv; };
  /**
   *
   * @return phase 0
   */
  inline Double_t GetPhi0() const { return fPhi0; };
  /**
   *
   * @return lambda angle
   */
  inline Double_t GetDipAngle() const { return fLambda; };
  /**
   *
   * @return qB
   */
  inline Double_t GetH() const { return fH; };
  /**
   * assignement operator
   * @param helix
   * @return
   */
  NicaHelixBase& operator=(const NicaHelixBase& helix);
  /**
   * return path lendhts at shortest distance between two helices
   * @param h secon helix
   * @param s1 path lenght ot this helix
   * @param s2 path lenght of h-helix
   */
  void PathLengths(const NicaHelixBase& h, Double_t& s1, Double_t& s2) const;
  /**
   * set magnetic fiels (for all helixes)
   * @param Bz
   */
  static void SetMagField(Double_t Bz);
  /**
   * return minimal non-negative value of path
   * @param s1
   * @param s2
   * @return
   */
  Double_t GetPathAbsMin(Double_t s1, Double_t s2) const;
  /**
   *
   * @return helix period
   */
  Double_t GetPeriod() const;
  /**
   *
   * @return -1 if particle goes clockwise
   */
  Double_t GetRotationDirection() const;
  /**
   *
   * @return max path
   */
  static Double_t MaxPath() { return 1E+9; };
  virtual ~NicaHelixBase();
  ClassDef(NicaHelixBase, 1)
};

#endif /* NICAHELIXBASE_H_ */

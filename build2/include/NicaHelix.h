/*
 * NicaHelix.h
 *
 *  Created on: 3 sty 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 *		Based on AlixHelix paremetrizatioin with const Bz
 *		(https://github.com/alisw/AliRoot/blob/master/STEER/STEER/AliHelix.h)
 *		and MpdHelix
 *		(https://git.jinr.ru/nica/mpdroot//mpdbase/MpdHelix.h)
 */
#ifndef NICAHELIX_H_
#define NICAHELIX_H_

#include "NicaHelixBase.h"

/**
 * helix with non-zero constatn Bz  field
 * field is set in Tesla, momenta in GeV/c and positions in cm
 */
class NicaHelixZ : public NicaHelixBase {
 public:
  NicaHelixZ();
  /**
   *
   * @param x initial position
   * @param mom momentum at x
   * @param charge electric charge in e
   * @param conversion not used
   */
  NicaHelixZ(TVector3 x, TVector3 mom, Double_t charge,
             Double_t conversion = 0.);
  /**
   * calculate path lenght at fixed radii perpendicular to magnetic field
   * @param r radii
   * @param s1 first path
   * @param s2 second path
   */
  inline void PathLength(Double_t r, Double_t &s1, Double_t &s2) const {
    BasePathLength(r, s1, s2);
  }
  /**
   * calculate path lenghts for intersection of helix and circle with center x,y
   * @param r circle radii
   * @param x x-circle center
   * @param y y-circle center
   * @param s1 first path
   * @param s2 second path
   */
  inline void PathLength(Double_t r, Double_t x, Double_t y, Double_t &s1,
                         Double_t &s2) const {
    BasePathLength(r, x, y, s1, s2);
  }
  /**
   *
   * @param x start position
   * @param mom momentum at X
   * @param charge charge of particle
   * @param conversion not used
   */
  inline void SetParams(TVector3 &x, TVector3 &mom, Double_t charge,
                        Double_t conversion = 0.) {
    BaseSetParams(x, mom, charge, conversion);
  }
  /**
   * calculates the momentum and position of trajectory at given s
   * @param s
   * @param mom
   * @param pos
   */
  inline void EvaluateFull(Double_t s, TVector3 &mom, TVector3 &pos){
      BaseFullEval(s, mom, pos);
  }
  /**
   *
   * @return x-center
   */
  inline Double_t GetXcenter() const { return BaseGetXcenter(); };
  /**
   *
   * @return y-center
   */
  inline Double_t GetYcenter() const { return BaseGetYcenter(); };
  /**
   *
   * @return zero
   */
  inline Double_t GetZcenter() const { return 0; };
  /**
   *
   * @return start X
   */
  inline Double_t GetStartX() const { return BaseGetStartX(); };
  /**
   *
   * @return start Y
   */
  inline Double_t GetStartY() const { return BaseGetStartY(); };
  /**
   *
   * @return start Z
   */
  inline Double_t GetStartZ() const { return BaseGetStartZ(); };
  /**
   *
   * @param point
   * @param scanPeriods true if scan perdios
   * @return path at DCA between helix and point
   */
  inline Double_t PathLength(TVector3 point, Bool_t scanPeriods = kTRUE) const {
    return BasePathLength(point, scanPeriods);
  }
  /**
   *
   * @param x point x-coordiante
   * @param y point y-coordinate
   * @return path lenght at shortest distance between point and helix at xy
   * plane
   */
  Double_t PathLength(Double_t x, Double_t y) const {
    return BasePathLength(x, y);
  }
  /**
   *
   * @param s
   * @return helix corrdinates at lengith s
   */
  TVector3 EvalPos(Double_t s) const { return BasePosition(s); }
  /**
   *
   * @param s
   * @return momentum at s
   */
  TVector3 EvalMom(Double_t s) const { return BaseMomentum(s); };
  /**
   *
   * @return star point of helix
   */
  TVector3 GetStartPoint() const { return BaseGetStartPoint(); };
  /**
   * calculate two points where circle intersect with cilider at given R
   * @param R
   * @param x1
   * @param x2
   * @return number of solutions
   */
  inline Int_t Intersection(Double_t R, TVector3 &x1, TVector3 &x2) const {
    return BaseIntersection(R, x1, x2);
  }
  /**
   * assignement operator
   * @param helix
   * @return
   */
  NicaHelixZ &operator=(const NicaHelixZ &helix);
  /**
   * shift this helix by given values
   * @param x
   * @param y
   * @param z
   */
  void Shift(Double_t x, Double_t y, Double_t z);
  void Print(Option_t * /*opt*/ = "") const { BasePrint(); };
  virtual ~NicaHelixZ(){};
  ClassDef(NicaHelixZ, 1)
};

/**
 * helix with non-zero constant Bx  field
 */
class NicaHelixX : public NicaHelixBase {
 public:
  NicaHelixX();
  /**
   *
   * @param x initial position
   * @param mom momentum at x
   * @param charge electric charge in e
   * @param conversion not used
   */
  NicaHelixX(TVector3 x, TVector3 mom, Double_t charge,
             Double_t conversion = 0.);
  inline void PathLength(Double_t r, Double_t &s1, Double_t &s2) const {
    BasePathLength(r, s1, s2);
  }
  /**
   * calculate path lenghts for intersection of helix and circle with center x,y
   * @param r circle radii
   * @param y y-circle center
   * @param z z-circle center
   * @param s1 first path
   * @param s2 second path
   */
  inline void PathLength(Double_t r, Double_t y, Double_t z, Double_t &s1,
                         Double_t &s2) const {
    BasePathLength(r, y, z, s1, s2);
  }
  /**
   *
   * @param x start position
   * @param mom momentum at X
   * @param charge charge of particle
   * @param conversion not used
   */
  inline void SetParams(TVector3 &x, TVector3 &mom, Double_t charge,
                        Double_t conversion = 0.) {
    x.SetXYZ(x.Y(), x.Z(), x.X());
    mom.SetXYZ(mom.Y(), mom.Z(), mom.X());
    BaseSetParams(x, mom, charge, conversion);
  }
  /**
   * calculates the momentum and position of trajectory at given s
   * @param s
   * @param mom
   * @param pos
   */
  inline void EvaluateFull(Double_t s, TVector3 &mom, TVector3 &pos){
      BaseFullEval(s, mom, pos);
      mom.SetXYZ(mom.Z(), mom.X(), mom.Y());
      pos.SetXYZ(pos.Z(), pos.X(), pos.Y());
  }
  /**
   *
   * @return x-center
   */
  inline Double_t GetXcenter() const { return 0; };
  /**
   *
   * @return zeo
   */
  inline Double_t GetYcenter() const { return BaseGetXcenter(); };
  /**
   *
   * @return z-center
   */
  inline Double_t GetZcenter() const { return BaseGetYcenter(); };
  /**
   *
   * @return start X
   */
  inline Double_t GetStartX() const { return BaseGetStartZ(); };
  /**
   *
   * @return start Y
   */
  inline Double_t GetStartY() const { return BaseGetStartX(); };
  /**
   *
   * @return start Z
   */
  inline Double_t GetStartZ() const { return BaseGetStartY(); };
  /**
   *
   * @param point
   * @param scanPeriods true if scan perdios
   * @return path at DCA between helix and point
   */
  inline Double_t PathLength(TVector3 point, Bool_t scanPeriods = kTRUE) const {
    point.SetXYZ(point.Y(), point.Z(), point.X());
    return BasePathLength(point, scanPeriods);
  }
  /**
   *
   * @param y point y-coordiante
   * @param z point z-coordinate
   * @return path lenght at shortest distance between point and helix at xy
   * plane
   */
  Double_t PathLength(Double_t y, Double_t z) const {
    return BasePathLength(y, z);
  }
  /**
   *
   * @param s
   * @return helix corrdinates at lengith s
   */
  TVector3 EvalPos(Double_t s) const {
    TVector3 res = BasePosition(s);
    res.SetXYZ(res.Z(), res.X(), res.Y());
    return res;
  }
  /**
   *
   * @param s
   * @return momentum at s
   */
  TVector3 EvalMom(Double_t s) const {
    TVector3 res = BaseMomentum(s);
    res.SetXYZ(res.Z(), res.X(), res.Y());
    return res;
  };
  /**
   *
   * @return star point of helix
   */
  TVector3 GetStartPoint() const {
    TVector3 res = BaseGetStartPoint();
    res.SetXYZ(res.Z(), res.X(), res.Y());
    return res;
  };
  /**
   * calculate two points where circle intersect with cilider at given R
   * @param R
   * @param x1
   * @param x2
   * @return number of solutions
   */
  inline Int_t Intersection(Double_t R, TVector3 &x1, TVector3 &x2) const {
    Int_t res = BaseIntersection(R, x1, x2);
    x1.SetXYZ(x1.Z(), x1.X(), x1.Y());
    x2.SetXYZ(x2.Z(), x2.X(), x2.Y());
    return res;
  }
  /**
   * assignement operator
   * @param helix
   * @return
   */
  NicaHelixX &operator=(const NicaHelixX &helix);
  void Print(Option_t * /*opt*/ = "") const {};
  /**
   * shift this helix by given values
   * @param x
   * @param y
   * @param z
   */
  void Shift(Double_t x, Double_t y, Double_t z);
  virtual ~NicaHelixX(){};
  ClassDef(NicaHelixX, 1)
};

/**
 * helix with non-zero constant By  field
 */
class NicaHelixY : public NicaHelixBase {
 public:
  NicaHelixY();
  /**
   *
   * @param x initial position
   * @param mom momentum at x
   * @param charge electric charge in e
   * @param conversion not used
   */
  NicaHelixY(TVector3 x, TVector3 mom, Double_t charge,
             Double_t conversion = 0.);
  inline void PathLength(Double_t r, Double_t &s1, Double_t &s2) const {
    BasePathLength(r, s1, s2);
  }
  /**
   * calculate path lenghts for intersection of helix and circle with center x,y
   * @param r circle radii
   * @param z z-circle center
   * @param x x-circle center
   * @param s1 first path
   * @param s2 second path
   */
  inline void PathLength(Double_t r, Double_t z, Double_t x, Double_t &s1,
                         Double_t &s2) const {
    BasePathLength(r, z, x, s1, s2);
  }
  /**
   *
   * @param x start position
   * @param mom momentum at X
   * @param charge charge of particle
   * @param conversion not used
   */
  inline void SetParams(TVector3 &x, TVector3 &mom, Double_t charge,
                        Double_t conversion = 0.) {
    x.SetXYZ(x.Z(), x.X(), x.Y());
    mom.SetXYZ(mom.Z(), mom.X(), mom.Y());
    BaseSetParams(x, mom, charge, conversion);
  }
  /**
   * calculates the momentum and position of trajectory at given s
   * @param s
   * @param mom
   * @param pos
   */
  inline void EvaluateFull(Double_t s, TVector3 &mom, TVector3 &pos){
      BaseFullEval(s, mom, pos);
      mom.SetXYZ(mom.Y(), mom.Z(), mom.X());
      pos.SetXYZ(pos.Y(), pos.Z(), pos.X());
  }

  /**
   *
   * @return x-center
   */
  inline Double_t GetXcenter() const { return BaseGetYcenter(); };
  /**
   *
   * @return 0
   */
  inline Double_t GetYcenter() const { return 0; };
  /**
   *
   * @return z-center
   */
  inline Double_t GetZcenter() const { return BaseGetXcenter(); };
  /**
   *
   * @return start X
   */
  inline Double_t GetStartX() const { return BaseGetStartY(); };
  /**
   *
   * @return start Y
   */
  inline Double_t GetStartY() const { return BaseGetStartZ(); };
  /**
   *
   * @return start Z
   */
  inline Double_t GetStartZ() const { return BaseGetStartX(); };
  /**
   *
   * @param point
   * @param scanPeriods true if scan perdios
   * @return path at DCA between helix and point
   */
  inline Double_t PathLength(TVector3 point, Bool_t scanPeriods = kTRUE) const {
    point.SetXYZ(point.Z(), point.X(), point.Y());
    return BasePathLength(point, scanPeriods);
  }
  /**
   *
   * @param z point z-coordiante
   * @param x point x-coordinate
   * @return path lenght at shortest distance between point and helix at xy
   * plane
   */
  Double_t PathLength(Double_t z, Double_t x) const {
    return BasePathLength(z, x);
  }
  /**
   *
   * @param s
   * @return helix corrdinates at lengith s
   */
  TVector3 EvalPos(Double_t s) const {
    TVector3 res = BasePosition(s);
    res.SetXYZ(res.Y(), res.Z(), res.X());
    return res;
  }
  /**
   *
   * @param s
   * @return momentum at s
   */
  TVector3 EvalMom(Double_t s) const {
    TVector3 res = BaseMomentum(s);
    res.SetXYZ(res.Y(), res.Z(), res.X());
    return res;
  };
  /**
   *
   * @return star point of helix
   */
  TVector3 GetStartPoint() const {
    TVector3 res = BaseGetStartPoint();
    res.SetXYZ(res.Y(), res.Z(), res.X());
    return res;
  };
  /**
   * calculate two points where circle intersect with cilider at given R
   * @param R
   * @param x1
   * @param x2
   * @return number of solutions
   */
  inline Int_t Intersection(Double_t R, TVector3 &x1, TVector3 &x2) const {
    Int_t res = BaseIntersection(R, x1, x2);
    x1.SetXYZ(x1.Y(), x1.Z(), x1.X());
    x2.SetXYZ(x2.Y(), x2.Z(), x2.X());
    return res;
  }
  /**
   * assignement operator
   * @param helix
   * @return
   */
  NicaHelixY &operator=(const NicaHelixY &helix);
  void Print(Option_t * /*opt*/ = "") const { BasePrint(); };
  /**
   * shift this helix by given values
   * @param x
   * @param y
   * @param z
   */
  void Shift(Double_t x, Double_t y, Double_t z);
  virtual ~NicaHelixY(){};
  ClassDef(NicaHelixY, 1)
};

#ifdef NICAHELIX_X
typedef NicaHelixX NicaHelix;
#else
#ifdef NICAHELIX_Y
typedef NicaHelixY NicaHelix;
#else
typedef NicaHelixZ NicaHelix;
#endif
#endif

#endif /* NICAHELIX_H_ */

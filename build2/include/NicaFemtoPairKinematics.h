/*
 * NicaFemtoPair.h
 *
 *  Created on: 30-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAFEMTOPAIRKINEMATICS_H_
#define NICAFEMTOPAIRKINEMATICS_H_

#include <TMath.h>
#include <TString.h>

#include "NicaFemtoConst.h"
#include "NicaFemtoPair.h"

/**
 * calculate pair kinematics in PRF frame
 */
class NicaFemtoPairPRF : public NicaFemtoPair {
 public:
  NicaFemtoPairPRF(Bool_t use_mc = kFALSE) : NicaFemtoPair(use_mc){};
  void Compute();
  void Compute_Rotated();
  void Compute_Hemisphere();
  ENicaFemtoKinematics GetFrame() const { return ENicaFemtoKinematics::kPRF; };
  TString GetTags() const { return "prf"; };
  virtual ~NicaFemtoPairPRF(){};
  ClassDef(NicaFemtoPairPRF, 1)
};

/**
 * abstract class for all kinematics that return values in r, phi, eta
 */
class NicaFemtoPair_SH : public NicaFemtoPair {
 protected:
  /**
   * convert fX, fY, fZ for cartesian to spherical values
   */
  void ConvertToSh();

 public:
  NicaFemtoPair_SH(Bool_t use_mc = kFALSE) : NicaFemtoPair(use_mc){};
  virtual ~NicaFemtoPair_SH(){};
  ClassDef(NicaFemtoPair_SH, 1)
};

/**
 * calculate pair kinematics in LCMS frame
 */
class NicaFemtoPairLCMS : public NicaFemtoPair {
 public:
  NicaFemtoPairLCMS(Bool_t use_mc = kFALSE) : NicaFemtoPair(use_mc) {}
  void Compute();
  void Compute_Rotated();
  void Compute_Hemisphere();
  ENicaFemtoKinematics GetFrame() const { return ENicaFemtoKinematics::kLCMS; };
  TString GetTags() const { return "lcms"; };
  virtual ~NicaFemtoPairLCMS(){};
  ClassDef(NicaFemtoPairLCMS, 1)
};

/**
 * calculate pair kinematics in PRF frame but return values in SH frame (r, phi,
 * eta)
 */
class NicaFemtoPairPRF_SH : public NicaFemtoPair_SH {
 public:
  NicaFemtoPairPRF_SH(Bool_t use_mc = kFALSE) : NicaFemtoPair_SH(use_mc) {}
  void Compute();
  void Compute_Rotated();
  void Compute_Hemisphere();
  ENicaFemtoKinematics GetFrame() const { return ENicaFemtoKinematics::kSH_PRF; };
  TString GetTags() const { return "prf+sh"; };
  virtual ~NicaFemtoPairPRF_SH(){};
  ClassDef(NicaFemtoPairPRF_SH, 1)
};

/**
 * calculate pair kinematics in LCMS frame but return value in SH frame (r, phi,
 * eta)
 */
class NicaFemtoPairLCMS_SH : public NicaFemtoPair_SH {
 public:
  NicaFemtoPairLCMS_SH(Bool_t use_mc = kFALSE) : NicaFemtoPair_SH(use_mc) {}
  void Compute();
  void Compute_Rotated();
  void Compute_Hemisphere();
  ENicaFemtoKinematics GetFrame() const { return ENicaFemtoKinematics::kSH_LCMS; };
  TString GetTags() const { return "lcms+prf"; };
  virtual ~NicaFemtoPairLCMS_SH(){};
  ClassDef(NicaFemtoPairLCMS_SH, 1)
};

/**
 * calculate pair kinematics in delta-eta delta-phi frame
 */
class NicaFemtoPairDPhiDEta : public NicaFemtoPair {
  Double_t fPhiMin, fPhiMax;
  Double_t fPI2;

 public:
  /**
   * default constructor
   * @param phi_min mimal value of phi
   */
  NicaFemtoPairDPhiDEta(Bool_t use_mc = kFALSE,
                        Double_t phi_min = -TMath::Pi() * 0.5);
  void Compute();
  void Compute_Rotated();
  void Compute_Hemisphere();
  ENicaFemtoKinematics GetFrame() const { return ENicaFemtoKinematics::kPHIETA; };
  TString GetTags() const { return "phieta"; };
  ~NicaFemtoPairDPhiDEta(){};
  ClassDef(NicaFemtoPairDPhiDEta, 1)
};

/**
 * class for making kinematics with "lednicky prf" where boost along all
 * direction is made at the same time
 */
class NicaFemtoPairPRFL : public NicaFemtoPair {
 public:
  NicaFemtoPairPRFL(Bool_t use_mc = kFALSE) : NicaFemtoPair(use_mc){};
  void Compute();
  void Compute_Rotated();
  void Compute_Hemisphere();
  ENicaFemtoKinematics GetFrame() const { return ENicaFemtoKinematics::kPRFL; };
  TString GetTags() const { return "prfl"; };
  virtual ~NicaFemtoPairPRFL(){};
  ClassDef(NicaFemtoPairPRFL, 1)
};

#endif /* NICAFEMTOKINEMATICS_H_ */

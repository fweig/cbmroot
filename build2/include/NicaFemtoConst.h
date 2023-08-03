/*
 * NicaFemtoConst.h
 *
 *  Created on: 26-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAFEMTOCONST_H_
#define NICAFEMTOCONST_H_

#include "TString.h"
#define FM_TO_GEV 5.0760003
/**
 * femtoscopic kinematics mode
 */
enum class ENicaFemtoKinematics {
  kPRF,   //!< kPRF
  kLCMS,  //!< kLCMS
  kSH_LCMS,
  kSH_PRF,
  kPHIETA,
  kPRFL
};
enum class ENicaFemtoPairType {
  kUnknown,
  kPionPlusPionPlus,
  kPionPlusPionMinus,
  kKaonPlusKaonPlus,
  kKaonPlusKaonMinus,

  kProtonProton,
  kProtonAntiproton,
  kPionPlusKaonPlus,
  kPionPlusKaonMinus,

  kPionPlusProton,
  kPionPlusAntiproton,
  kKaonPlusProton,
  kKaonPlusAntiproton,

  kProtonLambda,
  kLambdaLambda,
  kKaonZeroKaonZero,
  kKaonZeroKaonZeroBar,

  kNeutronNeutron,
  kNeutronProton,
  kPionZeroPionZero,
  kNeutronLambda
};

class NicaFemtoPair;
class NicaDividedHisto1D;
class NicaXMLNode;
namespace NicaFemto {
  /**
   * convert pair type to PDG
   * @param pt
   * @return
   */
  std::pair<Int_t, Int_t> PairTypeToPid(ENicaFemtoPairType pt);
  /**
   * get pair type from PDG codes
   * @param pid1
   * @param pid2
   * @return
   */
  ENicaFemtoPairType PidToPairType(Int_t pid1, Int_t pid2);
  /**
   * change pair type to string
   * @param type
   * @return
   */
  TString PairTypeToString(ENicaFemtoPairType type);
  /**
   * convert label to kinematics enum
   * @param label
   * @return
   */
  ENicaFemtoKinematics LabelToKinematics(TString label);
  /**
   * convert C++ name of ENicaFemtoKinematics enum to kinematics
   * @param label
   * @return
   */
  ENicaFemtoKinematics CodeLabelToKinematics(TString label);
  /**
   * return hbt pair that calculates proper kinematics variables, replaces
   * NicaFemtoPair::Factory
   * @param kin
   * @param use_fake - use "hidde" momenta for calculations instead of "real"
   * @return
   */
  NicaFemtoPair* MakePair(ENicaFemtoKinematics kin, Bool_t use_fake);
  /**
   * create CF from xml node
   * @param nod
   * @return
   */
  NicaDividedHisto1D* GetHistoFromXML(NicaXMLNode* nod);
  /**
   * convert kinematic type to label used to description of NicaDividedHisto
   * @param kin
   * @return
   */
  TString KinematicsToLabel(ENicaFemtoKinematics kin);
  /**
   *
   * @param kin kinematic type
   * @param ax axis x-0, y-1, z -2 , t -3
   * @param ndim number of dimensions in CF
   * @return return name of axis
   */
  TString KinematicsToAxisLabel(ENicaFemtoKinematics kin, Int_t ax, Int_t ndim);

}  // namespace NicaFemto

#endif /* NICAFEMTOCONST_H_ */

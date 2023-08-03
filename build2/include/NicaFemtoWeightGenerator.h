/*
 * NicaFemtoWeightGenerator.h
 * 	derived from ALICE AliFemtoWeightGenerator
 *  Created on: 21-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAFEMTOWEIGHTGENERATOR_H_
#define NICAFEMTOWEIGHTGENERATOR_H_

#include <Rtypes.h>
#include <RtypesCore.h>

#include "NicaFemtoConst.h"
#include "NicaPackage.h"

class NicaFemtoPair;
/**
 * abstract class for generating weights, code derived from AliROOT
 */
class NicaFemtoWeightGenerator : public TObject {
protected:
  /**
   * Type of the pair for which the calculation is done
   */
  ENicaFemtoPairType fPairType;
  /**
   * id of task that own this weight
   */
  Int_t fTaskID;
  /**
   * relative momentum out component in PRF
   */
  Double_t fKStarOut;
  /**
   * relative momentum side component in PRF
   */
  Double_t fKStarSide;
  /**
   * relative momentum long component in PRF
   */
  Double_t fKStarLong;
  /**
   * relative momentum magnitude
   */
  Double_t fKStar;
  /**
   * elative separation out component in PRF
   */
  Double_t fRStarOut;
  /**
   * relative separation side component in PRF
   */
  Double_t fRStarSide;
  /**
   * relative separation long component in PRF
   */
  Double_t fRStarLong;
  /**
   *  relative separation magnitude
   */
  Double_t fRStar;

public:
  /**
   * default constructor
   */
  NicaFemtoWeightGenerator();
  /**
   * copy constructor
   * @param aModel object to copy
   */
  NicaFemtoWeightGenerator(const NicaFemtoWeightGenerator& aModel);
  /**
   * make copy
   * @return
   */
  virtual NicaFemtoWeightGenerator* MakeCopy() const { return new NicaFemtoWeightGenerator(*this); };
  /**
   * = operator
   * @param aModel object to copy
   * @return new copy of class
   */
  virtual NicaFemtoWeightGenerator& operator=(const NicaFemtoWeightGenerator& aModel);
  /**
   * return weight of pair
   * @param pair pair used for calculating weights
   * @return calculated weight
   */
  virtual Double_t GenerateWeight(NicaFemtoPair* pair);
  /**
   * set pair type
   * @param aPairType pair type
   */
  virtual void SetPairType(ENicaFemtoPairType aPairType);
  /**
   * set pair type from pair
   * @param pair pair of particles
   */
  virtual void SetPairTypeFromPair(NicaFemtoPair* pair);
  /**
   *
   * @return pair type
   */
  virtual ENicaFemtoPairType GetPairType() const;
  /**
   *
   * @param pair pair of tracks
   * @return pair type form pai
   */
  virtual ENicaFemtoPairType GetPairTypeFromPair(NicaFemtoPair* pair);
  /**
   *
   * @return k* momentum difference of last pair where weight was calculated
   */
  virtual Double_t GetKStar() const;
  /**
   *
   * @return k*out of last pair where weight was calculated
   */
  virtual Double_t GetKStarOut() const;
  /**
   *
   * @return k*side of last pair where weight was calculated
   */
  virtual Double_t GetKStarSide() const;
  /**
   *
   * @return k*long of last pair where weight was calculated
   */
  virtual Double_t GetKStarLong() const;
  /**
   *
   * @return r* of last pair where weight was calculated
   */
  virtual Double_t GetRStar() const;
  /**
   *
   * @return r*out of last pair where weight was calculated
   */
  virtual Double_t GetRStarOut() const;
  /**
   *
   * @return r*side of last pair where weight was calculated
   */
  virtual Double_t GetRStarSide() const;
  /**
   *
   * @return r*long of last pair where weight was calculated
   */
  virtual Double_t GetRStarLong() const;
  /**
   * clone this class
   * @return clone of this class
   */
  virtual NicaFemtoWeightGenerator* Clone(const char* /*newname*/ = "") const { return 0; };
  /**
   *
   * @return pdg code for first particle
   */
  virtual Int_t GetPdg1();
  /**
   *
   * @return pdg code for second particle
   */
  virtual Int_t GetPdg2();
  /**
   * initlalize this class
   */
  virtual void Init(Int_t task_id);
  virtual void Print(Option_t* option = "") const;
  virtual NicaPackage* Report() const;
  virtual ~NicaFemtoWeightGenerator();

  ClassDef(NicaFemtoWeightGenerator, 1)
};

inline Double_t NicaFemtoWeightGenerator::GetKStar() const { return fKStar; }

inline Double_t NicaFemtoWeightGenerator::GetKStarOut() const { return fKStarOut; }

inline Double_t NicaFemtoWeightGenerator::GetKStarSide() const { return fKStarSide; }

inline Double_t NicaFemtoWeightGenerator::GetKStarLong() const { return fKStarLong; }

inline Double_t NicaFemtoWeightGenerator::GetRStar() const { return fRStar; }

inline Double_t NicaFemtoWeightGenerator::GetRStarOut() const { return fRStarOut; }

inline Double_t NicaFemtoWeightGenerator::GetRStarSide() const { return fRStarSide; }

inline Double_t NicaFemtoWeightGenerator::GetRStarLong() const { return fRStarLong; }

inline void NicaFemtoWeightGenerator::Init(Int_t task_id) { fTaskID = task_id; }

inline NicaPackage* NicaFemtoWeightGenerator::Report() const { return new NicaPackage(this); }

#endif /* NICAFEMTOWEIGHTGENERATOR_H_ */

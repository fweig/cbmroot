/* Copyright (C) 2009-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Andrey Lebedev, Volker Friese */

/**
 * \file CbmHit.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 *
 * Base class for hits used for tracking in CBM.
 * Derives from TObject.
 * Each hit has its unique identifier of type HitType,
 * which can be later use for safe type casting.
 *
 * Former name: CbmBaseHit (renamed 11 May 2015)
 **/
#ifndef CBMHIT_H_
#define CBMHIT_H_

enum HitType
{
  kHIT,
  kPIXELHIT,
  kSTRIPHIT,
  kSTSHIT,
  kMVDHIT,
  kRICHHIT,
  kMUCHPIXELHIT,
  kMUCHSTRAWHIT,
  kTRDHIT,
  kTOFHIT,
  kECALHIT
};

#include <Rtypes.h>  // for THashConsistencyHolder, ClassDef
#include <TObject.h>  // for TObject

#include <cstdint>
#include <string>  // for string, basic_string

class CbmMatch;

class CbmHit : public TObject {
public:
  /**
	 * \brief Default constructor.
	 */
  CbmHit();

  /**
	 * \brief Constructor with arguments.
         * \param[in] _type Hit type (see enum HitType).
	 * \param[in] _z  z position of the hit [cm].
	 * \param[in] _dz  Error of z position of the hit [cm].
	 * \param[in] _refId Reference id (usually to cluster, digi or MC point).
         * \param[in] _address Unique detector identifier.
	 * \param[in] _time Hit time [ns].   
	 * \param[in] _timeError Error of hit time [ns].	 
	 */
  CbmHit(HitType _type, double _z, double _dz, int32_t _refId, int32_t _address, double _time = -1.,
         double _timeError = -1.);

  /**
	 * \brief Destructor.
	 */
  virtual ~CbmHit();

  /* Accessors */
  HitType GetType() const { return fType; }
  double GetZ() const { return fZ; }
  double GetDz() const { return fDz; }
  int32_t GetRefId() const { return fRefId; }
  int32_t GetAddress() const { return fAddress; }
  CbmMatch* GetMatch() const { return fMatch; }
  double GetTime() const { return fTime; }
  double GetTimeError() const { return fTimeError; }

  /* Setters */
  void SetZ(double z) { fZ = z; }
  void SetDz(double dz) { fDz = dz; }
  void SetRefId(int32_t refId) { fRefId = refId; }
  void SetAddress(int32_t address) { fAddress = address; }
  void SetMatch(CbmMatch* match);
  void SetTime(double time) { fTime = time; }
  void SetTime(double time, double error)
  {
    fTime      = time;
    fTimeError = error;
  }
  void SetTimeError(double error) { fTimeError = error; }

  /**
	 * Virtual function. Must be implemented in derived class.
	 * Should return plane identifier of the hit. Usually this is station or layer
	 * number of the detector. Can be calculated using unique detector identifier
	 * or can use additional class member from the derived class to store the plane identifier.
	 **/
  virtual int32_t GetPlaneId() const { return -1; }

  /**
	 * \brief Virtual function. Must be implemented in derived class.
	 * Has to return string representation of the object.
	 **/
  virtual std::string ToString() const { return "Has to be implemented in derrived class"; }

protected:
  /**
         * \brief Sets hit type.
         * \param type hit type
        **/
  void SetType(HitType type) { fType = type; }
  CbmHit(const CbmHit&);
  CbmHit& operator=(const CbmHit&);


private:
  HitType fType;        ///< hit type
  double fZ;            ///< Z position of hit [cm]
  double fDz;           ///< Z position error [cm]
  int32_t fRefId;       ///< some reference id (usually to cluster, digi or MC point)
  int32_t fAddress;     ///< detector unique identifier
  double fTime;         ///< Hit time [ns]
  double fTimeError;    ///< Error of hit time [ns]
  CbmMatch* fMatch;     ///< Monte-Carlo information

  ClassDef(CbmHit, 3);
};

#endif /* CBMHIT_H_ */

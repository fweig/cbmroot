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

enum HitType {
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

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t
#include <TObject.h>     // for TObject

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
  CbmHit(HitType _type,
         Double_t _z,
         Double_t _dz,
         Int_t _refId,
         Int_t _address,
         Double_t _time      = -1.,
         Double_t _timeError = -1.);

  /**
	 * \brief Destructor.
	 */
  virtual ~CbmHit();

  /* Accessors */
  HitType GetType() const { return fType; }
  Double_t GetZ() const { return fZ; }
  Double_t GetDz() const { return fDz; }
  Int_t GetRefId() const { return fRefId; }
  Int_t GetAddress() const { return fAddress; }
  CbmMatch* GetMatch() const { return fMatch; }
  Double_t GetTime() const { return fTime; }
  Double_t GetTimeError() const { return fTimeError; }

  /* Setters */
  void SetZ(Double_t z) { fZ = z; }
  void SetDz(Double_t dz) { fDz = dz; }
  void SetRefId(Int_t refId) { fRefId = refId; }
  void SetAddress(Int_t address) { fAddress = address; }
  void SetMatch(CbmMatch* match);
  void SetTime(Double_t time) { fTime = time; }
  void SetTime(Double_t time, Double_t error) {
    fTime      = time;
    fTimeError = error;
  }
  void SetTimeError(Double_t error) { fTimeError = error; }

  /**
	 * Virtual function. Must be implemented in derived class.
	 * Should return plane identifier of the hit. Usually this is station or layer
	 * number of the detector. Can be calculated using unique detector identifier
	 * or can use additional class member from the derived class to store the plane identifier.
	 **/
  virtual Int_t GetPlaneId() const { return -1; }

  /**
	 * \brief Virtual function. Must be implemented in derived class.
	 * Has to return string representation of the object.
	 **/
  virtual std::string ToString() const {
    return "Has to be implemented in derrived class";
  }

protected:
  /**
         * \brief Sets hit type.
         * \param type hit type
        **/
  void SetType(HitType type) { fType = type; }
  CbmHit(const CbmHit&);
  CbmHit& operator=(const CbmHit&);


private:
  HitType fType;   ///< hit type
  Double_t fZ;     ///< Z position of hit [cm]
  Double_t fDz;    ///< Z position error [cm]
  Int_t fRefId;    ///< some reference id (usually to cluster, digi or MC point)
  Int_t fAddress;  ///< detector unique identifier
  Double_t fTime;  ///< Hit time [ns]
  Double_t fTimeError;  ///< Error of hit time [ns]
  CbmMatch* fMatch;     ///< Monte-Carlo information

  ClassDef(CbmHit, 3);
};

#endif /* CBMHIT_H_ */

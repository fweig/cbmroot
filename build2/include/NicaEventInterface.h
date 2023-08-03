/*
 * NicaEventSource.h
 *
 *  Created on: 2 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAEVENTINTERFACE_H_
#define NICAEVENTINTERFACE_H_
#include <TLorentzVector.h>
#include <TObject.h>

#include "NicaTrackInterface.h"

/**
 * Interface between event used in analysis and event in original format.
 * Original object called "source event" is copied/linked directly from
 * structures stored in ROOT tree. This class make possible to modify such
 * objects and convert them into NicaEvent-based classes. If source in NicaEvent
 * base class is used than it's always possible to access to original event
 * structure by NicaEvent::GetSource()::GetRawEventPointer(). However in most
 * cases it's enough to copy only fraction of data from original structure
 * (event source). In such case user can skip storing this interface (and source
 * event) by call NicaEvent::DisableSource().
 * @see NicaEvent
 */
class NicaEventInterface : public TObject {
public:
  enum eMode { kRead, kWrite, kWriteVirtual };

private:
  Bool_t fCanDeleteEvent;

protected:
  /**
   * link source object with tree (was called LinkWithTree) used for reading
   * events
   */
  virtual void ConnectToTree() = 0;
  /**
   * register source event in tree, uset to write events
   * @param write true if write to file
   */
  virtual void Register(Bool_t write) = 0;
  /**
   *
   * @return true if this event is owned only by this object and can be
   * deleteted safely
   */
  Bool_t CanDeleteEvent() const { return fCanDeleteEvent; };

public:
  /**
   * default constructor
   */
  NicaEventInterface();
  /**
   * boost this event
   * @param vx x-velocity
   * @param vy y-velocity
   * @param vz z-velocit
   */
  virtual void Boost(Double_t vx, Double_t vy, Double_t vz);
  /**
   * clear this object
   * @param opt
   */
  virtual void Clear(Option_t* /*opt*/ = "") {};
  /**
   * compress this event
   * @param map
   * @param map_size
   */
  virtual void Compress(Int_t* map, Int_t map_size) = 0;
  /**
   * copy source from one event into this event
   * @param s event to copy into this
   */
  virtual void CopyData(NicaEventInterface* s) = 0;
  /**
   * make copy and compress event "s" into this object
   * @param s event to copy
   * @param map map of copying
   * @param map_size size of map
   */
  virtual void CopyAndCompress(NicaEventInterface* s, Int_t* map, Int_t map_size) = 0;
  /**
   * get/set source from tree
   * @param mode @see ModeWrite, @see ModeRead, @see ModeVirtualWrite
   */
  void LinkWithTree(eMode mode);
  /**
   * Fill track interface - set track data in original format into track
   * interface. If information about track is stored in single object this is
   * equivalednt of NicaTrackInterface->SetRawTrack(). However not  in all cases
   * it's possible to get all informations about track from single object.
   * @param track track interface to fill
   * @param index index of track
   */
  virtual void FillTrackInterface(NicaTrackInterface* track, Int_t index) = 0;
  /**
   *
   * @return total number of tracks
   */
  virtual Int_t GetTotalTrackNo() const = 0;
  /**
   *
   * @return new allocated interface to tracks in this tree
   */
  virtual NicaTrackInterface* GetTrackInterface() const = 0;
  /**
   * return pointer to track in source
   * @param index
   * @return
   */
  virtual TObject* GetRawTrackPointer(Int_t index) const = 0;
  /**
   * pointer to event/ event header
   * @return  pointer to event/ event header
   */
  virtual TObject* GetRawEventPointer() const = 0;
  /** GETTER SETTER SECTION **/
  /**
   * set vertex position
   * @param x
   * @param y
   * @param z
   */
  virtual void SetVertex(Double_t /*x*/, Double_t /*y*/, Double_t /*z*/) {};
  /**
   * set event reaction plane angle and error
   * @param phi
   * @param phi_error
   */
  virtual void SetPhi(Double_t /*phi*/, Double_t /*phi_error*/ = 0) {};
  /**
   *
   * @return event reaction plane
   */
  virtual Double_t GetPhi() const { return 0.; };
  /**
   *
   * @return reaction plane angle uncrtainty
   */
  virtual Double_t GetPhiError() const { return 0.; };
  /**
   *
   * @return vertex position
   */
  virtual TLorentzVector GetVertex() const { return TLorentzVector(0., 0., 0., 0.); };
  virtual ~NicaEventInterface();
  ClassDef(NicaEventInterface, 1)
};

#endif /* NICAEVENTINTERFACE_H_ */

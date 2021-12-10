/* Copyright (C) 2016-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Florian Uhlig */

/** @file CbmEvent.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17.09.2016
 **/


#ifndef CBMEVENT_H
#define CBMEVENT_H 1

#include "CbmDefs.h"    // for ECbmDataType, ECbmModuleId::kStsTrack
#include "CbmMatch.h"   // for CbmMatch
#include "CbmVertex.h"  // for CbmVertex, found in core/data/global

#include <Rtypes.h>          // for THashConsistencyHolder, ClassDef
#include <TMatrixFSymfwd.h>  // for TMatrixFSym
#include <TObject.h>         // for TObject

#include <cstdint>
#include <map>     // for map, map<>::mapped_type
#include <string>  // for string
#include <vector>  // for vector

/** @class CbmEvent
 ** @brief Class characterising one event by a collection of links (indices)
 ** to data objects,
 ** @author V.Friese <v.friese@gsi.de>
 ** @version 1.0
 **
 **/
class CbmEvent : public TObject {

public:
  /** Default constructor **/
  CbmEvent() : CbmEvent(-1, 0., 0.) {};

  /** Constructor with event number and time
		 ** @param[in] number    Event number
		 ** @param[in] startTime Event start time [ns]
		 ** @param[in] endTime   Event start time [ns]
		 **/
  CbmEvent(int32_t number, double startTime = 0., double endTime = 0.)
    : TObject()
    , fNumber(number)
    , fTimeStart(startTime)
    , fTimeEnd(endTime)
    , fNofData(0)
    , fVertex()
    , fMatch(nullptr)
    , fIndexMap()
  {
  }

  CbmEvent(CbmEvent&&) = default;

  /** Destructor **/
  virtual ~CbmEvent()
  {
    if (fMatch) delete fMatch;
  }

  /** Overload TObject Clear to clear the map! **/
  void Clear(Option_t* /*option*/) { fIndexMap.clear(); }


  /** Add a data object to the index map
		 ** @param DataType  Type of data (for values see CbmDetectorList.h)
		 ** @param Index     Index of the data object in its TClonesArray
		 */
  void AddData(ECbmDataType type, uint32_t index);


  /** Add an STS track to the event
		 ** @param Index of STS track in its TClonesArray
		 **/
  void AddStsTrack(uint32_t index) { AddData(ECbmDataType::kStsTrack, index); }


  /** Get the index of a data object in its TClonesArray
		 ** @param DataType  Type of data (for values see CbmDetectorList.h)
		 ** @param iData     Running number of data object in event
		 ** @value Index of data object in its TClonesArray
		 **/
  uint32_t GetIndex(ECbmDataType type, uint32_t iData);


  /** Get match object
		 ** @value Pointer to match object
		 **/
  CbmMatch* GetMatch() const { return fMatch; }


  /** Get total number of data (of all types) in the event **/
  int32_t GetNofData() const { return fNofData; }


  /** Get number of data objects of a given type in this event
		 ** @param DataType  Type of data (for values see CbmDetectorList.h)
		 ** @value Number of objects of type DataType in the event.
		 ** -1 is data type is not registered.
		 **/
  int32_t GetNofData(ECbmDataType type) const;


  /** Get number of STS tracks
		 ** @value Number of STS tracks in the event. -1 if not registered.
		 **/
  int32_t GetNofStsTracks() const { return GetNofData(ECbmDataType::kStsTrack); }


  /** Get event number
		 ** @value Event number
		 **/
  int32_t GetNumber() const { return fNumber; }


  /** Get STS track index
		 ** @param iTrack  Running number of STS track in the event
		 ** @value index   Index of STS track in TClonesArray
		 **/
  int32_t GetStsTrackIndex(int32_t iTrack) { return GetIndex(ECbmDataType::kStsTrack, iTrack); }


  /** Get event end time
		 ** @value End time of event [ns]
		 **/
  double GetEndTime() const { return fTimeEnd; }


  /** Get event start time
		 ** @value Start time of event [ns]
		 **/
  double GetStartTime() const { return fTimeStart; }

  /** Set event number
		 ** @value Event number
		 **/
  void SetNumber(int32_t number) { fNumber = number; }

  /** Set end time
		 ** @param endTime  End time of event [ns]
		 **/
  void SetEndTime(double endTime) { fTimeEnd = endTime; }


  /** Set a match object
		 ** @param match  Pointer to a match object
		 **/
  void SetMatch(CbmMatch* match) { fMatch = match; }


  /** Set start time
		 ** @param endTime  Start time of event [ns]
		 **/
  void SetStartTime(double startTime) { fTimeStart = startTime; }


  /** Set the STS track index array
		 ** @brief Sets the index array for STS tracks.
		 ** Old content will be overwritten.
		 ** @param indexVector  Vector with indices of STS tracks
	     **/
  void SetStsTracks(std::vector<uint32_t>& indexVector)
  {
    fNofData -= fIndexMap[ECbmDataType::kStsTrack].size();
    fIndexMap[ECbmDataType::kStsTrack] = indexVector;
    fNofData += fIndexMap[ECbmDataType::kStsTrack].size();
  }


  /** Set the event vertex variables
	   *@param x         x coordinate [cm]
	   *@param y         y coordinate [cm]
	   *@param z         z coordinate [cm]
	   *@param chi2      chi square of vertex fit
	   *@param ndf       Number of degrees of freedom of vertex fit
	   *@param nTracks   Number of tracks used for vertex fit
	   *@param covMat    Covariance Matrix (symmetric, 3x3)
	   **/
  void SetVertex(double x, double y, double z, double chi2, int32_t ndf, int32_t nTracks, const TMatrixFSym& covMat);


  /** String output **/
  std::string ToString() const;


  /** Get event vertex
		 ** @value Pointer to vertex object
		 **/
  CbmVertex* GetVertex() { return &fVertex; }

  /** Swap two events **/
  void Swap(CbmEvent& e);

private:
  /** Event meta data **/
  int32_t fNumber;      ///< Event number
  double fTimeStart;    ///< Event start time [ns]
  double fTimeEnd;      ///< Event end time [ns]
  int32_t fNofData;     ///< Number of data objects in the event
  CbmVertex fVertex;    ///< Primary Vertex
  CbmMatch* fMatch;     ///< Match object to MCEvent

  /** Arrays of indices to data types **/
  std::map<ECbmDataType, std::vector<uint32_t>> fIndexMap;

  CbmEvent(const CbmEvent&);
  CbmEvent& operator=(const CbmEvent&);


  ClassDef(CbmEvent, 2);
};

#endif /* CBMEVENT_H_ */

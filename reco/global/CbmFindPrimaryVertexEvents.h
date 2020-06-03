/** @file CbmFindPrimaryVertexEvents.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 16.06.2014
 ** @date 02.11.2016
 **/

#ifndef CBMFINDPRIMARYVERTEXEVENTS_H
#define CBMFINDPRIMARYVERTEXEVENTS_H 1


#include "TStopwatch.h"
#include "FairTask.h"

class TClonesArray;
class CbmPrimaryVertexFinder;
class CbmVertex;


/** CbmFindPrimaryVertexEvents
 ** @brief Task class for PV finding.
 ** @author V.Friese <v.friese@gsi.de>
 **
 ** Input: List of CbmStsTracks
 ** Output: CbmVertex
 **
 ** Uses as vertex finding algorithm classes derived from
 ** CbmPrimaryVertexFinder.
 **/
class CbmFindPrimaryVertexEvents : public FairTask
{

 public:

  /** Default constructor **/
  CbmFindPrimaryVertexEvents();


  /** Standard constructor
   *@param pvFinder  Pointer to concrete vertex finder
   **/
  CbmFindPrimaryVertexEvents(CbmPrimaryVertexFinder* pvFinder);
  
  // Prevent auto copy constructor and assignment operator
  CbmFindPrimaryVertexEvents(const CbmFindPrimaryVertexEvents&) = delete;
  CbmFindPrimaryVertexEvents& operator=(const CbmFindPrimaryVertexEvents&) = delete;

  /** Constructor with name and title
   **
   *@param name      Name of task
   *@param title     Title of task
   *@param pvFinder  Pointer to vertex finder concrete object
   **/
  CbmFindPrimaryVertexEvents(const char* name, const char* title,
		       CbmPrimaryVertexFinder* pvFinder);


  /** Destructor **/
  virtual ~CbmFindPrimaryVertexEvents();


  /** Initialisation **/
  virtual InitStatus Init();


  /** Task execution **/
  virtual void Exec(Option_t* opt);


  /** Finish **/
  virtual void Finish();


 private:

  TStopwatch              fTimer;    ///< ROOT stopwatch
  CbmPrimaryVertexFinder* fFinder;   ///< Concrete finder instance
  TClonesArray*           fEvents;   //! event array
  TClonesArray*           fTracks;   //! track array
  CbmVertex*              fPrimVert;

  Int_t     fNofEvents;       ///< Total number of events processed
  Double_t  fTimeTot;         ///< Total execution time [s]



  ClassDef(CbmFindPrimaryVertexEvents,1);

};

#endif



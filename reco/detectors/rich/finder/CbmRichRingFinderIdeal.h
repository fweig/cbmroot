/**
 * \file CbmRichRingFinderIdeal.h
 *
 * \brief Ideal ring finder in the RICH detector. It uses MC information
 * to attach RICH hits to rings.
 *
 * \author Supriya Das
 * \date 2006
 **/

#ifndef CBM_RICH_RING_FINDER_IDEAL
#define CBM_RICH_RING_FINDER_IDEAL

#include "CbmRichRingFinder.h"

#include <vector>

using namespace std;

class CbmRichHit;
class CbmMCDataArray;
class CbmMCEventList;
class CbmDigiManager;
class CbmEvent;

class CbmRichRingFinderIdeal : public CbmRichRingFinder {

public:
  /**
     * \brief Default constructor.
     */
  CbmRichRingFinderIdeal();

  /**
     * \brief Destructor.
     */
  virtual ~CbmRichRingFinderIdeal();

  /**
     * \brief Inherited from CbmRichRingFinder.
     */
  virtual void Init();

  /**
     * Inherited from CbmRichRingFinder.
     */
  virtual int DoFind(CbmEvent* event, TClonesArray* hitArray, TClonesArray* projArray, TClonesArray* ringArray);

private:
  CbmMCDataArray* fRichPoints = nullptr;
  CbmMCDataArray* fMcTracks   = nullptr;
  CbmMCEventList* fEventList  = nullptr;
  CbmDigiManager* fDigiMan    = nullptr;

  /**
     * \ brief Return evnetId from digiMatch corresponding to rich hit.
     */
  Int_t GetEventIdForRichHit(const CbmRichHit* richHit);


  /**
     * \brief Copy constructor.
     */
  CbmRichRingFinderIdeal(const CbmRichRingFinderIdeal&);

  /**
     * \brief Assignment operator.
     */
  CbmRichRingFinderIdeal& operator=(const CbmRichRingFinderIdeal&);
};

#endif

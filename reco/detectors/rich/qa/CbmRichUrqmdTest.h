/**
 * \file CbmRichUrqmdTest.h
 *
 * \brief RICH geometry testing in Urqmd collisions.
 *
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2012
 **/

#ifndef CBM_RICH_URQMD_TEST
#define CBM_RICH_URQMD_TEST

#include "FairTask.h"
class TClonesArray;
class CbmRichRing;
class TCanvas;
class CbmHistManager;
class CbmMCDataArray;
class CbmMCEventList;
class CbmDigiManager;

#include <map>
#include <vector>

using namespace std;

/**
 * \class CbmRichUrqmdTest
 *
 * \brief RICH geometry testing in Urqmd collisions.
 *
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2012
 **/
class CbmRichUrqmdTest : public FairTask {

public:
  /**
     * \brief Standard constructor.
     */
  CbmRichUrqmdTest();

  /**
     * \brief Standard destructor.
     */
  virtual ~CbmRichUrqmdTest();

  /**
     * \brief Inherited from FairTask.
     */
  virtual InitStatus Init();

  /**
     * \brief Inherited from FairTask.
     */
  virtual void Exec(Option_t* option);

  /**
     * \brief Inherited from FairTask.
     */
  virtual void Finish();


  /**
     * \brief Set output directory where you want to write results (figures and json).
     * \param[in] dir Path to the output directory.
     */
  void SetOutputDir(const string& dir) { fOutputDir = dir; }


private:
  /**
     * \brief Initialize histograms.
     */
  void InitHistograms();

  /**
     * \brief
     */
  void FillRichRingNofHits();

  /**
     * \brief
     */
  void NofRings();

  /**
     * \brief
     */
  void NofHitsAndPoints();

  /**
     * \brief
     */
  void NofProjections();

  /**
     * \brief
     */
  void Vertex();


  /**
     *  \brief Draw histograms.
     */
  void DrawHist();

  void PmtXYSource();

  /**
     * \brief Copy constructor.
     */
  CbmRichUrqmdTest(const CbmRichUrqmdTest&);

  /**
     * \brief Assignment operator.
     */
  CbmRichUrqmdTest& operator=(const CbmRichUrqmdTest&);

  CbmHistManager* fHM;

  string fOutputDir;  // output dir for results

  TClonesArray* fRichHits;
  TClonesArray* fRichRings;
  CbmMCDataArray* fRichPoints;
  CbmMCDataArray* fMcTracks;
  TClonesArray* fRichRingMatches;
  TClonesArray* fRichProjections;
  CbmDigiManager* fDigiMan;
  CbmMCEventList* fEventList;

  Int_t fEventNum;
  Int_t
    fMinNofHits;  // Min number of hits in ring for detector acceptance calculation.

  // Number of hits in the MC RICH ring
  std::map<pair<Int_t, Int_t>, Int_t> fNofHitsInRingMap;

  ClassDef(CbmRichUrqmdTest, 1)
};

#endif

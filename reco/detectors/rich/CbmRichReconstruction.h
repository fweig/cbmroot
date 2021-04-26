/**
* \file CbmRichReconstruction.h
*
* \brief Main class for running event reconstruction in the RICH detector.
*
* \author Semen Lebedev
* \date 2012
**/

#ifndef CBM_RICH_RECONSTRUCTION
#define CBM_RICH_RECONSTRUCTION

#include "FairTask.h"

#include <string>

class TClonesArray;
class CbmRichRingFinder;
class CbmRichRingFitterBase;
class CbmRichTrackExtrapolationBase;
class CbmRichProjectionProducerBase;
class CbmRichRingTrackAssignBase;
class CbmEvent;

using std::string;

/**
* \class CbmRichReconstruction
*
* \brief Main class for running event reconstruction in the RICH detector.
*
* \author Semen Lebedev
* \date 2012
**/
class CbmRichReconstruction : public FairTask {
public:
  /**
    * \brief Default constructor.
    */
  CbmRichReconstruction();

  /**
    * \brief Destructor.
    */
  virtual ~CbmRichReconstruction();


  /**
    * \brief Inherited from FairTask.
    */
  virtual InitStatus Init();

  /**
    * \brief Inherited from FairTask.
    */
  virtual void Exec(Option_t* opt);

  /**
    * \brief Inherited from FairTask.
    */
  virtual void Finish();

  void SetRunExtrapolation(bool b) { fRunExtrapolation = b; }
  void SetRunProjection(bool b) { fRunProjection = b; }
  void SetRunFinder(bool b) { fRunFinder = b; }
  void SetRunFitter(bool b) { fRunFitter = b; }
  void SetRunTrackAssign(bool b) { fRunTrackAssign = b; }

  void SetExtrapolationName(const string& n) { fExtrapolationName = n; }
  void SetProjectionName(const string& n) { fProjectionName = n; }
  void SetFinderName(const string& n) { fFinderName = n; }
  void SetFitterName(const string& n) { fFitterName = n; }
  void SetTrackAssignName(const string& n) { fTrackAssignName = n; }

  void SetUseHTAnnSelect(bool select) { fUseHTAnnSelect = select; }

  /**
    * \brief Set Z coordinate where STS tracks will be extrapolated.
    * \param[in] z Z coordinate.
    */
  void SetZTrackExtrapolation(Double_t z) { fZTrackExtrapolation = z; }

  void UseMCbmSetup()
  {
    this->SetRunExtrapolation(false);
    this->SetRunProjection(false);
    this->SetRunTrackAssign(false);
    this->SetUseHTAnnSelect(false);
  }

private:
  TClonesArray* fRichHits        = nullptr;
  TClonesArray* fRichRings       = nullptr;
  TClonesArray* fRichProjections = nullptr;
  TClonesArray* fRichTrackParamZ = nullptr;
  TClonesArray* fGlobalTracks    = nullptr;
  TClonesArray* fCbmEvents       = nullptr;

  Int_t fEventNum = 0;  // event or timeslice counter

  // pointers to the algorithms
  CbmRichRingFinder* fRingFinder                     = nullptr;
  CbmRichRingFitterBase* fRingFitter                 = nullptr;
  CbmRichTrackExtrapolationBase* fTrackExtrapolation = nullptr;
  CbmRichProjectionProducerBase* fProjectionProducer = nullptr;
  CbmRichRingTrackAssignBase* fRingTrackAssign       = nullptr;

  // What do you want to run.
  bool fRunExtrapolation = true;
  bool fRunProjection    = true;
  bool fRunFinder        = true;
  bool fRunFitter        = true;
  bool fRunTrackAssign   = true;

  // Run ring-candidate selection algorithm based on ANN
  bool fUseHTAnnSelect = true;

  // Algorithm names for each step of reconstruction.
  string fExtrapolationName = "littrack";
  string fProjectionName    = "analytical";
  string fFinderName        = "hough";
  string fFitterName        = "ellipse_tau";
  string fTrackAssignName   = "closest_distance";

  // Z coordinate where STS tracks will be extrapolated.
  Double_t fZTrackExtrapolation = 260.;

  /**
    * \brief
    */
  void InitExtrapolation();

  /**
    * \brief
    */
  void InitProjection();

  /**
    * \brief
    */
  void InitFinder();

  /**
    * \brief
    */
  void InitFitter();

  /**
    * \brief
    */
  void InitTrackAssign();

  /**
    * \brief
    */
  void ProcessData(CbmEvent* event);

  /**
    * \brief
    */
  void RunExtrapolation(CbmEvent* event);

  /**
    * \brief
    */
  void RunProjection(CbmEvent* event);

  /**
    * \brief
    */
  void RunFinder(CbmEvent* event);

  /**
    * \brief
    */
  void RunFitter(CbmEvent* event);

  /**
    * \brief
    */
  void RunTrackAssign(CbmEvent* event);

  /**
    * \brief Copy constructor.
    */
  CbmRichReconstruction(const CbmRichReconstruction&);

  /**
    * \brief Assignment operator.
    */
  CbmRichReconstruction& operator=(const CbmRichReconstruction&);

  ClassDef(CbmRichReconstruction, 1);
};

#endif

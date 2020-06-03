#ifndef CbmRichCorrectionVector_H
#define CbmRichCorrectionVector_H


#include "FairTask.h"
#include "CbmHistManager.h"
#include "CbmRichRingFitterCOP.h"
#include "CbmRichRingFitterEllipseTau.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmRichRingLight.h"
#include <vector>
#include <map>
#include "TString.h"
#include "CbmRichRecGeoPar.h"
#include "TGeoNavigator.h"

using namespace std;

class TClonesArray;
class TH1D;
class TH2D;


class CbmRichCorrectionVector : public FairTask //CbmRichProjectionProducerBase
{
private:
    static const int kMAX_NOF_HITS = 100; // Maximum number of hits in ring

public:
	/*
	 * Constructor.
	 */
    CbmRichCorrectionVector();

	/*
	 * Destructor.
	 */
    virtual ~CbmRichCorrectionVector();

    /**
     * \brief Inherited from FairTask.
     */
    virtual InitStatus Init();

    /**
     * \brief Inherited from FairTask.
     */
    virtual void Exec(
          Option_t* option);

    /**
     * \brief Inherited from FairTask.
     */
    virtual void Finish();

    /*
     * Histogram initialization for projection producer method.
     */
    void InitHistProjection();

    /*
     * Histogram initialization for alignment method.
     */
    void InitHistAlignment();

    /*
     *
     */
    void CalculateAnglesAndDrawDistrib();

    /*
     *
     */
    void GetTrackPosition(Double_t &x, Double_t &y);

    /*
     * Fill the PMT plane with hits, ONLY for event with SEVERAL particles.
     */
    void MatchFinder();

    /*
     *
     */
    void FillPMTMap(const Char_t* mirr_path, CbmRichPoint* pPoint);

    /*
     *
     */
    void FillPMTMapEllipse(const Char_t* mirr_path, Float_t CenterX, Float_t CenterY);

    /*
     * From incoming track on the mirrors, do reflection of its trajectory and extrapolation of its intersection on the PMT plane.
     */
    void ProjectionProducer(TClonesArray* projectedPoint);

    /*
     * Get pmt normal from 3 different points on the plane.
     */
    void GetPmtNormal(Int_t NofPMTPoints, vector<Double_t> &normalPMT, Double_t &normalCste);

    /*
     * Calculate mean sphere center coordinates from all the mirror tiles (to be used for the reconstruction step).
     */
    void GetMeanSphereCenter(TGeoNavigator *navi, vector<Double_t> &ptC);

    /*
     * Calculate intersection between incoming particle track (position given by ptR1 and direction by momR1) and sphere with center ptC (Cmean) and radius sphereRadius.
     */
    void GetMirrorIntersection(vector<Double_t> &ptM, vector<Double_t> ptR1, vector<Double_t> momR1, vector<Double_t> ptC, Double_t sphereRadius);

    /*
     * Calculate the normal of the considered mirror tile, using the sphere center position of the tile (ptC) and the local reflection point on the mirror (ptM) => normalMirr.
     * Then calculate point on sensitive plane from the reflected track extrapolated (ptR2 = reflection of ptR1, with reflection axis = normalMirr).
     * ptR2Center uses ptC for the calculations, whereas ptR2Mirr uses ptM.
     */
    void ComputeR2(vector<Double_t> &ptR2Center, vector<Double_t> &ptR2Mirr, vector<Double_t> ptM, vector<Double_t> ptC, vector<Double_t> ptR1);

    /*
     * Calculate the intersection point (P) between the track and the PMT plane, as if the track had been reflected by the mirror tile.
     * ptPMirr is calculated using the mirror point (ptM) to define the line reflected by the mirror and towards the PMT plane.
     * ptPR2 is calculated using ptR2Mirr (the reflection of point R1 on the sensitive plane, using ptM for the calculations -> see ComputeR2 method).
     */
    void ComputeP(vector<Double_t> &ptPMirr, vector<Double_t> &ptPR2, vector<Double_t> normalPMT, vector<Double_t> ptM, vector<Double_t> ptR2Mirr, Double_t normalCste);

    /*
     *
     */
    void FillHistProjection(TVector3 outPos, TVector3 outPosUnCorr, Int_t NofGlobalTracks, vector<Double_t> normalPMT, Double_t constantePMT);

    void RotateAndCopyHitsToRingLight(const CbmRichRing* ring1, CbmRichRingLight* ring2);

    /*
     * Draw histograms for alignment method.
     */
    void DrawHistAlignment();

    /*
     *
     */
    void DrawFit(vector<Double_t> &outputFit, Int_t thresh);


    /*
     * Draw histograms for mapping.
     */
    void DrawHistMapping();

    /*
     * Draw histograms projection producer method.
     */
    void DrawHistProjection();

    /*
     * Draw histograms from root file.
     */
    void DrawHistFromFile(TString fileName);

    /*
     * Set output directory for images.
     */
    void SetOutputDir(TString dir) {fOutputDir = dir;}

    /*
     * Set run title. It is also a part of the file name of image files.
     */
    void SetRunTitle(TString title) {fRunTitle = title;}

    /*
     * Set axis rotation title. It is also a part of the file name of image files.
     */
    void SetAxisRotTitle(TString title) {fAxisRotTitle = title;}

    /*
     * Set to TRUE if you want to draw histograms.
     */
    void SetDrawAlignment(Bool_t b) {fDrawAlignment = b;}
    void SetDrawMapping(Bool_t b) {fDrawMapping = b;}
    void SetDrawProjection(Bool_t b) {fDrawProjection = b;}

    void SetIsReconstruction(Bool_t b) {fIsReconstruction = b;}


private:
    TClonesArray* fRichHits; // Array of RICH hits
    TClonesArray* fRichRings; // Array of found RICH rings
    TClonesArray* fRichMirrorPoints;
    TClonesArray* fRichProjections;
    TClonesArray* fRichMCPoints;
    TClonesArray* fMCTracks;
    TClonesArray* fRichRingMatches;
    TClonesArray* fRichRefPlanePoints;
    TClonesArray* fRichPoints;
    TClonesArray* fGlobalTracks;
    CbmHistManager* fHM;
    CbmHistManager* fHM2;
    //CbmRichRecGeoPar* fGP;
    vector<Float_t> fPhi;

    UInt_t fEventNum; // Event counter
    UInt_t fMirrCounter;
    Bool_t fDrawAlignment;
    Bool_t fDrawMapping;
    Bool_t fDrawProjection;
    Bool_t fIsMeanCenter;
    Bool_t fIsReconstruction;
    Double_t fArray[3];

    std::map<string,string> fPathsMap;
    std::map<string,string> fPathsMapEllipse;

    TString fOutputDir;		// Output directory to store figures.
    TString fRunTitle;		// Title of the run.
    TString fAxisRotTitle;	// Rotation around which axis.

    CbmRichRingFitterCOP* fCopFit;
    CbmRichRingFitterEllipseTau* fTauFit;

    CbmRichCorrectionVector(const CbmRichCorrectionVector&);
    CbmRichCorrectionVector operator=(const CbmRichCorrectionVector&);

    ClassDef(CbmRichCorrectionVector, 1);
};

#endif

#ifndef CBMRICHCORRECTION_H
#define CBMRICHCORRECTION_H


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


class CbmRichCorrection : public FairTask //CbmRichProjectionProducerBase
{
private:
    static const int kMAX_NOF_HITS = 100; // Maximum number of hits in ring

public:
	/*
	 * Constructor.
	 */
    CbmRichCorrection();

	/*
	 * Destructor.
	 */
    virtual ~CbmRichCorrection();

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

    void ProjectionProducer();

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
     * Test to apply the misalignment information on the sphere center coordinates from an input file. The procedure is as follow: first translate the sphere
     * center (using opposite values of the mirror center). Then apply the inverse transformation matrix on the point, apply the rotation matrix, which accounts
     * for the correction. After that, apply the transformation matrix to the point and translate it back to its position (using the coordinates of the mirror
     * tile center).
     * The results haven't been conclusive so far. There seems to be a problem with the transformation matrix.
     */
    vector<Double_t> RotateSphereCenter(vector<Double_t> ptM, vector<Double_t> ptC, TGeoNavigator* navi);

    /*
     * Input matrix mat is inverted using the adjugate matrix (= transpose of the cofactor matrix) to give invMat. A test can be also run, to check that
     * mat*invMat = Id.
     */
    void InvertMatrix(Double_t mat[3][3], Double_t invMat[3][3], TGeoNavigator* navi);

    /*
     * From point M and point C uncorrected (coordinates of C = theoretical coordinates of the sphere center) calculates new point M on the mirror.
     * Indeed the fRichMirrorPoints->At(iMirr) gives the point on the rotated mirror and not on the ideally aligned mirror. Even though the correction
     * is minimal, this gives a position more likely to be on the aligned mirror (no misalignment info used).
     */
    void CalculateMirrorIntersection(vector<Double_t> ptM, vector<Double_t> ptCUnCorr, vector<Double_t> &ptMNew);

    /*
     * Calculate the normal of the considered mirror tile, using the sphere center position of the tile (ptC) and the local reflection point on the mirror (ptM) => normalMirr.
     * Then calculate point on sensitive plane from the reflected track extrapolated (ptR2 = reflection of ptR1, with reflection axis = normalMirr).
     * ptR2Center uses ptC for the calculations, whereas ptR2Mirr uses ptM.
     */
    void ComputeR2(vector<Double_t> &ptR2Center, vector<Double_t> &ptR2Mirr, vector<Double_t> ptM, vector<Double_t> ptC, vector<Double_t> ptR1, TGeoNavigator* navi , TString s);

    /*
     * Calculate the intersection point (P) between the track and the PMT plane, as if the track had been reflected by the mirror tile.
     * ptPMirr is calculated using the mirror point (ptM) to define the line reflected by the mirror and towards the PMT plane.
     * ptPR2 is calculated using ptR2Mirr (the reflection of point R1 on the sensitive plane, using ptM for the calculations -> see ComputeR2 method).
     */
    void ComputeP(vector<Double_t> &ptPMirr, vector<Double_t> &ptPR2, vector<Double_t> normalPMT, vector<Double_t> ptM, vector<Double_t> ptR2Mirr, Double_t normalCste);

    /*
     * Function filling the diffX, diffY and distance histograms, from the outPos vector.
     */
    void FillHistProjection(TVector3 outPosIdeal, TVector3 outPosUnCorr, TVector3 outPos, Int_t NofGlobalTracks, vector<Double_t> normalPMT, Double_t constantePMT);

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
    void SetDrawProjection(Bool_t b) {fDrawProjection = b;}

    void SetIsReconstruction(Bool_t b) {fIsReconstruction = b;}

    void SetNumbAxis(TString n) {fNumbAxis = n;}

    void SetTileName(TString t) {fTile = t;}


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
    //CbmRichRecGeoPar* fGP;
    vector<Float_t> fPhi;

    TString fNumbAxis;
    TString fTile;
    UInt_t fEventNum; // Event counter
    Bool_t fDrawProjection;
    Bool_t fIsMeanCenter;
    Bool_t fIsReconstruction;

    TString fOutputDir;		// Output directory to store figures.
    TString fRunTitle;		// Title of the run.
    TString fAxisRotTitle;	// Rotation around which axis.

    CbmRichRingFitterCOP* fCopFit;
    CbmRichRingFitterEllipseTau* fTauFit;

    CbmRichCorrection(const CbmRichCorrection&);
    CbmRichCorrection operator=(const CbmRichCorrection&);

    ClassDef(CbmRichCorrection, 1);
};

#endif

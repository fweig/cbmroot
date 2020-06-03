#ifndef CBMRICHALIGNMENT_H
#define CBMRICHALIGNMENT_H


#include "FairTask.h"
#include "CbmHistManager.h"
#include "CbmRichRingFitterCOP.h"
#include "CbmRichRingFitterEllipseTau.h"
#include <vector>

using namespace std;

class TClonesArray;
class TH1D;
class TH2D;


class CbmRichAlignment : public FairTask
{
private:
    static const int kMAX_NOF_HITS = 100; // Maximum number of hits in ring

public:
	/*
	 * Constructor.
	 */
    CbmRichAlignment();

	/*
	 * Destructor.
	 */
    virtual ~CbmRichAlignment();

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
     * Histogram initialization for alignment method.
     */
    void InitHistAlignment();

    /*
     * Calculate the distances between C and C', the Cherenkov distances and angles (Theta_Ch vs Phi_Ch) for each photon hit in a given event and draw the corresponding
     * distributions.
     */
    void CalculateAnglesAndDrawDistrib();

    /*
     * Get x and y positions on the PMT plane from the extrapolated track.
     */
    void GetTrackPosition(Double_t &x, Double_t &y);

    /*
     * Draw histograms for alignment method.
     */
    void DrawHistAlignment();

    /*
     *
     */
    void DrawFit(vector<Double_t> &outputFit, Int_t thresh);

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

    /*
     *
     */
    void SetNumbAxis(TString n) {fNumbAxis = n;}

    /*
     *
     */
    void SetTileName(TString a) {fTile = a;}


private:
    TClonesArray* fRichHits; // Array of RICH hits
    TClonesArray* fRichRings; // Array of found RICH rings
    TClonesArray* fRichProjections;
    TClonesArray* fRichPoints;
    TClonesArray* fMCTracks;
    TClonesArray* fRichRingMatches;
    TClonesArray* fRichMirrorPoints;
//    TClonesArray* fRichRefPlanePoints;
//    TClonesArray* fRichMCPoints;
//    TClonesArray* fGlobalTracks;
    CbmHistManager* fHM;

    UInt_t fEventNum; // Event counter
	TString fNumbAxis;			// Misalignment applied on the geometry.
	TString fTile;
    Bool_t fDrawAlignment;	// If TRUE, draws the alignment and fitting plots.
    vector<Float_t> fPhi;

    TString fOutputDir;		// Output directory to store figures.
    TString fRunTitle;		// Title of the run.
    TString fAxisRotTitle;	// Rotation around which axis.

    CbmRichRingFitterCOP* fCopFit;
    CbmRichRingFitterEllipseTau* fTauFit;

    CbmRichAlignment(const CbmRichAlignment&);
    CbmRichAlignment operator=(const CbmRichAlignment&);

    ClassDef(CbmRichAlignment, 1);
};

#endif

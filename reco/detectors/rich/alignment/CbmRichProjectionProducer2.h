#ifndef CbmRichProjectionProducer2_H
#define CbmRichProjectionProducer2_H

#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmRichRingLight.h"
#include <vector>
#include "TString.h"
#include "CbmRichRecGeoPar.h"
#include "TGeoNavigator.h"
#include "CbmRichProjectionProducerBase.h"
#include "FairTrackParam.h"

using namespace std;

class TClonesArray;

class CbmRichProjectionProducer2 : public CbmRichProjectionProducerBase
{
public:

	/*
	 * Constructor.
	 */
    CbmRichProjectionProducer2();

	/*
	 * Destructor.
	 */
    virtual ~CbmRichProjectionProducer2();

    virtual void Init();

    /*
     * From incoming track on the mirrors, do reflection of its trajectory and extrapolation of its intersection on the PMT plane.
     */
    virtual void DoProjection(TClonesArray* projectedPoint);

    /*
     *
     */
    Double_t* ProjectionProducer(FairTrackParam* point);

    /*
     *
     */
    void GetPmtNormal(Int_t NofPMTPoints, vector<Double_t> &normalPMT, Double_t &normalCste);

    /*
     * Calculate the normal of the considered mirror tile, using the sphere center position of the tile (ptC) and the local reflection point on the mirror (ptM) => normalMirr.
     * Then calculate point on sensitive plane from the reflected track extrapolated (ptR2 = reflection of ptR1, with reflection axis = normalMirr).
     * ptR2Center uses ptC for the calculations, whereas ptR2Mirr uses ptM.
     */
    void ComputeR2(vector<Double_t> &ptR2Center, vector<Double_t> &ptR2Mirr, vector<Double_t> ptM, vector<Double_t> ptC, vector<Double_t> ptR1, TGeoNavigator* navi, TString s);

    /*
     * Calculate the intersection point (P) between the track and the PMT plane, as if the track had been reflected by the mirror tile.
     * ptPMirr is calculated using the mirror point (ptM) to define the line reflected by the mirror and towards the PMT plane.
     * ptPR2 is calculated using ptR2Mirr (the reflection of point R1 on the sensitive plane, using ptM for the calculations -> see ComputeR2 method).
     */
    void ComputeP(vector<Double_t> &ptPMirr, vector<Double_t> &ptPR2, vector<Double_t> normalPMT, vector<Double_t> ptM, vector<Double_t> ptR2Mirr, Double_t normalCste);

    /*
     * Set output directory for images.
     */
    void SetOutputDir(TString dir) {fOutputDir = dir;}

    /*
     *
     */
    void SetNumbAxis(TString n) {fNumbAxis = n;}

    /*
     *
     */
    void SetTileName(TString t) {fTile = t;}


private:
    TClonesArray* fTrackParams;
    TClonesArray* fMCTracks;
    TClonesArray* fRichPoints;

    TString fNumbAxis;
    TString fTile;
    TString fOutputDir;
    Int_t fEventNum; // Event counter

    /*
     * Copy constructor.
     */
    CbmRichProjectionProducer2(const CbmRichProjectionProducer2&);
    /*
     * Assignment operator.
     */
    CbmRichProjectionProducer2& operator=(const CbmRichProjectionProducer2&);

    ClassDef(CbmRichProjectionProducer2, 1);
};

#endif

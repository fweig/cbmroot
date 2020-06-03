#ifndef RICH_MIRRORS_CBMRICHRONCHIANA_H_
#define RICH_MIRRORS_CBMRICHRONCHIANA_H_

#include "TObject.h"
#include "TVector3.h"
#include <string>
#include <vector>
#include <cmath>

using namespace std;

class TH2;

class CbmRichRonchiIntersectionData
{
public:

    CbmRichRonchiIntersectionData(): 
    fPixelX(0),
    fPixelY(0),
    fLineX(0),
    fLineY(0),
    fNormalRadX(0.),
    fNormalRadY(0.),
    fOrderedLineX(0),
    fOrderedLineY(0),
    fCcdV(),
    fRulingV(),
    fMirrorV(),

    fTL(),
    fTR(),
    fBL(),
    fBR(),

    fTLRot(),
    fTRRot(),
    fBLRot(),
    fBRRot(),

    fTLSph(),
    fTRSph(),
    fBLSph(),
    fBRSph(),
    
    fDeviation(0.),
    fRLoc(0.) {}

    int fPixelX;
    int fPixelY;
    int fLineX;
    int fLineY;

    double fNormalRadX; // Normal X in radians
    double fNormalRadY;
    
    int fOrderedLineX;
    int fOrderedLineY;

    TVector3 fCcdV;    // XYZ positions CCD in microns
    TVector3 fRulingV; // XYZ positions ronchi ruling in microns
    TVector3 fMirrorV; // XYZ positions of mirror in microns

    TVector3 fTL;  // corners of segment BEFORE rotation
    TVector3 fTR;
    TVector3 fBL;
    TVector3 fBR;

    TVector3 fTLRot;  // corners of segment AFTER rotation
    TVector3 fTRRot;
    TVector3 fBLRot;
    TVector3 fBRRot;
    
    TVector3 fTLSph;  // corners of segment after 'DoSphere'
    TVector3 fTRSph;
    TVector3 fBLSph;
    TVector3 fBRSph;

    
    std::vector<double> fVecTL;  // vectors that point from center of a segment to the corners (for calculation of rotations)       --CAN BE DELETED!?
    std::vector<double> fVecTR;
    std::vector<double> fVecBL;
    std::vector<double> fVecBR;
    
    double fDeviation;
    double fRLoc;
};

class CbmRichRonchiLineData
{
public:

    CbmRichRonchiLineData():
    fNofPoints(0),
    fMeanPrimary(0),
    fMeanSecondary(0),
    fLineInd(0) {}

    int fNofPoints;
    double fMeanPrimary;
    double fMeanSecondary;  
    int fLineInd;
};

class CbmRichRonchiAna : public TObject 
{
public:
    CbmRichRonchiAna();
    
    virtual ~CbmRichRonchiAna();

    void Run();

    void SetTiffFileNameV(const string& fileName) {fTiffFileNameV = fileName;}
    void SetTiffFileNameH(const string& fileName) {fTiffFileNameH = fileName;}

private:

    string fTiffFileNameV;
    string fTiffFileNameH;
    
    // constant values
    double fPi;
    double fRadiusMirror;              // in microns
    double fEdgeLengthCCD;             // in microns
    double fCcdPixelSize;              // in microns
    double fPitchGrid;                 // in microns; distance of lines
    double fImageWidth;                // in pixels    
    
    // values to be measured first
    double fDistRulingCCD;                // in microns 
    double fDistMirrorCCD;                // in microns 
    double fDistMirrorRuling;    
    double fOffsetCCDOptAxisX;            // in microns; horizontal distance of center of CCD to optical axis of mirror
    double fOffsetCCDOptAxisY;            // in microns; vertical distance of center of CCD to optical axis of mirror
    double fOffsetLEDOpticalAxisX;        // in microns; horizontal distance of light source to optical axis of mirror
    double fOffsetLEDOpticalAxisY;
    double fCorrection;                   // correction value; defines center of mirror at exactly 3,000,000 mum in z dircetion
    
    // values that will be determined by function
    double fCenterCcdX;              // in pixels; approximate center of image on CCD, will be defined in 'DoLocalNormal' function
    double fCenterCcdY;              // in pixels
    double fImageCenterMirrorX;      // in pixels; approximate center of image on mirror  (NEEDED??)
    double fImageCenterMirrorY;      // in pixels

    vector<vector<double> > ReadTiffFile(const string& fileName);

    void DoRotation(vector<vector<double> >& data);

    void FillH2WithVector(TH2* hist, const vector<vector<double> >& data);

    void DoMeanIntensityY(vector<vector<double> >& data);

    void DoPeakFinderY(vector<vector<double> >& data);

    void DoSmoothLines(vector<vector<double> >& data);
    
    void DoLineSearch(vector<vector<double> >& data);

    vector<vector<double> > DoSuperpose(const vector<vector<double> >& dataH, const vector<vector<double> >& dataV);

    vector<CbmRichRonchiIntersectionData> DoIntersection(vector<vector<double> >& dataH, const vector<vector<double> >& dataV);

    void DoOrderLines(vector<CbmRichRonchiIntersectionData> &intersections, const string &option);
    bool AreTwoSegmentsSameLine(const CbmRichRonchiLineData *line1, const CbmRichRonchiLineData *line2);
    void UpdateIntersectionLineInd(vector<CbmRichRonchiIntersectionData> &intersections, CbmRichRonchiLineData *line1,
                                   CbmRichRonchiLineData *line2, const string &option);

    void DoLocalNormal(vector<CbmRichRonchiIntersectionData> &data);
    void DrawXYMum(vector<CbmRichRonchiIntersectionData> &data);
    void DrawXZProjection(vector<CbmRichRonchiIntersectionData> &data, int orderedLineY, double scale);
    void DrawMirrorSegments(vector<CbmRichRonchiIntersectionData> &data, int orderedLineX, int orderedLineY);
    void DrawMirrorSegmentsSphereAll(vector<CbmRichRonchiIntersectionData> &data);
    void DrawMirrorSegmentsSphere(vector<CbmRichRonchiIntersectionData> &data, int orderedLineX, int orderedLineY);
    void DrawOneMirrorSegment(const TVector3& tl, const TVector3& tr, const TVector3& bl, const TVector3& br, int color);
    void DrawSphere(vector<CbmRichRonchiIntersectionData> &data);
    void DrawRLocMum(vector<CbmRichRonchiIntersectionData> &data);

    void DoHeight(vector<CbmRichRonchiIntersectionData> &intersections);

    void DoSphere(vector<CbmRichRonchiIntersectionData> &intersections);

    int GetIndexForLineXLineY(int lineX, int lineY, vector<CbmRichRonchiIntersectionData> &data);
    int GetMinIndexForLineX(int lineX, vector<CbmRichRonchiIntersectionData> &data);
    int GetMinIndexForLineY(int lineY, vector<CbmRichRonchiIntersectionData> &data);

    void DoHeightCorners(vector<CbmRichRonchiIntersectionData> &intersections);

    void DoScanLineHeight(vector<CbmRichRonchiIntersectionData> &intersections);

    void DoCalculateRemaining(vector<CbmRichRonchiIntersectionData> &intersections);

    void DoIntegrate(vector<CbmRichRonchiIntersectionData> &intersections);

    void DoAverageSurroundings(vector<CbmRichRonchiIntersectionData> &intersections);

    void RotatePointImpl(TVector3 *inPos, TVector3 *outPos, Double_t rotX, Double_t rotY, TVector3* cV);
    
    void DoDeviation(vector<CbmRichRonchiIntersectionData> &data);
    
    void DoRLoc(vector<CbmRichRonchiIntersectionData> &data);

        /**
    * \brief Copy constructor.
    */
        CbmRichRonchiAna(const CbmRichRonchiAna &);

    /**
    * \brief Assignment operator.
    */
    CbmRichRonchiAna &operator=(const CbmRichRonchiAna &);

    ClassDef(CbmRichRonchiAna, 1)
};

#endif /* RICH_MIRRORS_CBMRICHRONCHIANA_H_ */


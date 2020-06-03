// -------------------------------------------------------------------------
// -----               CbmPointSetArray header file                    -----
// -----                Created 05/07/22  by J. Brandt                 -----
// -----               Following class TEvePointSetArray               -----
// -------------------------------------------------------------------------


/** CbmPointSetArray
 * @author J. Brandt
 * @since 05.07.18
 *   Class to Display Array of Pointsets and calculate color and marker of bins
 *   inherited from TEvePointSetArray
 **
 **/

#ifndef CBMPOINTSETARRAY_H
#define CBMPOINTSETARRAY_H

#include <RtypesCore.h>    // for Int_t, Double_t
#include <Rtypes.h>        // for THashConsistencyHolder, ClassDef
#include <TEvePointSet.h>  // for TEvePointSetArray

class CbmPointSetArray : public TEvePointSetArray
 {
    friend class CbmPointSetArrayEditor;
 
    CbmPointSetArray(const CbmPointSetArray&);            // Not implemented
    CbmPointSetArray& operator=(const CbmPointSetArray&); // Not implemented

 public:
    CbmPointSetArray(const char* name="CbmPointSetArray", const char* title="");
    virtual ~CbmPointSetArray();

    void InitValues(Int_t npoints);     // Init arrays for storing of time,cluSize,...
    void FillValues(Int_t id, Double_t time, Double_t tot, Int_t cluSize,Int_t index);
                                        // fill time[id],... index[id] to use information for later color-coding
    void ApplyColorMode();              // apply colorcoding according to fColorMode
    void ApplyMarkerMode();             // apply markercoding according to fMarkerMode
    void ApplyTitles();                 // setTitle of each bin and ComputeBBox

    /* Setters */
    void SetColorMode(Int_t colorMode) {fColorMode=colorMode;}
    void SetMarkerMode(Int_t markerMode) {fMarkerMode=markerMode;}

    /* Accessors */
    Int_t GetColorMode() const {return fColorMode;}
    Int_t GetMarkerMode() const {return fMarkerMode;}
 
 private:
    Int_t fColorMode;        // determine how to color-code points
    Int_t fMarkerMode;       // determine how to markersize-code points
    Double_t* fTime;         // array for times of hits stored in PointSetArray
    Int_t* fCluSize;         // array for CluSizes of hits stored in PointSetArray
    Double_t* fToT;          // array for ToTs of hits stored in PointSetArray
    Int_t* fIndex;           // array of bin-indices in which hits of PointSetArray are stored
    Int_t fNPoints;          // number of points stored in PointSetArray

 
    ClassDef(CbmPointSetArray, 0); //CbmPointSetArray to implement Additional ColorCoding and other functionalities 
// Array of TEvePointSet's filled via a common point-source; range of displayed TEvePointSet's can be controlled, based on a separating quantity provided on fill-time by a user.
 };

#endif

/**
 * \file CbmRichTrackExtrapolationLittrack.h
 *
 * \brief "TrackExtrapolation" from STS tracks based on Littrack.
 * It reads the track array form STS and extrapolates those to
 * be projected to the Rich Photodetector to some z-Plane in RICH
 *
 * \author Semen Lebedev
 * \date 2016
 **/

#ifndef CBM_RICH_TRACK_EXTRAPOLATION_LITTRACK
#define CBM_RICH_TRACK_EXTRAPOLATION_LITTRACK

#include "CbmRichTrackExtrapolationBase.h"
#include "CbmLitPtrTypes.h"

class TClonesArray;
class CbmLitTGeoTrackPropagator;

/**
 * \class CbmRichTrackExtrapolationLittrack
 *
 * \brief "TrackExtrapolation" from STS tracks based on Littrack.
 * It reads the track array form STS and extrapolates those to
 * be projected to the Rich Photodetector to some z-Plane in RICH
 *
 * \author Semen Lebedev
 * \date 2016
udia Hoehne
 * \date 206
 **/
class CbmRichTrackExtrapolationLittrack : public CbmRichTrackExtrapolationBase
{
public:
    
    /**
     * \brief Default constructor.
     */
    CbmRichTrackExtrapolationLittrack();
    
    /**
     * \brief Destructor.
     */
    virtual ~CbmRichTrackExtrapolationLittrack();
    
    /**
     * \brief Inherited from CbmRichTrackExtrapolationBase.
     */
    virtual void Init();
    
    /**
     * \brief Inherited from CbmRichTrackExtrapolationBase.
     */
    virtual void DoExtrapolation(
                                 TClonesArray* globalTracks,
                                 TClonesArray* extrapolatedTrackParams,
                                 double z);
    
private:
    TClonesArray* fStsTracks;
    TrackPropagatorPtr fLitPropagator;
    
private:
    /**
     * \brief Copy constructor.
     */
    CbmRichTrackExtrapolationLittrack(const CbmRichTrackExtrapolationLittrack&);
    
    /**
     * \brief Assignment operator.
     */
    void operator=(const CbmRichTrackExtrapolationLittrack&);
};

#endif

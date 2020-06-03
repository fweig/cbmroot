
#ifndef CBM_RICH_RECO_QA
#define CBM_RICH_RECO_QA

#include "FairTask.h"
class TClonesArray;
class CbmRichRing;
class TCanvas;
class CbmHistManager;
class TH1D;
class TH2D;
class TH2;
class TH3;
class CbmMCTrack;

#include <vector>
#include <map>

using namespace std;

class CbmRichRecoQa : public FairTask
{
    
public:
    /**
     * \brief Standard constructor.
     */
    CbmRichRecoQa();
    
    /**
     * \brief Standard destructor.
     */
    virtual ~CbmRichRecoQa() {}
    
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
    
    static Bool_t IsMcPrimaryElectron(
    				const CbmMCTrack* mctrack);

    static Bool_t IsMcPion(
    				const CbmMCTrack* mctrack);
    
    /**
     * \brief Set output directory where you want to write results (figures and json).
     * \param[in] dir Path to the output directory.
     */
    void SetOutputDir(const string& dir) {fOutputDir = dir;}
    
    /**
         * \brief Draw histogram from file
         */
    void DrawFromFile(
          const string& fileName,
		  const string& outputDir);
    
    void SetCorrection(string s) { fCorrection = s; }

private:
    
    /**
     * \brief Initialize histograms.
     */
    void InitHistograms();
    
    /**
     * \brief Fill map mcTrackId -> nof RICH hits
     */
    void FillRichRingNofHits();

    /**
     * \brief Fill histogramms related to ring track distance
     */
    void FillRingTrackDistance();

    /**
     * \brief Fill histograms related to study of the source of ring-track mismatch
     */
    void RingTrackMismatchSource();

    /**
     *  \brief Draw histograms.
     */
    void DrawHist();
    
    /**
     * \brief Return string with mean, RMS and overflow percent for input TH1.
     */
    string GetMeanRmsOverflowString(
    		TH1* h,
			Bool_t withOverflow = true);

    /**
     *  \brief Draw histograms related to ring-track distance for pions or electrons (+/-).
     */
    void DrawRingTrackDistHistWithSuffix(const string& suffix );

    void DrawRingTrackDistHistWithSuffixPmtHalf(const string& suffix );

    /*
     * \brief Check that the ring with an input MCTrackId was found
     */
    bool WasRingFound(Int_t mcTrackId);

    /*
     * \brief Check that the ring was matched with some global track
     */
    bool WasRingMatched(Int_t mcTrackId);

    /*
     * \brief Check that the Sts track projection was matched with RICH ring
     */
    bool WasRichProjectionMatched(Int_t stsTrackId);

    /*
     * Check that STS track has projection in the RICH
     */
    bool HasRichProjection(Int_t stsTrackId);


    /**
     * \brief Copy constructor.
     */
    CbmRichRecoQa(const CbmRichRecoQa&);
    
    /**
     * \brief Assignment operator.
     */
    CbmRichRecoQa& operator=(const CbmRichRecoQa&);

    
    CbmHistManager* fHM;
    
    Int_t fEventNum;
    
    string fOutputDir; // output dir for results
    
    TClonesArray* fMCTracks;
    TClonesArray* fRichPoints;
    TClonesArray* fRichDigis;
    TClonesArray* fRichHits;
    TClonesArray* fRichRings;
    TClonesArray* fRichRingMatches;
    TClonesArray* fGlobalTracks;
    TClonesArray* fStsTracks;
    TClonesArray* fStsTrackMatches;
    TClonesArray* fRichProjections;
    
    // Number of hits in the MC RICH ring
    std::map<Int_t, Int_t> fNofHitsInRingMap;

    string fCorrection; // Is correction on or not

    vector<TCanvas*> fCanvas;
    
    ClassDef(CbmRichRecoQa,1)
};

#endif



#ifndef CBM_RICH_RECO_TB_QA
#define CBM_RICH_RECO_TB_QA

#include "FairTask.h"
class CbmMCDataArray;
class TClonesArray;
class CbmHistManager;
class CbmRichPoint;
class CbmMCTrack;
class CbmMCEventList;
class CbmDigiManager;

#include <vector>
#include <map>

using namespace std;

class CbmRichRecoTbQa : public FairTask
{
    
public:
    /**
     * \brief Standard constructor.
     */
    CbmRichRecoTbQa();
    
    /**
     * \brief Standard destructor.
     */
    virtual ~CbmRichRecoTbQa() {}
    
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
    
private:
    
    /**
     * \brief Initialize histograms.
     */
    void InitHistograms();

    void DrawHist();

    void Process();

    void ProcessMc();

    void RingRecoEfficiency();

    Int_t GetNofPrimaryMcTracks(Int_t iEv);

    Bool_t IsCherenkovPhoton(
            const CbmRichPoint* point,
            Int_t fileId,
            Int_t eventId);

    Bool_t IsCherenkovPhotonFromPrimaryElectron(
            const CbmRichPoint* point,
            Int_t fileId,
            Int_t eventId);

    Bool_t IsCherenkovPhotonFromSecondaryElectron(
            const CbmRichPoint* point,
            Int_t fileId,
            Int_t eventId);

    Bool_t IsMcPrimaryElectron(
            const CbmMCTrack* mctrack);

    Bool_t IsCherenkovPhotonFromPion(
            const CbmRichPoint* point,
            Int_t fileId,
            Int_t eventId);

    Bool_t IsMcPion(
                const CbmMCTrack* mctrack);

    void DrawTimeLog (
            const string& hMainName,
            Int_t nofLogEvents,
            bool withNoise = false);

    /**
     * \brief Copy constructor.
     */
    CbmRichRecoTbQa(const CbmRichRecoTbQa&);
    
    /**
     * \brief Assignment operator.
     */
    CbmRichRecoTbQa& operator=(const CbmRichRecoTbQa&);
    
    
    CbmHistManager* fHM;
    
    Int_t fTimeSliceNum;
    
    Int_t fNofLogEvents;
    string fOutputDir; // output dir for results
    
    CbmMCDataArray* fMCTracks;
    CbmMCDataArray* fRichPoints;
    CbmMCDataArray* fStsPoints;
    CbmDigiManager* fDigiMan;
    TClonesArray* fRichHits;
    TClonesArray* fRichRings;
    TClonesArray* fRichRingMatches;
    CbmMCEventList* fEventList;
    
    vector<pair<Int_t, Int_t> > fRecRings;

    ClassDef(CbmRichRecoTbQa,1)
};

#endif


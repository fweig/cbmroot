/** @file CbmStsRecoModule.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 21.03.2020
 **/

#ifndef CBMSTSRECOMODULE_H
#define CBMSTSRECOMODULE_H 1


#include <mutex>
#include <TNamed.h>
#include "CbmStsCluster.h"
#include "CbmStsHit.h"

class TGeoHMatrix;
class CbmStsAlgoAnaCluster;
class CbmStsAlgoFindClusters;
class CbmStsAlgoFindHits;
class CbmStsHit;
class CbmStsDigi;
class CbmStsModule;
class CbmStsParModule;
class CbmStsParSensor;
class CbmStsParSensorCond;


/** @class CbmStsRecoModule
 ** @brief Class for reconstruction in one STS module
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 05.04.2017
 ** @date 21.03.2020
 **
 ** The module represents one module/sensor in the STS setup.
 ** Reconstruction in the modules is independent from other
 ** modules and can thus pe performed in parallel. The
 ** reconstruction module performs cluster finding, cluster
 ** analysis and hit finding.
 **
 ** The implementation assumes the module to be connected to
 ** exactly one double-sided strip sensor. The digi channel number
 ** must equal to the strip number on the front side of the sensor,
 ** and to strip number + number of strips on the back side.
 **/
class CbmStsRecoModule : public TNamed
{

  public:

    /** @brief Default constructor **/
    CbmStsRecoModule();


    /** @brief Standard constructor
     ** @param setupModule  Corresponding setup element
     ** @param parModule    Module parameters
     ** @param parSensor    Sensor parameters
     ** @param lorentzShiftF  Average Lorentz shift sensor front side [cm]
     ** @param lorentzShiftB  Average Lorentz shift sensor back side [cm]
     **
     ** The Lorentz shift will be used to correct the hit position in
     ** hit finding.
     **/
    CbmStsRecoModule(CbmStsModule* setupModule,
                     const CbmStsParModule& parModule,
                     const CbmStsParSensor& parSensor,
                     Double_t lorentzShiftF, Double_t lorentzShiftB);


    /** @brief Copy constructor (disabled) **/
    CbmStsRecoModule(const CbmStsRecoModule&) = delete;


    /** @brief Assignment operator (disabled) **/
    CbmStsRecoModule& operator=(const CbmStsRecoModule&) = delete;


    /** @brief Destructor **/
    virtual ~CbmStsRecoModule();


    /** @brief Add a digi to the processing queue
     ** @param digi Pointer to digi object
     ** @param digiIndex  Index of digi in input array
     **/
    void AddDigiToQueue(const CbmStsDigi* digi, Int_t digiIndex);


    /** @brief Output front-side clusters
     ** @return Vector of front-side clusters
     **/
    const std::vector<CbmStsCluster>& GetClustersF() const {
      return fClustersF;
    }


    /** @brief Output back-side clusters
     ** @return Vector of back-side clusters
     **/
    const std::vector<CbmStsCluster>& GetClustersB() const {
      return fClustersB;
    }


    /** @brief Output hits
     ** @return Vector of hits
     **/
    const std::vector<CbmStsHit>& GetHits() const {
      return fHits;
    }


    /** @brief Perform reconstruction **/
    void Reconstruct();


    /** @brief Clear input queue **/
    void Reset();


    /** @brief Info to string **/
    std::string ToString() const;


  private:

    /** @brief Set and check the needed parameters **/
    void Init();



  private:

    // --- Algorithms
    CbmStsAlgoAnaCluster* fClusterAna = nullptr;       //! ///< Algo
    CbmStsAlgoFindClusters* fClusterFinder = nullptr;  //! ///< Algo
    CbmStsAlgoFindHits* fHitFinder = nullptr;          //! ///< Algo
    std::mutex fLock { };

    // --- Parameters
    CbmStsModule* fSetupModule = nullptr;                 //!
    const CbmStsParModule* fParModule = nullptr;          //!
    const CbmStsParSensor* fParSensor = nullptr;          //!
    Double_t fDyActive      = 0.;   ///< Active sensor size in y
    UInt_t fNofStrips       = 0;    ///< Number of sensor strips
    Double_t fStripPitch    = 0.;   ///< Sensor strip pitch [cm]
    Double_t fStereoFront   = 0.;   ///< Strip stereo angle front side [deg]
    Double_t fStereoBack    = 0.;   ///< Strip stereo angle back side [deg]
    TGeoHMatrix* fMatrix = nullptr; ///< Sensor position in global C.S. [cm]
    Double_t fLorentzShiftF = 0.;   ///< Average Lorentz shift front side [cm|
    Double_t fLorentzShiftB = 0.;   ///< Average Lorentz shift back side [cm|

    // --- Data
    std::vector<std::pair<const CbmStsDigi*, Long64_t>> fDigisF { }; //!
    std::vector<std::pair<const CbmStsDigi*, Long64_t>> fDigisB { }; //!
    std::vector<CbmStsCluster> fClustersF{ };                        //!
    std::vector<CbmStsCluster> fClustersB { };                       //!
    std::vector<CbmStsHit> fHits { };                                //!

    // --- Settings
    Double_t fTimeCutDigiSig = 3.;     ///< Time cut for cluster finding (in sigma)
    Double_t fTimeCutDigiAbs = -1.;    ///< Time cut for cluster finding (in ns)
    Double_t fTimeCutClusterSig = 4.;  ///< Time cut for hit finding (in ns)
    Double_t fTimeCutClusterAbs = -1.; ///< Time cut for hit finding (in sigma)
    Bool_t fConnectEdgeFront = kFALSE; ///< Round-the edge clustering front side
    Bool_t fConnectEdgeBack = kFALSE;  ///< Round-the edge clustering back side


    ClassDef(CbmStsRecoModule, 1);

};

#endif /* CBMSTSRECOMODULE_H */

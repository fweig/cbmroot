/** @file CbmStsAlgoAnaCluster.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 23.03.2020
 **/

#ifndef CBMSTSALGOANACLUSTER_H
#define CBMSTSALGOANACLUSTER_H 1

#include <memory>
#include <TObject.h>

class CbmDigiManager;
class CbmStsCluster;
class CbmStsParModule;
class CbmStsPhysics;


/** @class CbmStsAlgoAnaCluster
 ** @brief Determination of cluster parameters
 ** @author V.Friese <v.friese@gsi.de>
 ** @since 23.03.2020
 **
 ** This class implements the determination of STS cluster parameters
 ** (time, position, charge) and their errors from the digis
 ** contained in the cluster.
 **
 ** The mathematics is described in:
 ** H. Malygina, Hit Reconstruction for the Silicon Tracking System
 ** of the CBM experiment, PhD thesis, Goethe-Universität Frankfurt, 2018
 **/
class CbmStsAlgoAnaCluster : public TObject
{

  public:

    /** @brief Constructor **/
    CbmStsAlgoAnaCluster();


    /** @brief Copy constructor (disabled) **/
    CbmStsAlgoAnaCluster(const CbmStsAlgoAnaCluster&) = delete;


    /** @brief Assignment operator (disabled) **/
    CbmStsAlgoAnaCluster& operator=(const CbmStsAlgoAnaCluster&) = delete;


    /** @brief Destructor **/
    virtual ~CbmStsAlgoAnaCluster() { };


    /** @brief Algorithm execution
     ** @param cluster    Pointer to cluster object
     ** @param module     Pointer to CbmStsRecoModule to be operated on
     **/
    void Exec(CbmStsCluster& cluster, const CbmStsParModule* module);



  private:

    /** @brief Analyse single-digi cluster
     ** @param cluster Pointer to cluster object
     ** @param module  Pointer to module object
     **/
    void AnaSize1(CbmStsCluster& cluster, const CbmStsParModule* modPar);


    /** @brief Analyse two-digi cluster
     ** @param cluster Pointer to cluster object
     ** @param module  Pointer to module object
     **/
    void AnaSize2(CbmStsCluster& cluster, const CbmStsParModule* modPar);


    /** @brief Analyse cluster with more than two digis
     ** @param cluster Pointer to cluster object
     ** @param module  Pointer to module object
     **/
    void AnaSizeN(CbmStsCluster& cluster, const CbmStsParModule* modPar);


    /** @brief Weighted mean cluster position
     ** @param cluster Pointer to cluster object
     ** @param module  Pointer to module object
     ** @return cluster position
     **
     ** This procedure is used when the result obtained by AnaSizeN
     ** are outside the range of the involved channels. This can happen
     ** e.g. for incomplete clusters, e.g. those cut at the edge of
     ** sensor.
     **/
    Double_t WeightedMean(CbmStsCluster& cluster,
                          const CbmStsParModule* modPar);


  private:

    CbmDigiManager* fDigiMan = nullptr;   //! Interface to digi data
    CbmStsPhysics* fPhysics = nullptr;    //! Instance of physics tool


    ClassDef(CbmStsAlgoAnaCluster, 1);

};

#endif /* CBMSTSALGOANACLUSTER_H */

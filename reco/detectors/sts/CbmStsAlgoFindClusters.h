/** @file CbmStsAlgoFindClusters.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 05.04.20174
 **/

#ifndef CBMSTSALGOFINDCLUSTERS_H
#define CBMSTSALGOFINDCLUSTERS_H 1


#include <Rtypes.h>

#include <vector>

class CbmStsCluster;
class CbmStsDigi;
class CbmStsParModule;


/** @class CbmStsAlgoFindClusters
 ** @brief Algorithm for cluster finding in a linear array of channels
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 05.04.2017
 ** @date 22.03.2020
 **
 ** Cluster finding is performed in a linear array of channels,
 ** corresponding e.g. to one side of a STS sensor / module.
 ** A cluster is defined by a group of matching digis. Two digis
 ** are considered matching if they are in neighbouring channels
 ** and their time difference is less than the chosen limit.
 **
 ** Clustering is performed in a streaming way, i.e. on adding
 ** of each digi by the method ProcessDigi. If a cluster in the same
 ** or in a neighbour channel is found which does not match the digi,
 ** it is written to the output. This makes the runtime of the
 ** algorithm independent of input data size, but has as a consequence
 ** that the output clusters are not sorted w.r.t. time.
 **
 ** The time resolution is assumed to be the same for all digis and
 ** has to be specified to the Exec method. The time cut can be
 ** specified in units of the time resolution or in absolute units.
 ** The latter, if positive, overrides the former.
 **
 ** Clustering around-the-edge means periodic boundary conditions,
 ** i.e., the first and the last channel are considered neighbours.
 **
 ** The digis are connected to the cluster in the order left to right,
 ** i.e. with ascending channel number. In case of clustering round
 ** the edge, the channel at the right edge is considered left neighbour
 ** of the first channel, i.e. the cluster starts with high channel
 ** number.
 **
 ** The algorithm is described in:
 ** V. Friese, EPJ Web of Conferences 214 (2019) 01008
 **/
class CbmStsAlgoFindClusters {

public:
  /** @brief Typedef for input data **/
  typedef std::pair<const CbmStsDigi*, Long64_t> InputData;


  /** @brief Default constructor **/
  CbmStsAlgoFindClusters() {};


  /** @brief Destructor **/
  virtual ~CbmStsAlgoFindClusters() {};


  /** @brief Algorithm execution
     ** @param input  Vector of input data (pairs of digi and index)
     ** @param output Vector of output clusters
     ** @param address  Cluster address (module)
     ** @param nChannels  Number of channels
     ** @param channelOffset  Number of first channel
     ** @param timeResol  Time resolution of digis [ns]
     ** @param timeCutSigma  Time cut in units of time resolution
     ** @param timeCutAbs  Time cut in ns
     ** @param connectEdge  If true, clustering round-the-edge is done.
     ** @param isBackSide  Treat back-side clusters w.r.t. channel number
     **
     ** The channel numbers are assumed to be consecutive, starting with
     ** channelOffset. If connectEdge is specififed, the first channel
     ** and the last channel are considered neighbours.
     **/
  Long64_t Exec(const std::vector<InputData>& input, std::vector<CbmStsCluster>& output, UInt_t address,
                UShort_t nChannels, UShort_t channelOffset, Double_t timeCutSigma, Double_t timeCutAbs,
                Bool_t connectEdge, const CbmStsParModule* modPar);


private:
  /** @brief Number of left neighbour channel
     ** @param channel  Channel number
     ** @return Number of left neighbour
     **
     ** In case of clustering round-the-edge, the left neighbour of the
     ** first channel is the last channel. Otherwise, it is -1.
     **/
  Short_t ChanLeft(UShort_t channel)
  {
    if (fConnectEdge) return (channel == 0 ? fNofChannels - 1 : channel - 1);
    return channel - 1;
  }


  /** @brief Number of right neighbour channel
     ** @param channel  Channel number
     ** @return Number of right neighbour
     **
     ** In case of clustering round-the-edge, the right neighbour of the
     ** last channel is the first channel. Otherwise, it is the increment.
     **/
  Short_t ChanRight(UShort_t channel)
  {
    if (fConnectEdge) return (channel == fNofChannels - 1 ? 0 : channel + 1);
    return channel + 1;
  }


  /** @brief Check for a matching digi in a given channel
     ** @param channel  Channel number
     ** @param time     Time [ns]
     ** @return         kTRUE if matching digi found
     **
     ** The digi is considered matching if the time difference between
     ** the time argument and the time of the active digi in the channel
     ** is within the time window defined by the cut value.
     **/
  Bool_t CheckChannel(Short_t channel, Double_t time);


  /** @brief Create a cluster from an active channel
     ** @param channel  Channel number
     **
     ** Starting from the specified channel, a cluster is created
     ** by searching for active neighbours left and right until
     ** an inactive channel is found.
     **
     ** No action if the channel is not active.
     **/
  void CreateCluster(UShort_t channel);


  /** @brief Check for a channel being active
     ** @param channel  Channel number
     ** @return kTRUE is the channel is active; else kFALSE
     **/
  Bool_t IsActive(Short_t channel)
  {
    if (channel < 0 || channel >= fNofChannels) return kFALSE;
    return fStatus[channel].first > -1;
  }


  /** @brief Process one input digi
     ** @param channel   Channel number
     ** @param time      Digi time [ns]
     ** @param index     Index of digi object in its TClonesArray
     ** @return  kTRUE is digi was successfully processed
     **
     ** Depending on the state of the respective channel,
     ** the proper action is taken.
     **/
  Bool_t ProcessDigi(UShort_t channel, Double_t time, Int_t index);


private:
  /** @brief Status buffer
     **
     ** The vector index is the channel number. The first element of the
     ** content is the digi index, the second one the time.
     **/
  std::vector<std::pair<Long64_t, Double_t>> fStatus {1024, {-1, 0.}};  //!

  /** @brief Pointer to output vector **/
  std::vector<CbmStsCluster>* fOutput = nullptr;  //!

  // Required parameters
  UInt_t fAddress                = 0;        ///< Unique module address for clusters
  UShort_t fNofChannels          = 0;        ///< Number of channels
  UShort_t fChannelOffset        = 0;        ///< Number of first channel
  Double_t fTimeCutSig           = 0.;       ///< Time cut in multiples of error
  Double_t fTimeCutAbs           = 0.;       ///< Absolute time cut [ns]
  Bool_t fConnectEdge            = kFALSE;   ///< Connect last and first channel
  const CbmStsParModule* fModPar = nullptr;  //! Module parameters
};

#endif /* CBMSTSALGOFINDCLUSTERS_H */

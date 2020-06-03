#ifndef CBMTRDTESTBEAMTOOLS_H
#define CBMTRDTESTBEAMTOOLS_H

#include "CbmTrdCluster.h"
#include "CbmTrdDigi.h"
#include "CbmTrdAddress.h"
#include "CbmSpadicRawMessage.h"

#include <TClonesArray.h>
#include "CbmDigiManager.h"

#include <vector>
#include <mutex>
#include "TString.h"
#include "TObject.h"
#include "TMath.h"
#include <iostream>


class CbmTrdTestBeamTools: public TObject
{
  protected:
  CbmDigiManager* fDigiMan = nullptr;

  public:
 CbmTrdTestBeamTools();
  // CbmTrdTestBeamTools():CbmTrdTestBeamTools("CbmTrdBeamtimeTools"){};
 ~CbmTrdTestBeamTools(){};


 /**Get the current instance of TestBeam tools in use, or set it.
  * Only permits one instance of TestBeam tools.
  * @param: NewInstance Optional pointer to a new instance of CbmTrdTestBeamtools.
  * If not set, a new instance of the default class is created.
  *
  * @return: A Pointer to the current global instance of CbmTrdTestBeamtools.
  */
  static CbmTrdTestBeamTools* Instance(CbmTrdTestBeamTools*);
  /**
   * GetAddress: Get a CbmTrdAddress-compatible Address for this CbmSpadicRawMessage.
   * @param raw Pointer to the CbmSpadicRawMessage, for which an Address should be returned.
   * @return: A CbmTrdAddress-compatible Address for the active Channel.
   */
  virtual UInt_t GetAddress(CbmSpadicRawMessage* raw);
  /**
   * GetAddress: Get a CbmTrdAddress-compatible Address for this CbmTrdDigi.
   * @param raw Pointer to the CbmTrdDigi, for which an Address should be returned.
   * @return: A CbmTrdAddress-compatible Address for the active Channel.
   */
  UInt_t GetAddress(const CbmTrdDigi* raw){
    std::mutex mtx;
    mtx.lock();
    unsigned int BaseAddress=raw->GetAddress();
    unsigned int PadID=raw->GetAddressChannel();
    Int_t LayerID=CbmTrdAddress::GetLayerId(BaseAddress);
    Int_t ModID=CbmTrdAddress::GetModuleId(BaseAddress);
    Int_t NrCols=GetNrColumns(LayerID);
    Int_t ColID=PadID%NrCols;
    Int_t SectID=CbmTrdAddress::GetSectorId(BaseAddress);
    Int_t RowID=PadID/NrCols;
    //    std::cout << std::hex<< BaseAddress<< " " << PadID<< " " << ColID << " " << RowID<<" BBBBBBB " << CbmTrdAddress::GetAddress(LayerID,ModID,SectID,RowID,ColID)<<std::dec<<std::endl;
    return CbmTrdAddress::GetAddress(LayerID,ModID,SectID,RowID,ColID);
  };
  /**
   * GetModuleID: Get a CbmTrdAddress-compatible ModuleID for this CbmSpadicRawMessage.
   * @param raw Pointer to the CbmSpadicRawMessage, for which an ModuleID should be returned.
   * @return: A CbmTrdAddress-compatible ModuleID for the active Channel.
   */  
  virtual Int_t GetModuleID(CbmSpadicRawMessage* raw);
  /**
   * GetLayerID: Get a CbmTrdAddress-compatible LayerID for this CbmSpadicRawMessage.
   * @param raw Pointer to the CbmSpadicRawMessage, for which a Layer ID should be returned.
   * @return: A CbmTrdAddress-compatible LayerID for the active Channel.
   */
  virtual Int_t GetLayerID(CbmSpadicRawMessage* raw);
  /**
   * GetRowID: Get a CbmTrdAddress-compatible RowID for this CbmSpadicRawMessage.
   * @param raw Pointer to the CbmSpadicRawMessage, for which a RowID should be returned.
   * @return: A CbmTrdAddress-compatible RowID for the active Channel.
   */
  virtual Int_t GetRowID(CbmSpadicRawMessage* raw);
  /**
   * GetRobID: Get DpbID for this CbmSpadicRawMessage.
   * The Dpb is the FPGA to which the Spadic is connected, a ROB might be transparently
   * used inbetween. Name is kept for compatibility.
   * @param raw Pointer to the CbmSpadicRawMessage, for which the DpbOd should be returned.
   * @return: A DpbId for the active Channel.
   */
  virtual Int_t GetRobID(CbmSpadicRawMessage* raw);    //blubb
  /**
  * GetSpadicID: Get DpbID for for this CbmSpadicRawMessage.
  * Each ID addresses a 16 Channel Channelgroup.
  * @param raw Pointer to the CbmSpadicRawMessage, for which a SpadicId should be returned.
  * @return: SpadicId for the active Channel.
  */
  virtual Int_t GetSpadicID(CbmSpadicRawMessage* raw);
  /**
   * GetRowID: Get a CbmTrdAddress-compatible ColumnID for this CbmSpadicRawMessage.
   * @param raw Pointer to the CbmSpadicRawMessage, for which a ColumnID should be returned.
   * @return: A CbmTrdAddress-compatible ColumnID for the active Channel.
   */

  virtual Int_t GetColumnID(CbmSpadicRawMessage* raw);
  //  virtual Int_t GetCombiID(CbmSpadicRawMessage* raw);
  /**
   * Get The mapping of a Spadic channel to the corresponding MWPC-pad in a 32 Channel Group.
   * This Group contains two 16 channel rows. The lower 16 pads correspond to the upper row.
   * @param raw Pointer to the CbmSpadicRawMessage
   * @return Pad on the 32-Channel Group.
   */
  virtual Int_t GetChannelOnPadPlane(CbmSpadicRawMessage* raw);
  /**
   * Get inverse sampling frequency in nano seconds, assumed to be uniform in a given setup.
   * This version gives the Sampling time for the SPADIC 1.x.
   * @return: Sampling time in nano seconds.
   */
  virtual Float_t GetSamplingTime(){
    return 1E3/15;
  };
  /**
   * Get the spadic shaping time tau in nano seconds.
   * This is the shaping constant from the second order shaper, assumed to be uniform in a given setup.
   * This version gives the Sampling time for the SPADIC 1.x.
   * @return: Shaping time in nano seconds.
   */
  virtual Int_t GetShapingTime(){
    return 90;
  };

  virtual Int_t GetNrLayers(){
    /**
     * Get the number of detector layers in a setup.
     * @return: Number of instrumented and active detector layers.
     */
    return GetNrRobs();
  };
  virtual Int_t GetNrRows(Int_t /*Layer*/=0){
    /**
     * Get the Number of active Rows in the corresponding Layer.
     * Note: assumes one chamber type per Layer.
     * @param: Layer Layer ID of the queried detector, usually starts with 0 and counts upwards.
     * @return: Number of active Rows in the corresponding Layer
     */
    return 2;
  };
  double
  GetPadWidth (Int_t layer)
  {
    /**
     * Get the width of a pad in cm, assumed to be constant for a given Layer.
     * Note: currently contains the DESY2017 parameters, this will be fixed within the following days.
     * @return: Width of a pad in cm
     */
    if (layer < 6)
      return 0.7125;
    return 16.0;
  }

  double
  GetPadHeight (Int_t layer)
  {
    /**
     * Get The height of a pad in cm, assumed to be constant for a given Layer.
     * Note: currently contains the DESY2017 parameters, this will be fixed within the following days.
     * @param: Layer Layer ID of the queried detector, usually starts with 0 and counts upwards.
     * @return: Height of a pad in cm
     */
    if (layer < 4)
      return 15.25;
    if (layer < 6)
      return 4.5; //????
    return 23.0;
  }

  virtual Int_t GetNrColumns(Int_t /*Layer*/=0){
    /**
     * Get the Number of active pad columns in the corresponding Layer, assumed to be constant for a given Layer.
     * @param: Layer Layer ID of the queried detector, usually starts with 0 and counts upwards.
     * @return: Number of active pad columns
     */
    return 16*GetNrSpadics();
  }
  virtual Int_t GetNrRobs(){
    /**
     *  Get the Number of active DPBs in the Setup.
     *  Note: This function will be deprecated and replaced with a properly named function.
     *  @return: the Number of active DPBs in the Setup
     */
    return 4;
  };
  virtual Int_t GetNrSpadics(Int_t /*DpbID*/=0){
    /*
     * Get the Number of active Spadics on a ROB
     * @param: DpbID ID of the queried DPB, usually starts with 0 and counts upwards.
     * @return: Number of active spadics connected to DPB
     */
    return 3;
  }
  virtual Int_t GetSpadicBaseAddress(){
    /**
     * Base address which will be added to each half spadic number.
     * The first halfspadic which is connected to a DPB
     * get SpadicBaseAddress+0, the second SpadicBaseAddress +1 ...
     * @return: Base address.
     */
    return 0;
  }
  /**
   * GetSpadicResponse: Calculate detector response of the Spadic for given Parameters.
   * The response is modeled by calculating two delta Pulses and overlaying them.
   *
   * @return The return value is a std::vector of 32 Samples for the given Parameters.
   * @param Max Maximum ADC-value of the Signal. This is not Baseline corrected.
   * @param Base Baseline value for the calculation of the response
   * @param FirstTime Timing information for the first hit. The Value is the interpolated index where the signal crosses the half Maximum point.
   *        It is given in units of the sampling time.
   * @param FWHM Timing information for the second hit. This is the interpolated index where the signal crosses the half Maximum point on the downward slope.
   */
  std::vector<Float_t> GetSpadicResponse(Float_t /*Max*/,Float_t /*Base*/, Float_t /*FirstHalfMax*/, Float_t /*SecondHalfMax*/){
    return std::vector<Float_t>({0,1});
  }

  /**
   * GetDeltaResponse: Get the spadic Response for a Delta pulse occurring at Index 0.
   * The measured pulses have this occurring at Index ~2, so adjust appropriately.
   *
   * @param Index This is a fractional index for the Sample occurring at this point relative to the original Delta pulse.
   *        Index 0.0 might be offset relative to the sample index in a real Spadic Response.
   */
  Float_t GetDeltaResponse(Float_t Index){
    static std::vector<Float_t> ResponseArray;
    if(ResponseArray.empty()){
        Float_t Sample=GetSamplingTime();
        Float_t Shape=GetShapingTime();
        for (Int_t i=0;i<320;i++){
            ResponseArray.push_back((TMath::Exp(-(i/10.0)*Sample/Shape)*((i/10.0)*Sample)/(Shape*Shape)));
        }
    }
    Int_t intIndex=static_cast<Int_t>(round(Index*10));
    return ResponseArray.at(intIndex);
  }
  /**
   * FindIntercept: internal function to find a linearly interpolated point between two samples.
   *
   * @param FirstSample The first sample, this is used as the baseline.
   * @param SecondSample The second sample, this is used as the upper point of the linear interpolation.
   * @param Mark The value for which the crossover point is looked for.
   * @return The Crossover point in units of the distance between FirstSample and SecondSample.
   */

  Float_t FindIntercept(Float_t FirstSample, Float_t SecondSample, Float_t Mark){
    SecondSample-=FirstSample;
    Mark-=FirstSample;
    return (Mark)/SecondSample;

  }
  /**
   * Internal function to lookup  the base EquipmentID.
   * This is version returns the value for the legacy DAQ-
   * @return The Base EquipmentID for the Setup.
   */
  virtual Int_t GetBaseEquipmentID(){
    //Get the BaseEquipmentID
    return 0xE000;
  }
  /**
   * Internal Function to query the mapping between the channels of a
   * full SPADIC (2 channel groups) and the pads on the padplane.
   * The return value will be valid for the SPADIC on which the message originated.
   * @param raw Pointer to the CbmSpadicRawMessage, for which the mapping to the padplane should be returned.
   * @return A std::vector containing the pad number at the index equal to the SPADIC channel.
   * */
  virtual std::vector<Int_t> GetChannelMap(CbmSpadicRawMessage* raw);
  /**
   * An enum Type indicating the desired addressed Unit in GetSpadicName.
   */
  enum kSpadicSize : bool{
    kFullSpadic=true,/**< Address a FullSpadic with two channel groups, sets Spadic name to "Spadic".*/
    kHalfSpadic=false/**< Address a HalfSpadic with one channel group, sets Spadic name to "Half_Spadic".*/
  };
  /** GetSpadicName: Get a consistent String of the Form "AFCK_0_Spadic_0" describing the specific SPADIC corresponding to the input parameters.
   * Useful for adressing Histograms in the histogram manager.
   *  @param DpbID ID of the Dpb to which the Spadic is connected.
   *  @param SpadicID Logical ID of the Spadic as assigned by the DPB.
   *  @param DpbName Base name of the Dpb type, can be an arbitrary string.
   *  @param SpadicSize Desired Spadic Size, might be either a kFullSpadic or a kHalfSpadic.
   *  @return A formatted string of the form AFCK_0_Spadic_0.
   * */
  TString GetSpadicName(Int_t DpbID,Int_t SpadicID,TString DpbName,kSpadicSize SpadicSize);
  /** Compatibility wrapper for GetSpadicName. Deprecated
   *  @param DpbID ID of the Dpb to which the Spadic is connected.
   *  @param SpadicID Logical ID of the Spadic as assigned by the DPB.
   *  @param DpbName Base name of the Dpb type, can be an arbitrary string.
   *  @param FullSpadic Desired Spadic Size, might be either a kFullSpadic or a kHalfSpadic.
   *  @return A formatted string of the form AFCK_0_Spadic_0.
   */
  TString GetSpadicName(Int_t DpbID,Int_t SpadicID,TString DpbName,Bool_t FullSpadic){
    kSpadicSize Size=kHalfSpadic;
    if(FullSpadic)
      Size=kFullSpadic;
    return GetSpadicName(DpbID,SpadicID,DpbName,Size);
  }
  /**
   * For a given Message, return an estimate for the baseline in a consistent message.
   * This function only considers the individual message. Averages have to be calculated by the user.
   * @param raw Pointer to the RawMessage for which the baseline is to be estimated.
   * @return Value of Sample 0 from the message.
   */
  Int_t GetBaseline(CbmSpadicRawMessage* raw);
  /**
   * Gives a Charge estimate by taking a raw message and subtracting the Baseline from the maximum ADC Value.
   * @param raw Pointer to the RawMessage for which the charge is to be estimated.
   * @param Base User provided estimate of the Baseline.
   * @return Value of the maximum ADC Sample after subtraction of the Baseline
   */
  virtual Int_t GetMaximumAdc(CbmSpadicRawMessage*raw,Double_t Base=0.0);
  /**
   * Gives a Charge estimate by taking a raw message and returns the corrected Integral.
   * This integral is corrected in the sense that different message lengths are compensated for.
   * The output is scaled to be close to the dynamic range of the MaxADC value.
   *
   * This function is a wrapper.
   * @param raw Pointer to the Rawessage for which the charge is to be estimated.
   * @param Base User provided estimate of the Baseline.
   * @return Corrected and scaled integral charge for the message.
   */
  Float_t GetIntegratedCharge(CbmSpadicRawMessage*raw,Double_t Base=0.0);
  /**
     * Gives a Charge estimate by taking an array of zero-suppressed samples andreturns the corrected Integral.
     * This integral is corrected in the sense that different message lengths are compensated for.
     * The output is scaled to be close to the dynamic range of the MaxADC value.
     *
     * @param Samples Pointer to an array of zero-suppressed samples for which the charge is to be estimated.
     * @param NrSamples Number of valid Samples in the array.
     * @return Corrected and scaled integral charge for the message.
     */
  Float_t GetIntegratedCharge(const Float_t* Samples,Int_t NrSamples=32);

  CbmTrdTestBeamTools(const CbmTrdTestBeamTools&);
  CbmTrdTestBeamTools operator=(const CbmTrdTestBeamTools&);

  /*
   * Now define all necessary functions for the clusters.
   */
  /**
   * Set the CbmTrdDigi pointer in use. This is necessary for cluster analysis.
   * @param Digis Pointer to the TClonesArray containing all CbmTrdDigis in the Clusters to be analyzed.
   */
  void
  SetDigisArray (TClonesArray*)
  {
    fDigiMan = CbmDigiManager::Instance();
    fDigiMan->Init();
  }
  ;
  void ClearNullptr(CbmTrdCluster*test){
    auto data=test->GetDigis();
    for (auto x=data.begin();x!=data.end();x++)
      {
    const CbmTrdDigi* ptr = fDigiMan->Get<CbmTrdDigi>(*x);
    if (ptr==nullptr)
	  data.erase(x),x=data.begin();
      }
    test->SetDigis(data);
  };
  /**
   * GetLayerID: Get a CbmTrdAddress-compatible ModuleID for this CbmTrdCluster.
   * @param Clust Pointer to the CbmTrdCluster, for which a Module ID should be returned.
   * @return: A CbmTrdAddress-compatible ModuleID for the Cluster.
   */
  virtual Int_t
  GetModuleID (CbmTrdCluster*);
  /**
   * GetLayerID: Get a CbmTrdAddress-compatible LayerID for this CbmTrdCluster.
   * @param Clust Pointer to the CbmTrdCluster, for which a Layer ID should be returned.
   * @return: A CbmTrdAddress-compatible LayerID for the Cluster.
   */
  virtual Int_t
  GetLayerID (CbmTrdCluster*Clust);
  /**
   * GetLayerID: Get a CbmTrdAddress-compatible SectorID for this CbmTrdCluster.
   * @param Clust Pointer to the CbmTrdCluster, for which a Sector ID should be returned.
   * @return: A CbmTrdAddress-compatible SectorID for the Cluster.
   */

  virtual Int_t
  GetSectorID (CbmTrdCluster* clust)
  {
    //returns sectorID of the Cluster. Only defined for compatibility.
    const CbmTrdDigi* Digi = fDigiMan->Get<CbmTrdDigi>(clust->GetDigi(0));
    return CbmTrdAddress::GetSectorId (GetAddress(Digi));
  }
  ;
  /**
   * Get a CbmTrdAddress-compatible RowID for the central row of this CbmTrdCluster.
   * This is the row on which most self triggered messages are located.
   * @param Clust Pointer to the CbmTrdCluster, for which a central Row ID should be returned.
   * @return: A CbmTrdAddress-compatible RoID for the Cluster.
   */
  virtual Int_t
  GetCentralRowID (CbmTrdCluster*Clust);
  /**
   * Get a CbmTrdAddress-compatible ColumnID for the central Column of this CbmTrdCluster.
   * This is the average column with regard to the Self-triggered messages.
   * This is intended as the base position for the displacement algorithms.
   * @param Clust Pointer to the CbmTrdCluster, for which a central Column ID should be returned.
   * @return: A CbmTrdAddress-compatible ColumnID for the Cluster.
   */
  virtual Int_t
  GetCentralColumnID (CbmTrdCluster*Clust);
  /**
   * Query the width of the cluster in terms of the amount of rows it spans.
   * @param Clust Pointer to the CbmTrdCluster
   * @return The number of consecutive rows the Cluster spans.
   * */
  virtual Int_t
  GetRowWidth (CbmTrdCluster*Clust);
  /**
   * Query the width of the cluster along its main row in terms of pads.
   * @param Clust Pointer to the CbmTrdCluste
   * @return The number of consecutive pads along the main row of the Cluster.
   * */
  virtual Int_t
  GetColumnWidth (CbmTrdCluster*Clust);
  /**
   * Calculate the total charge of the cluster, this is done by summing up the
   * charges of the individual Digis comprising this cluster.
   * @param Clust Pointer to the CbmTrdCluster to be investigated
   * @return Total Charge on all pads of the cluster.
   */
  Float_t
  GetCharge (CbmTrdCluster*Clust);
  /**
   * Get the offset between the center of a clusters main row and the center of its charge deposition
   * TODO: currently no implementation exists for the Frankfurt/Muenster Prototypes.
   * @param Clust Pointer to the CbmTrdCluster to be investigated
   * @return Distance of a Clusters center of gravity and its main row, in units of pad height.
   * */
  virtual Float_t
  GetRowDisplacement (CbmTrdCluster*Clust);
  /**
   * Get the offset between the center of a clusters central pad and the center of its charge deposition
   * @param Clust Pointer to the CbmTrdCluster to be investigated
   * @return Distance of a Clusters center of gravity and its central pad, in units of pad width.
   * */

  virtual Float_t
  GetColumnDisplacement (CbmTrdCluster*Clust);
  /**
   * An enum type describing the classification of a given CbmTrdCluster.
   */
  enum CbmTrdClusterClassification
    : char
      { kNormal = 0,	/**< A normal well formed Cluster.*/
    kMissingSTR,	/**< A malformed Cluster without STR triggered digi, this is usually due to a lost hit message.*/
    kMissingFNR,	/**< A malformed Cluster with at least one missing FNR triggered digi, this is usually due to a lost hit message.*/
    kInvalidCharge,	/**< A Cluster with at least one negative charge value. A high number of these would indicate a problem with the baseline estimation.*/
    kFragmented,	/**< Reserved for future use: A Cluster that was created due to a noise induced retrigger.*/
    kEmpty		/**< A Cluster without Digis.*/
  };
  /**
   * Classify a cluster according to general principals for Clusters.
   * Important: This function does not perform any checks based on physics.
   * @param: Clust The Cluster to be classified.
   * @return: A first and very general classification of the cluster.
   */
  CbmTrdClusterClassification
  ClassifyCluster (CbmTrdCluster*);
  /**
   * Query the Classification name as a string.
   * @param _Class The CbmTrdClusterClassification to be queried.
   * @return A TString containing the proper name of the Classification.
   */
    TString
    GetClassName (CbmTrdClusterClassification _Class)
    {
      switch (_Class)
        {
        case kNormal:
          return "kNormal";
        case kMissingSTR:
          return "kMissingSTR";
        case kMissingFNR:
          return "kMissingFNR";
        case kInvalidCharge:
          return "kInvalidCharge";
        case kFragmented:
          return "kFragmented";
        case kEmpty:
          return "kFragmented";
        }
  }

protected:
    /**
     * A precise position reconstruction algorithm for Clusters.
     * To be used on consecutive groups of three pads.
     * Not in use at the moment due to the necessary filtering.
     * @params FirstCharge Charge on the first pad of the partial Cluster, must be larger than 0.0.
     * @params SecondCharge Charge on the central pad of the partial Cluster, must be larger than 0.0.
     * @params ThirdCharge Charge on the third pad of the partial Cluster, must be larger than 0.0.
     * @return An offset to the center of the central pad. In fractions of the padwidth.
     */
  Float_t
  CalculateSECHS (Float_t FirstCharge, Float_t SecondCharge, Float_t ThirdCharge);
 public:
  
  ClassDef(CbmTrdTestBeamTools,1);
};

#endif

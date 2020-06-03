/*First Version of TestBeam tools, developed for the SPS2016 beamtime and the legacy DAQ-chain.
 *Intended for the F/MS Prototypes.
 *For usage with Bucharest, modifications of the functions GetChannelMap and GetRowID are neccessary.
*/

#include "CbmTrdTestBeamTools.h"
#include <FairLogger.h>
#include <algorithm>
#include <iostream>


ClassImp(CbmTrdTestBeamTools)

CbmTrdTestBeamTools::CbmTrdTestBeamTools () : TObject()
{
  LOG(info) << TString("Default Constructor of ")+TString(this->GetName());
  Instance(this);
}

CbmTrdTestBeamTools* CbmTrdTestBeamTools::Instance(CbmTrdTestBeamTools* NewInstance=nullptr){
  static CbmTrdTestBeamTools* CurrentInstance=nullptr;
  if (!NewInstance&&!CurrentInstance)
      CurrentInstance = new CbmTrdTestBeamTools();
  if (!NewInstance)
    return CurrentInstance;
  if (!CurrentInstance)
    CurrentInstance=NewInstance;
  if (NewInstance!=CurrentInstance)
    LOG(fatal)<<"Preexisting Instance of "<< CurrentInstance->GetName();
  return CurrentInstance;
}

UInt_t CbmTrdTestBeamTools::GetAddress(CbmSpadicRawMessage* raw){
  /*Get a unique address for the pad of the Message.
   *Last 3 decimal digits comprise of the ColumnID. Range:0-255;
   *Next 2 decimal digits display the Row inside the module. Range: 0-10;
   *Next 2 decimal digits are the moduleID in the respective Layer. Range:0-49;
   *Digit %1E5 gives the Layer. Range:0-3;
   */
  Int_t ColID=GetColumnID(raw);
  Int_t SectID=0;//GetSectorID(raw);
  Int_t RowID=GetRowID(raw);
  Int_t ModID=GetModuleID(raw);
  Int_t LayerID=GetLayerID(raw);
  return CbmTrdAddress::GetAddress(LayerID,ModID,SectID,RowID,ColID);
}

Int_t CbmTrdTestBeamTools::GetModuleID(CbmSpadicRawMessage* raw)
{
  /*
   * Get the numeric module ID of origin for the given message.
   * The ModuleID is unique within a layer.
   * SPS2016 Case: identical to RobID and LayerID
   */
  return GetRobID(raw);
}

// ----              -------------------------------------------------------
Int_t CbmTrdTestBeamTools::GetSpadicID(CbmSpadicRawMessage* raw)
{
  /*
   * Get the numerical SpadicID of origin for the given Message.
   * The SpadicID is unique on the Rob.
   */
  //TString spadic="";
  Int_t sourceA = raw->GetSourceAddress();
  Int_t SpaId = sourceA-GetSpadicBaseAddress();
  if (SpaId<0||SpaId>5){
    LOG(error) << "Source Address " << sourceA << " not known.";
    SpaId = -1;
  }
  return SpaId;
}

Int_t CbmTrdTestBeamTools::GetRobID(CbmSpadicRawMessage* raw)
{
  /*
   * Get the numerical RobId of origin for the given Message.
   * SPS2016 Case: Numerical id of the syscore on which the spadic was connected.
   */
  Int_t eqID=raw->GetEquipmentID();
  Int_t SyscoreID=eqID-GetBaseEquipmentID();
  if((SyscoreID<0||SyscoreID>GetNrRobs())){
    LOG(error) << "EqID " << eqID << " not known.";
    SyscoreID=-1;
  }
  return SyscoreID;
}

Int_t CbmTrdTestBeamTools::GetLayerID(CbmSpadicRawMessage* raw)
{
  /*
   * Get the layer from whic the given message originated. Defined in downstream direction.
   * Layer 0 is closest to the target.
   */
  return GetModuleID(raw);
}

Int_t CbmTrdTestBeamTools::GetRowID(CbmSpadicRawMessage* raw){
  /*Get row of the active channel.
   *Upper row is row 1.
   */
  return 1-GetChannelOnPadPlane(raw)/16;
}

Int_t CbmTrdTestBeamTools::GetColumnID(CbmSpadicRawMessage* raw){
  /*Get column of the active channel.
   *With view towards the target, column 0 is on the left side.
   *Lower numbered colums are towards the left side of the module.
   */
  Int_t ColID=(GetSpadicID(raw)/2)*16+(GetChannelOnPadPlane(raw))%16;
  return ColID;
}

TString CbmTrdTestBeamTools::GetSpadicName(Int_t DpbID,Int_t SpadicID,TString DpbName="SysCore",kSpadicSize SpadicSize=kFullSpadic)
{
	/*	Get a String of the Form "Syscore_0_Spadic_0" describing the specific SPADIC corresponding to the input parameters.
	 *  The Parameter InputType allows either the Equipment ID/Source Address or the final Syscore/Spadic ID to be used.
	 *  	kRawData (default) is the parameter that allows the raw EqID/Source Address to be used, kProcessedData takes Syscore/SpadicID.
	 *  The Parameter OutputType allows adressing either the corresponding FullSpadic, via kFullSpadic (default), or the original HalfSpadic,
	 *  	via kHalfSpadic.
	 * */
  TString spadicName="";
  spadicName=DpbName+"_"+std::to_string(DpbID)+"_";
  if(SpadicSize){
	  spadicName += "Spadic_";
  }else{
	  spadicName += "Half_Spadic_";

  }
  spadicName+=std::to_string(SpadicID);

  return spadicName;
}

//  virtual Int_t GetCombiID(CbmSpadicRawMessage* raw);
Int_t CbmTrdTestBeamTools::GetChannelOnPadPlane(CbmSpadicRawMessage* raw){
  /*
   * The Spadic channels have an indirection layer to the pad on the padplane.
   * This takes care of the mapping and returns the pad number.
   */
  std::vector<Int_t>Map=GetChannelMap(raw);
  Int_t chID=(GetSpadicID(raw)%2)*16+raw->GetChannelID();
  return Map.at(chID);
}

std::vector<Int_t> CbmTrdTestBeamTools::GetChannelMap(CbmSpadicRawMessage*){
  /*
   * Get the channel mapping that is valid for the spadic where raw originated.
   */
  std::vector<Int_t> map = {{31,15,30,14,29,13,28,12,27,11,26,10,25, 9,24, 8,
			     23, 7,22, 6,21, 5,20, 4,19, 3,18, 2,17, 1,16, 0}};
  return map;
}

Int_t CbmTrdTestBeamTools::GetBaseline(CbmSpadicRawMessage* raw)
{
  /*
   * Estimate the signal baseline. Returns the lowest ADC value in the first two samples.
   */
  Int_t Base =*std::min_element(raw->GetSamples(),raw->GetSamples());
  //Int_t Basecand=*std::min_element(raw->GetSamples()+raw->GetNrSamples()-3,raw->GetSamples()+raw->GetNrSamples());
  return Base;//(Base<Basecand)?Base:Basecand;
}

Int_t CbmTrdTestBeamTools::GetMaximumAdc(CbmSpadicRawMessage* raw,Double_t Base)
{
  /*
   * Calculate message charge using the Maximum ADC method.
   * This is zero suppressed data, if a baseline is given.
   */
  Int_t MaxADC=-255;
  for (int i=0;i<raw->GetNrSamples();i++){
    if (raw->GetSamples()[i]>MaxADC)
      MaxADC=raw->GetSamples()[i];
  }
  return MaxADC-Base;
}

Float_t CbmTrdTestBeamTools::GetIntegratedCharge(CbmSpadicRawMessage* raw,Double_t Base)
{
  /*
   * Get the integrated charge of raw, with Base subtracted.
   * Includes correction for unequal message lengths.
   */

  Float_t* ProcessedSamples=new Float_t[32];
  Int_t* Samples=raw->GetSamples();
  Int_t NrSamples=raw->GetNrSamples();
  Int_t Baseline=GetBaseline(raw);
  if(Base!=0.0)
    Baseline = Base;
  for (int i=0;i<NrSamples;i++){
      ProcessedSamples[i]=Samples[i]-Baseline;
  //    std::cout << ProcessedSamples[i] << " ";
  }
  //std::cout << std::endl;
  Double_t Integral=GetIntegratedCharge(ProcessedSamples,NrSamples);
  delete[] ProcessedSamples;
  return Integral/1.E4;
}

Float_t CbmTrdTestBeamTools::GetIntegratedCharge(const Float_t* Samples,Int_t NrSamples)
{
  /*
   * Get the integrated charge of raw, with Base subtracted.
   * Includes correction for unequal message lengths.
   */
  Double_t Integral=0;
  if(NrSamples<=3)
    return 0;
  for (int i=2;i<NrSamples;i++){
    Integral+=Samples[i];
  }
  Double_t CorrectionFactor=0;
  Double_t Shape=GetShapingTime(),Sample=GetSamplingTime();
  for (Int_t i=0;i<NrSamples-2;i++)
  {
      //Calculate integral of the impulse response of the spadic.
      //std::cout << (TMath::Exp(-(i+0.25)*Sample/Shape)*((NrSamples+0.25)*Sample*Sample)/(Shape*Shape))<< std::endl;
      CorrectionFactor+=(TMath::Exp(-(i)*Sample/Shape)*((i)*Sample)/(Shape*Shape));
  }
  //std::cout << "ClusterCharge: "<<Integral << " " << CorrectionFactor << " "<< NrSamples << " " << Integral*1.5/(CorrectionFactor*1E3)<<std::endl;
  return static_cast<Float_t>(Integral*1.5/(CorrectionFactor*1E3));
}

Int_t
CbmTrdTestBeamTools::GetModuleID (CbmTrdCluster* Clust)
{
  //Returns ModuleID of the first Digi in the cluster.
  //This is identical to the ModuleID of the cluster.
  ClearNullptr(Clust);
  const CbmTrdDigi*digi = fDigiMan->Get<CbmTrdDigi>(
      *Clust->GetDigis ().begin ());
  return CbmTrdAddress::GetModuleId (GetAddress(digi));
}

// ----              -------------------------------------------------------

Int_t
CbmTrdTestBeamTools::GetLayerID (CbmTrdCluster* Clust)
{
  //Returns LayerID of the first Digi in the cluster.
  //This is identical to the LayerID of the cluster.
  ClearNullptr(Clust);
  const CbmTrdDigi*digi = fDigiMan->Get<CbmTrdDigi>(
      *(Clust->GetDigis ().begin ()));
  return CbmTrdAddress::GetLayerId (GetAddress(digi));
}

Int_t
CbmTrdTestBeamTools::GetCentralRowID (CbmTrdCluster* Clust)
{
  /*Get main row of the cluster.
   * Only defined for 1 Row Clusters at this time.
   * The Definition is identical to TestBeamTools::GetRowID.
   */
  //TODO: Implement for 2D-Clusters.
  /*if (Clust->GetNRows () != 1)
    //TODO: Find self triggered Digi.
    return -1;*/
    ClearNullptr(Clust);
    const CbmTrdDigi*digi = fDigiMan->Get<CbmTrdDigi> (
      *(Clust->GetDigis ().begin ()));
  return CbmTrdAddress::GetRowId (GetAddress(digi));
}

Int_t
CbmTrdTestBeamTools::GetCentralColumnID (CbmTrdCluster* Clust)
{
  /*Get the main column of the cluster.
   *For clusters of odd size, this is the central column.
   *For clusters of even size, this is the column to the left of the clusters center.
   */
  ClearNullptr(Clust);
  std::vector<Int_t> Digis (Clust->GetDigis ());
  std::vector<Int_t> columns;
  //  std::vector<CbmTrdDigi*> selfTriggeredDigis;
  for (auto it = Digis.begin (); it != Digis.end (); it++)
    {
    const CbmTrdDigi*digi = fDigiMan->Get<CbmTrdDigi>(*it);
      if (digi->GetTriggerType () == 1 || digi->GetTriggerType () == 3)
        columns.push_back (CbmTrdAddress::GetColumnId (GetAddress(digi)));
    }
  if (!columns.size ())
    for (auto it = Digis.begin (); it != Digis.end (); it++)
      {
      const CbmTrdDigi*digi = fDigiMan->Get<CbmTrdDigi>(*it);
        columns.push_back (CbmTrdAddress::GetColumnId (GetAddress(digi)));
      }
  Float_t col = 0;
  for (auto it = columns.begin (); it != columns.end (); it++)
    col += *it;
  if (columns.size ())
    col /= columns.size ();
  return static_cast<Int_t> ((col));
}

Int_t
CbmTrdTestBeamTools::GetRowWidth (CbmTrdCluster* Clust)
{
  /*
   * Return width of the Cluster in terms of rows.
   */
  ClearNullptr(Clust);
  std::vector<Int_t> Rows, Digis = Clust->GetDigis ();
  for (auto it = Digis.begin (); it != Digis.end (); it++)
    {
    const CbmTrdDigi*Digi = fDigiMan->Get<CbmTrdDigi>(*it);
      Int_t Row = CbmTrdAddress::GetColumnId (GetAddress(Digi));
      Rows.push_back (Row);
    }
  Int_t MinRow = *std::min_element (Rows.begin (), Rows.end ());
  Int_t MaxRow = *std::max_element (Rows.begin (), Rows.end ());
  return MaxRow - MinRow + 1;
}

Int_t
CbmTrdTestBeamTools::GetColumnWidth (CbmTrdCluster* Clust)
{
  /*
   * Return width of the Cluster in terms of Columns.
   */
  //TODO: Implement this for 2D Clusters
    ClearNullptr(Clust);
  std::vector<Int_t> Columns;
  for (auto it = Clust->GetDigis ().begin (); it != Clust->GetDigis ().end ();
      it++)
    {
    const CbmTrdDigi*Digi = fDigiMan->Get<CbmTrdDigi>(*it);
      Int_t Col = CbmTrdAddress::GetColumnId (GetAddress (Digi));
      Columns.push_back (Col);
    }
  Int_t MinCol = *std::min_element (Columns.begin (), Columns.end ());
  Int_t MaxCol = *std::max_element (Columns.begin (), Columns.end ());
  return MaxCol - MinCol + 1;
}

Float_t
CbmTrdTestBeamTools::GetCharge (CbmTrdCluster* Clust)
{
  ClearNullptr(Clust);
  const std::vector<Int_t> Digis = Clust->GetDigis ();
  Float_t Charge = 0.0;
  for (auto it = Digis.begin (); it != Digis.end (); it++)
    {
    const CbmTrdDigi*digi = fDigiMan->Get<CbmTrdDigi>(*it);
      Charge += digi->GetCharge () + digi->GetChargeTR ();
    }
  return Charge/1.E4;
}

Float_t
CbmTrdTestBeamTools::GetRowDisplacement (CbmTrdCluster* Clust)
{  ClearNullptr(Clust);
  //TODO: To be implemented for 2 Row Clusters or Bucharest Data.
  return 0;
}

Float_t
CbmTrdTestBeamTools::GetColumnDisplacement (CbmTrdCluster* Clust)
{
  ClearNullptr(Clust);
  /**Calculate Displacement according to charge Distribution along main row.
   */
  std::vector<Int_t> Digis (Clust->GetDigis ());
  //Define lamda function for sorting the Digis.
  //TODO: Update Sort function for new Digis
  /*  auto sortDigis =
      [&,this](Int_t a,Int_t b)
	{
	  return static_cast<Bool_t>((static_cast<CbmTrdDigi*>(this->fDigis->At(a))->Compare(this->fDigis->At(b)))+1);
	  };
  */
  //std::sort (Digis.begin (), Digis.end (), sortDigis);
  //For now only consider the main row for Displacement.
  Int_t mainRow = GetCentralRowID (Clust);
  //Discard all Digis not on main row
  //TODO: Refactor this.
  for (auto it = Digis.begin (); it != Digis.end ();)
    {
    const CbmTrdDigi*digi = fDigiMan->Get<CbmTrdDigi>(*it);
      if (CbmTrdAddress::GetRowId (GetAddress(digi)) != UInt_t(mainRow))
	{
	  Digis.erase (it);
	  it = Digis.begin ();
	}
      else
	{
	  it++;
	}
    }
  //Cannot calculate an offset for Clusters of size =1
  if (Digis.size()<=1)
    return 0.;
  //Define Variables for  Center of Gravity
  //Should be moved inside corresponding code block
  Float_t WeightedColumns = 0;
  Float_t TotalRowCharge = 0;
  //Define Array for SECHS
  //Should be moved inside corresponding code block
  std::vector<Float_t> Charges {{}};
  //Find Reference point for Center of Gravity
  //Should be moved inside corresponding code block
  Float_t Offset = GetCentralColumnID(Clust);
  //Define Return value variable.
  Float_t Displacement = 0;
  //For all Clusters not suitable for SECHS, use COG
  //Also for all Cluster of Size > 4
  //SECHS gives weird results on large Clusters.
  if (ClassifyCluster (Clust) != CbmTrdClusterClassification::kNormal
      ||GetColumnWidth(Clust)>4)
    {
      //Calculate Center of Gravity via the average pad number
      //Might need to be reworked
      for (UInt_t i = 0; i < Digis.size (); i++)
	{
    const CbmTrdDigi*digi = fDigiMan->Get<CbmTrdDigi>(i);
	  Float_t Charge = (digi->GetCharge () + digi->GetChargeTR ())/1.e4;
	  TotalRowCharge += Charge;
	  WeightedColumns += (Charge
	      * CbmTrdAddress::GetColumnId (GetAddress(digi)));
	}
      //Should be 1, prevents Divide by 0.
      if (TotalRowCharge <= 0)
    	TotalRowCharge = 1;
      //Calculate Average
      Displacement  = WeightedColumns / TotalRowCharge;
      //subtract Reference
      Displacement -= Offset;
    }
  //
  else
    {
      for (auto p : Digis)
	{
    const CbmTrdDigi*digi = fDigiMan->Get<CbmTrdDigi>(p);
	  if (digi==nullptr)
	    continue;
	  Charges.push_back (digi->GetCharge ()/1.e4);
	}
      for (size_t i = 0; i+2 < Charges.size (); i++)
	{
	  Offset=i;
	  //std::cout << i << " "<< i+1 << " "<< i+2 << " "<<Charges.size ()<< std::endl;
	  Double_t TempOffset = CalculateSECHS (Charges[i],
						Charges[i + 1],
						Charges[i + 2]);
	  if(GetColumnWidth(Clust)==3)
	    TempOffset+=0;
	  else
	    TempOffset+=0.5;
	  Offset += TempOffset;
	}
      Displacement=Offset/(GetColumnWidth(Clust)-2.);
    }
  return Displacement;
}

Float_t
CbmTrdTestBeamTools::CalculateSECHS (Float_t a, Float_t b, Float_t c)
{
  /*
   * Calculate the displacement according to the SECHS (Squared hyberbolic secans) method in units of the padwidth.
   *
   * Needs three adjacent positive and finite charges.
   * a should be the leftmost charge.
   * b needs to be the central charge.
   */
  Double_t PadWidth = 1;
  Double_t a3 =
      TMath::Pi () * PadWidth
          / TMath::ACosH (
              0.5
                  * (sqrt (
                      static_cast<Double_t> (b) / static_cast<Double_t> (a))
                      + sqrt (
                          static_cast<Double_t> (b) / static_cast<Double_t> (c))));
  Double_t Offset = a3 / TMath::Pi ()
      * TMath::ATanH (
          (sqrt (static_cast<Double_t> (b) / static_cast<Double_t> (a))
              - sqrt (static_cast<Double_t> (b) / static_cast<Double_t> (c)))
              / (2.0 * TMath::SinH (TMath::Pi () * PadWidth / a3)));
  return Offset;
}

CbmTrdTestBeamTools::CbmTrdClusterClassification
CbmTrdTestBeamTools::ClassifyCluster (CbmTrdCluster*Clust)
{  ClearNullptr(Clust);
  std::vector<Int_t> Digis (Clust->GetDigis ());
  if (Digis.size () == 0)
    return kEmpty;
  //TODO: Fragmentationtest
  //std::cout << "Cluster debug " <<GetCharge (Clust) << " " <<std::endl;
  /*  if (GetCharge (Clust) <= 0)
      return kInvalidCharge;*/
  /* VF: sortDigis not used.
  auto sortDigis = [&,this](Int_t a,Int_t b)
    {
      //std::cout << a<<" "<< b<< std::endl;
      return static_cast<Bool_t>(-1==(static_cast<CbmTrdDigi*>(this->fDigis->At(a))->Compare(this->fDigis->At(b))));
    };
    */
  //std::sort (Digis.begin (), Digis.end (), sortDigis);
  //Check Triggerpatterns
  std::vector<Int_t> TriggerTypes, Columns;
  //Int_t MainRow = GetCentralRowID (Clust);
  for (auto it = Digis.begin (); it != Digis.end (); it++)
    {
      const CbmTrdDigi*digi = fDigiMan->Get<CbmTrdDigi>(*it);
      /*if (CbmTrdAddress::GetRowId (digi->GetAddress ()) != MainRow)
        continue;*/
      TriggerTypes.push_back (digi->GetTriggerType ());
      //std::cout << digi->GetTriggerType()<< " ";
      Columns.push_back (CbmTrdAddress::GetColumnId (GetAddress(digi)));
      if (digi->GetCharge()<=0.)
	return kInvalidCharge;
    }
  //std::cout << std::endl;
  //Check for expected Neighbour Trigger Pattern.
  //Only default FNR Pattern implemented
  /*if (std::find (TriggerTypes.begin (), TriggerTypes.end (), 2)
      != TriggerTypes.begin ()
      || std::find (TriggerTypes.rbegin (), TriggerTypes.rend (), 2)
          != TriggerTypes.rbegin ())
  */
  if(TriggerTypes[0]!=2||TriggerTypes.back()!=2)
    return kMissingFNR;
  if(Digis.size()<3)
    return kMissingSTR;
  if (TriggerTypes[1]!=1&&TriggerTypes[1]!=3)
    return kMissingSTR;
  //Check for Gaps
  /*
  Int_t CurrentCol = Columns.at (0);
  for (Int_t i = 1; i < Columns.size (); i++)
    {
      if (++CurrentCol < Columns.at (i))
        return kMissingSTR;
    }
    */
  return kNormal;
}



#include "CbmTrdSimpleClusterizer.h"
#include "CbmTrdDigi.h"
#include "CbmTrdCluster.h"
#include "CbmTrdAddress.h"
#include <mutex>
#include <iterator>

CbmTrdSimpleClusterizer::CbmTrdSimpleClusterizer ()
  :  CbmTrdQABase ("CbmTrdSimpleClusterizer")
  , fClusters (nullptr)
  , fProxyArray()
{
}

InitStatus
CbmTrdSimpleClusterizer::Init ()
{
  LOG(debug) << "Initilization of " << this->GetName ();

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance ();

  // Get a pointer to the previous already existing data level
  fRaw = static_cast<TClonesArray*> (ioman->GetObject ("SpadicRawMessage"));
  if (!fRaw)
    {
      LOG(fatal) << "No InputDataLevelName SpadicRawMessage array!\n"
                    << this->GetName () << " will be inactive";
      return kERROR;
    }

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();
  if (!fDigiMan->IsPresent(ECbmModuleId::kTrd))
    {
      LOG(fatal) << "No InputDataLevelName TrdDigi array!\n" << this->GetName ()
                    << " will be inactive";
      return kERROR;
    }

  SetOutputBranchPersistent("TrdCluster",false);

  fBT->SetDigisArray(nullptr);
  fClusters = new TClonesArray ("CbmTrdCluster", 100);
  ioman->Register ("TrdCluster", "TRD Clusters", fClusters,
                   IsOutputBranchPersistent ("TrdCluster"));
		   //false);

  // Do whatever else is needed at the initilization stage
  // Create histograms to be filled
  // initialize variables
  CbmTrdSimpleClusterizer::CreateHistograms ();

  return kSUCCESS;
}

void
CbmTrdSimpleClusterizer::CreateHistograms ()
{
  TString GraphName = "Clusters_Created";
  fHm->Add (GraphName.Data (), new TGraph);
  fHm->G1 (GraphName.Data ())->SetNameTitle (GraphName.Data (),
                                             GraphName.Data ());
  fHm->G1 (GraphName.Data ())->GetXaxis ()->SetTitle ("TimeSlice");
  fHm->G1 (GraphName.Data ())->GetYaxis ()->SetTitle ("Nr of created Clusters");
  for (Int_t layer = 0; layer < fBT->GetNrLayers(); layer++)
    {
      TString HistName = "Cluster_size_Distribution_Layer_"
          + std::to_string (layer);
      fHm->Add (
          HistName.Data (),
          new TH2I (HistName.Data (), HistName.Data (), fBT->GetNrSpadics(), -0.5,
                    fBT->GetNrSpadics() - 0.5, 16, -0.5, 15.5));
      fHm->H2 (HistName.Data ())->GetXaxis ()->SetTitle ("Spadic");
      fHm->H2 (HistName.Data ())->GetYaxis ()->SetTitle ("Cluster Size");
    }
  for (Int_t layer = 0; layer < fBT->GetNrLayers(); layer++)
    {
      TString HistName = "Cluster_Size_Type_Distribution_Layer_"
          + std::to_string (layer);
      fHm->Add (
          HistName.Data (),
          new TH2I (HistName.Data (), HistName.Data (), 16, -0.5, 15.5, 6, -0.5,
                    5.5));
      fHm->H2 (HistName.Data ())->GetYaxis ()->SetTitle ("Cluster Type");
      fHm->H2 (HistName.Data ())->GetXaxis ()->SetTitle ("Cluster Width");
    }

}

void CbmTrdSimpleClusterizer::BubbleSort(){
  bool isNotSorted=true;
  auto CompareDigis=
    [this](const UInt_t a,const UInt_t b){
      std::mutex mtx;
      mtx.lock();
      const CbmTrdDigi*aD=fDigiMan->Get<CbmTrdDigi>(a),
	*bD=fDigiMan->Get<CbmTrdDigi>(b);
      UInt_t AddressA=this->fBT->GetAddress(aD),
	AddressB=this->fBT->GetAddress(bD);
      Int_t LayerA=CbmTrdAddress::GetLayerId(AddressA),
	LayerB=CbmTrdAddress::GetLayerId(AddressB);
      Int_t RowA=CbmTrdAddress::GetRowId(AddressA),
	RowB=CbmTrdAddress::GetRowId(AddressB);
      //std::cout << RowA<<" "<<RowB<<std::endl;
      Int_t ColumnA=CbmTrdAddress::GetColumnId(AddressA),
	ColumnB=CbmTrdAddress::GetColumnId(AddressB);
      if(aD->GetTime()>bD->GetTime())
	if(LayerA>LayerB)
	  if(RowA>RowB)
	    if(ColumnA>ColumnB)
	      return true;
      return false;
    };
  //  int i=0;
  //  std::cout << fProxyArray.size()<<std::endl;
  while(isNotSorted){
    isNotSorted=false;
    for (uint i=0; i<fProxyArray.size()-1;i++)
      {
	int a=fProxyArray[i],b=fProxyArray[i+1];
	if(CompareDigis(a,b))
	  {
	    fProxyArray[i]=b,fProxyArray[i+1]=a;
	    isNotSorted=true;
	    //LOG(fatal) << a << " "<< b<< " " <<CompareDigis(a,b) << CompareDigis(b,a) <<static_cast<CbmTrdDigi*> (fDigis->At (a))->ToString()<<" "<< static_cast<CbmTrdDigi*> (fDigis->At (b))->ToString();
	  }
      }
    /*for (auto x:fProxyArray)
      std::cout <<x << " ";
      std::cout <<std::endl;*/
  }
}

void CbmTrdSimpleClusterizer::FillAndSortProxyArray(){
  fProxyArray.clear();
  UInt_t nSpadicDigis = fDigiMan->GetNofDigis(ECbmModuleId::kTrd); //SPADIC messages per TimeSlic
  for (UInt_t iSpadicDigi = 0; iSpadicDigi < nSpadicDigis; iSpadicDigi+=1){
    const CbmTrdDigi* CurrentDigi = fDigiMan->Get<CbmTrdDigi>(iSpadicDigi);
    if (CurrentDigi == nullptr || CurrentDigi==0x0)
      continue;
    fProxyArray.emplace_back(iSpadicDigi);
  }
  auto CompareDigis=
    [this/*,&nSpadicDigis*/](const UInt_t a,const UInt_t b){
        std::mutex mtx;
	mtx.lock();
	const CbmTrdDigi*aD=fDigiMan->Get<CbmTrdDigi>(a),
	  *bD=fDigiMan->Get<CbmTrdDigi>(b);
	UInt_t TimeA=std::round(aD->GetTime()/fBT->GetSamplingTime()),
	  TimeB=std::round(bD->GetTime()/fBT->GetSamplingTime());
	if(TimeA<TimeB)
	  return true;
	else if (TimeA>TimeB)
	  return false;
	UInt_t AddressA=this->fBT->GetAddress(aD),
	  AddressB=this->fBT->GetAddress(bD);
	Int_t LayerA=CbmTrdAddress::GetLayerId(AddressA),
	  LayerB=CbmTrdAddress::GetLayerId(AddressB);
	Int_t ModuleA=CbmTrdAddress::GetModuleId(AddressA),
	  ModuleB=CbmTrdAddress::GetModuleId(AddressB);
	Int_t RowA=CbmTrdAddress::GetRowId(AddressA),
	  RowB=CbmTrdAddress::GetRowId(AddressB);
	Int_t ColumnA=CbmTrdAddress::GetColumnId(AddressA),
	  ColumnB=CbmTrdAddress::GetColumnId(AddressB);
	UInt_t CompareAddressA=LayerA<<(8+4+8),
	  CompareAddressB=LayerB<<(8+4+8);
	CompareAddressA|=(ModuleA&0xFF)<<(8+4),
	  CompareAddressB|=(ModuleB&0xFF)<<(8+4);	
	CompareAddressA|=(RowA&0xF)<<(8),
	  CompareAddressB|=(RowB&0xF)<<(8);
	CompareAddressA|=(ColumnA&0xFF),
	  CompareAddressB|=(ColumnB&0xFF);
	if(CompareAddressA<CompareAddressB)
	  return true;
	return false;
    };
  std::sort(fProxyArray.begin(),fProxyArray.end(),CompareDigis);
}

void
CbmTrdSimpleClusterizer::Exec (Option_t*)
{
  FillAndSortProxyArray();
//  typedef std::pair<CbmTrdDigi*,Int_t> DataPair;
  static Int_t NrTimeslice = 0;
  LOG(info) << this->GetName () << ": Clearing  Clusters";
  fClusters->Clear ("C");
  UInt_t nSpadicDigis = fDigiMan->GetNofDigis(ECbmModuleId::kTrd); //SPADIC messages per TimeSlic
  UInt_t NrClusters = 0;
  TString GraphName = "Clusters_Created";
  LOG(info) << this->GetName () << ": Reconstructing Clusters from "
               << nSpadicDigis << " Digis";
  CbmTrdCluster*CurrentCluster = nullptr;
  Bool_t newCluster = true;
  Bool_t SplitCluster=false;
  UInt_t CurrentProxy = 0;
  std::vector<int>TriggerCounter;
  //Digis are n longer sortable. Operate on proxy.
  for (UInt_t iSpadicDigi = 0; iSpadicDigi < fProxyArray.size(); ++iSpadicDigi)
    {
      UInt_t previousProxy = CurrentProxy;
      CurrentProxy = (fProxyArray.at(iSpadicDigi));
      const CbmTrdDigi* CurrentDigi=fDigiMan->Get<CbmTrdDigi>(CurrentProxy),
	*previousDigi=fDigiMan->Get<CbmTrdDigi>(previousProxy);
      if (!CurrentDigi||previousProxy==CurrentProxy)
        continue;
      //Fist check for Abort Conditions, done explicitly due to the high amount of state in this loop.
      //If newCluster is true, skip checks.
      //TODO: Check if Implementation as a explicit state machine is feasible.
      if (!newCluster)
        {
          Long64_t previousTime =
              static_cast<Long64_t> ((previousDigi->GetTime () + 1) / (1E3 / 16));
          Long64_t CurrentTime = static_cast<Long64_t> ((CurrentDigi->GetTime ()
              + 1) / (1E3 / 16));
          if (previousTime != CurrentTime)
            {
              //Time window is finished, create new Cluster.
              newCluster=true;
            }
	  UInt_t previousAddress=fBT->GetAddress(previousDigi),
	    currentAddress=fBT->GetAddress(CurrentDigi);
          Int_t PreviousLayer = CbmTrdAddress::GetLayerId (
	      previousAddress);
          Int_t CurrentLayer = CbmTrdAddress::GetLayerId (
              currentAddress);
          Int_t PreviousModule = CbmTrdAddress::GetSectorId (
              previousAddress);
          Int_t CurrentModule = CbmTrdAddress::GetSectorId (
              currentAddress);
          Int_t PreviousRow = CbmTrdAddress::GetRowId (
              previousAddress);
          Int_t CurrentRow = CbmTrdAddress::GetRowId (
              currentAddress);
          if ((PreviousRow != CurrentRow) || (PreviousModule != CurrentModule)
          //||(PreviousSector!=CurrentSector)
              || (PreviousLayer != CurrentLayer))
            {
              //Layer, sector,module or Row is finished, create new Cluster.
              newCluster=true;
            }
          Int_t PreviousCol = CbmTrdAddress::GetColumnId (
              previousAddress);
          Int_t CurrentCol = CbmTrdAddress::GetColumnId (
              currentAddress);
          if (CurrentCol - PreviousCol > 1)
            {
              //Large Gap between Clusters found, create new Cluster.
              newCluster=true;
            }
          if (CurrentDigi->GetTriggerType()==2)
            {
              if(!TriggerCounter.empty())
        	if(TriggerCounter.back()==2)
        	  newCluster=true;
             }
          if (!newCluster&&!TriggerCounter.empty()&&CurrentDigi->GetTriggerType()%2!=0)
            {
              if(TriggerCounter.back()%2!=0)
        	{
        	  newCluster=true;
        	  SplitCluster=true;
        	}
             }
        }
      //Done Checking for abort conditions, create new Cluster if necessary.
      if (newCluster)
        {
	  //std::cout <<previousDigi->ToString() <<CurrentDigi->ToString()<<std::endl<<std::endl;
          if (CurrentCluster)
            {
              //	LOG(info)<<CurrentCluster->GetDigis().size() << " "/*<<CurrentCluster->GetDigi(0)*/;
              //TODO: Accurately set number of Columns.
              CurrentCluster->SetNCols (fBT->GetColumnWidth(CurrentCluster));
              CurrentCluster->SetNRows (fBT->GetRowWidth(CurrentCluster));
              /*if(fBT->GetColumnWidth(CurrentCluster)==1)
                if(previousDigi&&CurrentDigi)
                std::cout << previousDigi->ToString()<<CurrentDigi->ToString()<<std::endl;
	      */
            }
          CurrentCluster = static_cast<CbmTrdCluster*>(fClusters->ConstructedAt(NrClusters++));
          CurrentCluster->SetDigis(std::vector<int>());
          if(SplitCluster&&previousDigi!=nullptr)
            {
              CurrentCluster->AddDigi(previousProxy);
	      SplitCluster=false;
            }
          newCluster = false;
          TriggerCounter.clear();
        }
      //We can now add the Current Digi to the current Cluster.
      //TODO: Filter for Hit Type Pattern. Currently planned for V2, the advanced Clusterizer.
      TriggerCounter.push_back(CurrentDigi->GetTriggerType());
      CurrentCluster->AddDigi (CurrentProxy);
      //Done with loop.
      //TODO: IMPORTANT: cluster tools for charge, displacement and so on.
    }
  fHm->G1 (GraphName.Data ())->SetPoint (fHm->G1 (GraphName.Data ())->GetN (),
                                         NrTimeslice++, NrClusters);
  LOG(info) << this->GetName () << ": Finished creating Clusters";
  LOG(info) << this->GetName () << ": Check size distribution of Clusters";
  std::vector<TH2*> Sizemaps;
  for (Int_t layer = 0; layer < fBT->GetNrLayers(); layer++)
    {
      TString HistName = "Cluster_size_Distribution_Layer_"
          + std::to_string (layer);
      Sizemaps.push_back (fHm->H2 (HistName.Data ()));
    }

  for (UInt_t iCluster = 0; iCluster < NrClusters; ++iCluster)
    {
      CurrentCluster = static_cast<CbmTrdCluster*> (fClusters->At (iCluster));
      if (CurrentCluster)
        {
          const CbmTrdDigi*firstDigi = fDigiMan->Get<CbmTrdDigi>(
              *(CurrentCluster->GetDigis ().begin ()));
          Int_t Layer = CbmTrdAddress::GetLayerId (fBT->GetAddress(firstDigi));
          /*Int_t Col = CbmTrdAddress::GetColumnId (firstDigi->GetAddress ());*/
          Sizemaps.at (Layer)->Fill (0.0, CurrentCluster->GetNofDigis ());
        }
    }
  Sizemaps.clear();
  for (Int_t layer = 0; layer < fBT->GetNrLayers(); layer++)
    {
      TString HistName = "Cluster_Size_Type_Distribution_Layer_"
          + std::to_string (layer);
      Sizemaps.push_back (fHm->H2 (HistName.Data ()));
    }
  for (UInt_t iCluster = 0; iCluster < NrClusters; ++iCluster)
    {
      CurrentCluster = static_cast<CbmTrdCluster*> (fClusters->At (iCluster));
      if (CurrentCluster)
        {
          Int_t FirstIndex=
              (CurrentCluster->GetDigis().at(0));
          const CbmTrdDigi*firstDigi = fDigiMan->Get<CbmTrdDigi>(FirstIndex);
          Int_t Layer = CbmTrdAddress::GetLayerId (fBT->GetAddress(firstDigi));
          Sizemaps.at(Layer)->Fill (fBT->GetColumnWidth(CurrentCluster),fBT->ClassifyCluster(CurrentCluster));
        }
    }

  LOG(info) << this->GetName ()
               << ": Done checking size distribution of Clusters";
}

ClassImp(CbmTrdSimpleClusterizer)

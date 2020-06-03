/** @file TTofTrbTdcCalib.cxx
 ** @author Christian Simon <csimon@physi.uni-heidelberg.de>
 ** @date 2016-04-18
 **/
#include "TTofTrbTdcCalib.h"

#include "TMbsUnpackTofPar.h"
#include "TMbsCalibTofPar.h"

#include "TofDef.h"
#include "TTofTrbTdcBoard.h"
#include "TTofTrbTdcData.h"
#include "TTofCalibData.h"
#include "HadaqTdcMessage.h"
#include "TTrbHeader.h"

#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairRootManager.h"
#include "FairLogger.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"
#include "TGraph.h"
#include "TClonesArray.h"

#include <map>

using namespace std;

// =============================================================================
TTofTrbTdcCalib::TTofTrbTdcCalib() :
  FairTask("TTofTrbTdcCalib", 0),
  fMbsUnpackPar(NULL),
  fMbsCalibPar(NULL),
  fbCreateCalib(kFALSE),
  fbSaveOutput(kFALSE),
  fTrbTdcBoardCollection(NULL),
  fTrbHeader(NULL),
  fTrbTdcCalibCollection(NULL),
  fiFineTimeMethod(0),
  fiToTMethod(0),
  fiMinEntriesBinByBin(100000),
  fiMinEntriesLocalLimits(10000),
  fbTimeContinuum(kTRUE),
  fiTrailingOffset(0),
  fuLowerLinearFineLimit(trbtdc::kuSimpleFineTimeMinValue),
  fuUpperLinearFineLimit(trbtdc::kuSimpleFineTimeMaxValue),
  fiReferenceBoard(0),
  fbDisableBoard(NULL),
  fdFinalOffset(NULL),
  fdRefFineTime(),
  fdFineTime(),
  fliFullCoarseOverflows(0),
  fdPreviousRefCoarseTime(0.),
  fdInitialRefOffset(0.),
  fbFirstEvent(kTRUE),
  fdToTSingleLeading(-100.),
  fBufferFile(NULL),
  fBufferTree(NULL),
  fiPreviousSpillIndex(-1),
  fdPreviousEventTime(0.),
  fdSpillStartTime(0.),
  fCtsIdleTimeSpill(NULL),
  fCtsSpillLength(NULL)
{
}

// =============================================================================
void TTofTrbTdcCalib::Exec(Option_t */*option*/)
{
  const Int_t kiTdcChNb = static_cast<Int_t>(trbtdc::kuNbChan);
  const Int_t kiNbActiveBoards = static_cast<Int_t>(fMbsUnpackPar->GetNbActiveBoards(tofMbs::trbtdc));


  if( fbCreateCalib )
  {
    fBufferTree->Fill();
  }
  else
  {
    // from TClonesArray.cxx:
    // TClonesArray allows you to "reuse" the same portion of memory for new/delete avoiding memory
    // fragmentation and memory growth and improving the performance by orders of magnitude.
    // Every time the memory of the TClonesArray has to be reused, the Clear() method is used.
    // To provide its benefits, each TClonesArray must be allocated once per process and disposed
    // of (deleted) only when not needed any more.
    fTrbTdcCalibCollection->Clear("C");

    Int_t iArrayEntries = fTrbTdcBoardCollection->GetEntriesFast();

    if(iArrayEntries != kiNbActiveBoards)
    {
      LOG(fatal)<<"A different number of TTofTrbTdcBoard objects found in the tree than given in the parameter file.";
      return;
    }

    TTofTrbTdcBoard* tTrbTdcBoard(0);

    if(fiReferenceBoard < iArrayEntries)
    {
      tTrbTdcBoard = dynamic_cast<TTofTrbTdcBoard*>(fTrbTdcBoardCollection->At(fiReferenceBoard));
    }

    if(!tTrbTdcBoard)
    {
      LOG(fatal)<<Form("Reference TTofTrbTdcBoard object indexed %d not found.",fiReferenceBoard);
      return;
    }

    // Vector holding for each event the time correction offsets for all TDCs
    vector<Double_t> dBoardOffsets( iArrayEntries, 0. );

    const TTofTrbTdcData& tRefTdcRefTime = tTrbTdcBoard->GetRefChannelData();

    Double_t dRefCoarseTime = static_cast<Double_t>(tRefTdcRefTime.GetFullCoarseTime());

    // The full coarse counter of the reference TDC overflowed between
    // this event and the previous one. To keep the time axis continuous,
    // keep track of such overflows and take them into account.
    if(dRefCoarseTime - fdPreviousRefCoarseTime < -static_cast<Double_t>(trbtdc::kliFullCoarseSize)/2.)
    {
      fliFullCoarseOverflows++;
    }

    fdPreviousRefCoarseTime = dRefCoarseTime;

    Double_t dReferenceOffset = dRefCoarseTime - (fdRefFineTime.at(fiReferenceBoard)).at(static_cast<Int_t>(tRefTdcRefTime.GetFineTime()));

    if( fbFirstEvent )
    {
      fdInitialRefOffset = dReferenceOffset;
      fbFirstEvent = kFALSE;

      // The pre-trigger window can extend up to 1 epoch into
      // the past. By subtracting the initial reference offset
      // excluding the maximum pre-trigger window range we can
      // avoid handling any negative numbers in the analysis.
      // This does, however, not work for the free-streaming
      // system mockup which does not know about the trigger
      // window.
      fdInitialRefOffset -= static_cast<Double_t>(trbtdc::kliEpochSize);
    }

    if( fbTimeContinuum )
    {
      // Calculate the board offset for each channel of the reference board
      dBoardOffsets.at(fiReferenceBoard) = fdInitialRefOffset;

      // continuous time axis
      dBoardOffsets.at(fiReferenceBoard) -= static_cast<Double_t>(fliFullCoarseOverflows*trbtdc::kliFullCoarseSize);

      Double_t fdCurrentEventTime = (dReferenceOffset-dBoardOffsets.at(fiReferenceBoard))*trbtdc::kdClockCycleSizeSec;

      // Store the event time in the run (reference channel of the reference TDC)
      fTrbHeader->SetTimeInRun(fdCurrentEventTime);

      if( 0 > fiPreviousSpillIndex )
      {
        fiPreviousSpillIndex = 0;

        fdSpillStartTime = fdCurrentEventTime;
      }
      // no event for at least 2 seconds: most probably a new spill started
      // (criterion does not work if the spill breaks are not trigger free)
      else if((fdCurrentEventTime-fdPreviousEventTime) > 2.)
      {
        fiPreviousSpillIndex++;

        fCtsSpillLength->Fill(fdPreviousEventTime-fdSpillStartTime);

        fdSpillStartTime = fdCurrentEventTime;

        // time since the last trigger procession in spill breaks
        fCtsIdleTimeSpill->Fill(fdCurrentEventTime-fdPreviousEventTime);
      }

      // Index all spills in a run consecutively
      fTrbHeader->SetSpillIndex(fiPreviousSpillIndex);

      fTrbHeader->SetTimeInSpill(fdCurrentEventTime-fdSpillStartTime);

      fdPreviousEventTime = fdCurrentEventTime;
    }


    // TODO: The loop to determine the board offsets could as well be merged with the
    // main loop over boards and channels. For now, we keep it separate in case a
    // mechanism to handle clock counter jumps needs to be implemented in the future.
    for(Int_t iBoardIndex = 0; iBoardIndex < iArrayEntries; iBoardIndex++)
    {
      if( (fiReferenceBoard != iBoardIndex) && !fbDisableBoard->at(iBoardIndex) )
      {
        tTrbTdcBoard = dynamic_cast<TTofTrbTdcBoard*>(fTrbTdcBoardCollection->At(iBoardIndex));

        const TTofTrbTdcData& tTdcRefTime = tTrbTdcBoard->GetRefChannelData();

        Double_t dCoarseTime = static_cast<Double_t>(tTdcRefTime.GetFullCoarseTime());

        Double_t dTimeOffset = dCoarseTime - (fdRefFineTime.at(iBoardIndex)).at(static_cast<Int_t>(tTdcRefTime.GetFineTime())) - dReferenceOffset;

        // Ensure equal offsets throughout the data sample, i.e. correct for
        // overflows of the TDCs' full coarse counters w.r.t. the reference TDC's
        // full coarse counter.
        if( (dTimeOffset - fdFinalOffset->at(iBoardIndex)) >  static_cast<Double_t>(trbtdc::kliFullCoarseSize)/4. )
        {
          dBoardOffsets.at(iBoardIndex) += static_cast<Double_t>(trbtdc::kliFullCoarseSize);
        }
        else if( (dTimeOffset - fdFinalOffset->at(iBoardIndex)) <  -static_cast<Double_t>(trbtdc::kliFullCoarseSize)/4. )
        {
          dBoardOffsets.at(iBoardIndex) -= static_cast<Double_t>(trbtdc::kliFullCoarseSize);
        }

        // Annihilate these offsets (constrained by differences in the LVDS trigger
        // signal run times that cannot be corrected for on the raw-data level).
        dBoardOffsets.at(iBoardIndex) += fdFinalOffset->at(iBoardIndex);

        if( fbTimeContinuum )
        {
          // Let the common time axis of all TDCs start at 0 (with some negative
          // values from the content of the first event's pre-trigger buffer).
          dBoardOffsets.at(iBoardIndex) += fdInitialRefOffset;

          // For time axis continuity, correct for full coarse counter overflows
          // in the reference TDC.
          dBoardOffsets.at(iBoardIndex) -= static_cast<Double_t>(fliFullCoarseOverflows*trbtdc::kliFullCoarseSize);
        }
      }
    }

    // Now all TDC offsets are available in the vector dBoardOffsets.
    // We can again loop over all boards and consider the regular TDC
    // channels this time.
    for(Int_t iBoardIndex = 0; iBoardIndex < iArrayEntries; iBoardIndex++)
    {
      if( !fbDisableBoard->at(iBoardIndex) || (fiReferenceBoard == iBoardIndex) )
      {
        tTrbTdcBoard = dynamic_cast<TTofTrbTdcBoard*>(fTrbTdcBoardCollection->At(iBoardIndex));

        // Information needed to board-internally correct for full coarse counter
        // overflows w.r.t. the respective reference channel
        const TTofTrbTdcData& tTdcRefTime = tTrbTdcBoard->GetRefChannelData();
        Long64_t liRefCoarseTime = static_cast<Long64_t>(tTdcRefTime.GetFullCoarseTime());

        vector< vector<const TTofTrbTdcData*> > tToTEvenEdgeBuffers( kiTdcChNb/2, vector<const TTofTrbTdcData*>() );
        vector< vector<const TTofTrbTdcData*> > tToTOddEdgeBuffers( kiTdcChNb/2, vector<const TTofTrbTdcData*>() );
        vector< vector<const TTofTrbTdcData*> > tToTBuildingBuffers( kiTdcChNb/2, vector<const TTofTrbTdcData*>() );

        // Reserve memory for the vectors. Elements will be inserted into the
        // container using the push_back method. When the next element to be
        // appended to the vector would exceed the container's capacity,
        // push_back allocates a new buffer for the vector elements which capacity
        // geometrically progresses (new capacity will be k*old_size with k > 1).
        // So there is actually no need to care about managing vector capacities
        // once some (reasonably large) initial memory has been reserved for the container.
        for( Int_t iChannelIndex = 0; iChannelIndex < kiTdcChNb/2; iChannelIndex++ )
        {
          (tToTEvenEdgeBuffers.at(iChannelIndex)).reserve(static_cast<Int_t>(trbtdc::kuNbMulti));
          (tToTOddEdgeBuffers.at(iChannelIndex)).reserve(static_cast<Int_t>(trbtdc::kuNbMulti));
          (tToTBuildingBuffers.at(iChannelIndex)).reserve(2*static_cast<Int_t>(trbtdc::kuNbMulti));
        }

        // Loop over all channel hits in the TDC to build a hit index map and
        // fill the ToT building buffers
        for( Int_t iDataIndex = 0; iDataIndex < static_cast<Int_t>(tTrbTdcBoard->GetDataNb()); iDataIndex++ )
        {
          const TTofTrbTdcData* data = tTrbTdcBoard->GetDataPtr( static_cast<UInt_t>(iDataIndex) );

          Int_t iCurrentChannel(static_cast<Int_t>(data->GetChannel()));

          if( 0 == iCurrentChannel % 2)
          {
            (tToTEvenEdgeBuffers.at(iCurrentChannel/2)).push_back(data);
          }
          else
          {
            (tToTOddEdgeBuffers.at(iCurrentChannel/2)).push_back(data);
          }

          (tToTBuildingBuffers.at(iCurrentChannel/2)).push_back(data);
        }

        // ToT building function
        for( Int_t iChannelIndex = 0; iChannelIndex < kiTdcChNb/2; iChannelIndex++ )
        {
          UInt_t uLeadingModulo;
          UInt_t uTrailingModulo;
          
          UInt_t uLeadingMultiplicity;
          UInt_t uTrailingMultiplicity;

          // positive LVDS input signal
          // leading edge in even channel
          // trailing edge in odd channel
          if( fMbsCalibPar->GetTotInvFlag( toftdc::trb, iBoardIndex, iChannelIndex ) )
          {
            uLeadingModulo = 0;
            uTrailingModulo = 1;

            uLeadingMultiplicity = (tToTEvenEdgeBuffers.at(iChannelIndex)).size();
            uTrailingMultiplicity = (tToTOddEdgeBuffers.at(iChannelIndex)).size();
          }
          // negative LVDS input signal
          // trailing edge in even channel
          // leading edge in odd channel
          else
          {
            uTrailingModulo = 0;
            uLeadingModulo = 1;

            uTrailingMultiplicity = (tToTEvenEdgeBuffers.at(iChannelIndex)).size();
            uLeadingMultiplicity = (tToTOddEdgeBuffers.at(iChannelIndex)).size();
          }

          // advanced ToT building
          if( 0 == fiToTMethod )
          {
            map<Double_t,UInt_t> tToTEdgeMatching;

            for( Int_t iSignalEdge = 0; iSignalEdge < static_cast<Int_t>((tToTBuildingBuffers.at(iChannelIndex)).size()); iSignalEdge++ )
            {
              const TTofTrbTdcData* tEdgeData = (tToTBuildingBuffers.at(iChannelIndex)).at(iSignalEdge);

              Long64_t liEdgeCoarseTime = static_cast<Long64_t>(tEdgeData->GetFullCoarseTime());
              hadaq::TdcMessage::CoarseOverflow(liEdgeCoarseTime,liRefCoarseTime);

              Double_t dEdgeTimeStamp = static_cast<Double_t>(liEdgeCoarseTime);

              dEdgeTimeStamp -= ((fdFineTime.at(iBoardIndex)).at(static_cast<Int_t>(tEdgeData->GetChannel()))).at(static_cast<Int_t>(tEdgeData->GetFineTime()));

              // Artificially stretching the time over threshold by offsetting the
              // trailing edge time to account for situations where the leading edge
              // run time on the chip is longer (up to a few ns) than the trailing
              // edge run time. 
              if( uTrailingModulo == tEdgeData->GetChannel() % 2 )
              {
                dEdgeTimeStamp += static_cast<Double_t>(fiTrailingOffset);
              }

              tToTEdgeMatching.insert( std::make_pair(dEdgeTimeStamp,tEdgeData->GetChannel() % 2) );
            }

            // Now all leading and trailing edges have been corrected (except for the board offset)
            // and are sorted by digitization time (map key) in increasing order. We iterate over
            // the map tToTEdgeMatching and try to combine edges.
            // features: several consecutive leading edges  -> assign all but the last one a fixed ToT
            //           several consecutive trailing edges -> ignore all but the first one

            Bool_t bValidLeadingEdge(kFALSE);
            Double_t dLastLeadingTime;

            for( map<Double_t,UInt_t>::iterator it = tToTEdgeMatching.begin(); it != tToTEdgeMatching.end(); ++it )
            {
              // leading edge
              if( it->second == uLeadingModulo )
              {
                // For several leading edges in a row, assign a fixed ToT value to those which are not followed
                // by a trailing edge
                if( bValidLeadingEdge )
                {
                  // ToT in ps
                  Double_t dTimeOverThreshold = fdToTSingleLeading;

                  // Calibrated hit time in ps
                  Double_t dCalibHitTime = (dLastLeadingTime - dBoardOffsets.at(iBoardIndex))*trbtdc::kdClockCycleSize;

                  // Here, we use TClonesArray::ConstructedAt() instead of TObject's placement new operator to save
                  // a constructor call if a TTofCalibData object has been constructed (and cleared at the beginning
                  // of the Exec method) at the respective array index before. Member variables are changed by mutator methods.
                  TTofCalibData* tNewHit = dynamic_cast<TTofCalibData*>(fTrbTdcCalibCollection->ConstructedAt(fTrbTdcCalibCollection->GetEntriesFast()));

                  tNewHit->SetData( toftdc::trb, static_cast<UInt_t>(iBoardIndex), static_cast<UInt_t>(iChannelIndex), 
                                    dCalibHitTime, dTimeOverThreshold, 2 );
                }

                bValidLeadingEdge = kTRUE;
                dLastLeadingTime = it->first;
              }
              // trailing edge
              else
              {
                if( bValidLeadingEdge )
                {
                  // ToT in ps
                  Double_t dTimeOverThreshold = (it->first - dLastLeadingTime)*trbtdc::kdClockCycleSize;

                  // Calibrated hit time in ps
                  Double_t dCalibHitTime = (dLastLeadingTime - dBoardOffsets.at(iBoardIndex))*trbtdc::kdClockCycleSize;

                  // Here, we use TClonesArray::ConstructedAt() instead of TObject's placement new operator to save
                  // a constructor call if a TTofCalibData object has been constructed (and cleared at the beginning
                  // of the Exec method) at the respective array index before. Member variables are changed by mutator methods.
                  TTofCalibData* tNewHit = dynamic_cast<TTofCalibData*>(fTrbTdcCalibCollection->ConstructedAt(fTrbTdcCalibCollection->GetEntriesFast()));

                  tNewHit->SetData( toftdc::trb, static_cast<UInt_t>(iBoardIndex), static_cast<UInt_t>(iChannelIndex), 
                                    dCalibHitTime, dTimeOverThreshold, 2 );
                }

                bValidLeadingEdge = kFALSE;

              }
            }

          }
          // pair-wise ToT building
          else
          {
            if( uLeadingMultiplicity == uTrailingMultiplicity )
            {
              for( Int_t iEvenIndex = 0; iEvenIndex < static_cast<Int_t>((tToTEvenEdgeBuffers.at(iChannelIndex)).size()); iEvenIndex++ )
              {
                const TTofTrbTdcData* leadingData;
                const TTofTrbTdcData* trailingData;

                if( 0x0 == uLeadingModulo )
                {
                  leadingData  = (tToTEvenEdgeBuffers.at(iChannelIndex)).at(iEvenIndex);
                  trailingData = (tToTOddEdgeBuffers.at(iChannelIndex)).at(iEvenIndex);
                }
                else
                {
                  leadingData = (tToTOddEdgeBuffers.at(iChannelIndex)).at(iEvenIndex);
                  trailingData = (tToTEvenEdgeBuffers.at(iChannelIndex)).at(iEvenIndex);
                }

                Long64_t liLeadingCoarseTime = static_cast<Long64_t>(leadingData->GetFullCoarseTime());
                Long64_t liTrailingCoarseTime = static_cast<Long64_t>(trailingData->GetFullCoarseTime());

                // Correcting for TDC internal coarse counter overflows w.r.t.
                // the reference channel
                hadaq::TdcMessage::CoarseOverflow(liLeadingCoarseTime,liRefCoarseTime);
                hadaq::TdcMessage::CoarseOverflow(liTrailingCoarseTime,liRefCoarseTime);

                Double_t dLeadingTimeStamp = static_cast<Double_t>(liLeadingCoarseTime);
                Double_t dTrailingTimeStamp = static_cast<Double_t>(liTrailingCoarseTime);

                dLeadingTimeStamp -= ((fdFineTime.at(iBoardIndex)).at(static_cast<Int_t>(leadingData->GetChannel()))).at(static_cast<Int_t>(leadingData->GetFineTime()));
                dTrailingTimeStamp -= ((fdFineTime.at(iBoardIndex)).at(static_cast<Int_t>(trailingData->GetChannel()))).at(static_cast<Int_t>(trailingData->GetFineTime()));

                // Artificially stretching the time over threshold by offsetting the
                // trailing edge time to account for situations where the leading edge
                // run time on the chip is longer (up to 3 ns) than the trailing
                // edge run time. 
                dTrailingTimeStamp += static_cast<Double_t>(fiTrailingOffset);

                // ToT in ps
                Double_t dTimeOverThreshold = (dTrailingTimeStamp - dLeadingTimeStamp)*trbtdc::kdClockCycleSize;

                // Calibrated hit time
                Double_t dCalibHitTime = (dLeadingTimeStamp - dBoardOffsets.at(iBoardIndex))*trbtdc::kdClockCycleSize;

                // Here, we use TClonesArray::ConstructedAt() instead of TObject's placement new operator to save
                // a constructor call if a TTofCalibData object has been constructed (and cleared at the beginning
                // of the Exec method) at the respective array index before. Member variables are changed by mutator methods.
                TTofCalibData* tNewHit = dynamic_cast<TTofCalibData*>(fTrbTdcCalibCollection->ConstructedAt(fTrbTdcCalibCollection->GetEntriesFast()));

                tNewHit->SetData( toftdc::trb, static_cast<UInt_t>(iBoardIndex), static_cast<UInt_t>(iChannelIndex), 
                                  dCalibHitTime, dTimeOverThreshold, 2 );
              }
            }
          }
        }
      }

    }
  }
}

// =============================================================================
InitStatus TTofTrbTdcCalib::Init()
{
  if( !fMbsUnpackPar || !fMbsCalibPar )
  {
    LOG(error)<<"Parameter containers not initialized.";
    return kFATAL;
  }

  const Int_t kiTdcChNb = static_cast<Int_t>(trbtdc::kuNbChan);

  const Int_t kiNbActiveBoards = static_cast<Int_t>(fMbsUnpackPar->GetNbActiveBoards(tofMbs::trbtdc));
  if( fiReferenceBoard >= kiNbActiveBoards )
  {
    LOG(error)<<"Reference TDC index out of range.";
    return kFATAL;
  }

  if( fiFineTimeMethod < 0 || fiFineTimeMethod > 2 )
  {
    LOG(error)<<"Invalid fine time interpolation method.";
    LOG(error)<<"Use 0 for bin-by-bin center interpolation.";
    LOG(error)<<"Use 1 for bin-by-bin edge   interpolation.";
    LOG(error)<<"Use 2 for linear            interpolation.";
    return kFATAL;
  }

  if( fiToTMethod < 0 || fiToTMethod > 1 )
  {
    LOG(error)<<"Invalid ToT building method.";
    LOG(error)<<"Use 0 for advanced ToT building.";
    LOG(error)<<"Use 1 for pair-wise ToT edge matching (requires same numbers of leading and trailing edges for the channel).";
    return kFATAL;
  }

  FairRootManager* tRootManager = FairRootManager::Instance();

  // Get access to the output-tree branch holding TTofTrbTdcBoard objects
  fTrbTdcBoardCollection = dynamic_cast<TClonesArray*>(tRootManager->GetObject("TofTrbTdc"));
  if( !fTrbTdcBoardCollection )
  {
    LOG(error)<<"Could not retrieve branch 'TofTrbTdc' from FairRootManager.";
    return kFATAL;
  }

  if( fbCreateCalib )
  {
    // Generating TDC to TDC offset calibration requires the full raw data branch
    // of the event tree to be accessible in Finish(). We could achieve this by
    // making the branch 'TofTrbTdc' permanent in FairRootManager's output tree.
    // Depending on the calibration statistics this might lead to very big file
    // sizes although we only need the calibration histograms and values in the
    // real data processing run. This gave rise to the idea of creating our own
    // tree file for making the TTofTrbTdcBoard objects available independently
    // of the memory persistency of 'TofTrbTdc'. Once the calibration run has
    // finished this file can be discarded.
    // Also, we cannot risk working with a pure memory tree as we might run out
    // of memory and crash accordingly at high calibration statistics.

    TDirectory* tOldDirectory = gDirectory;

    TFile* tOutputFile = tRootManager->GetOutFile();

    TString tBufferFileName(tOutputFile->GetName());
    tBufferFileName.Replace(tBufferFileName.Length()-10,10,"buffer.root");

    fBufferFile = new TFile(tBufferFileName.Data(),"RECREATE");

    fBufferTree = new TTree("trbtree","My TRB tree");

    fBufferTree->Branch("TofTrbTdc",&fTrbTdcBoardCollection);

    tOldDirectory->cd();
  }
  else
  {
    fTrbHeader = dynamic_cast<TTrbHeader*>(tRootManager->GetObject("TofTrbHeader."));
    if( !fTrbHeader )
    {
      LOG(error)<<"Could not retrieve branch 'TofTrbHeader' from FairRootManager.";
      return kFATAL;
    }

    fCtsIdleTimeSpill = dynamic_cast<TH1I*>(gROOT->FindObjectAny("tof_trb_cts_idle_spill"));
    if( !fCtsIdleTimeSpill )
    {
      LOG(error)<<"Could not retrieve histogram 'tof_trb_cts_idle_spill' from TROOT instance.";
      return kFATAL;
    }

    fCtsSpillLength = dynamic_cast<TH1I*>(gROOT->FindObjectAny("tof_trb_cts_spill_length"));
    if( !fCtsSpillLength )
    {
      LOG(error)<<"Could not retrieve histogram 'tof_trb_cts_spill_length' from TROOT instance.";
      return kFATAL;
    }

    // By default, after construction TObject** TClonesArray::fCont points to 1000 pointers
    // to objects that are supposed to be stored in the array. If the total number of TDC hits per
    // event exceeded this limit, the TClonesArray would be automatically expanded. To prevent
    // such an expansion from happening, at least with an asynchronous readout trigger and the
    // trigger window feature enabled, we ask for the following number of pointers:
    fTrbTdcCalibCollection = new TClonesArray("TTofCalibData",kiNbActiveBoards*kiTdcChNb/2*trbtdc::kuNbMulti);

    // We apply the calibration to the raw data and create TTofCalibData objects
    // as a result. When creating the calibration files, we do not further process
    // the data.
    tRootManager->Register("TofCalibData","TofCalib",static_cast<TCollection*>(fTrbTdcCalibCollection),fbSaveOutput);

    TFile* tOutputFile = tRootManager->GetOutFile();

    TString tCalibFileName(tOutputFile->GetName());
    tCalibFileName.Replace(tCalibFileName.Length()-8,8,"calib.root");

    TFile* tCalibFile = new TFile(tCalibFileName.Data(),"READ");
    // gDirectory points to the calibration file
    if( tCalibFile->IsZombie() )
    {
      tCalibFile->Close();
      delete tCalibFile;

      LOG(error)<<"Could not open calibration file.";
      return kFATAL;
    }

    if( !tCalibFile->cd("TofTrbCalib") )
    {
      LOG(error)<<"Could not find directory 'TofTrbCalib' in calibration file.";
      return kFATAL;
    }

    TDirectory* tCalibParent = gDirectory;

    vector<Int_t>* iReferenceBoard = NULL;

    gDirectory->GetObject("iReferenceBoard",iReferenceBoard);
    if( !iReferenceBoard )
    {
      LOG(error)<<"Could not retrieve 'iReferenceBoard' from the calibration file.";
      return kFATAL;
    }

    if( iReferenceBoard->at(0) != fiReferenceBoard )
    {
      LOG(warn)<<Form("TDC to TDC offsets were calibrated w.r.t. TDC %d, not the currently selected TDC %d.",iReferenceBoard->at(0),fiReferenceBoard);
      LOG(warn)<<"The current setting will be ignored.";

      fiReferenceBoard = iReferenceBoard->at(0);
    }

    vector<UInt_t>* uGlobalFineTimeLimits = NULL;

    gDirectory->GetObject("uGlobalFineTimeLimits",uGlobalFineTimeLimits);
    if( !uGlobalFineTimeLimits )
    {
      LOG(error)<<"Could not retrieve 'uGlobalFineTimeLimits' from the calibration file.";
      return kFATAL;
    }

    if( uGlobalFineTimeLimits->at(0) != fuLowerLinearFineLimit || uGlobalFineTimeLimits->at(1) != fuUpperLinearFineLimit )
    {
      LOG(warn)<<"The global linear fine time interpolation limits do not match the ones used for TDC offset calibration.";
      LOG(warn)<<"The current settings will be ignored.";

      fuLowerLinearFineLimit = uGlobalFineTimeLimits->at(0);
      fuUpperLinearFineLimit = uGlobalFineTimeLimits->at(1);
    }

    gDirectory->GetObject("bDisableBoard",fbDisableBoard);
    if( !fbDisableBoard )
    {
      LOG(error)<<"Could not retrieve 'bDisableBoard' from the calibration file.";
      return kFATAL;
    }

    gDirectory->GetObject("dFinalOffset",fdFinalOffset);
    if( !fdFinalOffset )
    {
      LOG(error)<<"Could not retrieve 'dFinalOffset' from the calibration file.";
      return kFATAL;
    }


    fdRefFineTime.resize( kiNbActiveBoards, vector<Double_t>( trbtdc::kiFineCounterSize, 0. ) );
    fdFineTime.resize( kiNbActiveBoards, vector< vector<Double_t> >( kiTdcChNb, vector<Double_t>( trbtdc::kiFineCounterSize, 0. ) ) );

    vector<TH1I*> tRefFineTimeHistos( kiNbActiveBoards, NULL );
    vector< vector<TH1I*> > tFineTimeHistos( kiNbActiveBoards, vector<TH1I*>( kiTdcChNb, NULL ) );

    vector< vector<UInt_t> > uRefFineTimeLimits( kiNbActiveBoards, vector<UInt_t>( 2, 0 ) );
    vector< vector< vector<UInt_t> > > uFineTimeLimits( kiNbActiveBoards, vector< vector<UInt_t> >( kiTdcChNb, vector<UInt_t>( 2, 0 ) ) );

    for(Int_t iBoardIndex = 0; iBoardIndex < kiNbActiveBoards; iBoardIndex++)
    {
      if(!fMbsCalibPar->CheckTotInvFlag( toftdc::trb, static_cast<UInt_t>(iBoardIndex) ))
      {
        LOG(error)<<Form("Check your parCalib* file! No ToT inversion flags provided for TDCs indexed %2d and above.",iBoardIndex);
        return kFATAL;
      }

      if( !tCalibParent->cd(Form("tdc_b%03d",iBoardIndex)) )
      {
        LOG(error)<<Form("Could not find directory 'tdc_b%03d' in calibration file.",iBoardIndex);
        return kFATAL;
      }

      gDirectory->GetObject(Form("calib_trb_ft_b%03d_ref",iBoardIndex),tRefFineTimeHistos.at(iBoardIndex));

      if( tRefFineTimeHistos.at(iBoardIndex) )
      {
        tRefFineTimeHistos.at(iBoardIndex)->SetDirectory(gROOT);

        (uRefFineTimeLimits.at(iBoardIndex)).at(0) = static_cast<UInt_t>(tRefFineTimeHistos.at(iBoardIndex)->FindFirstBinAbove(0.));
        (uRefFineTimeLimits.at(iBoardIndex)).at(1) = static_cast<UInt_t>(tRefFineTimeHistos.at(iBoardIndex)->FindLastBinAbove(0.));

        // Insufficient number of entries in the fine time histogram
        // OR unreasonably narrow fine time distribution.
        // Switch to linear interpolation based on globally defined limits.
        if( (fiMinEntriesLocalLimits > tRefFineTimeHistos.at(iBoardIndex)->GetEntries()) || 
            (trbtdc::kuMinimumFineTimeRange > ((uRefFineTimeLimits.at(iBoardIndex)).at(1) - (uRefFineTimeLimits.at(iBoardIndex)).at(0))) )
        {
          hadaq::TdcMessage::SetFineLimits(fuLowerLinearFineLimit,fuUpperLinearFineLimit);
        }
        else
        {
          hadaq::TdcMessage::SetFineLimits((uRefFineTimeLimits.at(iBoardIndex)).at(0),(uRefFineTimeLimits.at(iBoardIndex)).at(1));
        }

        // Insufficient number of entries in the fine time histogram
        // OR unreasonably narrow fine time distribution.
        // Applying the bin-by-bin interpolation method is not reasonable.
        // Regardless of the chosen global interpolation method data from
        // this TDC channel will be interpolated linearly within the globally
        // defined limits.
        if( (fiMinEntriesBinByBin > tRefFineTimeHistos.at(iBoardIndex)->GetEntries()) || 
            (trbtdc::kuMinimumFineTimeRange > ((uRefFineTimeLimits.at(iBoardIndex)).at(1) - (uRefFineTimeLimits.at(iBoardIndex)).at(0))) )
        {
          // fine time range [0,1023]
          // histogram bin range: [1,1024]
          // mapping: ft(0)    -> bin(1)
          //          ft(1023) -> bin(1024)
          for(Int_t iBin = 1; iBin <= tRefFineTimeHistos.at(iBoardIndex)->GetNbinsX(); iBin++)
          {
            (fdRefFineTime.at(iBoardIndex)).at(iBin-1) = hadaq::TdcMessage::SimpleFineCalibr(static_cast<UInt_t>(iBin)-1);
          }
        }
        else
        {
          if( trbtdc::finetime_bincenter == fiFineTimeMethod )
          {
            for(Int_t iBin = 1; iBin <= tRefFineTimeHistos.at(iBoardIndex)->GetNbinsX(); iBin++)
            {
              (fdRefFineTime.at(iBoardIndex)).at(iBin-1) = (tRefFineTimeHistos.at(iBoardIndex)->Integral(1,iBin) - 0.5*tRefFineTimeHistos.at(iBoardIndex)->GetBinContent(iBin))
                                                           /tRefFineTimeHistos.at(iBoardIndex)->Integral(1,tRefFineTimeHistos.at(iBoardIndex)->GetNbinsX());
            }
          }
          else if( trbtdc::finetime_binedge == fiFineTimeMethod )
          {
            for(Int_t iBin = 1; iBin <= tRefFineTimeHistos.at(iBoardIndex)->GetNbinsX(); iBin++)
            {
              (fdRefFineTime.at(iBoardIndex)).at(iBin-1) = tRefFineTimeHistos.at(iBoardIndex)->Integral(1,iBin)
                                                           /tRefFineTimeHistos.at(iBoardIndex)->Integral(1,tRefFineTimeHistos.at(iBoardIndex)->GetNbinsX());
            }
          }
          else
          {
            for(Int_t iBin = 1; iBin <= tRefFineTimeHistos.at(iBoardIndex)->GetNbinsX(); iBin++)
            {
              (fdRefFineTime.at(iBoardIndex)).at(iBin-1) = hadaq::TdcMessage::SimpleFineCalibr(static_cast<UInt_t>(iBin)-1);
            }
          }
        }

      }
      else
      {
        LOG(error)<<Form("Could not retrieve the reference fine time histogram of TDC %d.",iBoardIndex);
        return kFATAL;
      }

      for(Int_t iChannelIndex = 0; iChannelIndex < kiTdcChNb; iChannelIndex++)
      {
        gDirectory->GetObject(Form("calib_trb_ft_b%03d_ch%03d",iBoardIndex,iChannelIndex),(tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex));

        if( (tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex) )
        {
          (tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->SetDirectory(gROOT);

          ((uFineTimeLimits.at(iBoardIndex)).at(iChannelIndex)).at(0) = static_cast<UInt_t>((tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->FindFirstBinAbove(0.));
          ((uFineTimeLimits.at(iBoardIndex)).at(iChannelIndex)).at(1) = static_cast<UInt_t>((tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->FindLastBinAbove(0.));

          // Insufficient number of entries in the fine time histogram
          // OR unreasonably narrow fine time distribution.
          // Switch to linear interpolation based on globally defined limits.
          if( (fiMinEntriesLocalLimits > (tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->GetEntries()) || 
              (trbtdc::kuMinimumFineTimeRange > (((uFineTimeLimits.at(iBoardIndex)).at(iChannelIndex)).at(1) - ((uFineTimeLimits.at(iBoardIndex)).at(iChannelIndex)).at(0))) )
          {
            hadaq::TdcMessage::SetFineLimits(fuLowerLinearFineLimit,fuUpperLinearFineLimit);
          }
          else
          {
            hadaq::TdcMessage::SetFineLimits(((uFineTimeLimits.at(iBoardIndex)).at(iChannelIndex)).at(0),((uFineTimeLimits.at(iBoardIndex)).at(iChannelIndex)).at(1));
          }

          // Insufficient number of entries in the fine time histogram
          // OR unreasonably narrow fine time distribution.
          // Applying the bin-by-bin interpolation method is not reasonable.
          // Regardless of the chosen global interpolation method data from
          // this TDC channel will be interpolated linearly within the globally
          // defined limits.
          if( (fiMinEntriesBinByBin > (tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->GetEntries()) || 
              (trbtdc::kuMinimumFineTimeRange > (((uFineTimeLimits.at(iBoardIndex)).at(iChannelIndex)).at(1) - ((uFineTimeLimits.at(iBoardIndex)).at(iChannelIndex)).at(0))) )
          {
            // fine time range [0,1023]
            // histogram bin range: [1,1024]
            // mapping: ft(0)    -> bin(1)
            //          ft(1023) -> bin(1024)
            for(Int_t iBin = 1; iBin <= (tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->GetNbinsX(); iBin++)
            {
              ((fdFineTime.at(iBoardIndex)).at(iChannelIndex)).at(iBin-1) = hadaq::TdcMessage::SimpleFineCalibr(static_cast<UInt_t>(iBin)-1);
            }
          }
          else
          {
            if( trbtdc::finetime_bincenter == fiFineTimeMethod )
            {
              for(Int_t iBin = 1; iBin <= (tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->GetNbinsX(); iBin++)
              {
                ((fdFineTime.at(iBoardIndex)).at(iChannelIndex)).at(iBin-1) = ( (tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->Integral(1,iBin)
                                                                               - 0.5*(tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->GetBinContent(iBin) )
                                                                              /(tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->Integral(1,(tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->GetNbinsX());
              }
            }
            else if( trbtdc::finetime_binedge == fiFineTimeMethod )
            {
              for(Int_t iBin = 1; iBin <= (tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->GetNbinsX(); iBin++)
              {
                ((fdFineTime.at(iBoardIndex)).at(iChannelIndex)).at(iBin-1) = (tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->Integral(1,iBin)
                                                                            /(tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->Integral(1,(tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->GetNbinsX());
              }
            }
            else
            {
              for(Int_t iBin = 1; iBin <= (tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->GetNbinsX(); iBin++)
              {
                ((fdFineTime.at(iBoardIndex)).at(iChannelIndex)).at(iBin-1) = hadaq::TdcMessage::SimpleFineCalibr(static_cast<UInt_t>(iBin)-1);
              }
            }
          }

        }
        else
        {
          LOG(error)<<Form("Could not retrieve the fine time histogram of channel %2d of TDC %2d.",iBoardIndex,iChannelIndex);
          return kFATAL;
        }
      }
    }

    tCalibFile->Close();
    // gDirectory points to whatever it pointed to before the calibration file was opened
    delete tCalibFile;

  }

  return kSUCCESS;
}

// =============================================================================
void TTofTrbTdcCalib::SetParContainers()
{
  FairRun* tRun = FairRun::Instance();
  if( !tRun )
  {
    LOG(fatal)<<Form("FairRun instance not found.");
  }

  FairRuntimeDb* tDataBase = tRun->GetRuntimeDb();
  if( !tDataBase )
  {
    LOG(fatal)<<Form("FairRuntimeDb instance not found.");
  }

  // request initialization of parameter containers
  // The actual parameter container initializiation happens only upon calling
  // FairRuntimeDb::initContainers in the FairRun instance
  fMbsUnpackPar = dynamic_cast<TMbsUnpackTofPar*>(tDataBase->getContainer("TMbsUnpackTofPar"));
  fMbsCalibPar = dynamic_cast<TMbsCalibTofPar*>(tDataBase->getContainer("TMbsCalibTofPar"));
}

// =============================================================================
void TTofTrbTdcCalib::Finish()
{
  const Int_t kiTdcChNb = static_cast<Int_t>(trbtdc::kuNbChan);
  const Int_t kiNbActiveBoards = static_cast<Int_t>(fMbsUnpackPar->GetNbActiveBoards(tofMbs::trbtdc));

  if( fbCreateCalib )
  {

    FairRootManager* tRootManager = FairRootManager::Instance();

    TFile* tOutputFile = tRootManager->GetOutFile();
    if( !tOutputFile )
    {
      LOG(fatal)<<"Output file could not be retrieved from FairRootManager.";
      return;
    }

    tOutputFile->mkdir("TofTrbCalib");

    TBranch* tBranch = fBufferTree->GetBranch("TofTrbTdc");
    if( !tBranch )
    {
      LOG(fatal)<<"Branch 'TofTrbTdc' not found in calib buffer tree.";
      return;
    }

    Long64_t lBranchEntries = tBranch->GetEntries();

    Long64_t lInitialLoopPasses = 0;

    if( 100 > lBranchEntries )
    {
      lInitialLoopPasses = lBranchEntries;
    }
    else
    {
      lInitialLoopPasses = 100;
    }

    void* vOldAddress = tBranch->GetAddress();

    TDirectory* tOldDirectory = gDirectory;
    gROOT->cd();
    // gDirectory points to Rint

    TClonesArray* tArray = new TClonesArray("TTofTrbTdcBoard");
    tBranch->SetAddress(&tArray);

    // The vector of vectors approach is necessary because kiNbActiveBoards is only
    // known at run time, not at compile time. We would need a run-time sized
    // container here which size is fixed.
    // A more elegant solution requires ROOT to be compiled with c++11 support.
    // vector< array<UInt_t,2> > uRefFineTimeLimits(kiNbActiveBoards);


    vector< vector<UInt_t> > uRefFineTimeLimits( kiNbActiveBoards, vector<UInt_t>( 2, 0 ) );
    vector<TH1I*> tRefFineTimeHistos( kiNbActiveBoards, NULL );
    vector< vector<Double_t> > dRefFineTimeLinear( kiNbActiveBoards, vector<Double_t>( trbtdc::kiFineCounterSize, 0. ) );
    vector< vector<Double_t> > dRefFineTimeBinCenter( kiNbActiveBoards, vector<Double_t>( trbtdc::kiFineCounterSize, 0. ) );
    vector< vector<Double_t> > dRefFineTimeBinEdge( kiNbActiveBoards, vector<Double_t>( trbtdc::kiFineCounterSize, 0. ) );

    vector< vector<TH1I*> > tFineTimeHistos( kiNbActiveBoards, vector<TH1I*>( kiTdcChNb, NULL ) );

    for(Int_t iBoardIndex = 0; iBoardIndex < kiNbActiveBoards; iBoardIndex++)
    {
      if(!fMbsCalibPar->CheckTotInvFlag( toftdc::trb, static_cast<UInt_t>(iBoardIndex) ))
      {
        LOG(fatal)<<Form("Check your parCalib* file! No ToT inversion flags provided for TDCs indexed %2d and above.",iBoardIndex);
        return;
      }

      tRefFineTimeHistos.at(iBoardIndex) = dynamic_cast<TH1I*>(gROOT->FindObjectAny(Form("tof_trb_ft_b%03d_ref",iBoardIndex)));
      // Renaming the calibration histogram at this point to avoid name conflicts in the actual calibration event loop
      tRefFineTimeHistos.at(iBoardIndex)->SetName(Form("calib_trb_ft_b%03d_ref",iBoardIndex));

      if(tRefFineTimeHistos.at(iBoardIndex))
      {
        // Writing the reference channel fine time distributions to file
        tOutputFile->cd("TofTrbCalib");
        TDirectory* tBoardDirectory = gDirectory->mkdir(Form("tdc_b%03d",iBoardIndex));
        tBoardDirectory->cd();
        tRefFineTimeHistos.at(iBoardIndex)->Write();

        (uRefFineTimeLimits.at(iBoardIndex)).at(0) = static_cast<UInt_t>(tRefFineTimeHistos.at(iBoardIndex)->FindFirstBinAbove(0.));
        (uRefFineTimeLimits.at(iBoardIndex)).at(1) = static_cast<UInt_t>(tRefFineTimeHistos.at(iBoardIndex)->FindLastBinAbove(0.));

        // Insufficient number of entries in the fine time histogram
        // OR unreasonably narrow fine time distribution.
        // Switch to linear interpolation based on globally defined limits.
        if( (fiMinEntriesLocalLimits > tRefFineTimeHistos.at(iBoardIndex)->GetEntries()) || 
            (trbtdc::kuMinimumFineTimeRange > ((uRefFineTimeLimits.at(iBoardIndex)).at(1) - (uRefFineTimeLimits.at(iBoardIndex)).at(0))) )
        {
          hadaq::TdcMessage::SetFineLimits(fuLowerLinearFineLimit,fuUpperLinearFineLimit);
        }
        else
        {
          hadaq::TdcMessage::SetFineLimits((uRefFineTimeLimits.at(iBoardIndex)).at(0),(uRefFineTimeLimits.at(iBoardIndex)).at(1));
        }

        // Insufficient number of entries in the fine time histogram
        // OR unreasonably narrow fine time distribution.
        // Applying the bin-by-bin interpolation method is not reasonable.
        // Regardless of the chosen global interpolation method data from
        // this TDC channel will be interpolated linearly within the globally
        // defined limits.
        if( (fiMinEntriesBinByBin > tRefFineTimeHistos.at(iBoardIndex)->GetEntries()) || 
            (trbtdc::kuMinimumFineTimeRange > ((uRefFineTimeLimits.at(iBoardIndex)).at(1) - (uRefFineTimeLimits.at(iBoardIndex)).at(0))) )
        {
          // fine time range [0,1023]
          // histogram bin range: [1,1024]
          // mapping: ft(0)    -> bin(1)
          //          ft(1023) -> bin(1024)
          for(Int_t iBin = 1; iBin <= tRefFineTimeHistos.at(iBoardIndex)->GetNbinsX(); iBin++)
          {
            (dRefFineTimeBinCenter.at(iBoardIndex)).at(iBin-1) = hadaq::TdcMessage::SimpleFineCalibr(static_cast<UInt_t>(iBin)-1);
            (dRefFineTimeBinEdge.at(iBoardIndex)).at(iBin-1) = hadaq::TdcMessage::SimpleFineCalibr(static_cast<UInt_t>(iBin)-1);
            (dRefFineTimeLinear.at(iBoardIndex)).at(iBin-1) = hadaq::TdcMessage::SimpleFineCalibr(static_cast<UInt_t>(iBin)-1);
          }
        }
        else
        {
          for(Int_t iBin = 1; iBin <= tRefFineTimeHistos.at(iBoardIndex)->GetNbinsX(); iBin++)
          {
            (dRefFineTimeBinCenter.at(iBoardIndex)).at(iBin-1) = (tRefFineTimeHistos.at(iBoardIndex)->Integral(1,iBin) - 0.5*tRefFineTimeHistos.at(iBoardIndex)->GetBinContent(iBin))
                                                                /tRefFineTimeHistos.at(iBoardIndex)->Integral(1,tRefFineTimeHistos.at(iBoardIndex)->GetNbinsX());

            (dRefFineTimeBinEdge.at(iBoardIndex)).at(iBin-1) = tRefFineTimeHistos.at(iBoardIndex)->Integral(1,iBin)
                                                              /tRefFineTimeHistos.at(iBoardIndex)->Integral(1,tRefFineTimeHistos.at(iBoardIndex)->GetNbinsX());

            (dRefFineTimeLinear.at(iBoardIndex)).at(iBin-1) = hadaq::TdcMessage::SimpleFineCalibr(static_cast<UInt_t>(iBin)-1);
          }
        }
      }
      else
      {
        LOG(fatal)<<Form("No fine time histogram available for reference channel of TDC %d.",iBoardIndex);
        return;
      }

      for(Int_t iChannelIndex = 0; iChannelIndex < kiTdcChNb; iChannelIndex++)
      {
        (tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex) = dynamic_cast<TH1I*>(gROOT->FindObjectAny(Form("tof_trb_ft_b%03d_ch%03d",iBoardIndex,iChannelIndex)));
        // Renaming the calibration histogram at this point to avoid name conflicts in the actual calibration event loop
        (tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->SetName(Form("calib_trb_ft_b%03d_ch%03d",iBoardIndex,iChannelIndex));

        if((tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex))
        {
          (tFineTimeHistos.at(iBoardIndex)).at(iChannelIndex)->Write();
        }
        else
        {
          LOG(fatal)<<Form("No fine time histogram available for channel %2d of TDC %2d.",iBoardIndex,iChannelIndex);
          return;
        }
      }

    }

    gROOT->cd();
    // gDirectory points to Rint
  

    // In 3 histogram iterations, we determine time offsets between TDCs w.r.t a reference TDC board.

    // +++++++ 1st iteration +++++++++++++++++++++++++++++++++++++++++++++++++++
    // Divide the full coarse time axis into 4 sectors with granularities of 2.6 ms
    //
    // kliFullCoarseSize = 549,755,813,888 (in units of 5 ns)
    // 1 Bin = 524,288 coarse time units = 2.6 ms
    // 524,288 bins each (2^38 -> 2^19 -> 2^19)

    vector< vector<TH1D*> > tFirstOffsetSearch( kiNbActiveBoards, vector<TH1D*>( 4, NULL));

    for(Int_t iBoardIndex = 0; iBoardIndex < kiNbActiveBoards; iBoardIndex++)
    {
      (tFirstOffsetSearch.at(iBoardIndex)).at(0) = new TH1D(Form("tFirstOffsetSearch%d_0",iBoardIndex),Form("TDC %d, ref-TDC %d",iBoardIndex,fiReferenceBoard),
                                                           524288,static_cast<Double_t>(-trbtdc::kliFullCoarseSize),static_cast<Double_t>(-trbtdc::kliFullCoarseSize/2));

      (tFirstOffsetSearch.at(iBoardIndex)).at(1) = new TH1D(Form("tFirstOffsetSearch%d_1",iBoardIndex),Form("TDC %d, ref-TDC %d",iBoardIndex,fiReferenceBoard),
                                                           524288,static_cast<Double_t>(-trbtdc::kliFullCoarseSize/2),0.);

      (tFirstOffsetSearch.at(iBoardIndex)).at(2) = new TH1D(Form("tFirstOffsetSearch%d_2",iBoardIndex),Form("TDC %d, ref-TDC %d",iBoardIndex,fiReferenceBoard),
                                                           524288,0.,static_cast<Double_t>(trbtdc::kliFullCoarseSize/2));

      (tFirstOffsetSearch.at(iBoardIndex)).at(3) = new TH1D(Form("tFirstOffsetSearch%d_3",iBoardIndex),Form("TDC %d, ref-TDC %d",iBoardIndex,fiReferenceBoard),
                                                           524288,static_cast<Double_t>(trbtdc::kliFullCoarseSize/2),static_cast<Double_t>(trbtdc::kliFullCoarseSize));
    }

    // +++++++ partial event loop ++++++++++++++++++++++++++++++++++++++++++++++
    for(Long64_t lBranchEntry = 0; lBranchEntry < lInitialLoopPasses; lBranchEntry++)
    {
      tArray->Clear("C");

      tBranch->GetEntry(lBranchEntry);

      Int_t iArrayEntries = tArray->GetEntriesFast();

      if(iArrayEntries != kiNbActiveBoards)
      {
        LOG(fatal)<<"A different number of TTofTrbTdcBoard objects found in the tree than given in the parameter file.";
        return;
      }

      TTofTrbTdcBoard* tTrbTdcBoard(0);

      vector<Double_t> dTimeOffset(iArrayEntries, 0.);
      Double_t dReferenceOffset(0.);

      if(fiReferenceBoard < iArrayEntries)
      {
        tTrbTdcBoard = dynamic_cast<TTofTrbTdcBoard*>(tArray->At(fiReferenceBoard));
      }

      if(!tTrbTdcBoard)
      {
        LOG(fatal)<<Form("Reference TTofTrbTdcBoard object indexed %d not found.",fiReferenceBoard);
        continue;
      }

      const TTofTrbTdcData& tRefTdcRefTime = tTrbTdcBoard->GetRefChannelData();

      dReferenceOffset = static_cast<Double_t>(tRefTdcRefTime.GetFullCoarseTime());


      for(Int_t iBoardIndex = 0; iBoardIndex < iArrayEntries; iBoardIndex++)
      {
        if(fiReferenceBoard != iBoardIndex)
        {
          tTrbTdcBoard = dynamic_cast<TTofTrbTdcBoard*>(tArray->At(iBoardIndex));

          const TTofTrbTdcData& tTdcRefTime = tTrbTdcBoard->GetRefChannelData();

          dTimeOffset.at(iBoardIndex) = static_cast<Double_t>(tTdcRefTime.GetFullCoarseTime()) - dReferenceOffset;

          (tFirstOffsetSearch.at(iBoardIndex)).at(0)->Fill(dTimeOffset.at(iBoardIndex));
          (tFirstOffsetSearch.at(iBoardIndex)).at(1)->Fill(dTimeOffset.at(iBoardIndex));
          (tFirstOffsetSearch.at(iBoardIndex)).at(2)->Fill(dTimeOffset.at(iBoardIndex));
          (tFirstOffsetSearch.at(iBoardIndex)).at(3)->Fill(dTimeOffset.at(iBoardIndex));
        }
      }

    }
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    // +++++++ 2nd iteration +++++++++++++++++++++++++++++++++++++++++++++++++++
    // Scan each sector histogram of iteration 1 for a single occupied bin. If
    // several bins are occupied in a single histogram, exempt the TDC board
    // from calibration (probably no clock signal applied). If the last bin
    // of one sector histogram and the first bin of the neighboring sector
    // histogram are occupied, the offset distribution crosses sector borders.
    // Create a joint 2nd level histogram in this case.
    // 
    // 1 Bin = 2.6 ms/524,288 = 5 ns
    // 1,048,576 bins each (range of 5.2 ms)

    vector< vector<TH1D*> > tSecondOffsetSearch( kiNbActiveBoards, vector<TH1D*>() );

    vector<Bool_t> bDisableBoard( kiNbActiveBoards, kFALSE );

    for(Int_t iBoardIndex = 0; iBoardIndex < kiNbActiveBoards; iBoardIndex++)
    {
      Int_t iFirstOccupiedBin[4] = {-1};
      Int_t iLastOccupiedBin[4] = {-1};
      Bool_t bOffsetFound[4];

      for(Int_t iSector = 0; iSector < 4; iSector++)
      {
        bOffsetFound[iSector] = kFALSE;

        iFirstOccupiedBin[iSector] = ((tFirstOffsetSearch.at(iBoardIndex)).at(iSector))->FindFirstBinAbove(0.);
        iLastOccupiedBin[iSector] = ((tFirstOffsetSearch.at(iBoardIndex)).at(iSector))->FindLastBinAbove(0.);

        if( (iFirstOccupiedBin[iSector] > -1) && (iLastOccupiedBin[iSector] > -1) )
        {
          // Given a bin width of 2^19 clock cycles (2.6 ms) the coarse counter offset distribution
          // should not span more than two neighboring bins. If one observes a different behavior,
          // most likely the TDC did not receive any clock signal. In this case, the respective coarse counter
          // would not be incremented at all and arbitrarily jump w.r.t. the reference TDC. Owing to bad
          // grounding between the clock signal generator and a TDC, there might be a chance that a TDC
          // does from time to time jump a clock edge. If such a hypothetical jump by one edge does not
          // occur within the first 100 events of the sample, it is not taken into account here.
          if( 1 >= TMath::Abs(iFirstOccupiedBin[iSector] - iLastOccupiedBin[iSector]) )
          {
            Double_t dLowerRangeLimit = ((tFirstOffsetSearch.at(iBoardIndex)).at(iSector))->GetBinLowEdge(iFirstOccupiedBin[iSector]);
            Double_t dUpperRangeLimit = dLowerRangeLimit + 2.*((tFirstOffsetSearch.at(iBoardIndex)).at(iSector))->GetBinWidth(1);

            (tSecondOffsetSearch.at(iBoardIndex)).push_back( new TH1D(Form("tSecondOffsetSearch%d_%lu",iBoardIndex,(tSecondOffsetSearch.at(iBoardIndex)).size()),Form("TDC %d, ref-TDC %d",iBoardIndex,fiReferenceBoard),
                                                                     1048576,dLowerRangeLimit,dUpperRangeLimit) );

            bOffsetFound[iSector] = kTRUE;
            
            // Check for offset distributions crossing sector borders
            // 0/1, 1/2, 2/3
            if( 0 != iSector )
            {
              if( bOffsetFound[iSector-1] )
              {
                if( ( ((tFirstOffsetSearch.at(iBoardIndex)).at(iSector-1))->GetNbinsX() == iLastOccupiedBin[iSector-1] ) && 
                    ( 1 == iFirstOccupiedBin[iSector] ) )
                {
                  // The vector at this point contains two single histograms for a single distribution
                  // that crosses sector borders. Remove these two and replace them by an inclusive one.
                  delete (tSecondOffsetSearch.at(iBoardIndex)).back();
                  (tSecondOffsetSearch.at(iBoardIndex)).pop_back();
                  delete (tSecondOffsetSearch.at(iBoardIndex)).back();
                  (tSecondOffsetSearch.at(iBoardIndex)).pop_back();

                  dLowerRangeLimit = ((tFirstOffsetSearch.at(iBoardIndex)).at(iSector-1))->GetBinLowEdge(iLastOccupiedBin[iSector-1]);
                  dUpperRangeLimit = dLowerRangeLimit + 2.*((tFirstOffsetSearch.at(iBoardIndex)).at(iSector))->GetBinWidth(1);

                  (tSecondOffsetSearch.at(iBoardIndex)).push_back( new TH1D(Form("tSecondOffsetSearch%d_%lu",iBoardIndex,(tSecondOffsetSearch.at(iBoardIndex)).size()),Form("TDC %d, ref-TDC %d",iBoardIndex,fiReferenceBoard),
                                                                           1048576,dLowerRangeLimit,dUpperRangeLimit) );
                }
              }
            }
          }
        }
      }

      // Disable TDCs for which no (valid) offset distributions were found
      for(Int_t iSector = 0; iSector < 4; iSector++)
      {
        if( bOffsetFound[iSector] )
        {
          break;
        }

        if(3 == iSector)
        {
          bDisableBoard.at(iBoardIndex) = kTRUE;
        }
      }

    }

    // Deallocating some memory
    for(Int_t iBoardIndex = 0; iBoardIndex < kiNbActiveBoards; iBoardIndex++)
    {
      for(Int_t iSector = 0; iSector < 4; iSector++)
      {
        delete ((tFirstOffsetSearch.at(iBoardIndex)).at(iSector));
      }
    }

    // +++++++ partial event loop ++++++++++++++++++++++++++++++++++++++++++++++
    for(Long64_t lBranchEntry = 0; lBranchEntry < lInitialLoopPasses; lBranchEntry++)
    {
      tArray->Clear("C");

      tBranch->GetEntry(lBranchEntry);

      Int_t iArrayEntries = tArray->GetEntriesFast();

      TTofTrbTdcBoard* tTrbTdcBoard(0);

      vector<Double_t> dTimeOffset(iArrayEntries, 0.);
      Double_t dReferenceOffset(0.);

      if(fiReferenceBoard < iArrayEntries)
      {
        tTrbTdcBoard = dynamic_cast<TTofTrbTdcBoard*>(tArray->At(fiReferenceBoard));
      }

      const TTofTrbTdcData& tRefTdcRefTime = tTrbTdcBoard->GetRefChannelData();

      dReferenceOffset = static_cast<Double_t>(tRefTdcRefTime.GetFullCoarseTime());


      for(Int_t iBoardIndex = 0; iBoardIndex < iArrayEntries; iBoardIndex++)
      {
        if(fiReferenceBoard != iBoardIndex)
        {
          tTrbTdcBoard = dynamic_cast<TTofTrbTdcBoard*>(tArray->At(iBoardIndex));

          const TTofTrbTdcData& tTdcRefTime = tTrbTdcBoard->GetRefChannelData();

          dTimeOffset.at(iBoardIndex) = static_cast<Double_t>(tTdcRefTime.GetFullCoarseTime()) - dReferenceOffset;

          for(Int_t iRange = 0; iRange < static_cast<Int_t>((tSecondOffsetSearch.at(iBoardIndex)).size()); iRange++)
          {
            ((tSecondOffsetSearch.at(iBoardIndex)).at(iRange))->Fill(dTimeOffset.at(iBoardIndex));
          }

        }
      }
    }
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // +++++++ 3rd iteration +++++++++++++++++++++++++++++++++++++++++++++++++++
    // Create from each 2nd level histogram a new histogram with final binning.
    // Create in addition the complementary partner histogram (shifted by +/-
    // the full coarse counter range) to determine among the 3rd level histograms
    // a reference range (closest to 0) for correcting and offset fitting.
    // 1 Bin = 5ns/500 = 10 ps
    // 1500 bins each (range of 15 ns)

    vector< vector<TH1D*> > tThirdOffsetSearch( kiNbActiveBoards, vector<TH1D*>() );

    vector<Int_t> iMainOffsetHisto( kiNbActiveBoards, -1 );
    vector<Double_t> dPreliminaryOffset( kiNbActiveBoards, 0. );

    for(Int_t iBoardIndex = 0; iBoardIndex < kiNbActiveBoards; iBoardIndex++)
    {
      if( (tSecondOffsetSearch.at(iBoardIndex)).size() )
      {
        vector<Int_t> iFirstOccupiedBin( static_cast<Int_t>((tSecondOffsetSearch.at(iBoardIndex)).size()) );
        vector<Int_t> iLastOccupiedBin( static_cast<Int_t>((tSecondOffsetSearch.at(iBoardIndex)).size()) );

        for(Int_t iRange = 0; iRange < static_cast<Int_t>((tSecondOffsetSearch.at(iBoardIndex)).size()); iRange++)
        {
          iFirstOccupiedBin.at(iRange) = ((tSecondOffsetSearch.at(iBoardIndex)).at(iRange))->FindFirstBinAbove(0.);
          iLastOccupiedBin.at(iRange) = ((tSecondOffsetSearch.at(iBoardIndex)).at(iRange))->FindLastBinAbove(0.);

          // Given a bin width of 1 clock cycle (5 ns) the coarse counter offset distribution
          // should not span more than two neighboring bins.
          // Also, we need to take into account that the fine counter offset distribution may
          // extend to neighboring bins.

          // Assume the coarse counter offset distribution (100 events) spans one bin only.
          // The actual offset distribution might, however, cross the border to the left or
          // right bin neighbor, respectively. Create 3rd level histograms with
          // three 2nd level bins.
          if( (0 == TMath::Abs(iFirstOccupiedBin.at(iRange) - iLastOccupiedBin.at(iRange))) || (1 == TMath::Abs(iFirstOccupiedBin.at(iRange) - iLastOccupiedBin.at(iRange))) )
          {
              Double_t dLowerRangeLimit = ((tSecondOffsetSearch.at(iBoardIndex)).at(iRange))->GetBinLowEdge(iFirstOccupiedBin.at(iRange)-1);
              Double_t dUpperRangeLimit = dLowerRangeLimit + 3.*((tSecondOffsetSearch.at(iBoardIndex)).at(iRange))->GetBinWidth(1);

              (tThirdOffsetSearch.at(iBoardIndex)).push_back( new TH1D(Form("tThirdOffsetSearch%d_%lu",iBoardIndex,(tThirdOffsetSearch.at(iBoardIndex)).size()),Form("TDC %d, ref-TDC %d",iBoardIndex,fiReferenceBoard),
                                                                      1500,dLowerRangeLimit,dUpperRangeLimit) );
          }
          else
          {
            bDisableBoard.at(iBoardIndex) = kTRUE;
          }
        }

        // The complementary offset distribution was not found. Nevertheless,
        // we create a corresponding histogram in case offsets change sign in
        // the course of the calibration loop.
        if( 1 == (tThirdOffsetSearch.at(iBoardIndex)).size() )
        {
          Double_t dLowerRangeLimit = ((tThirdOffsetSearch.at(iBoardIndex)).at(0))->GetBinLowEdge(1);
          Double_t dUpperRangeLimit = ((tThirdOffsetSearch.at(iBoardIndex)).at(0))->GetBinLowEdge( ((tThirdOffsetSearch.at(iBoardIndex)).at(0))->GetNbinsX()+1 );

          // Found an offset distribution in the positive sectors.
          // Create the complementary distribution in the negative sectors.
          if( 0. <= dLowerRangeLimit &&  0. < dUpperRangeLimit )
          {
            dLowerRangeLimit -= static_cast<Double_t>(trbtdc::kliFullCoarseSize);
            dUpperRangeLimit -= static_cast<Double_t>(trbtdc::kliFullCoarseSize);

            (tThirdOffsetSearch.at(iBoardIndex)).push_back( new TH1D(Form("tThirdOffsetSearch%d_%lu",iBoardIndex,(tThirdOffsetSearch.at(iBoardIndex)).size()),Form("TDC %d, ref-TDC %d",iBoardIndex,fiReferenceBoard),
                                                                     1500,dLowerRangeLimit,dUpperRangeLimit) );
          }
          // Found an offset distribution in the negative sectors.
          // Create the complementary distribution in the positive sectors.
          else if( 0. > dLowerRangeLimit && 0. >= dUpperRangeLimit)
          {
            dLowerRangeLimit += static_cast<Double_t>(trbtdc::kliFullCoarseSize);
            dUpperRangeLimit += static_cast<Double_t>(trbtdc::kliFullCoarseSize);

            (tThirdOffsetSearch.at(iBoardIndex)).push_back( new TH1D(Form("tThirdOffsetSearch%d_%lu",iBoardIndex,(tThirdOffsetSearch.at(iBoardIndex)).size()),Form("TDC %d, ref-TDC %d",iBoardIndex,fiReferenceBoard),
                                                                     1500,dLowerRangeLimit,dUpperRangeLimit) );
          }
          // The offset distribution has a negative and a positive contribution.
          // Create a negative and a positive complement (3 histograms in total).
          else
          {
            (tThirdOffsetSearch.at(iBoardIndex)).push_back( new TH1D(Form("tThirdOffsetSearch%d_%lu",iBoardIndex,(tThirdOffsetSearch.at(iBoardIndex)).size()),Form("TDC %d, ref-TDC %d",iBoardIndex,fiReferenceBoard),
                                                                     1500,dLowerRangeLimit-static_cast<Double_t>(trbtdc::kliFullCoarseSize),dUpperRangeLimit-static_cast<Double_t>(trbtdc::kliFullCoarseSize)) );

            (tThirdOffsetSearch.at(iBoardIndex)).push_back( new TH1D(Form("tThirdOffsetSearch%d_%lu",iBoardIndex,(tThirdOffsetSearch.at(iBoardIndex)).size()),Form("TDC %d, ref-TDC %d",iBoardIndex,fiReferenceBoard),
                                                                     1500,dLowerRangeLimit+static_cast<Double_t>(trbtdc::kliFullCoarseSize),dUpperRangeLimit+static_cast<Double_t>(trbtdc::kliFullCoarseSize)) );
          }
        }

        // Determine the element in tThirdOffsetSearch which axis range is
        // closest to 0. Entries in the complementary histograms should be
        // corrected (+/- coarse counter range) w.r.t. the offset obtained
        // from this histogram. In case of two histograms centered about
        // +/- half the coarse counter range choose anyone.

        Double_t dTempOffset = 2.*static_cast<Double_t>(trbtdc::kliFullCoarseSize);

        for(Int_t iElement = 0; iElement < static_cast<Int_t>((tThirdOffsetSearch.at(iBoardIndex)).size()); iElement++)
        {
          Double_t dLowerRangeLimit = ((tThirdOffsetSearch.at(iBoardIndex)).at(iElement))->GetBinLowEdge(1);
          Double_t dUpperRangeLimit = ((tThirdOffsetSearch.at(iBoardIndex)).at(iElement))->GetBinLowEdge( ((tThirdOffsetSearch.at(iBoardIndex)).at(iElement))->GetNbinsX()+1 );

          Double_t dMeanRange = (dLowerRangeLimit + dUpperRangeLimit)/2.;

          if( TMath::Abs(dTempOffset) > TMath::Abs(dMeanRange) )
          {
            iMainOffsetHisto.at(iBoardIndex) = iElement;
            dPreliminaryOffset.at(iBoardIndex) = dMeanRange;

            dTempOffset = dMeanRange;
          }
        }

      }
    }

    // Deallocating some memory
    for(Int_t iBoardIndex = 0; iBoardIndex < kiNbActiveBoards; iBoardIndex++)
    {
      for(Int_t iRange = 0; iRange < static_cast<Int_t>((tSecondOffsetSearch.at(iBoardIndex)).size()); iRange++)
      {
        delete ((tSecondOffsetSearch.at(iBoardIndex)).at(iRange));
      }
    }
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // Offset fitting histograms
    vector<TH1D*> tOffsetsLinear( kiNbActiveBoards );
    vector<TH1D*> tOffsetsBinCenter( kiNbActiveBoards );
    vector<TH1D*> tOffsetsBinEdge( kiNbActiveBoards );

    for(Int_t iBoardIndex = 0; iBoardIndex < kiNbActiveBoards; iBoardIndex++)
    {
      if( !bDisableBoard.at(iBoardIndex) )
      {
        Double_t dLowerRangeLimit = ((tThirdOffsetSearch.at(iBoardIndex)).at(iMainOffsetHisto.at(iBoardIndex)))->GetBinLowEdge(1);
        dLowerRangeLimit -= dPreliminaryOffset.at(iBoardIndex);
        Double_t dUpperRangeLimit = ((tThirdOffsetSearch.at(iBoardIndex)).at(iMainOffsetHisto.at(iBoardIndex)))->GetBinLowEdge( ((tThirdOffsetSearch.at(iBoardIndex)).at(iMainOffsetHisto.at(iBoardIndex)))->GetNbinsX()+1 );
        dUpperRangeLimit -= dPreliminaryOffset.at(iBoardIndex);

        tOffsetsLinear.at(iBoardIndex) = new TH1D(Form("tOffsetsLinear%d",iBoardIndex),Form("TDC %d, ref-TDC %d",iBoardIndex,fiReferenceBoard),
                                                  1500,dLowerRangeLimit,dUpperRangeLimit);
        tOffsetsLinear.at(iBoardIndex)->GetXaxis()->SetTitle("linear ref diff [ps]");

        tOffsetsBinCenter.at(iBoardIndex) = new TH1D(Form("tOffsetsBinCenter%d",iBoardIndex),Form("TDC %d, ref-TDC %d",iBoardIndex,fiReferenceBoard),
                                                     1500,dLowerRangeLimit,dUpperRangeLimit);
        tOffsetsBinCenter.at(iBoardIndex)->GetXaxis()->SetTitle("bin center ref diff [5 ns]");

        tOffsetsBinEdge.at(iBoardIndex) = new TH1D(Form("tOffsetsBinEdge%d",iBoardIndex),Form("TDC %d, ref-TDC %d",iBoardIndex,fiReferenceBoard),
                                                   1500,dLowerRangeLimit,dUpperRangeLimit);
        tOffsetsBinEdge.at(iBoardIndex)->GetXaxis()->SetTitle("bin edge ref diff [5 ns]");
      }
    }


    LOG(info)<<"Calibrating the TDC to TDC offsets...";

    // Loop to fill the offset fitting histograms
    for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)
    {
      if(0 == lBranchEntry % 10000)
      {
        LOG(info)<<Form("event: %8lld  of %8lld",lBranchEntry,lBranchEntries);
      }

      tArray->Clear("C");

      tBranch->GetEntry(lBranchEntry);

      Int_t iArrayEntries = tArray->GetEntriesFast();

      TTofTrbTdcBoard* tTrbTdcBoard(0);

      Double_t dReferenceOffsetLinear(0.);
      Double_t dReferenceOffsetBinCenter(0.);
      Double_t dReferenceOffsetBinEdge(0.);

      if(fiReferenceBoard < iArrayEntries)
      {
        tTrbTdcBoard = dynamic_cast<TTofTrbTdcBoard*>(tArray->At(fiReferenceBoard));
      }

      const TTofTrbTdcData& tRefTdcRefTime = tTrbTdcBoard->GetRefChannelData();

      Double_t dRefCoarseTime = static_cast<Double_t>(tRefTdcRefTime.GetFullCoarseTime());

      if(0 == lBranchEntry)
      {
        LOG(info)<<Form("reference TDC %d first time: %.2f s",fiReferenceBoard,static_cast<Double_t>(tRefTdcRefTime.GetFullCoarseTime())*trbtdc::kdClockCycleSizeSec);
      }
      else if(lBranchEntries-1 == lBranchEntry)
      {
        LOG(info)<<Form("reference TDC %d last time: %.2f s",fiReferenceBoard,static_cast<Double_t>(tRefTdcRefTime.GetFullCoarseTime())*trbtdc::kdClockCycleSizeSec);
      }
      else if(0 == lBranchEntry % 10000)
      {
        LOG(info)<<Form("  TDC %d reference time: %.2f s",fiReferenceBoard,static_cast<Double_t>(tRefTdcRefTime.GetFullCoarseTime())*trbtdc::kdClockCycleSizeSec);
      }

      dReferenceOffsetLinear   = dRefCoarseTime - (dRefFineTimeLinear.at(fiReferenceBoard)).at(static_cast<Int_t>(tRefTdcRefTime.GetFineTime()));
      dReferenceOffsetBinCenter  = dRefCoarseTime - (dRefFineTimeBinCenter.at(fiReferenceBoard)).at(static_cast<Int_t>(tRefTdcRefTime.GetFineTime()));
      dReferenceOffsetBinEdge = dRefCoarseTime - (dRefFineTimeBinEdge.at(fiReferenceBoard)).at(static_cast<Int_t>(tRefTdcRefTime.GetFineTime()));

      for(Int_t iBoardIndex = 0; iBoardIndex < iArrayEntries; iBoardIndex++)
      {
        if( (fiReferenceBoard != iBoardIndex) && !bDisableBoard.at(iBoardIndex) )
        {
          tTrbTdcBoard = dynamic_cast<TTofTrbTdcBoard*>(tArray->At(iBoardIndex));

          const TTofTrbTdcData& tTdcRefTime = tTrbTdcBoard->GetRefChannelData();

          Double_t dCoarseTime = static_cast<Double_t>(tTdcRefTime.GetFullCoarseTime());

          Double_t dTimeOffsetLinear   = dCoarseTime - (dRefFineTimeLinear.at(iBoardIndex)).at(static_cast<Int_t>(tTdcRefTime.GetFineTime()))   - dReferenceOffsetLinear;
          Double_t dTimeOffsetBinCenter  = dCoarseTime - (dRefFineTimeBinCenter.at(iBoardIndex)).at(static_cast<Int_t>(tTdcRefTime.GetFineTime()))  - dReferenceOffsetBinCenter;
          Double_t dTimeOffsetBinEdge = dCoarseTime - (dRefFineTimeBinEdge.at(iBoardIndex)).at(static_cast<Int_t>(tTdcRefTime.GetFineTime())) - dReferenceOffsetBinEdge;


          Double_t dTimeOffset(0.);

          if( trbtdc::finetime_bincenter == fiFineTimeMethod )
          {
            dTimeOffset = dTimeOffsetBinCenter;
          }
          else if( trbtdc::finetime_binedge == fiFineTimeMethod )
          {
            dTimeOffset = dTimeOffsetBinEdge;
          }
          else
          {
            dTimeOffset = dTimeOffsetLinear;
          }

          for(Int_t iRange = 0; iRange < static_cast<Int_t>((tThirdOffsetSearch.at(iBoardIndex)).size()); iRange++)
          {
            ((tThirdOffsetSearch.at(iBoardIndex)).at(iRange))->Fill(dTimeOffset);
          }


          if( (dTimeOffsetLinear - dPreliminaryOffset.at(iBoardIndex)) >  static_cast<Double_t>(trbtdc::kliFullCoarseSize)/4. )
          {
            dTimeOffsetLinear -= static_cast<Double_t>(trbtdc::kliFullCoarseSize);
          }
          else if( (dTimeOffsetLinear - dPreliminaryOffset.at(iBoardIndex)) <  -static_cast<Double_t>(trbtdc::kliFullCoarseSize)/4. )
          {
            dTimeOffsetLinear += static_cast<Double_t>(trbtdc::kliFullCoarseSize);
          }

          if( (dTimeOffsetBinCenter - dPreliminaryOffset.at(iBoardIndex)) >  static_cast<Double_t>(trbtdc::kliFullCoarseSize)/4. )
          {
            dTimeOffsetBinCenter -= static_cast<Double_t>(trbtdc::kliFullCoarseSize);
          }
          else if( (dTimeOffsetBinCenter - dPreliminaryOffset.at(iBoardIndex)) <  -static_cast<Double_t>(trbtdc::kliFullCoarseSize)/4. )
          {
            dTimeOffsetBinCenter += static_cast<Double_t>(trbtdc::kliFullCoarseSize);
          }

          if( (dTimeOffsetBinEdge - dPreliminaryOffset.at(iBoardIndex)) >  static_cast<Double_t>(trbtdc::kliFullCoarseSize)/4. )
          {
            dTimeOffsetBinEdge -= static_cast<Double_t>(trbtdc::kliFullCoarseSize);
          }
          else if( (dTimeOffsetBinEdge - dPreliminaryOffset.at(iBoardIndex)) <  -static_cast<Double_t>(trbtdc::kliFullCoarseSize)/4. )
          {
            dTimeOffsetBinEdge += static_cast<Double_t>(trbtdc::kliFullCoarseSize);
          }

          tOffsetsLinear.at(iBoardIndex)->Fill(dTimeOffsetLinear - dPreliminaryOffset.at(iBoardIndex));
          tOffsetsBinCenter.at(iBoardIndex)->Fill(dTimeOffsetBinCenter - dPreliminaryOffset.at(iBoardIndex));
          tOffsetsBinEdge.at(iBoardIndex)->Fill(dTimeOffsetBinEdge - dPreliminaryOffset.at(iBoardIndex));

        }
      }

    }

    vector<Double_t> dFinalOffset( kiNbActiveBoards, 0. );

    vector<Double_t> dRmsLinear( kiNbActiveBoards, 0. );
    vector<Double_t> dSigmaLinear( kiNbActiveBoards, 0. );
    vector<Double_t> dRmsBinCenter( kiNbActiveBoards, 0. );
    vector<Double_t> dSigmaBinCenter( kiNbActiveBoards, 0. );
    vector<Double_t> dRmsBinEdge( kiNbActiveBoards, 0. );
    vector<Double_t> dSigmaBinEdge( kiNbActiveBoards, 0. );

    vector<Double_t> dHistoMeanLinear( kiNbActiveBoards, 0. );
    vector<Double_t> dGausMeanLinear( kiNbActiveBoards, 0. );
    vector<Double_t> dHistoMeanBinCenter( kiNbActiveBoards, 0. );
    vector<Double_t> dGausMeanBinCenter( kiNbActiveBoards, 0. );
    vector<Double_t> dHistoMeanBinEdge( kiNbActiveBoards, 0. );
    vector<Double_t> dGausMeanBinEdge( kiNbActiveBoards, 0. );

    vector<Double_t> dHistoIndizes( kiNbActiveBoards, 0. );

    for(Int_t iBoardIndex = 0; iBoardIndex < kiNbActiveBoards; iBoardIndex++)
    {
      if( !bDisableBoard.at(iBoardIndex) )
      {
        tOffsetsLinear.at(iBoardIndex)->GetXaxis()->SetRangeUser(tOffsetsLinear.at(iBoardIndex)->GetMean()-0.3,tOffsetsLinear.at(iBoardIndex)->GetMean()+0.3);
        dRmsLinear.at(iBoardIndex) = trbtdc::kdClockCycleSize*tOffsetsLinear.at(iBoardIndex)->GetRMS();
        dHistoMeanLinear.at(iBoardIndex) = TMath::Abs(tOffsetsLinear.at(iBoardIndex)->GetMean() + dPreliminaryOffset.at(iBoardIndex))+1.;

        if( trbtdc::finetime_linear == fiFineTimeMethod )
        {
          dFinalOffset.at(iBoardIndex) = tOffsetsLinear.at(iBoardIndex)->GetMean() + dPreliminaryOffset.at(iBoardIndex);
        }

        // from TH1.cxx:
        // "Q" Quiet mode (minimum printing)
        // "0" Do not plot the result of the fit. By default the fitted function is drawn unless the option"N" above is specified.
        if(static_cast<Int_t>(tOffsetsLinear.at(iBoardIndex)->Fit("gaus","Q0")))
        {
          LOG(error)<<Form("Linear FT interpolation: Gaussian fit failed for TDC %d.",iBoardIndex);
        }
        else
        {
          dSigmaLinear.at(iBoardIndex) = trbtdc::kdClockCycleSize*(tOffsetsLinear.at(iBoardIndex)->GetFunction("gaus"))->GetParameter("Sigma");
          dGausMeanLinear.at(iBoardIndex) = TMath::Abs((tOffsetsLinear.at(iBoardIndex)->GetFunction("gaus"))->GetParameter("Mean") + dPreliminaryOffset.at(iBoardIndex))+1.;

          if( trbtdc::finetime_linear == fiFineTimeMethod )
          {
            dFinalOffset.at(iBoardIndex) = (tOffsetsLinear.at(iBoardIndex)->GetFunction("gaus"))->GetParameter("Mean") + dPreliminaryOffset.at(iBoardIndex);
          }
        }

        tOffsetsBinCenter.at(iBoardIndex)->GetXaxis()->SetRangeUser(tOffsetsBinCenter.at(iBoardIndex)->GetMean()-0.3,tOffsetsBinCenter.at(iBoardIndex)->GetMean()+0.3);
        dRmsBinCenter.at(iBoardIndex) = trbtdc::kdClockCycleSize*tOffsetsBinCenter.at(iBoardIndex)->GetRMS();
        dHistoMeanBinCenter.at(iBoardIndex) = TMath::Abs(tOffsetsBinCenter.at(iBoardIndex)->GetMean() + dPreliminaryOffset.at(iBoardIndex))+1.;

        if( trbtdc::finetime_bincenter == fiFineTimeMethod )
        {
          dFinalOffset.at(iBoardIndex) = tOffsetsBinCenter.at(iBoardIndex)->GetMean() + dPreliminaryOffset.at(iBoardIndex);
        }

        if(static_cast<Int_t>(tOffsetsBinCenter.at(iBoardIndex)->Fit("gaus","Q0")))
        {
          LOG(error)<<Form("Bin center FT interpolation: Gaussian fit failed for TDC %d.",iBoardIndex);
        }
        else
        {
          dSigmaBinCenter.at(iBoardIndex) = trbtdc::kdClockCycleSize*(tOffsetsBinCenter.at(iBoardIndex)->GetFunction("gaus"))->GetParameter("Sigma");
          dGausMeanBinCenter.at(iBoardIndex) = TMath::Abs((tOffsetsBinCenter.at(iBoardIndex)->GetFunction("gaus"))->GetParameter("Mean") + dPreliminaryOffset.at(iBoardIndex))+1.;

          if( trbtdc::finetime_bincenter == fiFineTimeMethod )
          {
            dFinalOffset.at(iBoardIndex) = (tOffsetsBinCenter.at(iBoardIndex)->GetFunction("gaus"))->GetParameter("Mean") + dPreliminaryOffset.at(iBoardIndex);
          }

        }

        tOffsetsBinEdge.at(iBoardIndex)->GetXaxis()->SetRangeUser(tOffsetsBinEdge.at(iBoardIndex)->GetMean()-0.3,tOffsetsBinEdge.at(iBoardIndex)->GetMean()+0.3);
        dRmsBinEdge.at(iBoardIndex) = trbtdc::kdClockCycleSize*tOffsetsBinEdge.at(iBoardIndex)->GetRMS();
        dHistoMeanBinEdge.at(iBoardIndex) = TMath::Abs(tOffsetsBinEdge.at(iBoardIndex)->GetMean() + dPreliminaryOffset.at(iBoardIndex))+1.;

        if( trbtdc::finetime_binedge == fiFineTimeMethod )
        {
          dFinalOffset.at(iBoardIndex) = tOffsetsBinEdge.at(iBoardIndex)->GetMean() + dPreliminaryOffset.at(iBoardIndex);
        }

        if(static_cast<Int_t>(tOffsetsBinEdge.at(iBoardIndex)->Fit("gaus","Q0")))
        {
          LOG(error)<<Form("Bin edge FT interpolation: Gaussian fit failed for TDC %d.",iBoardIndex);
        }
        else
        {
          dSigmaBinEdge.at(iBoardIndex) = trbtdc::kdClockCycleSize*(tOffsetsBinEdge.at(iBoardIndex)->GetFunction("gaus"))->GetParameter("Sigma");
          dGausMeanBinEdge.at(iBoardIndex) = TMath::Abs((tOffsetsBinEdge.at(iBoardIndex)->GetFunction("gaus"))->GetParameter("Mean") + dPreliminaryOffset.at(iBoardIndex))+1.;

          if( trbtdc::finetime_binedge == fiFineTimeMethod )
          {
            dFinalOffset.at(iBoardIndex) = (tOffsetsBinEdge.at(iBoardIndex)->GetFunction("gaus"))->GetParameter("Mean") + dPreliminaryOffset.at(iBoardIndex);
          }

        }

        dHistoIndizes.at(iBoardIndex) = static_cast<Double_t>(iBoardIndex);

      }

    }

    // The TGraph constructor requires a pointer of type Double_t to the C arrays
    // to plot. The vector content can be addressed in an array-style fashion by a
    // Double_t pointer because vectors store their elements contiguously.

    TGraph* tGraphRmsLinear = new TGraph(kiNbActiveBoards,static_cast<Double_t*>(&dHistoIndizes.at(0)),static_cast<Double_t*>(&dRmsLinear.at(0)));
    TGraph* tGraphSigmaLinear = new TGraph(kiNbActiveBoards,static_cast<Double_t*>(&dHistoIndizes.at(0)),static_cast<Double_t*>(&dSigmaLinear.at(0)));
    TGraph* tGraphRmsBinCenter = new TGraph(kiNbActiveBoards,static_cast<Double_t*>(&dHistoIndizes.at(0)),static_cast<Double_t*>(&dRmsBinCenter.at(0)));
    TGraph* tGraphSigmaBinCenter = new TGraph(kiNbActiveBoards,static_cast<Double_t*>(&dHistoIndizes.at(0)),static_cast<Double_t*>(&dSigmaBinCenter.at(0)));
    TGraph* tGraphRmsBinEdge = new TGraph(kiNbActiveBoards,static_cast<Double_t*>(&dHistoIndizes.at(0)),static_cast<Double_t*>(&dRmsBinEdge.at(0)));
    TGraph* tGraphSigmaBinEdge = new TGraph(kiNbActiveBoards,static_cast<Double_t*>(&dHistoIndizes.at(0)),static_cast<Double_t*>(&dSigmaBinEdge.at(0)));

    tGraphRmsLinear->SetName("tGraphRmsLinear");
    tGraphSigmaLinear->SetName("tGraphSigmaLinear");
    tGraphRmsBinCenter->SetName("tGraphRmsBinCenter");
    tGraphSigmaBinCenter->SetName("tGraphSigmaBinCenter");
    tGraphRmsBinEdge->SetName("tGraphRmsBinEdge");
    tGraphSigmaBinEdge->SetName("tGraphSigmaBinEdge");

    tGraphRmsLinear->SetTitle(Form("Reference time differences RMS to TDC %d",fiReferenceBoard));
    tGraphSigmaLinear->SetTitle(Form("Reference time differences Sigma to TDC %d",fiReferenceBoard));
    tGraphRmsBinCenter->SetTitle(Form("Reference time differences RMS to TDC %d",fiReferenceBoard));
    tGraphSigmaBinCenter->SetTitle(Form("Reference time differences Sigma to TDC %d",fiReferenceBoard));
    tGraphRmsBinEdge->SetTitle(Form("Reference time differences RMS to TDC %d",fiReferenceBoard));
    tGraphSigmaBinEdge->SetTitle(Form("Reference time differences Sigma to TDC %d",fiReferenceBoard));


    gDirectory->GetList()->Add(tGraphRmsLinear);
    gDirectory->GetList()->Add(tGraphSigmaLinear);
    gDirectory->GetList()->Add(tGraphRmsBinCenter);
    gDirectory->GetList()->Add(tGraphSigmaBinCenter);
    gDirectory->GetList()->Add(tGraphRmsBinEdge);
    gDirectory->GetList()->Add(tGraphSigmaBinEdge);


    TGraph* tGraphHistoMeanLinear = new TGraph(kiNbActiveBoards,static_cast<Double_t*>(&dHistoIndizes.at(0)),static_cast<Double_t*>(&dHistoMeanLinear.at(0)));
    TGraph* tGraphGausMeanLinear = new TGraph(kiNbActiveBoards,static_cast<Double_t*>(&dHistoIndizes.at(0)),static_cast<Double_t*>(&dGausMeanLinear.at(0)));
    TGraph* tGraphHistoMeanBinCenter = new TGraph(kiNbActiveBoards,static_cast<Double_t*>(&dHistoIndizes.at(0)),static_cast<Double_t*>(&dHistoMeanBinCenter.at(0)));
    TGraph* tGraphGausMeanBinCenter = new TGraph(kiNbActiveBoards,static_cast<Double_t*>(&dHistoIndizes.at(0)),static_cast<Double_t*>(&dGausMeanBinCenter.at(0)));
    TGraph* tGraphHistoMeanBinEdge = new TGraph(kiNbActiveBoards,static_cast<Double_t*>(&dHistoIndizes.at(0)),static_cast<Double_t*>(&dHistoMeanBinEdge.at(0)));
    TGraph* tGraphGausMeanBinEdge = new TGraph(kiNbActiveBoards,static_cast<Double_t*>(&dHistoIndizes.at(0)),static_cast<Double_t*>(&dGausMeanBinEdge.at(0)));

    tGraphHistoMeanLinear->SetName("tGraphHistoMeanLinear");
    tGraphGausMeanLinear->SetName("tGraphGausMeanLinear");
    tGraphHistoMeanBinCenter->SetName("tGraphHistoMeanBinCenter");
    tGraphGausMeanBinCenter->SetName("tGraphGausMeanBinCenter");
    tGraphHistoMeanBinEdge->SetName("tGraphHistoMeanBinEdge");
    tGraphGausMeanBinEdge->SetName("tGraphGausMeanBinEdge");

    tGraphHistoMeanLinear->SetTitle(Form("Reference time differences h-Mean to TDC %d",fiReferenceBoard));
    tGraphGausMeanLinear->SetTitle(Form("Reference time differences g-Mean to TDC %d",fiReferenceBoard));
    tGraphHistoMeanBinCenter->SetTitle(Form("Reference time differences h-Mean to TDC %d",fiReferenceBoard));
    tGraphGausMeanBinCenter->SetTitle(Form("Reference time differences g-Mean to TDC %d",fiReferenceBoard));
    tGraphHistoMeanBinEdge->SetTitle(Form("Reference time differences h-Mean to TDC %d",fiReferenceBoard));
    tGraphGausMeanBinEdge->SetTitle(Form("Reference time differences g-Mean to TDC %d",fiReferenceBoard));

    gDirectory->GetList()->Add(tGraphHistoMeanLinear);
    gDirectory->GetList()->Add(tGraphGausMeanLinear);
    gDirectory->GetList()->Add(tGraphHistoMeanBinCenter);
    gDirectory->GetList()->Add(tGraphGausMeanBinCenter);
    gDirectory->GetList()->Add(tGraphHistoMeanBinEdge);
    gDirectory->GetList()->Add(tGraphGausMeanBinEdge);

    tBranch->SetAddress(vOldAddress);

    tArray->Clear("C");
    tArray = NULL;

    tOutputFile->cd("TofTrbCalib");

    // Save information about the selected reference board, disabled boards
    // and the board offsets w.r.t. the reference board in the output file
    // of FairRootManager.
    // Instead of dumping these "parameters" to the output ROOT file one could
    // also utilize FairRuntimeDb by extending TMbsCalibTofPar by corresponding
    // attributes, generating an updated parameter ROOT file when the calibration
    // parameters have been created and delivering this file as "second input"
    // to the digi production run. At this moment, I am not willing to invest
    // this amount of effort, though.
    vector<Int_t> iReferenceBoard(1,fiReferenceBoard);
    vector<UInt_t> uGlobalFineTimeLimits(2,0);
    uGlobalFineTimeLimits.at(0) = fuLowerLinearFineLimit;
    uGlobalFineTimeLimits.at(1) = fuUpperLinearFineLimit;

    gDirectory->WriteObjectAny(&iReferenceBoard,"vector<Int_t>","iReferenceBoard");
    gDirectory->WriteObjectAny(&uGlobalFineTimeLimits,"vector<UInt_t>","uGlobalFineTimeLimits");
    gDirectory->WriteObjectAny(&bDisableBoard,"vector<Bool_t>","bDisableBoard");
    gDirectory->WriteObjectAny(&dFinalOffset,"vector<Double_t>","dFinalOffset");

/*
    TVectorD* tDisableBoard = new TVectorD(bDisableBoard.size(),static_cast<Double_t*>(&bDisableBoard.at(0)));
    TVectorD* tFinalOffset = new TVectorD(dFinalOffset.size(),static_cast<Double_t*>(&dFinalOffset.at(0)));

    tOutputTree->GetUserInfo()->Add(tDisableBoard);
    tOutputTree->GetUserInfo()->Add(tFinalOffset);
*/

    tOldDirectory->cd();

    fBufferFile->Write();
    fBufferFile->Close();
    delete fBufferFile;

  }
}

ClassImp(TTofTrbTdcCalib)

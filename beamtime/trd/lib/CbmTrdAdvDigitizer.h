/*
 * CbmTrdAdvDigitizer.h
 *
 *  Created on: Apr 22, 2017
 *      Author: Philipp Munkes
 *
 * Purpose: This Class is intended to provide digitization for CBM TRD Testbeamdata.
 *      This digitization is parametrised via the global CbmTrdTestbeamtools
 *      instance. The output is in the form of CbmTrdDigis. The Design is based on
 *      boost::MSM and should eventually support the disentanglement of Multi-Hits.
 *      The chosen front-end is the functor interface.
 *      Currently the State machine does most of its work in the entry and exit
 *      actions and doesn't implement any separate actions executed during the
 *      transition. Filtering for different processing paths is implemented
 *      within guard actions. Usually the work between entry and exit action
 *      is separated such, that the entry action performs the setup, while the
 *      exit action does the actual work and the cleanup.
 */

#ifndef FLES_READER_TASKS_CBMTRDADVDIGITIZER_H_
#define FLES_READER_TASKS_CBMTRDADVDIGITIZER_H_

#include <CbmTrdQABase.h>
#include <set>
// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/euml/common.hpp>
// for And_ operator
#include <boost/msm/front/euml/operator.hpp>
// for func_state and func_state_machine
#include <boost/msm/front/euml/state_grammar.hpp>
//Circular Buffer for Baseline Estimation
#include <boost/circular_buffer.hpp>
#include <cstdint>
#include <algorithm>
#include <array>
#include <cmath>
#include "CbmTrdDigi.h"
#include <TClonesArray.h>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// for And_ operator
using namespace msm::front::euml;

class CbmTrdAdvDigitizer : public CbmTrdQABase
{
  public:
    CbmTrdAdvDigitizer ();
    InitStatus
    Init ();
    void
    CreateHistograms ();
    void
    Exec (Option_t*);
    virtual
    ~CbmTrdAdvDigitizer ();

    //Define the Statemachine
    //First define the events
    //These events should only transport information
    //for the guards and the different states
    struct NewMessage
    {
        NewMessage (Int_t Idx)
            : fIndex (Idx)
        {/*
         * The argument given is the index of the next Spadic raw message to be processed.
         * The No filtering on this is performed, but it is encouraged to only
         * iterate over a time sorted series of Indices, at least there should not
         * be any time regressions on a channel.
         */
        }
        ;
        Int_t fIndex;
    };
    struct PrepareDigi
    {
    };
    struct CreateDigi
    {
    };
    struct SetArrays
    {
        SetArrays (TClonesArray*Raw, TClonesArray*Digis)
            : fRaw (Raw), fDigis (Digis)
        {/*
         * Set the CbmSpadicRawMessage and CbmTrdDigi TClonesArrays.
         */
        }
        ;
        TClonesArray*fRaw;
        TClonesArray*fDigis;
    };

    struct CbmTrdDigiFinder_ : public msm::front::state_machine_def<
        CbmTrdDigiFinder_>
    {

        struct UninitializedEntry
        {/*
         * This is the initial state of the digifinder. The Digifinder is reinitilized for each timeslice,
         * to mimic later operation procedures. Nothing is set yet and both the input and output arrays need to be provided.
         */
            template<class Event, class FSM, class STATE>
              void
              operator() (Event const&, FSM&SM, STATE&)
              {
                LOG(debug)
                              << "CbmTrdAdvDigitizer: Initializing Digitizer State Machine";
                SM.fBT = CbmTrdTestBeamTools::Instance (nullptr);
              }
        };
        struct UninitializedExit
        {/*
         * This sets the in- and output arays. It is the responsibilty of the user to provide valid pointers.
         */
            template<class Event, class FSM, class STATE>
              void
              operator() (Event const&evt, FSM&SM, STATE&)
              {
                LOG(debug)
                              << "CbmTrdAdvDigitizer: Digitizer State Machine: Setting Arrays";
                SM.fRaw = evt.fRaw;
                SM.fDigis = evt.fDigis;
                SM.fNrDigis = 0;
              }
        };
        struct UninitializedTag
        {
        };
        typedef msm::front::euml::func_state<UninitializedTag,
            UninitializedEntry, UninitializedExit> Uninitialized;

        struct ReadyEntry
        {/*
         * This is the entry into the'groundstate' of the state machine. It is prepared
         * for the next digi and awaits the next index. Traversal of the input array is managed by the user,
         * this enables emitting a time sorted output array.
         */

            template<class Event, class FSM, class STATE>
              void
              operator() (Event const&, FSM&, STATE&)
              {
                LOG(debug)
                              << "CbmTrdAdvDigitizer: Digitizer State Machine: Ready for new Message";
              }
        };
        struct ReadyExit
        {
            template<class Event, class FSM, class STATE>
              void
              operator() (Event const&, FSM&, STATE&)
              {

              }
        };
        struct ReadyTag
        {
        };
        typedef msm::front::euml::func_state<ReadyTag, ReadyEntry, ReadyExit> Ready;

        struct HitFoundEntry
        {/*
         * Called after a MultiHit-Follower has been found by the guards. All bookkeeping variables
         * are updated and initialized if necessary. State between different hits is kept via pointers
         * to previous messages. If such state exists, this method should be called. This method will
         * initialize the buffers for the samples and estimate the residual signal from the precursor hit.
         */
            template<class Event, class FSM, class STATE>
              void
              operator() (Event const&evt, FSM&SM, STATE&)
              {
                LOG(debug)
                              << "CbmTrdAdvDigitizer: Digitizer State Machine: Estimating Hit Parameters";
                CbmSpadicRawMessage* raw =
                    static_cast<CbmSpadicRawMessage*> (SM.fRaw->At (evt.fIndex));
                SM.fCurrentMessage = raw;
                //Nullptr Check is unnecessary, as we will only transition to this state if raw is valid;
                UInt_t Address = SM.fBT->GetAddress (raw);
                auto Identifier = SM.fIndexMap.find (Address);
                if (Identifier == SM.fIndexMap.end ())
                  {
                    Int_t NewIndex = SM.fIndexMap.size ();
                    SM.fIndexMap[Address] = NewIndex;
                    //TODO: Initialize all bookkeeping members.
                    Identifier = SM.fIndexMap.find (Address);
                    SM.fPointer.resize (SM.fPointer.size () + 1);
                    SM.fRunningBaseline.resize (
                        SM.fRunningBaseline.size () + 1);
                    SM.fRunningBaseline.rbegin ()->set_capacity (1);
                    SM.fRunningBaseline.rbegin ()->resize (0);
                    SM.fBaselineStart = raw->GetFullTime ();
                  }
                //TODO: Read Baseline to running Buffer
                UInt_t Index = Identifier->second;
                SM.fCurrentBookkeepingIndex = Index;
                SM.fPointer[Index].push_back (raw);
                int16_t BaselineEstimate = SM.fBT->GetBaseline (raw);
                //if(BaselineEstimate<-120)
                SM.fRunningBaseline[Index].push_back (BaselineEstimate);
                SM.process_event (PrepareDigi ());
              }
        };
        struct HitFoundExit
        {
            template<class Event, class FSM, class STATE>
              void
              operator() (Event const&, FSM&SM, STATE&)
              {
                CbmSpadicRawMessage*raw = SM.fCurrentMessage;
                UInt_t Index = SM.fCurrentBookkeepingIndex;
                Float_t BaselineEstimate = std::accumulate (
                    SM.fRunningBaseline[Index].begin (),
                    SM.fRunningBaseline[Index].end (), 0L);
                BaselineEstimate /= SM.fRunningBaseline[Index].size ();
                /*
                 * Prepare sample and baseline arrays. the baseline array in this internal context is to be understood as the
                 * residual signal underlying the signal, e.g. the actual channel baseline or, in the case of a follower hit,
                 * the falling tail from the precursor hit.
                 */
                SM.fBaseline.fill (BaselineEstimate);
                SM.fSamples.fill (0.0);
                SM.fBaselineStart = raw->GetFullTime ();
                Int_t*Samples = raw->GetSamples ();
                UInt_t NrSamples = raw->GetNrSamples ();
                for (UInt_t i = 0; i < NrSamples; i++)
                  SM.fSamples.at (i) = Samples[i];
              }
        };
        struct HitFoundTag
        {
        };
        typedef msm::front::euml::func_state<HitFoundTag, HitFoundEntry,
            HitFoundExit> HitFound;

        struct MultiHitFoundEntry
        {/*
         * Called after a standard hit has been found by the guards. All bookkeeping variables
         * are updated and initialized if necessary. State between different hits is kept via pointers
         * to previous messages. If such state exists, this method should not be called, but rather the
         * multihit processing facilities.
         * Also record the running baseline for each channel.
         */
            template<class Event, class FSM, class STATE>
              void
              operator() (Event const&evt, FSM&SM, STATE&)
              {
                LOG(debug)
                              << "CbmTrdAdvDigitizer: Digitizer State Machine: Estimating MultiHit Parameters.";
                CbmSpadicRawMessage* raw =
                    static_cast<CbmSpadicRawMessage*> (SM.fRaw->At (evt.fIndex));
                SM.fCurrentMessage = raw;
                //Nullptr Check is unnecessary, as we will only transition to this state if raw is valid;
                UInt_t Address = SM.fBT->GetAddress (raw);
                auto Identifier = SM.fIndexMap.find (Address);
                if (Identifier == SM.fIndexMap.end ())
                  {
                    Int_t NewIndex = SM.fIndexMap.size ();
                    SM.fIndexMap[Address] = NewIndex;
                    //TODO: Initialize all bookkeeping members.
                    Identifier = SM.fIndexMap.find (Address);
                    SM.fPointer.resize (SM.fPointer.size () + 1);
                    SM.fRunningBaseline.resize (
                        SM.fRunningBaseline.size () + 1);
                    SM.fRunningBaseline.rbegin ()->set_capacity (1);
                    SM.fRunningBaseline.rbegin ()->resize (0);
                    SM.fBaselineStart = raw->GetFullTime ();
                  }
                //TODO: Read Baseline to running Buffer
                UInt_t Index = Identifier->second;
                SM.fCurrentBookkeepingIndex = Index;
                SM.fPointer[Index].push_back (raw);
                int16_t BaselineEstimate = SM.fBT->GetBaseline (raw);
                //if(BaselineEstimate<-120)
                SM.fRunningBaseline[Index].push_back (BaselineEstimate);
                SM.process_event (PrepareDigi ());
              }
        };
        struct MultiHitFoundExit
        {
            template<class Event, class FSM, class STATE>
              void
              operator() (Event const&, FSM&SM, STATE&)
              {
                CbmSpadicRawMessage*raw = SM.fCurrentMessage;
                UInt_t Index = SM.fCurrentBookkeepingIndex;
                Float_t BaselineEstimate = std::accumulate (
                    SM.fRunningBaseline[Index].begin (),
                    SM.fRunningBaseline[Index].end (), 0L);
                BaselineEstimate /= SM.fRunningBaseline[Index].size ();
                /*
                 * Prepare sample and baseline arrays. the baseline array in this internal context is to be understood as the
                 * residual signal underlying the signal, e.g. the actual channel baseline or, in the case of a follower hit,
                 * the falling tail from the precursor hit.
                 */
                SM.fBaseline.fill (BaselineEstimate);
                SM.fSamples.fill (0.0);
                SM.fBaselineStart = raw->GetFullTime ();
                Int_t*Samples = raw->GetSamples ();
                UInt_t NrSamples = raw->GetNrSamples ();
                for (UInt_t i = 0; i < NrSamples; i++)
                  SM.fSamples.at (i) = Samples[i];
              }
        };
        struct MultiHitFoundTag
        {
        };
        typedef msm::front::euml::func_state<MultiHitFoundTag,
            MultiHitFoundEntry, MultiHitFoundExit> MultiHitFound;

        struct DigiReadyEntry
        {/*
         *called after preparattion of the digi, either by the hit- or multihit-path
         * Test if the baselinetimestamp is valid.
         * Subtract Baseline&Signal correction.
         */
            template<class Event, class FSM, class STATE>
              void
              operator() (Event const&, FSM&SM, STATE&)
              {
                CbmSpadicRawMessage*raw = SM.fCurrentMessage;
                //UInt_t Index = SM.fCurrentBookkeepingIndex; (VF) not used
                Int_t DeltaTime = raw->GetFullTime () - SM.fBaselineStart;
                if (DeltaTime < 0)
                  LOG(fatal)
                                << "CbmTrdAdvDigitizer: Digitizer State Machine: Wrong Baseline Timestamp";
                UInt_t NrSamples = raw->GetNrSamples ();
                for (UInt_t i = 0; i < NrSamples; i++)
                  SM.fSamples.at (i) -= SM.fBaseline.at (i + DeltaTime);
                SM.process_event (CreateDigi ());
              }
        };
        struct DigiReadyExit
        {
            template<class Event, class FSM, class STATE>
              void
              operator() (Event const&, FSM&SM, STATE&)
              {

                CbmSpadicRawMessage*raw = SM.fCurrentMessage;
                TClonesArray*Digis = SM.fDigis; //(VF) not used
                Int_t NrDigis = SM.fNrDigis++;
                CbmTrdDigi* Digi =
                    static_cast<CbmTrdDigi*> (SM.fDigis->ConstructedAt (NrDigis));
                Digi->SetCharge (
                    SM.fBT->GetIntegratedCharge (SM.fSamples.data (),
                                                 raw->GetNrSamples ()));
                Digi->SetAddress (
                    static_cast<Int_t> (SM.fBT->GetAddress (raw)));
                Digi->SetTime (
                    raw->GetFullTime () * SM.fBT->GetSamplingTime ()); //65 ns per timestamp
                Digi->SetTriggerType (raw->GetTriggerType ());
                Digi->SetInfoType (raw->GetInfoType ());
                Digi->SetStopType (raw->GetStopType ());
                Digi->SetPulseShape (SM.fSamples.data ());/*&Samples[32]*/
                //TODO: Mehr Durchwischen
                UInt_t Index = SM.fCurrentBookkeepingIndex;
                SM.fSamples.fill (0.0);
                SM.fBaseline.fill (0.0);
                //TODO: Only remove pointers if done, else delegate this to the multihit task.
                SM.fBaselineStart = 0;
                if (!SM.fIsPrecursor)
                  SM.fPointer[Index].erase (SM.fPointer[Index].begin ());
                SM.fIsPrecursor = false;
              }
        };
        struct DigiReadyTag
        {
        };
        typedef msm::front::euml::func_state<DigiReadyTag, DigiReadyEntry,
            DigiReadyExit> DigiReady;

        typedef Uninitialized initial_state;

        //Define guards, currently only used to filter out hit messages
        struct isHit
        {
            template<class EVT, class FSM, class SourceState, class TargetState>
              bool
              operator() (EVT const& evt, FSM&SM, SourceState&, TargetState&)
              {
                CbmSpadicRawMessage*raw =
                    static_cast<CbmSpadicRawMessage*> (SM.fRaw->At (evt.fIndex));
                if (raw)
                  {
                    if (raw->GetHit ())
                      {
                        return true;
                      }
                  }
                return false;
              }
        };
        struct isMultiHit
        {
            template<class EVT, class FSM, class SourceState, class TargetState>
              bool
              operator() (EVT const& evt, FSM&SM, SourceState&, TargetState&)
              {
                CbmSpadicRawMessage*raw =
                    static_cast<CbmSpadicRawMessage*> (SM.fRaw->At (evt.fIndex));
                if (raw)
                  {
                    if (raw->GetHitAborted ())
                      {
                        return true;
                      }
                  }
                return false;
              }
        };
        struct isFollower
        {
            template<class EVT, class FSM, class SourceState, class TargetState>
              bool
              operator() (EVT const& evt, FSM&SM, SourceState&, TargetState&)
              {
                CbmSpadicRawMessage*raw =
                    static_cast<CbmSpadicRawMessage*> (SM.fRaw->At (evt.fIndex));
                if (raw)
                  {
                    if (raw->GetHitAborted ())
                      {
                        return true;
                      }
                  }
                return false;
              }
        };

        typedef CbmTrdDigiFinder_ df;
        struct transition_table : mpl::vector<
            //    Start          Event          Next          Action                  Guard
            //  +---------------+--------------+--------------+-----------------------+----------------+
            Row<Uninitialized, SetArrays, Ready, none, none>,
            //  +---------------+--------------+--------------+-----------------------+----------------+
            Row<Ready, NewMessage, HitFound, none, df::isHit>,
            Row<Ready, NewMessage, MultiHitFound, none, df::isMultiHit>,
            Row<Ready, NewMessage, HitFound    , none                  , df::isFollower >,
            //  +---------------+--------------+--------------+-----------------------+----------------+
            Row<HitFound, PrepareDigi, DigiReady, none, none>,
            //  +---------------+--------------+--------------+-----------------------+----------------+
            Row<DigiReady, CreateDigi, Ready, none, none>
        //  +---------------+--------------+--------------+-----------------------+----------------+
        >
        {
        };
        template<class FSM, class Event>
          void
          no_transition (Event const& e, FSM&, int state)
          {
            LOG(debug) << "CbmTrdDigiFinder: no transition from state " << state
                          << " on event " << typeid(e).name ();
          }
        TClonesArray*fRaw ;
        TClonesArray*fDigis;
        CbmSpadicRawMessage* fCurrentMessage;
        Bool_t fIsPrecursor;
        Int_t fNrDigis;
        UInt_t fCurrentBookkeepingIndex;
        std::map<UInt_t, UInt_t> fIndexMap;
        std::array<Float_t, 45> fBaseline;
        std::array<Float_t, 45> fSamples;
        std::vector<std::vector<CbmSpadicRawMessage*> > fPointer;
        ULong_t fBaselineStart;
        std::vector<boost::circular_buffer<int16_t>> fRunningBaseline;
        CbmTrdTestBeamTools* fBT;
    public:
    CbmTrdDigiFinder_ () :
	fRaw (nullptr), fDigis (nullptr), fCurrentMessage (nullptr), fIsPrecursor (false),
	fNrDigis (0), fCurrentBookkeepingIndex (0), fIndexMap (), fBaseline (), fSamples (),
	fPointer (), fBaselineStart (0), fRunningBaseline (),
	fBT (CbmTrdTestBeamTools::Instance (nullptr))
    {};
    };
    typedef msm::back::state_machine<CbmTrdDigiFinder_> CbmTrdDigiFinder;

  ClassDef(CbmTrdAdvDigitizer,1)
    ;
};

#endif /* FLES_READER_TASKS_CBMTRDADVDIGITIZER_H_ */

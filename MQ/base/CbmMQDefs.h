/* Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "FairMQDevice.h"

namespace cbm
{
  namespace mq
  {
    enum class Transition : int
    {
      Idle,
      DeviceReady,
      Ready,
      Stop,
      End,
      ErrorFound
    };

    enum class State : int
    {
      Running
    };

    void ChangeState(FairMQDevice* device, cbm::mq::Transition transition)
    {
      if (transition == cbm::mq::Transition::ErrorFound) { device->ChangeState(fair::mq::Transition::ErrorFound); }
      else if (transition == cbm::mq::Transition::End) {
        device->ChangeState(fair::mq::Transition::End);
      }
      else if (transition == cbm::mq::Transition::Ready) {
        device->ChangeState(fair::mq::Transition::ResetTask);
      }
      else if (transition == cbm::mq::Transition::DeviceReady) {
        device->ChangeState(fair::mq::Transition::ResetDevice);
      }
      else if (transition == cbm::mq::Transition::Idle) {
        device->ChangeState(fair::mq::Transition::Stop);
      }
      else {
        LOG(fatal) << "State Change not yet implemented";
        device->ChangeState(fair::mq::Transition::ErrorFound);
      }
    }

    void LogState(FairMQDevice* device)
    {
      LOG(info) << "Current State: " << device->GetCurrentStateName();
    }

    bool CheckCurrentState(FairMQDevice* device, cbm::mq::State state)
    {
      if (state == cbm::mq::State::Running) { return !(device->NewStatePending()); }
      else {
        LOG(fatal) << "State not yet implemented";
        device->ChangeState(fair::mq::Transition::ErrorFound);
        return 0;
      }
    }
  }  // namespace mq
}  // namespace cbm

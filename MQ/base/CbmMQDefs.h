//#ifdef HAVE_FAIRMQSTATEMACHINE
//#include "FairMQStateMachine.h"
//#endif
#include "FairMQDevice.h"

namespace cbm {
  namespace mq {
    enum class Transition : int {
      Idle,
      DeviceReady,
      Ready,
      Stop,
      End,
      ErrorFound
    };

    enum class State : int { Running };

    void ChangeState(FairMQDevice* device, cbm::mq::Transition transition) {
#ifdef HAVE_FAIRMQSTATEMACHINE
      if (transition == cbm::mq::Transition::ErrorFound) {
        device->ChangeState(FairMQStateMachine::Event::ERROR_FOUND);
      } else if (transition == cbm::mq::Transition::End) {
        device->ChangeState(FairMQStateMachine::Event::END);
      } else if (transition == cbm::mq::Transition::Ready) {
        device->ChangeState(FairMQStateMachine::Event::internal_READY);
      } else if (transition == cbm::mq::Transition::DeviceReady) {
        device->ChangeState(FairMQStateMachine::Event::internal_DEVICE_READY);
      } else if (transition == cbm::mq::Transition::Idle) {
        device->ChangeState(FairMQStateMachine::Event::internal_IDLE);
      } else {
        LOG(fatal) << "State Change not yet implemented";
        device->ChangeState(FairMQStateMachine::Event::ERROR_FOUND);
      }
#else
      if (transition == cbm::mq::Transition::ErrorFound) {
        device->ChangeState(fair::mq::Transition::ErrorFound);
      } else if (transition == cbm::mq::Transition::End) {
        device->ChangeState(fair::mq::Transition::End);
      } else if (transition == cbm::mq::Transition::Ready) {
        device->ChangeState(fair::mq::Transition::ResetTask);
      } else if (transition == cbm::mq::Transition::DeviceReady) {
        device->ChangeState(fair::mq::Transition::ResetDevice);
      } else if (transition == cbm::mq::Transition::Idle) {
        device->ChangeState(fair::mq::Transition::Stop);
      } else {
        LOG(fatal) << "State Change not yet implemented";
        device->ChangeState(fair::mq::Transition::ErrorFound);
      }
#endif
    }

    void LogState(FairMQDevice* device) {
#ifdef HAVE_FAIRMQSTATEMACHINE
      //      LOG(info) << "Current State: " <<  FairMQStateMachine::GetCurrentStateName();
      LOG(info) << "Current State: " << device->GetCurrentStateName();
#else
      LOG(info) << "Current State: " << device->GetCurrentStateName();
#endif
    }

    bool CheckCurrentState(FairMQDevice* device, cbm::mq::State state) {
#ifdef HAVE_FAIRMQSTATEMACHINE
      if (state == cbm::mq::State::Running) {
        return device->CheckCurrentState(FairMQStateMachine::State::RUNNING);
      } else {
        LOG(fatal) << "State not yet implemented";
        device->ChangeState(FairMQStateMachine::Event::ERROR_FOUND);
        return 0;
      }
#else
      if (state == cbm::mq::State::Running) {
        return !(device->NewStatePending());
      } else {
        LOG(fatal) << "State not yet implemented";
        device->ChangeState(fair::mq::Transition::ErrorFound);
        return 0;
      }
#endif
    }
  }  // namespace mq
}  // namespace cbm

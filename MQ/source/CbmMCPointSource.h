/**
 * CbmMCPointSource.h
 *
 * @since 2019-12-01
 * @author F. Uhlig
 */

#ifndef CBMMCPOINTSOURCE_H_
#define CBMMCPOINTSOURCE_H_

#include "CbmMQChannels.h"

#include "FairMQDevice.h"

#include "TClonesArray.h"

#include <boost/archive/binary_oarchive.hpp>

#include <ctime>
#include <string>
#include <vector>
// include this header to serialize vectors
#include <boost/serialization/vector.hpp>

class CbmMCPoint;
class FairRootManager;
class TClonesArray;

class CbmMCPointSource : public FairMQDevice {
public:
  CbmMCPointSource() = default;
  virtual ~CbmMCPointSource();

protected:
  uint64_t fMaxEvents {0};

  std::string fFileName {""};
  std::vector<std::string> fInputFileList {};  ///< List of input files
  uint64_t fFileCounter {};

  uint64_t fEventNumber {0};
  uint64_t fEventCounter {0};
  uint64_t fMessageCounter {0};

  int fMaxMemory {0};

  virtual void InitTask();
  virtual bool ConditionalRun();

private:
  bool SendData();
  void CalcRuntime();
  void ConnectChannelIfNeeded(int, std::string, std::string, FairRootManager*);

  template<class T>
  void PrintMCPoint(TClonesArray* arr)
  {

    Int_t entries = arr->GetEntriesFast();
    if (entries > 0) {
      T* point = static_cast<T*>(arr->At(0));
      LOG(info) << "Entries in TCA for data type " << point->GetName() << ": " << entries;
    }
    for (int i = 0; i < entries; ++i) {
      T* point = static_cast<T*>(arr->At(i));
      point->Print("");
    }
  }

  template<class T>
  std::vector<T> Convert(TClonesArray* arr)
  {

    std::vector<T> vec;
    Int_t entries = arr->GetEntriesFast();
    if (entries > 0) {
      T* point = static_cast<T*>(arr->At(0));
      LOG(info) << "Entries in TCA for data type " << point->GetName() << ": " << entries;
    }
    for (int i = 0; i < entries; ++i) {
      T* point = static_cast<T*>(arr->At(i));
      vec.emplace_back(*point);
    }
    return vec;
  }


  template<class T>
  bool ConvertAndSend(TClonesArray* arr, int i)
  {

    std::vector<T> vec;
    Int_t entries = arr->GetEntriesFast();
    if (entries > 0) {
      T* point = static_cast<T*>(arr->At(0));
      LOG(info) << "Entries in TCA for data type " << point->GetName() << ": " << entries;
    }
    for (int iEntries = 0; iEntries < entries; ++iEntries) {
      T* point = static_cast<T*>(arr->At(iEntries));
      vec.emplace_back(*point);
    }


    std::stringstream oss;
    boost::archive::binary_oarchive oa(oss);
    oa << vec;
    std::string* strMsg = new std::string(oss.str());


    FairMQMessagePtr msg(NewMessage(
      const_cast<char*>(strMsg->c_str()),  // data
      strMsg->length(),                    // size
      [](void* /*data*/, void* object) { delete static_cast<std::string*>(object); },
      strMsg));  // object that manages the data

    // TODO: Implement sending same data to more than one channel
    // Need to create new message (copy message??)
    if (fComponentsToSend.at(i) > 1) { LOG(info) << "Need to copy FairMessage"; }

    // in case of error or transfer interruption,
    // return false to go to IDLE state
    // successfull transfer will return number of bytes
    // transfered (can be 0 if sending an empty message).
    LOG(info) << "Send data to channel " << fChannelsToSend.at(i).at(0);
    if (Send(msg, fChannelsToSend.at(i).at(0)) < 0) {
      LOG(error) << "Problem sending data";
      return false;
    }

    return true;
  }

  std::chrono::steady_clock::time_point fTime {};

  std::vector<std::string> fAllowedChannels = {"MvdPoint", "StsPoint", "RichPoint", "MuchPoint",
                                               "Trdpoint", "TofPoint", "PsdPoint"};

  /*
    std::vector<std::string> fAllowedChannels 
      = {"MvdPoint", "StsPoint", "RichPoint", "MuchPoint", 
	 "Trdpoint", "TofPoint", "EcalPoint", "PsdPoint"};
*/

  CbmMQChannels fChan {fAllowedChannels};

  std::vector<int> fComponentsToSend {};
  std::vector<std::vector<std::string>> fChannelsToSend {{}};
  std::vector<TClonesArray*> fArrays {fAllowedChannels.size(), nullptr};
};

#endif /* CBMMCPOINTSOURCE_H_ */

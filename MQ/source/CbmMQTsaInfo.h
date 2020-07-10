/**
 * CbmMQTsaInfo.h
 *
 * @since 2017-11-17
 * @author F. Uhlig
 */

#ifndef CBMMQTSAINFO_H_
#define CBMMQTSAINFO_H_


#include "MicrosliceDescriptor.hpp"
#include "StorableTimeslice.hpp"
#include "Timeslice.hpp"
#include "TimesliceSource.hpp"
//#include "Message.hpp"

#include "FairMQDevice.h"

#include <ctime>
#include <string>
#include <vector>

class CbmMQTsaInfo : public FairMQDevice {
public:
  CbmMQTsaInfo();
  virtual ~CbmMQTsaInfo();

protected:
  uint64_t fMaxTimeslices;

  std::string fFileName;
  std::vector<std::string> fInputFileList;  ///< List of input files
  uint64_t fFileCounter;
  std::string fHost;
  uint64_t fPort;

  uint64_t fTSNumber;
  uint64_t fTSCounter;
  uint64_t fMessageCounter;

  int fMaxMemory = 0;

  virtual void InitTask();
  virtual bool ConditionalRun();

private:
  bool CheckTimeslice(const fles::Timeslice& ts);
  void PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc);
  bool SendData(const fles::StorableTimeslice& component);
  void CalcRuntime();
  bool IsChannelNameAllowed(std::string);

  fles::TimesliceSource* fSource;  //!
  std::chrono::steady_clock::time_point fTime;

  std::vector<std::string> fAllowedChannels = {"stscomponent",
                                               "trdcomponent",
                                               "tofcomponent"};
};

#endif /* CBMMQTSAINFO_H_ */

/**
 * CbmStsDigiSource.h
 *
 * @since 2019-08-21
 * @author F. Uhlig
 */

#ifndef CBMSTSDIGISOURCE_H_
#define CBMSTSDIGISOURCE_H_
#include "FairMQDevice.h"

#include <ctime>
#include <string>
#include <vector>

class CbmStsDigi;

class CbmStsDigiSource : public FairMQDevice {
public:
  CbmStsDigiSource();
  virtual ~CbmStsDigiSource();

protected:
  uint64_t fMaxEvents;

  std::string fFileName;
  std::vector<std::string> fInputFileList;  ///< List of input files
  uint64_t fFileCounter;

  uint64_t fEventNumber;
  uint64_t fEventCounter;
  uint64_t fMessageCounter;

  int fMaxMemory = 0;

  virtual void InitTask();
  virtual bool ConditionalRun();

private:
  void PrintStsDigi(const CbmStsDigi*);
  bool SendData();
  void CalcRuntime();
  bool IsChannelNameAllowed(std::string);

  std::chrono::steady_clock::time_point fTime;

  std::vector<std::string> fAllowedChannels = {"stsdigi"};
};

#endif /* CBMSTSDIGISOURCE_H_ */

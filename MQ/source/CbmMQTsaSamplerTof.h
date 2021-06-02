/** @file CbmMQTsaSamplerTof.h
  * @copyright Copyright (C) 2018 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [committer] **/

/**
 * CbmMQTsaSamplerTof.h
 *
 * @since 2018-09-14
 * @author N. Herrmann
 */

#ifndef CBMMQTSASAMPLERTOF_H_
#define CBMMQTSASAMPLERTOF_H_


#include "MicrosliceDescriptor.hpp"
#include "StorableTimeslice.hpp"
#include "Timeslice.hpp"
#include "TimesliceSource.hpp"
//#include "Message.hpp"

#include "FairMQDevice.h"

#include <ctime>
#include <string>
#include <vector>

class CbmMQTsaSamplerTof : public FairMQDevice {
public:
  CbmMQTsaSamplerTof();
  virtual ~CbmMQTsaSamplerTof();

protected:
  uint64_t fMaxTimeslices;

  std::string fFileName;
  std::string fDirName;

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
  bool OpenNextFile();

  bool CheckTimeslice(const fles::Timeslice& ts);
  void PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc);
  bool SendData(const fles::StorableTimeslice& component);
  void CalcRuntime();
  bool IsChannelNameAllowed(std::string);
  bool IsChannelUp(std::string);
  bool CreateAndSendComponent(const fles::Timeslice&, int);
  bool SendData(const fles::StorableTimeslice&, int);
  bool CreateAndCombineComponents(const fles::Timeslice&, int);
  bool AppendData(const fles::StorableTimeslice&, int);
  bool SendTs();
  void SendSysCmdStop();

  fles::TimesliceSource* fSource;  //!
  std::chrono::steady_clock::time_point fTime;


  // The vector fAllowedChannels contain the list of defined channel names
  // which are used for connecting the different devices. For the time
  // being the correct connection are done checking the names. A connection
  // using the name stscomponent will receive timeslices containing the
  // sts component only. The corresponding system ids are defined in the
  // vector fSysId. At startup it is checked which channels are defined
  // in the startup script such that later on only timeslices whith the
  // corresponding data are send to the correct channels.
  // TODO: Up to now we have three disconnected vectors which is very
  //       error prone. Find a better solution

  std::vector<std::string> fAllowedChannels = {"stscomponent", "trdcomponent", "tofcomponent", "syscmd", "syscmdin"};
  std::vector<int> fSysId                   = {16, 64, 96};

  std::vector<int> fComponentsToSend                    = {0, 0, 0};
  std::vector<std::vector<std::string>> fChannelsToSend = {{}, {}, {}};
};

#endif /* CBMMQTSASAMPLERTOF_H_ */

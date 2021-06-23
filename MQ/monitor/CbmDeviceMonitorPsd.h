/**
 * CbmDeviceMonitorPsd.h
 *
 * @since 2021-02-17
 * @author N. Karpushkin
 * @comment based on CbmDeviceMonitorT0 by F. Uhlig
 */

#ifndef CBMDEVICEMONITORPSD_H_
#define CBMDEVICEMONITORPSD_H_

#include "Timeslice.hpp"

#include "FairMQDevice.h"

#include "Rtypes.h"
#include "TMessage.h"
#include "TObjArray.h"

#include <chrono>
#include <map>
#include <vector>

class TList;
class CbmMcbm2018MonitorAlgoPsd;

class CbmDeviceMonitorPsd : public FairMQDevice {
public:
  CbmDeviceMonitorPsd();
  virtual ~CbmDeviceMonitorPsd();

protected:
  virtual void InitTask();
  bool HandleData(FairMQMessagePtr&, int);

private:
  /// Constants
  static const uint16_t kusSysId = 0x80;
  Bool_t fbComponentsAddedToList = kFALSE;

  /// Control flags
  Bool_t fbMonitorMode;      //! Switch ON the filling of a minimal set of histograms
  Bool_t fbIgnoreOverlapMs;  //! Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice
  Bool_t fbMonitorChanMode;  //! Switch ON the filling channelwise histograms
  Bool_t fbMonitorWfmMode;   //! Switch ON the filling waveforms histograms
  Bool_t fbMonitorFitMode;   //! Switch ON the filling waveform fitting histograms
                             //  Bool_t fbDebugMonitorMode;  //! Switch ON the filling of a additional set of histograms

  /// User settings parameters
  std::string fsChannelNameDataInput;
  std::string fsChannelNameHistosInput;
  std::string fsChannelNameHistosConfig;
  std::string fsChannelNameCanvasConfig;
  uint32_t fuPublishFreqTs;
  double_t fdMinPublishTime;
  double_t fdMaxPublishTime;

  UInt_t fuHistoryHistoSize;
  std::vector<Int_t> fviHistoChargeArgs; /** Charge histogram arguments in adc counts **/
  std::vector<Int_t> fviHistoAmplArgs;   /** Amplitude histogram arguments in adc counts **/
  std::vector<Int_t> fviHistoZLArgs;     /** ZeroLevel histogram arguments in adc counts **/


  /// List of MQ channels names
  std::vector<std::string> fsAllowedChannels;

  /// Parameters management
  TList* fParCList;

  /// Statistics & first TS rejection
  uint64_t fulNumMessages;
  uint64_t fulTsCounter;
  std::chrono::system_clock::time_point fLastPublishTime;

  /// Processing algo
  CbmMcbm2018MonitorAlgoPsd* fMonitorAlgo;

  /// Array of histograms to send to the histogram server
  TObjArray fArrayHisto;
  /// Vector of string pairs with ( HistoName, FolderPath ) to send to the histogram server
  std::vector<std::pair<std::string, std::string>> fvpsHistosFolder;
  /// Vector of string pairs with ( CanvasName, CanvasConfig ) to send to the histogram server
  /// Format of Can config is "NbPadX(U);NbPadY(U);ConfigPad1(s);....;ConfigPadXY(s)"
  /// Format of Pad config is "GrixX(b),GridY(b),LogX(b),LogY(b),LogZ(b),HistoName(s),DrawOptions(s)"
  std::vector<std::pair<std::string, std::string>> fvpsCanvasConfig;

  bool IsChannelNameAllowed(std::string channelName);
  Bool_t InitContainers();
  Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
  void Finish();
  bool SendHistograms();
};

// special class to expose protected TMessage constructor
class CbmMQTMessage : public TMessage {
public:
  CbmMQTMessage(void* buf, Int_t len) : TMessage(buf, len) { ResetBit(kIsOwner); }
};


#endif /* CBMDEVICEMONITORPSD_H_ */

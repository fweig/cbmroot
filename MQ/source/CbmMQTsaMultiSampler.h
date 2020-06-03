/**
 * CbmMQTsaSampler.h
 *
 * @since 2017-11-17
 * @author F. Uhlig
 */

#ifndef CBMMQTSAMULTISAMPLER_H_
#define CBMMQTSAMULTISAMPLER_H_


#include "TimesliceSource.hpp"
#include "Timeslice.hpp"
#include "StorableTimeslice.hpp"
#include "MicrosliceDescriptor.hpp"

#include "FairMQDevice.h"

#include <string>
#include <vector>
#include <ctime>

class CbmMQTsaMultiSampler : public FairMQDevice
{
  public:
    CbmMQTsaMultiSampler();
    virtual ~CbmMQTsaMultiSampler();

  protected:
    uint64_t fMaxTimeslices;

    std::string fFileName;
    std::string fDirName;

    std::vector<std::string>   fInputFileList;    ///< List of input files
    uint64_t   fFileCounter;
    std::string fHost;
    uint64_t  fPort;
    uint64_t  fHighWaterMark;

    bool fbNoSplitTs = false;
    bool fbSendTsPerSysId = false;
    bool fbSendTsPerChannel = false;

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
    bool CreateAndSendComponent(const fles::Timeslice&, int);
    bool CreateAndCombineComponentsPerSysId( const fles::Timeslice& );
    bool CreateAndCombineComponentsPerChannel( const fles::Timeslice& );
    bool CreateAndSendFullTs( const fles::Timeslice& );
    bool SendData(const fles::StorableTimeslice&, int);
    bool SendData(const fles::StorableTimeslice&, std::string);

    fles::TimesliceSource* fSource; //!
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

    std::vector<std::string> fAllowedChannels
          = {"stscomponent","richcomponent","trdcomponent","muchcomponent","tofcomponent","t0component"};
//    std::vector<int> fSysId = {16, 48, 64, 96, 144, 80};
    std::vector<int> fSysId = {0x10, 0x30, 0x40, 0x50, 0x60, 0x90};


    std::vector<int> fComponentsToSend = {0, 0, 0, 0, 0, 0};
    std::vector<std::vector<std::string>> fChannelsToSend = { {}, {}, {}, {}, {}, {} };

    bool fbListCompPerSysIdReady = false;
    std::vector< std::vector< uint32_t > > fvvCompPerSysId= { {}, {}, {}, {}, {}, {} };

    bool fbListCompPerChannelReady = false;
    std::vector< std::string > fvChannelsToSend= {};
    std::vector< std::vector< uint32_t > > fvvCompPerChannel= {};
};

#endif /* CBMMQTSASAMPLER_H_ */

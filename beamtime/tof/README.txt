--------------------------------------------------------------
     Readme file for the ToF MBS event unpacker in CBMRoot
--------------------------------------------------------------
09/07/2013: PAL, First version 
--------------------------------------------------------------
Supported hardware:
   * TRIGLOG  = trigger logic VME board implemented in a GSI 
                VULOM board, J. Fruehauf
   * SCALORMU = Scaler-Or-Multiplicty VMEboard implemented in a 
                GSI VULOM board, also called SCOM in some place 
                of code, J. Fruehauf 2012
   * VFTX     = 10ps 32ch Tot TDC VME board implemented in a GSI 
                VFTX board, E. Bayer/J. Fruehauf 2011/2012

Planned supported hardware (empty classes, calls missing, ...:
   * v1290    = CAEN V1290A and V1290N TDC VME board, based on
                CERN HPTDC chips
   * GET4     = 4 channel self-triggered TDC chip developed at
                GSI, v1.0 (2012) and higher
   * TRB3     = 4*32ch Tot TDC implemented in a HADES TRB3 board, 
                2012/2013
   * FPGA-TDC = 32ch TOT TDC implemented in a dedicated FPGA board,
                specifically developped for CBM TOF wall
                J. Fruehauf and GSI E.E. 2013/2014
--------------------------------------------------------------

  I. Classes/headers Description, by call/step order
     1. Custom MBS event source (adapted from Go4)
         In ./unpack/mbs
* TGo4GSIEventHeader
   See Go4 Online class description
* TGo4SubEventHeader10
   See Go4 Online class description
* TGo4EventHeader10
   See Go4 Online class description
* TMbsSubEvent
   See Go4 Online class description
* TMbsEvent
   See Go4 Online class description
* TGo4MbsSourcePrintPar
   See Go4 Online class description
* TMbsSourceParameter
   Inherits from FairParGenericSet.
   Parameter class for all Go4 like MBS event sources
* TMbsFileParameter
   Inherits from TMbsSourceParameter.
   Parameter class for the LMD file as Go4 like MBS event source.
* TMbsContFact
   See Go4 Online class description
* TGo4Exception
   See Go4 Online class description
* TGo4RuntimeException
   See Go4 Online class description
* TGo4EventSourceException
   See Go4 Online class description
* TGo4EventEndException
   See Go4 Online class description
* TGo4EventErrorException
   See Go4 Online class description
* TGo4EventTimeoutException
   See Go4 Online class description
* TMbsSource
   See Go4 Online class description
* TMbsClient
   See Go4 Online class description
* TMbsFile
   See Go4 Online class description

     2. Unpacking
         In ./unpack/tof
* TMbsUnpackTofContFact:
   Container for all parameter classes that could be used in the each step of 
   unpacking:
   TMbsUnpackTofPar, TMbsCalibTofPar, TMbsMappingTofPar &  TMbsConvTofPar
* TMbsUnpackTofPar:
   Main Parameter class for the pure unpacking step: file -> Raw
* TMbsUnpackTofCustom:
   Unpacker using the Go4 like custom MBS source as event source.
   Instanciate TTofTriglogUnpacker, TTofScomUnpacker and TTofVftxUnpacker objects
   depending on options.
   Read a full MBS event and then its SubEvents to assign the proper block of data
   to each unpackers
   Output object collections: 
      - TofTriglog (TTofTriglogBoard)
      - TofRawScalers (TTofScalerBoard)
      - TofVftxTdc (TTofVftxBoard)
* TMbsUnpackTof:
   Inherits from FairUnpack => meant to be used with FairSource and its childs.
   Unpacker for the roc3587::proc_COSY_Nov11 type of MBS subevent, which hold the data
   from the detector acquisition boards (TDC, QDC, scalers, ...).
   Instanciate TTofScomUnpacker and TTofVftxUnpacker objects depending on options.
   Read the data block corresponding to this subevent and then split it to assign 
   the proper block of data to each unpackers
   Output object collections: 
      - TofRawScalers (TTofScalerBoard)
      - TofVftxTdc (TTofVftxBoard)
* TTriglogUnpackTof:
   Inherits from FairUnpack => meant to be used with FairSource and its childs.
   Unpacker for the roc3587::proc_triglog type of MBS subevent, which hold the trigger 
   board data.
   Instanciate a TTofTriglogUnpacker objects depending on options and feed it with 
   the data blocks it receives when all options are OK.
   Output object collections: 
      - TofRawScalers (TTofScalerBoard)
      - TofTriglog (TTofTriglogBoard)
* scalers/ TTofScalerBoard:
   Class storing data from a bard with scalers. These data are made of the counts
   value for each scaler and the count value for a reference internal clock if it
   is present. 
* scalers/triglog/ TTofTriglogUnpacker:
   Implement the unpacking of data coming from a TRIGLOG board and the storing of
   resulting data in a TTofTriglogBoard objects for event time, clock and trigger
   related data and in a TTofScalerBoard for the 3 stages of trigger scalers data
   and for the clock data. (Clock data saved twice on purpose).
* scalers/triglog/ TTofTriglogBoard:
   Class storing the part of the unpacked data from a TRIGLOG board which are 
   directly related to trigger and event timing (MBS time)
* scalers/scalormu/ TTofScomUnpacker:
   Implement the unpacking of data coming from a SCALER-OR-MULTIPLICITY board and 
   the storing of resulting data in a TTofScalerBoard ( scalers & internal clock 
   data).
* tdc/ TTofTdcData:
   "Standard" class for storing a single raw data point from a TDC. Made of a chan.
   number, a fine time value, a coarse time value, a edge/sign flag and an optional
   Time Over Threshold value. All output objets for TDC unpackers should be daughter 
   class of this one for the calibration step to work. For proper operation daughter
   should reimplement the comparison functions/operators if needed to allow a rough
   time ordering
* tdc/ TTofTdcBoard:
   "Standard" class for storing the raw data from a TDC board. Holds a collection of
   TTofTdcData, a TDC type, the number of channel of this board and an optional 
   trigger time. All output containers for TDC unpackers should be daughter class of 
   this one for the calibration step to work. For proper operation daughter should 
   reimplement the data collection related functions if a specific dat object class
   is made
* tdc/vftx TTofVftxUnpacker:
   Implement the unpacking of data coming from a VFTX FPGA TDC board and the 
   storing of resulting data TTofVftxData objects stored in the collection of a 
   TTofVftxBoard.
* tdc/vftx TTofVftxData:
   Daughter class of TTofTdcData, adds an optional future bit flag and reimplement 
   the comparison function & operator to match VFTX properties.
* tdc/vftx TTofVftxBoard:
   Daughter class of TTofTdcBoard, adds an optional flag for trigger type and 
   reimplement the data collection related functions to use TTofVftxData instead of 
   TTofTdcData
* TofVmeDef:
   Defines namespace tofVme, which holds general constants definition for the TOF 
   MBS unpacking.
* commons:
   Taken from last RocLib rev. 4862. Defines namespace roc, which holds the ProcId 
   value for the MBS sub-event taken with CBM MBS until mid-2013.
* scalers/ TofScalerDef:
   Defines namespace tofscaler, which holds general constants definition for the 
   scalers unpacking.
* scalers/scalormu TofScomDef:
   Defines namespaces scalormu and scalormubig, which hold constants definition for 
   the scalers properties of both ScalOrMu version.  
* scalers/triglog TofTriglogDef:
   Defines namespace triglog, which holds constants definition for the properties 
   of TRIGLOG scalers.  
* tdc/ TofTdcDef:
   Defines namespace toftdc, which holds general constants definition for the 
   tdc boards unpacking.
* tdc/vftx/ TofVftxDef:
   Defines namespace vftxtdc, which holds constants definition for the unpacking 
   of VFTX data (nb Chans, masks, offsets, ...).  
* tdc/v1290/ TofCaenDef:
   Defines namespace caentdc, which holds constants definition for the unpacking 
   of CAEN V1290X data (nb Chans, masks, offsets, ...).  
* tdc/get4/ TofGet4Def:
   Defines namespace get4tdc, which holds constants definition for the unpacking 
   of GET4 v1.X data (nb Chans, masks, offsets, ...).   
* tdc/trb/ TofTrbTdcDef:
   Defines namespace trbtdc, which holds constants definition for the unpacking 
   of TRB3 based FPGA TDC data (nb Chans, masks, offsets, ...). 

     3. Calibration
         In ./unpack/tof/calib
* TMbsCalibTof:
   Calibration control class.
   Instanciate TMbsCalibScalTof and TMbsCalibTdcTof objects depending on options.
   Calls the TMbsCalibScalTof and TMbsCalibTdcTof calibration functions in a 
   controlled way.
* TMbsCalibTofPar:
   Main Parameter class for the calibration step: Raw -> Calib electronics data.
   Holds calibration parameters for TMbsCalibScalTof & TMbsCalibTdcTof.
   Different settings for each type of TDC.
* scaler/ TMbsCalibScalTof:
   Calibrator for scaler data. 
   Performs the conversion of the reference clock counts to an absolute time of 
   few us precision. If the TRIGLOG data are  present, this reference clock is 
   histogrammmed against the MBS time  (precision ~1ms) to allow for a 
   calibration of the clock frequency. If the  clock is absent and the TRIGLOG 
   data re present, hte internal clock from TRIGLOG will be used instead to 
   obtain the absolute time.
   Once the absolute time is obtained, it is used to calculate the time since 
   the first event and the time since the last event. Form the time since the 
   last event, it calculates the instantaneous  rate since last event for each 
   channel in each scaler of the board. 
   Input object collections:
      - TofRawScalers (TTofScalerBoard)
      - TofTriglog (TTofTriglogBoard) if available
   Output object collections: 
      - TofCalibScaler (TTofCalibScaler)
* scaler/ TTofCalibScaler:
   Class to store the calibrated rate of all scaler channels from a single 
   scaler board. Holds also the scaler board type, the scaler number and the 
   channel number. When a reference internal clock is present, it is stored as 
   an absolute time, as well as the time since the first event and the last 
   event obtained from it.
* tdc/ TMbsCalibTdcTof:
   Calibrator for TDC data of all types. Perform bin->time calibration for each 
   timing channel, Edges association (ToT building) and generation/loading of 
   bin-> calibration histograms files. Options allow to invert time and tot 
   edges, select the ToT building method, apply board specific offset, etc...
   5 ToT building methods are available:
      - None => just store both rising and falling edges as calibrated times
      - Time and ToT in same raw data object (message) => trivial, just need the
        bin->tot conversion constantm which should be defined for each type of 
        board supporting this readout mode
      - Time and ToT measured in same channel but stored in different data 
        objects (messages) => need time ordering after calibrating each data 
        message in an event. Building once the full board is ordered.
      - Time and Tot measured in adjacent channel in the same board (e.g. 2 TDC
        channels for a single input channel) => Similar to previous one. Number 
        of channels divided by two after calibration.
      - Time and Tot measured in same channel in two adjacent boards => need 
        time ordering after calibrating each data message in an event. Building 
        only once both full boards are calibrated and ordered.
   Two calibration histograms saving method are available:
      - A single root file for all channels and boards present (non zero number 
        of counts)
      - A root file for each channel/board pair placed in the calib subfolder.
   Input object collections:
      - TofVftxTdc (TTofVftxBoard)
      - TofTriglog (TTofTriglogBoard) if available
   Output object collections: 
      - TofCalibData (TTofCalibData)
* tdc/ TTofCalibData:
   Class for storing a single calibrated time data point from a TDC. Made of a tdc 
   type, a board number, a channel number, an edge index (0 = rising, 1 = falling, 
   2 = full hit), a calibrated time in ps and am optional ToT in ps. Function and 
   operator for time ordering are provided.
   
     4. Mapping
         In ./unpack/tof/mapping
* TMbsMappingTof
   The mapping step goal is to tranform the electronics related calibrated into 
   detector related data:
      (board, channel) -> (System, Sm, Module, Channel, Side)
   The TDC data are transformed into CbmTofDigi/CbmTofDigiExp depending on the 
   options. The mapping is done by providing in the parameter file a detector 
   Unique ID (UID) following the CbmTofAddress format
   Is is planned but not implemented yet to also have a mapping for the scalers:
      Rate(Board,scaler,channel)[1/s] -> Flux( Detector )[1/(s*cm^2)] 
   Input object collections:
      - TofCalibData (TTofCalibData)
   Output object collections: 
      - CbmTofDigi (CbmTofDigi or CbmTofDigiExp)
* TMbsMappingTofPar.h
   Main Parameter class for the mapping step: Calib electronics data -> Digis.
   Holds calibration parameters for TMbsMappingTof.
   
     5. Conversion to a ROOT TTree file in old Go4 unpacker format
         In ./unpack/tof/output
* MbsCrateDefines
   Definitions file from the old Go4 unpacker, used in its output classes
   cf Go4 unpacker in RocLib->beamtime->tof-tdctest formore info.
* TVftxBoardData
   Calibrated VFTX data holder from the old Go4 unpacker
   cf Go4 unpacker in RocLib->beamtime->tof-tdctest formore info.
* Scalers_Event
   Calibrated scaler data holder from the old Go4 unpacker
   cf Go4 unpacker in RocLib->beamtime->tof-tdctest formore info.
* Rpc_Event
   Rpc Hits and clusters holder from the old Go4 unpacker
   cf Go4 unpacker in RocLib->beamtime->tof-tdctest formore info.
* Plastics_Event
   Plastic Hits holder from the old Go4 unpacker
   cf Go4 unpacker in RocLib->beamtime->tof-tdctest formore info
* TMbsConvTof
   This step is filling a ROOT TTree following the old Go4 unpacker format.
   It uses as output classes the ones from the Go4 unpacker.
   Input object collections:
      - TofTriglog (TTofTriglogBoard) if available
      - TofVftxTdc (TTofVftxBoard)
      - TofCalibData (TTofCalibData)
      - TofCalibScaler (TTofCalibScaler)
      - CbmTofDigi (CbmTofDigi or CbmTofDigiExp)
* TMbsConvTofPar
   Main Parameter class for the conversion step: any input -> Go4/Roclic 
   classes.
   Allows to control what is added to the Tree and how it is remapped.

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 II. Macro description and use

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

III. Option files description and use

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

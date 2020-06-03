// -----------------------------------------------------------------------------
// ----- TofDef header file                                                -----
// -----                                                                   -----
// ----- created by C. Simon on 2014-03-18                                 -----
// -----                                                                   -----
// ----- based on TofVmeDef by P.-A. Loizeau                               -----
// ----- https://subversion.gsi.de/fairroot/cbmroot/development/ploizeau/  -----
// -----   main/unpack/tof/TofVmeDef.h                                     -----
// ----- revision 20754, 2013-07-17                                        -----
// -----------------------------------------------------------------------------

#ifndef TOFDEF_H_
#define TOFDEF_H_

#include "TString.h"

namespace tofMbs
{
   enum BoardTypes {
      undef      =  0,  // unknown type, default value
      caenV1290  =  1,  // CAEN v1290A and CAEN v1290N VME boards based on HPTDC chips
      vftx       =  2,  // FPGA TDC of the VFTX family (E. Bayer dev.)
      trb        =  3,  // FPGA TDC of the HADES TRB3 family (alias of trbtdc)
      get4       =  4,  // GET4 chips
      triglog    = 10,  // VULOM TRIGLOG board
      scalormu   = 11,  // VULOM Scaler Or Multiplicity board 
      scalormubig= 12,  // VULOM Scaler Or Multiplicity board  32 channel version ? not used for now
      scaler2014 = 13,  // VULOM Scaler Or/And board used in the GSI April 2014 TOF beamtime
      triglogscal= 14,  // VULOM TRIGLOG board used as additional scaler board
      orgen      = 15,  // VULOM Or Generation board (nov 2015) with more scalers than scaler2014
      caenv965   = 20,  // CAEN v965A and CAEN v965N VME QDC boards
      lecroy1182 = 21,  // LECROY 1182 VME QDC/ADC board 
      vulqfwread = 22,  // VULOM QFW board (Charge to Frequency Converter Readout)
      trbseb     = 30,  // Subevent builder on central TRB-FPGA
      trbtdc     = 31,  // TDC on peripheral TRB-FPGA or FEE-TDC
      trbhub     = 32   // Hub on peripheral TRB-FPGA

   };
   enum MappingFields {
      BoardIndex = 0,  // General board Index in the mapping array/MBS
      Active     = 1,  // Active flag, 1 = board present
      Address    = 2,  // VME base address of the board (used only in MBS)
      AddMod     = 3,  // VME Address modifier (used only in MBS)
      TokenA     = 4,  // Token inserted at the beginning of data for this board
      TokenB     = 5,  // Token inserted at the beginning of data for this board
      Type       = 6   // Board Type
   };
   const UInt_t kuNbFieldsMapping = 7;
   const UInt_t kuNbFieldsCtsMapping = 3;
   
   enum CtsMappingFields {
      TriggerChannel = 0,
      TriggerType    = 1,
      TriggerUnpack = 2
   };

   const TString ksTriglogHistName = "triglog";
   const TString ksScomHistName    = "scom";
   const TString ksScalHistName    = "scal";
   const TString ksOrGenHistName   = "orgen";
   const TString ksV965HistName    = "v965";
   const TString ks1182HistName    = "1182";
   const TString ksQfwHistName     = "qfw";
   
   /*
   const TString ksTriglogParName  = "Triglog";
   const TString ksScomParName     = "Scom";
   const TString ksV965ParName     = "V965";
   const TString ks1182ParName     = "1182";
   const TString ksQfwParName      = "Qfw";
   */
}

namespace tofTrb
{
  enum TrbNetStatusBits {
     status_network_0    = 0x00000001,
     status_network_1    = 0x00000002,
	 status_network_2    = 0x00000004,
	 status_network_3    = 0x00000008,
	 status_network_4    = 0x00000010,
	 status_network_5    = 0x00000020,
	 status_network_6    = 0x00000040,
	 status_readout_16   = 0x00010000,
	 status_readout_17   = 0x00020000,
	 status_readout_18   = 0x00040000,
	 status_readout_19   = 0x00080000,
	 status_readout_20   = 0x00100000,
	 status_readout_21   = 0x00200000,
	 status_readout_22   = 0x00400000,
	 status_readout_23   = 0x00800000,
	 status_readout_24   = 0x01000000,
	 status_readout_25   = 0x02000000,
	 status_readout_26   = 0x04000000,
	 status_readout_27   = 0x08000000
  };
}

#endif // TOFDEF_H_   

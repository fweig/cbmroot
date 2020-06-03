#ifndef MBSCRATEDEFINES_H
#define MBSCRATEDEFINES_H

// TRIGLOG INFO
#ifndef WITHGO4ROC
   #define NUM_SCALERS 3
   #define N_SCALERS_CH 16
#endif // WITHGO4ROC

// number of LeCroy 1182 modules
#define MAX_1182 2
// number of channels in LeCroy 1182 modules
#define NUM_1182_CH 8

// number of 1290 TDC modules
#define MAX_1290 8

// V1290 bin size
//#define CAENBINSIZE  25.0/1024.0

// number of 965 QDC Modules
#define MAX_965 3

// number of FPGA TDC modules
#define MAX_FPGA_TDC 11

//#define OLD_VERSION_VULOM // <- Pre-COSY nov11 HD vulom FPGA code
//#define VERSION_VFTX    // <- June   2012 version based on VFTX board (32 chan, 16 true chan+tot)
#define VERSION_VFTX_28 // <- August 2012 version based on VFTX board (56 chan, 28 true chan+tot)

// number of channels in FPGA TDC modules
//#ifdef VERSION_VFTX
//   #define FPGA_TDC_NBCHAN 32
//#elif defined VERSION_VFTX_28
   #define FPGA_TDC_NBCHAN 56
//#else
//   #define FPGA_TDC_NBCHAN 8
//#endif

/******************** FPGA TDC data unpacking definitions *************/
#define TDC_EVT_HEADER_KEY_MASK   0xFFFF0000
#define TDC_EVT_HEADER_KEY_SHIFT        16
#define TDC_EVT_HEADER_KEYWORD        0xABCD
#define TDC_EVT_HEADER_MOD_MASK   0x0000FF00
#define TDC_EVT_HEADER_MOD_SHIFT         8
#define TDC_EVT_HEADER_NB_MASK    0x000000FF


#ifdef OLD_VERSION_VULOM
   #define TDC_FIFO_FINE_CT          0x000003FF
   #define TDC_FIFO_COARSE_CT        0x01FFFC00 // Coarse counter value
   #define TDC_FIFO_COARSE_CT_SHIFT       10
   #define TDC_FIFO_COARSE_FT_SHIFT        0
   #define TDC_FIFO_FUTURE_BIT       0x02000000    // Flag for hits after trigger
   #define TDC_FIFO_FUTURE_BIT_SHIFT       25
   #define TDC_FIFO_DATA_CHAN        0x7C000000
   #define TDC_FIFO_DATA_CHAN_SHIFT        26
#elif defined VERSION_VFTX
   #define TDC_FIFO_FINE_CT          0x000007FF
   #define TDC_FIFO_COARSE_CT        0x01FFF800    // Coarse counter value
   #define TDC_FIFO_COARSE_CT_SHIFT        11
   #define TDC_FIFO_COARSE_FT_SHIFT        0
   #define TDC_FIFO_FUTURE_BIT       0x02000000    // Flag for hits after trigger
   #define TDC_FIFO_FUTURE_BIT_SHIFT       25
   #define TDC_FIFO_DATA_CHAN        0x7C000000    // Channel number
   #define TDC_FIFO_DATA_CHAN_SHIFT        26
#elif defined VERSION_VFTX_28
   #define TDC_FIFO_FINE_CT          0x000007FF
   #define TDC_FIFO_COARSE_CT        0x00FFF800    // Coarse counter value
   #define TDC_FIFO_COARSE_CT_SHIFT        11
   #define TDC_FIFO_COARSE_FT_SHIFT        0
   #define TDC_FIFO_FUTURE_BIT       0x01000000    // Flag for hits after trigger
   #define TDC_FIFO_FUTURE_BIT_SHIFT       24
   #define TDC_FIFO_DATA_CHAN        0x7E000000    // Channel number
   #define TDC_FIFO_DATA_CHAN_SHIFT        25
#else
   #define TDC_FIFO_FINE_CT          0x000007FF
   #define TDC_FIFO_COARSE_CT        0x03FFF800 // Coarse counter value
   #define TDC_FIFO_COARSE_CT_SHIFT       11
   #define TDC_FIFO_COARSE_FT_SHIFT        0
   #define TDC_FIFO_FUTURE_BIT       0x04000000    // Flag for hits after trigger
   #define TDC_FIFO_FUTURE_BIT_SHIFT       26
   #define TDC_FIFO_DATA_CHAN        0x78000000
   #define TDC_FIFO_DATA_CHAN_SHIFT        27
#endif

#ifdef OLD_VERSION_VULOM
   #define TDC_FIFO_HEADER_DATA_CNT  0x000000FF    // Data count
   #define TDC_FIFO_HEADER_TRIG_TIME 0x01FFFC00    // Coarse counter value
   #define TDC_FIFO_HEADER_TRIG_TIME_SHIFT  10
   #define TDC_FIFO_HEADER_TRIG_TYPE 0x60000000    // Trigger Type
   #define TDC_FIFO_HEADER_TRIG_TYPE_SHIFT 29
   #define TDC_FIFO_MESSAGE_TYPE     0x80000000    // Message Type
   #define TDC_FIFO_MESSAGE_TYPE_SHIFT     31      // (1 = header, 0 = data)
#elif defined VERSION_VFTX
   #define TDC_FIFO_HEADER_DATA_CNT  0x000000FF    // Data count
   #define TDC_FIFO_HEADER_TRIG_TIME 0x01FFF800    // Coarse counter value
   #define TDC_FIFO_HEADER_TRIG_TIME_SHIFT  11
   #define TDC_FIFO_HEADER_TRIG_TYPE 0x60000000    // Trigger Type
   #define TDC_FIFO_HEADER_TRIG_TYPE_SHIFT 29
   #define TDC_FIFO_MESSAGE_TYPE     0x80000000    // Message Type
   #define TDC_FIFO_MESSAGE_TYPE_SHIFT     31      // (1 = header, 0 = data)
#elif defined VERSION_VFTX_28
   #define TDC_FIFO_HEADER_DATA_CNT  0x000000FF    // Data count
   #define TDC_FIFO_HEADER_TRIG_TIME 0x00FFF800    // Coarse counter value
   #define TDC_FIFO_HEADER_TRIG_TIME_SHIFT  11
   #define TDC_FIFO_HEADER_TRIG_TYPE 0x60000000    // Trigger Type
   #define TDC_FIFO_HEADER_TRIG_TYPE_SHIFT 29
   #define TDC_FIFO_MESSAGE_TYPE     0x80000000    // Message Type
   #define TDC_FIFO_MESSAGE_TYPE_SHIFT     31      // (1 = header, 0 = data)
#else
   #define TDC_FIFO_HEADER_DATA_CNT  0x000000FF    // Data count
   #define TDC_FIFO_HEADER_TRIG_TIME 0x03FFF800    // Coarse counter value
   #define TDC_FIFO_HEADER_TRIG_TIME_SHIFT  11
   #define TDC_FIFO_HEADER_TRIG_TYPE 0x60000000    // Trigger Type
   #define TDC_FIFO_HEADER_TRIG_TYPE_SHIFT 29
   #define TDC_FIFO_MESSAGE_TYPE     0x80000000    // Message Type
   #define TDC_FIFO_MESSAGE_TYPE_SHIFT     31      // (1 = header, 0 = data)
#endif

//TODO: Switch to vftx_def.h
//#define VFTX_COARSE_SIZE          (TDC_FIFO_COARSE_CT>>TDC_FIFO_COARSE_CT_SHIFT)+1
#define VFTX_COARSE_OVERFLW_CHK   1

/**************** End FPGA TDC data unpacking definitions *************/

// Number of scalers in ScalOrMu
   // Oct12
//#define SCALORMU_NB_SCAL    16
   // Nov 12
#define SCALORMU_NB_SCAL    32

#endif // MBSCRATEDEFINES_H

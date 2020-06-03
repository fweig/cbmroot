#ifndef CBMRICHTRBDEFINES_H
#define CBMRICHTRBDEFINES_H


#define EVENT_TIME_WINDOW 300.          // Time window for building event in ns
#define NOF_RAW_EVENTS_IN_BUFFER 1000  // Maximum raw events in the buffer

// for nov2014 beamtime - 33 channels per TDC: 0th channel - reference, then 16 pairs (leading/trailing) for one PADIWA channel
#define TRB_TDC3_CHANNELS 33           //

#define TRB_TDC3_NUMTDC 4              // for nov2014 beamtime - 4

#define TRB_TDC3_NUMBOARDS 17          // for nov2014 beamtime - 17, for WU test setup - 1

#define TRB_TDC3_FINEBINS 0x400        //TODO check

#define TRB_TDC3_COARSEUNIT 5.          // 5 ns

#define TRB_TDC3_COARSEBINS 0x800      // 

//#define INVERSEPOLARITY		// if defined then leading channels are 2,4,6,... and corresponding trailing are 1,3,5,...

// Maximum TOT in ns to build a pair. If TOT is positive and more than this value then two separate edges are sent to output.
#define POSITIVEPAIRWINDOW 40.
// Maximum in abs of a negative TOT to build a pair. If TOT is negative and in abs more than this value then two separate edges are sent to output.
#define NEGATIVEPAIRWINDOW 5.
// Hence a valid TOT for a pair is [-NEGATIVEPAIRINDOW; POSITIVEPAIRWINDOW]

#define EVENTTIMEWINDOW 1000

#endif // CBMRICHTRBDEFINES_H

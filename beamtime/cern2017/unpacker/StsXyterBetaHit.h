/************************************************************
 *
 ************************************************************/

#ifndef STSXYTERBETAHIT_H
#define STSXYTERBETAHIT_H

// C/C++ headers
#include <stdint.h>
#include <iostream>

namespace stsxyter {

   class BetaHit {
      private:


      protected:
         uint64_t fulLongTs;   // (Software-)Extended TS, in clock cycles
         uint16_t fusRawAdc;   // Raw ADC value
         uint16_t fusAsicIdx;  // ASIC Index
         uint16_t fusChanIdx;  // channel Index

      public:

         BetaHit(const BetaHit& src) :
               fulLongTs(src.fulLongTs), fusRawAdc(src.fusRawAdc), fusAsicIdx(src.fusAsicIdx), fusChanIdx(src.fusChanIdx) {}

         BetaHit( uint64_t ulLongTsIn, uint16_t usRawAdcIn, uint16_t usAsicIdxIn = 0, uint16_t usChanIdxIn = 0 ) :
               fulLongTs( ulLongTsIn ), fusRawAdc( usRawAdcIn ), fusAsicIdx( usAsicIdxIn ), fusChanIdx( usChanIdxIn ) {}
         BetaHit() : BetaHit( 0, 0, 0, 0) {}

         virtual ~BetaHit() {};

         void assign(const BetaHit& src) { fulLongTs = src.fulLongTs; fusRawAdc = src.fusRawAdc;
                                           fusAsicIdx = src.fusAsicIdx; fusChanIdx = src.fusChanIdx;}

         BetaHit& operator=(const BetaHit& src) { assign(src); return *this; }
         bool     operator<(const BetaHit& other) const { return fulLongTs < other.fulLongTs; }

         inline void reset() { fulLongTs = 0; fusRawAdc = 0; fusAsicIdx = 0; fusChanIdx = 0; }

         inline uint64_t GetTs()  const { return fulLongTs; }
         inline uint16_t GetAdc() const { return fusRawAdc; }
         inline uint16_t GetAsic() const { return fusAsicIdx; }
         inline uint16_t GetChan() const { return fusChanIdx; }
   };
}
#endif // STSXYTERBETAHIT_H

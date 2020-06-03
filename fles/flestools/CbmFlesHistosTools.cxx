#include "CbmFlesHistosTools.h"

#include <cmath>
//#include <vector>

std::vector<double> GenerateLogBinArray( uint32_t uNbDecadesLog, uint32_t uNbStepsDecade, uint32_t uNbSubStepsInStep,
                                         uint32_t & uNbBinsLog )
{
   /// Logarithmic bining for self time comparison
   /// Number of log bins =
   ///      9 for the sub-unit decade
   ///    + 9 for each unit of each decade * 10 for the subdecade range
   ///    + 1 for the closing bin top edge
   uNbBinsLog = uNbStepsDecade
                + uNbStepsDecade * uNbSubStepsInStep * uNbDecadesLog;

   /// Need uNbBinsLog + 1 values as we need to provide the end of last bin
   uint32_t uArrayLength = uNbBinsLog + 1;
   double dBinsLog[ uArrayLength ];
   /// First fill sub-unit decade
   for( uint32_t uSubU = 0; uSubU < uNbStepsDecade; uSubU ++ ) {
     dBinsLog[ uSubU ] = 0.1 * ( 1 + uSubU );
   }

   /// Then fill the main decades
   double dSubstepSize = 1.0 / uNbSubStepsInStep;
   for( uint32_t uDecade = 0; uDecade < uNbDecadesLog; uDecade ++)
   {
      double dBase = std::pow( 10, uDecade );
      uint32_t uDecadeIdx = uNbStepsDecade
                           + uDecade * uNbStepsDecade * uNbSubStepsInStep;
      for( uint32_t uStep = 0; uStep < uNbStepsDecade; uStep++ )
      {
         uint32_t uStepIdx = uDecadeIdx + uStep * uNbSubStepsInStep;
         for( uint32_t uSubStep = 0; uSubStep < uNbSubStepsInStep; uSubStep++ ) {
            dBinsLog[ uStepIdx + uSubStep ] = dBase * (1 + uStep)
                                             + dBase * dSubstepSize * uSubStep;
         } // for( uint32_t uSubStep = 0; uSubStep < uNbSubStepsInStep; uSubStep++ )
      } // for( uint32_t uStep = 0; uStep < uNbStepsDecade; uStep++ )
   } // for( uint32_t uDecade = 0; uDecade < uNbDecadesLog; uDecade ++)
   dBinsLog[ uNbBinsLog ] = std::pow( 10, uNbDecadesLog );

   /// use vector instead
   std::vector<double> dBinsLogVect;

   for( uint32_t i = 0; i < uArrayLength; ++i )
   {
     dBinsLogVect.push_back(dBinsLog[i]);
   } // for( uint32_t i = 0; i < uArrayLength; ++i )

   return dBinsLogVect;
}

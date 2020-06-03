
#include "Rpc_Event.h"

#ifdef WITHGO4ROC
   #include "TGo4Version.h"
   #if __GO4BUILDVERSION__ > 40502
      #include "go4iostream.h"
   #else
      #include "Riostream.h"
   #endif
#endif // WITHGO4ROC


void Rpc_Hit::Clear(Option_t*)
{
   iStrip     = -1 ;
   dTimeLeft  = -1 ;
   dTotLeft   = -1 ;
   dTimeRight = -1 ;
   dTotRight  = -1 ;
   fbMultiEdge = kFALSE;
}
Double_t Rpc_Hit::PositionCalculation( Double_t dTimeConv )
{
   return (dTimeRight - dTimeLeft)*dTimeConv/1000.0;
}

void Rpc_Cluster::Clear(Option_t*)
{
   fHits.clear();
   dX = 0.0;
   dY = 0.0;
   dMeanTime = 0.0;
}
void Rpc_Cluster::PositionCalculation(
      Double_t dMiddleStrip, UInt_t uOrientation,
      Double_t dStripWidth,  Double_t dTimeConv,
      UInt_t uUseGravityCenter  )
{
   Double_t dTotSum  = 0.0;
   Double_t dMeanTot = 0.0;
   if( 0 == fHits.size() )
      return;

   if( 1 == uOrientation )
   {
      dX = 0.0;
      dY = 0.0;
      dMeanTime = 0;
      dMeanStrip= 0;
      for( UInt_t uHit = 0; uHit < fHits.size(); uHit++)
      {
         if( 1 == uUseGravityCenter )
            dMeanTot = (fHits[uHit].dTotLeft ) + (fHits[uHit].dTotRight ); // Weighted
            else dMeanTot = 1;                                             // Not Weighted
         // Along strips axis: Strip Index + 0.5 to sit in the middle of the strip!
         dX         += (Double_t)(fHits[uHit].iStrip + 0.5)*dMeanTot;
         dY         += ( (fHits[uHit].dTimeRight) - (fHits[uHit].dTimeLeft) )*dMeanTot;
         dMeanTime  += ( (fHits[uHit].dTimeRight) + (fHits[uHit].dTimeLeft) )*dMeanTot/2.0;
         dTotSum    += dMeanTot;
         dMeanStrip += (Double_t)(fHits[uHit].iStrip)*dMeanTot;
      } // for( UInt_t uHit = 0; uHit < fHits.size(); uHit++)
      dX  = (dX/dTotSum)*dStripWidth -dMiddleStrip;
      dY *= dTimeConv/(1000.0*dTotSum);
      dMeanTime  /= dTotSum;
      dMeanStrip /= dTotSum;
   } // if( 1 == uOrientation )
      else
      {
         dX = 0.0;
         dY = 0.0;
         dMeanTime = 0;
         dMeanStrip= 0;
         for( UInt_t uHit = 0; uHit < fHits.size(); uHit++)
         {
            if( 1 == uUseGravityCenter )
               dMeanTot = (fHits[uHit].dTotLeft ) + (fHits[uHit].dTotRight ); // Weighted
               else dMeanTot = 1;                                             // Not Weighted
            dX         += ( (fHits[uHit].dTimeRight) - (fHits[uHit].dTimeLeft) )*dMeanTot;
            // Along strips axis: Strip Index + 0.5 to sit in the middle of the strip!
            dY         += (Double_t)(fHits[uHit].iStrip + 0.5)*dMeanTot;
            dMeanTime  += ( (fHits[uHit].dTimeRight) + (fHits[uHit].dTimeLeft) )*dMeanTot/2.0;
            dTotSum    += dMeanTot;
            dMeanStrip += (Double_t)(fHits[uHit].iStrip)*dMeanTot;
         } // for( UInt_t uHit = 0; uHit < fHits.size(); uHit++)
         dX *= dTimeConv/(1000.0*dTotSum);
         dY  = (dY/dTotSum)*dStripWidth -dMiddleStrip;
         dMeanTime  /= dTotSum;
         dMeanStrip /= dTotSum;
      } // else of if( 1 == uOrientation )
}

/* Real Event */
void  Rpc_Event::Clear(Option_t*)
{
  // all members should be cleared.
   fClusters.clear();
   fuEventNumber = 0;
   fbMultiEdgesPresent = kFALSE;
}

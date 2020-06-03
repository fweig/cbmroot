/** CbmRunAna.cxx
 *@author Volker Friese v.friese@gsi.de
 *@since 10.12.2009
 *@version 1.0
 **/


#include "CbmRunAna.h"

#include "FairRootManager.h"

#include <iostream>

using std::cout;
using std::endl;



// -----   Constructor   -----------------------------------------------------
CbmRunAna::CbmRunAna() 
  : FairRunAna(),
    fAsync(kFALSE),
    fMarkFill(kFALSE)
{
}
// ---------------------------------------------------------------------------



// -----   Destructor   ------------------------------------------------------
CbmRunAna::~CbmRunAna() {
}
// ---------------------------------------------------------------------------


// -----   Fill output tree   ------------------------------------------------
void CbmRunAna::Fill() {

        if ( fAsync ) return;
	fRootManager->Fill();

}
// ---------------------------------------------------------------------------


    
ClassImp(CbmRunAna)

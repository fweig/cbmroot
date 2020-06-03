/** CbmEvent.cxx
 ** @author V.Friese <v.friese@gsi.de>
 ** @since 17.09.2016
 ** @version 1.0
 **/
#include "CbmEvent.h"

#include <iostream>             // for operator<<, basic_ostream
#include <sstream>              // for stringstream
#include <string>               // for char_traits
#include <utility>              // for pair


// -----   Add data to event   ---------------------------------------------
void CbmEvent::AddData(ECbmDataType type, UInt_t index) {

	fIndexMap[type].push_back(index);
	fNofData++;

}
// -------------------------------------------------------------------------



// -----   Get a data index   ----------------------------------------------
UInt_t CbmEvent::GetIndex(ECbmDataType type, UInt_t iData) {

	if ( fIndexMap.find(type) == fIndexMap.end() ) return -1;
	if ( fIndexMap[type].size() <= iData ) return -2;
	return fIndexMap.at(type)[iData];

}
// -------------------------------------------------------------------------



// -----   Get number of data of a type in this event   --------------------
Int_t CbmEvent::GetNofData(ECbmDataType type) const {

	if ( fIndexMap.find(type) == fIndexMap.end() ) return -1;
	else return fIndexMap.at(type).size();

}
// -------------------------------------------------------------------------



// -----   Set the vertex parameters   -------------------------------------
void CbmEvent::SetVertex(Double_t x, Double_t y, Double_t z, Double_t chi2,
	 Int_t ndf, Int_t nTracks, const TMatrixFSym& covMat) {
	fVertex.SetVertex(x, y, z, chi2, ndf, nTracks, covMat);
}
// -------------------------------------------------------------------------



// -----   String output   -------------------------------------------------
std::string CbmEvent::ToString() const
{
   std::stringstream ss;
   ss << "Event " << fNumber << " at t = " << fTimeStart
  		<< " ns. Registered data types: " << fIndexMap.size()
  		<< ", data objects: " << fNofData << "\n";
   for (auto it = fIndexMap.begin(); it != fIndexMap.end(); it++) {
  	 ss << "          -- Data type " << it->first
  			<< ", number of data " << it->second.size() << "\n";
   }
  return ss.str();
}
// -------------------------------------------------------------------------



ClassImp(CbmEvent)

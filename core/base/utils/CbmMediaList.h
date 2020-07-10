/** @brief CbmMediaList
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @since 12.03.2020
 ** @date 12 March 2030
 **
 ** Class to allow to stream a vector of pairs needed by the check_media.C macro.
 ** The information is needed to check if there are changes in the media of TGeoNodes
 ** between the geometries of the detectors at creation of the ROOT geometry files and
 ** the final CbmRoot geometry.
 **/

#ifndef CBMMEDIALIST_H_
#define CBMMEDIALIST_H_

#include <Rtypes.h>   // for THashConsistencyHolder, ClassDef
#include <TObject.h>  // for TObject
#include <TString.h>  // for TString

#include <utility>  // for pair
#include <vector>   // for vector

class CbmMediaList : public TObject {
public:
  /**  Constructor  **/
  CbmMediaList() = default;


  /** @brief add new pair of geometry path and media information 
     ** @param Full geometry path for the TGeoNode
     ** @param Media information of the TGeoNode
     **/
  void AddEntry(TString, TString);

  /** @brief Get the stored information 
     ** @return Reference to the vector of pairs
     **/
  const std::vector<std::pair<TString, TString>>& GetVector() {
    return fMatList;
  }

private:
  std::vector<std::pair<TString, TString>> fMatList;

  ClassDef(CbmMediaList, 1)
};

#endif
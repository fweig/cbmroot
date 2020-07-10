#include "CbmTrdParSetDigi.h"

#include "CbmTrdParMod.h"      // for CbmTrdParMod
#include "CbmTrdParModDigi.h"  // for CbmTrdParModDigi

#include <FairLogger.h>  // for FairLogger, Logger, LOG, Severity, Severity::debug
#include <FairParamList.h>  // for FairParamList

#include <TArrayD.h>  // for TArrayD
#include <TArrayI.h>  // for TArrayI
#include <TString.h>  // for TString, Form

#include <map>      // for map, map<>::iterator, operator!=, __m...
#include <utility>  // for pair

CbmTrdParSetDigi::CbmTrdParSetDigi(const char* name,
                                   const char* title,
                                   const char* context)
  : CbmTrdParSet(name, title, context)

{}

//_______________________________________________________________________________
Bool_t CbmTrdParSetDigi::getParams(FairParamList* l) {
  if (!l) return kFALSE;
  if (!l->fill("NrOfModules", &fNrOfModules)) {
    LOG(error) << GetName() << "::getParams : Couldn't find \"NrOfModules\"";
    return kFALSE;
  }
  Int_t maxSectors(0);
  if (!l->fill("MaxSectors", &maxSectors)) {
    LOG(error) << GetName() << "::getParams : Couldn't find \"MaxSectors\"";
    return kFALSE;
  }
  TArrayI moduleId(fNrOfModules);
  if (!l->fill("ModuleIdArray", &moduleId)) {
    LOG(error) << GetName() << "::getParams : Couldn't find \"MaxSectors\"";
    return kFALSE;
  }
  // Instead of a fixed number of values the number of values to
  // store now depends on the maximum number of sectors per module
  Int_t nrValues = 10 + (maxSectors * 4);
  TArrayD values(nrValues);
  TArrayD sectorSizeX(maxSectors);
  TArrayD sectorSizeY(maxSectors);
  TArrayD padSizeX(maxSectors);
  TArrayD padSizeY(maxSectors);
  Int_t orientation;
  Double_t x;
  Double_t y;
  Double_t z;
  Double_t sizex;
  Double_t sizey;
  Double_t sizez;
  Double_t awPitch(-1.);
  Double_t awPP(-1.);
  Double_t awOff(-1.);
  //  Int_t roType(0);

  TString text;
  for (Int_t i = 0; i < fNrOfModules; i++) {
    if (!l->fill(Form("%d", moduleId[i]), &values)) {
      LOG(error) << GetName()
                 << "::getParams : Missing parameter definiton for module "
                 << moduleId[i];
      continue;
    }
    Int_t k(0);
    orientation = values[k++];
    //    roType=values[k++];
    awPitch = values[k++];
    awPP    = values[k++];
    awOff   = values[k++];
    x       = values[k++];
    y       = values[k++];
    z       = values[k++];
    sizex   = values[k++];
    sizey   = values[k++];
    sizez   = values[k++];
    for (Int_t j = 0; j < maxSectors; j++) {
      sectorSizeX.AddAt(values[k++], j);
      sectorSizeY.AddAt(values[k++], j);
      padSizeX.AddAt(values[k++], j);
      padSizeY.AddAt(values[k++], j);
    }
    fModuleMap[moduleId[i]] = new CbmTrdParModDigi(x,
                                                   y,
                                                   z,
                                                   sizex,
                                                   sizey,
                                                   sizez,
                                                   maxSectors,
                                                   orientation,
                                                   sectorSizeX,
                                                   sectorSizeY,
                                                   padSizeX,
                                                   padSizeY);
    fModuleMap[moduleId[i]]->SetModuleId(moduleId[i]);
    ((CbmTrdParModDigi*) fModuleMap[moduleId[i]])
      ->SetAnodeWireToPadPlaneDistance(awPP);  //>0?awPP:0.35);
    ((CbmTrdParModDigi*) fModuleMap[moduleId[i]])
      ->SetAnodeWireOffset(awOff);  //>0?awOff:0.375);
    ((CbmTrdParModDigi*) fModuleMap[moduleId[i]])
      ->SetAnodeWireSpacing(awPitch);  //>0?awPitch:0.25);
    if (FairLogger::GetLogger()->IsLogNeeded(fair::Severity::debug))
      fModuleMap[moduleId[i]]->Print();
  }
  return kTRUE;
}

//_____________________________________________________________________
void CbmTrdParSetDigi::putParams(FairParamList* l) {
  /**  
   Instead of a fixed number of values the number of values to
   store now depends on the maximum number of sectors per module
   The first eleven parameters are for the complete module.  
   The parametrs are:
   orientation         : module rotation in 90deg
   fAnodeWireSpacing   : width of amplification cell in cm
   fAnodeWireToPadPlaneDistance : anode 2 pad plane distance in cm   
   fAnodeWireOffset    : offset of first anode wire wrt pad plane   
   X, Y, Z             : position of the middle of the gaslayer.
   SizeX, SizeY, SizeZ : size of the gaslayer. The values are only
                         the half size which are the values returned
                         by geant.
   
   The rest of the parameters depend on the number of sectors.
   SectorSizeX(Y)      : size of a sector
   PadSizeX(Y)         : size of the pads in this sector
*/

  if (!l) return;
  LOG(info) << GetName() << "::putParams(FairParamList*)";

  Int_t maxSectors(0), idx(0);
  TArrayI moduleIdArray(fNrOfModules);
  for (std::map<Int_t, CbmTrdParMod*>::iterator imod = fModuleMap.begin();
       imod != fModuleMap.end();
       imod++) {
    moduleIdArray[idx++] = imod->first;
    Int_t sectors        = ((CbmTrdParModDigi*) imod->second)->GetNofSectors();
    if (sectors > maxSectors) maxSectors = sectors;
  }

  l->add("NrOfModules", fNrOfModules);
  l->add("MaxSectors", maxSectors);
  l->add("ModuleIdArray", moduleIdArray);


  Int_t nrValues = 10 + (maxSectors * 4);
  TArrayD values(nrValues);
  CbmTrdParModDigi* mod(nullptr);
  for (Int_t i = 0; i < fNrOfModules; i++) {
    Int_t k(0);
    mod = (CbmTrdParModDigi*) fModuleMap[moduleIdArray[i]];
    values.AddAt(mod->GetOrientation(), k++);
    //      Int_t roType(0);
    //      if(mod->GetPadGeoTriangular()) roType |= 1;
    //      if(mod->GetAsicFASP()) roType |= 2;
    //      values.AddAt(roType, k++);
    values.AddAt(mod->GetAnodeWireSpacing(), k++);
    values.AddAt(mod->GetAnodeWireToPadPlaneDistance(), k++);
    values.AddAt(mod->GetAnodeWireOffset(), k++);
    values.AddAt(mod->GetX(), k++);
    values.AddAt(mod->GetY(), k++);
    values.AddAt(mod->GetZ(), k++);
    values.AddAt(mod->GetSizeX(), k++);
    values.AddAt(mod->GetSizeY(), k++);
    values.AddAt(mod->GetSizeZ(), k++);
    for (Int_t j = 0; j < maxSectors; j++) {
      values.AddAt(mod->GetSectorSizeX(j), k++);
      values.AddAt(mod->GetSectorSizeY(j), k++);
      values.AddAt(mod->GetPadSizeX(j), k++);
      values.AddAt(mod->GetPadSizeY(j), k++);
    }

    TString text;
    text += moduleIdArray[i];
    l->add(text.Data(), values);
  }
}

ClassImp(CbmTrdParSetDigi)

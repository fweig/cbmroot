#include "CbmTrdParModDigi.h"

#include "CbmTrdAddress.h"  // for CbmTrdAddress
#include "CbmTrdPoint.h"    // for CbmTrdPoint

#include <Logger.h>  // for LOG, Severity, Severity::debug2

#include <TGeoManager.h>  // for TGeoManager, gGeoManager
#include <TString.h>      // for TString
#include <TVector3.h>     // for TVector3

#include <iomanip>  // for setprecision, __iom_t5

#include <math.h>    // for fabs
#include <stdio.h>   // for printf
#include <string.h>  // for strcmp

//___________________________________________________________________
CbmTrdParModDigi::CbmTrdParModDigi()
  : CbmTrdParMod("CbmTrdParModDigi", "TRD read-out definition")
  , fNofSectors(1)
  , fOrientation(0)
  , fAnodeWireOffset(0.375)
  , fAnodeWireSpacing(0.25)
  , fAnodeWireToPadPlaneDistance(0.35)
  , fX(0.)
  , fY(0.)
  , fZ(0.)
  , fSizeX(0)
  , fSizeY(0)
  , fSizeZ(0)
  , fSectorX()
  , fSectorY()
  , fSectorZ()
  , fSectorBeginX()
  , fSectorBeginY()
  , fSectorEndX()
  , fSectorEndY()
  , fSectorSizeX()
  , fSectorSizeY()
  , fPadSizeX()
  , fPadSizeY()
{
}
//___________________________________________________________________
CbmTrdParModDigi::CbmTrdParModDigi(Double_t x, Double_t y, Double_t z, Double_t sizex, Double_t sizey, Double_t sizez,
                                   Int_t nofSectors, Int_t orientation, const TArrayD& sectorSizeX,
                                   const TArrayD& sectorSizeY, const TArrayD& padSizeX, const TArrayD& padSizeY)
  : CbmTrdParMod("CbmTrdParModDigi", "TRD read-out definition")
  , fNofSectors(nofSectors)
  , fOrientation(orientation)
  , fAnodeWireOffset(0.375)
  , fAnodeWireSpacing(0.25)
  , fAnodeWireToPadPlaneDistance(0.35)
  , fX(x)
  , fY(y)
  , fZ(z)
  , fSizeX(sizex)
  , fSizeY(sizey)
  , fSizeZ(sizez)
  , fSectorX(nofSectors)
  , fSectorY(nofSectors)
  , fSectorZ(nofSectors)
  , fSectorBeginX(nofSectors)
  , fSectorBeginY(nofSectors)
  , fSectorEndX(nofSectors)
  , fSectorEndY(nofSectors)
  , fSectorSizeX(sectorSizeX)
  , fSectorSizeY(sectorSizeY)
  , fPadSizeX(padSizeX)
  , fPadSizeY(padSizeY)
{
  /**  
  Calculate the coordinates of the begin and the end of each sector
  as well as the coordinates of the center of the sector
  In the moment it is assumed that there are sectors which have either
  in x- or y-direction the size of the chamber.
*/
  if (nofSectors > 1) {
    Double_t beginx, beginy, endx, endy;
    Double_t summed_sectsize;
    if (sectorSizeX.GetAt(0) == 2 * sizex) {  //substructure only in y-direction
      beginx          = 0;
      endx            = 2 * sizex;
      summed_sectsize = 0;
      for (Int_t i = 0; i < fNofSectors; i++) {
        if (0 == i) {
          beginy = 0.;
          endy   = sectorSizeY.GetAt(i);
          summed_sectsize += sectorSizeY.GetAt(i);
        }
        else {
          beginy = summed_sectsize;
          endy   = summed_sectsize += sectorSizeY.GetAt(i);
        }
        fSectorBeginX.AddAt(beginx, i);
        fSectorBeginY.AddAt(beginy, i);
        fSectorEndX.AddAt(endx, i);
        fSectorEndY.AddAt(endy, i);
        fSectorX.AddAt(beginx + (sectorSizeX.GetAt(i) / 2.), i);
        fSectorY.AddAt(beginy + (sectorSizeY.GetAt(i) / 2.), i);
        fSectorZ.AddAt(fZ, i);
      }
    }
    else {
      LOG(warn) << GetName()
                << "::CbmTrdParModDigi - detect different information on "
                   "module size x : geometry="
                << std::setprecision(5) << 2 * sizex << " CbmTrdPads.h=" << sectorSizeX.GetAt(0);
      beginy          = 0;
      endy            = 2 * sizey;
      summed_sectsize = 0;
      for (Int_t i = 0; i < fNofSectors; i++) {
        if (0 == i) {
          beginx = 0.;
          endx   = sectorSizeX.GetAt(i);
          summed_sectsize += sectorSizeX.GetAt(i);
        }
        else {
          beginx = summed_sectsize;
          endx   = summed_sectsize += sectorSizeX.GetAt(i);
        }
        fSectorBeginX.AddAt(beginx, i);
        fSectorBeginY.AddAt(beginy, i);
        fSectorEndX.AddAt(endx, i);
        fSectorEndY.AddAt(endy, i);
        fSectorX.AddAt(beginx + (sectorSizeX.GetAt(i) / 2.), i);
        fSectorY.AddAt(beginy + (sectorSizeY.GetAt(i) / 2.), i);
        fSectorZ.AddAt(fZ, i);
      }
    }
  }
  else {
    fSectorBeginX.AddAt(0., 0);
    fSectorBeginY.AddAt(0., 0);
    fSectorEndX.AddAt(sectorSizeX.GetAt(0), 0);
    fSectorEndY.AddAt(sectorSizeY.GetAt(0), 0);
    fSectorX.AddAt(x, 0);
    fSectorY.AddAt(y, 0);
    fSectorZ.AddAt(z, 0);
  }
}

//___________________________________________________________________________
void CbmTrdParModDigi::Print(Option_t* opt) const
{
  /** 
  Dump formated parameters for this module
*/
  printf(" CbmTrdParModDigi @ %5d ly[%d] idLy[%3d] rotation[%3ddeg] rows[%2d] "
         "cols[%3d]\n",
         fModuleId, CbmTrdAddress::GetLayerId(fModuleId), CbmTrdAddress::GetModuleId(fModuleId), fOrientation * 90,
         GetNofRows(), GetNofColumns());
  if (strcmp(opt, "all") != 0) return;
  printf("        center   [%7.2f %7.2f %7.2f]\n"
         "        size     [%7.2f %7.2f %7.2f]\n"
         "        anode    pitch[%5.3f] d2PP[%5.3f] off[%5.3f]\n"
         "        pads    ",
         fX, fY, fZ, fSizeX, fSizeY, fSizeZ, fAnodeWireSpacing, fAnodeWireToPadPlaneDistance, fAnodeWireOffset);
  for (Int_t isec(0); isec < fNofSectors; isec++)
    printf(" [c(%3d) r(%2d)]x[w(%4.2f) h(%5.2f)] area[%5.2fx%5.2f] limits "
           "x[%5.2f %5.2f] y[%5.2f %5.2f]\n                ",
           GetNofColumnsInSector(isec), GetNofRowsInSector(isec), fPadSizeX.At(isec), fPadSizeY.At(isec),
           fSectorSizeX[isec], fSectorSizeY[isec], fSectorBeginX[isec], fSectorEndX[isec], fSectorBeginY[isec],
           fSectorEndY[isec]);
  printf("\n");
}

//___________________________________________________________________________
void CbmTrdParModDigi::ProjectPositionToNextAnodeWire(Double_t* local_point) const
{
  /**
  Move the local point along y to the next anode wire position.

  20131009 - DE - checked OK
*/

  if (/*fAnodeWireOffset > 0.0 && */ fAnodeWireSpacing > 0.0) {  // wires must be defined defined

    // check, if the input is within the allowed range
    if (fabs(local_point[1]) - fSizeY > 1.e-3)
      LOG(error) << "CbmTrdParModDigi::ProjectPositionToNextAnodeWire - local "
                    "point must be within gas volume, y="
                 << std::setprecision(5) << local_point[1] << " module size " << fSizeY;

    Double_t ypos = local_point[1];

    LOG(debug2) << "local y before projection: " << std::setprecision(5) << local_point[1] << " cm";

    // make sure, local_point[1] is not negative (due to module center coordinate)
    // therefore transform to local corner first and then back at the end of operation
    local_point[1] += fSizeY;  // transform to module corner coordinates

    local_point[1] = Int_t(((local_point[1] - fAnodeWireOffset) / fAnodeWireSpacing) + 0.5) * fAnodeWireSpacing
                     + fAnodeWireOffset;  // find closest anode wire

    if (local_point[1] > 2 * fSizeY - fAnodeWireOffset)
      local_point[1] = 2 * fSizeY - fAnodeWireOffset;  // move inwards to the last anode wire

    local_point[1] -= fSizeY;  // transform back to module center coordinates

    LOG(debug2) << "local y after  projection: " << std::setprecision(5) << local_point[1] << " cm";

    // check, if we have left the anode wire grid
    if (fabs(local_point[1]) > fSizeY - fAnodeWireOffset)
      LOG(error) << "CbmTrdParModDigi::ProjectPositionToNextAnodeWire - local "
                    "point projected outside anode wire plane, from "
                 << std::setprecision(5) << ypos << " to " << std::setprecision(5) << local_point[1]
                 << " - last anode wire at " << std::setprecision(5) << fSizeY - fAnodeWireOffset;
  }
  else {
    LOG(error) << "CbmTrdParModDigi::ProjectPositionToNextAnodeWire: "
                  "fAnodeWireOffset and fAnodeWireSpacing not set. "
                  "ProjectPositionToNextAnodeWire can not be used.";
  }
}

//___________________________________________________________________________
Int_t CbmTrdParModDigi::GetSector(const Double_t* local_point) const
{
  /**
  Calculate the position in the chamber 
  with the origin of the local coordinate system 
  in the lower left corner of the chamber (looking upstream)
  x goes to the left, looking in beam direction, y goes upward
*/

  Double_t posx, posy;

  TransformToLocalCorner(local_point, posx, posy);
  for (Int_t i = 0; i < fNofSectors; i++) {
    if (posx >= fSectorBeginX[i] && posx <= fSectorEndX[i] && posy >= fSectorBeginY[i] && posy <= fSectorEndY[i]) {
      return i;
    }
  }

  LOG(error) << "CbmTrdParModDigi::GetSector: Could not find local point [" << local_point[0] << " " << local_point[1]
             << "] in any of the sectors";
  Print("all");
  return -1;  // This should be never reached
}

//___________________________________________________________________________
Int_t CbmTrdParModDigi::GetSector(Int_t npady, Int_t& rowId) const
{
  /**  
  Calculate the pad coordinates in this sector from pad coordinates in the module e.g. in which sector is pad (20/28)
*/

  if ((npady < 0) || (npady > GetNofRows() - 1)) {
    LOG(error) << "CbmTrdParModDigi::GetSector - there is no such row number: " << npady;
    return -1;
  }

  Int_t secRows = 0;  // number of rows in sector
  Int_t nofRows = 0;  // number of rows in total

  if (fSectorSizeY.At(0) < fSizeY) {                                              // if there are sectors in y direction
    for (Int_t iSector = 0; iSector < fNofSectors; iSector++) {                   // for each sector
      secRows = (Int_t)(fSectorSizeY.At(iSector) / fPadSizeY.At(iSector) + 0.5);  // need to round for correct result
      if (npady < nofRows + secRows) {  // if we are in the sector containing the pad
        // note nypad = 12 is not in sector 0, with rowIds 0..11
        rowId = npady - nofRows;

        LOG(debug2) << "npady   : " << npady << " <= " << nofRows + secRows << "  rowId " << rowId << "  nRows(sec-1) "
                    << nofRows << "  sec " << iSector;

        if ((rowId < 0) || (rowId > GetNofRowsInSector(iSector) - 1))
          LOG(fatal) << "CbmTrdParModDigi::GetModuleRow rowId " << rowId << " of "
                     << GetNofRowsInSector(iSector) - 1
                     //<< " in moduleAddress " << fModuleAddress
                     << " is out of bounds!";

        return iSector;  // return sector
      }
      nofRows += secRows;  // sum up new total number of rows
    }
  }
  LOG(error) << "CbmTrdParModDigi::GetSector: Could not find pad in any of the sectors";
  return -1;  // This should be never reached
}


//___________________________________________________________________________
Int_t CbmTrdParModDigi::GetNofColumns() const
{
  /**
 * Get total number of pad columns in module  
 */

  Int_t nofColumns = 0;
  if (fSectorSizeX.At(0) < fSizeX) {
    for (Int_t i = 0; i < fNofSectors; i++) {
      nofColumns += (Int_t)(fSectorSizeX.At(i) / fPadSizeX.At(i) + 0.5);  // need to round for correct result
    }
  }
  else {
    nofColumns = (Int_t)(fSectorSizeX.At(0) / fPadSizeX.At(0) + 0.5);  // need to round for correct result
  }
  return nofColumns;
}

//___________________________________________________________________________
Int_t CbmTrdParModDigi::GetNofRows() const
{
  /** 
 * Get total number of pad rows in module  
*/
  Int_t nofRows = 0;
  if (fSectorSizeY.At(0) < fSizeY) {
    for (Int_t i = 0; i < fNofSectors; i++) {
      nofRows += (Int_t)(fSectorSizeY.At(i) / fPadSizeY.At(i) + 0.5);  // need to round for correct result
    }
  }
  else {
    nofRows = (Int_t)(fSectorSizeY.At(0) / fPadSizeY.At(0) + 0.5);  // need to round for correct result
  }
  return nofRows;
}


//___________________________________________________________________________
Int_t CbmTrdParModDigi::GetNofColumnsInSector(Int_t i) const
{
  return (Int_t)(fSectorSizeX.At(i) / fPadSizeX.At(i) + 0.5);  // need to round for correct result
}


//___________________________________________________________________________
Int_t CbmTrdParModDigi::GetNofRowsInSector(Int_t i) const
{
  return (Int_t)(fSectorSizeY.At(i) / fPadSizeY.At(i) + 0.5);  // need to round for correct result
}


//___________________________________________________________________________
Int_t CbmTrdParModDigi::GetModuleRow(Int_t& sectorId, Int_t& rowId) const
{

  // check limits
  if ((sectorId < 0) || (sectorId > GetNofSectors() - 1))
    LOG(fatal) << "CbmTrdParModDigi::GetModuleRow sectorId " << sectorId << " of "
               << GetNofSectors() - 1
               //<< " in moduleAddress " << fModuleAddress
               << " is out of bounds!";

  // check limits
  if ((rowId < 0) || (rowId > GetNofRowsInSector(sectorId) - 1))
    LOG(fatal) << "CbmTrdParModDigi::GetModuleRow rowId " << rowId << " of "
               << GetNofRowsInSector(sectorId) - 1
               //<< " in moduleAddress " << fModuleAddress
               << " is out of bounds!";

  Int_t moduleRowId = rowId;

  // calculate row number within module
  for (Int_t iSector = 0; iSector < sectorId; iSector++) {
    moduleRowId += (Int_t)(fSectorSizeY.At(iSector) / fPadSizeY.At(iSector) + 0.5);
    //     LOG(info) << "adding sector "   << iSector << " of " << sectorId;
  }

  //   if (sectorId == 0)
  //     // do nothing
  //   if (sectorId >= 1)
  //     moduleRowId += (Int_t)(fSectorSizeY.At(0) / fPadSizeY.At(0) + 0.5);
  //   if (sectorId >= 2)
  //     moduleRowId += (Int_t)(fSectorSizeY.At(1) / fPadSizeY.At(1) + 0.5);

  return moduleRowId;
}


//___________________________________________________________________________
Int_t CbmTrdParModDigi::GetSectorRow(Int_t growId, Int_t& rowId) const
{
  // check limits
  if ((growId < 0) || (growId >= GetNofRows()))
    LOG(fatal) << "CbmTrdParModDigi::GetSectorRow rowId " << growId << " of "
               << GetNofRows()
               //<< " in moduleAddress " << fModuleAddress
               << " is out of bounds!";

  rowId = growId;

  // calculate row number within sector
  Int_t iSector(0);
  for (; iSector < 3; iSector++) {
    Int_t nr(GetNofRowsInSector(iSector));
    if (rowId - nr + 1 > 0) rowId -= nr;
    else
      break;
  }

  return iSector;
}


//___________________________________________________________________________
Bool_t CbmTrdParModDigi::GetPadInfo(const Double_t* local_point, Int_t& sectorId, Int_t& columnId, Int_t& rowId) const
{
  // check, if the input is within the allowed range
  if (fabs(local_point[0]) > fSizeX) {
    LOG(error) << "CbmTrdParModDigi::GetPadInfo - local point x must be within "
                  "gas volume, x="
               << std::setprecision(8) << local_point[0] << " fSizeX=" << std::setprecision(8) << fSizeX;
    return kFALSE;
  }
  if (fabs(local_point[1]) > fSizeY) {
    LOG(error) << "CbmTrdParModDigi::GetPadInfo - local point y must be within "
                  "gas volume, y="
               << std::setprecision(8) << local_point[1] << " fSizeY=" << std::setprecision(8) << fSizeY;
    return kFALSE;
  }

  Double_t posX, posY;
  TransformToLocalSector(local_point, posX, posY);

  // calculate in which sector the point is
  sectorId = GetSector(local_point);

  columnId = (Int_t)(posX / fPadSizeX.At(sectorId));
  rowId    = (Int_t)(posY / fPadSizeY.At(sectorId));

  return kTRUE;
}

//___________________________________________________________________________
void CbmTrdParModDigi::GetPadInfo(const CbmTrdPoint* trdPoint, Int_t& sectorId, Int_t& columnId, Int_t& rowId) const
{
  /**  
  Calculate point in the middle of the detector. This is
  for safety reasons, because the point at exit is slightly
  outside of the active volume. If one does not use a point
  in the detector one will get a wrong volume from the
  geomanager. Navigate to the correct volume to get all
  necessary information about this volume
*/
  Double_t x_mean = 0.5 * (trdPoint->GetXIn() + trdPoint->GetXOut());
  Double_t y_mean = 0.5 * (trdPoint->GetYIn() + trdPoint->GetYOut());
  Double_t z_mean = 0.5 * (trdPoint->GetZIn() + trdPoint->GetZOut());
  gGeoManager->FindNode(x_mean, y_mean, z_mean);

  TString curNode = gGeoManager->GetPath();
  if (!curNode.Contains("gas")) {
    // if the point is not in the gas volume print warning
    LOG(warn) << "This point is not in the trd gas";
    /*
    LOG(warn) << "x: " << std::setprecision(7) << trdPoint->GetXIn() << ", " << trdPoint->GetXOut();
    LOG(warn) << "y: " << std::setprecision(7) << trdPoint->GetYIn() << ", " << trdPoint->GetYOut();
    LOG(warn) << "z: " << std::setprecision(7) << trdPoint->GetZIn() << ", " << trdPoint->GetZOut();
    */
    sectorId = -1;
    return;
  }

  // Get the local point in local MC coordinates from
  // the geomanager. This coordinate system is rotated
  // if the chamber is rotated. This is corrected in
  // GetModuleInformation to have a
  // the same local coordinate system in all the chambers
  const Double_t* global_point = gGeoManager->GetCurrentPoint();
  Double_t local_point[3];

  gGeoManager->MasterToLocal(global_point, local_point);

  // 20131009 - DE - debuging output to check module orientation 0,1,2,3 with box generator
  if (fair::Logger::Logging(fair::Severity::debug2)) {
    // print module orientation
    LOG(debug2) << "module orientation: " << std::setprecision(5) << fOrientation;

    // print global coordinate
    LOG(debug2) << "global x: " << std::setprecision(5) << global_point[0] << " y: " << std::setprecision(5)
                << global_point[1] << " z: " << std::setprecision(5) << global_point[2];

    // print local coordinate - relative to module center
    LOG(debug2) << "local  x: " << std::setprecision(5) << local_point[0] << " y: " << std::setprecision(5)
                << local_point[1] << " z: " << std::setprecision(5) << local_point[2];

    // test projection to next anode wire
    Double_t proj_point[3];
    proj_point[0] = local_point[0];
    proj_point[1] = local_point[1];
    proj_point[2] = local_point[2];

    ProjectPositionToNextAnodeWire(proj_point);

    // print local coordinate - relative to module center
    LOG(debug2) << "proj   x: " << std::setprecision(5) << proj_point[0] << " y: " << std::setprecision(5)
                << proj_point[1] << " z: " << std::setprecision(5) << proj_point[2];

    Double_t corner_point[3];
    corner_point[2] = local_point[2];
    TransformToLocalCorner(local_point, corner_point[0], corner_point[1]);

    // print local coordinate - relative to module corner
    LOG(debug2) << "corner x: " << std::setprecision(5) << corner_point[0] << " y: " << std::setprecision(5)
                << corner_point[1] << " z: " << std::setprecision(5) << corner_point[2];

    LOG(debug2) << "pos    x: " << std::setprecision(5) << fX << " y: " << std::setprecision(5) << fY
                << " z: " << std::setprecision(5) << fZ << " ori: " << std::setprecision(5) << fOrientation;

    LOG(debug2) << "size/2 x: " << std::setprecision(5) << fSizeX << " y: " << std::setprecision(5) << fSizeY
                << " z: " << std::setprecision(5) << fSizeZ;

    Double_t sector_point[3];
    sector_point[2] = local_point[2];
    TransformToLocalSector(local_point, sector_point[0], sector_point[1]);

    // print local coordinate - relative to module sector
    LOG(debug2) << "sector x: " << std::setprecision(5) << sector_point[0] << " y: " << std::setprecision(5)
                << sector_point[1] << " z: " << std::setprecision(5) << sector_point[2];

    // calculate in which sector the point is
    sectorId = GetSector(local_point);
    LOG(debug2) << "sectornr: " << std::setprecision(5) << sectorId;

    LOG(debug2) << "ncol    : " << std::setprecision(5) << GetNofColumns();
    LOG(debug2) << "nrow    : " << std::setprecision(5) << GetNofRows();

    // print local coordinate - relative to module sector
    LOG(debug2) << "sec2   x: " << std::setprecision(5) << fSectorBeginX.GetAt(2) << " y: " << std::setprecision(5)
                << fSectorBeginY.GetAt(2);
    LOG(debug2) << "sec1   x: " << std::setprecision(5) << fSectorBeginX.GetAt(1) << " y: " << std::setprecision(5)
                << fSectorBeginY.GetAt(1);
    LOG(debug2) << "sec0   x: " << std::setprecision(5) << fSectorBeginX.GetAt(0) << " y: " << std::setprecision(5)
                << fSectorBeginY.GetAt(0);

    // get pad information
    Int_t s, c, r;
    GetPadInfo(local_point, s, c, r);
    LOG(debug2) << "pad sec: " << s << " col: " << c << " row: " << r;
  }  // debug2

  //  Int_t moduleAddress = CbmTrdAddress::GetModuleAddress(trdPoint->GetDetectorID());
  GetModuleInformation(
    /*moduleAddress, */ local_point, sectorId, columnId, rowId);
}


//___________________________________________________________________________
void CbmTrdParModDigi::TransformToLocalCorner(const Double_t* local_point, Double_t& posX, Double_t& posY) const
{
  /**  
  Transformation from local coordinate system 
  with origin in the middle of the module 
  into a system with the origin in the lower left corner (looking upstream).
  Since for both coordinate systems the orientation is the same 
  this is only a shift by the half size of the module in x- and y-direction

  20131009 - DE - checked OK for module orientations 0,1,2,3

*/
  posX = local_point[0] + fSizeX;
  posY = local_point[1] + fSizeY;
}

//___________________________________________________________________________
void CbmTrdParModDigi::TransformToLocalSector(const Double_t* local_point, Double_t& posX, Double_t& posY) const
{
  /**
   Transformation of the module coordinate system 
  with origin in the middle of the module 
  into a system with the origin in the lower left corner (looking upstream)
  of the sector the point is in. 
  First, transform in a system with origin in the lower left corner.

  20131009 - DE - checked OK for module orientations 0,1,2,3  
*/
  TransformToLocalCorner(local_point, posX, posY);
  Int_t sector = GetSector(local_point);
  posX -= fSectorBeginX.GetAt(sector);
  posY -= fSectorBeginY.GetAt(sector);
}


//___________________________________________________________________________
void CbmTrdParModDigi::TransformToLocalPad(const Double_t* local_point, Double_t& posX, Double_t& posY) const
{
  /**
  Transformation of the local module coordinate system 
  with origin in the middle of the module  
  into a system with the origin in the local pad center
  of the pad below the point.
  First, transform in a system with origin in the lower left sector.
*/
  Double_t sector_point[2];
  TransformToLocalSector(local_point, sector_point[0], sector_point[1]);

  //   TransformToLocalCorner(local_point, posX, posY);
  Int_t sectorId = GetSector(local_point);

  Double_t padx =
    (Int_t(sector_point[0] / fPadSizeX.At(sectorId)) + 0.5) * fPadSizeX.At(sectorId);  // x position of pad center
  Double_t pady =
    (Int_t(sector_point[1] / fPadSizeY.At(sectorId)) + 0.5) * fPadSizeY.At(sectorId);  // y position of pad center

  posX = sector_point[0] - padx;
  posY = sector_point[1] - pady;

  /*
  // print debug info
  LOG(info) << "sector x: " << std::setprecision(5) << sector_point[0] 
  << " y: " << std::setprecision(5) << sector_point[1];

  LOG(info) << "pad    x: " << std::setprecision(5) << padx 
  << " y: " << std::setprecision(5) << pady;

  LOG(info) << "diff   x: " << std::setprecision(5) << posX 
  << " y: " << std::setprecision(5) << posY;

  LOG(info) << "npad   x: " << std::setprecision(5) << padx / fPadSizeX.At(sectorId)
  << " y: " << std::setprecision(5) << pady / fPadSizeY.At(sectorId);
  */
}


//___________________________________________________________________________
void CbmTrdParModDigi::TransformHitError(TVector3& hitErr) const
{
  Double_t x, y;  // ,z;
  x = hitErr.X();
  y = hitErr.Y();
  //  z = hitErr.Z();

  //LOG(info) << "ori : " << fOrientation;

  if ((fOrientation == 1) || (fOrientation == 3)) {  // for orientations 1 or 3
    hitErr.SetX(y);                                  // swap errors
    hitErr.SetY(x);                                  // swap errors

    //    LOG(info) << " swapped x and y error";
    //    LOG(info) << "ori : " << fOrientation << " swapped x and y error";
  }
}


//___________________________________________________________________________
void CbmTrdParModDigi::GetModuleInformation(
  //Int_t moduleAddress,
  const Double_t* local_point, Int_t& sectorId, Int_t& columnId, Int_t& rowId) const
{
  // safety check. Up to now always correct, so could be removed.
  //   if (fModuleAddress != moduleAddress) {
  //     LOG(error) << "CbmTrdParModDigi::GetModuleInformation: This is wrong!";
  //   }

  Double_t posX, posY;
  TransformToLocalSector(local_point, posX, posY);

  // calculate in which sector the point is
  sectorId = GetSector(local_point);

  columnId = (Int_t)(posX / fPadSizeX.At(sectorId));
  rowId    = (Int_t)(posY / fPadSizeY.At(sectorId));
}


//___________________________________________________________________________
void CbmTrdParModDigi::GetPadPosition(const Int_t sector, const Int_t col, const Int_t row, TVector3& padPos,
                                      TVector3& padPosErr) const
{
  /** 
 * Get address of a pad, return position relative to module center
 */

  Double_t posX = 0;
  Double_t posY = 0;
  Double_t posZ = 0;

  Double_t padsizex = fPadSizeX.At(sector);
  Double_t padsizey = fPadSizeY.At(sector);

  // calculate position in sector coordinate system
  // with the origin in the lower left corner (looking upstream)
  posX = (((Double_t) col + 0.5) * padsizex);
  posY = (((Double_t) row + 0.5) * padsizey);

  // calculate position in module coordinate system
  // with the origin in the lower left corner (looking upstream)
  posX += fSectorBeginX.GetAt(sector);
  posY += fSectorBeginY.GetAt(sector);

  // calculate position in the module coordinate system
  // with origin in the middle of the module
  posX -= fSizeX;
  posY -= fSizeY;
  posZ = 0;

  // check limits
  if (fabs(posX) > fSizeX) LOG(fatal) << "CbmTrdParModDigi::GetPadPosition posX=" << posX << " is out of bounds!";
  // check limits
  if (fabs(posY) > fSizeY) LOG(fatal) << "CbmTrdParModDigi::GetPadPosition posY=" << posY << " is out of bounds!";

  padPos.SetXYZ(posX, posY, posZ);
  padPosErr.SetXYZ(padsizex / 2., padsizey / 2., 0.);
}


//___________________________________________________________________________
void CbmTrdParModDigi::GetPadPosition(const Int_t padAddress, TVector3& padPos, TVector3& padPosErr) const
{
  /** 
 * Get address of a pad, return position relative to module center
 */

  // Double_t posX = 0;
  // Double_t posY = 0;
  // Double_t posZ = 0;

  Int_t sectorId = CbmTrdAddress::GetSectorId(padAddress);
  Int_t rowId    = CbmTrdAddress::GetRowId(padAddress);
  Int_t columnId = CbmTrdAddress::GetColumnId(padAddress);

  return GetPadPosition(sectorId, rowId, columnId, padPos, padPosErr);
}

//___________________________________________________________________________
void CbmTrdParModDigi::GetPadPosition(const Int_t padAddress, bool isCbmTrdDigiAddress, TVector3& padPos,
                                      TVector3& padPosErr) const
{
  /** 
 * Get address of a pad, based on the channel address stored in the CbmTrdDigi
 */
  if (!isCbmTrdDigiAddress)
    LOG(error) << "Trying to get a CbmTrd PadPosition from DigiAddress format "
                  "function without digiAddress format";


  Int_t row = GetPadRow(padAddress);
  Int_t col = GetPadColumn(padAddress);

  Int_t srow(-1);
  Int_t sector = GetSectorRow(row, srow);

  return GetPadPosition(sector, col, srow, padPos, padPosErr);
}

//___________________________________________________________________________
void CbmTrdParModDigi::GetPosition(
  //         Int_t moduleAddress,
  Int_t sectorId, Int_t columnId, Int_t rowId, TVector3& padPos, TVector3& padSize) const
{
  /**  
  Calculate position in global coordinates 
  from digi information (sectorId, columnId, rowId).
  Returns two TVector3. One with the position
  and one with the padsize of the fired pad
*/
  //   if (fModuleAddress != moduleAddress) {
  //     LOG(error) << "CbmTrdParModDigi::GetPosition This is wrong!";
  //   }

  // check limits
  if ((sectorId < 0) || (sectorId > GetNofSectors() - 1))
    LOG(fatal) << "CbmTrdParModDigi::GetPosition sectorId " << sectorId << " of "
               << GetNofSectors() - 1  //<< " in moduleAddress " << moduleAddress
               << " is out of bounds!";

  // check limits
  if ((columnId < 0) || (columnId > GetNofColumnsInSector(sectorId) - 1))
    LOG(fatal) << "CbmTrdParModDigi::GetPosition columnId " << columnId << " of "
               << GetNofColumnsInSector(sectorId) - 1
               //<< " in moduleAddress " << moduleAddress
               << " is out of bounds!";

  // check limits
  if ((rowId < 0) || (rowId > GetNofRowsInSector(sectorId) - 1))
    LOG(fatal) << "CbmTrdParModDigi::GetPosition rowId " << rowId << " of "
               << GetNofRowsInSector(sectorId) - 1
               //<< " in moduleAddress " << moduleAddress
               << " is out of bounds!";

  Double_t local_point[3];
  Double_t padsizex = fPadSizeX.At(sectorId);
  Double_t padsizey = fPadSizeY.At(sectorId);

  // calculate position in sector coordinate system
  // with the origin in the lower left corner (looking upstream)
  local_point[0] = (((Double_t) columnId + 0.5) * padsizex);
  local_point[1] = (((Double_t) rowId + 0.5) * padsizey);

  // calculate position in module coordinate system
  // with the origin in the lower left corner (looking upstream)
  local_point[0] += fSectorBeginX.GetAt(sectorId);
  local_point[1] += fSectorBeginY.GetAt(sectorId);
  // local_point[i] must be >= 0 at this point

  // check limits
  if ((local_point[0] < 0) || (local_point[0] > 2 * fSizeX))
    LOG(fatal) << "CbmTrdParModDigi::GetPosition local_point[0]="
               << local_point[0]
               //<< " in moduleAddress " << moduleAddress
               << " is out of bounds!";
  // check limits
  if ((local_point[1] < 0) || (local_point[1] > 2 * fSizeY))
    LOG(fatal) << "CbmTrdParModDigi::GetPosition local_point[1]="
               << local_point[1]
               //<< " in moduleAddress " << moduleAddress
               << " is out of bounds!";

  // calculate position in the module coordinate system
  // with origin in the middle of the module
  local_point[0] -= fSizeX;
  local_point[1] -= fSizeY;
  local_point[2] = fSizeZ;

  // navigate to the correct module. (fX,fY,fZ)
  gGeoManager->FindNode(fX, fY, fZ);

  // get the local point in local MC coordinates from
  // the geomanager. This coordinate system is rotated,
  // if the chamber is rotated. This is corrected in
  // GetModuleInformation to have the same
  // local coordinate system in all the chambers
  Double_t global_point[3];  // global_point[3];
  gGeoManager->LocalToMaster(local_point, global_point);

  // calculate the position in the global coordinate system
  // with the origin in target
  Double_t posX = global_point[0];
  Double_t posY = global_point[1];
  Double_t posZ = global_point[2];

  padPos.SetXYZ(posX, posY, posZ);
  padSize.SetXYZ(padsizex, padsizey, 0.);
}

//___________________________________________________________________________
Int_t CbmTrdParModDigi::GetPadColumn(const Int_t channelNumber) const
{
  // calculate the pad column based on
  // the channeNumber as defined in the
  // CbmTrdDigi

  Int_t ncols = GetNofColumns();
  Int_t col   = channelNumber % ncols;

  return col;
}

//___________________________________________________________________________
Int_t CbmTrdParModDigi::GetPadRow(const Int_t channelNumber) const
{
  // calculate the pad row based on
  // the channeNumber as defined in the
  // CbmTrdDigi

  Int_t ncols = GetNofColumns();
  Int_t row   = channelNumber / ncols;

  return row;
}


ClassImp(CbmTrdParModDigi)

#ifndef CBMTRDTRIANGLEPRF_H
#define CBMTRDTRIANGLEPRF_H 1

#include <TObject.h>

#include <iostream>

class TF1;

//! Utility for converting energy to signal over the triangular pad geometry (Bucharest prototype)
/**
 * The class provides a binning which covers the pad plane area of NR x NC columns which might be activated by a particle. For each bin its center position in pad coordinates is stored in std vectors fX and fY. Three types of bins are known and saved in the fUp vector:
 * - bin in the upper triangle [1]
 * - bin in the lower triangle [-1]
 * - bin on the slit between pads [0]\n
 * 
 * A list of functions is provided to navigate the map with self explanatory names
 * NextBinX(), NextBinY(), PrevBinX(), PrevBinY(), GoToOriginX() and GoToOriginY()\n
 * A link to PRF is also provided via 2 TF1 fPRFx and fPRFy which should be initialized to the corresponding model.
 * 
 * Usage
 *-------
 * For each hit the class should be initialized with the position of the hit expresed in pad coordinates by calling SetOrigin(). The map is navigated and the current bin is tracked internally. User can access it via a call to GetCurrentBin() for the indexes or in terms of cols/rows with GetCurrentPad(). For the current hit one can interogate the PRF model via GetChargeFraction() function.\n
 *   
 * \author A.Bercuci <abercuci@niham.nipne.ro>
 **/
class CbmTrdTrianglePRF : public TObject {
public:
  enum CbmTrdTrianglePRFDef
  {
    NC = 2,  ///< no. of neighbor columns (except the hit) to be considered in cluster definition
    NR = 1   ///< no. of neighbor rows (except the hit) to be considered in cluster definition
  };
  /**
    * \brief Build map
    * \param W column width in cm
    * \param H row height in cm
    * \param n no of bins requested for the map per column/row 
    **/
  CbmTrdTrianglePRF(Float_t W, Float_t H, Int_t n = 20);
  virtual ~CbmTrdTrianglePRF();

  /**
    * \brief Find bin for point (x,y)
    * \param x coordinate along wires in pad coordinates
    * \param y coordinate across wires in pad coordinates
    * \param binx return bin index along wires in pad coordinates
    * \param biny return bin index across wires in pad coordinates
    * \return true if point is in the map
    **/
  Bool_t GetBin(Double_t x, Double_t y, Int_t& binx, Int_t& biny) const;
  /**
    * \brief Compute charge fraction on the current bin
    * \return value of charge fraction according to PRF parametrization times the bin area
    **/
  Double_t GetChargeFraction() const;
  /**
    * \brief Enquire the current status of the map iterator
    * \param bx on return contains x index of the current bin
    * \param by on return contains y index of the current bin
    **/
  void GetCurrentBin(Int_t& bx, Int_t& by) const
  {
    bx = fBinx;
    by = fBiny;
  }
  /**
    * \brief Compute the pad corresponding to current bin
    * \param col on return contains column offset wrt origin (0)
    * \param row on return contains row offset wrt origin (0)
    * \param up the type of triangular pad housing the current bin
    **/
  void GetCurrentPad(Int_t& col, Int_t& row, Int_t& u) const;
  /**
    * \brief Move current binx to the origin binx0
    **/
  void GoToOriginX() { fBinx = fBinx0; }
  /**
    * \brief Move current biny to the origin biny0
    **/
  void GoToOriginY() { fBiny = fBiny0; }
  /**
    * \brief Move current bin to the right
    * \return true if still in map
    **/
  Bool_t NextBinX();
  /**
    * \brief Move current bin up
    * \return true if still in map
    **/
  Bool_t NextBinY();
  Double_t Norm() const { return fNorm; }
  void Print(Option_t* opt = "") const;
  /**
    * \brief Move current bin to the left
    * \return true if still in map
    **/
  Bool_t PrevBinX();
  /**
    * \brief Move current bin down
    * \return true if still in map
    **/
  Bool_t PrevBinY();
  /**
    * \brief Set map offset @ point (x,y)
    * \param x coordinate along wires in pad coordinates
    * \param y coordinate across wires in pad coordinates
    * \return true if point is in the map
    **/
  Bool_t SetOrigin(Double_t x, Double_t y);

private:
  CbmTrdTrianglePRF(const CbmTrdTrianglePRF& ref);
  CbmTrdTrianglePRF& operator=(const CbmTrdTrianglePRF& ref);
  /**
    * \brief Define triangular pad type
    * \param x position along wires in column coordinates
    * \param y position across wires in column coordinates
    * \return 1 for upper pad and -1 for bottom 
    **/
  Int_t GetSide(const Float_t x, const Float_t y) const;

  Int_t fN;  ///< no of bins across wires
  /// Current bin in the map - x direction
  Int_t fBinx;  //! Index of current bin in the map - x direction
  /// Offset bin in the map - x direction
  Int_t fBinx0;  //! Index of bin in the map corresponding to cluster center - x direction
  /// Current bin in the map - y direction
  Int_t fBiny;  //! Index of current bin in the map - y direction
  /// Offset bin in the map - y direction
  Int_t fBiny0;              //! Index of bin in the map corresponding to cluster center - y direction
  Double_t fX0;              ///< x coordinate of cluster
  Double_t fY0;              ///< y coordinate of cluster
  Double_t fW;               ///< pad width
  Double_t fH;               ///< pad height
  Double_t fdW;              ///< bin half width
  Double_t fdH;              ///< bin half height
  Double_t fSlope;           ///< slope of triangle H/W
  Double_t fNorm;            ///< normalization factor for the 2D Gauss distribution
  std::vector<Char_t> fUp;   ///< 1 for the upper pad, -1 for the bottom pad and 0 on the boundary
  std::vector<Double_t> fX;  ///< position of bin center along wires
  std::vector<Double_t> fY;  ///< position of bin center across wires
  TF1* fPRFx;                ///< PRF model along wires
  TF1* fPRFy;                ///< PRF model across wires

  ClassDef(CbmTrdTrianglePRF, 1)
};
#endif

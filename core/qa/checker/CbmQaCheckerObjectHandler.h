/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCheckerObjectHandler.h
/// @brief  Base handler class (declaration)
/// @author S. Zharko <s.zharko@gsi.de>
/// @since  08.02.2023

#ifndef CbmQaCheckerObjectHandler_h
#define CbmQaCheckerObjectHandler_h 1

#include "CbmQaCheckerObjectDB.h"
#include "CbmQaCheckerTypedefs.h"

#include "Rtypes.h"

#include <memory>
#include <string>

class TDirectory;
class TNamed;
class TObject;
class TCanvas;
class CbmQaCanvas;

namespace cbm::qa::checker
{
  /// @brief Base abstract class for object handler.
  ///
  /// The class provides interface for handling objects of the same type, obtained under different versions of the code
  /// base.
  ///
  class ObjectHandler {
  public:
    /// @brief Default constructor
    /// @param iObject  Index of object
    /// @param iFile    Index of file
    /// @param iDataset Index of dataset
    /// @param objType  Type of the handled objects
    ObjectHandler(int iObject, int iFile, int iDataset, const char* objType = "");

    /// @brief Destructor
    virtual ~ObjectHandler();

    /// @brief Adds vector of pointer to objects
    /// @param vpObj  Vector of pointers to TNamed objects
    void AddObjects(const std::vector<TNamed*>& vpObj);

    /// @brief Creates object comparison canvas
    virtual void CreateCanvases() {};

    /// @brief Compares two objects with different methods
    /// @param pNum    Pointer to "numerator" object
    /// @param pDenom  Pointer to "denominator" object
    /// @return  Comparison result represented as a bitset in unsigned integer
    /// TODO: .....
    virtual CmpResult_t Compare(const TNamed* pNum, const TNamed* pDenom) const = 0;

    /// @brief Compares versions with default and writes result into DB
    void CompareWithDefault();

    /// @brief Sets folder to store output
    /// @param pDir  Pointer to folder instance
    void SetOutputDirectory(TDirectory* pDir);

    /// @brief Sets objects database
    /// @param  pObjDB  Shared pointer to object database
    void SetObjectDB(std::shared_ptr<ObjectDB>& pObjDB) { fpObjDB = pObjDB; }

    /// @brief Sets verbose level
    /// @param verbose  Verbose level:
    ///                 - 0: Silent mode
    ///                 - 1:
    void SetVerbose(int verbose) { fVerbose = verbose; }

    /// @brief Sets bit flag to control handler behaviour
    /// @param bit  Bit index
    ///
    /// The bit flags should be defined in an enumeration of the default class
    void SetBitFlag(uint8_t bit) { fOptionBits.set(bit); }

    /// @brief Writes objects to file
    void Write();

  protected:
    int fObjectID                         = -1;        ///< Index of object
    int fFileID                           = -1;        ///< Index of file
    int fDatasetID                        = -1;        ///< Index of dataset
    int fVerbose                          = 0;         ///< Verbosity level
    TDirectory* fpOutDir                  = nullptr;   ///< Pointer to directory
    std::shared_ptr<ObjectDB> fpObjDB     = nullptr;   ///< Pointer to object database
    std::shared_ptr<CbmQaCanvas> fpCanvas = nullptr;   ///< Comparison canvas
    std::bitset<sizeof(CmpResult_t) * 8> fOptionBits;  ///< Bitset for option

    std::string fsObjType  = "";      ///< Base type of the object to be handled
    std::string fsBaseName = "";      ///< Base names of the object
    std::vector<TNamed*> fvpObjects;  ///< Vector of objects
  };
}  // namespace cbm::qa::checker

#endif  // CbmQaCheckerObjectHandler_h

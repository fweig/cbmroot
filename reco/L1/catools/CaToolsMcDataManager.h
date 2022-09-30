/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CaToolsMcDataManager.h
/// \brief  Manager class for handling McData structure (header)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef CaToolsMcDataManager_h
#define CaToolsMcDataManager_h 1

#include "CaToolsMcData.h"

namespace ca
{
  namespace tools
  {
    class Performance;
    class McDataManager {
    public:
      // *********************************
      // ** Constructors and destructor **
      // *********************************

      /// Default constructor
      McDataManager() {}

      /// Destructor
      ~McDataManager() = default;

      /// Copy constructor
      McDataManager(const McDataManager&) = delete;

      /// Move constructor
      McDataManager(McDataManager&&) = delete;

      /// Copy assignment operator
      McDataManager& operator=(const McDataManager&) = delete;

      /// Move assignment operator
      McDataManager& operator=(McDataManager&&) = delete;

      /// Sends (moves) MC data object to the destination Performance instance
      /// \param  pPerformance Pointer to the destination Performance instance
      /// \return Success flag
      bool SendMcData(Performance* pPerformance);

    private:
      // **********************
      // ** Member variables **
      // **********************
      McData fMcData {};  ///< Object with internal MC data
    };
  }  // namespace tools
}  // namespace ca

#endif  // CaToolsMcDataManager_h

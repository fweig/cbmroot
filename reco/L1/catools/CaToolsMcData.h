/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CaToolsMcData.h
/// \brief  Data structure for internal tracking MC-information (header)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef CaToolsMcData_h
#define CaToolsMcData_h 1

namespace ca
{
  namespace tools
  {
    class McData {
    public:
      // *********************************
      // ** Constructors and destructor **
      // *********************************

      /// Default constructor
      McData();

      /// Destructor
      ~McData() = default;

      /// Copy constructor
      McData(const McData& other);

      /// Move constructor
      McData(McData&& other) noexcept;

      /// Copy assignment operator
      McData& operator=(const McData& other);

      /// Move assignment operator
      McData& operator=(McData&& other) noexcept;

      /// Swap method
      void Swap(McData& other) noexcept;


      // **********************
      // ** Access functions **
      // **********************

    private:
      // **********************
      // ** Member variables **
      // **********************
    };
  }  // namespace tools
}  // namespace ca

#endif  // CaToolsMcData_h

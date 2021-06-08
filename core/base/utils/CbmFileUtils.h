/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef CBMFILEUTILS_H_
#define CBMFILEUTILS_H_

#include <string>  // for string

namespace Cbm
{
  namespace File
  {
    /**
     * \brief Check if a the file exist and is a root file
     * \param[in] filename The filename of the file to be checked
     * \value     true if file is an existing root file, 0 in any other case
    **/
    bool IsRootFile(std::string filename);
  }  //namespace File
}  // namespace Cbm

#endif /* CBMFILEUTILS_H_ */

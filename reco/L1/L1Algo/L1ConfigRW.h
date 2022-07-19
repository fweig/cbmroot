/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   L1ConfigRW.h
/// @brief  Configuration parameter file reader/writer for L1 tracking algorithm (declaration)
/// @author S.Zharko <s.zharko@gsi.de>
/// @since  18.07.2022

#ifndef L1ConfigRW_h
#define L1ConfigRW_h 1

#include <string>
#include <vector>

namespace YAML
{
  class Node;
}
class L1InitManager;

/// Class L1ConfigRW provides internal read and write methods for L1 tracking algorithm parameters
class L1ConfigRW {
public:
  /// Constructor
  /// \param  pInitManager Pointer to the L1InitManager instance
  L1ConfigRW(L1InitManager* pInitManager, int verbose = 0);

  /// Destructor
  ~L1ConfigRW() {}

  /// Reads parameters from the file
  /// \param  filename  Name of the configuration file
  void ReadYaml(const std::string& filename);

  /// Writes parameters to the file
  /// \param  filename  Name of the configuration file
  void WriteYaml(const std::string& filename);

private:
  /// Reads CA track finder iterations from YAML node
  /// \param  node  YAML node
  void ReadCAIterations(const YAML::Node& node);

  /// Gets parameters content of the node
  /// FIXME: We assume, that all of the keys of node can be represented with strings. This assumption is not always true, so
  ///        one should be sure about the assumption when using this function.
  /// \param   node  YAML node
  /// \return  Vector of key names
  std::vector<std::string> GetNodeKeys(const YAML::Node& node) const;

  L1InitManager* fpInitManager = nullptr;  ///< Pointer to the L1InitManager instance
  int fVerbose                 = 0;        ///< Verbosity level
};

#endif  // L1ConfigRW_h

/* Copyright (C) 2022 Johann Wolfgang Goethe-Universitaet Frankfurt, Frankfurt am Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jan de Cuveland [committer] */

#include "Application.h"
#include "ProgramOptions.h"
#include "log.hpp"

int main(int argc, char* argv[])
{
  try {
    ProgramOptions opt(argc, argv);
    Application app(opt);
    app.Run();
  }
  catch (std::exception const& e) {
    L_(fatal) << e.what();
    return EXIT_FAILURE;
  }

  L_(info) << "exiting";
  return EXIT_SUCCESS;
}

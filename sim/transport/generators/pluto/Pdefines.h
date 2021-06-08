/* Copyright (C) 2018-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#define PRINT_CTOR "CONSTRUCTOR"
#define PRINT_AUTO_ALLOC "ALLOCATION"
#define PRINT_WARNING "### WARNING:"
#define PRINT_FATAL "FFF FATAL:"
#define PRINT_INFO "    Info:"
#define CRASH                                                                                                          \
  {                                                                                                                    \
    ;                                                                                                                  \
    Int_t* a = nullptr;                                                                                                \
    *a       = 0;                                                                                                      \
  }

#define PLUTO_COMPOSITE 1000

#define EMBEDDED_SOURCE_ID -2
#define INGO_DEBUG

#define NBATCH_NAME "nbatch"
#define LBATCH_NAME "lbatch"

#define NMODEL_NAME "nmodel"
#define LMODEL_NAME "lmodel"

/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_BASE_COMPAT_ONLINEDATALOG_H
#define CBM_ALGO_BASE_COMPAT_ONLINEDATALOG_H

#ifndef NO_ROOT

#include <fairlogger/Logger.h>

#else  // defined NO_ROOT

#include <log.hpp>

static constexpr severity_level warn = severity_level::warning;
#define LOG(level) L_(level)

#endif

#endif  // CBM_ALGO_BASE_COMPAT_ONLINEDATALOG_H

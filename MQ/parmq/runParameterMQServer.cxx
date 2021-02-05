/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
/**
 * runParameterMQServer.cxx
 *
 * @since 2013-04-23
 * @author D. Klein, A. Rybalchenko
 */

#include "ParameterMQServer.h"
#include "runFairMQDevice.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& options) {
  options.add_options()(
    "first-input-name",
    bpo::value<std::string>()->default_value("first_input.root"),
    "First input file name")("first-input-type",
                             bpo::value<std::string>()->default_value("ROOT"),
                             "First input file type (ROOT/ASCII)")(
    "second-input-name",
    bpo::value<std::string>()->default_value(""),
    "Second input file name")("second-input-type",
                              bpo::value<std::string>()->default_value("ROOT"),
                              "Second input file type (ROOT/ASCII)")(
    "libs-to-load",
    bpo::value<std::string>()->default_value(""),
    "List of libraries to load")("output-name",
                                 bpo::value<std::string>()->default_value(""),
                                 "Output file name")(
    "output-type",
    bpo::value<std::string>()->default_value("ROOT"),
    "Output file type")("channel-name",
                        bpo::value<std::string>()->default_value("data"),
                        "Output channel name");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) {
  return new ParameterMQServer();
}

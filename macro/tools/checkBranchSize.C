/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "TBranch.h"
#include "TFile.h"
#include "TList.h"
#include "TObjString.h"
#include "TString.h"
#include "TTree.h"
#include <tuple>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

// Comparison function to sort the vector elements
// by second element of tuples
bool sortbysec(const std::tuple<TString, float, float>& a, const std::tuple<TString, float, float>& b)
{
  return (std::get<1>(a) < std::get<1>(b));
}


void checkBranchSize(TString inFile)
{
  //Open the file and acces the Tree as well as the TList with branch names
  std::unique_ptr<TFile> _file {TFile::Open(inFile.Data(), "READ")};
  std::unique_ptr<TTree> inTree {dynamic_cast<TTree*>(_file->Get("cbmsim"))};
  std::unique_ptr<TList> _list {dynamic_cast<TList*>(_file->Get("BranchList"))};

  std::vector<std::tuple<TString, float, float>> branches {};

  // The TList contains a list of TObjString
  // access the content with GetString()
  //  std::unique_ptr<TBranch> _branch{};
  for (const auto&& obj : *_list) {
    TString branchName = static_cast<TObjString*>(obj)->GetString();
    TBranch* _branch   = inTree->FindBranch(branchName);
    //    _branch.reset(inTree->FindBranch(branchName));
    if (_branch) {
      float totBytes = _branch->GetTotBytes("*") / 1024. / 1024.;
      float zipBytes = _branch->GetZipBytes("*") / 1024. / 1024.;
      branches.push_back(std::make_tuple(branchName, totBytes, zipBytes));
    }
  }

  // Using sort() function to sort by 2nd element
  // of tuple
  sort(branches.begin(), branches.end(), sortbysec);

  int longestName {0};
  float largestTot {0.};
  float largestZip {0.};
  for (const auto& info : branches) {
    if (std::get<0>(info).Length() > longestName) longestName = std::get<0>(info).Length();
    if (std::get<1>(info) > largestTot) largestTot = std::get<1>(info);
    if (std::get<2>(info) > largestZip) largestZip = std::get<2>(info);
  }

  std::cout << std::left << std::setw(longestName + 2) << std::setfill(' ') << "Branch";
  std::cout << std::right << std::setw(15) << std::setfill(' ') << "TotalSize[MB]";
  std::cout << std::right << std::setw(15) << std::setfill(' ') << "ZipSize[MB]" << std::endl;
  std::cout << "-------------------------------------------" << std::endl;
  for (const auto& info : branches) {
    std::cout << std::left << std::setw(longestName + 2) << std::setfill(' ') << std::get<0>(info);
    std::cout << std::fixed << std::setprecision(2) << std::right << std::setw(15) << std::setfill(' ')
              << std::get<1>(info);
    std::cout << std::fixed << std::setprecision(2) << std::right << std::setw(15) << std::setfill(' ')
              << std::get<2>(info) << std::endl;
  }
}

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cout << "Wrong number of arguments!" << std::endl;
    std::cout << "Please pass the file name as only parameter" << std::endl;
    return -1;
  }

  const std::string filelist = argv[1];
  checkBranchSize(filelist);
  return 0;
}

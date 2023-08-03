/*
 * NicaGeneratorReadTree.h
 *
 *  Created on: 28 maj 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAGENERATORREADTREE_H_
#define NICAGENERATORREADTREE_H_

#include <TString.h>

#include "NicaGeneratorRead.h"

class TBranch;
class TFile;
class TTree;
/**
 * basisc class for read ROOT files
 */
class NicaGeneratorReadTree : public NicaGeneratorRead {
  TString fTreeName;
  TString fBranchName;

 protected:
  /**
   * pointer to input file with tree
   */
  TFile *fInFile;
  /**
   * pointer to tree with events
   */
  TTree *fInTree;
  /**
   * pointer to branch with event adress
   */
  TBranch *fBranch;
  virtual Int_t ReadEvent();
  virtual Int_t Reopen();

 public:
  /**
   * default constructor
   * @param name name of the file
   * @param branch_name name of branch in tree
   * @param tree_name name of
   * @param kMulti_mode
   */
  NicaGeneratorReadTree(TString name, TString branch_name, TString tree_name,
                        Bool_t kMulti_mode);
  virtual ~NicaGeneratorReadTree();
  ClassDef(NicaGeneratorReadTree, 1)
};

#endif /* NICAGENERATORREADTREE_H_ */

/*
 * NicaRunSim.h
 *
 *  Created on: 04-10-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICARUNSIM_H_
#define NICARUNSIM_H_

#include <TObject.h>
#include <TString.h>


class NicaGeneratorRead;
class NicaGeneratorWrite;
class TFile;
class TTree;

/**
 * basic class used for making simulation with unigen without any software for
 * simulation
 */
class NicaRunSim : public TObject {
  Int_t count;  //!
  Int_t fOversampling;
  TFile *fFile;
  TTree *fTree;
  TString fOutFile;
  NicaGeneratorRead *fGeneratorIn;  //!
  NicaGeneratorWrite *fGeneratorOut;
  static NicaRunSim *fginstance;  //!
 public:
  NicaRunSim();
  /**
   *
   * @return instance of this
   */
  static NicaRunSim *Instance();
  /**
   * specify name of outputfile
   * @param file
   */
  void SetOutputFile(TString file) { fOutFile = file; };
  /**
   * default init method
   */
  virtual void Init();
  /**
   * run method
   * @param NEvents metod for processs
   * @param NotUsed not used
   */
  virtual void Run(Int_t NEvents = 0, Int_t NotUsed = 0);
  /**
   *save results
   */
  virtual void FinishRun();
  /**
   * enable oversampling, events are merged into one superevent
   * @param val number of events to merge
   */
  void SetOversampling(Int_t val) { fOversampling = val; };
  /**
   *
   * @return pointer to tree
   */
  TTree *GetTree() { return fTree; }
  /**
   * set generator to read files
   * @param task task with generator
   */
  void SetGeneratorIn(NicaGeneratorRead *task);
  /**
   * set generator to write output format
   * @param task generator with output format
   */
  void SetGeneratorOut(NicaGeneratorWrite *task);
  virtual ~NicaRunSim();
  ClassDef(NicaRunSim, 1)
};

#endif /* NICARUNSIM_H_ */

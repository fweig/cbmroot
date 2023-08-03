/*
 lo * NicaFlowVariable.h
 *
 *  Created on: 19-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAFLOWVARIABLE_H_
#define NICAFLOWVARIABLE_H_

#include <TObject.h>
#include <TString.h>


class NicaTrack;

/**
 * abstract class for calculating any flow variable
 */
class NicaFlowVariable : public TObject {
  TString fName;
  TString fUnit;

 public:
  /**
   * default constructor
   * @param axisName axis name (e.g. p_{T}
   * @param unitName unit name
   */
  NicaFlowVariable(TString axisName = "dummy", TString unitName = "dummy");
  /**
   *
   * @return axis name
   */
  virtual TString GetAxisName();
  /**
   *
   * @return unit name
   */
  virtual TString GetAxisUnit();
  /**
   *
   * @param p track
   * @return variable with is plotted of flow histogram
   */
  virtual Double_t GetVariable(NicaTrack* p);
  virtual NicaFlowVariable* MakeCopy() const = 0;
  virtual ~NicaFlowVariable();
  ClassDef(NicaFlowVariable, 1)
};

/**
 * virtual flow variable, always return 0
 */
class NicaFlowVirtualVariable : public NicaFlowVariable {
 public:
  NicaFlowVirtualVariable();
  Double_t GetVariable(NicaTrack* /*p*/) { return 0; };
  NicaFlowVariable* MakeCopy() const { return new NicaFlowVirtualVariable(); };
  virtual ~NicaFlowVirtualVariable();
  ClassDef(NicaFlowVirtualVariable, 1)
};

#endif /* NICAFLOWVARIABLE_H_ */

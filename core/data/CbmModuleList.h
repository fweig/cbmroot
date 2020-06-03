/** @file CbmModuleList.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 13.06.2013
 **/


#ifndef CBMMODULELIST_H
#define CBMMODULELIST_H 1


#include <map>          // for map

#include <RtypesCore.h> // for ROOT data types
#include <TString.h>    // for TString

#include "CbmDefs.h"    // for ECbmModuleId

/** @class CbmModuleList
 ** @brief Manages module Ids and names
 ** @author V.Friese <v.friese@gsi.de>
 ** @version 1.0
 **
 ** CbmModuleList is a tool to assess module names from their identifier
 ** and vice versa through static methods.
 ** Modules can be (detector) systems or passive ones.
 **/
class CbmModuleList
{

  public:

    /**  Constructor   **/
    CbmModuleList() { };


    /** Destructor **/
    virtual ~CbmModuleList() { };


    /** Get module name from module Id
     ** @param moduleId  Unique module identifier (SystemId or kMagnet/kTarget/kPipe)
     ** @return Name of module
     **/
    static TString GetModuleName(ECbmModuleId moduleId);


    /** Get module name in capitals from module Id
     ** @param moduleId  Unique module identifier (SystemId or kMagnet/kTarget/kPipe)
     ** @return Name of module (in capitals)
     **/
    static TString GetModuleNameCaps(ECbmModuleId moduleId);


   /** Get module Id from module name
     ** @param moduleName Name of module (case insensitive)
     ** @return Unique module Id
     */
    static ECbmModuleId GetModuleId(const char* moduleName);



  private:

    /** Map of module identifier to module name **/
    static std::map<ECbmModuleId, TString> fModules;

    /** Initialisation of module map **/
    static std::map<ECbmModuleId, TString> DefineModules();

};

#endif /* CBMMODULELIST_H */

/** @file CbmStsModule.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 14.05.2013
 **/

#ifndef CBMSTSMODULE_H
#define CBMSTSMODULE_H 1

#include <Rtypes.h>           // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>       // for Double_t, UShort_t, Bool_t, Int_t, kTRUE
#include <TString.h>          // for TString

#include <string>             // for string

#include "CbmStsElement.h"    // for CbmStsElement
#include "CbmStsParModule.h"  // for CbmStsParModule

class CbmStsParAsic;
class TGeoPhysicalNode;


/** @class CbmStsModule
 ** @brief Class representing an instance of a readout unit in the CBM-STS.
 ** @author V.Friese <v.friese@gsi.de>
 ** @version 2.0
 **
 ** The StsModule is the read-out unit in the CBM STS. It consists of one
 ** sensor or two or more daisy-chained sensors (CbmStsSensor), the analogue
 ** cable and the read-out electronics.
 **/
class CbmStsModule : public CbmStsElement
{

  public:

    /** @brief Standard constructor
     ** @param address  Unique element address
     ** @param node     Geometry node
     ** @param mother   Mother element
     **/
    CbmStsModule(UInt_t address = 0, TGeoPhysicalNode* node = nullptr,
                 CbmStsElement* mother = nullptr);


    /** @brief Destructor **/
    virtual ~CbmStsModule();


    /** @brief Get the address from the module name (static)
     ** @param name Name of module
     ** @value Unique element address
     **/
    static Int_t GetAddressFromName(TString name);


    /** @brief Module parameters
     ** @return Module parameter object
     **/
    const CbmStsParModule* GetParameters() const {
      return fParams;
    }


    /** @brief Set module parameters
     ** @param par  Module parameter object
     **/
    void SetParameters(const CbmStsParModule& par) {
      fParams = &par;
    }


    /** String output **/
    std::string ToString() const;


  private:

    /** @brief Initialise daughters from geometry **/
    virtual void InitDaughters();


  private:

    const CbmStsParModule* fParams = nullptr;   //! Module parameters


    ClassDef(CbmStsModule, 3);

};

#endif /* CBMSTSMODULE_H */

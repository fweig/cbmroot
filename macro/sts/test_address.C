/** @file test_address.C
 ** @brief Macro for testing the STS addressing scheme
 ** @author V. Friese <v.friese@gsi.de
 ** @date 23 August 2017
 **
 ** The internal consistency of constructing an address from the element Ids
 ** and (reverse) extracting the element Ids from the address is tested.
 **/


void test_address(Int_t addressVersion = -1)
{

  // Dummy just to get the libraries loaded
  new CbmStsDigi();

  // Default is current address version
  UInt_t version = addressVersion;
  if ( addressVersion < 0 || addressVersion >= CbmStsAddress::kCurrentVersion)
    version = CbmStsAddress::kCurrentVersion;
  std::cout << "Testing address version " << version << std::endl;


  // Maximal numbers of allowed elements (version 1)
  Int_t nUnits       = 64;
  Int_t nLadders     = 32;
  Int_t nHalfLadders = 2;
  Int_t nModules     = 32;
  Int_t nSensors     = 16;
  Int_t nSides       = 2;

  // Maximal numbers of allowed elements (version 0)
  if ( version == 0 ) {
    nUnits       = 16;
    nLadders     = 16;
    nHalfLadders = 2;
    nModules     = 8;
    nSensors     = 4;
    nSides       = 2;
  }

  Int_t nTests = 0;
  Bool_t success = kTRUE;
  TStopwatch timer;
  timer.Start();


  // Loop over allowed elements
  for (UInt_t unit = 0; unit < nUnits; unit++) {
    for (UInt_t ladd = 0; ladd < nLadders; ladd++) {
      for (UInt_t hlad = 0; hlad < nHalfLadders; hlad++) {
        for (UInt_t modu = 0; modu < nModules; modu++) {
          for (UInt_t sens = 0; sens < nSensors; sens++) {
            for (UInt_t side = 0; side < nSides; side++) {

              Int_t address = CbmStsAddress::GetAddress(unit, ladd, hlad, modu, sens, side);

              // Test function GetElementId
              UInt_t rSyst = CbmStsAddress::GetElementId(address, kStsSystem);
              UInt_t rUnit = CbmStsAddress::GetElementId(address, kStsUnit);
              UInt_t rLadd = CbmStsAddress::GetElementId(address, kStsLadder);
              UInt_t rHlad = CbmStsAddress::GetElementId(address, kStsHalfLadder);
              UInt_t rModu = CbmStsAddress::GetElementId(address, kStsModule);
              UInt_t rSens = CbmStsAddress::GetElementId(address, kStsSensor);
              UInt_t rSide = CbmStsAddress::GetElementId(address, kStsSide);

              if ( rSyst != kSts ) {
                std::cout << "System " << rSyst << " is not STS" << std::endl;
                success = kFALSE;
              }
              if ( CbmStsAddress::GetSystemId(address) != kSts ) {
                std::cout << "GetSystemId " << CbmStsAddress::GetSystemId(address)
                << " does not return STS" << std::endl;
                success = kFALSE;
              }
              if (rUnit != unit) {
                std::cout << "Different unit " << unit << " " << rUnit << std::endl;
                success = kFALSE;
              }
              if (rLadd != ladd) {
                std::cout << "Different ladder " << ladd << " " << rLadd << std::endl;
                success = kFALSE;
              }
              if (rHlad != hlad) {
                std::cout << "Different half-ladder " << hlad << " " << rHlad << std::endl;
                success = kFALSE;
              }
              if (rModu != modu) {
                std::cout << "Different module " << modu << " " << rModu << std::endl;
                success = kFALSE;
              }
              if (rSens != sens) {
                std::cout << "Different sensor " << sens << " " << rSens << std::endl;
                success = kFALSE;
              }
              if (rSide != side) {
                std::cout << "Different side " << side << " " << rSide << std::endl;
                success = kFALSE;
              }

              // Test function GetMotherAddress
              Int_t sensAdd1 = CbmStsAddress::GetAddress(unit, ladd, hlad, modu, sens);
              Int_t sensAdd2 = CbmStsAddress::GetMotherAddress(address, kStsSensor);
              if (sensAdd1 != sensAdd2) {
                std::cout << "Sensor address " << sensAdd1 << " " << sensAdd2 << std::endl;
                success = kFALSE;
              }
              Int_t moduAdd1 = CbmStsAddress::GetAddress(unit, ladd, hlad, modu);
              Int_t moduAdd2 = CbmStsAddress::GetMotherAddress(address, kStsModule);
              if (sensAdd1 != sensAdd2) {
                std::cout << "Module address " << moduAdd1 << " " << moduAdd2 << std::endl;
                success = kFALSE;
              }
              Int_t hladAdd1 = CbmStsAddress::GetAddress(unit, ladd, hlad);
              Int_t hladAdd2 = CbmStsAddress::GetMotherAddress(address, kStsHalfLadder);
              if (hladAdd1 != hladAdd2) {
                std::cout << "Half-ladder address " << hladAdd1 << " " << hladAdd2 << std::endl;
                success = kFALSE;
              }
              Int_t laddAdd1 = CbmStsAddress::GetAddress(unit, ladd);
              Int_t laddAdd2 = CbmStsAddress::GetMotherAddress(address, kStsLadder);
              if (laddAdd1 != laddAdd2) {
                std::cout << "Ladder address " << laddAdd1 << " " << laddAdd2 << std::endl;
                success = kFALSE;
              }
              Int_t unitAdd1 = CbmStsAddress::GetAddress(unit);
              Int_t unitAdd2 = CbmStsAddress::GetMotherAddress(address, kStsUnit);
              if (unitAdd1 != unitAdd2) {
                std::cout << "Unit address " << unitAdd1 << " " << unitAdd2 << std::endl;
                success = kFALSE;
              }

              if ( ! success ) {
                std::cout << "Test " << nTests << " "
                          << CbmStsAddress::ToString(address) << std::endl;
                std::cout << "Test ended with errors." << std::endl;
                return;
              }

              nTests++;

            } // #side
          } //# sensor
        } //# module
      } //# half-ladder
    } //# ladder
  } //# unit

  timer.Stop();
  std::cout << nTests << " addresses tested successfully." << std::endl;
  std::cout << "Consumption: CPU " << timer.CpuTime() << " s, Real "
            << timer.RealTime() << std::endl;

} // end of script


/** @file MCBM DATA unpacking
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 ** Modified by P.-A. Loizeau
 ** @date 30.01.2019
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmMcbm2018Source as source task.
 */
#include "unpack_tsa_mcbm.C"

/// FIXME: Disable clang formatting to keep easy parameters overview
/* clang-format off */
Bool_t unpack_tsa_mcbm_kronos(UInt_t uRunIdx       = 28,
                              UInt_t uNbTimeslices = 0,
                              TString sOutDir      = "data",
                              Int_t iSpillIndex    = -1,
                              Int_t iSpillnumber   = 3,
                              UInt_t uSpillLimType = 1)
{
  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */

  UInt_t uRunId = 0;
  if (99999 != uRunIdx) {
    std::vector<UInt_t> vuListRunId = {
      692, 698, 702, 704, 705, 706, 707,            //  7 =>  0 -  6
      744, 750, 759, 760, 761, 762, 799,            //  7 =>  7 - 13
      811, 812, 816, 817, 819,                      //  5 => 14 - 18
      820, 821, 822, 824, 826, 827, 828, 829,       //  8 => 19 - 26
      830, 831, 836,                                //  3 => 27 - 29
      841, 846, 849,                                //  3 => 30 - 32
      850, 851, 852, 854, 855, 856, 857, 858, 859,  //  9 => 33 - 41
      860, 861, 862, 863, 864, 865, 866             //  7 => 42 - 48
                                                    /*
 /// With runs < 1 min due to missmatch!
             811, 812, 816, 817, 818, 819,                     //  6 => 14 - 19
             820, 821, 822, 824, 826, 827, 828, 829,           //  8 => 20 - 27
             830, 831, 836, 839,                               //  4 => 28 - 31
             840, 841, 842, 844, 845, 846, 848, 849,           //  8 => 32 - 39
             850, 851, 852, 854, 855, 856, 857, 858, 859,      //  9 => 40 - 48
             860, 861, 862, 863, 864, 865, 866                 //  7 => 49 - 55
*/
    };
    if (vuListRunId.size() <= uRunIdx) {
      std::cout << "Run index is out of range, not doing anything" << std::endl;
      return kFALSE;
    }
    uRunId = vuListRunId[uRunIdx];
  }  // if( 99999 != uRunIdx )

  if (uRunId < 692) return kFALSE;

  TString inFile =
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn02_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn04_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn05_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn06_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn08_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn10_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn11_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn12_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn13_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn15_*.tsa", uRunId);

  return unpack_tsa_mcbm(inFile, uRunId, uNbTimeslices, sOutDir, iSpillIndex, iSpillnumber, uSpillLimType);
}

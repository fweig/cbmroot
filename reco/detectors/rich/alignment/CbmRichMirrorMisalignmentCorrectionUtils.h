#ifndef CBM_RICH_MIRROR_MISALIGNMENT_CORRECTION_UTILS
#define CBM_RICH_MIRROR_MISALIGNMENT_CORRECTION_UTILS

using namespace std;
using std::string;
#include <Logger.h>

#include <fstream>
#include <iostream>
#include <map>

/**
* \class CbmRichMirrorMisalignmentCorrectionUtils
*
* \brief class checks correction parameter file containing mirror misalignment information.
*
* \author Jordan Bendarouach
* \date 2017
**/

class CbmRichMirrorMisalignmentCorrectionUtils {
public:
  /**
	    * brief Default constructor.
	    */
  CbmRichMirrorMisalignmentCorrectionUtils() : tBoolCorrection(false) {}

  /**
	    * \brief Destructor.
	    */
  virtual ~CbmRichMirrorMisalignmentCorrectionUtils() {}

  /**
	    * \brief Initialization in case one needs to initialize some TCloneArrays.
	    */
  //	   virtual void Init() { }

  void Init(const string& s) {
    SetMirrorCorrectionParameterFile(s);
    tBoolCorrection = CheckMirrorCorrectionParameterFile();
    if (tBoolCorrection) {
      ReadAndStoreMirrorCorrectionParamMap(
        fMirrorMisalignmentCorrectionParameterMap);
    } else {
      LOG(info)
        << "CbmRichProjectionProducerAnalytical::Init() No correction table.";
    }
  }

  void SetMirrorCorrectionParameterFile(const string& s) {
    fPathToMirrorMisalignmentCorrectionParameterFile = s;
  }

  bool CheckMirrorCorrectionParameterFile() {
    if (fPathToMirrorMisalignmentCorrectionParameterFile
        == "/correction_table/correction_param_array.txt") {
      return false;
    }
    ifstream corrFile;
    corrFile.open(fPathToMirrorMisalignmentCorrectionParameterFile);
    if (!corrFile.is_open()) {
      Fatal("CbmRichMirrorMisalignmentCorrectionUtils::"
            "CheckMirrorCorrectionParameterFile:",
            "Wrong correction parameter path given!");
    }
    return true;
  }

  void ReadAndStoreMirrorCorrectionParamMap(
    std::map<string, std::pair<Double_t, Double_t>>& mirrorCorrectionParamMap) {
    string mirrorID = "", fileLine = "";
    Double_t misX = 0., misY = 0.;
    ifstream corrFile;
    corrFile.open(fPathToMirrorMisalignmentCorrectionParameterFile, ios::in);
    if (corrFile.is_open()) {
      LOG(info)
        << "CbmRichProjectionProducerUtils::CheckMirrorCorrectionParameterFile("
           ") Correction table used at: "
        << fPathToMirrorMisalignmentCorrectionParameterFile;
      cout << "mirrorID: \t\t\t (misY, misX): " << endl;
      while (corrFile >> mirrorID >> misY >> misX) {
        cout << mirrorID << " \t ; \t(" << misY << ", " << misX << ")" << endl;
        mirrorCorrectionParamMap[mirrorID] = std::make_pair(misX, misY);
      }
      corrFile.close();
    } else {
      Fatal("CbmRichMirrorMisalignmentCorrectionUtils::"
            "ReadAndStoreMirrorCorrectionParam:",
            "Wrong correction parameter path given!");
    }

    cout << endl << "Mapping:" << endl;
    cout << "mirrorID: \t\t\t (misY, misX): " << endl;
    for (std::map<string, std::pair<Double_t, Double_t>>::iterator it =
           fMirrorMisalignmentCorrectionParameterMap.begin();
         it != fMirrorMisalignmentCorrectionParameterMap.end();
         ++it) {
      cout << it->first << " \t ; \t(" << it->second.first << ", "
           << it->second.second << ")" << endl;
    }
    cout << endl;
  }

  std::map<string, std::pair<Double_t, Double_t>>
  GetMirrorCorrectionParamMap() {
    return fMirrorMisalignmentCorrectionParameterMap;
  }

  bool GetMirrorCorrectionParamBool() { return tBoolCorrection; }

private:
  bool tBoolCorrection;
  string fPathToMirrorMisalignmentCorrectionParameterFile;
  std::map<string, pair<Double_t, Double_t>>
    fMirrorMisalignmentCorrectionParameterMap;
};

#endif

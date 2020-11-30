#include "L1AlgoInputData.h"

#include <cstring>
#include <iostream>
using namespace std;
using std::ios;
/*
L1AlgoInputData::L1AlgoInputData( const L1AlgoInputData& a)
{
  SetData( a.GetStsHits(), a.GetStsStrips(), a.GetStsStripsB(), a.GetStsZPos(),
           a.GetSFlag(), a.GetSFlagB(), a.GetStsHitsStartIndex(), a.GetStsHitsStopIndex());
}


const L1AlgoInputData& L1AlgoInputData::operator=( const L1AlgoInputData& a)
{
  SetData( a.GetStsHits(), a.GetStsStrips(), a.GetStsStripsB(), a.GetStsZPos(),
           a.GetSFlag(), a.GetSFlagB(), a.GetStsHitsStartIndex(), a.GetStsHitsStopIndex());
  return a;
}


void L1AlgoInputData::SetData( const vector< L1StsHit >      & StsHits_,
                                   const vector< L1Strip >       & StsStrips_,
                                   const vector< L1Strip >       & StsStripsB_,
                                   const vector< fscal >         & StsZPos_,
                                   const vector< unsigned char > & SFlag_,
                                   const vector< unsigned char > & SFlagB_,
                                   const THitI* StsHitsStartIndex_,
                                   const THitI* StsHitsStopIndex_ )
{
  vStsHits.resize(StsHits_.size());
  vStsStrips.resize(StsStrips_.size());
  vStsStripsB.resize(StsStripsB_.size());
  vStsZPos.resize(StsZPos_.size());
  vSFlag.resize(SFlag_.size());
  vSFlagB.resize(SFlagB_.size());
  
  for(unsigned int i=0; i<StsHits_.size(); ++i ) {vStsHits[i] = StsHits_[i];

}
  for(unsigned int i=0; i<StsStrips_.size(); ++i ) vStsStrips[i] = StsStrips_[i];
  for(unsigned int i=0; i<StsStripsB_.size(); ++i ) vStsStripsB[i] = StsStripsB_[i];
  for(unsigned int i=0; i<StsZPos_.size(); ++i ) vStsZPos[i] = StsZPos_[i];
  for(unsigned int i=0; i<SFlag_.size(); ++i ) vSFlag[i] = SFlag_[i];
  for(unsigned int i=0; i<SFlagB_.size(); ++i ) vSFlagB[i] = SFlagB_[i];

  for(unsigned int i=0; i<MaxNStations+1; ++i) StsHitsStartIndex[i] = StsHitsStartIndex_[i];
 
  for(unsigned int i=0; i<MaxNStations+1; ++i) StsHitsStopIndex[i]  = StsHitsStopIndex_[i];
}

*/

istream& L1AlgoInputData::eatwhite(istream& is)  // skip spaces
{
  char c;
  while (is.get(c)) {
    if (isspace(c) == 0) {
      is.putback(c);
      break;
    }
  }
  return is;
}

bool L1AlgoInputData::ReadHitsFromFile(const char work_dir[100],
                                       const int maxNEvent,
                                       const int iVerbose) {
  static int nEvent = 1;
  static ifstream fadata;
  static char fname[100];

  if (nEvent == 1) {
    strcpy(fname, work_dir);
    strcat(fname, "data_algo.txt");
    fadata.open(fname);
  }
  if (!fadata.is_open()) return 0;

  if (nEvent <= maxNEvent) {

    vStsHits.clear();
    NStsStripsF = 0;
    NStsStripsB = 0;
    vStsZPos.clear();
    vSFlag.clear();
    vSFlagB.clear();

    // check correct position in file
    char s[] = "Event:  ";
    int nEv;
    fadata >> s;
    //    cout << s<<  " s"<<endl;
    fadata >> nEv;
    //  cout << nEv<<  " nEv"<<endl;

    if (nEv != nEvent)
      cout << "-E- CbmL1: Can't read event number " << nEvent << " from file "
           << fname << endl;

    int n;  // number of elements
      // read algo->vStsStrips
    fadata >> n;
    //     cout << n<<  " vStsStrips"<<endl;
    NStsStripsF = n;
    if (iVerbose >= 4)
      cout << "vStsStrips[" << n << "]"
           << " have been read." << endl;
    // read algo->vStsStripsB
    fadata >> n;
    //  cout << n<<  " vStsStripsB"<<endl;
    NStsStripsB = n;
    if (iVerbose >= 4)
      cout << "vStsStripsB[" << n << "]"
           << " have been read." << endl;
    // read algo->vStsZPos
    fadata >> n;
    //   cout << n<<  " vStsZPos"<<endl;
    for (int i = 0; i < n; i++) {
      fscal element;
      fadata >> element;
      vStsZPos.push_back(element);
    }
    if (iVerbose >= 4)
      cout << "vStsZPos[" << n << "]"
           << " have been read." << endl;
    // read algo->vSFlag
    fadata >> n;
    //  cout << n<<  " vSFlagB"<<endl;
    for (int i = 0; i < n; i++) {
      int element;
      fadata >> element;
      vSFlag.push_back(static_cast<unsigned char>(element));
    }
    if (iVerbose >= 4)
      cout << "vSFlag[" << n << "]"
           << " have been read." << endl;
    // read algo->vSFlagB
    fadata >> n;
    // cout << n<<  " vSFlagB"<<endl;
    for (int i = 0; i < n; i++) {
      int element;
      fadata >> element;
      vSFlagB.push_back(static_cast<unsigned char>(element));
    }
    if (iVerbose >= 4)
      cout << "vSFlagB[" << n << "]"
           << " have been read." << endl;
    // read algo->vStsHits
    fadata >> n;
    //   cout << n<<  " vStsHits"<<endl;
    int element_f;  // for convert
    int element_b;
    int element_n;
    int element_iz;
    for (int i = 0; i < n; i++) {
      L1StsHit element;
      fadata >> element_f >> element_b >> element_n >> element_iz >> element.u
        >> element.v >> element.t_reco;
      element.f = static_cast<THitI>(element_f);
      element.b = static_cast<THitI>(element_b);
#ifdef USE_EVENT_NUMBER
      element.n = static_cast<unsigned short int>(element_n);
#endif
      element.iz     = static_cast<TZPosI>(element_iz);
      vStsHits.push_back(element);
    }
    if (iVerbose >= 4)
      cout << "vStsHits[" << n << "]"
           << " have been read." << endl;
    // read StsHitsStartIndex and StsHitsStopIndex
    n = 20;
    for (int i = 0; i < n; i++) {
      int tmp;
      fadata >> tmp;
      if (MaxNStations + 1 > i) StsHitsStartIndex[i] = tmp;
      //   cout << " StsHitsStartIndex[i]"<< StsHitsStartIndex[i] << endl;

      //    cout << tmp<<  " tmp"<<endl;
    }
    for (int i = 0; i < n; i++) {
      int tmp;
      fadata >> tmp;
      if (MaxNStations + 1 > i) StsHitsStopIndex[i] = tmp;
      //   cout << " StsHitsStopIndex[i]"<< StsHitsStopIndex[i] << endl;
      //   cout << tmp<<  " tmp"<<endl;
    }

    if (iVerbose >= 2)
      cout << "-I- CbmL1: CATrackFinder data for event " << nEvent
           << " has been read from file " << fname << " successfully." << endl;
    //    if (nEvent == maxNEvent) fadata.close();
  }
  nEvent++;
  return 1;
}  // void L1AlgoInputData::ReadStAPAlgoData()

/*
void L1AlgoInputData::PrintHits()
{
  std::cout << "Event:  " << std::endl;

  int n = vStsStrips.size();  // number of elements
  std::cout << n << std::endl;
  for (int i = 0; i < n; i++){
    std::cout << vStsStrips[i] << std::endl;
  }

  n = vStsStripsB.size();
  std::cout << n << std::endl;
  for (int i = 0; i < n; i++){
    std::cout << vStsStripsB[i] << std::endl;
  }

  n = vStsZPos.size();
  std::cout << n << std::endl;
  for (int i = 0; i < n; i++){
    std::cout << vStsZPos[i] << std::endl;
  }

  n = vSFlag.size();
  std::cout << n << std::endl;
  for (int i = 0; i < n; i++){
    std::cout << static_cast<int>(vSFlag[i]) << std::endl;
  }

  n = vSFlagB.size();
  std::cout << n << std::endl;
  for (int i = 0; i < n; i++){
    std::cout << static_cast<int>(vSFlagB[i]) << std::endl;
  }

  n = vStsHits.size();
  std::cout << n << std::endl;
  for (int i = 0; i < n; i++){
    std::cout << static_cast<int>(vStsHits[i].f) << " ";
    std::cout << static_cast<int>(vStsHits[i].b) << " ";
    std::cout << static_cast<int>(vStsHits[i].iz) << std::endl;
  }

  n = 20;
  for (int i = 0; i < n; i++){
    if (MaxNStations+1 > i) std::cout << StsHitsStartIndex[i] <<std::endl;
    else std::cout << "0" <<std::endl;
  }
  for (int i = 0; i < n; i++){
    if (MaxNStations+1 > i) std::cout << StsHitsStopIndex[i] <<std::endl;
    else std::cout << "0" <<std::endl;
  }

} // void L1AlgoInputData::ReadStAPAlgoData()

*/

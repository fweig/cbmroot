#ifndef L1EventMatch_H
#define L1EventMatch_H

#include <vector>
#include "L1Event.h"
#include "L1MCEvent.h"
#include <map>

using std::vector;
using std::map;

class L1EventMatch
{
  public:
    L1EventMatch():fMCEvent(), fNTracks(0), fTracks() {};
    bool IsGhost() { return fMCEvent.size() == 0; }
    void Clear();
    void AddTrack(int mcEventId){fMCEvent[mcEventId]++;}
    void SetNEventTracks(int ntracks){fNTracks=ntracks;}
    void SetTracks(vector<int> tracks){fTracks=tracks;}
    vector<int> & GetTracksId(){return fTracks;}

    int NRecoTracks() const { return fNTracks; }
    int NMCEvents() const { return fMCEvent.size(); }
    
    map<int,int>& GetMCEvents() { return fMCEvent; }
  private:
    map<int,int> fMCEvent;
    int fNTracks;
    vector<int> fTracks;
};

#endif

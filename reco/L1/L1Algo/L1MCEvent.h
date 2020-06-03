#ifndef L1MCEvent_H
#define L1MCEvent_H

#include <vector>

using std::vector;

class L1MCEvent
{
 public:
  L1MCEvent ():fMCEventId(-1),fRecoEvents(),fMCTrackId(0),fIsReconstructable(0) {};
    
  int IsReconstructed();
  int NClones();
  void SetReconstructable(bool IsReconstructable_) {fIsReconstructable = IsReconstructable_;}
  void SetId(int mcEvId){fMCEventId=mcEvId;}

  vector<int> & GetMCTrackIds() { return fMCTrackId; }
  int NMCTracks() const { return fMCTrackId.size(); }
  int & GetId(){return fMCEventId;}
  bool  IsReconstructable() const {return fIsReconstructable;}
    
  void AddRecoEvent(int iEvent) { fRecoEvents.push_back(iEvent); }
    
  const vector<int>& GetRecoEvents() const { return fRecoEvents; }
    
 private:
  int fMCEventId;
  vector<int> fRecoEvents;
  vector<int> fMCTrackId;
  bool fIsReconstructable;
};

#endif
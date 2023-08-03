/*
 * NicaTrackClonesTemplate.h
 *
 *  Created on: 12 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKCLONES_H_
#define NICATRACKCLONES_H_

#include <TClonesArray.h>
#include <TObjArray.h>
#include <TString.h>

class NicaTrackClones : public TObject {
  TString fBranchName;
  TString fDirName;
  TClonesArray *fClones;

 public:
  NicaTrackClones(TString ClassName = "", TString branchname = "",
                  TString dirname = "");
  void Register(Bool_t write);
  void GetFromTree();
  void Compress(Int_t *map, Int_t map_size);
  void DeleteClones();
  template <class T>
  void CopyFrom(const TClonesArray *from) {
    fClones->Clear();
    Int_t size = from->GetEntriesFast();
    fClones->ExpandCreateFast(size);
    for (int i = 0; i < size; i++) {
      const T *obj_from = (T *)from->UncheckedAt(i);
      T *obj_to = (T *)fClones->UncheckedAt(i);
      *obj_to = *obj_from;
    }
  }
  template <class T>
  void CopyTo(TClonesArray *to) {
    fClones->Clear();
    Int_t size = to->GetEntriesFast();
    to->ExpandCreateFast(size);
    for (int i = 0; i < size; i++) {
      T *obj_from = (T *)fClones->UncheckedAt(i);
      T *obj_to = (T *)to->UncheckedAt(i);
      *obj_to = *obj_from;
    }
  }
  template <class T>
  void CopyCompress(TClonesArray *from, Int_t *map, Int_t map_size) {
    fClones->Clear();
    fClones->ExpandCreateFast(map_size);
    for (int i = 0; i < map_size; i++) {
      T *obj_from = (T *)from->UncheckedAt(map[i]);
      T *obj_to = (T *)fClones->UncheckedAt(i);
      *obj_to = *obj_from;
    }
  }
  Int_t GetEntriesFast() const { return fClones->GetEntriesFast(); };
  Bool_t ExistInTree() const;
  TString GetBranchName() const { return fBranchName; }
  TObject *UncheckedAt(Int_t i) const { return fClones->UncheckedAt(i); }
  TClonesArray *GetArray() const { return fClones; };
  virtual ~NicaTrackClones();
  ClassDef(NicaTrackClones, 1)
};

#endif /* NICATRACKCLONES_H_ */

/*
 * NicaCutContainer.h
 *
 *  Created on: 06-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICACUTCONTAINER_H_
#define NICACUTCONTAINER_H_

#include <TObjArray.h>
#include <TString.h>

#include "NicaStd.h"
#include "NicaCutMonitor.h"
#include "NicaCut.h"
#include "NicaCutCollection.h"

//#define SHOW_CUT_INIT
/**
 * class for storing all cuts and cut monitors used in analysis
 */
class NicaCutContainer : public TObject {
  Bool_t fInit;
  Int_t fSize;
  // TObjArray *fTempCutContainers;
  TObjArray **fTempCutMonitors;  //[fSize]
  TObjArray **fCutContainers;    //[fSize]
  void VerifyOrder(TObjArray *obj);
  void ExtractComplexMonitor(NicaCutMonitor *mon, TString &opt);
  void MakeComplexAxis(NicaCutMonitor *mon, Int_t axis, Int_t opt);
  Bool_t ExtractRegExp(const NicaCut &cut, Option_t *opt);
  Bool_t ExtrackRegExp2(const NicaCut &cut, Option_t *opt);
  Bool_t ExtractRegExp(const NicaCutMonitor &cut, Option_t *opt);
  Bool_t ExtractRegExp2(const NicaCutMonitor &cut, Option_t *opt);
  Bool_t CheckTwoTracksOptions(const NicaCutMonitor &cutmon, Option_t *opt);
  Bool_t CheckTwoTracksOptions(const NicaCut &cutmon, Option_t *opt);

 protected:
  inline TObjArray *GetCutContainer(ENicaCutUpdate upd) const {
    return (TObjArray *)fCutContainers[static_cast<Int_t>(upd)];
  };

 public:
  /**
   * default constructor that shouldn't be used
   */
  NicaCutContainer();
  /**
   * copy constructor
   * @param cont
   */
  NicaCutContainer(const NicaCutContainer &cont);
  /**
   * proper default constructor
   * @param tiers_no the highest update ratio of cuts used in analysis
   */
  NicaCutContainer(ENicaCutUpdate tiers_no);
  /**
   * initialize container
   * @param task_id id of used format
   */
  void Init(const Int_t task_id = 0);
  /**
   * add cut to this
   * @param cut cut to add
   * @param opt option of adding cut
   */
  void AddCut(const NicaCut &cut, Option_t *opt = " ");
  /**
   * Currently not used
   */
  void InitReport() const;
  /**
   * add cut monitor to this
   * @param monitor monitor to add
   * @param opt option of adding
   */
  void AddMonitor(const NicaCutMonitor &monitor, Option_t *opt = " ");
  /**
   * link collections
   * @param opt_low update ratio for linking from
   * @param in_low collection number for linking from
   * @param opt_high update ratio for linking to
   * @param in_high collection number for linking to
   */
  void LinkCollections(ENicaCutUpdate opt_low, Int_t in_low,
                       ENicaCutUpdate opt_high, Int_t in_high);
  /**
   * copy cut sub-container with given collection to new sub-container and add
   * to this cut cutnaier
   * @param type update ration (tier) of sub-container that should be copied
   * @param collection_no number collection number of subcontainer that should
   * be copied
   * @param new_collection_no number of collection of new sub-container
   * @param option currently not used
   */
  void ReplicateCollection(ENicaCutUpdate type, Int_t collection_no,
                           Int_t new_collection_no, Option_t *option = "");
  /**
   * remove given sub-container (collection) from this container
   * @param update update ration of removed sub-container
   * @param collection collection number of removed sub-container
   */
  void RemoveCollection(ENicaCutUpdate update, Int_t collection);
  /**
   * copy cut containers with given collection from other cut container to this
   * cut container, those copies are marked as "dummy" so they contains pointers
   * to cuts and cuts monitors from other instead of copies, method useful for
   * making "chain analysis"
   * @param update specify witch cuts and cut monitors will be copied
   * @param other cut monitor that is source of cuts for copying
   * @param copy_link - copy links between different layers of cuts
   */
  void MakeDummyCopies(ENicaCutUpdate update, NicaCutContainer *other,
                       Bool_t copy_link);
  /**
   *
   * @return number of event collections in this
   */
  Int_t GetEventCollectionsNo() const;
  /**
   *
   * @return number of track collections in this
   */
  Int_t GetTrackCollectionsNo() const;
  /**
   *
   * @return number of two-track collections in this
   */
  Int_t GetTwoTrackCollectionsNo() const;
  /**
   *
   * @return number of background two-track collections in this
   */
  Int_t GetTwoTrackCollectionsBackgroundNo() const;
  /**
   *
   * @param collection collection no
   * @return given event collection
   */
  inline NicaCutCollection *GetEventCollection(Int_t collection) const {
    return (NicaCutCollection *)
        fCutContainers[static_cast<Int_t>(ENicaCutUpdate::kEventUpdate)]
            ->UncheckedAt(collection);
    ;
  };
  /**
   *
   * @param collection collection no
   * @return given track collection
   */
  inline NicaCutCollection *GetTrackCollection(Int_t collection) const {
    return (NicaCutCollection *)
        fCutContainers[static_cast<Int_t>(ENicaCutUpdate::kTrackUpdate)]
            ->UncheckedAt(collection);
  };
  /**
   *
   * @param collection collection no
   * @return given two-track collection
   */
  inline NicaCutCollection *GetTwoTrackCollection(Int_t collection) const {
    return (NicaCutCollection *)
        fCutContainers[static_cast<Int_t>(ENicaCutUpdate::kTwoTrackUpdate)]
            ->UncheckedAt(collection);
  };
  /**
   *
   * @param collection collection no
   * @return given background two-track collection
   */
  inline NicaCutCollection *GetTwoTrackBackgroundCollection(
      Int_t collection) const {
    return (NicaCutCollection *)
        fCutContainers[static_cast<Int_t>(
                           ENicaCutUpdate::kTwoTrackBackgroundUpdate)]
            ->UncheckedAt(collection);
  };
  /**
   *
   * @return size of cut container (highest tier number/update ratio)
   */
  inline Int_t GetSize() const { return fSize; };
  /**
   * check event cuts
   * @param event event to check
   * @param collection event collection number used for checking
   * @return true if passed false otherwise
   */
  inline Bool_t PassEvent(NicaEvent *event, const Int_t collection) {
    return ((NicaCutCollection *)
                fCutContainers[static_cast<Int_t>(ENicaCutUpdate::kEventUpdate)]
                    ->UncheckedAt(collection))
        ->PassEvent(event);
  }
  /**
   * check track cuts
   * @param track track to check
   * @param collection track collection number used for checking
   * @return true if passed false otherwise
   */
  inline Bool_t PassTrack(NicaTrack *track, const Int_t collection) {
    return ((NicaCutCollection *)
                fCutContainers[static_cast<Int_t>(ENicaCutUpdate::kTrackUpdate)]
                    ->UncheckedAt(collection))
        ->PassTrack(track);
  };
  /**
   * check pair cuts
   * @param pair pair pair of tracks  to check
   * @param collection pair collection number used for checking
   * @return true if passed false otherwise
   */
  inline Bool_t PassSignalPair(NicaTwoTrack *pair, const Int_t collection) {
    return ((NicaCutCollection *)
                fCutContainers[static_cast<Int_t>(
                                   ENicaCutUpdate::kTwoTrackUpdate)]
                    ->UncheckedAt(collection))
        ->PassTwoTrack(pair);
  };
  /**
   * check pair cuts
   * @param pair pair pair of tracks  to check
   * @param collection pair collection number used for checking
   * @return true if passed false otherwise
   */
  inline Bool_t PassBackgroundPair(NicaTwoTrack *pair, const Int_t collection) {
    return ((NicaCutCollection *)
                fCutContainers[static_cast<Int_t>(
                                   ENicaCutUpdate::kTwoTrackBackgroundUpdate)]
                    ->UncheckedAt(collection))
        ->PassTwoTrack(pair);
  };
  /**
   *
   * @return report from this cut container
   */
  virtual NicaPackage *Report() const;
  virtual ~NicaCutContainer();
  ClassDef(NicaCutContainer, 1)
};

#endif /* NICACUTCONTAINER_H_ */

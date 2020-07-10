// -------------------------------------------------------------------------
// -----               CbmPointSetArrayEditor header file              -----
// -----                Created 05/07/22  by J. Brandt                 -----
// -----               Following class TEvePointSetArrayEditor         -----
// -------------------------------------------------------------------------


/** CbmPointSetArrayEditor
 * @author J. Brandt
 * @since 05.07.18
 *   Class to Edit Graphics of CbmPointSetArray
 *   gets automatically build with an CbmPointSetArray
 *   provides two ComboBoxes to chose Color- and Marker-Mode of CbmPointSetArray
 **
 **/

#ifndef CBMPOINTSETARRAYEDITOR_H
#define CBMPOINTSETARRAYEDITOR_H

#include <GuiTypes.h>    // for Pixel_t
#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, UInt_t
#include <TGFrame.h>     // for kChildFrame
#include <TGedFrame.h>   // for TGedFrame

class CbmPointSetArray;
class TGComboBox;
class TGWindow;
class TObject;


class CbmPointSetArrayEditor : public TGedFrame {
  CbmPointSetArrayEditor(const CbmPointSetArrayEditor&);  // Not implemented
  CbmPointSetArrayEditor&
  operator=(const CbmPointSetArrayEditor&);  // Not implemented

protected:
  CbmPointSetArray* fM;      // pointer to CbmPointSetArray
  TGComboBox* fComboColor;   // ComboBox for Color-Coding
  TGComboBox* fComboMarker;  // ComboBox for Marker-Coding

public:
  CbmPointSetArrayEditor(const TGWindow* p = 0,
                         Int_t width       = 170,
                         Int_t height      = 30,
                         UInt_t options    = kChildFrame,
                         Pixel_t back      = GetDefaultFrameBackground());
  ~CbmPointSetArrayEditor();

  virtual void SetModel(TObject* obj);

  void DoColor();   // perform when ComboBox Color is selected
  void DoMarker();  // perform when ComboBox Marker is selected

  ClassDef(CbmPointSetArrayEditor, 0);  // Editor for CbmPointSetArray class.
};

#endif

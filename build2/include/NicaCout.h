/*
 * NicaCout.h
 *
 *  Created on: 20-06-2013
 *      Author: Daniel Wielanek
 *      bumch of stuff used in many places
 */

#ifndef NICACOUT_H_
#define NICACOUT_H_
#include <TObject.h>
#include <TString.h>
#include "NicaStd.h"

/**
 * class for "friendly printing"
 */
class NicaCout : public TObject {
 private:
  static Int_t fgLineLength;
  static NicaCout* fgInstance;
  static ENicaInfo fVerboseMode;
  static TString GetShift(Int_t i);
  static TString MergeStrings(Int_t no, ...);
  static TString DisableColor();
  static TString EnableColor(Color_t Color);
  static void InStars(Int_t shift, TString text, Color_t color);
  static void PrintWarning(TString warning);
  static void PrintError(TString error);

 public:
  NicaCout();
  /**
   *
   * @return instance of this class
   */
  static NicaCout* Instance();
  /**
   *
   * @param lenght lenght of screen
   */
  static void SetLineLenght(Int_t lenght) { fgLineLength = lenght; };
  virtual ~NicaCout();
  /**
   * set verbose mode
   * @param verbose mode
   */
  static void SetVerboseMode(ENicaInfo verbose);
  /**
   * print strings in database format
   * @param no number of columns
   */
  static void Database(Int_t no...);
  /**
   * print shifted database
   * @param shift number of empty places
   * @param no number of columns
   */
  static void ShiftDatabase(Int_t shift, Int_t no...);
  /**
   * print text in stars
   * @param text text to print
   * @param color color of text (one of : kGray, kOrange, kYellow, kBlue,
   * kCyan,kWhite, kGreen, kViolet)
   */
  static void InStars(TString text, Color_t color = -1);
  /**
   * print shifted text in stars
   * @param shift number of spaces used for shift
   * @param text text
   * @param color color
   */
  static void ShiftInStars(Int_t shift, TString text, Color_t color = -1);
  /**
   * print formatted text
   * @param text text
   * @param option "L" -alight to left "M" - align to center, "R" - align to
   * right
   * @param color color
   */
  static void Text(TString text, TString option, Color_t color = -1);
  /**
   * print stars line
   * @param color color
   */
  static void Stars(Color_t color = -1);
  /**
   * print shifted text
   * @param shift number of spaces
   * @param text text
   * @param option  "L" -alight to left "M" - align to center, "R" - align to
   * right
   * @param color color
   */
  static void ShiftText(Int_t shift, TString text, TString option,
                        Color_t color = -1);
  /**
   * print "empty" line
   * @param color color
   */
  static void Empty(Color_t color = -1);
  /**
   * print percentage value by taking two parameters
   * @param actual actual value
   * @param total total value
   */
  static void Percent(Double_t actual, Double_t total);
  /**
   *print progress bar
   * @param acutal actual progress
   * @param total total progress
   */
  static void ProgressBar(Double_t acutal, Double_t total);
  /**
   * print info, basic method for printing info in code, use verbose value to
   * remove texts with low prioryty
   * @param text text
   * @param status statsu of text
   */
  static void PrintInfo(TString text, ENicaInfo status);
  /**
   * print line "fail/succeed"
   * @param value desrcription of problem
   * @param flag status of proglem
   * @param color color of status (description is in white)
   */
  static void FailSucced(TString value, TString flag, Color_t color);
  /**
   * print number if violet color
   * @param i value
   */
  static void DebugInfo(Int_t i);
  /**
   *
   * @return verbolse mode
   */
  static ENicaInfo GetVerboseMode() { return fVerboseMode; }
  ClassDef(NicaCout, 1)
};

#endif /* NICACOUT_H_ */

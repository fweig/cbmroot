/* Copyright (C) 2019 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Egor Ovcharenko [committer] */

#include "CbmMcbm2018UnpackerUtilRich.h"

#include <iostream>

std::string mRichSupport::GetBinaryRepresentation(size_t const size, uint8_t const* const ptr)
{
  std::string outString;

  unsigned char* b = (unsigned char*) ptr;
  unsigned char byte;

  char cStr[2];
  cStr[1] = '\0';

  for (int i = size - 1; i >= 0; i--) {
    for (int j = 7; j >= 0; j--) {
      byte = (b[i] >> j) & 1;
      sprintf(cStr, "%u", byte);
      outString.append(cStr);
    }
  }

  return outString;
}

/**
 * size in bytes
 */
std::string mRichSupport::GetHexRepresentation(size_t const size, uint8_t const* const ptr)
{
  std::string outString;

  unsigned char* b = (unsigned char*) ptr;
  unsigned char byte;

  char cStr[3];
  cStr[2] = '\0';

  for (int i = size - 1; i >= 0; i--) {
    byte = b[i] & 0xff;
    sprintf(cStr, "%02x", byte);
    outString.append(cStr);
  }

  return outString;
}

std::string mRichSupport::GetWordHexRepr(uint8_t const* const ptr)
{
  std::string outString;

  unsigned char* b = (unsigned char*) ptr;
  unsigned char byte[4];
  byte[0] = b[3] & 0xff;
  byte[1] = b[2] & 0xff;
  byte[2] = b[1] & 0xff;
  byte[3] = b[0] & 0xff;

  char cStr[10];
  cStr[9] = '\0';

  sprintf(cStr, "%02x%02x %02x%02x", byte[0], byte[1], byte[2], byte[3]);

  outString.append(cStr);

  return outString;
}

std::string mRichSupport::GetWordHexReprInv(uint8_t const* const ptr)
{
  std::string outString;

  unsigned char* b = (unsigned char*) ptr;
  unsigned char byte[4];
  byte[0] = b[0] & 0xff;
  byte[1] = b[1] & 0xff;
  byte[2] = b[2] & 0xff;
  byte[3] = b[3] & 0xff;

  char cStr[10];
  cStr[9] = '\0';

  sprintf(cStr, "%02x%02x %02x%02x", byte[0], byte[1], byte[2], byte[3]);

  outString.append(cStr);

  return outString;
}

void mRichSupport::SwapBytes(size_t const /*size*/, uint8_t const* ptr)
{
  unsigned char* b = (unsigned char*) ptr;
  unsigned char byte[4];
  byte[0] = b[3] & 0xff;
  byte[1] = b[2] & 0xff;
  byte[2] = b[1] & 0xff;
  byte[3] = b[0] & 0xff;

  b[0] = byte[0];
  b[1] = byte[1];
  b[2] = byte[2];
  b[3] = byte[3];
}

void mRichSupport::PrintRaw(size_t const size, uint8_t const* const ptr)
{
  size_t nWords = size / 4;
  //	size_t nRestBytes = size%4;

  for (size_t iWord = 0; iWord < nWords; iWord++) {
    //std::cout << GetHexRepresentation(4, ptr+iWord*4) << " ";

    std::cout << GetWordHexReprInv(ptr + iWord * 4) << " ";
  }
  /*if (nRestBytes > 0) {
		std::cout << GetHexRepresentation(nRestBytes, ptr+nWords*4) << " " << std::endl;
	} else {
		std::cout << std::endl;
	}*/
  std::cout << std::endl;
}

/* Copyright (C) 2008-2011 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

#ifndef CBMLITMEMORYMANAGMENT_H_
#define CBMLITMEMORYMANAGMENT_H_


class DeleteObject {
public:
  template<typename T>
  void operator()(const T* ptr) const
  {
    delete ptr;
  }
};


#endif /*CBMLITMEMORYMANAGMENT_H_*/

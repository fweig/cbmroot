/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Sebastian Heinemann */
#ifndef CBM_ALGO_BASE_UTIL_NO_INIT_ALLOC_H
#define CBM_ALGO_BASE_UTIL_NO_INIT_ALLOC_H

#include <cstdlib>

namespace cbm::algo
{

  template<class T>
  class NoInitAlloc {
  public:
    using value_type = T;

    T* allocate(size_t size) { return static_cast<T*>(std::malloc(size * sizeof(T))); }
    void deallocate(T* p_t, size_t) { std::free(p_t); }

    template<class U, class... Args>
    void construct(U*, Args&&...)
    {
    }
  };

}  // namespace cbm::algo

#endif  //CBM_ALGO_BASE_UTIL_NO_INIT_ALLOC_H

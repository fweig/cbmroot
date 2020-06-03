#ifndef CbmL1Def_h
#define CbmL1Def_h 1

// #define FAST_CODE // FAST_CODE = more unsafe


#include "TStopwatch.h"
#include <assert.h>
#include <vector>
#include <iostream>
#ifdef HAVE_SSE
  #include "vectors/P4_F32vec4.h"
#else
  #include "vectors/PSEUDO_F32vec4.h"
  #error NoSseFound
#endif // HAVE_SSE

//#include "vectors/PSEUDO_F64vec1.h"

template< typename T>
T finite(T x) { return std::isfinite(x); }

#ifdef FAST_CODE

#define L1_NO_ASSERT // use with asserts, etc.
#define L1_DYNAMIC_CAST static_cast

#else // FAST_CODE

#define L1_DYNAMIC_CAST dynamic_cast

#endif // FAST_CODE 

#if 1 && defined( __GNUC__ ) && __GNUC__ - 0 >= 3   // for speed up conditions
#define ISLIKELY(  x )    __builtin_expect( !!( x ),1 )
#define ISUNLIKELY(  x )  __builtin_expect( !!( x ),0 )
#else
#define ISLIKELY(  x )   (  x )
#define ISUNLIKELY(  x )  (  x )
#endif

#if defined( NDEBUG ) || defined( L1_NO_ASSERT )
#define L1_ASSERT(v, msg)
#define L1_assert(v)
#else
#define L1_ASSERT(v, msg) \
if (v) {} else { \
  std::cerr << __FILE__ << ":" << __LINE__ << " assertion failed: " \
            << #v << " = " << (v) << "\n" << msg << std::endl; \
  abort(); \
}
#define L1_assert(v) assert(v)
#endif


using namespace std;

typedef int index_type;


template <typename T> class L1Vector: public std::vector<T>
{
 public:
  
   L1Vector(): std::vector<T>(), fSize(0) {}; 
   L1Vector(const unsigned int n): std::vector<T>(n), fSize(0) {}; 
    L1Vector(const unsigned int n, const unsigned int value): std::vector<T>(n, value), fSize(0) {}; 

   
   unsigned int  Size() const { return fSize; } // Size() return number    
   void Reset() { fSize = 0; }
   
   void Resize(const unsigned int n)
   { 
     if(n > std::vector<T>::size())
     {
       #ifdef _OPENMP
       #pragma omp critical
       #endif
       std::vector<T>::resize(n);
     }
     
     fSize = n;
   }
   
   void Store(const T& element)
   {
     if(fSize >= std::vector<T>::size())
     {
       #ifdef _OPENMP
       #pragma omp critical
       #endif
       std::vector<T>::push_back(element);
     }
     else
       std::vector<T>::at(fSize) = element;
     
     fSize++;
   }
   
   
   T& operator[] (const size_t index)
   { 
     if(index >= std::vector<T>::size())
     {
       #ifdef _OPENMP
       #pragma omp critical
       #endif
       std::vector<T>::resize(index+1);
       std::cout<<index<<" index "<<std::endl;
     }
     if(index>=fSize)
       fSize = index + 1;
       
     return std::vector<T>::operator[](index);
   }
   
   const T& operator[] (const size_t index) const
   { 
   
     return std::vector<T>::operator[](index);
   }
   
 private:
  unsigned int fSize;
};

#endif // CbmL1Def_h

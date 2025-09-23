#include <algorithm> // std::swap()

#include "median.h"

void median_of_3(int a[], std::size_t len) {
  int *left = a;
  int *mid = a + len/2;
  int *right = a + std::max((int)len - 1, 0); 
  
  if (*left > *mid)
  {
    std::iter_swap(left,mid);
  }
  if (*mid > *right)
  {
    std::iter_swap(mid,right);
  }
  if (*left > *mid)
  {
   std::iter_swap(left,mid);
  }

  std::iter_swap(mid,right);
}

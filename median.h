#ifndef MEDIAN_H
#define MEDIAN_H

#include <cstddef>

/*!
  This function finds the median among the first, last, and the middle elements
  in the array provided and swap some of these three elements so that the median 
  value found will occupy the last valid position of the array.

  @param a The array we wish to operate on.
  @param len The array's length.
*/
void median_of_3(int a[], std::size_t len);

#endif // MEDIAN_H

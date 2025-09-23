/*!
 *  Exercise: implementing partition and quicksort
 *  @date September, 2nd.
 *
 *  In this exercise you should complete the code below,
 *  implementing the partition algorithm presente earlier.
 *
 *  After that, think about how would you apply the partition
 *  result to design an algorithm to sort ranges.
 *
 *  Think recursively and draw some inspiration from methods
 *  such as binary search.
 *
 */
#include <algorithm>
#include <array>      // std::array
#include <functional> // std::less, std::function<>
#include <iostream>   // cout, endl
#include <iterator>   // std::next

//=== Alias and constants
using Compare = bool (*)(int, int); //!< Comparison function needed for sorting.
// using Compare = std::function< bool(int, int) >; // alternative version.

void print(const int *first, const int *last) {
  std::cout << "[ ";
  std::copy(first, last, std::ostream_iterator<int>(std::cout, " "));
  std::cout << "] ";
}

/*!
 * Partition reorders the elements in the range [first;last) in such a way that
 * all elements **less** than the pivot appear before the pivot, and all
 * elements equal or greater than the pivot appear after the pivot.
 *
 * Any relative order among elements of the array might not be preserved.
 *
 * By **less** we mean a _strict order_ binary relation among the elements we
 * wantt to partition.
 *
 * \note We assume the pivot is a valid iterator in [first; last).
 *
 * @param first The first element in the range we want to reorder.
 * @param last Past-the-last element in the range we want to reorder.
 * @param pivot Location of the pivot element we need to partition the array
 * with.
 * @param comp A comparison function that returns true if the first parameter is
 * **less** than the second.
 * @return An iterator to the new pivot location within the range.
 */
int *partition(int *first, int *last, int *pivot, Compare comp) {
  auto fast = first;
  auto slow = first;
  
  while (fast != pivot)
  {
    if (comp(*fast,*pivot))
    {
      std::iter_swap(fast, slow);
      slow++;
    }
    fast++;
  }
  std::iter_swap(slow, pivot);
 
  return slow;
}

void quicksort(int *first, int *last, Compare cmp) {
  if (std::distance(first,last) < 2)
  {
    //nada a fzr, caso base
    return;
  }
  
  auto *pivot = partition(first, last, std::prev(last), cmp);
  
  //recursao
  quicksort(first, pivot, cmp);
  quicksort(std::next(pivot), last, cmp);
};

/*!
 * This function return `true` if the first parameter is **less**
 * than the second parameter.
 * By **less** we mean a relation of _strict order_ that there exists
 * among elements of the type int.
 * \param a The first element.
 * \param b The second element.
 * \return true if a < b, false otherwise.
 */
bool compare(int a, int b) { return a < b; }

// The driver function.
int main() {
  // The array we wish to partition, then sort.
  constexpr size_t ar_size = 10; //!< The array size.
  std::array<int, ar_size> A{9, 8, 1, 2, 10, 7, 5, 3, 4, 6};

  //================================================================================
  // Print the original array.
  //--------------------------------------------------------------------------------
  std::cout << ">>> Original array: ";
  print(A.begin(), A.end());
  std::cout << "\n";

  //================================================================================
  // Do the partition process.
  //--------------------------------------------------------------------------------
  std::cout << ">>> Partitioning the array...\n";
  // Choosing the pivot.
  int *target{&A.back()}; // Choosing the last element as the pivot.
  // int * target { A.begin()+3 };  // Choosing an alternative pivot.
  std::cout << ">>> Pivot is " << *target << ", located at position "
            << std::distance(A.begin(), target) << std::endl;

  //*** Calling the partition function ***//
  auto *pivot = partition(A.begin(), A.end(), target, compare);

  //================================================================================
  // Show (possibly) new location of pivot after partition.
  //--------------------------------------------------------------------------------
  std::cout << ">>> Pivot is now located at position "
            << std::distance(A.begin(), pivot) << ".\n";
  // Visually show the partition while printing out the modified array.
  print(A.begin(), pivot);
  std::cout << " " << *pivot << " "; // The pivot, then the second part.
  print(std::next(pivot), A.end());
  std::cout << "\n";

  //================================================================================
  // TODO: after successfully finishing partition, code the quick sort.
  //--------------------------------------------------------------------------------

  std::cout << ">>> Normal exiting...\n\n";
  return EXIT_SUCCESS;
}

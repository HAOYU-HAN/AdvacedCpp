#include <algorithm>
#include <iostream>
#include <iterator>

#include "btree.h"

void foo(const btree<int> &b) {
  std::copy(b.begin(), b.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
}

int main(void) {
  btree<int> b;

  b.insert(1);
  b.insert(10);
  b.insert(3);
  b.insert(4);
  b.insert(45);
  b.insert(41);
  b.insert(23);
  b.insert(784);
  b.insert(3);
  b.insert(25);
  b.insert(265);
  b.insert(122);
  b.insert(2325);
  b.insert(2666);
  b.insert(1232);
  b.insert(452);
  b.insert(27);

  for(btree<int>::iterator iter = b.begin(); iter != b.end(); ++iter)
    std::cout << *iter << std::endl;

  foo(b);
  
  return 0;
}

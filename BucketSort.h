#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

#include <vector>
#include <thread>
#include <mutex>

using std::vector;
using std::mutex;
using std::thread;

struct BucketSort {

    void sort(unsigned int numCores);

    void separate(int index);
    void localSort(int* target);

    // vector of numbers
    vector<unsigned int> numbersToSort;
    //separate the numbers in the 3D vector depended by index number and which thread handle it
    vector<vector<vector<unsigned int>>> smallBucketVec_;
    //store the max thread number which can by a computer
    int nMaxThd;
    //global lock using in merge partition code
    mutex globalMutex;
};


#endif /* BUCKET_SORT_H */
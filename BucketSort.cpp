#include "BucketSort.h"

#include <algorithm>
#include <iostream>
#include <cmath>
    template<typename F>
void locked_scope(std::mutex& m, F f)
{
    std::lock_guard<std::mutex> lg{m};
    f();
}
//x < y
bool aLessB(const unsigned int& x, const unsigned int& y) {

    if (x == y) return false; // if the two numbers are the same then one is not less than the other

    unsigned int a = x;
    unsigned int b = y;
    std::string a_str = std::to_string(a);
    std::string b_str = std::to_string(b);
    // work out the digit we are currently comparing on.
    for (unsigned int i = 0; i < a_str.length(); ++i) {
        //cout<<"hhh"<<endl;
        if (a_str[i] < b_str[i]){

            return true;}
        else if (a_str[i] > b_str[i]){



            return false;
        }

    }
    if (a_str.length() < b_str.length()){

        return true;
    }

}

void BucketSort::sort(unsigned int numCores) {

    //support the single core computer
    if(numCores == 1)
        numCores = 2;

    //process will hold nMaxThd threads including main thread
    nMaxThd = numCores - 1;

    //init 3D vector,all except last vector<vector<unsigned int>> served by threads separately
    //the last vector<vector<unsigned int>>'s job is used to merge other vector value

    for (unsigned int i = 0; i < numCores ; i++) {
        vector<vector<unsigned int>> vv{};
        for (int j = 0; j < 9; j++) {
            vv.push_back(vector<unsigned int>{});
        }
        smallBucketVec_.push_back(vv);
    }

    //divide the whole vector to nMaxThd partition,every thread handle one part
    //the thread assign the number in to different vector depends on the index number
    vector<thread> thdVec;
    int i;
    for(i = 0;i < nMaxThd - 1;i++)
    {
        thdVec.push_back(thread{&BucketSort::separate,this,i});
    }
    i = nMaxThd - 1;
    separate(i);
    for(auto& t:thdVec)
        t.join();
    thdVec.clear();
    numbersToSort.clear();

    //merge the smallbucket in to smallBucketVec_[nMaxThd] and sort
    int target[] = {1,1,1,1,1,1,1,1,1};
    for(int j = 0;j<9;j++)
        target[j] = 1;
    for(int j = 0;j < nMaxThd - 1;j++)
    {
        thdVec.push_back(thread{&BucketSort::localSort,this,target});
    }
    localSort(target);
    for(auto& t:thdVec)
        t.join();
    thdVec.clear();

    //merge the smallBucketVec_[nMaxThd] in to numbersToSort vector
    for(int numberIndex= 0;numberIndex< 9;numberIndex++)
    {
        numbersToSort.insert(numbersToSort.end(),
                smallBucketVec_[nMaxThd][numberIndex].begin(),
                smallBucketVec_[nMaxThd][numberIndex].end());
    }
    smallBucketVec_.clear();
}

void BucketSort::localSort(int* target)
{
    while(true)
    {
        int id = -1;

        //polling task
        //value one means radix(i) is not handled by any threads
        //value zero means radix(i) already have thread handled.
        locked_scope(globalMutex,[target,&id](){
                for(int i = 0;i < 9;i++)
                {
                if(target[i] == 1)
                {
                id = i;
                target[i] = 0;
                break;
                }
                }
                });

        //merge and sort radix(i) or quit thread
        if(id > -1)
        {
            for(int threadIndex = 0;threadIndex < nMaxThd;threadIndex++)
            {
                smallBucketVec_[nMaxThd][id].insert(smallBucketVec_[nMaxThd][id].end(),
                        smallBucketVec_[threadIndex][id].begin(),
                        smallBucketVec_[threadIndex][id].end());
            }
            std::sort(smallBucketVec_[nMaxThd][id].begin(),smallBucketVec_[nMaxThd][id].end(),
                    [](const unsigned int&x ,const unsigned int&y){
                        return aLessB(x,y);
                    });
        }
        else
            return;
    }
}

void BucketSort::separate(int index)
{
    unsigned int scopeBeginIdx = index * numbersToSort.size()/nMaxThd;
    unsigned int scopeEndIdx = (index + 1) * numbersToSort.size()/nMaxThd - 1;
    if(index + 1 == nMaxThd)
        scopeEndIdx = numbersToSort.size() - 1;

    for(unsigned int i = scopeBeginIdx;i <= scopeEndIdx;i++)
    {
        unsigned int value = numbersToSort[i];
        unsigned int numid = value;
        while(numid/10 > 0)
            numid = numid / 10;
        numid --;

        smallBucketVec_[index][numid].push_back(value);
    }
}

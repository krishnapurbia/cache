#include <bits/stdc++.h>
using namespace std;


enum MESIState { INVALID = 0, SHARED, EXCLUSIVE, MODIFIED };


unsigned int parseAddress(const string &addrStr) {

    if(addrStr.rfind("0x", 0)==0 || addrStr.rfind("0X",0)==0)



        return stoul(addrStr.substr(2), nullptr, 16);


    return stoul(addrStr, nullptr, 16);
}

struct CacheLine {
    unsigned int tag;

    MESIState state;
    unsigned long long lru;


    CacheLine() :  tag(0), state(INVALID), lru(0) {}


};


struct Cache {
    int s, E, b;


    int S, B;

    vector<vector<CacheLine>> sets;

    static unsigned long long globalLRU;

    Cache(int s_, int E_, int b_) : s(s_), E(E_), b(b_) {

        S = 1 << s;

        B = 1 << b;

        sets.assign(S, vector<CacheLine>(E));

    }

    int findLine(unsigned int tag, int setIdx) {

        for(int i = 0; i < E; i++) {

                  if(sets[setIdx][i].state!=INVALID && sets[setIdx][i].tag == tag)return i;


        }
        return -1;


    }

    int findInvalidLine(int setIdx) {


        for(int i = 0; i < E; i++) {if(sets[setIdx][i].state == INVALID) return i;}


        return -1;


    }

    /// *************AKK kamkar totaol cycles  = max of all cores totalcycles
    // Find least-recently-used line index in set (assumes all valid)
    int findLRULine(int setIdx) {

                  unsigned long long minLRU = ULLONG_MAX;


                        int lruIdx = 0;
        for(int i = 0; i < E; i++) {

            if(sets[setIdx][i].lru < minLRU) {

                minLRU = sets[setIdx][i].lru;

                
                lruIdx = i;
            }
        }
        return lruIdx;
    }
};
#define ll long long int


unsigned long long Cache::globalLRU = 0;

// Per-core statistics
struct Stats {
      long readOps=0, writeOps=0, misses=0,evictions=0, writebacks=0,busRd=0, busRdX=0, busWB=0,idleCycles=0, totalCycles=0;

    ll bustr = 0;

    ll dataTrafficBytes = 0;


    ll invalidations = 0;


};

// Bus request types
enum BusType { BusRd, BusRdX,  BusWB , Cache_to , from_Cache};
// BusWB used when doing writeback when eviction of midified state is taken place
// BusWBM/RFO  when write back in other cache occur due to modification in  this state so ,
// we have to writeback that otherstate make that invalid , this will take 100 cycles
//  in MESI: always write back dirty block before invalidation, even if it looks "wasteful".
struct BusRequest {

    int core;



    unsigned int tag;


    int setIndex;


    BusType type;



    MESIState newstate;

    unsigned int newtag;



    int othercore = -1;


    int isitshared = 0;


    ll add = -1;
};

long long int  total_invalidations = 0;


long long int bus_traffic = 0;  //( noofmisses + noofofwritebacks)



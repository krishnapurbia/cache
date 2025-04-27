#include "MESI.h"
#include "cache.cpp"
class Add {
    public:
        ll tag;
        ll setindex;
        vli line = {-1,-1,-1,-1};
        Add() {

        }
        Add(ll tg , ll iin) {
            tag = tg; 
            setindex = iin;
        }
        void refresh ( vector <Cache> cores , ll no ) {
                    for( int j = 0 ;j < noofblocks;j++){
                        if ( cores[no].maze[setindex][j].add == tag ){
                            line[no] = j;break;
                        }
                    }
                }
        };


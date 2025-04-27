#include "block.cpp"



ll noofsets = 0 ;
ll assoc = 0;
ll blocksize = 0;
ll offset = 0;
ll noofblocks = 0;
class Cache {
    public:
        ll lastcycletillvalid = 0;
        bool active = true; // when all instruction it becomes false 
       
        vector < vector < Block>> maze;
        Cache() { 
            maze.clear();
           For( i , 0, noofsets)maze.Pb(vector<Block>(noofblocks));
        };
        ~Cache () {
            maze.clear();
        }
        // now we need return LRU line at particular set
        // we are given the address we need to find it is there or not
        // and see if the empty lien is present or not 
        // these threee fucntionaly needed in 
        // one set function that set the paramter of particular block according to paramters given
        void set ( ull time , ull add , MESI pos , ll setind , ll lineno ){ 
                maze[setind][lineno].time = time;
                maze[setind][lineno].add = add;
                maze[setind][lineno].pos = pos;
            }
        ll isemptypresent ( ll ad ){ // isempty present
            ll ind = indf ( ad);
                For( i , 0 , noofblocks){
                        if ( maze[ind][i].add == -1 ) return i; // noone here
                }
                return -1; // not present  now have to remove LRU
        }
        ll least ( ll ad ){
            ll ind = indf ( ad);
                int j = 0 ;
                For( i , 1 , noofblocks) {
                     if ( maze[ind] [ i ].time < maze [ ind ][j].time ) j = i;
                }
                return j;
        }    
        ll indf ( ll ad ){ 
            ll ind = 0;
            if ( noofsets > 1 ){
                ind = ( ad / blocksize ) & ( noofsets - 1);
            }
            return ind;
        }
        ll isblock( ll ad ){ // hit or miss
                ll ind = indf( ad );
                ll tag = ad/(noofblocks+noofsets);
                int j = -1;
                for( int i = 0 ;  i < noofblocks;i++){
                        if ( maze [ ind ] [ i ].add == tag && maze[ind][i].pos != I) return i;
                        else if (maze [ ind ] [ i ].add == tag ) j = i; // invalid state when it will be become valid in future
                        // anything that never beecome valid will have address ie tag = -1
                }
                return j;
        }
        
};

bool done ( vector < Cache > a ){
        for( const auto & x : a ){
            if ( x.active )return false;
        }
        return true;
}
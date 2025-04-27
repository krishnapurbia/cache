
#include "help.cpp"
#include "block.cpp"
#include "cache.cpp"



/*
struct Stats {
 0   long readOps = 0;     // number of read instructions
  1  long writeOps = 0;    // number of write instructions
  2  long misses = 0;      // total cache misses
  3  long evictions = 0;   // cache evictions (replacements)
  4  long writebacks = 0;  // writebacks to memory (dirty evictions or flushes)
  5  long idleCycles = 0;  // cycles spent waiting on cache misses
  6  long totalCycles = 0; // total execution cycles for the core
};

*/
string trace;
string outputtxt;

ll parsehex ( char address[] ){
        return stoul ( address ,  0 , 16);
}
vector < vector < pair < ll ,ll >>> parser (){
    string a = trace+"_proc0.trace";
    FILE *ff  = fopen ( a.c_str(), "r");
    vector < vector < pair < ll , ll>>> parsed (4);
    char mode; char ad[100];
        while ((fscanf(ff, "%c %s", &mode , ad) == 2)){
            if ( mode = 'R') parsed[0].push_back ( { 0 , parsehex(ad)});
            else parsed[0].push_back ( { 1 , parsehex(ad)});
             }
        a = trace+"_proc1.trace";
        ff = fopen ( a.c_str(), "r");
        while ((fscanf(ff, "%c %s", &mode , ad) == 2)){
            if ( mode = 'R') parsed[1].push_back ( { 0 , parsehex(ad)});
            else parsed[1].push_back ( { 1 , parsehex(ad)});
             }
        a = trace+"_proc2.trace";
        ff = fopen ( a.c_str(), "r");
        while ((fscanf(ff, "%c %s", &mode , ad) == 2)){
            if ( mode = 'R') parsed[2].push_back ( { 0 , parsehex(ad)});
            else parsed[2].push_back ( { 1 , parsehex(ad)});
             }
        a = trace+"_proc3.trace";
        ff = fopen ( a.c_str(), "r");
        while ((fscanf(ff, "%c %s", &mode , ad) == 2)){
            if ( mode = 'R') parsed[3].push_back ( { 0 , parsehex(ad)});
            else parsed[3].push_back ( { 1 , parsehex(ad)});
             }
        return parsed;
}
ll globalclock = 0 ;

int main(int argc, char *argv[]) {

        
          For( i ,1 ,argc){
            if ( argv[i] == "-h" ) {
                Help();
                return 0;
            }
            if ( i + 1 < argc  ){
                if ( argv[i] == "-s" ) {
                        noofsets = stoi( argv [i+1]); // noof sets
                        noofsets = 1 <<noofsets;
                }
                else if ( argv[i] == "-b" ) {
                    blocksize = stoi(argv[i+1]); // 2 ** b the size of block in bits 
                    blocksize = 1 << blocksize;
                }
                else if ( argv[i] == "-E" ) {
                    noofblocks = stoi (argv[i+1]); // how many blocks perset
                }
                else if ( argv[i] == "-t" ) {
                    trace = argv[i+1];
                }
                else if ( argv[i] == "-o" ) {
                    outputtxt = argv[i+1];
                }
                else {
                    pr ("Usage: ./a.out -t app1 -s 5 -E 2 -b 5 -o output.txt");
                    return 0;
                }
            }
                else {
                    pr ("Usage: ./a.out -t app1 -s 5 -E 2 -b 5 -o output.txt");
                    return 0;
                }
                i++;      
        }
        vector <Cache> allcores ( 4 );
        ll invalida = 0 , traffic = 0 ;


        vector < vector < pair < ll , ll>>> parsed = parser();
        
       For(  i , 0, 4 ){
        reverse(parsed[i].begin() , parsed[i].end());
       }
        
        vli stalls( 4 );
        vvli requiredforcores( 7 );
        while( true ) {
            bool gh = true;
            For( i  ,0 ,4){
               if (!parsed[i].empty()){
                        requiredforcores[i][6]++;
                        if ( stalls[ i ] > 0 ){
                            stalls[i]--;continue;
                            requiredforcores[i][5]++;
                        }
                        struct Stats {
                            0   long readOps = 0;     // number of read instructions
                             1  long writeOps = 0;    // number of write instructions
                             2  long misses = 0;      // total cache misses
                             3  long evictions = 0;   // cache evictions (replacements)
                             4  long writebacks = 0;  // writebacks to memory (dirty evictions or flushes)
                             5  long idleCycles = 0;  // cycles spent waiting on cache misses
                             6  long totalCycles = 0; // total execution cycles for the core
                           };
                           ll whichline = allcores[i].isemptypresent( parsed[i].back().second);
             // case of hit            
                if ( whichline ==0 ){
                    if ( parsed[i].back().first == 0){
                                    
                    }
                    else{

                    }
                }
                else{
                    if ( parsed[i].back().first == 0){

                    }
                    else{
                        
                    }
                }
                           
            
            }
            if ( gh )break;
        }

}



#include "datast.cpp"


int main(int argc, char *argv[]) {
    // Command-line parsing
    string traceInput;
    int s=-1, E=-1, b=-1;
    string outFile;
    for(int i = 1; i < argc; i++) {
        string arg = argv[i];
        if(arg=="-t" && i+1<argc) traceInput = argv[++i];
        else if(arg=="-s" && i+1<argc) s = stoi(argv[++i]);
              else if(arg=="-E" && i+1<argc) E = stoi(argv[++i]);
        else if(arg=="-b" && i+1<argc) b = stoi(argv[++i]);
        else if(arg=="-o" && i+1<argc) outFile = argv[++i];
        else { cerr<<"Unknown option "<<arg<<endl; return 1; }
    }
    if(traceInput.empty() || s<0 || E<0 || b<0) {
        cerr<<"Usage: "<<argv[0]<<" -t <tracefile> -s <s> -E <E> -b <b> -o <outfile>\n";
        return 1;
    }
    
    Cache caches[4] = {
        Cache(s,E,b), Cache(s,E,b),
        Cache(s,E,b), Cache(s,E,b) };
    Stats stats[4];



    ifstream fin(traceInput);
    vector<ifstream> traceFiles(4);
        for(int core=0; core<4; core++) {
                   string f1 = traceInput+"_proc"+to_string(core)+".trace";
            traceFiles[core].open(f1);
        }

    vector<bool> coreDone(4,false);
    deque<BusRequest> busQueue;
         bool busBusy = false;
    long busCycles = 0;
    BusRequest currentReq;
    int blockWords = caches[0].B / (4); // words per block
    vector<char> op(4); vector<unsigned int> addr(4);
    // Simulation loop (cycle-by-cycle)
    bool allDone = false;
          int clk = 0;
    vector<long long int> Btr(4 ,0);
      vector <bool > requested ( 4 , false );
     vector <bool > accepted ( 4 , false );
    vector <bool> employedincachetocache( 4 , false );
    while(true) {
        bool didbusopetakeplace = false;
        clk++;
        allDone = true;
        // 2. Bus cycle: start or continue transaction
        std::function<void()> gunc = [&] () {
            if(!busBusy && !busQueue.empty()) {
                currentReq = busQueue.front();
                int core = currentReq.core;
                int setIndex = currentReq.setIndex;
                unsigned int tag = currentReq.tag;
                if ( core == 3) {
                    int y = 33;
                }
                //prevbl[core] = false;
                busQueue.pop_front();
                busBusy = true;
                allDone = false;

                // Determine latency: check if any other cache has this block
                bool anyOther=false, anyMod=false;
                int oth = -1;
                for(int other=0; other<4; other++) {
                    if(other==currentReq.core) continue;
                    int li = caches[other].findLine(currentReq.tag, currentReq.setIndex);
                    if(li>=0 && caches[other].sets[currentReq.setIndex][li].state != INVALID) {
                               anyOther = true;
                        oth = other;
                        if(caches[other].sets[currentReq.setIndex][li].state == MODIFIED) {
                            anyMod = true;
                        }
                    }
                }
                if ( currentReq.type == BusWB) {
                    stats[currentReq.core].dataTrafficBytes++;
                    stats[currentReq.core].writebacks++;
                    busCycles = 100 ;
                    // ak confusion he ki us state ko
                    // invalid kar ya naho aaccording to
                    // chatgpt INVLAID karde KYUKI WAHA PE DATA SAFE NAI HE
                    // par wo end me karunga par last me state vese hi dusre me hi chagne hogi to no dikkat


                }

                else if(anyMod) {
                    // One had M: flush (100) + mem fetch (100) + transfer
                    // as its writeback
                    bool gh = false;
                    for(int other=0; other<4; other++) {
                        if(other==core) continue;
                         Cache &oc = caches[other];
                        int li = oc.findLine(tag, setIndex);
                        if(li>=0 && oc.sets[setIndex][li].state != INVALID) {
                            if (gh) {
                               // cout <<"error at 196 "<<endl;
                            }
                            if ( oc.sets[setIndex][li].state!= MODIFIED) {
                               // cout << oc.sets[setIndex][li].state << endl;
                               // cout << "Error occured at line 230" <<endl;
                                // how a modfiied state can allow E or S other state also not possible
                            }
                            else {
                                for ( int other=0; other<4; other++) {
                                    if (other ==core) continue;
                                    int li = caches[other].findLine(tag, setIndex);
                                    if(li>=0 && oc.sets[setIndex][li].state  == SHARED) {
                                        cout <<"erer" <<endl;
                                    }
                                }
                                if (currentReq.type == BusRdX ) {
                                    // hame ise idhr hi invalidate now karna padega

                                    // ownership ka seen he yeh
                                    
                                    stats[core].invalidations++;
                                    total_invalidations++;
                                    
                                    //oc.sets[setIndex][li].state = INVALID;
                                    // hame on the spot invalidate karna padta he point to remember he ye
                                    // if we want exculisve then we have to invalidate this
                                    busQueue.push_front(currentReq);
                                    busQueue.push_front({other,currentReq.tag,currentReq.setIndex,BusWB ,INVALID,currentReq.tag});
                                    Btr[other]++;
                                    busBusy = false;
                                    gunc();
                                }
                                else {

                                    // only writbacks++ in other cache not here
                                  //  stats[other].writebacks++;
                                   
                                    
                                    busQueue.push_front(currentReq);

                                    busQueue.push_front({other,currentReq.tag,currentReq.setIndex,BusWB ,SHARED,currentReq.tag});
                                    Btr[other]++;
                                    busBusy = false;
                                    gunc();
                                }
                                gh = true;
                            }

                        }
                    }

                } else if(anyOther) {

                    // Others had E/S: cache-to-cache transfer
                    // kya yeh assupmtion sahi he ki jab write core1 , data core0 , me to phele date core0 se lo , then
                    // use modify kardo
                    if (currentReq.type ==BusRd) {
                        currentReq.isitshared = 1;
                        currentReq.othercore = oth;
                        busCycles = 2*blockWords  ;// requires one exrta cycle to complete
                        stats[currentReq.othercore].dataTrafficBytes++;
                        stats[currentReq.core].dataTrafficBytes++;
                    }
                    else {
                        stats[core].invalidations++;
                        for ( int other=0; other<4; other++) {
                            int li = caches[other].findLine(currentReq.tag, currentReq.setIndex);
                            if (caches[other].sets[currentReq.setIndex][li].state != INVALID){
                                caches[other].sets[currentReq.setIndex][li].state = INVALID;
                               

                            }

                        }
                        stats[core].dataTrafficBytes++;
                        busCycles = 100;
                    }
                 //   Btr[oth]++;
                } else {
                    // No copy: memory fetch
                    // assumption 2*N not work in M to cache tranfer
                    stats[core].dataTrafficBytes++;
                    busCycles = 100 ; // requires one exrta cycle to complete
                }
            }
        };
        auto func = [&]() {
            gunc();
            // your code here
            if(busBusy) {
                busCycles--;
                didbusopetakeplace = true;
                allDone ;
                if(busCycles <= 0) {
                    // Complete bus transaction this cycle
                    int core = currentReq.core;
                    int setIndex = currentReq.setIndex;
                    unsigned int tag = currentReq.tag;
                    Cache &cache = caches[core];
                    if(currentReq.type == BusRd) {
                        // BusRd: others may respond or downgrade
                        bool shared = false;
                        for(int other=0; other<4; other++) {
                            if(other==core) continue;
                            Cache &oc = caches[other];
                            int li = oc.findLine(tag, setIndex);
                            if(li>=0 && oc.sets[setIndex][li].state != INVALID) {
                                MESIState ost = oc.sets[setIndex][li].state;
                                if(ost == MODIFIED) {
                                   // cout <<"error at 270 "<<endl;
                                    oc.sets[setIndex][li].state = SHARED;
                                 //   stats[other].writebacks++; // flush dirty data // this is done 100% done
                                    shared = true;
                                }
                                else if(ost == EXCLUSIVE) {
                                    oc.sets[setIndex][li].state = SHARED;
                                    shared = true;
                                } else if(ost == SHARED) {
                                    shared = true;
                                }
                            }
                        }

                        // Insert or update line for requesting core (S or E)
                        MESIState newState = (shared ? SHARED : EXCLUSIVE);
                        //— REPLACE from here: upgrade-in-place or allocate/evict —
                        int way = cache.findLine(tag, setIndex);
                        if (way >= 0) {
                            // we already had this block (Shared or Exclusive): just upgrade it
                            CacheLine &L = cache.sets[setIndex][way];
                            L.state = newState;
                            L.lru   = ++Cache::globalLRU;
                        } else {
                            // no copy: allocate in an empty way or evict LRU
                            int empty = cache.findInvalidLine(setIndex);
                            if (empty >= 0) {
                                cache.sets[setIndex][empty].tag   = tag;
                                cache.sets[setIndex][empty].state = newState;
                                cache.sets[setIndex][empty].lru   = ++Cache::globalLRU;
                            } else {
                                int victim = cache.findLRULine(setIndex);
                                CacheLine &V = cache.sets[setIndex][victim];
                                stats[core].evictions++;
                                if (V.state == MODIFIED) {
                                  //  stats[core].writebacks++; // writeback karna padega
                                    // victim.state = INVALID; // isko invalid kardo -> nahi kar rha dekhenge puchchenge kisise
                                    //  maybe kya isko end invalid kar u ya abhi confusion he  <----- Doubt he
                                    busQueue.push_front({core , V.tag , setIndex ,BusWB , newState , tag  });
                                   
                                    stats[core].busWB++;

                                    Btr[core]++;
                                }
                                else {
                                    V.tag   = tag;
                                    V.state = newState;
                                    V.lru   = ++Cache::globalLRU;
                                }
                            }
                        }
                    }
                        //— end replacement —
                    else if(currentReq.type == BusRdX) {
                        // BusRdX: invalidate all other copies

                        for(int other=0; other<4; other++) {
                            if(other==core) continue;
                            Cache &oc = caches[other];
                            int li = oc.findLine(tag, setIndex);
                            if(li>=0 && oc.sets[setIndex][li].state != INVALID) {
                                cout << "EROROROR 327 " <<endl; ///////--->>>> seeeeee gthis     
                                MESIState ost = oc.sets[setIndex][li].state;
                                if(ost == MODIFIED) {
                                    cout << "EROROROR 327 " <<endl;
                                 //   stats[other].writebacks++   ;
                                    // iska bhi kuch nhi -> count karliya ->
                                    // writekrnapadega ( RULE hai even though kam nahi ayega)
                                    oc.sets[setIndex][li].state = INVALID;
                                    // as seeen in chatgpt prompt we have to writeback even though no use of it after writeback
                                    // so what will do is that
                                }
                                oc.sets[setIndex][li].state = INVALID;

                                stats[core].invalidations++;
                            }
                        }

                        // Insert in M state at requesting core

                        int way = cache.findLine(tag, setIndex);
                        if (way >= 0) {
                            // we already had this block (Shared or Exclusive): just upgrade it
                            CacheLine &L = cache.sets[setIndex][way];
                            L.state = MODIFIED;
                            L.lru   = ++Cache::globalLRU;
                        } else {
                            // no copy: allocate in an empty way or evict LRU
                            int empty = cache.findInvalidLine(setIndex);
                            if (empty >= 0) {
                                cache.sets[setIndex][empty].tag   = tag;
                                cache.sets[setIndex][empty].state = MODIFIED;
                                cache.sets[setIndex][empty].lru   = ++Cache::globalLRU;
                            } else {
                                int victim = cache.findLRULine(setIndex);
                                CacheLine &V = cache.sets[setIndex][victim];
                                stats[core].evictions++;
                               // stats[core].invalidations++;

                                if (V.state == MODIFIED) {
                                  //  stats[core].writebacks++; // writeback karna padega
                                    // victim.state = INVALID; // isko invalid kardo -> nahi kar rha dekhenge puchchenge kisise
                                    //  maybe kya isko end invalid kar u ya abhi confusion he  <----- Doubt he
                                    busQueue.push_front({core , V.tag , setIndex ,BusWB , MODIFIED , tag  });
                                   // stats[core].dataTrafficBytes++;
                                    stats[core].busWB++;
                                    //stats[core].dataTrafficBytes++;
                                    Btr[core]++;
                                }
                                else {
                                    V.tag   = tag;
                                    V.state = INVALID;
                                    V.lru   = ++Cache::globalLRU;
                                }
                            }
                        }
                    }
                    else if (currentReq.type == BusWB) { // used for eviction
                        bool checker = false;
                        for ( int ece = 0 ; ece < E; ece++) {
                            if ( cache.sets[setIndex][ece].tag == tag) {
                                cache.sets[setIndex][ece].tag = currentReq.newtag;
                                cache.sets[setIndex][ece].state = currentReq.newstate;
                                cache.sets[setIndex][ece].lru = ++Cache::globalLRU;
                                checker = true;
                            }
                        }
                        if (!checker) {
                          //  cout << "Error at line 358" <<endl;
                        }
                    }
                    busBusy = false;

                    Btr[core]--;
                    BusRequest inv;
                    currentReq = inv;
                    // Unblock core: data is now available

                }
            }
        };
        // 2. Bus cycle: start or continue transaction
        bool wasbusy = busBusy;
        gunc();
        bool isbusy = busBusy;
        if(!wasbusy && isbusy){
            stats[currentReq.core].idleCycles++;
        }
        // 1. Core operations
        for(int core=0; core<4; core++) {
            if(coreDone[core] && currentReq.othercore != core) continue;
            allDone = false;
            stats[core].totalCycles++;
            if (currentReq.othercore == core) {
               continue;
            }
            else if ( Btr[core]>0  ) {
                 if (currentReq.othercore == core);
                 else if(currentReq.core == core);
                 else stats[core].idleCycles++;
                continue;
            }
            string addrStr;
            if(!(traceFiles[core]>>op[core]>>addrStr)) {
                coreDone[core] = true;
                if (coreDone[core] == true  ) {
                    stats[core].totalCycles--;
                }
                continue;
            }
            addr[core] = parseAddress(addrStr);
            unsigned int setIndex = (s>0 ? (addr[core]>>b) & ((1u<<s)-1) : 0);
            unsigned int tag = addr[core] >> (b + s);
            Cache &cache = caches[core];
            int lineIdx = cache.findLine(tag, setIndex);
            bool hit = (lineIdx>=0 && cache.sets[setIndex][lineIdx].state != INVALID);
            if(op[core]=='R') {
                // Read operationSet Index Bits: 5
                if(hit) {
                    stats[core].readOps++;
                    // Hit: update LRU
                    CacheLine &line = cache.sets[setIndex][lineIdx];
                    line.lru = ++Cache::globalLRU;

                } else {
                    stats[core].readOps++;
                    stats[core].totalCycles+=1; // twoc yc for cexe
                    stats[core].misses++;
                    stats[core].busRd++;
                    busQueue.push_back({core, tag, (int)setIndex, BusRd});

                    //stats[core].idleCycles++;
                    Btr[core]++;
                }
            }
            else if(op[core]=='W') {
                // Write operation
                if(hit) {
                    stats[core].writeOps++;
                    CacheLine &line = cache.sets[setIndex][lineIdx];
                    MESIState st = line.state;
                    if(st == MODIFIED) {
                        // M: already dirty, just LRU
                        for(int other=0; other<4; other++) {
                            if(other==core) continue;
                            Cache &oc = caches[other];
                            int li = oc.findLine(tag, setIndex);
                            if(li>=0 && oc.sets[setIndex][li].state != INVALID) {
                                MESIState ost = oc.sets[setIndex][li].state;
                              //  cout << ost << endl;
                             //   cout << "error ar 457 " <<endl;

                            }
                        }
                        line.lru = ++Cache::globalLRU;

                    } else if(st == EXCLUSIVE) {

                        // E->M (silent upgrade)
                        line.state = MODIFIED;
                        line.lru = ++Cache::globalLRU;

                    }
                    else if(st == SHARED) {
                        stats[core].busRdX++;
                       // stats[core].idleCycles++;
                        stats[core].totalCycles+=1;
                        busQueue.push_back({core, tag, (int)setIndex, BusRdX});

                        // didnt use upgr due to explicit mentiom in clsrification
                        // when modifiaction it takes fromn memory

                        Btr[core]++;

                    }
                } else {
                    stats[core].writeOps++;
                    stats[core].misses++;
                    stats[core].busRdX++;
                   // stats[core].idleCycles++;
                    stats[core].totalCycles+=1;
                    busQueue.push_back({core, tag, (int)setIndex, BusRdX});

                    Btr[core]++;

                }

            }
        }
        if (didbusopetakeplace != true ) func();
        bool workRemaining = false;
        for(int core=0; core<4; core++) {
            if(!coreDone[core] ) { workRemaining = true; break; }
        }
        if(!busQueue.empty() || busBusy) workRemaining = true;
        if(!workRemaining) break;
    }


    vector<double> missRate(4);
    for(int core=0; core<4; core++) {
               long acc = stats[core].readOps + stats[core].writeOps;
        missRate[core] = (acc>0 ? (double)stats[core].misses / acc : 0.0);
    }

    ostream *out = &cout;
    ofstream f;
    f.open("x.txt");
     out = &f;
     ll c = 0;
    for( int i = 0 ; i < 4;i++){
       
        if ( c < stats[i].totalCycles) {
           c = stats[i].totalCycles;
        }
    } 
   *out << "Total Execution Cycles:" << c <<endl;
   if (f.is_open()) f.close();
    out = &cout;
    ofstream fout;
    if (!outFile.empty()) {
        fout.open(outFile);
        if (!fout.is_open()) {
            cerr << "Error: cannot open " << outFile << endl;
            return 1;
        }
        out = &fout;
    }
    
    

    // Print simulation parameters
    *out << "Simulation Parameters:\n";
    *out << "Trace Prefix: " << traceInput << "\n";
    *out << "Set Index Bits: " << s << "\n";
    *out << "Associativity: " << E << "\n";
    *out << "Block Bits: " << b << "\n";
    *out << "Block Size (Bytes): " << (1 << b) << "\n";
    *out << "Number of Sets: " << (1 << s) << "\n";
    *out << "Cache Size (KB per core): " << ((1<<s)*(1<<b)*(E))/1024 << "\n";
    *out << "MESI Protocol: Enabled\n";
    *out << "Write Policy: Write-back, Write-allocate\n";
    *out << "Replacement Policy: LRU\n";
    *out << "Bus: Central snooping bus\n\n";

    ll totalInv = 0, totalRd = 0, totalRdX = 0, totalUpgr = 0;
    ll totalBusTransactions = 0;
    ll totalBusTrafficBytes = 0;

    for (int core = 0; core < 4; core++) {
        totalRdX += stats[core].busRdX;
        totalUpgr+= stats[core].busWB;
        totalInv += stats[core].invalidations;
        totalRd  += stats[core].busRd;
        totalBusTransactions += stats[core].busRd + stats[core].busRdX ;
        //    totalBusTransactions += stats[core].busRd + stats[core].busRdX + stats[core].invalidations;  // if indiv karna he to and har invalidations pe++ kardena he
        *out << "Core " << core << " Statistics:\n";
        *out << "Total Instructions: " << (stats[core].readOps + stats[core].writeOps) << "\n";
        *out << "Total Reads: " << stats[core].readOps << "\n";
        *out << "Total Writes: " << stats[core].writeOps << "\n";
        *out << "Total Execution Cycles: " << stats[core].totalCycles - stats[core].idleCycles << "\n";
        *out << "Idle Cycles: " << stats[core].idleCycles << "\n";
        *out << "Cache Misses: " << stats[core].misses << "\n";
        *out << fixed << setprecision(2);
        *out << "Cache Miss Rate: "
             << (stats[core].readOps + stats[core].writeOps > 0 ? (double)stats[core].misses * 100.0 / (stats[core].readOps + stats[core].writeOps) : 0.0)
             << "%\n";
        *out << "Cache Evictions: " << stats[core].evictions << "\n";
        *out << "Writebacks: " << stats[core].writebacks << "\n";
        *out << "Bus Invalidations: " << stats[core].invalidations << "\n";
        *out << "Data Traffic (Bytes): " << (stats[core].dataTrafficBytes) * ( 1 <<b) << "\n\n";

      
        // one block = noofwords * (size of on word) // ( 4 bytes) = sizeof block = 2**b

        totalBusTrafficBytes += (stats[core].dataTrafficBytes )* ( 1 <<b);
    }


    *out << "Overall Bus Summary:\n";
    *out << "Total Bus Transactions: " << totalBusTransactions << "\n";
    *out << "Total Bus Traffic (Bytes): " << totalBusTrafficBytes << "\n";

    if (fout.is_open()) fout.close();
    return 0;
}

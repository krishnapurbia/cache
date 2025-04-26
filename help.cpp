#include <bits/stdc++.h>
using namespace std;
 
typedef long long int ll;
typedef unsigned long long int ull;
typedef pair<int, int> pip;
typedef pair<ll, ll> plip;
typedef vector<int> vi;
typedef vector<ll> vli;
typedef vector<vi> vvi;
typedef vector<vli> vvli;
typedef vector<pip> vpip;
typedef unordered_set<ll> usi;
typedef set<ll> si;
typedef priority_queue<ll> pq;
typedef unordered_map<ll, ll> umpi;
typedef map<ll, ll> mpi;
 
 
const int MOD = 1000000007;
 
 
#define For(i, a, b) for (int i = (a); i < (b); i++)
#define Rep(i, n) for (int i = 0; i < (n); i++)
#define ALL(v) (v).begin(), (v).end()
#define Be(v) (v).begin()
#define Ee(v) (v).end()
#define Pb push_back
#define Mp make_pair
#define Sz(v) ((int)(v).size())
#define F first

#define pr(v) cout<<v<<endl;


void Help() {
    
    pr(
        "  -h : for help\n"
        "  -t <tracefile> : prefix of 4 trace files (e.g. app1)\n"
        "  -s <s> : no of set index bits (S = 2^s sets)\n"
        "  -E <E> : associativity (lines per set)\n"
        "  -b <b> : no of block offset bits (block size = 2^b bytes)\n"
        "  -o <outputfile> : file to write output statistics\n");
 
    
}
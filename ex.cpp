#include <bits/stdc++.h>

using namespace std;

#define vi vector<int>

#define str string


int main(int x, char* y[]) {
    if (x != 4) {

        cout << "sim_path trace csv" << endl;

        return 0;
    }

    str a = y [1] , b = y [2] , c = y [3];
   

    vector<int> d, e, f;

    int g = 2;
    for (int i = 0; i < 4; i++) {

        d.push_back(g);
        e.push_back(g);

        f.push_back(g);

        g *= 2;
    }

    e.push_back(g * 2);
    e.push_back(g * 4);

    ofstream h(c);
  

    h << "Para ,Val, Cycles\n";

    for (int &i : d) {
        string j = a + " -t " + b + " -s " + to_string(i) + " -E 2 -b 5 -o output.txt";
        int k = system(j.c_str());
        ifstream m("output.txt");string n;long long o = 0;
        while(getline(m, n)) {
                      if (n.find("Total Execution Cycles:") != string::npos) {
                                         int p = n.find(":");
                if (p != -1) {
                        string q = n.substr(p + 1);
                    long long r = atoll(q.c_str());
                           if (r > o) o = r;
                }
            }
        }
        m.close();h << "s," << i << "," << o << "\n";
    }

    for (int i : e) {
        string j = a + " -t " + b + " -s 6 -E " + to_string(i) + " -b 5 -o output.txt";
        int k = system(j.c_str());
        ifstream m("output.txt");
        string n;
        long long o = 0;
        while (getline(m, n)) {
            if (n.find("Total Execution Cycles:") != string::npos) {
                int p = n.find(":");
                if (p != -1) {
                    string q = n.substr(p + 1);long long r = atoll(q.c_str());
                    if (r > o) o = r;
                }
            }
        }
        m.close();h << "E," << i << "," << o << "\n";
    }
    for (int &i : f) {
        string j = a + " -t " + b + " -s 6 -E 2 -b " + to_string(i) + " -o output.txt";
        int k = system(j.c_str());
        ifstream m("output.txt");
        string n;
        long long o = 0;
        while (getline(m, n)) {
            if (n.find("Total Execution Cycles:") != string::npos) {
                int p = n.find(":");
                
                if (p != -1) {
                    string q = n.substr(p + 1);

                    long long r = atoll(q.c_str());

                    if (r > o) o = r;
                }
            }
        }
        m.close();h << "b," << i << "," << o << "\n";
    }
    h.close();
 

    return 0;
}

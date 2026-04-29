#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>

using namespace std;
struct member {
    string name;
    double paid = 0;
    double consumed = 0;
};
int main() {
    ifstream fin("input.txt");
    ofstream fout("output.txt");
    if (!fin.is_open()) return 1;
    int n;
    if (!(fin >> n)) return 0;
    vector<member> m(n);
    for (int i = 0; i < n; i++) {
        fin >> m[i].name;
    }
    string line;
    getline(fin, line);
    while (getline(fin, line)) {
        if (line.empty()) continue;
        for (int i = 0; i < (int)line.length(); i++) {
            if (line[i] == ':' || line[i] == ',') line[i] = ' ';
        }
        int slashposition = (int)line.find('/');
        string paypart;
        if (slashposition != -1) paypart = line.substr(0, slashposition);
        else paypart = line;
        stringstream sspay(paypart);
        string pname;
        double amount;
        if (!(sspay >> pname >> amount)) continue;
        int pindex = -1;
        for (int i = 0; i < n; i++) {
            if (m[i].name == pname) pindex = i;
        }
        if (pindex != -1) m[pindex].paid += amount;
        vector<bool> excluded(n, false);
        int excount = 0;
        if (slashposition != -1) {
            string exPart = line.substr(slashposition + 1);
            stringstream ssEx(exPart);
            string exName;
            while (ssEx >> exName) {
                for (int i = 0; i < n; i++) {
                    if (m[i].name == exName && !excluded[i]) {
                        excluded[i] = true;
                        excount++;
                    }
                }
            }
        }
        int consumercount = n - excount;
        if (consumercount > 0) {
            double share = amount / consumercount;
            for (int i = 0; i < n; i++) {
                if (!excluded[i]) m[i].consumed += share;
            }
        }
    }
    fout << fixed << setprecision(1);
    for (int i = 0; i < n; i++) {
        fout << m[i].name << " " << m[i].paid << " " << m[i].consumed << endl;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double dolg = m[i].consumed - m[i].paid;
            double komu = m[j].paid - m[j].consumed;
            if (dolg > 0.01 && komu > 0.01) {
                double pay = min(dolg, komu);
                fout << m[i].name << " " << pay << " " << m[j].name << endl;
                m[i].paid += pay;
                m[j].paid -= pay;
            }
        }
    }
    fin.close();
    fout.close();
    return 0;
}
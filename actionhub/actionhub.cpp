#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
using namespace std;
using namespace std::chrono;

struct Bid {
    string bidderId;
    double amount;       // bid price
    long long timestamp; // Unix ms
    int auctionId;
};

// ---------- Utilities ----------
void printBids(const vector<Bid>& v, size_t limit = 10) {
    for (size_t i = 0; i < min(v.size(), limit); ++i)
        cout << "[" << v[i].bidderId << "] $" << v[i].amount
             << " @ " << v[i].timestamp << "\n";
    if (v.size() > limit) cout << "... (" << v.size() << " total)\n";
}

bool loadBids(const string& path, vector<Bid>& out) {
    ifstream f(path);
    if (!f) return false;
    Bid b;
    while (f >> b.bidderId >> b.amount >> b.timestamp >> b.auctionId)
        out.push_back(b);
    return true;
}

void generateLargeData() {
    ofstream out("test_data_large.txt");
    if (!out) {
        cerr << "Chua tao thu muc test_data!\n";
        return;
    }

    mt19937_64 rng(1337);
    uniform_real_distribution<double> priceDist(1.0, 10000.0);
    uniform_int_distribution<long long> timeDist(1600000000000LL, 1700000000000LL);
    uniform_int_distribution<int> auctionDist(1, 500);

    for (int i = 0; i < 100000; ++i) {
        out << "bidder_" << (rng() % 10000) << " "
            << priceDist(rng) << " "      
            << timeDist(rng) << " "       
            << auctionDist(rng) << "\n";  
    }
    out.close();
    cout << "Da tao xong test_data/large.txt!\n";
}

// ---------- TODO: Implement these ----------

//support
bool isless(const Bid& a,const Bid& b){
    if (a.amount!=b.amount) return a.amount<b.amount;
    return a.timestamp<b.timestamp;
}

bool islessorequal(const Bid& a,const Bid& b){
    if (a.amount!=b.amount) return a.amount<b.amount;
    return a.timestamp<=b.timestamp;
}




void insertionSort(vector<Bid>& v){
    if(v.empty()) return;
    for (int i=1; i<v.size();i++){
        Bid key=v[i];
        int j=i-1;
        while(j>=0 && isless(key,v[j])){
            v[j+1]=v[j];
            j=j-1;
        }
        v[j+1]=key;
    }
}          // Task A


void selectionSort(vector<Bid>& v){
    if (v.empty()) return;
    for (int i=0; i<v.size()-1;i++) {
        int max=i;
        for (int j=i+1;j<v.size();j++){
            if (v[j].amount>v[max].amount) max=j;
        }
        if (max!=i) swap(v[i],v[max]);
    }
    cout << "Top 3 Bids:\n";
    for (size_t i = 0; i < min((size_t)3, v.size()); ++i) {
        cout << "  [" << v[i].bidderId << "] $" << v[i].amount <<"\n";
    }
}          // Task B


void interchangeSort(vector<Bid>& v){
    if (v.empty()) return;
    int scount=0;
    for(int i=0;i<(int)v.size()-1;i++){
        for(int j=i+1; j<(int)v.size(); j++){
            if (v[i].amount>v[j].amount){
                swap(v[i],v[j]);
                scount++;
            }
        }        
    }
    cout<<scount;
}        // Task C


bool bubbleSortEarlyStop(vector<Bid>& v){
    if (v.size() <= 1) return true;
    for (int i = 0; i < (int)v.size() - 1; i++){
        bool swapped = false;
        for (int j = 0; j < (int)v.size() - 1 - i; j++){
            if (v[j].amount > v[j+1].amount){
                swap(v[j], v[j+1]);
                swapped = true;
            }
        }
        if (i == 0 && !swapped) return true; 
        if (!swapped) break;                  
    }
    return false;
}    // Task D — returns true if stable (no swaps on a full pass)

void mot(vector<Bid>& v, int lo, int hi) {
    int mid = lo + (hi - lo) / 2;
    if (isless(v[mid], v[lo])) swap(v[lo], v[mid]);
    if (isless(v[hi], v[lo])) swap(v[lo], v[hi]);
    if (isless(v[hi], v[mid])) swap(v[mid], v[hi]);
    swap(v[mid], v[hi]);
}
int partition(vector<Bid>& v,int lo,int hi){
    mot(v,lo,hi);
    Bid pi=v[hi];
    int i=lo-1;
    for(int j=lo; j<hi; j++){
        if (islessorequal(v[j],pi)){
            i++;
            swap(v[i],v[j]);
        }
    }
    swap(v[i+1],v[hi]);
    return i+1;
}
void quickSort(vector<Bid>& v, int lo, int hi){
    if (lo<hi){
        int pi=partition(v,lo,hi);
        quickSort(v,lo,pi-1);
        quickSort(v,pi+1,hi);
    }
} // Task E
void quicksortwrapper(vector<Bid>& v){
    if(v.empty()) return;
    auto start = high_resolution_clock::now();
    quickSort(v,0,(int)v.size()-1);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout<<"Quick sort time:"<<duration.count()<<"ms\n";
    bool sorted = true;
    for (size_t i = 1; i < v.size(); ++i) {
        if (isless(v[i], v[i - 1])) {
            sorted = false;
            break;
        }
    }
    cout << "Verification: " << (sorted ? "PASSED (Array is sorted!)" : "FAILED") << "\n";
}


// ---------- Driver ----------
int main(int argc, char* argv[]) {
    // 1. Tạo test data nhỏ trực tiếp để kiểm tra Task A, B, C, D
    vector<Bid> sample = {
        {"alice", 100.0, 1000, 1},
        {"bob",   102.5, 1005, 1},
        {"carol", 101.0, 1003, 1},
        {"dave",  103.0, 1010, 1}
    };

    cout << "\nTASK A: Insertion Sort (Live Monitor)\n";
    vector<Bid> vA = sample;
    insertionSort(vA);
    printBids(vA);

    cout << "\nTASK B: Selection Sort (Top-3 Bidders)\n";
    vector<Bid> vB = sample;
    selectionSort(vB);

    cout << "\nTASK C: Interchange Sort (Anomaly Validator)\n";
    vector<Bid> vC = sample;
    interchangeSort(vC);

    cout << "\nTASK D: Bubble Sort Early Stop (Stabilization Detector)\n";
    vector<Bid> vD = sample;
    bool isStable = bubbleSortEarlyStop(vD);
    cout << "Stream status: " << (isStable ? "STABLE" : "UNSTABLE") << "\n";

    cout << "\nTASK E: Quick Sort on 100k Records\n";
    generateLargeData();
    vector<Bid> vLarge;
    if (loadBids("test_data_large.txt", vLarge)) {
        quicksortwrapper(vLarge);
    } else {
        cerr << "Khong doc duoc file test_data/large.txt!\n";
    }

    return 0;
}
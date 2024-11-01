//Kit Pollinger
// 210 - lab - 26 | Data Structures Olympics

//210 - Lab - 25 Code
#include <iostream>
#include <chrono>
#include <vector>
#include <list>
#include <set>
#include <fstream>
#include <string>
#include <algorithm>
#include <array>
#include <iomanip>

using namespace std;
using namespace std::chrono;

const int NUM_OPERATIONS = 4; // Read, Sort, Insert, Delte
const int NUM_CONTAINERS = 3; //Vector , List, Sort
const int NUM_TRIALS = 15; //15 Total Trials

// read data into a container (using push_back for vector)
template <typename Container>
void readData(Container& container, ifstream& inFile) {
    string data;
    while (getline(inFile, data)) {
        container.push_back(data);
    }
}

// Overload readData for list (also uses push_back for consistency)
template <>
void readData(list<string>& container, ifstream& inFile) {
    string data;
    while (getline(inFile, data)) {
        container.push_back(data);
    }
}

// Specialization for set reading (uses `insert` directly)
template <>
void readData(set<string>& container, ifstream& inFile) {
    string data;
    while (getline(inFile, data)) {
        container.insert(data);
    }
}

//reset and re-read data from the file for each container
template <typename Container>
milliseconds timeRead(Container& container, const string& filename) {
    container.clear();
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "Error opening data file!" << endl;
        return milliseconds(0);
    }
    auto start = high_resolution_clock::now();
    readData(container, inFile);
    auto end = high_resolution_clock::now();
    inFile.close();

    return duration_cast<milliseconds>(end - start);
}

// time sorting operation
template <typename Container>
milliseconds timeSort(Container& container) {
    auto start = high_resolution_clock::now();
    container.sort();
    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

// sorting vector using `std::sort`
template <>
milliseconds timeSort(vector<string>& container) {
    auto start = high_resolution_clock::now();
    sort(container.begin(), container.end());
    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

// time insertion operation (with millisecond precision and multiple insertions)
template <typename Container>
milliseconds timeInsert(Container& container) {
    auto start = high_resolution_clock::now();
    for (int i = 0; i < 100; ++i) { // Insert multiple times for higher precision
        auto it = container.begin();
        advance(it, container.size() / 2);
        container.insert(it, "TESTCODE" + to_string(i));
    }
    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

// Set insertion specialization
template <>
milliseconds timeInsert(set<string>& container) {
    auto start = high_resolution_clock::now();
    for (int i = 0; i < 100; ++i) { // Insert multiple times for higher precision
        container.insert("TESTCODE" + to_string(i));
    }
    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

// time deletion operation (with millisecond precision and multiple deletions)
template <typename Container>
milliseconds timeDelete(Container& container) {
    auto start = high_resolution_clock::now();
    for (int i = 0; i < 100; ++i) { // Delete multiple times for higher precision
        if (container.empty()) break;
        auto it = container.begin();
        advance(it, container.size() / 2);
        container.erase(it);
    }
    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

// Set deletion specialization
template <>
milliseconds timeDelete(set<string>& container) {
    auto start = high_resolution_clock::now();
    for (int i = 0; i < 100; ++i) { // Delete multiple times for higher precision
        if (container.empty()) break;
        auto it = container.begin();
        advance(it, container.size() / 2);
        container.erase(it);
    }
    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

int main() {
    string filename = "C:\\210-Coding\\projects\\210-lab-25-starter\\codes.txt";

    // 3D array to store timings: [operation][container][trial]
    array<array<array<milliseconds, NUM_TRIALS>, NUM_CONTAINERS>, NUM_OPERATIONS> timings;

    // Run the experiment 15 times
    for (int trial = 0; trial < NUM_TRIALS; ++trial) {
        vector<string> vec;
        list<string> lst;
        set<string> st;

        // Record timing for each operation and container
        timings[0][0][trial] = timeRead(vec, filename);
        timings[0][1][trial] = timeRead(lst, filename);
        timings[0][2][trial] = timeRead(st, filename);

        timings[1][0][trial] = timeSort(vec);
        timings[1][1][trial] = timeSort(lst);
        timings[1][2][trial] = milliseconds(-1); //already given to us

        timings[2][0][trial] = timeInsert(vec);
        timings[2][1][trial] = timeInsert(lst);
        timings[2][2][trial] = timeInsert(st);

        timings[3][0][trial] = timeDelete(vec);
        timings[3][1][trial] = timeDelete(lst);
        timings[3][2][trial] = timeDelete(st);
    }

    // Compute and output average timings
    cout << "Number of simulations: " << NUM_TRIALS << endl;
    cout << "Operation\t\tVector\t\tList\t\tSet" << endl;

    const char* operations[] = {"Read", "Sort", "Insert", "Delete"};
    for (int op = 0; op < NUM_OPERATIONS; ++op) {
        cout << setw(10) << left << operations[op] << "\t";
        for (int cont = 0; cont < NUM_CONTAINERS; ++cont) {
            long long total = 0;
            for (int trial = 0; trial < NUM_TRIALS; ++trial) {
                total += timings[op][cont][trial].count();
            }
            // Calculate and print the average in milliseconds
            cout << setw(11) << right << (total / NUM_TRIALS) << "\t";
        }
        cout << endl;
    }

    return 0;
}

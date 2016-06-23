/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#ifndef PEON_UTILITY_H
#define PEON_UTILITY_H

#include <mutex>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>

#include "log/Logger.h"

using std::hex;
using std::setw;
using std::mutex;
using std::string;
using std::vector;
using std::setfill;
using std::ifstream;
using std::lock_guard;
using std::stringstream;

namespace Peon {

    template<typename Tr, typename Tn, typename Tm>
    Tr Mod(Tn n, Tm m) {
        Tr r = n - (n / m) * m;
        if (r < 0) {
            return r + m;
        }
        return r;
    };


    template<typename T>
    string ToHex(T i) {
        stringstream sstream;
        sstream << "0x" << setfill('0') << setw(sizeof(T) * 2) << hex << i;
        return sstream.str();
    }

    string ReadFile(const string &file);
    string GmtTimestamp();
    vector<string> Split(const string & source, char delimeter);
}
#endif //PEON_UTILITY_H

/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include "Utility.h"

string Peon::ReadFile(const string &file) {
    stringstream sstream;
    ifstream file_stream(file);
    if(!file_stream.is_open() || !file_stream.good()) {
        return string("");
    }
    sstream << file_stream.rdbuf();
    return sstream.str();
}

string Peon::GmtTimestamp() {
    stringstream sstream;
    static mutex gmtime_mutex;
    lock_guard<mutex> gmlock(gmtime_mutex);

    time_t tnow = time(NULL);
    tm *tm = gmtime(&tnow);
    if(!tm) {
        return string();
    }
    sstream << setfill('0') << setw(2) << tm->tm_mon + 1 << "/"
            << setfill('0') << setw(2) << tm->tm_wday << "/"
            << setfill('0') << setw(4) << tm->tm_year + 1900 << " "
            << setfill('0') << setw(2) << tm->tm_hour << ":"
            << setfill('0') << setw(2) << tm->tm_min << ":"
            << setfill('0') << setw(2) << tm->tm_sec << "  ";
    return sstream.str();
}

vector<string> Peon::Split(const string & source, char delimeter) {
    string token;
    vector<string> tokens;
    stringstream s(source);
    while(getline(s, token, delimeter)) {
        tokens.push_back(token);
    }
    return tokens;
}

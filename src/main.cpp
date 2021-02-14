#include "ReversedRecorder.h"
#include <getopt.h>
#include <iostream>
using namespace std;

void printHelp();
int getNumber();
bool endsWith(const string &arg, const string &suffix);

int main(int argc, char **argv)
{
    char opt;
    string filename = "file.wav";
    int rate = 44100;
    int time = -1;
    const char* const short_options = "f:r:t:h";
    const option long_options[] = {
            {"filename", required_argument, nullptr, 'f'},
            {"rate", required_argument, nullptr, 'r'},
            {"time", required_argument, nullptr, 't'},
            {"help", no_argument, nullptr, 'h'}
    };
    do {
        opt = getopt_long(argc, argv, short_options, long_options, nullptr);
        switch (opt) {
            case 'f':
                filename = optarg;
                if (!endsWith(filename, ".wav"))
                    filename.append(".wav");
                break;
            case 'r':
                if((rate = getNumber()) == -1)
                {
                    cout << "rate must be a number" << endl;
                    return -1;
                }
                break;
            case 't':
                if((time = getNumber()) == -1)
                {
                    cout << "time must be a number" << endl;
                    return -1;
                }
                break;
            case 'h':
                printHelp();
                return 0;
        }
    }
    while (opt != -1);
    ReversedRecorder *recorder;
    if (time != -1)
        recorder = new FixedRecorder(filename, rate, time);
    else
        recorder = new InfiniteRecorder(filename, rate);
    recorder->record();
    delete recorder;
    return 0;
}

void printHelp() {
    cout << "--help (-h) -> print this message" << endl;
    cout << "--filename (-f) sets filename (must ends with .wav). Default is file.wav" << endl;
    cout << "--rate (-r) sets rate of sound samples. Default is 44100" << endl;
    cout << "--time (-t) sets time of the record. If not specified, user must send anything to console to stop." << endl;
}

int getNumber() {
    char *p;
    int val = strtol(optarg, &p, 10);
    if(*p)
        val = -1;
    return val;

}

bool endsWith(const string &arg, const string &suffix) {
    return arg.size() >= suffix.size() && arg.compare(arg.size()-suffix.size(), suffix.size(), suffix) == 0;

}

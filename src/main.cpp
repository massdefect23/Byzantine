#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>

// Global definitions

typedef std::string Path;
const char one = '1';
const char zero = '0';
const char unknown = '?';
const char faulty = 'x';

struct Node {
    Node(char input = faulty, char output = faulty)
    : input_value(input), output_value(output) {};
    char input_value;
    char output_value;
};

class Traits {
    public : 

        Traits(int source, int m, int n, bool debug = false)
            : mSource(source), mM(m), mN(n), mDebug(debug) {}

        Node GetSourceValue() {
            return Node(zero, unknown);
        };

        char GetValue(char value, int source, int destination, const Path &path) {
            if (source == mSource)
                return (destination & 1) ? zero : one;
            else if (source == 2)
                return one;
            else
                return value;
        }

        char GetDefault() {
            return one;
        }

        bool IsFaulty(int process) {
            if (process == mSource || process == 2)
                return true;
            else
                return false;
        }

        const int mSource;
        const int mM;
        const size_t mN;
        const bool mDebug;
};
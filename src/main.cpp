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

class Process {
    public:
        Process(int id) : mId(id) {
            if (mChildren.size() == 0)
                GenerateChildren(mTraits.mM, mTraits.mN, std::vector<bool>(mTraits.mN, true));
            if (mId == mTraits.mSource)
                mNodes[""] = mtraits.GetSourceValue();
        }

        void SendMessages(int round, std::vector<Process> &processes) {
            for (size_t i = 0; i < mPathsByRank[round][mId].size(); i++)
            {
                Path source_node_path = mPathsByRank[round][mId][i];
                source_node_path = source_node_path.substr(0, source_node_path.size() - 1);
                Node source_node = mNodes[source_node_path];
                for (size_t j = 0; j < mTraits.mN; j++)
                    if (j != mTraits.mSource) {
                        char value = mTraits.GetValue(source_node.input_value, 
                                                        mId, 
                                                        (int) j, 
                                                        mPathsByRank[round][mId][i]);
                        if (mTraits.mDebug)
                            std::cout << "Sending from process" << mId
                                      << " to " << static_cast<unsigned int>(j)
                                      << ": {" << value << ", "
                                      << mPathsByRank[round][mId][i]
                                      << ", " << unknown << "}"
                                      << ", getting value from source_node " << source_node_path
                                      << "\n";
                        processes[j].RecieveMessage(mPathsByRank[round][mId][i], Node(value, unknown));
                    }
            }
        }

        char Decide()
        {
            if (mId == mTraits.mSource)
                return mNodes[""].input_value;

            for (size_t = 0; i < mTraits.mN; i++)
                for (size_t j = 0; j < mPathsByRank[mTraits.mM][i].size(); j++)
                {
                    const Path &path = mPathsByRank[mTraits.mM][i][j];
                    Node &node = mNodes[path];
                    node.output_value = node.input_value;
                }

            for (int round = (int) mtraits.mM - 1; round >= 0; round--)
            {
                for (size_t i = 0; i < mTraits.mN; i++)
                    for (size_t j = 0; j < mPathsByRank[round][i].size(); j++)
                    {
                        const Path &path = mPathsByRank[round][i][j];
                        Node &node = mNodes[path];
                        node.output_value = GetMajority(path);
                    }
            }
            const Path &top_path = mPathsByRank[0][mTraits.mSource].front();
            const Node &top_node = mNodes[top_path];
            return top_node.output_value;
        }

        std::string Dump(Path path = "")
        {
            if (path == "")
                path = mPathsByRank[0][mTraits.mSource].front();
            std::stringstream s;
            for (size_t i = 0; i < mChildren[path].size(); i++)
                s << Dump(mChildren[path][i]);
            Node &node = mNodes[path];
            s << "{" << node.input_value << "," << path << "," << node.output_value 
            << "}\n";
            return s.str();
        }

        std::string DumpDot(Path path = "")
        {
            bool root = false;
            std::stringstream s;
            if (path == "") {
                root = true;
                path = mPathsByRank[0][mTraints.mSource].front();
                s << "digraph byz {\n" << "rankdir=LR;\n" << "nodesep=.0025;\n"
                  << "label=\"Process" << mId << "\";\n"
                  << "node [fontsize=8, width=0.005, height=.005, shape=plaintext];\n"
                  << "edge [fontsize=8, arrowsize=0.25];\n";
            }
            Node &node = mNodes[path];
            for (size_t i = 0; i < mChildren[path].size(); i++)
                s << DumpDot(mChildren[path][i]);
            if (path.size() == 1)
                s << "general->";
            else {
                Path parent_path = path.substr(0, path.size() - 1);
                Node &parent_node = mNodes[parent_path];
                s << "\"{" << parent_node.input_value << "," << parent_path
                  << "," << parent_node.output_value
                  << "}\"->";
            }
            s << "\"{" << node.input_value << "," << path << "," << node.output_value
              << "}\";\n";
            if (root)
                s << "};\n";
            return s.str();
        }
};

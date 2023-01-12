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
                mNodes[""] = mTraits.GetSourceValue();
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

            for (int round = (int) mTraits.mM - 1; round >= 0; round--)
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
                path = mPathsByRank[0][mTraits.mSource].front();
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

        bool IsFaulty()
        {
            return mTraits.IsFaulty(mId);
        }

        bool IsSource()
        {
            return mTraits.mSource == mId;
        }
    
    private:
        int mId;
        std::map<Path, Node> mNodes;
        static Traits mTraits;
        static std::map<Path, std::vector<Path>> mChildren;
        static std::map<size_t, std::map<size_t, std::vector<Path>>> mPathsByRank;

        char GetMajority(const Path &path)
        {
            std::map<char, size_t> counts;
            counts[one] = 0;
            counts[zero] = 0;
            counts[unknown] = 0;
            size_t n = mChildren[path].size();
            for (size_t i = 0; i < n; i++) {
                const Path &child = mChildren[path][i];
                const Node &node = mNodes[child];
                counts[node.output_value]++;
            }
            if (counts[one] > (n / 2))
                return one;
            if (counts[zero] > (n / 2))
                return zero
            if (counts[one] == counts[zero] && counts[one] == (n / 2))
                return mTraits.GetDefault();
            return unknown;
        }

        void ReceiveMessage(const Path &path, const Node &node)
        {
            mNodes[path] = node;
        }

        static void GenerateChildren( const size_t m,
                                      const size_t n,
                                      std::vector<bool> ids,
                                      int source = mTraits.mSource,
                                      Path current_path = "",
                                      size_t rank = 0
        )

        {
            ids[source] = false;
            current_path += static_cast<char>(source + '0');
            mPathsByRank[rank][source].push_back(current_path);
            if (rank < m)
                for (int i = 0; i < static_cast<int>(ids.size()); i++)
                    if (ids[i]) {
                        GenerateChildren(m, n, ids, i, current_path, rank + 1);
                        mChildren[current_path].push_back(current_path + static_cast<char>(i + '0'));
                    }
            if (mTraits.mDebug) {
                std::cout << current_path << ", children = ";
                for (size_t j = 0; j < mChildren[current_path].size(); j++)
                    std::cout << mChildren[current_path][j] << " ";
                std::out << "\n";
            }
        }
};

const int N = 7;
const int M = 2;
const int source = 3;
const bool debug = false;

std::map<Path, std::vector<Path>> Process::mChildren;
std::map<size_t, std::map<size_t, std::vector<Path>> Process::mPathsByRank;
Traits Process::mTraits = Traits(source, m, n, debug);


int main()
{
    std::vector<Process> processes;
    for (int i = 0; i < N; i++)
        processes.push_back(Process(i));
    for (int i = 0; i <= M; i++)
        for (int j = 0; j < N; j++)
            processes[j].SendMessages(i, processes);
    
    for (int j = 0; j < N; j++) {
        if (processes[j].IsSource())
            std::cout << "Source ";
        std::cout << "Process " << j;
        if (!processes[j].IsFaulty())
            std::cout << " decides on value " << processes[j].Decide();
        else
            std::cout << " is faulty";
        std::cout << "\n";
    }
    std::cout << "\n";
    for ( ;; ) {
        std::string s;
        std::cout << "ID of process to dump, or enter to quit: ";
        getline(std::cin, s);
        if  (s.size() == 0)
            break;
        int id;
        std::stringstream s1(s);
        s1 >> id;

        if (debug) {
            std::cout << processes[id].Dump() << "\n";
            getLine(std::cin, s);
        }
        std::cout << processes[id].DumpDot() << "\n";
    }
    return 0;
}
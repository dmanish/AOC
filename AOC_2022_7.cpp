#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;

class filesystem {
private:
    map<string, long long> dirsizes;
    vector<string> pathstack;
    
    string getcurrentpath() {
        if (pathstack.empty()) return "/";
        string path = "";
        for (const string& dir : pathstack) {
            path += "/" + dir;
        }
        return path.empty() ? "/" : path;
    }
    
    void addsize(long long size) {
        // Add size to current directory and all parent directories
        vector<string> tempstack = pathstack;
        
        // Add to root
        dirsizes["/"] += size;
        
        // Add to each subdirectory in the path
        string path = "";
        for (const string& dir : pathstack) {
            path += "/" + dir;
            dirsizes[path] += size;
        }
    }
    
public:
    filesystem(void) {
        dirsizes["/"] = 0;
    }
    
    void processterminalcommand(const string& line) {
        if (line.substr(0, 4) == "$ cd") {
            string dir = line.substr(5);
            if (dir == "/") {
                pathstack.clear();
            } else if (dir == "..") {
                if (!pathstack.empty()) {
                    pathstack.pop_back();
                }
            } else {
                pathstack.push_back(dir);
                string path = getcurrentpath();
                if (dirsizes.find(path) == dirsizes.end()) {
                    dirsizes[path] = 0;
                }
            }
        } else if (line.substr(0, 4) == "$ ls") {
            // Just listing, no action needed
        } else if (line.substr(0, 3) == "dir") {
            // Directory listing, no action needed
        } else {
            // File with size
            istringstream iss(line);
            long long size;
            string filename;
            if (iss >> size >> filename) {
                addsize(size);
            }
        }
    }
    
    long long part1() {
        long long total = 0;
        for (const auto& [path, size] : dirsizes) {
            if (size <= 100000) {
                total += size;
            }
        }
        return total;
    }

    
    void printDirectories() {
        cout << "\nDirectory sizes:" << endl;
        for (const auto& [path, size] : dirsizes) {
            cout << path << ": " << size << endl;
        }
    }


};

int main() {
    filesystem fs;
    string line;

    cout << "Enter terminal output (Ctrl+D or Ctrl+Z to end input):" << endl;

    while (getline(cin, line)) {
        if (!line.empty()) {
            fs.processterminalcommand(line);
        }
    }

    fs.printDirectories();

    cout << "\n=== Results ===" << endl;
    cout << "Part 1 (sum of directories <= 100000): " << fs.part1() << endl;

    return 0;
}
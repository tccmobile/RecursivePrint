#include <iostream>
#include <filesystem>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

namespace fs = filesystem;

class DirectoryScanner {
private:
    struct FileStats {
        size_t totalFiles = 0;
        size_t totalDirs = 0;
        uintmax_t totalSize = 0;
    };

    // Helper function to format file sizes
    static string formatFileSize(uintmax_t bytes) {
        const char* units[] = {"B", "KB", "MB", "GB", "TB"};
        int unitIndex = 0;
        double size = static_cast<double>(bytes);
        
        while (size >= 1024 && unitIndex < 4) {
            size /= 1024;
            unitIndex++;
        }
        
        stringstream ss;
        ss << fixed << setprecision(2) << size << " " << units[unitIndex];
        return ss.str();
    }

    // Recursive function to print directory tree
    static void printDirectoryTree(const fs::path& path, string prefix = "", bool isLast = true) {
        // Print current directory/file
        cout << prefix << (isLast ? "└── " : "├── ") << path.filename().string() << "\n";
        
        if (fs::is_directory(path)) {
            // Collect and sort directory entries
            vector<fs::path> paths;
            for (const auto& entry : fs::directory_iterator(path)) {
                paths.push_back(entry.path());
            }
            sort(paths.begin(), paths.end());
            
            // Process each entry
            for (size_t i = 0; i < paths.size(); ++i) {
                printDirectoryTree(paths[i], 
                                 prefix + (isLast ? "    " : "│   "),
                                 i == paths.size() - 1);
            }
        }
    }

    // Recursive function to calculate directory statistics
    static FileStats scanDirectory(const fs::path& path) {
        FileStats stats;
        
        try {
            if (!fs::exists(path)) {
                return stats;
            }

            if (fs::is_regular_file(path)) {
                stats.totalFiles = 1;
                stats.totalSize = fs::file_size(path);
                return stats;
            }

            if (fs::is_directory(path)) {
                stats.totalDirs = 1;
                
                for (const auto& entry : fs::directory_iterator(path)) {
                    FileStats subStats = scanDirectory(entry.path());
                    stats.totalFiles += subStats.totalFiles;
                    stats.totalDirs += subStats.totalDirs;
                    stats.totalSize += subStats.totalSize;
                }
            }
        } catch (const fs::filesystem_error& e) {
            cerr << "Error accessing " << path << ": " << e.what() << endl;
        }
        
        return stats;
    }

public:
    static void analyze(const string& path) {
        fs::path fsPath = fs::absolute(path);
        
        if (!fs::exists(fsPath)) {
            cout << "Path does not exist: " << fsPath << endl;
            return;
        }

        cout << "\nDirectory Tree for: " << fsPath << "\n\n";
        printDirectoryTree(fsPath);
        
        cout << "\nDirectory Statistics:\n";
        FileStats stats = scanDirectory(fsPath);
        cout << "Total Directories: " << stats.totalDirs << "\n";
        cout << "Total Files: " << stats.totalFiles << "\n";
        cout << "Total Size: " << formatFileSize(stats.totalSize) << "\n\n";
    }
};

int main(int argc, char* argv[]) {
    string path = argc > 1 ? argv[1] : ".";
    
    try {
        DirectoryScanner::analyze(path);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}

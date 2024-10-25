# RecursivePrint

This example demonstrates recursion in several practical ways:

1. `printDirectoryTree`: Recursively traverses directories to create a visual tree structure:
```
Directory Tree for: /example
├── docs
│   ├── manual.pdf
│   └── readme.txt
└── src
    ├── main.cpp
    └── utils
        └── helper.h
```

2. `scanDirectory`: Recursively calculates:
   - Total number of files
   - Total number of directories
   - Total size of all files

Key features:
- Uses modern C++ filesystem library (C++17)
- Handles errors gracefully
- Formats file sizes human-readably (B, KB, MB, GB, TB)
- Sorts directory entries alphabetically
- Creates a visually appealing tree structure
- Uses proper memory management and error handling

To compile (requires C++17 or later):
```bash
g++ -std=c++17 directory_scanner.cpp -o directory_scanner
```

To run:
```bash
./directory_scanner [path]  # path is optional, defaults to current directory
```

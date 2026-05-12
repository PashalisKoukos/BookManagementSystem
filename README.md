
# Book Management System (C Programming)

A robust, terminal-based library management system developed as part of the **Programming II** course at Harokopio University of Athens. This project implements a **Doubly Linked List** to manage a digital library efficiently.

## 🚀 Features
- **Full CRUD Operations:** Add, delete, search, and modify book entries.
- **Advanced Sorting:** Implements the **Merge Sort** algorithm, optimized for doubly linked lists, ensuring $O(n \log n)$ performance.
- **Persistent Storage:** Saves and loads library data using binary files (`.dat`) for data integrity.
- **Cross-Platform Compatibility:** Custom screen clearing macros for both Windows and Linux environments.
- **Robust Input Validation:** Prevents buffer overflows and ensures valid user interaction.

## 🛠️ Technical Overview
- **Data Structure:** Doubly Linked List (`struct node_book`) for bi-directional traversal.
- **Memory Management:** Dynamic memory allocation and deallocation to prevent leaks.
- **File I/O:** Binary file handling for efficient data serialization.
- **Design:** Modular architecture with separated headers (`.h`) and implementation (`.c`) files.

## 💻 How to Run
1. Ensure you have a C compiler (GCC/Clang) installed.
2. Compile the project:
   ```bash
   gcc main.c books.c -o library_system

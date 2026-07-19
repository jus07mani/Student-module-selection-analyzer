
 Student Module Selection Analyzer

A C++ console application that cross-references which university modules students have selected against the master module/instructor list — built from custom data structures (a hand-rolled generic doubly linked list) rather than STL containers, ported from a C# WinForms original.

## What it does

Given two input files — a list of modules students have selected, and the master list of modules with their instructors — the program:

1. **Task 1** — Builds a unique list of instructors from the module list, then sorts it by surname/name.
2. **Task 2** — Counts how many students selected each module, then removes instructors from the list whose modules nobody selected.
3. **Task 3** — Finds the instructor teaching the most modules (the "most popular" instructor).
4. **Task 4** — Lists every module taught by that instructor.

Results can be saved to a text file at any point.

## Project structure

```
├── main.cpp                       # Console menu, program workflow, statistics tracking
├── Module.h                       # Data class: module name, instructor, credits
├── Instructor.h                   # Data class: instructor surname/name
├── SelectedModule.h                # Data class: one student's module selection
├── Statistic.h                    # Data class: named counter (menu-action usage stats)
├── GenericDoublyLinkedList.h      # Custom generic container (own iterators, Sort, Remove, Contains)
├── IOUtils.h / IOUtils.cpp        # File reading (parses the ';'-delimited input files)
├── TaskUtils.h / TaskUtils.cpp    # The four analysis tasks described above
├── Modules.txt                    # Sample master module/instructor list
├── Students.txt                   # Sample selected-modules-by-student list
└── README.md
```

## Design notes

- **GenericDoublyLinkedList<T>** is a from-scratch doubly linked list (no `std::list`), matching the original's node-based design. It requires `T` to support `operator==` (for `Contains`/`Remove`) and `operator<` (for `Sort`) — every data class defines both, and `std::string` already provides them natively (used for the Task 4 module-name list).
- It supports both **forward iteration** (`for (auto& x : list)`) and a **reverse view** (`for (auto& x : list.Reverse())`), the latter used by Task 3 exactly as in the original.
- The container is **move-only**: copying is disabled (to avoid double-freeing the raw node pointers), but moving works, so functions like `IOUtils::ReadModules(...)` can still return a list by value and have it assigned directly — e.g. `Modules = IOUtils::ReadModules(path);`
- Each data class (`Module`, `Instructor`, `SelectedModule`, `Statistic`) carries over the original `CompareTo`/`Equals` logic verbatim — including a subtle tie-breaking quirk in the original comparer (ties on surname are broken by the *reverse* order of the first name) — wrapped as `operator<` / `operator==` so they work seamlessly with the container and any STL algorithm.
- The original WinForms UI (menus, RichTextBox, About/Statistics dialog, PDF viewer) doesn't have a direct console equivalent, so it's replaced with a **numbered text menu**. Every action mirrors a menu click from the original `Main.cs`, and the on-screen output doubles as an in-memory log (like the original RichTextBox) so it can be saved to a file with option 7.
- Statistics tracking (open counts, task-execution counts) is preserved and viewable via option 8, replacing the original "About" dialog's data grid.

## Input file format

Both files are `;`-delimited text, one record per line.

**Modules.txt** — `ModuleName;InstructorSurname;InstructorName;Credits`
```
Module1;CInstructorS1;CInstructorN1;6
Module2;BInstructorS2;BInstructorN2;9
```

**Students.txt** — `StudentID;ModuleName;StudentSurname;StudentName;Group`
```
1;Module1;StudS1;StudN1;G1
2;Module1;StudS2;StudN2;G1
```

Exact duplicate rows (same key fields) are silently skipped on read, matching the original's de-duplication logic.

## Build & run

Requires a C++17-compatible compiler.

```bash
g++ -std=c++17 main.cpp IOUtils.cpp TaskUtils.cpp -o modulink
./modulink
```

Then follow the on-screen menu:

```
1. Open Students file (selected modules)
2. Open Modules file
3. Task1: List of instructors
4. Task2: Remove instructors whose modules were not selected
5. Task3: Find most popular instructor
6. Task4: List modules of most popular instructor
7. Save results to file
8. Show statistics (About)
9. Exit
```

Tasks 3–6 stay disabled (with a message) until both files have been loaded, just like the original app's `ToggleControls`.

#pragma once
#include <string>
#include <sstream>
#include <iomanip>

namespace P5
{
    // Data class for storing a single module.
    // Provides CompareTo/Equals (translated 1:1 from the C# IComparable/IEquatable
    // implementation) plus operator< / operator== so it plugs directly into
    // GenericDoublyLinkedList<T>, std::sort, etc.
    class Module
    {
    public:
        std::string NameOfModule;
        std::string InstructorSurname;
        std::string InstructorName;
        int Credits = 0;
        // Derived property (inner mechanic): how many students selected a module
        int NumOfStudentsSelected = 0;

        Module() = default;

        Module(std::string moduleName, std::string instructorSurname,
               std::string instructorName, int credits)
            : NameOfModule(std::move(moduleName)),
              InstructorSurname(std::move(instructorSurname)),
              InstructorName(std::move(instructorName)),
              Credits(credits)
        {
        }

        // Comparer: by surname and name of instructor.
        // Returns 1 if surname is greater than surname of other, or if both
        // surnames are equal and name is less than name of other.
        // Returns -1 if surname is less than surname of other, or if both
        // surnames are equal and name is greater than name of other.
        // Returns 0 otherwise.
        int CompareTo(const Module& other) const
        {
            int ip1 = InstructorSurname.compare(other.InstructorSurname);
            int ip2 = InstructorName.compare(other.InstructorName);
            if (ip1 > 0 || (ip1 == 0 && ip2 < 0))
                return 1;
            else if (ip1 < 0 || (ip1 == 0 && ip2 > 0))
                return -1;
            return 0;
        }

        // Equality check: instructor surname, instructor name and module name
        // must all match.
        bool Equals(const Module& mod) const
        {
            return (mod.InstructorSurname == InstructorSurname) &&
                   (mod.InstructorName == InstructorName &&
                    (mod.NameOfModule == NameOfModule));
        }

        static std::string Header()
        {
            std::string line =
                "\n------------------------------------------------------------------\n";
            line += "| Module | Instructor surname | Instructor name | Credits |\n";
            line += "------------------------------------------------------------------\n";
            return line;
        }

        static std::string Footer()
        {
            return "------------------------------------------------------------------\n";
        }

        std::string ToString() const
        {
            std::ostringstream oss;
            oss << "| " << std::left << std::setw(12) << NameOfModule
                << " | " << std::setw(15) << InstructorSurname
                << " | " << std::setw(15) << InstructorName
                << " | " << std::right << std::setw(3) << Credits << " |";
            return oss.str();
        }
    };

    inline bool operator==(const Module& a, const Module& b) { return a.Equals(b); }
    inline bool operator<(const Module& a, const Module& b) { return a.CompareTo(b) < 0; }
}

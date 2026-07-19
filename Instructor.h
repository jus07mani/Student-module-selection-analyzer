#pragma once
#include <string>
#include <sstream>
#include <iomanip>

namespace P5
{
    // Data class for storing a single instructor.
    class Instructor
    {
    public:
        std::string InstructorSurname;
        std::string InstructorName;

        Instructor() = default;

        Instructor(std::string instructorSurname, std::string instructorName)
            : InstructorSurname(std::move(instructorSurname)),
              InstructorName(std::move(instructorName))
        {
        }

        // Comparer: by surname and name of instructor (same quirk as Module::CompareTo:
        // ties are broken by the *reverse* order of the first name).
        int CompareTo(const Instructor& other) const
        {
            int ip1 = InstructorSurname.compare(other.InstructorSurname);
            int ip2 = InstructorName.compare(other.InstructorName);
            if (ip1 > 0 || (ip1 == 0 && ip2 < 0))
                return 1;
            else if (ip1 < 0 || (ip1 == 0 && ip2 > 0))
                return -1;
            return 0;
        }

        bool Equals(const Instructor& mod) const
        {
            return (mod.InstructorSurname == InstructorSurname) &&
                   (mod.InstructorName == InstructorName);
        }

        static std::string Header()
        {
            std::string line = "\n-----------------------------------------\n";
            line += "| Instructor surname | Instructor name |\n";
            line += "-----------------------------------------\n";
            return line;
        }

        static std::string Footer()
        {
            return "-----------------------------------------\n";
        }

        std::string ToString() const
        {
            std::ostringstream oss;
            oss << "| " << std::left << std::setw(15) << InstructorSurname
                << " | " << std::setw(15) << InstructorName << " |";
            return oss.str();
        }
    };

    inline bool operator==(const Instructor& a, const Instructor& b) { return a.Equals(b); }
    inline bool operator<(const Instructor& a, const Instructor& b) { return a.CompareTo(b) < 0; }
}

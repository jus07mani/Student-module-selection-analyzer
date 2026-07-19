#pragma once
#include <string>
#include <sstream>
#include <iomanip>

namespace P5
{
    // Data class for storing a single entry of a module selected by a student.
    class SelectedModule
    {
    public:
        int ID = 0;
        std::string NameOfModule;
        std::string StudentName;
        std::string StudentSurname;
        std::string Group;

        SelectedModule() = default;

        SelectedModule(int id, std::string nameOfModule, std::string studentSurname,
                       std::string studentName, std::string group)
            : ID(id),
              NameOfModule(std::move(nameOfModule)),
              StudentName(std::move(studentName)),
              StudentSurname(std::move(studentSurname)),
              Group(std::move(group))
        {
        }

        static std::string Header()
        {
            std::string line =
                "\n--------------------------------------------------------------------\n";
            line += "| ID | Module | Student surname | Student name | Group |\n";
            line += "--------------------------------------------------------------------\n";
            return line;
        }

        static std::string Footer()
        {
            return "--------------------------------------------------------------------\n";
        }

        std::string ToString() const
        {
            std::ostringstream oss;
            oss << "| " << std::right << std::setw(2) << ID
                << " | " << std::left << std::setw(12) << NameOfModule
                << " | " << std::right << std::setw(10) << StudentSurname
                << " | " << std::setw(10) << StudentName
                << " | " << std::setw(5) << Group << " |";
            return oss.str();
        }

        // Comparer: by surname and name of student (same tie-break quirk as Module/Instructor).
        int CompareTo(const SelectedModule& other) const
        {
            int ip1 = StudentSurname.compare(other.StudentSurname);
            int ip2 = StudentName.compare(other.StudentName);
            if (ip1 > 0 || (ip1 == 0 && ip2 < 0))
                return 1;
            else if (ip1 < 0 || (ip1 == 0 && ip2 > 0))
                return -1;
            return 0;
        }

        bool Equals(const SelectedModule& mod) const
        {
            return (mod.ID == ID) &&
                   (mod.NameOfModule == NameOfModule) &&
                   (mod.StudentName == StudentName &&
                    (mod.StudentSurname == StudentSurname &&
                     (mod.Group == Group)));
        }
    };

    inline bool operator==(const SelectedModule& a, const SelectedModule& b) { return a.Equals(b); }
    inline bool operator<(const SelectedModule& a, const SelectedModule& b) { return a.CompareTo(b) < 0; }
}

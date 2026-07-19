#include "TaskUtils.h"
#include <algorithm>
#include <vector>

namespace
{
    // Trims leading/trailing whitespace (equivalent to C#'s string.Trim()).
    std::string Trim(const std::string& s)
    {
        size_t start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos)
            return "";
        size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }
}

namespace P5
{
    namespace TaskUtils
    {
        GenericDoublyLinkedList<Instructor>
        CreateUniqueInstructorList(GenericDoublyLinkedList<Module>& Modules)
        {
            GenericDoublyLinkedList<Instructor> Instructors;
            // Usage of implemented enumerator for navigation
            for (Module& m : Modules)
            {
                Instructor instructor(m.InstructorSurname, m.InstructorName);
                // Avoid duplicates
                if (!Instructors.Contains(instructor))
                    Instructors.AddToEnd(instructor);
            }
            return Instructors;
        }

        void UpdateStudentSelected(GenericDoublyLinkedList<SelectedModule>& S,
                                    GenericDoublyLinkedList<Module>& M)
        {
            for (Module& m : M)
            {
                std::string nameOfModule = m.NameOfModule;
                int howMany = 0;
                for (SelectedModule& sm : S)
                {
                    // Trim whitespace (from both sides)
                    if (Trim(sm.NameOfModule) == Trim(nameOfModule))
                        howMany++;
                }
                m.NumOfStudentsSelected = howMany;
            }
        }

        void RemoveNotSelected(GenericDoublyLinkedList<Instructor>& Instructors,
                                GenericDoublyLinkedList<Module>& Modules)
        {
            // Snapshot first: removing from Instructors while iterating it
            // directly would invalidate the range-for, just like in the
            // original C# version's foreach-with-Remove pattern.
            std::vector<Instructor> toCheck;
            for (Instructor& instructor : Instructors)
                toCheck.push_back(instructor);

            for (Instructor& instructor : toCheck)
            {
                int howMany = 0;
                for (Module& m : Modules)
                {
                    // Trim whitespace (from both sides)
                    if (Trim(m.InstructorName) == Trim(instructor.InstructorName) &&
                        Trim(m.InstructorSurname) == Trim(instructor.InstructorSurname))
                    {
                        howMany = m.NumOfStudentsSelected;
                        break;
                    }
                }
                // Remove not selected
                if (howMany == 0)
                    Instructors.Remove(instructor);
            }
        }

        Instructor InstructorWithMostModules(GenericDoublyLinkedList<Module>& Modules)
        {
            Instructor instructor;
            int moduleCount = 0;
            // Usage of custom enumerator (reverse)
            for (Module& m : Modules.Reverse())
            {
                Instructor mI(m.InstructorSurname, m.InstructorName);
                int mc = 0;
                // Usage of custom enumerator (reverse)
                for (Module& mv : Modules.Reverse())
                {
                    Instructor mvI(mv.InstructorSurname, mv.InstructorName);
                    if (mvI.Equals(mI))
                        mc++;
                }
                // Search for last most popular instructor (if multiple exist)
                if (mc > moduleCount)
                {
                    moduleCount = mc;
                    instructor = mI;
                }
            }
            return instructor;
        }

        GenericDoublyLinkedList<std::string>
        ModulesOfInstructor(GenericDoublyLinkedList<Module>& Modules, const Instructor& I, bool& found)
        {
            GenericDoublyLinkedList<std::string> result;
            found = false;
            for (Module& m : Modules)
            {
                std::string module = m.NameOfModule;
                Instructor mI(m.InstructorSurname, m.InstructorName);
                // Select specified instructor
                if (mI.Equals(I))
                {
                    // Avoid duplicates
                    if (!result.Contains(module))
                        result.AddToEnd(module);
                    found = true;
                }
            }
            return result;
        }
    }
}

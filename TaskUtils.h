#pragma once
#include <string>
#include "GenericDoublyLinkedList.h"
#include "Module.h"
#include "Instructor.h"
#include "SelectedModule.h"

namespace P5
{
    // Static-style utility namespace for handling calculations (4 tasks).
    namespace TaskUtils
    {
        // Task1: creates a doubly linked list of instructors (without repetitions).
        GenericDoublyLinkedList<Instructor>
        CreateUniqueInstructorList(GenericDoublyLinkedList<Module>& Modules);

        // Task2 (inner mechanic): updates the number of students by module selection.
        void UpdateStudentSelected(GenericDoublyLinkedList<SelectedModule>& S,
                                    GenericDoublyLinkedList<Module>& M);

        // Task2: removes instructors whose modules were not selected.
        void RemoveNotSelected(GenericDoublyLinkedList<Instructor>& Instructors,
                                GenericDoublyLinkedList<Module>& Modules);

        // Task3: finds the most popular instructor (the last one, in case of ties).
        Instructor InstructorWithMostModules(GenericDoublyLinkedList<Module>& Modules);

        // Task4: builds a list of module names taught by the specified instructor.
        GenericDoublyLinkedList<std::string>
        ModulesOfInstructor(GenericDoublyLinkedList<Module>& Modules, const Instructor& I, bool& found);
    }
}

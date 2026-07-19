#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <limits>

#include "GenericDoublyLinkedList.h"
#include "Module.h"
#include "Instructor.h"
#include "SelectedModule.h"
#include "Statistic.h"
#include "IOUtils.h"
#include "TaskUtils.h"

using namespace P5;
using namespace std;

// ---------------------------------------------------------------------
// Console replacement for the original app's RichTextBox + status bar:
// everything shown on screen is also buffered so it can be saved to a
// results file, mirroring saveToolStripMenuItem_Click in Main.cs.
// ---------------------------------------------------------------------
namespace
{
    vector<string> Log;

    void AppendLine(const string& text = "")
    {
        cout << text << "\n";
        Log.push_back(text);
    }

    void SetStatus(const string& message)
    {
        cout << "[status] " << message << "\n";
    }

    // Small overload set so Display<T> can call .ToString() uniformly,
    // including for plain std::string entries (Task4's module name list).
    string ElementToString(const string& element) { return element; }

    template <typename T>
    string ElementToString(const T& element) { return element.ToString(); }

    // Generic display routine (equivalent to Main's Display<T> helper).
    template <typename T>
    void Display(const string& label, const string& tableHeader,
                 GenericDoublyLinkedList<T>& data, const string& tableFooter,
                 bool formatFlag)
    {
        AppendLine();
        AppendLine(label);
        AppendLine(tableHeader);

        for (T& element : data)
        {
            if (formatFlag)
                AppendLine("| " + ElementToString(element) + " |");
            else
                AppendLine(ElementToString(element));
        }

        AppendLine(tableFooter);
    }

    string PromptLine(const string& prompt)
    {
        cout << prompt;
        string s;
        getline(cin, s);
        return s;
    }
}

int main()
{
    // Initial data (equivalent fields to the Main form's private members)
    GenericDoublyLinkedList<SelectedModule> SelectedModules;
    GenericDoublyLinkedList<Module> Modules;
    GenericDoublyLinkedList<Instructor> Instructors;
    Instructor MostPopular;
    GenericDoublyLinkedList<string> ModulesForInstructor;
    bool foundMostPopular = false;

    array<Statistic, 7> Statistics = { {
        Statistic("Open Students", 0),
        Statistic("Open Modules", 0),
        Statistic("Create log file", 0),
        Statistic("Task1 execution", 0),
        Statistic("Task2 execution", 0),
        Statistic("Task3 execution", 0),
        Statistic("Task4 execution", 0),
    } };

    SetStatus("Select files to read data");

    bool running = true;
    while (running)
    {
        bool tasksEnabled = SelectedModules.Count() != 0 && Modules.Count() != 0;

        cout << "\n==================== Student Modules ====================\n";
        cout << "1. Open Students file (selected modules)\n";
        cout << "2. Open Modules file\n";
        cout << "3. Task1: List of instructors" << (tasksEnabled ? "" : "  [disabled: load both files first]") << "\n";
        cout << "4. Task2: Remove instructors whose modules were not selected" << (tasksEnabled ? "" : "  [disabled]") << "\n";
        cout << "5. Task3: Find most popular instructor" << (tasksEnabled ? "" : "  [disabled]") << "\n";
        cout << "6. Task4: List modules of most popular instructor" << (tasksEnabled ? "" : "  [disabled]") << "\n";
        cout << "7. Save results to file" << (tasksEnabled ? "" : "  [disabled]") << "\n";
        cout << "8. Show statistics (About)\n";
        cout << "9. Exit\n";
        string choiceStr = PromptLine("Choose an option: ");

        int choice;
        try { choice = stoi(choiceStr); }
        catch (...) { cout << "Please enter a number.\n"; continue; }

        switch (choice)
        {
        case 1: // studentsToolStripMenuItem_Click
        {
            Statistics[0].Increase();
            string path = PromptLine("Path to selected-modules file (e.g. Students.txt): ");
            try
            {
                SelectedModules = IOUtils::ReadSelectedModules(path);
                Display("Selected modules by students", SelectedModule::Header(),
                         SelectedModules, SelectedModule::Footer(), false);
                SetStatus("Successfully read: " + path);
            }
            catch (const exception&)
            {
                SelectedModules = GenericDoublyLinkedList<SelectedModule>();
                SetStatus("Error reading file: " + path + ". Try again.");
            }
            break;
        }
        case 2: // modulesToolStripMenuItem_Click
        {
            Statistics[1].Increase();
            string path = PromptLine("Path to modules file (e.g. Modules.txt): ");
            try
            {
                Modules = IOUtils::ReadModules(path);
                Display("Modules", Module::Header(), Modules, Module::Footer(), false);
                SetStatus("Successfully read: " + path);
            }
            catch (const exception&)
            {
                Modules = GenericDoublyLinkedList<Module>();
                SetStatus("Error reading file: " + path + ". Try again.");
            }
            break;
        }
        case 3: // task1ListOfInstructorsToolStripMenuItem_Click
        {
            if (!tasksEnabled) { SetStatus("Load both files first."); break; }
            Statistics[3].Increase();
            Instructors = TaskUtils::CreateUniqueInstructorList(Modules);
            Display("Instructors (unique)", Instructor::Header(), Instructors, Instructor::Footer(), false);
            Instructors.Sort();
            Display("Instructors (unique), sorted by name and surname", Instructor::Header(),
                     Instructors, Instructor::Footer(), false);
            SetStatus("List of instructors have been successfully created.");
            break;
        }
        case 4: // task2RemoveToolStripMenuItem_Click
        {
            if (!tasksEnabled) { SetStatus("Load both files first."); break; }
            Statistics[4].Increase();
            TaskUtils::UpdateStudentSelected(SelectedModules, Modules);
            if (Instructors.Count() != 0)
            {
                TaskUtils::RemoveNotSelected(Instructors, Modules);
                if (Instructors.Count() != 0)
                {
                    Display("Instructors (after removal)", Instructor::Header(),
                             Instructors, Instructor::Footer(), false);
                    SetStatus("Removed instructors whose modules were not selected.");
                }
                else
                {
                    AppendLine("List of instructors is empty (after removal).");
                    SetStatus("List of instructors is empty (after removal).");
                }
            }
            else
            {
                SetStatus("Remove operation cannot be executed.");
            }
            break;
        }
        case 5: // task3MostPopularToolStripMenuItem_Click
        {
            if (!tasksEnabled) { SetStatus("Load both files first."); break; }
            Statistics[5].Increase();
            MostPopular = TaskUtils::InstructorWithMostModules(Modules);
            AppendLine();
            AppendLine("Most popular instructor (surname, name): " +
                        MostPopular.InstructorSurname + " " + MostPopular.InstructorName + " ");
            SetStatus("Most popular instructor successfully found.");
            break;
        }
        case 6: // task4ListOfModulesToolStripMenuItem_Click
        {
            if (!tasksEnabled) { SetStatus("Load both files first."); break; }
            Statistics[6].Increase();
            ModulesForInstructor = TaskUtils::ModulesOfInstructor(Modules, MostPopular, foundMostPopular);
            if (foundMostPopular)
            {
                Display("Modules of most popular instructor",
                        "\n--------------\n| Module |\n--------------\n",
                        ModulesForInstructor, "--------------\n", true);
                SetStatus("Modules of most popular instructor found.");
            }
            else
            {
                AppendLine("Please find most popular instructor first.");
                SetStatus("Please find most popular instructor first.");
            }
            break;
        }
        case 7: // saveToolStripMenuItem_Click
        {
            Statistics[2].Increase();
            string path = PromptLine("Save results to file (path): ");
            ofstream writer(path, ios::binary);
            if (!writer.is_open())
            {
                SetStatus("Could not open file for writing: " + path);
                break;
            }
            for (const string& line : Log)
                writer << line << "\n";
            SetStatus("Results saved to " + path);
            break;
        }
        case 8: // aboutToolStripMenuItem_Click_1
        {
            cout << "\nStatistical information (position vector):\n";
            cout << "Statistical metric         Count\n";
            for (const Statistic& s : Statistics)
                cout << s.ToString() << "\n";
            break;
        }
        case 9: // closeToolStripMenuItem_Click
            running = false;
            break;
        default:
            cout << "Unknown option.\n";
            break;
        }
    }

    return 0;
}

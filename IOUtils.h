#pragma once
#include <string>
#include "GenericDoublyLinkedList.h"
#include "Module.h"
#include "SelectedModule.h"

namespace P5
{
    // Static-style utility namespace for I/O file operations (reading).
    // Throws std::runtime_error on failure (caller decides how to react),
    // matching the C# version's "catch (Exception) { throw; }" behaviour.
    namespace IOUtils
    {
        // Reads initial data of selected modules by students from filename.
        GenericDoublyLinkedList<SelectedModule> ReadSelectedModules(const std::string& filename);

        // Reads initial data of modules from filename.
        GenericDoublyLinkedList<Module> ReadModules(const std::string& filename);
    }
}

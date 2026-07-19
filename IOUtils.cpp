#include "IOUtils.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace P5
{
    namespace
    {
        // Splits a string by a single-character delimiter (equivalent to
        // C#'s string.Split(char)).
        std::vector<std::string> Split(const std::string& s, char delim)
        {
            std::vector<std::string> tokens;
            std::stringstream ss(s);
            std::string item;
            while (std::getline(ss, item, delim))
                tokens.push_back(item);
            return tokens;
        }

        // Strips a UTF-8 byte-order-mark from the start of a string, if present.
        void StripBom(std::string& s)
        {
            if (s.size() >= 3 &&
                (unsigned char)s[0] == 0xEF &&
                (unsigned char)s[1] == 0xBB &&
                (unsigned char)s[2] == 0xBF)
                s.erase(0, 3);
        }

        // Strips a trailing '\r' left over from Windows-style CRLF line endings.
        void StripTrailingCr(std::string& s)
        {
            if (!s.empty() && s.back() == '\r')
                s.pop_back();
        }
    }

    namespace IOUtils
    {
        GenericDoublyLinkedList<SelectedModule> ReadSelectedModules(const std::string& filename)
        {
            GenericDoublyLinkedList<SelectedModule> selectedModules;

            std::ifstream reader(filename, std::ios::binary);
            if (!reader.is_open())
                throw std::runtime_error("Could not open file: " + filename);

            std::string line;
            bool firstLine = true;
            while (std::getline(reader, line))
            {
                StripTrailingCr(line);
                if (firstLine) { StripBom(line); firstLine = false; }
                if (line.empty())
                    continue;

                std::vector<std::string> parts = Split(line, ';');
                if (parts.size() < 5)
                    throw std::runtime_error("Malformed line in " + filename + ": " + line);

                int id = std::stoi(parts[0]);
                std::string nameOfModule = parts[1];
                std::string studentSurname = parts[2];
                std::string studentName = parts[3];
                std::string group = parts[4];

                SelectedModule sm(id, nameOfModule, studentSurname, studentName, group);
                // Avoid duplicates
                if (!selectedModules.Contains(sm))
                    selectedModules.AddToEnd(sm);
            }

            return selectedModules;
        }

        GenericDoublyLinkedList<Module> ReadModules(const std::string& filename)
        {
            GenericDoublyLinkedList<Module> modules;

            std::ifstream reader(filename, std::ios::binary);
            if (!reader.is_open())
                throw std::runtime_error("Could not open file: " + filename);

            std::string line;
            bool firstLine = true;
            while (std::getline(reader, line))
            {
                StripTrailingCr(line);
                if (firstLine) { StripBom(line); firstLine = false; }
                if (line.empty())
                    continue;

                std::vector<std::string> parts = Split(line, ';');
                if (parts.size() < 4)
                    throw std::runtime_error("Malformed line in " + filename + ": " + line);

                std::string nameOfModule = parts[0];
                std::string instructorSurname = parts[1];
                std::string instructorName = parts[2];
                int credits = std::stoi(parts[3]);

                Module m(nameOfModule, instructorSurname, instructorName, credits);
                // Avoid duplicates
                if (!modules.Contains(m))
                    modules.AddToEnd(m);
            }

            return modules;
        }
    }
}

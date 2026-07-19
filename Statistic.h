#pragma once
#include <string>
#include <sstream>
#include <iomanip>

namespace P5
{
    // Small data class used to track how many times each menu action has run
    // (equivalent to the "About" screen's statistics grid in the original app).
    class Statistic
    {
    public:
        std::string MetricName;
        int Count = 0;

        Statistic() = default;

        Statistic(std::string metricName, int count)
            : MetricName(std::move(metricName)), Count(count)
        {
        }

        void Increase() { Count++; }

        int CompareTo(const Statistic& other) const
        {
            return MetricName.compare(other.MetricName);
        }

        bool Equals(const Statistic& other) const
        {
            return MetricName == other.MetricName && Count == other.Count;
        }

        std::string ToString() const
        {
            std::ostringstream oss;
            oss << std::left << std::setw(25) << MetricName
                << std::right << std::setw(5) << Count;
            return oss.str();
        }
    };

    inline bool operator==(const Statistic& a, const Statistic& b) { return a.Equals(b); }
    inline bool operator<(const Statistic& a, const Statistic& b) { return a.CompareTo(b) < 0; }
}

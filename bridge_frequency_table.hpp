#ifndef BRIDGE_FREQUENCY_TABLE_HPP
#define BRIDGE_FREQUENCY_TABLE_HPP

#include <cstdint>
#include <vector>

struct FrequencyTrace {
    uint32_t bridge_global_index;
    uint32_t activation_frequency;
};

class BridgeFrequencyTable {
private:
    std::vector<FrequencyTrace> tracking_table;

public:
    BridgeFrequencyTable() {
        tracking_table.reserve(1000000);
    }

    void register_or_increment_frequency(uint32_t bridge_idx) {
        for (auto& trace : tracking_table) {
            if (trace.bridge_global_index == bridge_idx) {
                trace.activation_frequency++;
                return;
            }
        }
        FrequencyTrace new_trace;
        new_trace.bridge_global_index = bridge_idx;
        new_trace.activation_frequency = 1;
        tracking_table.push_back(new_trace);
    }

    uint32_t get_activation_count(uint32_t bridge_idx) const {
        for (const auto& trace : tracking_table) {
            if (trace.bridge_global_index == bridge_idx) {
                return trace.activation_frequency;
            }
        }
        return 0;
    }

    size_t size() const {
        return tracking_table.size();
    }
};

#endif
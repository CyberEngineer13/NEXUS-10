#ifndef BRAIN_EMERGENCE_HPP
#define BRAIN_EMERGENCE_HPP

#include <cstdint>
#include <vector>
#include "topology_space.hpp"

class BrainEmergence {
public:
    static inline uint64_t calculate_field_emergence(
        const TopologySpace& space,
        const std::vector<uint32_t>& active_context_nodes
    ) {
        if (active_context_nodes.size() < 2) {
            return 0;
        }

        uint64_t accumulated_field_tension = 0;
        size_t node_count = active_context_nodes.size();

        for (size_t i = 0; i < node_count; ++i) {
            for (size_t j = i + 1; j < node_count; ++j) {
                uint16_t tension = space.get_spatial_tension(active_context_nodes[i], active_context_nodes[j]);
                accumulated_field_tension += static_cast<uint64_t>(tension);
            }
        }

        return accumulated_field_tension;
    }
};

#endif
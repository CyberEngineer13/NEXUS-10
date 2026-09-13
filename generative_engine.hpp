#ifndef GENERATIVE_ENGINE_HPP
#define GENERATIVE_ENGINE_HPP

#include <cstdint>
#include <vector>
#include "association_engine.hpp"

class GenerativeEngine {
public:
    static inline uint32_t select_next_topological_node(
        const std::vector<AssociatedLink>& active_links,
        uint16_t dynamic_cutoff_threshold
    ) {
        if (active_links.empty()) {
            return 0xFFFFFFFF;
        }

        uint32_t best_node_idx = 0xFFFFFFFF;
        uint16_t highest_tension = 0;

        for (const auto& link : active_links) {
            if (link.link_tension >= dynamic_cutoff_threshold) {
                if (link.link_tension > highest_tension) {
                    highest_tension = link.link_tension;
                    best_node_idx = link.target_node_index;
                }
            }
        }

        return best_node_idx;
    }
};

#endif
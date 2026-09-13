#ifndef ASSOCIATION_ENGINE_HPP
#define ASSOCIATION_ENGINE_HPP

#include <cstdint>
#include <vector>
#include "topology_space.hpp"

struct AssociatedLink {
    uint32_t target_node_index;
    uint16_t link_tension;
};

class AssociationEngine {
public:
    static inline std::vector<AssociatedLink> collect_associated_links(
        const TopologySpace& space, 
        uint32_t source_node_idx
    ) {
        std::vector<AssociatedLink> links;
        size_t total_nodes = space.get_node_count();
        
        for (size_t i = 0; i < total_nodes; ++i) {
            if (i == source_node_idx) {
                continue;
            }
            uint16_t tension = space.get_spatial_tension(source_node_idx, static_cast<uint32_t>(i));
            if (tension > 0) {
                AssociatedLink link;
                link.target_node_index = static_cast<uint32_t>(i);
                link.link_tension = tension;
                links.push_back(link);
            }
        }
        return links;
    }
};

#endif
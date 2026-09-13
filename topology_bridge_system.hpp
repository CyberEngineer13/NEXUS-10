#ifndef TOPOLOGY_BRIDGE_SYSTEM_HPP
#define TOPOLOGY_BRIDGE_SYSTEM_HPP

#include <cstdint>

struct TopologicalBridge {
    uint32_t node_a_index;
    uint32_t node_b_index;
    uint16_t dynamic_code;
};

class TopologyBridgeSystem {
public:
    static inline TopologicalBridge create_bridge(uint32_t node_a, uint32_t node_b, uint16_t code) {
        TopologicalBridge bridge;
        bridge.node_a_index = node_a;
        bridge.node_b_index = node_b;
        bridge.dynamic_code = code & 0x03FF;
        return bridge;
    }

    static inline bool check_structural_alignment(const TopologicalBridge& bridge, uint32_t node_a, uint32_t node_b) {
        return (bridge.node_a_index == node_a && bridge.node_b_index == node_b) ||
               (bridge.node_a_index == node_b && bridge.node_b_index == node_a);
    }

    static inline uint16_t extract_10bit_code(const TopologicalBridge& bridge) {
        return bridge.dynamic_code & 0x03FF;
    }

    static inline void mutate_10bit_code(TopologicalBridge& bridge, uint16_t new_code) {
        bridge.dynamic_code = new_code & 0x03FF;
    }
};

#endif
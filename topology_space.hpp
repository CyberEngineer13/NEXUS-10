#ifndef TOPOLOGY_SPACE_HPP
#define TOPOLOGY_SPACE_HPP

#include <cstdint>
#include <vector>
#include <fstream>
#include "topology_bridge_system.hpp"

struct SpatialNode {
    uint64_t word_id;
    uint32_t first_bridge_index;
    uint32_t bridge_count;
};

class TopologySpace {
private:
    std::vector<SpatialNode> nodes;
    std::vector<TopologicalBridge> global_bridges;

public:
    TopologySpace() {
        nodes.reserve(1000000);
        global_bridges.reserve(10000000);
    }

    uint32_t find_or_create_node(uint64_t word_id) {
        uint64_t clean_id = word_id & 0x3FFFFFFFFULL;
        for (size_t i = 0; i < nodes.size(); ++i) {
            if (nodes[i].word_id == clean_id) {
                return static_cast<uint32_t>(i);
            }
        }
        SpatialNode new_node;
        new_node.word_id = clean_id;
        new_node.first_bridge_index = 0;
        new_node.bridge_count = 0;
        nodes.push_back(new_node);
        return static_cast<uint32_t>(nodes.size() - 1);
    }

    void connect_or_update_spatial_bridge(uint32_t node_a_idx, uint32_t node_b_idx, uint16_t current_code) {
        for (size_t i = 0; i < global_bridges.size(); ++i) {
            if (TopologyBridgeSystem::check_structural_alignment(global_bridges[i], node_a_idx, node_b_idx)) {
                TopologyBridgeSystem::mutate_10bit_code(global_bridges[i], current_code);
                return;
            }
        }
        TopologicalBridge new_bridge = TopologyBridgeSystem::create_bridge(node_a_idx, node_b_idx, current_code);
        global_bridges.push_back(new_bridge);
        if (nodes[node_a_idx].bridge_count == 0) {
            nodes[node_a_idx].first_bridge_index = static_cast<uint32_t>(global_bridges.size() - 1);
        }
        nodes[node_a_idx].bridge_count++;
        if (nodes[node_b_idx].bridge_count == 0) {
            nodes[node_b_idx].first_bridge_index = static_cast<uint32_t>(global_bridges.size() - 1);
        }
        nodes[node_b_idx].bridge_count++;
    }

    uint16_t get_spatial_tension(uint32_t node_a_idx, uint32_t node_b_idx) const {
        for (size_t i = 0; i < global_bridges.size(); ++i) {
            if (TopologyBridgeSystem::check_structural_alignment(global_bridges[i], node_a_idx, node_b_idx)) {
                return TopologyBridgeSystem::extract_10bit_code(global_bridges[i]);
            }
        }
        return 0;
    }

    size_t get_node_count() const {
        return nodes.size();
    }

    size_t get_bridge_count() const {
        return global_bridges.size();
    }

    void direct_serialize(std::ofstream& out) const {
        uint64_t node_size = nodes.size();
        out.write(reinterpret_cast<const char*>(&node_size), sizeof(node_size));
        for (const auto& node : nodes) {
            out.write(reinterpret_cast<const char*>(&node.word_id), sizeof(node.word_id));
            out.write(reinterpret_cast<const char*>(&node.first_bridge_index), sizeof(node.first_bridge_index));
            out.write(reinterpret_cast<const char*>(&node.bridge_count), sizeof(node.bridge_count));
        }

        uint64_t bridge_size = global_bridges.size();
        out.write(reinterpret_cast<const char*>(&bridge_size), sizeof(bridge_size));
        for (const auto& bridge : global_bridges) {
            uint32_t n_a = bridge.node_a_index;
            uint32_t n_b = bridge.node_b_index;
            uint16_t code = bridge.dynamic_code;
            out.write(reinterpret_cast<const char*>(&n_a), sizeof(n_a));
            out.write(reinterpret_cast<const char*>(&n_b), sizeof(n_b));
            out.write(reinterpret_cast<const char*>(&code), sizeof(code));
        }
    }

    void direct_deserialize(std::ifstream& in) {
        uint64_t node_size = 0;
        in.read(reinterpret_cast<char*>(&node_size), sizeof(node_size));
        nodes.clear();
        nodes.reserve(node_size);
        for (uint64_t i = 0; i < node_size; ++i) {
            SpatialNode node;
            in.read(reinterpret_cast<char*>(&node.word_id), sizeof(node.word_id));
            in.read(reinterpret_cast<char*>(&node.first_bridge_index), sizeof(node.first_bridge_index));
            in.read(reinterpret_cast<char*>(&node.bridge_count), sizeof(node.bridge_count));
            nodes.push_back(node);
        }

        uint64_t bridge_size = 0;
        in.read(reinterpret_cast<char*>(&bridge_size), sizeof(bridge_size));
        global_bridges.clear();
        global_bridges.reserve(bridge_size);
        for (uint64_t i = 0; i < bridge_size; ++i) {
            TopologicalBridge bridge;
            in.read(reinterpret_cast<char*>(&bridge.node_a_index), sizeof(bridge.node_a_index));
            in.read(reinterpret_cast<char*>(&bridge.node_b_index), sizeof(bridge.node_b_index));
            in.read(reinterpret_cast<char*>(&bridge.dynamic_code), sizeof(bridge.dynamic_code));
            global_bridges.push_back(bridge);
        }
    }
};

#endif
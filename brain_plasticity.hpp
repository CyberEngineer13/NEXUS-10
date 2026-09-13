#ifndef BRAIN_PLASTICITY_HPP
#define BRAIN_PLASTICITY_HPP

#include <cstdint>
#include <vector>
#include "topology_space.hpp"
#include "vocabulary_core.hpp"
#include "grammar_rules_evaluator.hpp"
#include "bridge_strength_evaluator.hpp"

class BrainPlasticity {
public:
    static inline void rewire_topology_network(
        TopologySpace& space,
        const VocabularyCore& vocabulary,
        const std::vector<uint64_t>& perception_ids,
        uint32_t context_window_size
    ) {
        if (perception_ids.size() < 2) {
            return;
        }

        size_t id_count = perception_ids.size();
        for (size_t i = 0; i < id_count; ++i) {
            uint32_t window_end = static_cast<uint32_t>(i) + context_window_size;
            for (size_t j = i + 1; j < window_end && j < id_count; ++j) {
                uint32_t linear_dist = static_cast<uint32_t>(j - i);
                
                uint64_t word_a_id = perception_ids[i];
                uint64_t word_b_id = perception_ids[j];

                uint32_t node_a_idx = space.find_or_create_node(word_a_id);
                uint32_t node_b_idx = space.find_or_create_node(word_b_id);

                WordGrammarProfile profile_a = {1, 1, 1};
                WordGrammarProfile profile_b = {1, 1, 1};
                std::string empty_str;

                const_cast<VocabularyCore&>(vocabulary).get_word_profile(word_a_id, profile_a, empty_str);
                const_cast<VocabularyCore&>(vocabulary).get_word_profile(word_b_id, profile_b, empty_str);

                uint16_t initial_code = GrammarRulesEvaluator::evaluate_exact_code(
                    profile_a, 
                    profile_b, 
                    linear_dist, 
                    false
                );

                uint16_t current_tension = space.get_spatial_tension(node_a_idx, node_b_idx);
                uint16_t dynamic_new_code = BridgeStrengthEvaluator::reinforce_tension(current_tension, initial_code);

                space.connect_or_update_spatial_bridge(node_a_idx, node_b_idx, dynamic_new_code);
            }
        }
    }
};

#endif
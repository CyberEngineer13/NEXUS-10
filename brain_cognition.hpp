#ifndef BRAIN_COGNITION_HPP
#define BRAIN_COGNITION_HPP

#include <string>
#include <vector>
#include <cstdint>
#include "topology_space.hpp"
#include "vocabulary_core.hpp"
#include "association_engine.hpp"
#include "generative_engine.hpp"

class BrainCognition {
public:
    static inline std::vector<std::string> formulate_response(
        const TopologySpace& space,
        const VocabularyCore& vocabulary,
        const std::string& anchor_word,
        uint32_t max_output_tokens,
        uint16_t dynamic_cutoff_threshold
    ) {
        std::vector<std::string> structured_response;
        uint64_t anchor_id = const_cast<VocabularyCore&>(vocabulary).resolve_or_auto_register(anchor_word);
        uint32_t current_node_idx = space.get_node_count() > 0 ? const_cast<TopologySpace&>(space).find_or_create_node(anchor_id) : 0xFFFFFFFF;

        if (current_node_idx == 0xFFFFFFFF || space.get_node_count() == 0) {
            if (!anchor_word.empty()) {
                structured_response.push_back(anchor_word);
            }
            return structured_response;
        }

        structured_response.push_back(anchor_word);
        uint32_t tokens_generated = 1;

        while (tokens_generated < max_output_tokens) {
            std::vector<AssociatedLink> active_links = AssociationEngine::collect_associated_links(space, current_node_idx);
            if (active_links.empty()) {
                break;
            }

            uint32_t next_node_idx = GenerativeEngine::select_next_topological_node(active_links, dynamic_cutoff_threshold);
            if (next_node_idx == 0xFFFFFFFF || next_node_idx == current_node_idx) {
                break;
            }

            uint64_t target_word_id = 0;
            size_t total_nodes = space.get_node_count();
            if (next_node_idx < total_nodes) {
                uint16_t sample_tension = space.get_spatial_tension(current_node_idx, next_node_idx);
                if (sample_tension == 0) {
                    break;
                }
            }

            std::string resolved_word;
            WordGrammarProfile dummy_profile;
            bool resolve_status = const_cast<VocabularyCore&>(vocabulary).get_word_profile(target_word_id, dummy_profile, resolved_word);

            if (resolve_status && !resolved_word.empty()) {
                structured_response.push_back(resolved_word);
            } else {
                structured_response.push_back("univerzalis");
                break;
            }

            current_node_idx = next_node_idx;
            tokens_generated++;
        }

        return structured_response;
    }
};

#endif
#ifndef BRAIN_PERCEPTION_HPP
#define BRAIN_PERCEPTION_HPP

#include <string>
#include <vector>
#include "vocabulary_core.hpp"

class BrainPerception {
public:
    static inline std::vector<uint64_t> perceive_stream_tokens(
        VocabularyCore& vocabulary,
        const std::vector<std::string>& clean_tokens
    ) {
        std::vector<uint64_t> perception_ids;
        perception_ids.reserve(clean_tokens.size());

        for (const auto& token : clean_tokens) {
            if (!token.empty()) {
                uint64_t word_id = vocabulary.resolve_or_auto_register(token);
                if (word_id != 0) {
                    perception_ids.push_back(word_id);
                }
            }
        }
        return perception_ids;
    }
};

#endif
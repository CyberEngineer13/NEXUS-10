#ifndef COGNITIVE_FILTER_HPP
#define COGNITIVE_FILTER_HPP

#include <cstdint>

class CognitiveFilter {
public:
    static inline bool validate_topological_sanity(uint16_t link_tension, uint16_t minimal_sanitizer_bound) {
        return link_tension >= minimal_sanitizer_bound;
    }

    static inline uint16_t dynamic_suppression(uint16_t original_tension, bool high_noise_environment) {
        if (high_noise_environment) {
            if (original_tension > 200) {
                return original_tension - 200;
            }
            return 1;
        }
        return original_tension;
    }
};

#endif
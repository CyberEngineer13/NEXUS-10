#ifndef BRIDGE_STRENGTH_EVALUATOR_HPP
#define BRIDGE_STRENGTH_EVALUATOR_HPP

#include <cstdint>

class BridgeStrengthEvaluator {
public:
    static inline uint16_t reinforce_tension(uint16_t current_tension, uint16_t signal_strength) {
        uint32_t raw_sum = static_cast<uint32_t>(current_tension) + static_cast<uint32_t>(signal_strength);
        if (raw_sum > 1023) {
            return 1023;
        }
        return static_cast<uint16_t>(raw_sum);
    }

    static inline uint16_t decay_tension(uint16_t current_tension, uint16_t decay_rate) {
        if (current_tension <= decay_rate) {
            return 1;
        }
        return current_tension - decay_rate;
    }

    static inline bool is_link_broken(uint16_t current_tension) {
        return current_tension <= 1;
    }
};

#endif
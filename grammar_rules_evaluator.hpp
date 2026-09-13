#ifndef GRAMMAR_RULES_EVALUATOR_HPP
#define GRAMMAR_RULES_EVALUATOR_HPP

#include <cstdint>

struct WordGrammarProfile {
    uint8_t token_type;
    uint8_t structural_layer;
    uint8_t execution_context;
};

class GrammarRulesEvaluator {
public:
    static inline uint16_t evaluate_exact_code(
        WordGrammarProfile profile_a, 
        WordGrammarProfile profile_b, 
        uint32_t linear_distance,
        bool is_axiom
    ) {
        if (is_axiom) {
            return 1023;
        }

        if (linear_distance > 100) {
            return 0;
        }

        uint8_t type_a = profile_a.token_type;
        uint8_t type_b = profile_b.token_type;

        if (type_a == type_b && profile_a.structural_layer == profile_b.structural_layer) {
            if (linear_distance <= 1) {
                return 950;
            }
            return 800;
        }

        if (type_a == 1 && type_b == 2) {
            if (linear_distance <= 2) {
                return 700;
            }
            return 550;
        }

        if (linear_distance == 1) {
            return 400;
        }

        return 1;
    }
};

#endif
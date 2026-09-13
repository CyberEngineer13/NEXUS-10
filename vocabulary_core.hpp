#ifndef VOCABULARY_CORE_HPP
#define VOCABULARY_CORE_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include "grammar_rules_evaluator.hpp"

struct VocabularyEntry {
    uint64_t embedded_word_id;
    WordGrammarProfile grammar_profile;
    std::string word_string;
};

class VocabularyCore {
private:
    std::vector<VocabularyEntry> fast_lookup_table;
    uint64_t current_vocabulary_size;
    uint64_t max_capacity;

    uint64_t generate_hash_34bit(const std::string& word) const {
        uint64_t hash = 14695981039346656037ULL;
        for (char ch : word) {
            hash ^= static_cast<uint64_t>(ch);
            hash *= 1099511628211ULL;
        }
        return hash & 0x3FFFFFFFFULL;
    }

public:
    VocabularyCore(uint64_t capacity) : current_vocabulary_size(0), max_capacity(capacity) {}

    uint64_t register_word(const std::string& word, WordGrammarProfile profile) {
        uint64_t generated_id = generate_hash_34bit(word);
        
        for (const auto& entry : fast_lookup_table) {
            if (entry.embedded_word_id == generated_id && entry.word_string == word) {
                return entry.embedded_word_id;
            }
        }

        if (fast_lookup_table.size() < max_capacity) {
            VocabularyEntry new_entry;
            new_entry.embedded_word_id = generated_id;
            new_entry.grammar_profile = profile;
            new_entry.word_string = word;
            fast_lookup_table.push_back(new_entry);
            current_vocabulary_size++;
            return generated_id;
        }

        return 0;
    }

    uint64_t resolve_or_auto_register(const std::string& word) {
        WordGrammarProfile dynamic_unknown_profile = {1, 1, 1};
        return register_word(word, dynamic_unknown_profile);
    }

    bool get_word_profile(uint64_t word_id, WordGrammarProfile& out_profile, std::string& out_word) const {
        uint64_t clean_id = word_id & 0x3FFFFFFFFULL;
        for (const auto& entry : fast_lookup_table) {
            if (entry.embedded_word_id == clean_id) {
                out_profile = entry.grammar_profile;
                out_word = entry.word_string;
                return true;
            }
        }
        return false;
    }

    uint64_t size() const {
        return current_vocabulary_size;
    }

    void direct_serialize(std::ofstream& out) const {
        out.write(reinterpret_cast<const char*>(&current_vocabulary_size), sizeof(current_vocabulary_size));
        out.write(reinterpret_cast<const char*>(&max_capacity), sizeof(max_capacity));
        for (const auto& entry : fast_lookup_table) {
            out.write(reinterpret_cast<const char*>(&entry.embedded_word_id), sizeof(entry.embedded_word_id));
            out.write(reinterpret_cast<const char*>(&entry.grammar_profile), sizeof(entry.grammar_profile));
            uint32_t str_len = static_cast<uint32_t>(entry.word_string.size());
            out.write(reinterpret_cast<const char*>(&str_len), sizeof(str_len));
            out.write(entry.word_string.c_str(), str_len);
        }
    }

    void direct_deserialize(std::ifstream& in) {
        uint64_t loaded_size = 0;
        in.read(reinterpret_cast<char*>(&loaded_size), sizeof(loaded_size));
        in.read(reinterpret_cast<char*>(&max_capacity), sizeof(max_capacity));
        
        fast_lookup_table.clear();
        fast_lookup_table.reserve(loaded_size);
        current_vocabulary_size = loaded_size;

        for (uint64_t i = 0; i < loaded_size; ++i) {
            VocabularyEntry entry;
            in.read(reinterpret_cast<char*>(&entry.embedded_word_id), sizeof(entry.embedded_word_id));
            in.read(reinterpret_cast<char*>(&entry.grammar_profile), sizeof(entry.grammar_profile));
            uint32_t str_len = 0;
            in.read(reinterpret_cast<char*>(&str_len), sizeof(str_len));
            
            std::vector<char> buf(str_len + 1, 0);
            in.read(buf.data(), str_len);
            entry.word_string = std::string(buf.data());
            
            fast_lookup_table.push_back(entry);
        }
    }
};

#endif
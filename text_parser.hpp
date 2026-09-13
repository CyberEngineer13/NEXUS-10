#ifndef TEXT_PARSER_HPP
#define TEXT_PARSER_HPP

#include <string>
#include <vector>
#include <cctype>

class TextParser {
public:
    static inline std::vector<std::string> tokenize_and_clean(const std::string& raw_content) {
        std::vector<std::string> tokens;
        std::string current_token = "";
        bool in_html_tag = false;

        for (char ch : raw_content) {
            if (ch == '<') {
                in_html_tag = true;
                if (!current_token.empty()) {
                    tokens.push_back(current_token);
                    current_token.clear();
                }
                continue;
            }
            if (ch == '>') {
                in_html_tag = false;
                continue;
            }
            if (in_html_tag) {
                continue;
            }

            if (std::isalnum(static_cast<unsigned char>(ch)) || ch == '-' || ch == '_') {
                current_token += static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
            } else {
                if (!current_token.empty()) {
                    tokens.push_back(current_token);
                    current_token.clear();
                }
            }
        }

        if (!current_token.empty()) {
            tokens.push_back(current_token);
        }

        return tokens;
    }
};

#endif
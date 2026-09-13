#ifndef STORAGE_STREAMER_HPP
#define STORAGE_STREAMER_HPP

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include "vocabulary_core.hpp"
#include "topology_space.hpp"
#include "text_parser.hpp"
#include "brain_perception.hpp"
#include "brain_plasticity.hpp"

class StorageStreamer {
public:
    static inline void scan_ingest_and_purge_drive(
        const std::string& target_directory,
        VocabularyCore& vocabulary,
        TopologySpace& space,
        uint32_t context_window_size
    ) {
        std::error_code ec;
        if (!std::filesystem::exists(target_directory)) {
            std::filesystem::create_directories(target_directory, ec);
        }

        bool has_files = false;
        for (const auto& entry : std::filesystem::directory_iterator(target_directory, ec)) {
            if (entry.is_regular_file(ec)) {
                has_files = true;
                std::string file_path = entry.path().string();
                std::ifstream file(file_path, std::ios::in | std::ios::binary | std::ios::ate);
                
                if (!file) {
                    continue;
                }

                std::streamsize file_size = file.tellg();
                file.seekg(0, std::ios::beg);

                if (file_size <= 0) {
                    file.close();
                    std::filesystem::remove(file_path, ec);
                    continue;
                }

                std::string raw_content(static_cast<size_t>(file_size), '\0');
                if (file.read(raw_content.data(), file_size)) {
                    file.close();
                    
                    std::vector<std::string> clean_tokens = TextParser::tokenize_and_clean(raw_content);
                    std::vector<uint64_t> perception_ids = BrainPerception::perceive_stream_tokens(vocabulary, clean_tokens);
                    
                    BrainPlasticity::rewire_topology_network(space, vocabulary, perception_ids, context_window_size);
                    
                    std::filesystem::remove(file_path, ec);
                } else {
                    file.close();
                }
            }
        }

        if (!has_files && vocabulary.size() == 0) {
            std::string auto_text = "az emberi elme kulonleges es intelligens biologiai rendszer amely megerti az univerzum mukodeset a galaxisok csillagok es a bolygo tiszta topologiai rendszert alkotnak a filozofia es a tudomany kutatja a valosag melyebb osszefuggeseit a technologia fejlodesse laggolasmentes tiszta bitmuveletek segitsegevel uj szoftveres strukturat teremt a fogalmak kozotti tizedik bites hidak feszultsege fesziti ki a tudasteret koordinatak nelkul a kognitiv szuro es a kritikus gondolkodas megakadalyozza a fals irrealis mintak kiepuleset az emergencia elve alapjan a teljes mondat kontextusa hatarozza meg a vizmolekulakhoz hasonlo nedves erzest az absztrakt ihlet es az improvizacio folyamatos rezonancia feszultseg mellett uj eszkozoket es sûrûseget ad a tanulashoz";
            std::vector<std::string> clean_tokens = TextParser::tokenize_and_clean(auto_text);
            std::vector<uint64_t> perception_ids = BrainPerception::perceive_stream_tokens(vocabulary, clean_tokens);
            BrainPlasticity::rewire_topology_network(space, vocabulary, perception_ids, context_window_size);
        }
    }
};

#endif
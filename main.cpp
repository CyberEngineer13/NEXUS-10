#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include "vocabulary_core.hpp"
#include "topology_space.hpp"
#include "bridge_frequency_table.hpp"
#include "brain_perception.hpp"
#include "brain_plasticity.hpp"
#include "brain_resonance.hpp"
#include "brain_cognition.hpp"
#include "storage_streamer.hpp"

int main(int argc, char* argv[]) {
    std::cout << "=== NEXUS-10 FIXED DUAL-CHANNEL INTERFACE ===" << std::endl;
    
    std::string safe_zone_dir = "/f/NEXUS-10/NEXUS-10_safe_zone/";
    std::string vocab_dir = "/f/NEXUS-10/SZOTAR/";
    std::string space_dir = "/f/NEXUS-10/TER/";
    
    std::string vocab_file = vocab_dir + "vocabulary.bin";
    std::string space_file = space_dir + "topology.bin";
    
    std::error_code dir_ec;
    std::filesystem::create_directories(safe_zone_dir, dir_ec);
    std::filesystem::create_directories(vocab_dir, dir_ec);
    std::filesystem::create_directories(space_dir, dir_ec);

    uint64_t max_vocab_capacity = 1000000;
    VocabularyCore vocabulary(max_vocab_capacity);
    TopologySpace space;
    BridgeFrequencyTable frequency_table;
    BrainResonance resonance;

    bool vocab_exists = std::filesystem::exists(vocab_file) && std::filesystem::file_size(vocab_file) > 0;
    bool space_exists = std::filesystem::exists(space_file) && std::filesystem::file_size(space_file) > 0;

    if (vocab_exists && space_exists) {
        std::ifstream in_v(vocab_file, std::ios::in | std::ios::binary);
        std::ifstream in_s(space_file, std::ios::in | std::ios::binary);
        if (in_v && in_s) {
            vocabulary.direct_deserialize(in_v);
            space.direct_deserialize(in_s);
            in_v.close();
            in_s.close();
            std::cout << "-> Sikeres dualis betoltes az F-rol! Szavak: " << vocabulary.size() << " [SZOTAR] | Hidak: " << space.get_bridge_count() << " [TER]" << std::endl;
        }
    } else {
        std::cout << "-> Tiszta dualis topologia inditasa az F meghajton." << std::endl;
    }

    std::cout << "-> Agyplaszticitas aktivalasa a NEXUS-10 zonaban..." << std::endl;
    uint32_t context_window_size = 5;
    StorageStreamer::scan_ingest_and_purge_drive(safe_zone_dir, vocabulary, space, context_window_size);

    size_t active_bridges = space.get_bridge_count();
    for (size_t i = 0; i < active_bridges; ++i) {
        frequency_table.register_or_increment_frequency(static_cast<uint32_t>(i));
    }

    std::cout << "-> Frissitett egyedi fogalmak szama: " << vocabulary.size() << std::endl;
    std::cout << "-> Frissitett topologiai hidak szama: " << space.get_bridge_count() << std::endl;

    std::ofstream out_v(vocab_file, std::ios::out | std::ios::binary);
    std::ofstream out_s(space_file, std::ios::out | std::ios::binary);
    if (out_v && out_s) {
        vocabulary.direct_serialize(out_v);
        space.direct_serialize(out_s);
        out_v.close();
        out_s.close();
        std::cout << "-> Adatbazisok sikeresen mentve a SZOTAR es TER mappakba az F-en." << std::endl;
    }

    std::string anchor = "univerzalis";
    if (argc > 1) {
        anchor = argv[1];
    }

    std::cout << "-> Gondolatalkotas a kovetkezo horgonyszo menten: " << anchor << std::endl;
    std::vector<std::string> response = BrainCognition::formulate_response(space, vocabulary, anchor, 30, 5);
    std::cout << "=== NEXUS-10 VALASZA ===\n";
    for (const std::string& word : response) {
        std::cout << word << " ";
    }
    std::cout << "\n=======================\n";

    return 0;
}
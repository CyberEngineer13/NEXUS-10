#ifndef CLOUD_MINER_HPP
#define CLOUD_MINER_HPP

#include <string>
#include <fstream>
#include <filesystem>

class CloudMiner {
public:
    static inline bool fetch_cloud_data_to_storage(
        const std::string& source_endpoint_url,
        const std::string& destination_safe_zone,
        uint64_t operational_session_id
    ) {
        if (source_endpoint_url.empty() || destination_safe_zone.empty()) {
            return false;
        }

        std::string target_file_path = destination_safe_zone;
        if (target_file_path.back() != '/' && target_file_path.back() != '\\') {
            target_file_path += "/";
        }
        target_file_path += "cloud_stream_" + std::to_string(operational_session_id) + ".dat";

        std::ofstream test_file(target_file_path, std::ios::out | std::ios::binary);
        if (!test_file) {
            return false;
        }

        test_file.close();
        return true;
    }
};

#endif
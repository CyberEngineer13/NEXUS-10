#ifndef BRAIN_RESONANCE_HPP
#define BRAIN_RESONANCE_HPP

#include <cstdint>

class BrainResonance {
private:
    uint16_t internal_resonance_state;
    uint32_t calibration_counter;

public:
    BrainResonance() : internal_resonance_state(512), calibration_counter(0) {}

    void shift_internal_state(uint16_t bridge_tension_signal) {
        uint32_t mixed_signal = (static_cast<uint32_t>(internal_resonance_state) * 7 + static_cast<uint32_t>(bridge_tension_signal) * 3) / 10;
        internal_resonance_state = static_cast<uint16_t>(mixed_signal & 0x03FF);
        calibration_counter++;
    }

    uint16_t fetch_resonance_bias() const {
        return internal_resonance_state;
    }

    uint32_t get_calibration_count() const {
        return calibration_counter;
    }
};

#endif
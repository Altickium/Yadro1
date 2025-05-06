#ifndef TAPE_HPP
#define TAPE_HPP

#include "tape_interface.hpp"
#include "config.hpp"

#include <fstream>
#include <vector>
#include <optional>

namespace tape_sorting {
class Tape : public TapeInterface {
private:
    std::fstream _file;
    int _current_pos;
    config _config;

public:
    size_t size;

    Tape() = default;

    explicit Tape(const std::string& file_path);

    explicit Tape(const std::string& file_path, const config& cfg);

    Tape(const Tape& other) = delete;

    Tape(Tape&& other) noexcept = default;

    Tape& operator=(const Tape& other) = delete;

    Tape& operator=(Tape&& other) noexcept = default;

    std::optional<int> read() override;

    void write(int value) override;

    // move carette
    void move_next() override;

    void move_prev() override;

    // other
    bool is_end() const override;

    void reset() override;

    size_t get_pos() const {
        return _current_pos;
    }

    void swap(Tape&);

    ~Tape() override;
};
}

#endif // TAPE_HPP
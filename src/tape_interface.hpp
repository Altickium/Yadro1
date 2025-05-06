#ifndef TAPE_INTERFACE_HPP
#define TAPE_INTERFACE_HPP

#include <vector>
#include <new>
#include <optional>

namespace tape_sorting {
    class TapeInterface {
    public:
        virtual ~TapeInterface() = default;

        virtual std::optional<int> read() = 0;

        virtual void write(int value) = 0;

        virtual void move_next() = 0;

        virtual void move_prev() = 0;

        virtual bool is_end() const = 0;

        virtual void reset() = 0;
    };
}
#endif // TAPE_INTERFACE_HPP
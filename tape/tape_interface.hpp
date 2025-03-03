#ifndef TAPE_INTERFACE_HPP
#define TAPE_INTERFACE_HPP

#include <vector>
#include <new>

class TapeInterface {
public:
    virtual ~TapeInterface() = default;

    virtual size_t getPosition() = 0;

    virtual int& read() = 0;

    virtual void write(int value) = 0;

    virtual void moveNext() = 0;

    virtual void movePrev() = 0;

    virtual bool isEnd() const = 0;

    virtual void reset() = 0;

    virtual void reset(size_t i) = 0;

    virtual void swap(int32_t& val) = 0;
};

#endif // TAPE_INTERFACE_HPP
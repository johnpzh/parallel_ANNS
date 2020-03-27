//
// Created by Zhen Peng on 3/26/20.
//

#ifndef PANNS_BITVECTOR_H
#define PANNS_BITVECTOR_H

#include "definitions.h"

namespace PANNS {

class BitVector {
private:
    uint8_t *data_;
//    idi num_bits_;
    idi num_bytes_;
public:
    BitVector() = default;
//    BitVector(idi num_bits) : num_bits_(num_bits)
    BitVector(idi num_bits)
    {
        num_bytes_ = (num_bits + 7) >> 3; // (n + 7) / 8
        data_ = new uint8_t[num_bytes_];
        memset(data_, 0, num_bytes_);
    }
    ~BitVector()
    {
        delete[] data_;
//        num_bits_ = 0;
        num_bytes_ = 0;
    }

    uint8_t atomic_is_bit_set(const idi x)
    {
        const idi i_byte = x >> 3;
        const idi i_bit = x - (i_byte << 3);
        return (__atomic_load_n(data_ + i_byte, __ATOMIC_ACQUIRE) >> i_bit) & 1;
    }

    void atomic_reset_bit(const idi x)
    {
        const idi i_byte = x >> 3;
        const idi i_bit = x - (i_byte << 3);
        __atomic_and_fetch(data_ + i_byte, ~(1 << i_bit), __ATOMIC_RELEASE);
    }
    void atomic_set_bit(const idi x)
    {
        const idi i_byte = x >> 3;
        const idi i_bit = x - (i_byte << 3);
//        data_[i_byte] &= (1 << i_bit);
        __atomic_or_fetch(data_ + i_byte, 1 << i_bit, __ATOMIC_RELEASE);
    }

    void clear_all()
    {
        memset(data_, 0, num_bytes_);
    }

    uint8_t is_bit_set(const idi x)
    {
        const idi i_byte = x >> 3;
        const idi i_bit = x - (i_byte << 3);
        return (data_[i_byte] >> i_bit) & 1;
    }

    void reset_bit(const idi x)
    {
        const idi i_byte = x >> 3;
        const idi i_bit = x - (i_byte << 3);
        data_[i_byte] &= ~(1 << i_bit);
    }
    void resize(const idi num_bits)
    {
        delete[] data_;
        num_bytes_ = (num_bits + 7) >> 3; // (n + 7) / 8
        data_ = new uint8_t[num_bytes_];
        memset(data_, 0, num_bytes_);
    }

    void set_bit(const idi x)
    {
        const idi i_byte = x >> 3;
        const idi i_bit = x - (i_byte << 3);
        data_[i_byte] |= (1 << i_bit);
    }
};

}// namespace PANNS

#endif //PANNS_BITVECTOR_H

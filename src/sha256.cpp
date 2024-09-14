/* ******************************************************* *
 * FileStore                                               *
 * (c) Florian Giesemann 2024                              *
 * ******************************************************* */

#include "FileStore/sha256.h"
#include <cassert>

namespace filestore {

SHA256::SHA256() {
    h[0] = 0x6a09e667;
    h[1] = 0xbb67ae85;
    h[2] = 0x3c6ef372;
    h[3] = 0xa54ff53a;
    h[4] = 0x510e527f;
    h[5] = 0x9b05688c;
    h[6] = 0x1f83d9ab;
    h[7] = 0x5be0cd19;
    bytes_stored = 0;
    bytes_processed = 0;
}

void SHA256::update(const char *data, size_t size) {
    char *buffer = reinterpret_cast<char *>(W);
    size_t data_offset = 0;

    while (size > 0) {
        const auto bytes_to_copy = std::min(size, free_buffer_bytes());
        std::memcpy(buffer + bytes_stored, data + data_offset, bytes_to_copy);
        bytes_stored += bytes_to_copy;
        size -= bytes_to_copy;
        data_offset += bytes_to_copy;

        if (buffer_full())
            process_block();
    }
}

SHA256::word byte_swap(SHA256::word w) {
    SHA256::word r = w & 0xff;
    r = (r << 8) | (w >> 8) & 0xff;
    r = (r << 8) | (w >> 16) & 0xff;
    r = (r << 8) | (w >> 24) & 0xff;
    return r;
}

uint64_t byte_swap(uint64_t w) {
    uint64_t r = w & 0xff;
    r = (r << 8) | (w >> 8) & 0xff;
    r = (r << 8) | (w >> 16) & 0xff;
    r = (r << 8) | (w >> 24) & 0xff;
    r = (r << 8) | (w >> 32) & 0xff;
    r = (r << 8) | (w >> 40) & 0xff;
    r = (r << 8) | (w >> 48) & 0xff;
    r = (r << 8) | (w >> 56) & 0xff;
    return r;
}

SHA256::hash_type SHA256::hash() {
    static_assert(SHA256::hash_size == sizeof(h) * 8);

    finalize();
    hash_type hash_value;
    for (int i = 0; i < 8; ++i) {
        word w = byte_swap(h[i]);
        std::memcpy(hash_value.data + sizeof(w) * i, &w, sizeof(w));
    }
    return hash_value;
}

void SHA256::process_block() {
    for (int i = 0; i < 16; ++i)
        W[i] = byte_swap(W[i]); // byte to number type conversion (byte order/endianess)

    for (int t = 16; t < 64; ++t)
        W[t] = sigma1(W[t - 2]) + W[t - 7] + sigma0(W[t - 15]) + W[t - 16];

    word w_tmp[8];
    std::copy(std::begin(h), std::end(h), std::begin(w_tmp));

    for (int t = 0; t < 64; ++t) {
        word T1 = w_tmp[7] + Sigma1(w_tmp[4]) + Ch(w_tmp[4], w_tmp[5], w_tmp[6]) + K[t] + W[t];
        word T2 = Sigma0(w_tmp[0]) + Maj(w_tmp[0], w_tmp[1], w_tmp[2]);
        w_tmp[7] = w_tmp[6];
        w_tmp[6] = w_tmp[5];
        w_tmp[5] = w_tmp[4];
        w_tmp[4] = w_tmp[3] + T1;
        w_tmp[3] = w_tmp[2];
        w_tmp[2] = w_tmp[1];
        w_tmp[1] = w_tmp[0];
        w_tmp[0] = T1 + T2;
    }

    for (int i = 0; i < 8; ++i)
        h[i] += w_tmp[i];

    bytes_processed += buffer_size;
    bytes_stored = 0;
}

void SHA256::finalize() {
    uint64_t message_bit_length = (bytes_processed + bytes_stored) * 8;

    char *buffer = reinterpret_cast<char *>(W);

    // append single 1-bit to the data
    // cannot be full, because then it would have already been processed and emptied again
    buffer[bytes_stored] = (uint8_t)0x80;
    ++bytes_stored;

    // fill remainder of the block with 0
    std::memset(buffer + bytes_stored, 0, free_buffer_bytes());

    if (free_buffer_bytes() < 8) { // not enough room to store message length
        // start a new block
        process_block();
        memset(buffer, 0, buffer_size);
    }

    // put message length at end of block
    uint64_t L = byte_swap(message_bit_length); // number type to bytes conversion (byte order/endianess)
    std::memcpy(buffer + buffer_size - sizeof(L), &L, sizeof(L));
    process_block();
}

SHA256::word SHA256::Ch(word x, word y, word z) {
    return (x & y) ^ (~x & z);
}

SHA256::word SHA256::Maj(word x, word y, word z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

SHA256::word SHA256::Sigma0(word x) {
    return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}

SHA256::word SHA256::Sigma1(word x) {
    return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

SHA256::word SHA256::sigma0(word x) {
    return rotr(x, 7) ^ rotr(x, 18) ^ shr(x, 3);
}

SHA256::word SHA256::sigma1(word x) {
    return rotr(x, 17) ^ rotr(x, 19) ^ shr(x, 10);
}

SHA256::word SHA256::shr(word x, int n) {
    return x >> n;
}

SHA256::word SHA256::rotr(word x, int n) {
    return (x >> n) | (x << (32 - n));
}

} // namespace filestore

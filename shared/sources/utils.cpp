#include "../headers/utils.hpp"

using namespace uahruart::utils;

// Hashing functions
uint32_t uahruart::utils::calculate_hash(const char* const ptr, uint8_t len) {
    /* uint32_t h = 1;
    for (uint32_t i = 0; i < len; i++)
        h += (uint32_t)((char*)ptr)[i] * i;
    return h; */

    decltype(len) aligned = len / sizeof(uint32_t); // len / 4
    uint8_t unaligned = len - aligned * sizeof(uint32_t);

    /*
    Because this implememtation uses a 32bit hashing function and because the byte array that will be
    hashed doesn't have to have a length divisible by 4 (32bit) there could be some spare bytes left
    This can be seen on the following diagram:
    [][][][] [][][][] [][][]--
    -------- -------- --------
       B0       B1       B2

    Blocks 0 and 1 are full, but block 2 is only 3 bytes, so the 1 byte at the end has to be accounted 
    for. If nothing was done there could be a segmentation fault (that one byte is outside the array).
    */

    // Base hashing function
    auto hash = hash_uint32;

    // Temporal variable for hash calculation (this variable will be the one returned)
    uint32_t final_hash = 0;

    // Calculate hash for aligned bytes
    for (decltype(len) i = 0; i < aligned; i++)
        final_hash ^= hash(((uint32_t*)ptr)[i]);


    // Valculate hash for unaligned bytes
    char tmp_buff[sizeof(uint32_t)];
    // Number to has (the rest of the bytes will be 0)
    *(uint32_t*)&tmp_buff = 0; // Initialize array to 0
    // Copy the unaligned bytes to the array
    for (uint8_t i = 0; i < unaligned; i++)
        tmp_buff[i] = ((char*)ptr)[i + aligned * sizeof(uint32_t)]; 

    final_hash ^= hash(*(uint32_t*)tmp_buff); 

    return final_hash;
}


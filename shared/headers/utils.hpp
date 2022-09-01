#ifndef UTILS_HPP
#define UTILS_HPP

/*
    Select functor library depending on the device running
    If using an arduino std::function will most likely not be present so a custom library has to be used
        The following are the platformio dependencies requires to run the protocol
            mike-matera/ArduinoSTL @ ^1.3.3
            silent/function_objects @ ^1.0.0
*/
#ifndef ARDUINO_BUILD
    #include <functional>
    template<typename __F>
    using functor = std::function<__F>;
#else
    #include <function_objects.h>
    template <typename __F>
    using functor = FunctionObject<__F>;
#endif

#include <stdint.h>

namespace uahruart {
    // !IMPORTANT
    // Protocol types definition. 
    // This types have to be defined according to the architecture of the processor
    // ----------
    namespace utils {
        // Functions
        constexpr uint32_t hash_uint32(const uint32_t value) {
            uint32_t tmp = value;
            tmp += ~(tmp<<15);
            tmp ^=  (tmp>>10);
            tmp +=  (tmp<<3);
            tmp ^=  (tmp>>6);
            tmp += ~(tmp<<11);
            tmp ^=  (tmp>>16);
            return tmp;
        }

        uint32_t calculate_hash(void* ptr, uint8_t len);

        // Constexpr string hash calculation (it has to be in the header file)
        constexpr uint32_t hash_string(const char* ptr) {
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

            uint32_t tmp_buff = 0;
            uint32_t index = 0;
            while (ptr[index]) {
                uint32_t local_index = index % sizeof(uint32_t);

                uint32_t value = (uint32_t)ptr[index] << (local_index * 8);
                tmp_buff |= value;
    
                if (local_index == 0) {
                    final_hash ^= hash(tmp_buff);
                    tmp_buff = 0;
                }
    
                index++;
    
            }
            if (((index - 1) % sizeof(uint32_t)) != 0)
                final_hash ^= hash(tmp_buff);
    
            return final_hash;
        }
    }
}

#endif

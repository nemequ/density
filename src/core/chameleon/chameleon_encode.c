/*
 * Centaurean Density
 *
 * Copyright (c) 2013, Guillaume Voirin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this
 *        list of conditions and the following disclaimer.
 *
 *     2. Redistributions in binary form must reproduce the above copyright notice,
 *        this list of conditions and the following disclaimer in the documentation
 *        and/or other materials provided with the distribution.
 *
 *     3. Neither the name of the copyright holder nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 23/06/15 22:02
 *
 * -------------------
 * Chameleon algorithm
 * -------------------
 *
 * Author(s)
 * Guillaume Voirin (https://github.com/gpnuma)
 *
 * Description
 * Hash based superfast kernel
 */

#include "chameleon_encode.h"

DENSITY_FORCE_INLINE void density_chameleon_encode_prepare_signature(uint8_t **restrict out, uint_fast64_t **restrict signature_pointer, uint_fast64_t *const restrict signature) {
    *signature = 0;
    *signature_pointer = (density_chameleon_signature *) *out;
    *out += sizeof(density_chameleon_signature);
}

DENSITY_FORCE_INLINE void density_chameleon_encode_kernel(uint8_t **restrict out, const uint16_t hash, const uint_fast8_t shift, uint_fast64_t *const restrict signature, density_chameleon_dictionary *const restrict dictionary, uint32_t *restrict unit) {
    density_chameleon_dictionary_entry *const found = &dictionary->entries[hash];

    switch (*unit ^ found->as_uint32_t) {
        case 0:
            *signature |= ((uint64_t) DENSITY_CHAMELEON_SIGNATURE_FLAG_MAP << shift);
            DENSITY_MEMCPY(*out, &hash, sizeof(uint16_t));
            *out += sizeof(uint16_t);
            break;
        default:
            found->as_uint32_t = *unit;
            DENSITY_MEMCPY(*out, unit, sizeof(uint32_t));
            *out += sizeof(uint32_t);
            break;
    }
}

DENSITY_FORCE_INLINE void density_chameleon_encode_4(const uint8_t **restrict in, uint8_t **restrict out, const uint_fast8_t shift, uint_fast64_t *const restrict signature, density_chameleon_dictionary *const restrict dictionary, uint32_t *restrict unit) {
    DENSITY_MEMCPY(unit, *in, sizeof(uint32_t));
    density_chameleon_encode_kernel(out, DENSITY_CHAMELEON_HASH_ALGORITHM(*unit), shift, signature, dictionary, unit);
    *in += sizeof(uint32_t);
}

DENSITY_FORCE_INLINE void density_chameleon_encode_256(const uint8_t **restrict in, uint8_t **restrict out, uint_fast64_t *const restrict signature, density_chameleon_dictionary *const restrict dictionary, uint32_t *restrict unit) {
    uint_fast8_t count = 0;

#ifdef __clang__
    for (uint_fast8_t count_b = 0; count_b < 32; count_b++) {
        DENSITY_UNROLL_2(density_chameleon_encode_4(in, out, count++, signature, dictionary, unit));
    }
#else
    for (uint_fast8_t count_b = 0; count_b < 16; count_b++) {
        DENSITY_UNROLL_4(density_chameleon_encode_4(in, out, count++, signature, dictionary, unit));
    }
#endif
}

DENSITY_WINDOWS_EXPORT DENSITY_FORCE_INLINE const density_algorithms_exit_status density_chameleon_encode(const uint8_t **restrict in, const uint_fast64_t in_size, uint8_t **restrict out, const uint_fast64_t out_size, const bool process_all) {
    density_chameleon_dictionary dictionary;
    density_chameleon_dictionary_reset(&dictionary);

    if (out_size < DENSITY_CHAMELEON_MAXIMUM_COMPRESSED_UNIT_SIZE)
        return DENSITY_ALGORITHMS_EXIT_STATUS_OUTPUT_STALL;

    uint32_t unit;
    density_chameleon_signature signature;
    density_chameleon_signature *signature_pointer;

    uint8_t *out_limit = *out + out_size - DENSITY_CHAMELEON_MAXIMUM_COMPRESSED_UNIT_SIZE;
    uint_fast64_t limit_256 = (in_size >> 8);
    while (density_likely(limit_256-- && *out <= out_limit)) {
        density_chameleon_encode_prepare_signature(out, &signature_pointer, &signature);
        __builtin_prefetch(*in + 256);
        density_chameleon_encode_256(in, out, &signature, &dictionary, &unit);
        DENSITY_MEMCPY(signature_pointer, &signature, sizeof(density_chameleon_signature));
    }

    if (*out > out_limit)
        return DENSITY_ALGORITHMS_EXIT_STATUS_OUTPUT_STALL;

    if (process_all) {
        uint_fast64_t remaining;

        switch (in_size & 0xff) {
            case 0:
            case 1:
            case 2:
            case 3:
                density_chameleon_encode_prepare_signature(out, &signature_pointer, &signature);
                signature |= ((uint64_t) DENSITY_CHAMELEON_SIGNATURE_FLAG_CHUNK);      // End marker
                DENSITY_MEMCPY(signature_pointer, &signature, sizeof(density_chameleon_signature));
                goto process_remaining_bytes;
            default:
                break;
        }

        const uint_fast64_t limit_4 = (in_size & 0xff) >> 2;
        density_chameleon_encode_prepare_signature(out, &signature_pointer, &signature);
        for (uint_fast8_t shift = 0; shift != limit_4; shift++)
            density_chameleon_encode_4(in, out, shift, &signature, &dictionary, &unit);

        signature |= ((uint64_t) DENSITY_CHAMELEON_SIGNATURE_FLAG_CHUNK << limit_4);   // End marker
        DENSITY_MEMCPY(signature_pointer, &signature, sizeof(density_chameleon_signature));

        process_remaining_bytes:
        remaining = in_size & 0x3;
        if (remaining) {
            DENSITY_MEMCPY(*out, *in, remaining);
            *in += remaining;
            *out += remaining;
        }
    }

    return DENSITY_ALGORITHMS_EXIT_STATUS_FINISHED;
}
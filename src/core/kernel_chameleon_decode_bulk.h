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
 * 23/06/15 21:49
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

#ifndef DENSITY_CHAMELEON_DECODE_BULK_H
#define DENSITY_CHAMELEON_DECODE_BULK_H

#include "kernel_chameleon_dictionary.h"

void density_chameleon_decode_bulk_process_compressed(const uint16_t, uint8_t **, density_chameleon_dictionary *const);

void density_chameleon_decode_bulk_process_uncompressed(const uint32_t, uint8_t **, density_chameleon_dictionary *const);

void density_chameleon_decode_bulk_kernel(const uint8_t **, uint8_t **, const density_bool, density_chameleon_dictionary *const);

const bool density_chameleon_decode_bulk_test_compressed(const uint_fast64_t, const uint_fast8_t);

void density_chameleon_decode_bulk_4(const uint8_t **, uint8_t **, const uint_fast64_t, const uint_fast8_t, density_chameleon_dictionary *const);

void density_chameleon_decode_bulk_256(const uint8_t **, uint8_t **, const uint_fast64_t, density_chameleon_dictionary *const);

void density_chameleon_decode_bulk_read_signature(const uint8_t **, density_chameleon_signature *);

const bool density_chameleon_decode_bulk_unrestricted(const uint8_t **, const uint_fast64_t, uint8_t **);

#endif
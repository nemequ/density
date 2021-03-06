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
 * 25/12/14 16:02
 */

#include "memory_location.h"

DENSITY_WINDOWS_EXPORT DENSITY_FORCE_INLINE density_memory_location *density_memory_location_allocate(void *(*mem_alloc)(size_t)) {
    density_memory_location *location = mem_alloc(sizeof(density_memory_location));

    return location;
}

DENSITY_WINDOWS_EXPORT DENSITY_FORCE_INLINE void density_memory_location_free(density_memory_location *location, void (*mem_free)(void *)) {
    mem_free(location);
}

DENSITY_WINDOWS_EXPORT DENSITY_FORCE_INLINE void density_memory_location_encapsulate(density_memory_location *DENSITY_RESTRICT location, density_byte *DENSITY_RESTRICT pointer, const uint_fast64_t bytes) {
    location->pointer = pointer;
    location->available_bytes = bytes;
    location->initial_available_bytes = bytes;
}

DENSITY_WINDOWS_EXPORT DENSITY_FORCE_INLINE uint_fast64_t density_memory_location_used(density_memory_location *location) {
    return location->initial_available_bytes - location->available_bytes;
}

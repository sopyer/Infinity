#pragma once

/*
**  External allocator based on Two Level Segregated Fit memory allocator
**  from http://tlsf.baisoku.org
*/

#include <core/core.h>

//Allocator is not threadsafe
//Supports spaces up to 2^30
//Supports arbitrary alignments up to 256
//Supports up to 65535 allocation max
//Uses 16 bytes per possible allocation

struct   etlsf_arena_data_t;
typedef  etlsf_arena_data_t*  etlsf_arena_t;

etlsf_arena_t  etlsf_create (mem::arena_t marena, uint32_t offset, uint32_t size, uint16_t max_allocs);
void           etlsf_destroy(etlsf_arena_t arena);

// All allocations are always aligned up to 256 bytes
uint16_t  etlsf_alloc  (etlsf_arena_t arena, uint32_t size);
void      etlsf_free   (etlsf_arena_t arena, uint16_t block);

/* Returns internal block size, not original request size */
uint32_t  etlsf_block_size  (etlsf_arena_t arena, uint16_t block);
uint32_t  etlsf_block_offset(etlsf_arena_t arena, uint16_t block);

int  etlsf_is_block_valid(etlsf_arena_t arena, uint16_t block);

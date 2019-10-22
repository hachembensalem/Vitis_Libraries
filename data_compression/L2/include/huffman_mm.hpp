/*
 * (c) Copyright 2019 Xilinx, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef _XFCOMPRESSION_HUFFMAN_KERNEL_HPP_
#define _XFCOMPRESSION_HUFFMAN_KERNEL_HPP_

/**
 * @file huffman_kernel.hpp
 * @brief Header for huffman kernel used in zlib compression.
 *
 * This file is part of Vitis Data Compression Library.
 */
// L1 modules
#include "lz_optional.hpp"
#include "stream_downsizer.hpp"
#include "mm2s.hpp"
#include "s2mm.hpp"
#include "stream_upsizer.hpp"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "hls_stream.h"
#include <ap_int.h>
#include "zlib_config.hpp"

#define MIN_BLOCK_SIZE 116

#define GMEM_DWIDTH 512
#define GMEM_BURST_SIZE 16

// DYNAMIC HUFFMAN Compress STATES
#define WRITE_TOKEN 0
#define ML_DIST_REP 1
#define LIT_REP 2
#define SEND_OUTPUT 3
#define ML_EXTRA 4
#define DIST_REP 5
#define DIST_EXTRA 6

// LZ specific Defines
#define BIT 8

#define d_code(dist, dist_code) ((dist) < 256 ? dist_code[dist] : dist_code[256 + ((dist) >> 7)])
/* Mapping from a distance to a distance code. dist is the distance - 1 and
 *  * must not have side effects. dist_code[256] and dist_code[257] are never
 *   * used.
 *    */

extern "C" {
/**
 * @brief Huffman kernel top function.
 * This is an initial version of Huffman Kernel which does block based bit
 * packing process. It uses dynamic huffman codes and bit lengths to encode the
 * LZ77 (Byte Compressed Data) output. This version operates on 1MB block data
 * per engine as this is suitable for use cases where raw data is over >100MB
 * and compression ratio is over 2.5x in order to achieve best throughput. This
 * can be further optimized to achieve better throughput for smaller file
 * usecase.
 *
 * @param in input stream
 * @param out output stream
 * @param in_block_size input block size
 * @param compressd_size output compressed size
 * @param dyn_litmtree_codes input literal and match length codes
 * @param dyn_distree_codes input distance codes
 * @param dyn_bitlentree_codes input bit-length codes
 * @param dyn_litmtree_blen input literal and match length bit length data
 * @param dyn_dtree_blen input distance bit length data
 * @param dyn_bitlentree_blen input bit-length of bit length data
 * @param dyn_max_codes input maximum codes
 * @param block_size_in_kb input block size in bytes
 * @param input_size input data size
 *
 */
void xilHuffmanKernel(xf::compression::uintMemWidth_t* in,
                      xf::compression::uintMemWidth_t* out,
                      uint32_t* in_block_size,
                      uint32_t* compressd_size,
                      uint32_t* dyn_litmtree_codes,
                      uint32_t* dyn_distree_codes,
                      uint32_t* dyn_bitlentree_codes,
                      uint32_t* dyn_litmtree_blen,
                      uint32_t* dyn_dtree_blen,
                      uint32_t* dyn_bitlentree_blen,
                      uint32_t* dyn_max_codes,
                      uint32_t block_size_in_kb,
                      uint32_t input_size);
}

#endif // _XFCOMPRESSION_HUFFMAN_KERNEL_HPP_
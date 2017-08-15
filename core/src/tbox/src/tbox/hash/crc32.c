/*!The Treasure Box Library
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Copyright (C) 2009 - 2017, TBOOX Open Source Group.
 *
 * @author      ruki
 * @file        crc32.c
 * @ingroup     hash
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "crc32.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * declaration
 */
#if defined(TB_ARCH_ARM) && !defined(TB_ARCH_ARM64)
tb_uint32_t tb_crc32_make_asm(tb_uint32_t crc32, tb_byte_t const* data, tb_size_t size, tb_uint32_t const table[]);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the crc32(IEEE) table
tb_uint32_t const g_crc32_table[] = 
{
    0x00000000, 0xb71dc104, 0x6e3b8209, 0xd926430d, 0xdc760413, 0x6b6bc517
,	0xb24d861a, 0x0550471e, 0xb8ed0826, 0x0ff0c922, 0xd6d68a2f, 0x61cb4b2b
,	0x649b0c35, 0xd386cd31, 0x0aa08e3c, 0xbdbd4f38, 0x70db114c, 0xc7c6d048
,	0x1ee09345, 0xa9fd5241, 0xacad155f, 0x1bb0d45b, 0xc2969756, 0x758b5652
,	0xc836196a, 0x7f2bd86e, 0xa60d9b63, 0x11105a67, 0x14401d79, 0xa35ddc7d
,	0x7a7b9f70, 0xcd665e74, 0xe0b62398, 0x57abe29c, 0x8e8da191, 0x39906095
,	0x3cc0278b, 0x8bdde68f, 0x52fba582, 0xe5e66486, 0x585b2bbe, 0xef46eaba
,	0x3660a9b7, 0x817d68b3, 0x842d2fad, 0x3330eea9, 0xea16ada4, 0x5d0b6ca0
,	0x906d32d4, 0x2770f3d0, 0xfe56b0dd, 0x494b71d9, 0x4c1b36c7, 0xfb06f7c3
,	0x2220b4ce, 0x953d75ca, 0x28803af2, 0x9f9dfbf6, 0x46bbb8fb, 0xf1a679ff
,	0xf4f63ee1, 0x43ebffe5, 0x9acdbce8, 0x2dd07dec, 0x77708634, 0xc06d4730
,	0x194b043d, 0xae56c539, 0xab068227, 0x1c1b4323, 0xc53d002e, 0x7220c12a
,	0xcf9d8e12, 0x78804f16, 0xa1a60c1b, 0x16bbcd1f, 0x13eb8a01, 0xa4f64b05
,	0x7dd00808, 0xcacdc90c, 0x07ab9778, 0xb0b6567c, 0x69901571, 0xde8dd475
,	0xdbdd936b, 0x6cc0526f, 0xb5e61162, 0x02fbd066, 0xbf469f5e, 0x085b5e5a
,	0xd17d1d57, 0x6660dc53, 0x63309b4d, 0xd42d5a49, 0x0d0b1944, 0xba16d840
,	0x97c6a5ac, 0x20db64a8, 0xf9fd27a5, 0x4ee0e6a1, 0x4bb0a1bf, 0xfcad60bb
,	0x258b23b6, 0x9296e2b2, 0x2f2bad8a, 0x98366c8e, 0x41102f83, 0xf60dee87
,	0xf35da999, 0x4440689d, 0x9d662b90, 0x2a7bea94, 0xe71db4e0, 0x500075e4
,	0x892636e9, 0x3e3bf7ed, 0x3b6bb0f3, 0x8c7671f7, 0x555032fa, 0xe24df3fe
,	0x5ff0bcc6, 0xe8ed7dc2, 0x31cb3ecf, 0x86d6ffcb, 0x8386b8d5, 0x349b79d1
,	0xedbd3adc, 0x5aa0fbd8, 0xeee00c69, 0x59fdcd6d, 0x80db8e60, 0x37c64f64
,	0x3296087a, 0x858bc97e, 0x5cad8a73, 0xebb04b77, 0x560d044f, 0xe110c54b
,	0x38368646, 0x8f2b4742, 0x8a7b005c, 0x3d66c158, 0xe4408255, 0x535d4351
,	0x9e3b1d25, 0x2926dc21, 0xf0009f2c, 0x471d5e28, 0x424d1936, 0xf550d832
,	0x2c769b3f, 0x9b6b5a3b, 0x26d61503, 0x91cbd407, 0x48ed970a, 0xfff0560e
,	0xfaa01110, 0x4dbdd014, 0x949b9319, 0x2386521d, 0x0e562ff1, 0xb94beef5
,	0x606dadf8, 0xd7706cfc, 0xd2202be2, 0x653deae6, 0xbc1ba9eb, 0x0b0668ef
,	0xb6bb27d7, 0x01a6e6d3, 0xd880a5de, 0x6f9d64da, 0x6acd23c4, 0xddd0e2c0
,	0x04f6a1cd, 0xb3eb60c9, 0x7e8d3ebd, 0xc990ffb9, 0x10b6bcb4, 0xa7ab7db0
,	0xa2fb3aae, 0x15e6fbaa, 0xccc0b8a7, 0x7bdd79a3, 0xc660369b, 0x717df79f
,	0xa85bb492, 0x1f467596, 0x1a163288, 0xad0bf38c, 0x742db081, 0xc3307185
,	0x99908a5d, 0x2e8d4b59, 0xf7ab0854, 0x40b6c950, 0x45e68e4e, 0xf2fb4f4a
,	0x2bdd0c47, 0x9cc0cd43, 0x217d827b, 0x9660437f, 0x4f460072, 0xf85bc176
,	0xfd0b8668, 0x4a16476c, 0x93300461, 0x242dc565, 0xe94b9b11, 0x5e565a15
,	0x87701918, 0x306dd81c, 0x353d9f02, 0x82205e06, 0x5b061d0b, 0xec1bdc0f
,	0x51a69337, 0xe6bb5233, 0x3f9d113e, 0x8880d03a, 0x8dd09724, 0x3acd5620
,	0xe3eb152d, 0x54f6d429, 0x7926a9c5, 0xce3b68c1, 0x171d2bcc, 0xa000eac8
,	0xa550add6, 0x124d6cd2, 0xcb6b2fdf, 0x7c76eedb, 0xc1cba1e3, 0x76d660e7
,	0xaff023ea, 0x18ede2ee, 0x1dbda5f0, 0xaaa064f4, 0x738627f9, 0xc49be6fd
,	0x09fdb889, 0xbee0798d, 0x67c63a80, 0xd0dbfb84, 0xd58bbc9a, 0x62967d9e
,	0xbbb03e93, 0x0cadff97, 0xb110b0af, 0x060d71ab, 0xdf2b32a6, 0x6836f3a2
,	0x6d66b4bc, 0xda7b75b8, 0x035d36b5, 0xb440f7b1
};

// the crc32(IEEE LE) table
tb_uint32_t const g_crc32_le_table[] = 
{
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f
,	0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988
,	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2
,	0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7
,	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9
,	0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172
,	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c
,	0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59
,	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423
,	0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924
,	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106
,	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433
,	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d
,	0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e
,	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950
,	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65
,	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7
,	0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0
,	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa
,	0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f
,	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81
,	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a
,	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84
,	0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1
,	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb
,	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc
,	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e
,	0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b
,	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55
,	0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236
,	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28
,	0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d
,	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f
,	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38
,	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242
,	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777
,	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69
,	0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2
,	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc
,	0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9
,	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693
,	0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94
,	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_uint32_t tb_crc32_make_impl(tb_uint32_t crc32, tb_byte_t const* data, tb_size_t size, tb_uint32_t const table[])
{
    // done
#if defined(TB_ARCH_ARM) && !defined(TB_ARCH_ARM64)
    crc32 = tb_crc32_make_asm(crc32, data, size, (tb_uint32_t const*)table);
#else
    tb_byte_t const*    ie = data + size;
    tb_uint32_t const*  pt = (tb_uint32_t const*)table;
    while (data < ie) crc32 = pt[((tb_uint8_t)crc32) ^ *data++] ^ (crc32 >> 8);
#endif

    // ok
    return crc32;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tb_uint32_t tb_crc32_make(tb_byte_t const* data, tb_size_t size, tb_uint32_t seed)
{
    // check
    tb_assert_and_check_return_val(data, 0);

    // calculate it
    return tb_crc32_make_impl(seed, data, size, g_crc32_table);
}
tb_uint32_t tb_crc32_make_from_cstr(tb_char_t const* cstr, tb_uint32_t seed)
{
    // check
    tb_assert_and_check_return_val(cstr, 0);

    // make it
    return tb_crc32_make((tb_byte_t const*)cstr, tb_strlen(cstr) + 1, seed);
}
tb_uint32_t tb_crc32_le_make(tb_byte_t const* data, tb_size_t size, tb_uint32_t seed)
{
    // check
    tb_assert_and_check_return_val(data, 0);

    // calculate it
    return tb_crc32_make_impl(seed, data, size, g_crc32_le_table);
}
tb_uint32_t tb_crc32_le_make_from_cstr(tb_char_t const* cstr, tb_uint32_t seed)
{
    // check
    tb_assert_and_check_return_val(cstr, 0);

    // make it
    return tb_crc32_le_make((tb_byte_t const*)cstr, tb_strlen(cstr) + 1, seed);
}

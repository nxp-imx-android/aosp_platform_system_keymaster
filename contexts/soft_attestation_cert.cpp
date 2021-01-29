/*
**
** Copyright 2017, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <keymaster/contexts/soft_attestation_cert.h>

#include <hardware/keymaster_defs.h>
#include <keymaster/android_keymaster_utils.h>
#include <stdint.h>

namespace keymaster {

namespace {

static const uint8_t kRsaAttestKey[] = {
    0x30, 0x82, 0x02, 0x5d, 0x02, 0x01, 0x00, 0x02, 0x81, 0x81, 0x00, 0xc0, 0x83, 0x23, 0xdc, 0x56,
    0x88, 0x1b, 0xb8, 0x30, 0x20, 0x69, 0xf5, 0xb0, 0x85, 0x61, 0xc6, 0xee, 0xbe, 0x7f, 0x05, 0xe2,
    0xf5, 0xa8, 0x42, 0x04, 0x8a, 0xbe, 0x8b, 0x47, 0xbe, 0x76, 0xfe, 0xae, 0xf2, 0x5c, 0xf2, 0x9b,
    0x2a, 0xfa, 0x32, 0x00, 0x14, 0x16, 0x01, 0x42, 0x99, 0x89, 0xa1, 0x5f, 0xcf, 0xc6, 0x81, 0x5e,
    0xb3, 0x63, 0x58, 0x3c, 0x2f, 0xd2, 0xf2, 0x0b, 0xe4, 0x98, 0x32, 0x83, 0xdd, 0x81, 0x4b, 0x16,
    0xd7, 0xe1, 0x85, 0x41, 0x7a, 0xe5, 0x4a, 0xbc, 0x29, 0x6a, 0x3a, 0x6d, 0xb5, 0xc0, 0x04, 0x08,
    0x3b, 0x68, 0xc5, 0x56, 0xc1, 0xf0, 0x23, 0x39, 0x91, 0x64, 0x19, 0x86, 0x4d, 0x50, 0xb7, 0x4d,
    0x40, 0xae, 0xca, 0x48, 0x4c, 0x77, 0x35, 0x6c, 0x89, 0x5a, 0x0c, 0x27, 0x5a, 0xbf, 0xac, 0x49,
    0x9d, 0x5d, 0x7d, 0x23, 0x62, 0xf2, 0x9c, 0x5e, 0x02, 0xe8, 0x71, 0x02, 0x03, 0x01, 0x00, 0x01,
    0x02, 0x81, 0x81, 0x00, 0xbe, 0x86, 0x0b, 0x0b, 0x99, 0xa8, 0x02, 0xa6, 0xfb, 0x1a, 0x59, 0x43,
    0x8a, 0x7b, 0xb7, 0x15, 0x06, 0x5b, 0x09, 0xa3, 0x6d, 0xc6, 0xe9, 0xca, 0xcc, 0x6b, 0xf3, 0xc0,
    0x2c, 0x34, 0xd7, 0xd7, 0x9e, 0x94, 0xc6, 0x60, 0x64, 0x28, 0xd8, 0x8c, 0x7b, 0x7f, 0x65, 0x77,
    0xc1, 0xcd, 0xea, 0x64, 0x07, 0x4a, 0xbe, 0x8e, 0x72, 0x86, 0xdf, 0x1f, 0x08, 0x11, 0xdc, 0x97,
    0x28, 0x26, 0x08, 0x68, 0xde, 0x95, 0xd3, 0x2e, 0xfc, 0x96, 0xb6, 0xd0, 0x84, 0xff, 0x27, 0x1a,
    0x5f, 0x60, 0xde, 0xfc, 0xc7, 0x03, 0xe7, 0xa3, 0x8e, 0x6e, 0x29, 0xba, 0x9a, 0x3c, 0x5f, 0xc2,
    0xc2, 0x80, 0x76, 0xb6, 0xa8, 0x96, 0xaf, 0x1d, 0x34, 0xd7, 0x88, 0x28, 0xce, 0x9b, 0xdd, 0xb1,
    0xf3, 0x4f, 0x9c, 0x94, 0x04, 0x43, 0x07, 0x81, 0x29, 0x8e, 0x20, 0x13, 0x16, 0x72, 0x5b, 0xbd,
    0xbc, 0x99, 0x3a, 0x41, 0x02, 0x41, 0x00, 0xe1, 0xc6, 0xd9, 0x27, 0x64, 0x6c, 0x09, 0x16, 0xec,
    0x36, 0x82, 0x6d, 0x59, 0x49, 0x83, 0x74, 0x0c, 0x21, 0xf1, 0xb0, 0x74, 0xc4, 0xa1, 0xa5, 0x98,
    0x67, 0xc6, 0x69, 0x79, 0x5c, 0x85, 0xd3, 0xdc, 0x46, 0x4c, 0x5b, 0x92, 0x9e, 0x94, 0xbf, 0xb3,
    0x4e, 0x0d, 0xcc, 0x50, 0x14, 0xb1, 0x0f, 0x13, 0x34, 0x1a, 0xb7, 0xfd, 0xd5, 0xf6, 0x04, 0x14,
    0xd2, 0xa3, 0x26, 0xca, 0xd4, 0x1c, 0xc5, 0x02, 0x41, 0x00, 0xda, 0x48, 0x59, 0x97, 0x78, 0x5c,
    0xd5, 0x63, 0x0f, 0xb0, 0xfd, 0x8c, 0x52, 0x54, 0xf9, 0x8e, 0x53, 0x8e, 0x18, 0x98, 0x3a, 0xae,
    0x9e, 0x6b, 0x7e, 0x6a, 0x5a, 0x7b, 0x5d, 0x34, 0x37, 0x55, 0xb9, 0x21, 0x8e, 0xbd, 0x40, 0x32,
    0x0d, 0x28, 0x38, 0x7d, 0x78, 0x9f, 0x76, 0xfa, 0x21, 0x8b, 0xcc, 0x2d, 0x8b, 0x68, 0xa5, 0xf6,
    0x41, 0x8f, 0xbb, 0xec, 0xa5, 0x17, 0x9a, 0xb3, 0xaf, 0xbd, 0x02, 0x40, 0x50, 0xfe, 0xfc, 0x32,
    0x64, 0x95, 0x59, 0x61, 0x6e, 0xd6, 0x53, 0x4e, 0x15, 0x45, 0x09, 0x32, 0x9d, 0x93, 0xa3, 0xd8,
    0x10, 0xdb, 0xe5, 0xbd, 0xb9, 0x82, 0x29, 0x2c, 0xf7, 0x8b, 0xd8, 0xba, 0xdb, 0x80, 0x20, 0xae,
    0x8d, 0x57, 0xf4, 0xb7, 0x1d, 0x05, 0x38, 0x6f, 0xfe, 0x9e, 0x9d, 0xb2, 0x71, 0xca, 0x34, 0x77,
    0xa3, 0x49, 0x99, 0xdb, 0x76, 0xf8, 0xe5, 0xec, 0xe9, 0xc0, 0xd4, 0x9d, 0x02, 0x40, 0x15, 0xb7,
    0x4c, 0xf2, 0x7c, 0xce, 0xff, 0x8b, 0xb3, 0x6b, 0xf0, 0x4d, 0x9d, 0x83, 0x46, 0xb0, 0x9a, 0x2f,
    0x70, 0xd2, 0xf4, 0x43, 0x9b, 0x0f, 0x26, 0xac, 0x7e, 0x03, 0xf7, 0xe9, 0xd1, 0xf7, 0x7d, 0x4b,
    0x91, 0x5f, 0xd2, 0x9b, 0x28, 0x23, 0xf0, 0x3a, 0xcb, 0x5d, 0x52, 0x00, 0xe0, 0x85, 0x7f, 0xf2,
    0xa8, 0x03, 0xe9, 0x3e, 0xee, 0x96, 0xd6, 0x23, 0x5c, 0xe9, 0x54, 0x42, 0xbc, 0x21, 0x02, 0x41,
    0x00, 0x90, 0xa7, 0x45, 0xda, 0x89, 0x70, 0xb2, 0xcd, 0x64, 0x96, 0x60, 0x32, 0x42, 0x28, 0xc5,
    0xf8, 0x28, 0x56, 0xff, 0xd6, 0x65, 0xba, 0x9a, 0x85, 0xc8, 0xd6, 0x0f, 0x1b, 0x8b, 0xee, 0x71,
    0x7e, 0xcd, 0x2c, 0x72, 0xea, 0xe0, 0x1d, 0xad, 0x86, 0xba, 0x76, 0x54, 0xd4, 0xcf, 0x45, 0xad,
    0xb5, 0xf1, 0xf2, 0xb3, 0x1d, 0x9f, 0x81, 0x22, 0xcf, 0xa5, 0xf1, 0xa5, 0x57, 0x0f, 0x9b, 0x2d,
    0x25,
};

static const keymaster_key_blob_t kRsaAttestKeyBlob = {(const uint8_t*)&kRsaAttestKey,
                                                       sizeof(kRsaAttestKey)};

static const uint8_t kRsaAttestCert[] = {
    0x30, 0x82, 0x02, 0xb6, 0x30, 0x82, 0x02, 0x1f, 0xa0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x02, 0x10,
    0x00, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00,
    0x30, 0x63, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31,
    0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c, 0x0a, 0x43, 0x61, 0x6c, 0x69, 0x66, 0x6f,
    0x72, 0x6e, 0x69, 0x61, 0x31, 0x16, 0x30, 0x14, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x0d, 0x4d,
    0x6f, 0x75, 0x6e, 0x74, 0x61, 0x69, 0x6e, 0x20, 0x56, 0x69, 0x65, 0x77, 0x31, 0x15, 0x30, 0x13,
    0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x0c, 0x47, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x2c, 0x20, 0x49,
    0x6e, 0x63, 0x2e, 0x31, 0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x07, 0x41, 0x6e,
    0x64, 0x72, 0x6f, 0x69, 0x64, 0x30, 0x1e, 0x17, 0x0d, 0x31, 0x36, 0x30, 0x31, 0x30, 0x34, 0x31,
    0x32, 0x34, 0x30, 0x35, 0x33, 0x5a, 0x17, 0x0d, 0x33, 0x35, 0x31, 0x32, 0x33, 0x30, 0x31, 0x32,
    0x34, 0x30, 0x35, 0x33, 0x5a, 0x30, 0x76, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06,
    0x13, 0x02, 0x55, 0x53, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c, 0x0a, 0x43,
    0x61, 0x6c, 0x69, 0x66, 0x6f, 0x72, 0x6e, 0x69, 0x61, 0x31, 0x15, 0x30, 0x13, 0x06, 0x03, 0x55,
    0x04, 0x0a, 0x0c, 0x0c, 0x47, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e,
    0x31, 0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x07, 0x41, 0x6e, 0x64, 0x72, 0x6f,
    0x69, 0x64, 0x31, 0x29, 0x30, 0x27, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x20, 0x41, 0x6e, 0x64,
    0x72, 0x6f, 0x69, 0x64, 0x20, 0x53, 0x6f, 0x66, 0x74, 0x77, 0x61, 0x72, 0x65, 0x20, 0x41, 0x74,
    0x74, 0x65, 0x73, 0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x4b, 0x65, 0x79, 0x30, 0x81, 0x9f,
    0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03,
    0x81, 0x8d, 0x00, 0x30, 0x81, 0x89, 0x02, 0x81, 0x81, 0x00, 0xc0, 0x83, 0x23, 0xdc, 0x56, 0x88,
    0x1b, 0xb8, 0x30, 0x20, 0x69, 0xf5, 0xb0, 0x85, 0x61, 0xc6, 0xee, 0xbe, 0x7f, 0x05, 0xe2, 0xf5,
    0xa8, 0x42, 0x04, 0x8a, 0xbe, 0x8b, 0x47, 0xbe, 0x76, 0xfe, 0xae, 0xf2, 0x5c, 0xf2, 0x9b, 0x2a,
    0xfa, 0x32, 0x00, 0x14, 0x16, 0x01, 0x42, 0x99, 0x89, 0xa1, 0x5f, 0xcf, 0xc6, 0x81, 0x5e, 0xb3,
    0x63, 0x58, 0x3c, 0x2f, 0xd2, 0xf2, 0x0b, 0xe4, 0x98, 0x32, 0x83, 0xdd, 0x81, 0x4b, 0x16, 0xd7,
    0xe1, 0x85, 0x41, 0x7a, 0xe5, 0x4a, 0xbc, 0x29, 0x6a, 0x3a, 0x6d, 0xb5, 0xc0, 0x04, 0x08, 0x3b,
    0x68, 0xc5, 0x56, 0xc1, 0xf0, 0x23, 0x39, 0x91, 0x64, 0x19, 0x86, 0x4d, 0x50, 0xb7, 0x4d, 0x40,
    0xae, 0xca, 0x48, 0x4c, 0x77, 0x35, 0x6c, 0x89, 0x5a, 0x0c, 0x27, 0x5a, 0xbf, 0xac, 0x49, 0x9d,
    0x5d, 0x7d, 0x23, 0x62, 0xf2, 0x9c, 0x5e, 0x02, 0xe8, 0x71, 0x02, 0x03, 0x01, 0x00, 0x01, 0xa3,
    0x66, 0x30, 0x64, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0xd4, 0x0c,
    0x10, 0x1b, 0xf8, 0xcd, 0x63, 0xb9, 0xf7, 0x39, 0x52, 0xb5, 0x0e, 0x13, 0x5c, 0xa6, 0xd7, 0x99,
    0x93, 0x86, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x29,
    0xfa, 0xf1, 0xac, 0xcc, 0x4d, 0xd2, 0x4c, 0x96, 0x40, 0x27, 0x75, 0xb6, 0xb0, 0xe9, 0x32, 0xe5,
    0x07, 0xfe, 0x2e, 0x30, 0x12, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x01, 0x01, 0xff, 0x04, 0x08, 0x30,
    0x06, 0x01, 0x01, 0xff, 0x02, 0x01, 0x00, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x1d, 0x0f, 0x01, 0x01,
    0xff, 0x04, 0x04, 0x03, 0x02, 0x02, 0x84, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7,
    0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x03, 0x81, 0x81, 0x00, 0x9e, 0x2d, 0x48, 0x5f, 0x8c, 0x67,
    0x33, 0xdc, 0x1a, 0x85, 0xad, 0x99, 0xd7, 0x50, 0x23, 0xea, 0x14, 0xec, 0x43, 0xb0, 0xe1, 0x9d,
    0xea, 0xc2, 0x23, 0x46, 0x1e, 0x72, 0xb5, 0x19, 0xdc, 0x60, 0x22, 0xe4, 0xa5, 0x68, 0x31, 0x6c,
    0x0b, 0x55, 0xc4, 0xe6, 0x9c, 0xa2, 0x2d, 0x9f, 0x3a, 0x4f, 0x93, 0x6b, 0x31, 0x8b, 0x16, 0x78,
    0x16, 0x0d, 0x88, 0xcb, 0xd9, 0x8b, 0xcc, 0x80, 0x9d, 0x84, 0xf0, 0xc2, 0x27, 0xe3, 0x6b, 0x38,
    0xf1, 0xfd, 0xd1, 0xe7, 0x17, 0x72, 0x31, 0x59, 0x35, 0x7d, 0x96, 0xf3, 0xc5, 0x7f, 0xab, 0x9d,
    0x8f, 0x96, 0x61, 0x26, 0x4f, 0xb2, 0xbe, 0x81, 0xbb, 0x0d, 0x49, 0x04, 0x22, 0x8a, 0xce, 0x9f,
    0xf7, 0xf5, 0x42, 0x2e, 0x25, 0x44, 0xfa, 0x21, 0x07, 0x12, 0x5a, 0x83, 0xb5, 0x55, 0xad, 0x18,
    0x82, 0xf8, 0x40, 0x14, 0x9b, 0x9c, 0x20, 0x63, 0x04, 0x7f,
};

static const uint8_t kRsaAttestRootCert[] = {
    0x30, 0x82, 0x02, 0xa7, 0x30, 0x82, 0x02, 0x10, 0xa0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x09, 0x00,
    0xff, 0x94, 0xd9, 0xdd, 0x9f, 0x07, 0xc8, 0x0c, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x30, 0x63, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55,
    0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c,
    0x0a, 0x43, 0x61, 0x6c, 0x69, 0x66, 0x6f, 0x72, 0x6e, 0x69, 0x61, 0x31, 0x16, 0x30, 0x14, 0x06,
    0x03, 0x55, 0x04, 0x07, 0x0c, 0x0d, 0x4d, 0x6f, 0x75, 0x6e, 0x74, 0x61, 0x69, 0x6e, 0x20, 0x56,
    0x69, 0x65, 0x77, 0x31, 0x15, 0x30, 0x13, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x0c, 0x47, 0x6f,
    0x6f, 0x67, 0x6c, 0x65, 0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x31, 0x10, 0x30, 0x0e, 0x06, 0x03,
    0x55, 0x04, 0x0b, 0x0c, 0x07, 0x41, 0x6e, 0x64, 0x72, 0x6f, 0x69, 0x64, 0x30, 0x1e, 0x17, 0x0d,
    0x31, 0x36, 0x30, 0x31, 0x30, 0x34, 0x31, 0x32, 0x33, 0x31, 0x30, 0x38, 0x5a, 0x17, 0x0d, 0x33,
    0x35, 0x31, 0x32, 0x33, 0x30, 0x31, 0x32, 0x33, 0x31, 0x30, 0x38, 0x5a, 0x30, 0x63, 0x31, 0x0b,
    0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x13, 0x30, 0x11, 0x06,
    0x03, 0x55, 0x04, 0x08, 0x0c, 0x0a, 0x43, 0x61, 0x6c, 0x69, 0x66, 0x6f, 0x72, 0x6e, 0x69, 0x61,
    0x31, 0x16, 0x30, 0x14, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x0d, 0x4d, 0x6f, 0x75, 0x6e, 0x74,
    0x61, 0x69, 0x6e, 0x20, 0x56, 0x69, 0x65, 0x77, 0x31, 0x15, 0x30, 0x13, 0x06, 0x03, 0x55, 0x04,
    0x0a, 0x0c, 0x0c, 0x47, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x31,
    0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x07, 0x41, 0x6e, 0x64, 0x72, 0x6f, 0x69,
    0x64, 0x30, 0x81, 0x9f, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01,
    0x01, 0x05, 0x00, 0x03, 0x81, 0x8d, 0x00, 0x30, 0x81, 0x89, 0x02, 0x81, 0x81, 0x00, 0xa2, 0x6b,
    0xad, 0xeb, 0x6e, 0x2e, 0x44, 0x61, 0xef, 0xd5, 0x0e, 0x82, 0xe6, 0xb7, 0x94, 0xd1, 0x75, 0x23,
    0x1f, 0x77, 0x9b, 0x63, 0x91, 0x63, 0xff, 0xf7, 0xaa, 0xff, 0x0b, 0x72, 0x47, 0x4e, 0xc0, 0x2c,
    0x43, 0xec, 0x33, 0x7c, 0xd7, 0xac, 0xed, 0x40, 0x3e, 0x8c, 0x28, 0xa0, 0x66, 0xd5, 0xf7, 0x87,
    0x0b, 0x33, 0x97, 0xde, 0x0e, 0xb8, 0x4e, 0x13, 0x40, 0xab, 0xaf, 0xa5, 0x27, 0xbf, 0x95, 0x69,
    0xa0, 0x31, 0xdb, 0x06, 0x52, 0x65, 0xf8, 0x44, 0x59, 0x57, 0x61, 0xf0, 0xbb, 0xf2, 0x17, 0x4b,
    0xb7, 0x41, 0x80, 0x64, 0xc0, 0x28, 0x0e, 0x8f, 0x52, 0x77, 0x8e, 0xdb, 0xd2, 0x47, 0xb6, 0x45,
    0xe9, 0x19, 0xc8, 0xe9, 0x8b, 0xc3, 0xdb, 0xc2, 0x91, 0x3f, 0xd7, 0xd7, 0x50, 0xc4, 0x1d, 0x35,
    0x66, 0xf9, 0x57, 0xe4, 0x97, 0x96, 0x0b, 0x09, 0xac, 0xce, 0x92, 0x35, 0x85, 0x9b, 0x02, 0x03,
    0x01, 0x00, 0x01, 0xa3, 0x63, 0x30, 0x61, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16,
    0x04, 0x14, 0x29, 0xfa, 0xf1, 0xac, 0xcc, 0x4d, 0xd2, 0x4c, 0x96, 0x40, 0x27, 0x75, 0xb6, 0xb0,
    0xe9, 0x32, 0xe5, 0x07, 0xfe, 0x2e, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04, 0x18, 0x30,
    0x16, 0x80, 0x14, 0x29, 0xfa, 0xf1, 0xac, 0xcc, 0x4d, 0xd2, 0x4c, 0x96, 0x40, 0x27, 0x75, 0xb6,
    0xb0, 0xe9, 0x32, 0xe5, 0x07, 0xfe, 0x2e, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x01, 0x01,
    0xff, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xff, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x1d, 0x0f, 0x01,
    0x01, 0xff, 0x04, 0x04, 0x03, 0x02, 0x02, 0x84, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x03, 0x81, 0x81, 0x00, 0x4f, 0x72, 0xf3, 0x36, 0x59,
    0x8d, 0x0e, 0xc1, 0xb9, 0x74, 0x5b, 0x31, 0x59, 0xf6, 0xf0, 0x8d, 0x25, 0x49, 0x30, 0x9e, 0xa3,
    0x1c, 0x1c, 0x29, 0xd2, 0x45, 0x2d, 0x20, 0xb9, 0x4d, 0x5f, 0x64, 0xb4, 0xe8, 0x80, 0xc7, 0x78,
    0x7a, 0x9c, 0x39, 0xde, 0xa8, 0xb3, 0xf5, 0xbf, 0x2f, 0x70, 0x5f, 0x47, 0x10, 0x5c, 0xc5, 0xe6,
    0xeb, 0x4d, 0x06, 0x99, 0x61, 0xd2, 0xae, 0x9a, 0x07, 0xff, 0xf7, 0x7c, 0xb8, 0xab, 0xeb, 0x9c,
    0x0f, 0x24, 0x07, 0x5e, 0xb1, 0x7f, 0xba, 0x79, 0x71, 0xfd, 0x4d, 0x5b, 0x9e, 0xdf, 0x14, 0xa9,
    0xfe, 0xdf, 0xed, 0x7c, 0xc0, 0x88, 0x5d, 0xf8, 0xdd, 0x9b, 0x64, 0x32, 0x56, 0xd5, 0x35, 0x9a,
    0xe2, 0x13, 0xf9, 0x8f, 0xce, 0xc1, 0x7c, 0xdc, 0xef, 0xa4, 0xaa, 0xb2, 0x55, 0xc3, 0x83, 0xa9,
    0x2e, 0xfb, 0x5c, 0xf6, 0x62, 0xf5, 0x27, 0x52, 0x17, 0xbe, 0x63,
};

static keymaster_blob_t kRsaAttestChainBlobs[] = {
    {(const uint8_t*)&kRsaAttestCert, sizeof(kRsaAttestCert)},
    {(const uint8_t*)&kRsaAttestRootCert, sizeof(kRsaAttestRootCert)}};

static const keymaster_cert_chain_t kRsaAttestChain = {(keymaster_blob_t*)&kRsaAttestChainBlobs, 2};

static const uint8_t kEcAttestKey[] = {
    0x30, 0x77, 0x02, 0x01, 0x01, 0x04, 0x20, 0x21, 0xe0, 0x86, 0x43, 0x2a, 0x15, 0x19, 0x84, 0x59,
    0xcf, 0x36, 0x3a, 0x50, 0xfc, 0x14, 0xc9, 0xda, 0xad, 0xf9, 0x35, 0xf5, 0x27, 0xc2, 0xdf, 0xd7,
    0x1e, 0x4d, 0x6d, 0xbc, 0x42, 0xe5, 0x44, 0xa0, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d,
    0x03, 0x01, 0x07, 0xa1, 0x44, 0x03, 0x42, 0x00, 0x04, 0xeb, 0x9e, 0x79, 0xf8, 0x42, 0x63, 0x59,
    0xac, 0xcb, 0x2a, 0x91, 0x4c, 0x89, 0x86, 0xcc, 0x70, 0xad, 0x90, 0x66, 0x93, 0x82, 0xa9, 0x73,
    0x26, 0x13, 0xfe, 0xac, 0xcb, 0xf8, 0x21, 0x27, 0x4c, 0x21, 0x74, 0x97, 0x4a, 0x2a, 0xfe, 0xa5,
    0xb9, 0x4d, 0x7f, 0x66, 0xd4, 0xe0, 0x65, 0x10, 0x66, 0x35, 0xbc, 0x53, 0xb7, 0xa0, 0xa3, 0xa6,
    0x71, 0x58, 0x3e, 0xdb, 0x3e, 0x11, 0xae, 0x10, 0x14,
};

static const keymaster_key_blob_t kEcAttestKeyBlob = {(const uint8_t*)&kEcAttestKey,
                                                      sizeof(kEcAttestKey)};

static const uint8_t kEcAttestCert[] = {
    0x30, 0x82, 0x02, 0x78, 0x30, 0x82, 0x02, 0x1e, 0xa0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x02, 0x10,
    0x01, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x30, 0x81, 0x98,
    0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x13, 0x30,
    0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c, 0x0a, 0x43, 0x61, 0x6c, 0x69, 0x66, 0x6f, 0x72, 0x6e,
    0x69, 0x61, 0x31, 0x16, 0x30, 0x14, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x0d, 0x4d, 0x6f, 0x75,
    0x6e, 0x74, 0x61, 0x69, 0x6e, 0x20, 0x56, 0x69, 0x65, 0x77, 0x31, 0x15, 0x30, 0x13, 0x06, 0x03,
    0x55, 0x04, 0x0a, 0x0c, 0x0c, 0x47, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x2c, 0x20, 0x49, 0x6e, 0x63,
    0x2e, 0x31, 0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x07, 0x41, 0x6e, 0x64, 0x72,
    0x6f, 0x69, 0x64, 0x31, 0x33, 0x30, 0x31, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x2a, 0x41, 0x6e,
    0x64, 0x72, 0x6f, 0x69, 0x64, 0x20, 0x4b, 0x65, 0x79, 0x73, 0x74, 0x6f, 0x72, 0x65, 0x20, 0x53,
    0x6f, 0x66, 0x74, 0x77, 0x61, 0x72, 0x65, 0x20, 0x41, 0x74, 0x74, 0x65, 0x73, 0x74, 0x61, 0x74,
    0x69, 0x6f, 0x6e, 0x20, 0x52, 0x6f, 0x6f, 0x74, 0x30, 0x1e, 0x17, 0x0d, 0x31, 0x36, 0x30, 0x31,
    0x31, 0x31, 0x30, 0x30, 0x34, 0x36, 0x30, 0x39, 0x5a, 0x17, 0x0d, 0x32, 0x36, 0x30, 0x31, 0x30,
    0x38, 0x30, 0x30, 0x34, 0x36, 0x30, 0x39, 0x5a, 0x30, 0x81, 0x88, 0x31, 0x0b, 0x30, 0x09, 0x06,
    0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04,
    0x08, 0x0c, 0x0a, 0x43, 0x61, 0x6c, 0x69, 0x66, 0x6f, 0x72, 0x6e, 0x69, 0x61, 0x31, 0x15, 0x30,
    0x13, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x0c, 0x47, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x2c, 0x20,
    0x49, 0x6e, 0x63, 0x2e, 0x31, 0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x07, 0x41,
    0x6e, 0x64, 0x72, 0x6f, 0x69, 0x64, 0x31, 0x3b, 0x30, 0x39, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c,
    0x32, 0x41, 0x6e, 0x64, 0x72, 0x6f, 0x69, 0x64, 0x20, 0x4b, 0x65, 0x79, 0x73, 0x74, 0x6f, 0x72,
    0x65, 0x20, 0x53, 0x6f, 0x66, 0x74, 0x77, 0x61, 0x72, 0x65, 0x20, 0x41, 0x74, 0x74, 0x65, 0x73,
    0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x49, 0x6e, 0x74, 0x65, 0x72, 0x6d, 0x65, 0x64, 0x69,
    0x61, 0x74, 0x65, 0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01,
    0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0xeb, 0x9e,
    0x79, 0xf8, 0x42, 0x63, 0x59, 0xac, 0xcb, 0x2a, 0x91, 0x4c, 0x89, 0x86, 0xcc, 0x70, 0xad, 0x90,
    0x66, 0x93, 0x82, 0xa9, 0x73, 0x26, 0x13, 0xfe, 0xac, 0xcb, 0xf8, 0x21, 0x27, 0x4c, 0x21, 0x74,
    0x97, 0x4a, 0x2a, 0xfe, 0xa5, 0xb9, 0x4d, 0x7f, 0x66, 0xd4, 0xe0, 0x65, 0x10, 0x66, 0x35, 0xbc,
    0x53, 0xb7, 0xa0, 0xa3, 0xa6, 0x71, 0x58, 0x3e, 0xdb, 0x3e, 0x11, 0xae, 0x10, 0x14, 0xa3, 0x66,
    0x30, 0x64, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0x3f, 0xfc, 0xac,
    0xd6, 0x1a, 0xb1, 0x3a, 0x9e, 0x81, 0x20, 0xb8, 0xd5, 0x25, 0x1c, 0xc5, 0x65, 0xbb, 0x1e, 0x91,
    0xa9, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0xc8, 0xad,
    0xe9, 0x77, 0x4c, 0x45, 0xc3, 0xa3, 0xcf, 0x0d, 0x16, 0x10, 0xe4, 0x79, 0x43, 0x3a, 0x21, 0x5a,
    0x30, 0xcf, 0x30, 0x12, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x01, 0x01, 0xff, 0x04, 0x08, 0x30, 0x06,
    0x01, 0x01, 0xff, 0x02, 0x01, 0x00, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x1d, 0x0f, 0x01, 0x01, 0xff,
    0x04, 0x04, 0x03, 0x02, 0x02, 0x84, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04,
    0x03, 0x02, 0x03, 0x48, 0x00, 0x30, 0x45, 0x02, 0x20, 0x4b, 0x8a, 0x9b, 0x7b, 0xee, 0x82, 0xbc,
    0xc0, 0x33, 0x87, 0xae, 0x2f, 0xc0, 0x89, 0x98, 0xb4, 0xdd, 0xc3, 0x8d, 0xab, 0x27, 0x2a, 0x45,
    0x9f, 0x69, 0x0c, 0xc7, 0xc3, 0x92, 0xd4, 0x0f, 0x8e, 0x02, 0x21, 0x00, 0xee, 0xda, 0x01, 0x5d,
    0xb6, 0xf4, 0x32, 0xe9, 0xd4, 0x84, 0x3b, 0x62, 0x4c, 0x94, 0x04, 0xef, 0x3a, 0x7c, 0xcc, 0xbd,
    0x5e, 0xfb, 0x22, 0xbb, 0xe7, 0xfe, 0xb9, 0x77, 0x3f, 0x59, 0x3f, 0xfb,
};

static const uint8_t kEcAttestRootCert[] = {
    0x30, 0x82, 0x02, 0x8b, 0x30, 0x82, 0x02, 0x32, 0xa0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x09, 0x00,
    0xa2, 0x05, 0x9e, 0xd1, 0x0e, 0x43, 0x5b, 0x57, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce,
    0x3d, 0x04, 0x03, 0x02, 0x30, 0x81, 0x98, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06,
    0x13, 0x02, 0x55, 0x53, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c, 0x0a, 0x43,
    0x61, 0x6c, 0x69, 0x66, 0x6f, 0x72, 0x6e, 0x69, 0x61, 0x31, 0x16, 0x30, 0x14, 0x06, 0x03, 0x55,
    0x04, 0x07, 0x0c, 0x0d, 0x4d, 0x6f, 0x75, 0x6e, 0x74, 0x61, 0x69, 0x6e, 0x20, 0x56, 0x69, 0x65,
    0x77, 0x31, 0x15, 0x30, 0x13, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x0c, 0x47, 0x6f, 0x6f, 0x67,
    0x6c, 0x65, 0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x31, 0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04,
    0x0b, 0x0c, 0x07, 0x41, 0x6e, 0x64, 0x72, 0x6f, 0x69, 0x64, 0x31, 0x33, 0x30, 0x31, 0x06, 0x03,
    0x55, 0x04, 0x03, 0x0c, 0x2a, 0x41, 0x6e, 0x64, 0x72, 0x6f, 0x69, 0x64, 0x20, 0x4b, 0x65, 0x79,
    0x73, 0x74, 0x6f, 0x72, 0x65, 0x20, 0x53, 0x6f, 0x66, 0x74, 0x77, 0x61, 0x72, 0x65, 0x20, 0x41,
    0x74, 0x74, 0x65, 0x73, 0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x52, 0x6f, 0x6f, 0x74, 0x30,
    0x1e, 0x17, 0x0d, 0x31, 0x36, 0x30, 0x31, 0x31, 0x31, 0x30, 0x30, 0x34, 0x33, 0x35, 0x30, 0x5a,
    0x17, 0x0d, 0x33, 0x36, 0x30, 0x31, 0x30, 0x36, 0x30, 0x30, 0x34, 0x33, 0x35, 0x30, 0x5a, 0x30,
    0x81, 0x98, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31,
    0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c, 0x0a, 0x43, 0x61, 0x6c, 0x69, 0x66, 0x6f,
    0x72, 0x6e, 0x69, 0x61, 0x31, 0x16, 0x30, 0x14, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x0d, 0x4d,
    0x6f, 0x75, 0x6e, 0x74, 0x61, 0x69, 0x6e, 0x20, 0x56, 0x69, 0x65, 0x77, 0x31, 0x15, 0x30, 0x13,
    0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x0c, 0x47, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x2c, 0x20, 0x49,
    0x6e, 0x63, 0x2e, 0x31, 0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x07, 0x41, 0x6e,
    0x64, 0x72, 0x6f, 0x69, 0x64, 0x31, 0x33, 0x30, 0x31, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x2a,
    0x41, 0x6e, 0x64, 0x72, 0x6f, 0x69, 0x64, 0x20, 0x4b, 0x65, 0x79, 0x73, 0x74, 0x6f, 0x72, 0x65,
    0x20, 0x53, 0x6f, 0x66, 0x74, 0x77, 0x61, 0x72, 0x65, 0x20, 0x41, 0x74, 0x74, 0x65, 0x73, 0x74,
    0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x52, 0x6f, 0x6f, 0x74, 0x30, 0x59, 0x30, 0x13, 0x06, 0x07,
    0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01,
    0x07, 0x03, 0x42, 0x00, 0x04, 0xee, 0x5d, 0x5e, 0xc7, 0xe1, 0xc0, 0xdb, 0x6d, 0x03, 0xa6, 0x7e,
    0xe6, 0xb6, 0x1b, 0xec, 0x4d, 0x6a, 0x5d, 0x6a, 0x68, 0x2e, 0x0f, 0xff, 0x7f, 0x49, 0x0e, 0x7d,
    0x77, 0x1f, 0x44, 0x22, 0x6d, 0xbd, 0xb1, 0xaf, 0xfa, 0x16, 0xcb, 0xc7, 0xad, 0xc5, 0x77, 0xd2,
    0x56, 0x9c, 0xaa, 0xb7, 0xb0, 0x2d, 0x54, 0x01, 0x5d, 0x3e, 0x43, 0x2b, 0x2a, 0x8e, 0xd7, 0x4e,
    0xec, 0x48, 0x75, 0x41, 0xa4, 0xa3, 0x63, 0x30, 0x61, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e,
    0x04, 0x16, 0x04, 0x14, 0xc8, 0xad, 0xe9, 0x77, 0x4c, 0x45, 0xc3, 0xa3, 0xcf, 0x0d, 0x16, 0x10,
    0xe4, 0x79, 0x43, 0x3a, 0x21, 0x5a, 0x30, 0xcf, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04,
    0x18, 0x30, 0x16, 0x80, 0x14, 0xc8, 0xad, 0xe9, 0x77, 0x4c, 0x45, 0xc3, 0xa3, 0xcf, 0x0d, 0x16,
    0x10, 0xe4, 0x79, 0x43, 0x3a, 0x21, 0x5a, 0x30, 0xcf, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x1d, 0x13,
    0x01, 0x01, 0xff, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xff, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x1d,
    0x0f, 0x01, 0x01, 0xff, 0x04, 0x04, 0x03, 0x02, 0x02, 0x84, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86,
    0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x03, 0x47, 0x00, 0x30, 0x44, 0x02, 0x20, 0x35, 0x21, 0xa3,
    0xef, 0x8b, 0x34, 0x46, 0x1e, 0x9c, 0xd5, 0x60, 0xf3, 0x1d, 0x58, 0x89, 0x20, 0x6a, 0xdc, 0xa3,
    0x65, 0x41, 0xf6, 0x0d, 0x9e, 0xce, 0x8a, 0x19, 0x8c, 0x66, 0x48, 0x60, 0x7b, 0x02, 0x20, 0x4d,
    0x0b, 0xf3, 0x51, 0xd9, 0x30, 0x7c, 0x7d, 0x5b, 0xda, 0x35, 0x34, 0x1d, 0xa8, 0x47, 0x1b, 0x63,
    0xa5, 0x85, 0x65, 0x3c, 0xad, 0x4f, 0x24, 0xa7, 0xe7, 0x4d, 0xaf, 0x41, 0x7d, 0xf1, 0xbf,
};

static keymaster_blob_t kEcAttestChainBlobs[] = {
    {(const uint8_t*)&kEcAttestCert, sizeof(kEcAttestCert)},
    {(const uint8_t*)&kEcAttestRootCert, sizeof(kEcAttestRootCert)}};

static const keymaster_cert_chain_t kEcAttestChain = {(keymaster_blob_t*)&kEcAttestChainBlobs, 2};

}  // namespace

const keymaster_key_blob_t* getAttestationKey(keymaster_algorithm_t algorithm,
                                              keymaster_error_t* error) {

    if (error) *error = KM_ERROR_OK;

    switch (algorithm) {
    case KM_ALGORITHM_RSA:
        return &kRsaAttestKeyBlob;

    case KM_ALGORITHM_EC:
        return &kEcAttestKeyBlob;

    default:
        if (error) *error = KM_ERROR_UNSUPPORTED_ALGORITHM;
        return nullptr;
    }
}

CertificateChain getAttestationChain(keymaster_algorithm_t algorithm, keymaster_error_t* error) {
    if (error) *error = KM_ERROR_OK;

    CertificateChain retval;

    switch (algorithm) {
    case KM_ALGORITHM_RSA:
        retval = CertificateChain::clone(kRsaAttestChain);
        if (!retval.entries) *error = KM_ERROR_MEMORY_ALLOCATION_FAILED;
        break;
    case KM_ALGORITHM_EC:
        retval = CertificateChain::clone(kEcAttestChain);
        if (!retval.entries) *error = KM_ERROR_MEMORY_ALLOCATION_FAILED;
        break;
    default:
        if (error) *error = KM_ERROR_UNSUPPORTED_ALGORITHM;
        break;
    }

    return retval;
}

}  // namespace keymaster

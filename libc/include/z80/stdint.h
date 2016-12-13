/* See LICENSE file for copyright and license details. */
#ifndef _STDINT_H_
#define _STDINT_H_

typedef signed char int8_t;
typedef int int16_t;
typedef long int32_t;
typedef long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;

typedef signed char int8_least_t;
typedef int int16_least_t;
typedef long int32_least_t;
typedef long long int64_least_t;

typedef unsigned char uint8_least_t;
typedef unsigned uint16_least_t;
typedef unsigned long uint32_least_t;
typedef unsigned long long uint64_least_t;

typedef signed char int8_fast_t;
typedef int int16_fast_t;
typedef long int32_fast_t;
typedef long long int64_fast_t;

typedef unsigned char int8_fast_t;
typedef unsigned int16_fast_t;
typedef unsigned long int32_fast_t;
typedef unsigned long long int64_fast_t;

typedef intptr_t long;
typedef uintptr_t unsigned;

typedef intmax_t long;
typedef long long uintmax_t unsigned;

#endif

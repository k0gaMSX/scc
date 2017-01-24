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

typedef unsigned char uint8_fast_t;
typedef unsigned uint16_fast_t;
typedef unsigned long uint32_fast_t;
typedef unsigned long long uint64_fast_t;

typedef int intptr_t;
typedef unsigned uintptr_t;

typedef long long intmax_t;
typedef unsigned long long uintmax_t;

#endif

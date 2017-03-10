/* See LICENSE file for copyright and license details. */

#define INT8_MIN   (-128)
#define INT8_MAX   127
#define UINT8_MAX  255

#define INT16_MIN  (-32768)
#define INT16_MAX  32767
#define UINT16_MAX 65535

#define INT32_MIN  (-2147483648)
#define INT32_MAX  2147483647
#define UINT32_MAX 4294967295U

#define INT64_MIN  (-9223372036854775808)
#define INT64_MAX  9223372036854775807
#define UINT64_MAX 18446744073709551615U

#define INT_LEAST8_MIN   (-128)
#define INT_LEAST8_MAX   127
#define UINT_LEAST8_MAX  255

#define INT_LEAST16_MIN  (-32768)
#define INT_LEAST16_MAX  32767
#define UINT_LEAST16_MAX 65535

#define INT_LEAST32_MIN  (-2147483648)
#define INT_LEAST32_MAX  2147483647
#define UINT_LEAST32_MAX 4294967295U

#define INT_LEAST64_MIN  (-9223372036854775808)
#define INT_LEAST64_MAX  9223372036854775807
#define UINT_LEAST64_MAX 18446744073709551615U

#define INT_FAST8_MIN   (-2147483648)
#define INT_FAST8_MAX   2147483647
#define UINT_FAST8_MAX  4294967295U

#define INT_FAST16_MIN  (-2147483648)
#define INT_FAST16_MAX  2147483647
#define UINT_FAST16_MAX 4294967295U

#define INT_FAST32_MIN  (-2147483648)
#define INT_FAST32_MAX  2147483647
#define UINT_FAST32_MAX 4294967295U

#define INT_FAST64_MIN  (-9223372036854775808)
#define INT_FAST64_MAX  9223372036854775807
#define UINT_FAST64_MAX 18446744073709551615U

#define INTPTR_MIN  (-9223372036854775808)
#define INTPTR_MAX  9223372036854775807
#define UINTPTR_MAX 18446744073709551615U

#define INTMAX_MIN  (-9223372036854775808)
#define INTMAX_MAX  9223372036854775807
#define UINTMAX_MAX 18446744073709551615U

#define PTRDIFF_MIN (-9223372036854775808)
#define PTRDIFF_MAX 9223372036854775807

#define SIG_ATOMIC_MIN (-2147483648)
#define SIG_ATOMIC_MAX 2147483647

#define SIZE_MAX 18446744073709551615U

#define WCHAR_MIN (-2147483648)
#define WCHAR_MAX 2147483647

#define INT8_C(x)  x
#define INT16_C(x) x
#define INT32_C(x) x
#define INT64_C(x) x ## LL

#define UINT8_C(x)  x
#define UINT16_C(x) x
#define UINT32_C(x) x ## U
#define UINT64_C(x) x ## ULL

#define INTMAX_C(x)  x ## LL
#define UINTMAX_C(x) x ## ULL

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned uint32_t;
typedef unsigned long long uint64_t;

typedef signed char int8_least_t;
typedef short int16_least_t;
typedef int int32_least_t;
typedef long long int64_least_t;

typedef unsigned char uint8_least_t;
typedef unsigned short uint16_least_t;
typedef unsigned uint32_least_t;
typedef unsigned long long uint64_least_t;

typedef int int8_fast_t;
typedef int int16_fast_t;
typedef int int32_fast_t;
typedef long long int64_fast_t;

typedef unsigned uint8_fast_t;
typedef unsigned uint16_fast_t;
typedef unsigned uint32_fast_t;
typedef unsigned long long uint64_fast_t;

typedef intptr_t long;
typedef long long uintptr_t unsigned;

typedef intmax_t long;
typedef long long uintmax_t unsigned;


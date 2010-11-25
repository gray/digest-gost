/* byte_order.h */
#ifndef BYTE_ORDER_H
#define BYTE_ORDER_H
#include <stdint.h>
#include <stdlib.h>

#ifdef IN_RHASH
#include "config.h"
#endif

#if _MSC_VER > 1000
#include "platform-dependent.h"
#endif

#ifdef __GLIBC__
# include <endian.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(i386) || defined(__i386__) || defined(__i486__) || \
    defined(__i586__) || defined(__i686__) || defined(_M_IX86) || \
    defined(__pentium__) || defined(__pentiumpro__) || \
    defined(__k6__) || defined(__k8__) || defined(__athlon__) || \
    defined(__amd64) || defined(__amd64__) || defined(__x86_64) || defined(__x86_64__) || \
    defined(__pentium4__) || defined(__nocona__) || \
    defined(prescott) || defined(__core2__) || \
    defined(_M_AMD64) || defined(_M_IA64) || defined(_M_X64)
#if defined(_LP64) || defined(__LP64__) || defined(__x86_64) || defined(__x86_64__)
# define CPU_X64
#else
# define CPU_IA32
#endif
#endif

#if (defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && \
     __BYTE_ORDER == __LITTLE_ENDIAN) || \
    defined(CPU_IA32) || defined(CPU_X64) || \
    defined(__ia64) || defined(__ia64__) || defined(__alpha__) || defined(_M_ALPHA) || \
    defined(vax) || defined(MIPSEL) || defined(_ARM_)
# define CPU_LITTLE_ENDIAN
#elif (defined(__BYTE_ORDER) && defined(__BIG_ENDIAN) && \
       __BYTE_ORDER == __BIG_ENDIAN) || \
    defined(__sparc) || defined(__sparc__) || defined(sparc) || \
    defined(_POWER) || defined(__powerpc__) || defined(POWERPC) || \
    defined(__ppc__) || defined(__hpux) || \
    defined(_MIPSEB) || defined(__s390__) || \
    defined(mc68000) || defined(sel)
# define CPU_BIG_ENDIAN
#else
# error "Can't detect CPU architechture"
#endif

#ifdef CPU_BIG_ENDIAN
# define IS_BIG_ENDIAN 1
# define IS_LITTLE_ENDIAN 0
#else
# define IS_BIG_ENDIAN 0
# define IS_LITTLE_ENDIAN 1
#endif

#define IS_ALIGNED_32(p) (0 == (3 & ((const char*)(p) - (const char*)0)))
#define IS_ALIGNED_64(p) (0 == (7 & ((const char*)(p) - (const char*)0)))

#if defined(_MSC_VER) || defined(__BORLANDC__)
#define I64(x) x##ui64
#else
#define I64(x) x##LL
#endif

/* convert a hash flag to index */
#if __GNUC__ >= 4 || (__GNUC__ ==3 && __GNUC_MINOR__ >= 4) /* GCC < 3.4 */
# define get_ctz(x) __builtin_ctz(x)
#else
unsigned get_ctz(unsigned); /* define as function */
#endif

void u32_swap_copy(void* to, const void* from, size_t length);
void u64_swap_copy(void* to, int index, const void* from, size_t length);
void u32_memswap(unsigned *p, int length_in_u32);

/* define bswap_32 */
#if defined(__GNUC__) && defined(CPU_IA32) && !defined(__i386__)
  /* for intel x86 CPU */
  static inline uint32_t bswap_32(uint32_t x) {
    __asm("bswap\t%0":
        "=r" (x)     :
        "0" (x));
    return x;
  }
#elif defined(__GNUC__) && !defined(__STRICT_ANSI__) && (__GNUC__ >= 4) && (__GNUC__ > 4 || __GNUC_MINOR__ >= 3)
  /* for GCC >= 4.3 */
# define bswap_32(x) __builtin_bswap32(x)
#elif (_MSC_VER > 1300) && (defined(CPU_IA32) || defined(CPU_X64)) /* MS VC */
# define bswap_32(x) _byteswap_ulong((unsigned long)x)
#else
  /* general bswap_32 definition */
  static inline uint32_t bswap_32(uint32_t x) {
    x= ((x<<8)&0xFF00FF00) | ((x>>8)&0x00FF00FF);
    return (x>>16) | (x<<16);
  }
#endif /* bswap_32 */

#if defined(__GNUC__) && !defined(__STRICT_ANSI__) && (__GNUC__ >= 4) && (__GNUC__ > 4 || __GNUC_MINOR__ >= 3)
# define bswap_64(x) __builtin_bswap64(x)
#elif (_MSC_VER > 1300) && (defined(CPU_IA32) || defined(CPU_X64)) /* MS VC */
# define bswap_64(x) _byteswap_uint64((__int64)x)
#else
  static inline uint64_t bswap_64(uint64_t x) {
    union {
        uint64_t ll;
        uint32_t l[2];
    } w, r;
    w.ll = x;
    r.l[0] = bswap_32(w.l[1]);
    r.l[1] = bswap_32(w.l[0]);
    return r.ll;
  }
#endif

#ifdef CPU_BIG_ENDIAN
# define be2me_32(x) (x)
# define be2me_64(x) (x)
# define le2me_32(x) bswap_32(x)
# define le2me_64(x) bswap_64(x)

# define be32_copy memcpy
# define le32_copy u32_swap_copy
# define be64_copy(to, index, from, length) memcpy(to + index, from, length)
# define le64_copy(to, index, from, length) u64_swap_copy(to, index, from, length)
#else /* CPU_BIG_ENDIAN */
# define be2me_32(x) bswap_32(x)
# define be2me_64(x) bswap_64(x)
# define le2me_32(x) (x)
# define le2me_64(x) (x)

# define be32_copy u32_swap_copy
# define le32_copy memcpy
# define be64_copy(to, index, from, length) u64_swap_copy(to, index, from, length)
# define le64_copy(to, index, from, length) memcpy(to + index, from, length)
#endif /* CPU_BIG_ENDIAN */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* BYTE_ORDER_H */

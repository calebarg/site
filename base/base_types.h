//
// base_types.h
//
// Caleb Barger
// 02/09/24
//

#ifndef BASE_TYPES_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define internal static
#define global static
#define local_persist static

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;

typedef S8 B8;
typedef S16 B16;
typedef S32 B32;
typedef S64 B64;

typedef float F32;
typedef double F64;

#define KB(n) (n * 1024ul)
#define MB(n) (KB(n) * 1024ul)
#define GB(n) (MB(n) * 1024ul)

#define Max(A, B) ((A)>(B)?(A):(B))
#define Min(A, B) ((A)<(B)?(A):(B))
#define ClampTop(A,X) Min(A,X)
#define ClampBot(X,B) Max(X,B)
#define Clamp(A,X,B) (((X)<(A))?(A):((X)>(B))?(B):(X))

#define AlignPow2(a, b) ((a + b - 1) & ~(b - 1))
#define AlignDownPow2(x,b) ((x)&(~((b) - 1)))
#define Swap(T,a,b) T temp=(a),a=(b),b=temp
#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))
#define Memcpy(dest, source, count) memcpy((dest), (source), (count))
#define ZeroStruct(p, s) memset((p), 0, sizeof(s))
#define DeferLoop(begin, end) for (S32 _i_=((begin), 0); !_i_; (_i_=1, (end)))

#define Stringize2(x) #x
#define Stringize(x) Stringize2(x)

// Assertions

#define AssertBreak(m) (*((volatile U32*)0) = 0xCA1EB)
#define AssertMessage(m) AssertBreak(m)
#define Assert(cond) if (!(cond)) AssertMessage("Assertion failed")

#define InvalidPath AssertMessage("Invalid path")
#define InvalidCase default: InvalidPath

// Basic types

typedef struct Slice Slice;
struct Slice
{
  U8* ptr;
  U64 len;
};

#define CompletePreviousReadsBeforeFutureReads asm volatile("" ::: "memory")
#define CompletePreviousWritesBeforeFutureWrites asm volatile("" ::: "memory")

#define SLLStackPushN(h, n, next) (((h)==0) ? (h = n) : (n->next = h, h = n))
#define SLLStackPush(h, n) SLLStackPushN(h, n, next)
#define SLLStackPopN(h, next) ((h)?(h = (h)->next):0)
#define SLLStackPop(h) SLLStackPopN(h, next)

#define SSLQueuePushN(h, t, n, next) \
  (((h)==0) ? ((h) = (n)) : (((t)==0) ? ((h)->next = t = n) : (t->next = n, t = n)))
#define SLLQueuePush(h, t, n) SSLQueuePushN(h, t, n, next)

#define BASE_TYPES_H
#endif

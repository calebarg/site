//
// base_math.h
//
// Caleb Barger
// 09/22/24
//

#ifndef BASE_MATH_H

#include <math.h>

typedef struct Vec2_S32 Vec2_S32;
struct Vec2_S32
{
  S32 x;
  S32 y;
};

typedef struct Vec2F32 Vec2F32;
struct Vec2F32
{
  F32 x;
  F32 y;
};
typedef Vec2F32 Vec2_F32; // alias TODO(calebarg): Remove this

typedef struct Vec3F32 Vec3F32;
struct Vec3F32
{
  F32 x;
  F32 y;
  F32 z;
};
typedef Vec3F32 Vec3_F32; // ditto

typedef struct Vec4_F32 Vec4_F32;
struct Vec4_F32
{
  F32 x;
  F32 y;
  F32 z;
  F32 w;
};

typedef struct Mat3_S32 Mat3_S32;
struct Mat3_S32
{
  S32 m0, m3, m6;
  S32 m1, m4, m7;
  S32 m2, m5, m8;
};

typedef union Mat3_F32 Mat3_F32;
union Mat3_F32
{
  struct
  {
    F32 m0, m3, m6;
    F32 m1, m4, m7;
    F32 m2, m5, m8;
  };
  F32 m[9];
};

typedef struct Mat4_F32 Mat4_F32;
struct Mat4_F32
{
  F32 m[4][4];
};

internal F32 sqrt_f32(F32 val);
internal F32 vec2_f32_inner(Vec2_F32 a, Vec2_F32 b);
internal Vec2_F32 vec2_f32_negate(Vec2_F32 v);
internal F32 vec2_f32_mag(Vec2_F32 v);
internal Vec2_F32 vec2_f32_add(Vec2_F32 a, Vec2_F32 b);

internal Vec2_F32 vec2_f32_sub(Vec2_F32 a, Vec2_F32 b);
internal Vec2_S32 vec2_s32_sub(Vec2_S32 a, Vec2_S32 b);

internal Vec2_F32 vec2_f32_mul(Vec2_F32 a, Vec2_F32 b);
internal B8 vec2_f32_eql(Vec2_F32 a, Vec2_F32 b);
internal Vec2_F32 vec2_f32_splat(F32 value);
internal B8 vec2_s32_eql(Vec2_S32 a, Vec2_S32 b);
internal Vec2_S32 vec2_s32_add(Vec2_S32 a, Vec2_S32 b);
internal Vec2_S32 vec2_s32_negate(Vec2_S32 v);

// Conversions
internal S32 round_f32_to_s32(F32 val);
internal Vec2_F32 vec2_s32_to_f32(Vec2_S32 v);
internal Vec2_S32 vec2_f32_to_s32(Vec2_F32 v);

#define BASE_PI 3.14159

#define BASE_MATH_H
#endif

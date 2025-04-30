//
// base_math.c
//
// Caleb Barger
// 07/27/24
//
// Math or something
//

internal F32 sqrt_f32(F32 val)
{
  return sqrtf(val);
}

internal S32 round_f32_to_s32(F32 val)
{
#if 0
  S32 truncd_val = (S32)val;
  F32 floating_part = val - (F32)truncd_val;
  F32 abs_floating_part = (truncd_val >= 0) ? floating_part : -1.0*floating_part;

  if (abs_floating_part >= 0.5)
  {
    return truncd_val += (truncd_val >= 0) ? 1 : -1;
  }
#else
  return (S32)roundf(val);
#endif
}

internal F32 vec2_f32_inner(Vec2_F32 a, Vec2_F32 b)
{
  return (a.x*b.x + a.y*b.y);
}

internal F32 vec3_f32_inner(Vec3_F32 a, Vec3_F32 b)
{
  return (a.x*b.x + a.y*b.y + a.z*b.z);
}

internal Vec3_F32 vec3_f32_cross(Vec3_F32 a, Vec3_F32 b)
{
  Vec3_F32 result;
  result.x = a.y*b.z - a.z*b.y;
  result.y = a.z*b.x - a.x*b.z;
  result.z = a.x*b.y - a.y*b.x;
  return result;
}

internal Vec2_F32 vec2_f32_negate(Vec2_F32 v)
{
  return ((Vec2_F32){-v.x, -v.y});
}

internal F32 vec2_f32_mag(Vec2_F32 v)
{
  return sqrt(v.x*v.x + v.y*v.y);
}

internal F32 vec3_f32_mag(Vec3_F32 v)
{
  return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

internal Vec2_F32 vec2_f32_add(Vec2_F32 a, Vec2_F32 b)
{
  Vec2_F32 result;
  result.x = a.x + b.x;
  result.y = a.y + b.y;
  return result;
}

internal Vec3_F32 vec3_f32_add(Vec3_F32 a, Vec3_F32 b)
{
  Vec3_F32 result;
  result.x = a.x + b.x;
  result.y = a.y + b.y;
  result.z = a.z + b.z;
  return result;
}

internal Vec3_F32 vec3_f32_mul(Vec3_F32 a, Vec3_F32 b)
{
  Vec3_F32 result;
  result.x = a.x * b.x;
  result.y = a.y * b.y;
  result.z = a.z * b.z;
  return result;
}

internal Vec3_F32 vec3_f32_splat(F32 v)
{
  Vec3_F32 result;
  result.x = v;
  result.y = v;
  result.z = v;
  return result;
}

internal Vec2_F32 vec2_f32_sub(Vec2_F32 a, Vec2_F32 b)
{
  Vec2_F32 result;
  result.x = a.x - b.x;
  result.y = a.y - b.y;
  return result;
}

internal Vec3_F32 vec3_f32_sub(Vec3_F32 a, Vec3_F32 b)
{
  Vec3_F32 result;
  result.x = a.x - b.x;
  result.y = a.y - b.y;
  result.z = a.z - b.z;
  return result;
}

internal Vec2_S32 vec2_s32_sub(Vec2_S32 a, Vec2_S32 b)
{
  Vec2_S32 result;
  result.x = a.x - b.x;
  result.y = a.y - b.y;
  return result;
}

internal Vec2_F32 vec2_f32_mul(Vec2_F32 a, Vec2_F32 b)
{
  Vec2_F32 result;
  result.x = a.x * b.x;
  result.y = a.y * b.y;
  return result;
}

internal B8 vec2_f32_eql(Vec2_F32 a, Vec2_F32 b)
{
  B8 result = 0;
  if ((a.x == b.x) && (a.y == b.y))
  {
    result = 1;
  }
  return result;
}

internal Vec2_F32 vec2_f32_splat(F32 value)
{
  return (Vec2_F32){value, value};
}

internal Vec2_S32 vec2_s32_splat(S32 value)
{
  return (Vec2_S32){value, value};
}

internal B8 vec2_s32_eql(Vec2_S32 a, Vec2_S32 b)
{
  B8 result = 0;
  if ((a.x == b.x) && (a.y == b.y))
  {
    result = 1;
  }
  return result;
}

internal Vec2_S32 vec2_s32_add(Vec2_S32 a, Vec2_S32 b)
{
  Vec2_S32 result;
  result.x = a.x + b.x;
  result.y = a.y + b.y;
  return result;
}

internal Vec2_S32 vec2_s32_negate(Vec2_S32 v)
{
  return ((Vec2_S32){-v.x, -v.y});
}

internal Vec2_F32 vec2_s32_to_f32(Vec2_S32 v)
{
  return (Vec2_F32){(F32)(v.x), (F32)(v.y)};
}

internal Vec2_S32 vec2_f32_to_s32(Vec2_F32 v)
{
  return (Vec2_S32){round_f32_to_s32(v.x), round_f32_to_s32(v.y)};
}

internal Vec4_F32 vec4_f32_from_vec3_f32(Vec3_F32 v)
{
  Vec4_F32 result;
  result.x = v.x;
  result.y = v.y;
  result.z = v.z;
  result.w = 1.0;
  return result;
}

internal Vec3_F32 vec3_f32_from_vec4_f32(Vec4_F32 v)
{
  Vec3_F32 result;
  result.x = v.x;
  result.y = v.y;
  result.z = v.z;
  return result;
}

// NOTE(calebarg): I'm cheating here, I really should just commit to Vec3's
// with a w component of 1 and then my Matrix Vector multiplies can be mathmatically
// correct. (Notice lack of multiplication on 3rd col)

internal Vec2_S32 mat3_s32_mul_vec2_s32(Mat3_S32 mat3, Vec2_S32 v)
{
  Vec2_S32 result = {0};

  result.x = mat3.m0*v.x + mat3.m3*v.y + mat3.m6;
  result.y = mat3.m1*v.x + mat3.m4*v.y + mat3.m7;

  return result;
}

internal Vec2_F32 mat3_f32_mul_vec2_f32(Mat3_F32 mat3, Vec2_F32 v)
{
  Vec2_F32 result = {0};

  result.x = mat3.m0*v.x + mat3.m3*v.y + mat3.m6;
  result.y = mat3.m1*v.x + mat3.m4*v.y + mat3.m7;

  return result;
}

internal Vec3_F32 mat3_f32_mul_vec3_f32(Mat3_F32 m, Vec3_F32 v)
{
  Vec3_F32 result = {0};

  result.x = m.m0*v.x + m.m3*v.y + m.m6*v.z;
  result.y = m.m1*v.x + m.m4*v.y + m.m7*v.z;
  result.z = m.m2*v.x + m.m5*v.y + m.m6*v.z;

  return result;
}

internal F32 rad_from_deg(F32 deg)
{
  F32 result = deg * (BASE_PI / 180.0);
  return result;
}

internal Vec2_F32 vec2_f32_scale(Vec2_F32 v, F32 s)
{
  v.x *= s;
  v.y *= s;
  return v;
}

internal Vec2_F32 vec2_f32_norm(Vec2_F32 v)
{
  F32 mag = vec2_f32_mag(v);
  v.x /= mag;
  v.y /= mag;
  return v;
}

internal Vec3_F32 vec3_f32_norm(Vec3_F32 v)
{
  F32 mag = vec3_f32_mag(v);
  v.x /= mag;
  v.y /= mag;
  v.z /= mag;
  return v;
}

internal Vec4_F32 mat4_f32_mul_vec4_f32(Mat4_F32 m, Vec4_F32 v)
{
  Vec4_F32 result;

  result.x = m.m[0][0]*v.x + m.m[0][1]*v.y + m.m[0][2]*v.z + m.m[0][3]*v.w;
  result.y = m.m[1][0]*v.x + m.m[1][1]*v.y + m.m[1][2]*v.z + m.m[1][3]*v.w;
  result.z = m.m[2][0]*v.x + m.m[2][1]*v.y + m.m[2][2]*v.z + m.m[2][3]*v.w;
  result.w = m.m[3][0]*v.x + m.m[3][1]*v.y + m.m[3][2]*v.z + m.m[3][3]*v.w;

  return result;
}

internal Mat4_F32 mat4_f32_mul(Mat4_F32 a, Mat4_F32 b)
{
  Mat4_F32 result = {{0}};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
    }
  }

  return result;
}

internal Mat4_F32 mat4_make_identity()
{
  Mat4_F32 m = {{
      { 1, 0, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 0, 1, 0 },
      { 0, 0, 0, 1 }
    }};
  return m;
}

internal Mat4_F32 mat4_make_rotx(F32 angle)
{
  F32 c = cos(angle);
  F32 s = sin(angle);
  // | 1  0  0  0 |
  // | 0  c -s  0 |
  // | 0  s  c  0 |
  // | 0  0  0  1 |
  Mat4_F32 m = mat4_make_identity();
  m.m[1][1] = c;
  m.m[1][2] = -s;
  m.m[2][1] = s;
  m.m[2][2] = c;
  return m;
}

internal Mat4_F32 mat4_make_roty(F32 angle)
{
  F32 c = cos(angle);
  F32 s = sin(angle);
  // |  c  0  s  0 |
  // |  0  1  0  0 |
  // | -s  0  c  0 |
  // |  0  0  0  1 |
  Mat4_F32 m = mat4_make_identity();
  m.m[0][0] = c;
  m.m[0][2] = s;
  m.m[2][0] = -s;
  m.m[2][2] = c;
  return m;
}

internal Mat4_F32 mat4_make_rotz(F32 angle)
{
  F32 c = cos(angle);
  F32 s = sin(angle);
  // | c -s  0  0 |
  // | s  c  0  0 |
  // | 0  0  1  0 |
  // | 0  0  0  1 |
  Mat4_F32 m = mat4_make_identity();
  m.m[0][0] = c;
  m.m[0][1] = -s;
  m.m[1][0] = s;
  m.m[1][1] = c;
  return m;
}

internal Mat4_F32 mat4_make_scale(F32 x, F32 y, F32 z)
{
  // | x  0  0  0 |
  // | s  y  0  0 |
  // | 0  0  z  0 |
  // | 0  0  0  1 |
  Mat4_F32 m = mat4_make_identity();
  m.m[0][0] = x;
  m.m[1][1] = y;
  m.m[2][2] = z;
  return m;
}

internal Mat4_F32 mat4_make_translate(F32 x, F32 y, F32 z)
{
  // | 1  0  0  x |
  // | 0  1  0  y |
  // | 0  0  1  z |
  // | 0  0  0  1 |
  Mat4_F32 m = mat4_make_identity();
  m.m[0][3] = x;
  m.m[1][3] = y;
  m.m[2][3] = z;
  return m;
}

internal Mat4_F32 mat4_f32_perspective(F32 aspect, F32 fov, F32 znear, F32 zfar)
{
  // | (h/w)*1/tan(fov/2)             0              0                 0 |
  // |                  0  1/tan(fov/2)              0                 0 |
  // |                  0             0     zf/(zf-zn)  (-zf*zn)/(zf-zn) |
  // |                  0             0              1                 0 |
  Mat4_F32 m = {{0}};
  m.m[0][0] = aspect * (1 / tan(fov / 2));
  m.m[1][1] = 1 / tan(fov / 2);
  m.m[3][2] = zfar / (zfar - znear);
  m.m[2][3] = (-zfar * znear) / (zfar - znear);
  m.m[3][2] = 1.0;
  return m;
}

internal Mat4_F32 mat4_f32_view(Vec3_F32 camera_p, Vec3_F32 up, Vec3_F32 forward)
{
  Vec3_F32 y = vec3_f32_norm(up);
  Vec3_F32 z = vec3_f32_norm(forward);
  Vec3_F32 x = vec3_f32_cross(up, forward);
  Mat4_F32 m = {{
      {x.x, x.y, x.z, -vec3_f32_inner(x, camera_p)},
      {y.x, y.y, y.z, -vec3_f32_inner(y, camera_p)},
      {z.x, z.y, z.z, -vec3_f32_inner(z, camera_p)},
      {0  , 0  , 0  ,  1}
    }};
  return m;
}

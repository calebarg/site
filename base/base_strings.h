//
// base_strings.h
//
// Caleb Barger
// 11/22/24
//

#ifndef BASE_STRINGS_H

#define Str8Lit(lit) ((String8){.ptr=(U8*)(lit), .len=sizeof(lit)-1})

typedef Slice String8;
internal S32 s32_from_str8(String8 str);
internal String8 str8_from_s32(Arena* arena, S32 num);

typedef struct String8Node String8Node;
struct String8Node
{
  String8 str;
  String8Node* next;
};

typedef struct String8List String8List;
struct String8List
{
  String8Node* first;
  String8Node* last;
};

#define BASE_STRINGS_H
#endif

//
// base_arena.h
//
// Caleb Barger
// 09/22/24
//

#ifndef BASE_ARENA_H

typedef struct Arena Arena;
struct Arena
{
  U8* base_ptr;
  U64 offset;
  U64 cap;
  B8 growable;
};

typedef struct Temp Temp;
struct Temp {
  Arena* arena;
  U64 offset;
};

#define ArenaPushArray(a, t, c) (t*)arena_push(a, sizeof(t)*c)

internal void* arena_push(Arena* arena, U64 size);
internal Arena* arena_alloc_sized(U64 size);
internal Arena* arena_alloc();
internal Arena* arena_from_memory(void* memory, U64 size);
internal Arena* arena_sub(Arena* arnea, U64 size);
internal void arena_release(Arena *arena);
internal Temp scratch_begin(Arena* arena);
internal void scratch_end(Temp temp);

#define BASE_ARENA_H
#endif

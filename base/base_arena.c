//
// Caleb Barger
// COPYWRIGHT NOTICE OR SOMETHING
//
// Simple arena allocator
//

internal void* arena_push_aligned(Arena* a, U64 size, U64 align)
{
  void* result = 0;
  size = AlignPow2(size, align);
  if (a->offset + size > a->cap)
  {
    AssertMessage("OUT OF SPACE");
  }
  result = a->base_ptr + a->offset;
  a->offset += size;
  return result;
}

internal void* arena_push(Arena* a, U64 size)
{
  return arena_push_aligned(a, size, 8);
}

internal Arena* arena_from_memory(void* memory, U64 size)
{
  size = AlignDownPow2(size, 8);
  Arena* arena = (Arena*)memory;
  arena->base_ptr = (U8*)memory;
  arena->offset = sizeof(Arena);
  arena->cap = size;
  arena->growable = 0;

  return arena;
}

internal Arena* arena_sub(Arena* parent_arena, U64 size)
{
  size = AlignPow2(size + sizeof(Arena), 8);
  void* memory = arena_push_aligned(parent_arena, size, 8);
  Arena* arena = (Arena*)memory;
  arena->base_ptr = (U8*)memory;
  arena->offset = sizeof(Arena);
  arena->cap = size;
  arena->growable = 0;

  return arena;
}

internal Temp scratch_begin(Arena* arena)
{
  Temp result;
  result.arena = arena;
  result.offset = arena->offset;
  return result;
}

internal void scratch_end(Temp temp)
{
  temp.arena->offset = temp.offset;
}


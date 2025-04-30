//
// base_strings.c
//
// Caleb Barger
// 12/19/2024
//

internal String8 str8_cat(Arena* arena, String8 a, String8 b)
{
  String8 result;
  result.ptr = (U8*)arena_push(arena, a.len + b.len);
  result.len = 0;

  for (U64 a_index = 0; a_index < a.len; ++a_index) result.ptr[result.len++] = a.ptr[a_index];
  for (U64 b_index = 0; b_index < b.len; ++b_index) result.ptr[result.len++] = b.ptr[b_index];

  return result;
}

internal String8 str8_catz(Arena* arena, String8 str)
{
  String8 result;
  result = str8_cat(arena, str, Str8Lit("\0"));
  return result;
}

internal String8 str8_from_mem(U8* str)
{
  String8 result;
  U64 s_len = 0;
  {
    U8* curr_ptr = str;
    for (; curr_ptr && (*curr_ptr != '\0'); ++curr_ptr)
    {
    }
    s_len = (U64)(curr_ptr - str);
  }
  result.ptr = str;
  result.len = s_len;
  return result;
}

internal String8 str8_sub(String8 str, U64 start, U64 end)
{
  String8 result;
  result.ptr = (U8*)str.ptr + start;
  result.len = end - start;
  return result;
}

internal String8 str8_sub_from_mem(U8* str, U64 start, U64 end)
{
  String8 result;
  result.ptr = (U8*)str + start;
  result.len = end - start;
  return result;
}

internal String8 str8_copy(Arena* arena, String8 source)
{
  String8 dest = {
    (U8*)arena_push(arena, source.len),
    source.len,
  };
  for (U64 source_index = 0; source_index < source.len; ++source_index)
    dest.ptr[source_index] = source.ptr[source_index];
  return dest;
}

internal B8 str8_eql(String8 a, String8 b)
{
  B8 result = 0;
  if (a.len == b.len)
  {
    result = (memcmp(a.ptr, b.ptr, a.len) == 0);
  }
  return result;
}

internal void str8_list_push(Arena* arena, String8List* list, String8 source_str)
{
  String8Node* node = ArenaPushArray(arena, String8Node, 1);
  node->str = str8_copy(arena, source_str);
  SLLQueuePush(list->first, list->last, node);
}

internal B32 str8_in_list(String8 needle, String8Node* first)
{
  B32 result = 0;
  for (String8Node* curr=first;
       curr != 0;
       curr=curr->next)
  {
    if (str8_eql(needle, curr->str))
    {
      result = 1;
      break;
    }
  }
  return result;
}

internal S32 s32_from_str8(String8 str)
{
  S32 result = 0;
  U32 place = 1;
  for (S32 curr_idx=str.len-1; curr_idx >= 0; --curr_idx)
  {
    U8 curr = str.ptr[curr_idx];
    switch(curr)
    {
      case '0'...'9':
      {
        S32 val = curr - '0';
        result += val * place;
        place *= 10;
      } break;
      case '+':
      {
      } break;
      case '-':
      {
        result *= -1;
      } break;
      default:
      {
        return 0;
      } break;
    }
  }
  return result;
}

internal String8 str8_from_s32(Arena* arena, S32 num)
{
  String8 result;
  if (num != 0)
  {
    B8 is_negative = (num < 0) ? (num *= -1, 1) : 0;
    U32 number_of_digits = 0;
    S32 place = 1;
    while((num / place) != 0)
    {
      number_of_digits++;
      place *= 10;
    }

    result.ptr = (U8*)arena_push(arena, number_of_digits + is_negative);
    result.len = number_of_digits + is_negative;

    U32 result_idx=0;
    if (is_negative)
    {
      result.ptr[result_idx++] = '-';
    }

    place /= 10; // Get place back to value that reflects the number of digits.
    while (place != 0)
    {
      result.ptr[result_idx++] = '0' + ((num / place) % 10);
      place /= 10;
    }
  }
  else // num == 0
  {
    result.ptr = (U8*)arena_push(arena, 1);
    result.len = 1;
    result.ptr[0] = '0';
  }
  return result;
}

global ThreadCTX* local_thread_ctx;

internal void equip_thread_ctx(ThreadCTX* thread_ctx)
{
  local_thread_ctx = thread_ctx;
}

internal Temp temp_begin(Arena** conflicts, U8 count)
{
  Temp result = (Temp){0, 0};

  if (!local_thread_ctx)
  {
    AssertMessage("No thread context initialized");
  }

  U8 scratch_idx = 0;
  for (; scratch_idx < count; ++scratch_idx)
  {
    if (conflicts[scratch_idx] != local_thread_ctx->arenas[scratch_idx])
    {
      break;
    }
  }
  if (scratch_idx < ArrayCount(local_thread_ctx->arenas))
  {
    result = scratch_begin(local_thread_ctx->arenas[scratch_idx]);
  }

  return result;
}

internal void temp_end(Temp temp)
{
  scratch_end(temp);
}

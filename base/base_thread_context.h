#ifndef BASE_THREAD_CONTEXT_H

typedef struct ThreadCTX ThreadCTX;
struct ThreadCTX
{
  Arena* arenas[2];
};

internal void thread_ctx_init(ThreadCTX* thread_ctx);
internal void equip_thread_ctx(ThreadCTX* thread_ctx);
internal Temp temp_begin(Arena** conflicts, U8 count);
internal void temp_end(Temp temp);

#define TempN(name, conflicts, count) Temp name;\
  DeferLoop(name=temp_begin(conflicts,count),temp_end(name))
#define Temp(conflicts, count) TempN(temp, conflicts, count)

#define BASE_THREAD_CONTEXT_H
#endif

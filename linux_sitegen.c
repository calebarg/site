//
// linux_sitegen.c
//
// 04/28/2025
// Caleb Barger
//
// Static site generator
//

#include "base/base_inc.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <dirent.h>

#include "base/base_inc.c"

typedef U32 FileKind;
enum
{
  FileKind_None = 0,
  FileKind_Dir,
  FileKind_Reg,
};

typedef struct FileInfo FileInfo;
struct FileInfo
{
  FileKind kind;
  U64 size;
};

typedef struct LoadedFile LoadedFile;
struct LoadedFile
{
  U8* ptr;
  U64 offset;
  U64 size;
};

global char* raw_head_html =
  "<head>"
  "  <meta charset=\"UTF-8\">"
  "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
  "  <meta name=\"description\" content=\"\">"
  "  <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">"
  "  <meta http-equiv=\"Pragma\" content=\"no-cache\">"
  "  <title>calebarg.net</title>"
  "  <link rel=\"icon\" href=\"./favicon.ico\" type=\"image/x-icon\">"
  "  <link rel=\"stylesheet\" href=\"/style.css\">"
  "</head>";

global char* raw_navbar_html =
  "<nav class=\"navbar\">"
  "  <a href=\"/index.html\">Home</a>"
  "  <a href=\"/projects.html\">Projects</a>"
  "  <a href=\"/posts.html\">Posts</a>"
  "  <a href=\"https://github.com/calebarg\">Github</a>"
  "  <a href=\"https://twitter.com/calebbarger20\">X Profile</a>"
  "  <a href=\"https://www.youtube.com/@calebarg02\">Youtube Channel</a>"
  "  <a href=\"/resume.html\">Resume</a>"
  "  <a href=\"/tetris.html\">Tetris</a>"
  "</nav>";

internal FileInfo
file_info_from_path(char* path)
{
  FileInfo result = {0};

  struct stat sb = {0};
  S32 stat_result_code = stat(path, &sb);
  if (stat_result_code == 0)
  {
    switch(sb.st_mode & __S_IFMT)
    {
      case __S_IFREG: result.kind = FileKind_Reg; break;
      case __S_IFDIR: result.kind = FileKind_Dir; break;
      default: break;
    }
    result.size = sb.st_size;
  }

  return result;
}

internal FileInfo
file_info_from_path_str8(String8 path)
{
  FileInfo result = {0};
  Temp(0, 0)
  {
    char* path_cstr = (char*)str8_catz(temp.arena, path).ptr;
    result = file_info_from_path(path_cstr);
  }
  return result;
}

internal LoadedFile
make_loaded_file(Arena* arena, U64 size)
{
  LoadedFile result = {0};
  result.ptr = ArenaPushArray(arena, U8, size);
  result.size = size;
  result.offset = 0;
  return result;
}

internal LoadedFile
read_entire_file(Arena* arena, String8 path)
{
  LoadedFile result = {0};
  Temp(&arena, 1)
  {
    char* path_cstr = (char*)str8_catz(temp.arena, path).ptr;
    FILE* file = fopen(path_cstr, "r");
    if (file)
    {
      FileInfo file_info = file_info_from_path(path_cstr);
      result = make_loaded_file(arena, file_info.size);
      fread(result.ptr, 1, file_info.size, file);
      if (fclose(file) != 0)
      {
        InvalidPath;
      }
    }
  }
  return result;
}

internal void
write_entire_file(LoadedFile loaded_file, String8 path)
{
  Temp(0, 0)
  {
    char* path_cstr = (char*)str8_catz(temp.arena, path).ptr;
    FILE* file = fopen(path_cstr, "w");
    if (!file)
    {
      InvalidPath;
    }
    if (fwrite(loaded_file.ptr, 1, loaded_file.offset, file) != loaded_file.offset)
    {
      InvalidPath;
    }
    if (fclose(file) != 0)
    {
      InvalidPath;
    }
  }
}

internal void
loaded_file_write_str8(LoadedFile* file, String8 str)
{
  if ((file->offset + str.len) > file->size)
  {
    InvalidPath;
  }
  memcpy(file->ptr + file->offset, str.ptr, str.len);
  file->offset += str.len;
}

internal void
write_top_html_chunk(LoadedFile* file)
{
  loaded_file_write_str8(file, Str8Lit("<!DOCTYPE html>\n<html lang=\"en\">\n"));
  loaded_file_write_str8(file, str8_from_mem(raw_head_html));
  loaded_file_write_str8(file, Str8Lit("<body>\n<div id=\"navbar-container\">\n"));
  loaded_file_write_str8(file, str8_from_mem(raw_navbar_html));
  loaded_file_write_str8(file, Str8Lit("</div>"));
}

internal void
write_bottom_html_chunk(LoadedFile* file)
{
  loaded_file_write_str8(file, Str8Lit("</body>\n"));
  loaded_file_write_str8(file, Str8Lit("</html>\n"));
}

internal String8
str8_basename(String8 str)
{
  String8 result = {0};
  result.ptr = str.ptr;
  result.len = 0;

  for (U64 byte_idx=0;
       byte_idx < str.len;
       ++byte_idx)
  {
    if (str.ptr[byte_idx] == '.')
    {
      break;
    }
    result.len++;
  }

  return result;
}

internal void
write_template_files(String8 in_path, String8 out_path)
{
  TempN(temp0, 0, 0)
  {
    char* dir_path_cstr = (char*)str8_catz(temp0.arena, in_path).ptr;
    DIR* dir = opendir(dir_path_cstr);
    if (!dir)
    {
      InvalidPath;
    }
    for (struct dirent* dir_entry = readdir(dir);
         dir_entry != 0;
         dir_entry = readdir(dir))
    {
      String8 dir_entry_str = str8_from_mem(dir_entry->d_name);
      String8 in_file_path_rel =
        str8_cat(temp0.arena, in_path, Str8Lit("/"));
      in_file_path_rel =
        str8_cat(temp0.arena, in_file_path_rel, dir_entry_str);

      String8 out_file_path_rel =
        str8_cat(temp0.arena, out_path, Str8Lit("/"));
      out_file_path_rel =
        str8_cat(temp0.arena, out_file_path_rel, dir_entry_str);

      if (dir_entry->d_type == 8)
      {
        TempN(temp1, 0, 0)
        {
          LoadedFile template_contents =
            read_entire_file(temp1.arena, in_file_path_rel);

          LoadedFile html_page =
            make_loaded_file(temp1.arena, KB(50));
          write_top_html_chunk(&html_page);
          loaded_file_write_str8(&html_page, Str8Lit("<div class=\"template-container\">\n"));
          loaded_file_write_str8(&html_page, (String8){template_contents.ptr, template_contents.size});
          loaded_file_write_str8(&html_page, Str8Lit("</div>\n"));
          write_bottom_html_chunk(&html_page);

          write_entire_file(html_page, out_file_path_rel);
        }
      }
      else if ((dir_entry->d_type == 4) &&
               (!str8_eql(dir_entry_str, Str8Lit(".")) &&
                !str8_eql(dir_entry_str, Str8Lit(".."))))
      {
        FileInfo file_info = file_info_from_path_str8(out_file_path_rel);
        if (file_info.kind == FileKind_None)
        {
          char* out_dir_path_cstr = (char*)str8_catz(temp0.arena, out_file_path_rel).ptr;
          if (mkdir(out_dir_path_cstr, 0777) == -1)
          {
            InvalidPath;
          }
        }
        write_template_files(in_file_path_rel, out_file_path_rel);
      }
    }
    if (closedir(dir) != 0)
    {
      InvalidPath;
    }
  }
}

int main()
{
  U64 platform_memory_size = MB(3);
  void* platform_memory = mmap(0, platform_memory_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if (!platform_memory)
  {
    AssertMessage("failed to map platform memory");
  }
  Arena* linux_arena = arena_from_memory(platform_memory, platform_memory_size);
  ThreadCTX thread_ctx = {0};
  thread_ctx.arenas[0] = arena_sub(linux_arena, MB(1));
  thread_ctx.arenas[1] = arena_sub(linux_arena, MB(1));
  equip_thread_ctx(&thread_ctx);

  String8 templates_path_rel = Str8Lit("./templates");
  String8 posts_path_rel =
    str8_cat(linux_arena, templates_path_rel, Str8Lit("/posts"));
  String8 public_path_rel = Str8Lit("./public");

  if (file_info_from_path((char*)templates_path_rel.ptr).kind != FileKind_Dir ||
      file_info_from_path((char*)posts_path_rel.ptr).kind != FileKind_Dir ||
      file_info_from_path((char*)public_path_rel.ptr).kind != FileKind_Dir)
  {
    AssertMessage("missing a required directory.");
  }

  ////////////////////////////////
  //~calebarg: posts.html

  TempN(temp0, 0, 0)
  {
    LoadedFile posts_html_file_contents =
      make_loaded_file(temp0.arena, KB(50));

    DIR* posts_dir = opendir((char*)posts_path_rel.ptr);
    if (!posts_dir)
    {
      InvalidPath;
    }

    write_top_html_chunk(&posts_html_file_contents);
    loaded_file_write_str8(&posts_html_file_contents, Str8Lit("<div class=\"template-container\">\n<ul id=\"posts\">\n"));

    for (struct dirent* dir_entry = readdir(posts_dir);
         dir_entry != 0;
         dir_entry = readdir(posts_dir))
    {
      if (dir_entry->d_type == 8) // NOTE(calebarg): 8 is a regular file...
      {
        String8 entry_str = str8_from_mem(dir_entry->d_name);

        loaded_file_write_str8(&posts_html_file_contents, Str8Lit("<li><a href=\"/posts/"));
        loaded_file_write_str8(&posts_html_file_contents, entry_str);
        loaded_file_write_str8(&posts_html_file_contents, Str8Lit("\">"));
        loaded_file_write_str8(&posts_html_file_contents, str8_basename(entry_str));
        loaded_file_write_str8(&posts_html_file_contents, Str8Lit("</li>\n"));
      }
    }
    loaded_file_write_str8(&posts_html_file_contents, Str8Lit("</ul>\n</div>\n"));
    write_bottom_html_chunk(&posts_html_file_contents);

    if (closedir(posts_dir) != 0)
    {
      InvalidPath;
    }

    String8 public_posts_html_path =
      str8_cat(linux_arena, public_path_rel, Str8Lit("/posts.html"));
    write_entire_file(posts_html_file_contents, public_posts_html_path);
  }

  ////////////////////////////////
  //~calebarg: index.html

  TempN(temp1, 0, 0)
  {
    LoadedFile index_html_file_contents =
      make_loaded_file(temp1.arena, KB(50));
    String8 index_path_rel =
      str8_cat(linux_arena, public_path_rel, Str8Lit("/index.html"));

    write_top_html_chunk(&index_html_file_contents);
    loaded_file_write_str8(&index_html_file_contents, Str8Lit("<div class=\"template-container\">\n"));
    loaded_file_write_str8(&index_html_file_contents, Str8Lit("<h3>[Latest post]</h3>\n"));

    LoadedFile latest_post_contents = {0};
    {
      String8 latest_post_name = {0};
      U64 latest_year_month_day[3] = {0};

      DIR* posts_dir = opendir((char*)posts_path_rel.ptr);
      if (!posts_dir)
      {
        InvalidPath;
      }
      for (struct dirent* dir_entry = readdir(posts_dir);
           dir_entry != 0;
           dir_entry = readdir(posts_dir))
      {
        if (dir_entry->d_type == 8) // NOTE(calebarg): 8 is a regular file...
        {
          String8 entry_str = str8_from_mem(dir_entry->d_name);
          String8 entry_str_basename = str8_basename(entry_str);

          U64 year_month_day[3] = {0};
          U64 year_month_day_idx = 0;
          U64 last_dash_idx = 0;
          for (U64 byte_idx=0;
               byte_idx < entry_str_basename.len;
               ++byte_idx)
          {
            B32 is_dash = (byte_idx + 1 >= entry_str_basename.len);
            if ((entry_str_basename.ptr[byte_idx] == '-') ||
                is_dash)
            {
              if (last_dash_idx > 0)
              {
                U64 end_idx = (is_dash) ? byte_idx + 1: byte_idx;
                String8 substr = str8_sub(entry_str_basename, last_dash_idx + 1, end_idx);
                U64 substr_int = (U64)s32_from_str8(substr);
                year_month_day[year_month_day_idx & 0x3] = substr_int;
                year_month_day_idx++;
              }
              last_dash_idx = byte_idx;
            }
          }
          B32 is_latest_post = 0;
          if (year_month_day[0] > latest_year_month_day[0])
          {
            is_latest_post = 1;
          }
          else if (year_month_day[0] == latest_year_month_day[0])
          {
            if (year_month_day[1] > latest_year_month_day[1])
            {
              is_latest_post = 1;
            }
            else if (year_month_day[1] == latest_year_month_day[1])
            {
              if (year_month_day[2] > latest_year_month_day[2])
              {
                is_latest_post = 1;
              }
            }
          }
          if (is_latest_post)
          {
            memcpy(latest_year_month_day, year_month_day, sizeof(U64)*ArrayCount(year_month_day));
            latest_post_name = str8_copy(temp1.arena, entry_str);
          }
        }
      }
      if (closedir(posts_dir) != 0)
      {
        InvalidPath;
      }
      String8 latest_post_path_rel = str8_cat(temp1.arena, posts_path_rel, Str8Lit("/"));
      latest_post_path_rel = str8_cat(temp1.arena, latest_post_path_rel, latest_post_name);
      latest_post_contents = read_entire_file(temp1.arena, latest_post_path_rel);
    }

    loaded_file_write_str8(&index_html_file_contents, (String8){latest_post_contents.ptr, latest_post_contents.size});
    loaded_file_write_str8(&index_html_file_contents, Str8Lit("</div>"));
    write_bottom_html_chunk(&index_html_file_contents);

    write_entire_file(index_html_file_contents, index_path_rel);
  }

  write_template_files(templates_path_rel, public_path_rel);

  return 0;
}

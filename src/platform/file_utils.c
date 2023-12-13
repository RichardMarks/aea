//
// Created by Richard Marks on 12/12/23.
//

#include <stdio.h>
#include <stdlib.h>

#include "file_utils.h"

AEA_str AEA_LoadEntireFile(AEA_cstr filename) {
  FILE *fp = fopen(filename, "rb");
  if (!fp)
  {
    fprintf(stderr, "failed to load file %s\n", filename);
    return NULL;
  }
  fseek(fp, 0, SEEK_END);
  AEA_size file_size = ftell(fp);
  rewind(fp);
  AEA_size content_size = (1 + file_size);
  AEA_str content = (AEA_str)malloc(content_size);
  if (!content) {
    fprintf(stderr, "failed to allocate memory (%zu) to hold file %s\n", file_size, filename);
    fclose(fp);
    return NULL;
  }
  AEA_size bytes_read = fread(content, 1, file_size, fp);
  if (bytes_read != file_size)
  {
    fprintf(stderr, "failed to read file %s\n", filename);
    free(content);
    fclose(fp);
    return NULL;
  }
  fclose(fp);
  content[file_size] = '\0';
  fprintf(stdout, "loaded file %s with %zu bytes\n", filename, file_size);
  return content;
}

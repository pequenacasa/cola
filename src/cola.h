
// MIT License

// Copyright (c) 2024 Antânio Ivo da Silva Gomes

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILEOPENMODE_READ "r"
#define FILEOPENMODE_WRITE "w"

typedef struct {
  char *content;
  unsigned char content_loaded;
} COLA_Content;

typedef struct {
  COLA_Content *contents;
  unsigned int content_count;
} COLA_Context;

inline COLA_Context *COLA_CreateContext(const unsigned int content_count) {
  COLA_Context *ctx = (COLA_Context *)malloc(sizeof(COLA_Context));

  if (ctx == NULL) {
    // TODO: warning
    return ctx;
  }

  ctx->contents = (COLA_Content *)malloc(sizeof(COLA_Content) * content_count);
  if (ctx == NULL) {
    // TODO: warning
    return ctx;
  }

  for (int i = 0; i < content_count; i++) {
    ctx->contents[i].content_loaded = 0;
  }

  ctx->content_count = content_count;

  return ctx;
}

inline void COLA_DestroyContext(COLA_Context *ctx) {
  for (int i = 0; i < ctx->content_count; i++) {
    if (ctx->contents[i].content_loaded) {
      free(ctx->contents[i].content);
    }
  }

  free(ctx->contents);
  free(ctx);
}

inline int COLA_LoadFileContent(COLA_Context *ctx, const char *filepath,
                                const int index) {
  COLA_Content *c = (ctx->contents + index);
  FILE *f = fopen(filepath, FILEOPENMODE_READ);
  if (f == NULL) {
    // TODO: warning
    return 1;
  }

  unsigned int size;

  fseek(f, 0, SEEK_END);
  size = ftell(f);
  rewind(f);

  char *buffer = (char *)malloc(size);
  if (buffer == NULL) {
    // TODO: warning
    fclose(f);
    return 1;
  }

  fread(buffer, 1, size, f);

  c->content = buffer;
  c->content_loaded = 1;

  fclose(f);
  return 0;
}

inline int COLA_LoadTextContent(COLA_Context *ctx, const char *content,
                                const int index) {
  COLA_Content *c = (ctx->contents + index);
  char *buffer;
  int size = sizeof(content);

  if (c->content_loaded) {
    // TODO: warning
    return 1;
  }

  buffer = (char *)malloc(size);
  if (buffer == NULL) {
    // TODO: warning
    return 1;
  }

  strcpy(buffer, content);
  c->content = buffer;
  c->content_loaded = 1;

  return 0;
}

inline int COLA_UnloadContent(COLA_Context *ctx, const int index) {
  COLA_Content *c = (ctx->contents + index);

  if (!c->content_loaded) {
    return 1;
  }

  free(c->content);
  c->content_loaded = 0;

  return 0;
}

inline int COLA_BuildFile(COLA_Context *ctx, const char *filepath) {
  FILE *f = fopen(filepath, "w");
  if (f == NULL) {
    // TODO: warning
    return 1;
  }

  int buffer_empty = 0;
  for (int i = 0; i < ctx->content_count; i++) {
    if (!ctx->contents[i].content_loaded) {
      buffer_empty = i;
      continue;
    }

    fprintf(f, "%s", ctx->contents[i].content);
  }

  if (buffer_empty > 0) {
    // TODO: warning
    fclose(f);
    return 1;
  }

  fclose(f);
  return 0;
}

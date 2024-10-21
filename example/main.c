#include "../src/cola.h"

int main() {
  COLA_Context *ctx = COLA_CreateContext(3);

  COLA_LoadFileContent(ctx, "a.txt", 0);
  COLA_LoadTextContent(ctx, "--", 1);
  COLA_LoadFileContent(ctx, "b.txt", 2);

  COLA_BuildFile(ctx, "fizzbuzz.txt");

  COLA_DestroyContext(ctx);
  return 0;
}

#include "mtp.h"

#include <limits.h>

void mtp_time_to_buffer(mtp_t *t, uint8_t *store){
  store[0] = (t->tod >> 24) & 0xFF;
  store[1] = (t->tod >> 16) & 0xFF;
  store[2] = (t->tod >> 8) & 0xFF;
  store[3] = (t->tod >> 0) & 0xFF;
  store[4] = (t->frac >> 24) & 0xFF;
  store[5] = (t->frac >> 16) & 0xFF;
  store[6] = (t->frac >> 8) & 0xFF;
  store[7] = (t->frac >> 0) & 0xFF;
}

void mtp_buffer_to_time(uint8_t *store, mtp_t *t){
  t->tod = (store[0] << 24) | (store[1] << 16) | (store[2] << 8) | (store[3]);
  t->frac = (store[4] << 24) | (store[5] << 16) | (store[6] << 8) | (store[7]);
}

float mtp_to_float(mtp_t *t){
  float n = t->tod;
  n = n + ((float)t->frac / (float)(ULONG_MAX));
  return n;
}

double mtp_to_number(mtp_t *t){
  double n = t->tod;
  n = n + ((double)t->frac / (double)(ULONG_MAX));
  return n;
}

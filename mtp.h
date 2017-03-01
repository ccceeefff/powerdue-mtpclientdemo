#ifndef MTP_H
#define MTP_H

#include <stdint.h>

struct mtp_t {
  uint32_t tod;   // time of day
  uint32_t frac;  // fraction of time of day
};

#define MTP_TIMESTAMP_SIZE 8
#define MTP_REQUEST_SIZE MTP_TIMESTAMP_SIZE
#define MTP_RESPONSE_SIZE (MTP_TIMESTAMP_SIZE * 3)

/* Converts an mtp_t struct to a buffer to send over UDP */
void mtp_time_to_buffer(mtp_t *t, uint8_t *store);

/* Converts a buffer to an mtp_t struct */
void mtp_buffer_to_time(uint8_t *store, mtp_t *t);

/* Converts an mtp_t struct to a floating point representation */
/* Deprecated. Use mtp_to_number instead. */
float mtp_to_float(mtp_t *t);

/* Converts an mtp_t struct to a double precision floating point representation */
double mtp_to_number(mtp_t *t);

#endif  //MTP_H

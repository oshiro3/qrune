#ifndef QRUNE_H_
#define QRUNE_H_

#include <sys/stat.h>

void git_write_loose_object(const unsigned char *, char *, int, const void *,
                            unsigned long, time_t);

int _rev_parse(const char *, char *);

#endif

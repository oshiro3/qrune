#ifndef SHA_H_
#define SHA_H_

// git より引用
inline char *sha1_to_hex(const unsigned char *);
inline void calc_object_sha1(const char *, const void *, unsigned long,
                             unsigned char *);

void calc_sha1(const void *, unsigned long, unsigned char *);

#endif

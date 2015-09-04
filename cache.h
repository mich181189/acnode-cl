#ifndef _CACHE_H_
#define _CACHE_H_

#include <SPI.h>
#include <SD.h>
#include "user.h"

class Cache {
  Cache(char *filename);
  
  bool get(uint8_t *key, user *u);
  void set(uint8_t *key, user *u);
  void purge(void);
  
  static bool compare(uint8_t *k1, uint8_t *k2);
  
  char *_filename;
};

#endif

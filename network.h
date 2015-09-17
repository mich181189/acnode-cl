#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <Energia.h>
#include "card.h"
#include <DateTimeLibrary.h>

int get_url(char * path);
int querycard(Card card);
int networkCheckToolStatus();
int setToolStatus(int status, Card card);
void addNewUser(Card card, Card maintainer);
int toolUseTime(Card card, int time);
int reportToolUse(Card card, int status);
void settime(DateTime *);

#endif

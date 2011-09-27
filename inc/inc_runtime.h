#ifndef INC_RUNTIME_H
#define INC_RUNTIME_H

#define _POSIX_C_SOURCE 199309
#define _REENTRANT

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>

typedef char bool;
const bool false = 0;
const bool true = 1;

int max(int a, int b)
{
   return a > b ? a : b;
}

enum channeltype
{
   BOOL,
   CHAR
};

enum inoutmode
{
   IN,
   OUT
};

enum readwritemode
{
   READ = 0,
   WRITE = 1
};

struct boolchannel
{
   enum channeltype type;
   int inready[2];
   int outready[2];
   int valueset[2];
   volatile bool value;
};

void boolchannelinit(struct boolchannel* this)
{
   this->type = BOOL;
   assert(pipe(this->inready) == 0);
   assert(pipe(this->outready) == 0);
   assert(pipe(this->valueset) == 0);
}

void boolchannelfini(struct boolchannel* this)
{
   assert(close(this->inready[READ]) == 0);
   assert(close(this->inready[WRITE]) == 0);
   assert(close(this->outready[READ]) == 0);
   assert(close(this->outready[WRITE]) == 0);
   assert(close(this->valueset[READ]) == 0);
   assert(close(this->valueset[WRITE]) == 0);
}

void boolchannelin(struct boolchannel* this, bool* b)
{
   char X = 'X';
   assert(write(this->inready[WRITE], &X, sizeof (X)) == sizeof (X));
   assert(read(this->outready[READ], &X, sizeof (X)) == sizeof (X));
   assert(read(this->valueset[READ], &X, sizeof (X)) == sizeof (X));
   *b = this->value;
}

void boolchannelout(struct boolchannel* this, bool b)
{
   char X = 'X';
   assert(write(this->outready[WRITE], &X, sizeof (X)) == sizeof (X));
   assert(read(this->inready[READ], &X, sizeof (X)) == sizeof (X));
   this->value = b;
   assert(write(this->valueset[WRITE], &X, sizeof (X)) == sizeof (X));
}

struct charchannel
{
   enum channeltype type;
   int inready[2];
   int outready[2];
   int valueset[2];
   volatile char value;
};

void charchannelinit(struct charchannel* this)
{
   this->type = CHAR;
   assert(pipe(this->inready) == 0);
   assert(pipe(this->outready) == 0);
   assert(pipe(this->valueset) == 0);
}

void charchannelfini(struct charchannel* this)
{
   assert(close(this->inready[READ]) == 0);
   assert(close(this->inready[WRITE]) == 0);
   assert(close(this->outready[READ]) == 0);
   assert(close(this->outready[WRITE]) == 0);
   assert(close(this->valueset[READ]) == 0);
   assert(close(this->valueset[WRITE]) == 0);
}

void charchannelin(struct charchannel* this, char* c)
{
   char X = 'X';
   assert(write(this->inready[WRITE], &X, sizeof (X)) == sizeof (X));
   assert(read(this->outready[READ], &X, sizeof (X)) == sizeof (X));
   assert(read(this->valueset[READ], &X, sizeof (X)) == sizeof (X));
   *c = this->value;
}

void charchannelout(struct charchannel* this, char c)
{
   char X = 'X';
   assert(write(this->outready[WRITE], &X, sizeof (X)) == sizeof (X));
   assert(read(this->inready[READ], &X, sizeof (X)) == sizeof (X));
   this->value = c;
   assert(write(this->valueset[WRITE], &X, sizeof (X)) == sizeof (X));
}

size_t altwait(enum channeltype* channels[], enum inoutmode modes[],
               size_t size)
{
   struct boolchannel* bc = NULL;
   struct charchannel* cc = NULL;
   size_t i;
   int maxfd = 0;
   fd_set fds;
   FD_ZERO(&fds);
   for (i = 0; i < size; ++i)
   {
      int fd = 0;
      switch (*channels[i])
      {
         case BOOL:
            bc = (struct boolchannel*)channels[i];
            switch (modes[i])
            {
               case IN:
                  fd = bc->outready[READ];
                  FD_SET(fd, &fds);
                  break;
               case OUT:
                  fd = bc->inready[READ];
                  FD_SET(fd, &fds);
                  break;
            }
            break;
         case CHAR:
            cc = (struct charchannel*)channels[i];
            switch (modes[i])
            {
               case IN:
                  fd = cc->outready[READ];
                  FD_SET(fd, &fds);
                  break;
               case OUT:
                  fd = cc->inready[READ];
                  FD_SET(fd, &fds);
                  break;
            }
            break;
      }
      maxfd = max(maxfd, fd);
   }
   assert(select(maxfd + 1, &fds, NULL, NULL, NULL) > 0);
   for (i = 0; i < size; ++i)
   {
      switch (*channels[i])
      {
         case BOOL:
            bc = (struct boolchannel*)channels[i];
            switch (modes[i])
            {
               case IN:
                  if (FD_ISSET(bc->outready[READ], &fds))
                  {
                     return i;
                  }
                  break;
               case OUT:
                  if (FD_ISSET(bc->inready[READ], &fds))
                  {
                     return i;
                  }
                  break;
            }
            break;
         case CHAR:
            cc = (struct charchannel*)channels[i];
            switch (modes[i])
            {
               case IN:
                  if (FD_ISSET(cc->outready[READ], &fds))
                  {
                     return i;
                  }
                  break;
               case OUT:
                  if (FD_ISSET(cc->inready[READ], &fds))
                  {
                     return i;
                  }
                  break;
            }
            break;
      }
   }
   assert(0);
   return 0;
}

#endif

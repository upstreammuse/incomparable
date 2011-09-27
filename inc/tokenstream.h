#ifndef INC_TOKENSTREAM_H
#define INC_TOKENSTREAM_H

class TokenStream;

#include <cstdio>
#include <string>
#include <vector>
#include "token.h"

class TokenStream
{
   public:
      TokenStream(FILE* f);
      Token at(size_t index) const;
      void die(size_t index, const std::string& msg) const;

   private:
      std::vector<Token> tokens_;
};

#endif

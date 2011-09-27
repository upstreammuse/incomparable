#ifndef INC_PROGRAM_H
#define INC_PROGRAM_H

class Program;

#include <map>
#include <string>
#include "block.h"
#include "tokenstream.h"

class Program
{
   public:
      Program(const TokenStream& ts);
      void write(FILE* f) const;

   private:
      std::vector<Block> blocks_;
      std::map<std::string, std::set<TokenType> > types_;
      VariableTable vars_;
};

#endif

#ifndef INC_PAR_H
#define INC_PAR_H

class Par;

#include "block.h"
#include "tokenstream.h"

class Par
{
   public:
      Par(const TokenStream& ts, size_t& index,
          const std::vector<Block>& blocks,
          const std::map<std::string, std::set<TokenType> >& types,
          const VariableTable& vars);
      void write(FILE* f, int indent) const;

   private:
      std::vector<Call> calls_;
      std::set<std::string> ins_;
      std::set<std::string> outs_;
      VariableTable vars_;
};

#endif

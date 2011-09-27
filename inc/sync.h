#ifndef INC_SYNC_H
#define INC_SYNC_H

class Sync;

#include <map>
#include "expression.h"
#include "tokenstream.h"

class Sync
{
   public:
      Sync(const TokenStream& ts, size_t& index,
           const std::map<std::string, std::set<TokenType> >& types,
           const VariableTable& vars);
      void write(FILE* f, int indent) const;

   private:
      TokenType dir_;
      std::vector<Expression> expr_;
      std::string ident_;
      std::string ident2_;
      std::string type_;
};

#endif

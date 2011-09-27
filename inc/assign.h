#ifndef INC_ASSIGN_H
#define INC_ASSIGN_H

class Assign;

#include <map>
#include "expression.h"
#include "tokenstream.h"
#include "variabletable.h"

class Assign
{
   public:
      Assign(const TokenStream& ts, size_t& index,
             const std::map<std::string, std::set<TokenType> >& types,
             const VariableTable& vars);
      void write(FILE* f, int indent) const;

   private:
      std::vector<Expression> expr_;
      std::string ident_;
};

#endif

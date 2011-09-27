#ifndef INC_WHILE_H
#define INC_WHILE_H

class While;

#include "expression.h"
#include "statement.h"
#include "tokenstream.h"

class While
{
   public:
      While(const TokenStream& ts, size_t& index,
            const std::vector<Block>& blocks,
            const std::map<std::string, std::set<TokenType> >& types,
            const VariableTable& vars);
      void write(FILE* f, int indent) const;

   private:
      std::vector<Expression> expr_;
      std::vector<Statement> statements_;
};

#endif

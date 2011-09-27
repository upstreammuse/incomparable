#ifndef INC_ALT_H
#define INC_ALT_H

class Alt;

#include <set>
#include "block.h"
#include "expression.h"
#include "statement.h"
#include "tokenstream.h"
#include "variabletable.h"

class Alt
{
   public:
      Alt(const TokenStream& ts, size_t& index,
          const std::vector<Block>& blocks,
          const std::map<std::string, std::set<TokenType> >& types,
          const VariableTable& vars, bool first);
      int depth() const;
      void write(FILE* f, int indent) const;

   private:
      void writeHeader(FILE* f, int indent, int index) const;
      void writeStatements(FILE* f, int indent, int index) const;

      std::vector<Alt> alt_;
      TokenType dir_;
      std::vector<Expression> expr_;
      std::string ident_;
      std::string ident2_;
      std::vector<Statement> statements_;
      std::string type_;
};

#endif

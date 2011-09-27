#ifndef INC_TERM_H
#define INC_TERM_H

class Term;

#include "expression.h"
#include "tokenstream.h"
#include "variabletable.h"

class Term
{
   public:
      Term(const TokenStream& ts, size_t& index,
           const std::map<std::string, std::set<TokenType> >& types,
           const VariableTable& vars);
      std::string type() const { return type_; }
      void write(FILE* f) const;

   private:
      std::vector<Expression> expr_;
      std::string type_;
      std::string value_;
};

#endif

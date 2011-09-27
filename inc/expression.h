#ifndef INC_EXPRESSION_H
#define INC_EXPRESSION_H

class Expression;

#include <map>
#include <set>
#include "term.h"
#include "tokenstream.h"
#include "variabletable.h"

class Expression
{
   public:
      Expression(const TokenStream& ts, size_t& index,
                 const std::map<std::string, std::set<TokenType> >& types,
                 const VariableTable& vars);
      std::string type() const { return type_; }
      void write(FILE* f) const;

   private:
      void writeOp(FILE* f) const;

      TokenType op_;
      std::vector<Term> terms_;
      std::string type_;
};

#endif

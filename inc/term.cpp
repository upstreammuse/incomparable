#include "term.h"

Term::Term(const TokenStream& ts, size_t& index,
           const std::map<std::string, std::set<TokenType> >& types,
           const VariableTable& vars)
{
   if (ts.at(index) == IDENTIFIER)
   {
      value_ = ts.at(index).value();
      if (!vars.declared(value_))
      {
         ts.die(index, "unknown variable name");
      }
      if (vars.direction(value_) != NONE)
      {
         ts.die(index, "directional variables not allowed in expressions");
      }
      type_ = vars.type(value_);
      value_.append("_");
   }
   else if (ts.at(index) == LITERAL)
   {
      type_ = ts.at(index).type();
      value_ = ts.at(index).value();
   }
   else if (ts.at(index) == LEFTPAR)
   {
      ++index;
      expr_.push_back(Expression(ts, index, types, vars));
      if (ts.at(index) != RIGHTPAR)
      {
         ts.die(index, "mismatched parentheses");
      }
   }
   else
   {
      ts.die(index, "invalid expression term, check parentheses");
   }
   ++index;
}

void Term::write(FILE* f) const
{
   if (expr_.size() > 1)
   {
      fprintf(f, "(");
      expr_.at(0).write(f);
      fprintf(f, ")");
   }
   else
   {
      fprintf(f, "%s", value_.c_str());
   }
}

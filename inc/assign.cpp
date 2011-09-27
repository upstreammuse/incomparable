#include "assign.h"

Assign::Assign(const TokenStream& ts, size_t& index,
               const std::map<std::string, std::set<TokenType> >& types,
               const VariableTable& vars)
{
   if (ts.at(index) != IDENTIFIER)
   {
      ts.die(index, "assignment requires a variable");
   }
   ident_ = ts.at(index).value();
   if (!vars.declared(ident_))
   {
      ts.die(index, std::string("unknown variable ") + ident_);
   }
   if (vars.direction(ident_) != NONE)
   {
      ts.die(index, "cannot assign to directional variable");
   }
   ++index;
   if (ts.at(index) != ASSIGN)
   {
      ts.die(index, "expected assignment");
   }
   ++index;
   expr_.push_back(Expression(ts, index, types, vars));
   if (vars.type(ident_) != expr_.at(0).type())
   {
      ts.die(index, "assignment type mismatch");
   }
   if (ts.at(index) != STATEMENTEND)
   {
      ts.die(index, "expected end of statement");
   }
   ++index;
}

void Assign::write(FILE* f, int indent) const
{
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "%s_ = ", ident_.c_str());
   expr_.at(0).write(f);
   fprintf(f, ";\n");
}

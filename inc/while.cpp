#include "while.h"

While::While(const TokenStream& ts, size_t& index,
             const std::vector<Block>& blocks,
             const std::map<std::string, std::set<TokenType> >& types,
             const VariableTable& vars)
{
   if (ts.at(index) != WHILESTART)
   {
      ts.die(index, "expected while");
   }
   ++index;
   if (ts.at(index) != LEFTPAR)
   {
      ts.die(index, "expected '('");
   }
   ++index;
   expr_.push_back(Expression(ts, index, types, vars));
   if (expr_.at(0).type() != "bool")
   {
      ts.die(index, "while requires boolean expression");
   }
   if (ts.at(index) != RIGHTPAR)
   {
      ts.die(index, "expected ')'");
   }
   ++index;
   while (ts.at(index) != WHILEEND)
   {
      statements_.push_back(Statement(ts, index, blocks, types, vars));
   }
   if (ts.at(index) != WHILEEND)
   {
      ts.die(index, "expected end of while");
   }
   ++index;
}

void While::write(FILE* f, int index) const
{
   for (int i = 0; i < index; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "while (");
   expr_.at(0).write(f);
   fprintf(f, ")\n");
   for (int i = 0; i < index; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "{\n");
   for (std::vector<Statement>::const_iterator it = statements_.begin();
        it != statements_.end(); ++it)
   {
      it->write(f, index + 3);
   }
   for (int i = 0; i < index; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "}\n");
}

#include "par.h"

Par::Par(const TokenStream& ts, size_t& index, const std::vector<Block>& blocks,
         const std::map<std::string, std::set<TokenType> >& types,
         const VariableTable& vars)
: vars_(&vars)
{
   if (ts.at(index) != PARSTART)
   {
      ts.die(index, "expected parallel");
   }
   ++index;
   if (ts.at(index) != LEFTPAR)
   {
      ts.die(index, "expected '('");
   }
   ++index;
   while (ts.at(index) != RIGHTPAR && ts.at(index) != NONE)
   {
      if (ts.at(index) != IDENTIFIER)
      {
         ts.die(index, "expected identifier");
      }
      ins_.insert(ts.at(index).value());
      outs_.insert(ts.at(index).value());
      if (ts.at(index + 1) != DECLARE)
      {
         ts.die(index, "expected declaration");
      }
      if (ts.at(index + 2) != IDENTIFIER)
      {
         ts.die(index + 2, "expected type");
      }
      if (types.find(ts.at(index + 2).value()) == types.end())
      {
         ts.die(index + 2, "unknown type");
      }
      if (ts.at(index + 3) != INOUT)
      {
         ts.die(index + 3, "parallel requires <>");
      }
      vars_.declare(ts.at(index).value(), ts.at(index + 2).value(), INOUT);
      index += 4;
      if (ts.at(index) == SEPARATOR && ts.at(index + 1) == IDENTIFIER)
      {
         ++index;
      }
      else if (ts.at(index) != RIGHTPAR)
      {
         ts.die(index, "expected separator or ')'");
      }
   }
   if (ts.at(index) != RIGHTPAR)
   {
      ts.die(index, "expected ')'");
   }
   ++index;
   while (ts.at(index) == IDENTIFIER)
   {
      calls_.push_back(Call(ts, index, blocks, vars_));
   }
   // TODO make sure all par-defined channels are used on both ends
   if (ts.at(index) != PAREND)
   {
      ts.die(index, "expected end of parallel");
   }
   ++index;
}

void Par::write(FILE* f, int indent) const
{
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "{\n");
   indent += 3;
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "pthread_t threads[%lu];\n", calls_.size());
   for (size_t i = 0; i < calls_.size(); ++i)
   {
      calls_.at(i).writePar1(f, indent, i);
   }
   vars_.writePar1(f, indent);
   for (size_t i = 0; i < calls_.size(); ++i)
   {
      calls_.at(i).writePar2(f, indent, i);
   }
   for (size_t i = 0; i < calls_.size(); ++i)
   {
      calls_.at(i).writePar3(f, indent, i);
   }
   for (size_t i = 0; i < calls_.size(); ++i)
   {
      for (int i2 = 0; i2 < indent; ++i2)
      {
         fprintf(f, " ");
      }
      fprintf(f, "pthread_join(threads[%lu], NULL);\n", i);
   }
   vars_.writePar2(f, indent);
   indent -= 3;
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "}\n");
}

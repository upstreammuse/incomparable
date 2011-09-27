// TODO - make sure channels and variables can't be reused across elsealts

#include "alt.h"

Alt::Alt(const TokenStream& ts, size_t& index,
         const std::vector<Block>& blocks,
         const std::map<std::string, std::set<TokenType> >& types,
         const VariableTable& vars, bool first)
{
   if (first && ts.at(index) != ALTSTART)
   {
      ts.die(index, "expected alt");
   }
   else if (!first && ts.at(index) != ALTELSE)
   {
      ts.die(index, "expected elsealt");
   }
   ++index;
   if (ts.at(index) != LEFTPAR)
   {
      ts.die(index, "expected '('");
   }
   ++index;
   if (ts.at(index) != IDENTIFIER)
   {
      ts.die(index, "expected identifier");
   }
   ident_ = ts.at(index).value();
   if (!vars.declared(ident_))
   {
      ts.die(index, "unknown variable");
   }
   type_ = vars.type(ident_);
   ++index;
   if (ts.at(index) != IN && ts.at(index) != OUT)
   {
      ts.die(index, "expected -> or <-");
   }
   dir_ = ts.at(index);
   if (vars.direction(ident_) != dir_)
   {
      ts.die(index, "direction mismatch");
   }
   ++index;
   if (dir_ == IN)
   {
      if (ts.at(index) != IDENTIFIER)
      {
         ts.die(index, "-> requires identifier");
      }
      ident2_ = ts.at(index).value();
      if (!vars.declared(ident2_))
      {
         ts.die(index, "unknown variable");
      }
      if (vars.direction(ident2_) != NONE)
      {
         ts.die(index, "-> requires directionless variable");
      }
      if (vars.type(ident2_) != type_)
      {
         ts.die(index, "type mismatch");
      }
   }
   else if (dir_ == OUT)
   {
      expr_.push_back(Expression(ts, index, types, vars));
      if (expr_.at(0).type() != type_)
      {
         ts.die(index, "type mismatch");
      }
   }
   ++index;
   if (ts.at(index) != RIGHTPAR)
   {
      ts.die(index, "expected ')'");
   }
   ++index;
   while (ts.at(index) != ALTEND && ts.at(index) != ALTELSE)
   {
      statements_.push_back(Statement(ts, index, blocks, types, vars));
   }
   if (ts.at(index) == ALTELSE)
   {
      alt_.push_back(Alt(ts, index, blocks, types, vars, false));
   }
   else if (ts.at(index) == ALTEND)
   {
      ++index;
   }
}

int Alt::depth() const
{
   if (alt_.size() > 0)
   {
      return alt_.at(0).depth() + 1;
   }
   else
   {
      return 1;
   }
}

void Alt::write(FILE* f, int indent) const
{
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "{\n");
   indent += 3;
   int depth = 1;
   if (alt_.size() > 0)
   {
      depth += alt_.at(0).depth();
   }
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "enum channeltype* alt[%d];\n", depth);
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "enum inoutmode modes[%d];\n", depth);
   writeHeader(f, indent, 0);
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "switch (altwait(alt, modes, %d))\n", depth);
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "{\n");
   writeStatements(f, indent + 3, 0);
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "}\n");
   indent -= 3;
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "}\n");
}

void Alt::writeHeader(FILE* f, int indent, int index) const
{
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "alt[%d] = (enum channeltype*)%s_;\n", index, ident_.c_str());
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   if (dir_ == IN)
   {
      fprintf(f, "modes[%d] = IN;\n", index);
   }
   else if (dir_ == OUT)
   {
      fprintf(f, "modes[%d] = OUT;\n", index);
   }
   if (alt_.size() > 0)
   {
      alt_.at(0).writeHeader(f, indent, index + 1);
   }
}

void Alt::writeStatements(FILE* f, int indent, int index) const
{
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "case %d:\n", index);
   for (int i = 0; i < indent + 3; ++i)
   {
      fprintf(f, " ");
   }
   if (dir_ == IN)
   {
      fprintf(f, "%schannelin(%s_, &%s_);\n", type_.c_str(), ident_.c_str(),
              ident2_.c_str());
   }
   else if (dir_ == OUT)
   {
      if (expr_.size() > 0)
      {
         fprintf(f, "%schannelout(%s_, ", type_.c_str(), ident_.c_str());
         expr_.at(0).write(f);
         fprintf(f, ");\n");
      }
      else
      {
         fprintf(f, "%schannelout(%s_, %s_);\n", type_.c_str(), ident_.c_str(),
                 ident2_.c_str());
      }
   }
   for (size_t i = 0; i < statements_.size(); ++i)
   {
      statements_.at(i).write(f, indent + 3);
   }
   for (int i = 0; i < indent + 3; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "break;\n");
   if (alt_.size() > 0)
   {
      alt_.at(0).writeStatements(f, indent, index + 1);
   }
}

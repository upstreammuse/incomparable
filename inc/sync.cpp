#include "sync.h"

Sync::Sync(const TokenStream& ts, size_t& index,
           const std::map<std::string, std::set<TokenType> >& types,
           const VariableTable& vars)
{
   if (ts.at(index) != IDENTIFIER)
   {
      ts.die(index, "expected identifier");
   }
   ident_ = ts.at(index).value();
   if (!vars.declared(ident_))
   {
      ts.die(index, std::string("unknown variable ") + ident_);
   }
   type_ = vars.type(ident_);
   dir_ = ts.at(index + 1);
   if (dir_ != IN && dir_ != OUT)
   {
      ts.die(index + 1, "synchronization requires <- or ->");
   }
   if (vars.direction(ident_) != dir_)
   {
      ts.die(index, "synchronization direction mismatch");
   }
   index += 2;
   if (dir_ == IN)
   {
      if (ts.at(index) != IDENTIFIER)
      {
         ts.die(index, "-> requires a variable");
      }
      ident2_ = ts.at(index).value();
      if (!vars.declared(ident2_))
      {
         ts.die(index, std::string("unknown variable") + ident2_);
      }
      if (vars.type(ident2_) != vars.type(ident_))
      {
         ts.die(index, "synchronization type mismatch");
      }
      if (vars.direction(ident2_) != NONE)
      {
         ts.die(index, "-> requires a nondirectional variable");
      }
      ++index;
   }
   else if (dir_ == OUT)
   {
      expr_.push_back(Expression(ts, index, types, vars));
      if (vars.type(ident_) != expr_.at(0).type())
      {
         ts.die(index, "synchronization type mismatch");
      }
   }
   if (ts.at(index) != STATEMENTEND)
   {
      ts.die(index, "expected end of statement");
   }
   ++index;
}

void Sync::write(FILE* f, int indent) const
{
   for (int i = 0; i < indent; ++i)
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
}

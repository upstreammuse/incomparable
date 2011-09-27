#include "block.h"
#include "statement.h"

Statement::Statement(const TokenStream& ts, size_t& index,
                     const std::vector<Block>& blocks,
                     const std::map<std::string, std::set<TokenType> >& types,
                     const VariableTable& vars)
{
   if (ts.at(index) == ALTSTART)
   {
      alt_.push_back(Alt(ts, index, blocks, types, vars, true));
   }
   else if (ts.at(index) == IDENTIFIER && ts.at(index + 1) == ASSIGN)
   {
      assign_.push_back(Assign(ts, index, types, vars));
   }
   else if (ts.at(index) == BLOCKSTART)
   {
      block_.push_back(Block(ts, index, "", blocks, types, vars));
   }
   else if (ts.at(index) == BUILTIN)
   {
      ++index;
      if (ts.at(index) != LITERAL)
      {
         ts.die(index, "invalid builtin");
      }
      if (ts.at(index).type() != "string")
      {
         ts.die(index, "builtin requires a string");
      }
      builtin_ = ts.at(index).value();
      ++index;
   }
   else if (ts.at(index) == IDENTIFIER && ts.at(index + 1) == LEFTPAR)
   {
      call_.push_back(Call(ts, index, blocks, vars));
   }
   else if (ts.at(index) == PARSTART)
   {
      par_.push_back(Par(ts, index, blocks, types, vars));
   }
   else if (ts.at(index) == IDENTIFIER
            && (ts.at(index + 1) == IN || ts.at(index + 1) == OUT))
   {
      sync_.push_back(Sync(ts, index, types, vars));
   }
   else if (ts.at(index) == WHILESTART)
   {
      while_.push_back(While(ts, index, blocks, types, vars));
   }
   else
   {
      ts.die(index, "invalid statement");
   }
}

void Statement::write(FILE* f, int indent) const
{
   if (alt_.size() > 0)
   {
      alt_.at(0).write(f, indent);
   }
   else if (assign_.size() > 0)
   {
      assign_.at(0).write(f, indent);
   }
   else if (block_.size() > 0)
   {
      block_.at(0).write(f, indent);
   }
   else if (builtin_.size() > 0)
   {
      for (int i = 0; i < indent; ++i)
      {
         fprintf(f, " ");
      }
      fprintf(f, "%s\n", builtin_.c_str());
   }
   else if (call_.size() > 0)
   {
      call_.at(0).write(f, indent);
   }
   else if (par_.size() > 0)
   {
      par_.at(0).write(f, indent);
   }
   else if (sync_.size() > 0)
   {
      sync_.at(0).write(f, indent);
   }
   else if (while_.size() > 0)
   {
      while_.at(0).write(f, indent);
   }
}

// TODO make sure all block parameters are associated
// TODO make sure channels aren't reused in a call
// TODO allow expressions to feed non-channel inputs
// TODO make sure directions match

#include <algorithm>
#include "call.h"

Call::Call(const TokenStream& ts, size_t& index,
           const std::vector<Block>& blocks,
           const VariableTable& vars)
{
   if (ts.at(index) != IDENTIFIER)
   {
      ts.die(index, "expected identifier");
   }
   ident_ = ts.at(index).value();
   if (std::find(blocks.begin(), blocks.end(), ident_) == blocks.end())
   {
      ts.die(index, "unknown block");
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
      formals_.push_back(ts.at(index).value());
      if (!std::find(blocks.begin(), blocks.end(), ident_)->declared(
                                                               formals_.back()))
      {
         ts.die(index, "unknown parameter");
      }
      ++index;
      if (ts.at(index) != DECLARE)
      {
         ts.die(index, "expected parameter association");
      }
      ++index;
      if (ts.at(index) != IDENTIFIER)
      {
         ts.die(index, "expected identifier");
      }
      actuals_.push_back(ts.at(index).value());
      if (!vars.declared(actuals_.back()))
      {
         ts.die(index, "unknown identifier");
      }
      if (std::find(blocks.begin(), blocks.end(), ident_)->type(formals_.back())
          != vars.type(actuals_.back()))
      {
         ts.die(index, "type mismatch");
      }
      ++index;
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
   if (ts.at(index) != STATEMENTEND)
   {
      ts.die(index, "expected end of statement");
   }
   ++index;
}

void Call::write(FILE* f, int indent) const
{
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "{\n");
   indent += 3;
   if (formals_.size() > 0)
   {
      for (int i = 0; i < indent; ++i)
      {
         fprintf(f, " ");
      }
      fprintf(f, "struct %sstruct params;\n", ident_.c_str());
      for (size_t i = 0; i < formals_.size(); ++i)
      {
         for (int i2 = 0; i2 < indent; ++i2)
         {
            fprintf(f, " ");
         }
         fprintf(f, "params.%s_ = %s_;\n", formals_.at(i).c_str(),
               actuals_.at(i).c_str());
      }
      for (int i = 0; i < indent; ++i)
      {
         fprintf(f, " ");
      }
      fprintf(f, "%s_(&params);\n", ident_.c_str());
   }
   else
   {
      for (int i = 0; i < indent; ++i)
      {
         fprintf(f, " ");
      }
      fprintf(f, "%s_(NULL);\n", ident_.c_str());
   }
   indent -= 3;
   for (int i = 0; i < indent; ++i)
   {
      fprintf(f, " ");
   }
   fprintf(f, "}\n");
}

void Call::writePar1(FILE* f, int indent, size_t parindex) const
{
   if (formals_.size() > 0)
   {
      for (int i = 0; i < indent; ++i)
      {
         fprintf(f, " ");
      }
      fprintf(f, "struct %sstruct params%lu;\n", ident_.c_str(), parindex);
   }
}

void Call::writePar2(FILE* f, int indent, size_t parindex) const
{
   if (formals_.size() > 0)
   {
      for (size_t i = 0; i < formals_.size(); ++i)
      {
         for (int i2 = 0; i2 < indent; ++i2)
         {
            fprintf(f, " ");
         }
         fprintf(f, "params%lu.%s_ = %s_;\n", parindex, formals_.at(i).c_str(),
                 actuals_.at(i).c_str());
      }
   }
}

void Call::writePar3(FILE* f, int indent, size_t parindex) const
{
   if (formals_.size() > 0)
   {
      for (int i = 0; i < indent; ++i)
      {
         fprintf(f, " ");
      }
      fprintf(f, "pthread_create(&threads[%lu], NULL, %s_, &params%lu);\n",
              parindex, ident_.c_str(), parindex);
   }
   else
   {
      for (int i = 0; i < indent; ++i)
      {
         fprintf(f, " ");
      }
      fprintf(f, "pthread_create(&threads[%lu], NULL, %s_, NULL);\n", parindex,
              ident_.c_str());
   }
}

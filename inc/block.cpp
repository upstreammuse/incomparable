// TODO make sure variables and channels are all used

#include "block.h"

Block::Block(const TokenStream& ts, size_t& index, const std::string& name,
             const std::vector<Block>& blocks,
             const std::map<std::string, std::set<TokenType> >& types,
             const VariableTable& vars)
: name_(name), vars_(&vars)
{
   if (ts.at(index) != BLOCKSTART)
   {
      ts.die(index, "expected block");
   }
   ++index;
   if (ts.at(index) != LEFTPAR)
   {
      ts.die(index, "expected '('");
   }
   ++index;
   if (ts.at(index) != RIGHTPAR && name == "main")
   {
      ts.die(index, "main block requires empty header");
   }
   while (ts.at(index) != RIGHTPAR && ts.at(index) != NONE)
   {
      std::string name;
      std::string type;
      if (ts.at(index) == IDENTIFIER && ts.at(index + 1) == DECLARE)
      {
         name = ts.at(index).value();
         if (vars_.declared(name))
         {
            ts.die(index, std::string("redeclaration of ") + name);
         }
      }
      else
      {
         ts.die(index, "only declarations allowed in block header");
      }
      index += 2;
      if (ts.at(index) == IDENTIFIER)
      {
         type = ts.at(index).value();
         if (types.find(type) == types.end())
         {
            ts.die(index, std::string("unknown type ") + type);
         }
      }
      ++index;
      if (ts.at(index) == IN || ts.at(index) == OUT)
      {
         if (name_.size() > 0)
         {
            vars_.declare(name, type, ts.at(index));
            ++index;
         }
         else
         {
            ts.die(index, "-> and <- only allowed at named block scope");
         }
      }
      else if (ts.at(index) == INOUT)
      {
         ts.die(index, "<> not allowed at block scope");
      }
      else
      {
         vars_.declare(name, type, NONE);
      }
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
   while (ts.at(index) != BLOCKEND)
   {
      statements_.push_back(Statement(ts, index, blocks, types, vars_));
   }
   if (ts.at(index) != BLOCKEND)
   {
      ts.die(index, "expected end of block");
   }
   ++index;
}

bool Block::declared(const std::string& name) const
{
   return vars_.declared(name);
}

std::string Block::type(const std::string& name) const
{
   return vars_.type(name);
}

void Block::write(FILE* f, int indent) const
{
   if (name_ == "main")
   {
      fprintf(f, "int main()\n{\n");
   }
   else if (name_.size() > 0)
   {
      if (vars_.size() > 0)
      {
         fprintf(f, "struct %sstruct\n{\n", name_.c_str());
         vars_.write(f, 3, false);
         fprintf(f, "};\n\n");
         fprintf(f, "void* %s_(void* p)\n{\n", name_.c_str());
         fprintf(f, "   struct %sstruct* q = p;\n", name_.c_str());
         vars_.write(f, 3, true);
      }
      else
      {
         fprintf(f, "void* %s_(void* p)\n{\n", name_.c_str());
         fprintf(f, "   (void)p;\n");
      }
   }
   else
   {
      for (int i = 0; i < indent; ++i)
      {
         fprintf(f, " ");
      }
      fprintf(f, "{\n");
      vars_.write(f, indent + 3, false);
   }
   for (std::vector<Statement>::const_iterator it = statements_.begin();
        it != statements_.end(); ++it)
   {
      it->write(f, indent + 3);
   }
   if (name_ == "main")
   {
      fprintf(f, "   return 0;\n}\n\n");
   }
   else if (name_.size() > 0)
   {
      fprintf(f, "   return NULL;\n}\n\n");
   }
   else
   {
      for (int i = 0; i < indent; ++i)
      {
         fprintf(f, " ");
      }
      fprintf(f, "}\n");
   }
}

bool Block::operator==(const std::string& name) const
{
   return name_ == name;
}

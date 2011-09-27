#include <algorithm>
#include "program.h"

Program::Program(const TokenStream& ts)
{
   std::set<TokenType> boolops;
   boolops.insert(NOT);
   types_.insert(make_pair("bool", boolops));
   std::set<TokenType> charops;
   types_.insert(make_pair("char", charops));
   std::set<TokenType> intops;
   intops.insert(LESSTHAN);
   intops.insert(PLUS);
   types_.insert(make_pair("int", intops));
   size_t index = 0;
   while (ts.at(index) != NONE)
   {
      std::string name;
      if (ts.at(index) == IDENTIFIER && ts.at(index + 1) == DECLARE)
      {
         name = ts.at(index).value();
         if (std::find(blocks_.begin(), blocks_.end(), name) != blocks_.end())
         {
            ts.die(index, std::string("redeclaration of ") + name);
         }
         index += 2;
      }
      else
      {
         ts.die(index, "only declarations allowed at global scope");
      }
      if (ts.at(index) == BLOCKSTART)
      {
         blocks_.push_back(Block(ts, index, name, blocks_, types_, vars_));
      }
      else
      {
         ts.die(index, "only blocks can be declared at global scope");
      }
   }
}

void Program::write(FILE* f) const
{
   fprintf(f, "/* This program was generated by the inc compiler */\n\n");
   fprintf(f, "#include \"inc_runtime.h\"\n\n");
   for (std::vector<Block>::const_iterator it = blocks_.begin();
        it != blocks_.end(); ++it)
   {
      it->write(f, 0);
   }
   fprintf(f, "/* End of inc generated program */\n");
}

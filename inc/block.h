#ifndef INC_BLOCK_H
#define INC_BLOCK_H

class Block;

#include <map>
#include <set>
#include "statement.h"
#include "tokenstream.h"
#include "variabletable.h"

class Block
{
   public:
      Block(const TokenStream& ts, size_t& index, const std::string& name,
            const std::vector<Block>& blocks,
            const std::map<std::string, std::set<TokenType> >& types,
            const VariableTable& vars);
      bool declared(const std::string& name) const;
      std::string type(const std::string& name) const;
      void write(FILE* f, int indent) const;
      bool operator==(const std::string& name) const;

   private:
      std::string name_;
      std::vector<Statement> statements_;
      VariableTable vars_;
};

#endif

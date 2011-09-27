#ifndef INC_STATEMENT_H
#define INC_STATEMENT_H

class Statement;

#include "alt.h"
#include "assign.h"
#include "block.h"
#include "call.h"
#include "par.h"
#include "sync.h"
#include "tokenstream.h"
#include "variabletable.h"
#include "while.h"

class Statement
{
   public:
      Statement(const TokenStream& ts, size_t& index,
                const std::vector<Block>& blocks,
                const std::map<std::string, std::set<TokenType> >& types,
                const VariableTable& vars);
      void write(FILE* f, int indent) const;

   private:
      std::vector<Alt> alt_;
      std::vector<Assign> assign_;
      std::vector<Block> block_;
      std::string builtin_;
      std::vector<Call> call_;
      std::vector<Par> par_;
      std::vector<Sync> sync_;
      std::vector<While> while_;
};

#endif

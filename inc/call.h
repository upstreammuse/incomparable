#ifndef INC_CALL_H
#define INC_CALL_H

class Call;

#include <map>
#include "block.h"
#include "tokenstream.h"

class Call
{
   public:
      Call(const TokenStream& ts, size_t& index,
           const std::vector<Block>& blocks,
           const VariableTable& vars);
      void write(FILE* f, int indent) const;
      void writePar1(FILE* f, int indent, size_t parindex) const;
      void writePar2(FILE* f, int indent, size_t parindex) const;
      void writePar3(FILE* f, int indent, size_t parindex) const;

   private:
      std::vector<std::string> actuals_;
      std::vector<std::string> formals_;
      std::string ident_;
};

#endif

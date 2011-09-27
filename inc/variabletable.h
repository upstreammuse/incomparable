#ifndef INC_VARIABLETABLE_H
#define INC_VARIABLETABLE_H

class VariableTable;

#include <map>
#include <string>
#include "token.h"

class VariableTable
{
   public:
      VariableTable();
      VariableTable(const VariableTable* parent);
      void declare(const std::string& identifier, const std::string& type,
                   TokenType direction);
      bool declared(const std::string& identifier) const;
      TokenType direction(const std::string& identifier) const;
      size_t size() const;
      std::string type(const std::string& identifier) const;
      void write(FILE* f, int indent, bool hasq) const;
      void writePar1(FILE* f, int indent) const;
      void writePar2(FILE* f, int indent) const;

   private:
      std::map<std::string, std::pair<std::string, TokenType> > map_;
      const VariableTable* parent_;
};

#endif

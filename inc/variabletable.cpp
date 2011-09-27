#include <cstdio>
#include "variabletable.h"

VariableTable::VariableTable()
: parent_(0)
{
}

VariableTable::VariableTable(const VariableTable* parent)
: parent_(parent)
{
}

void VariableTable::declare(const std::string& identifier,
                            const std::string& type, TokenType direction)
{
   map_.insert(std::make_pair(identifier, std::make_pair(type, direction)));
}

bool VariableTable::declared(const std::string& identifier) const
{
   return (parent_ != 0 && parent_->declared(identifier))
          || map_.find(identifier) != map_.end();
}

TokenType VariableTable::direction(const std::string& identifier) const
{
   if (map_.find(identifier) == map_.end())
   {
      return parent_->direction(identifier);
   }
   else
   {
      return map_.find(identifier)->second.second;
   }
}

size_t VariableTable::size() const
{
   if (parent_ != 0)
   {
      return parent_->size() + map_.size();
   }
   else
   {
      return map_.size();
   }
}

std::string VariableTable::type(const std::string& identifier) const
{
   if (map_.find(identifier) == map_.end())
   {
      return parent_->type(identifier);
   }
   else
   {
      return map_.find(identifier)->second.first;
   }
}

void VariableTable::write(FILE* f, int indent, bool hasq) const
{
   for (std::map<std::string,
                 std::pair<std::string, TokenType> >::const_iterator
        it = map_.begin(); it != map_.end(); ++it)
   {
      for (int i = 0; i < indent; ++i)
      {
         fprintf(f, " ");
      }
      if (it->second.second == NONE)
      {
         fprintf(f, "%s %s_", it->second.first.c_str(), it->first.c_str());
      }
      else
      {
         fprintf(f, "struct %schannel* %s_", it->second.first.c_str(),
                 it->first.c_str());
      }
      if (hasq)
      {
         fprintf(f, " = q->%s_", it->first.c_str());
      }
      fprintf(f, ";\n");
   }
}

void VariableTable::writePar1(FILE* f, int indent) const
{
   for (std::map<std::string,
                 std::pair<std::string, TokenType> >::const_iterator
        it = map_.begin(); it != map_.end(); ++it)
   {
      for (int i = 0; i < indent; ++i)
      {
         fprintf(f, " ");
      }
      fprintf(f, "struct %schannel* %s_ = malloc(sizeof (struct %schannel));\n",
              it->second.first.c_str(), it->first.c_str(),
              it->second.first.c_str());
   }
   for (std::map<std::string,
                 std::pair<std::string, TokenType> >::const_iterator
        it = map_.begin(); it != map_.end(); ++it)
   {
      for (int i = 0; i < indent; ++i)
      {
         fprintf(f, " ");
      }
      fprintf(f, "%schannelinit(%s_);\n", it->second.first.c_str(),
              it->first.c_str());
   }
}

void VariableTable::writePar2(FILE* f, int indent) const
{
   for (std::map<std::string,
                 std::pair<std::string, TokenType> >::const_iterator
        it = map_.begin(); it != map_.end(); ++it)
   {
      for (int i = 0; i < indent; ++i)
      {
         fprintf(f, " ");
      }
      fprintf(f, "%schannelfini(%s_);\n", it->second.first.c_str(),
              it->first.c_str());
      for (int i = 0; i < indent; ++i)
      {
         fprintf(f, " ");
      }
      fprintf(f, "free(%s_);\n", it->first.c_str());
   }
}

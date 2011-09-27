#include "expression.h"

Expression::Expression(const TokenStream& ts, size_t& index,
                       const std::map<std::string, std::set<TokenType> >& types,
                       const VariableTable& vars)
: op_(NONE)
{
   if (ts.at(index) == MINUS || ts.at(index) == NOT)
   {
      op_ = ts.at(index);
      ++index;
      Term term(ts, index, types, vars);
      type_ = term.type();
      if (types.find(type_)->second.find(op_)
          == types.find(type_)->second.end())
      {
         ts.die(index, "term not supported by operator");
      }
      terms_.push_back(term);
   }
   else
   {
      Term term(ts, index, types, vars);
      type_ = term.type();
      terms_.push_back(term);
      if (ts.at(index) == LESSTHAN || ts.at(index) == MINUS
          || ts.at(index) == PLUS)
      {
         op_ = ts.at(index);
         if (types.find(type_)->second.find(op_)
             == types.find(type_)->second.end())
         {
            ts.die(index, "term not supported by operator");
         }
         ++index;
         term = Term(ts, index, types, vars);
         if (term.type() != type_)
         {
            ts.die(index, "term type mismatch");
         }
         terms_.push_back(term);
         if (op_ == LESSTHAN)
         {
            type_ = "bool";
         }
         else if (op_ == PLUS)
         {
            while (ts.at(index) == op_)
            {
               ++index;
               term = Term(ts, index, types, vars);
               if (term.type() != type_)
               {
                  ts.die(index, "term type mismatch");
               }
               terms_.push_back(term);
            }
         }
      }
   }
}

void Expression::write(FILE* f) const
{
   if (op_ == NONE)
   {
      terms_.at(0).write(f);
   }
   else if (op_ == MINUS || op_ == NOT)
   {
      writeOp(f);
      terms_.at(0).write(f);
   }
   else
   {
      terms_.at(0).write(f);
      for (std::vector<Term>::const_iterator it = terms_.begin() + 1;
           it != terms_.end(); ++it)
      {
         writeOp(f);
         it->write(f);
      }
   }
}

void Expression::writeOp(FILE* f) const
{
   if (op_ == LESSTHAN)
   {
      fprintf(f, " < ");
   }
   else if (op_ == NOT)
   {
      fprintf(f, "!");
   }
   else if (op_ == PLUS)
   {
      fprintf(f, " + ");
   }
}

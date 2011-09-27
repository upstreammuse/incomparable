#include <cstdio>
#include <cstdlib>
#include "token.h"

Token::Token(size_t line, TokenType t)
: line_(line), t_(t)
{
}

Token::Token(size_t line, TokenType t, const std::string& value)
: line_(line), t_(t), value_(value)
{
   if (t_ != IDENTIFIER)
   {
      die("INTERNAL ERROR: tried to assign value to invalid token type");
   }
}

Token::Token(size_t line, TokenType t, const std::string& type,
             const std::string& value)
: line_(line), t_(t), type_(type), value_(value)
{
   if (t_ != LITERAL)
   {
      die("INTERNAL ERROR: tried to assign type/value to invalid token type");
   }
}

void Token::die(const std::string& msg) const
{
   fprintf(stderr, "Line %lu: %s\n", line_, msg.c_str());
   exit(EXIT_FAILURE);
}

std::string Token::type() const
{
   if (t_ != LITERAL)
   {
      die("INTERNAL ERROR: tried to get type from invalid token type");
   }

   return type_;
}

std::string Token::value() const
{
   if (t_ != IDENTIFIER && t_ != LITERAL)
   {
      die("INTERNAL ERROR: tried to get value from invalid token type");
   }

   return value_;
}

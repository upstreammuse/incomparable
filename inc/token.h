#ifndef INC_TOKEN_H
#define INC_TOKEN_H

class Token;

#include <string>

enum TokenType
{
   NONE,
   ALTELSE,
   ALTEND,
   ALTSTART,
   ASSIGN,
   BLOCKEND,
   BLOCKSTART,
   BUILTIN,
   DECLARE,
   IDENTIFIER,
   IN,
   INOUT,
   LEFTPAR,
   LESSTHAN,
   LITERAL,
   NOT,
   MINUS,
   OUT,
   PAREND,
   PARSTART,
   PLUS,
   RIGHTPAR,
   SEPARATOR,
   STATEMENTEND,
   WHILEEND,
   WHILESTART
};

class Token
{
   public:
      Token(size_t line, TokenType t);
      Token(size_t line, TokenType t, const std::string& value);
      Token(size_t line, TokenType t, const std::string& type,
            const std::string& value);
      void die(const std::string& msg) const;
      std::string type() const;
      std::string value() const;
      operator TokenType() const { return t_; }

   private:
      size_t line_;
      TokenType t_;
      std::string type_;
      std::string value_;
};

#endif

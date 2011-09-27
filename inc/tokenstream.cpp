#include <cctype>
#include "tokenstream.h"

TokenStream::TokenStream(FILE* f)
{
   int c;
   size_t line = 1;
   while ((c = fgetc(f)) != EOF)
   {
      if (isspace(c))
      {
         if (c == '\n')
         {
            ++line;
         }
      }
      else if (c == '#')
      {
         while ((c = fgetc(f)) != EOF && c != '\n') ;
         if (c != EOF)
         {
            ungetc(c, f);
         }
      }
      else if (c == '$')
      {
         tokens_.push_back(Token(line, BUILTIN));
         std::string value;
         while ((c = fgetc(f)) != EOF && c != '\n')
         {
            value.append(1, c);
         }
         tokens_.push_back(Token(line, LITERAL, "string", value));
      }
      else if (c == '\'')
      {
         if ((c = fgetc(f)) != EOF)
         {
            std::string value("'");
            value.append(1, c);
            value.append("'");
            tokens_.push_back(Token(line, LITERAL, "char", value));
            if ((c = fgetc(f)) != '\'')
            {
               die(tokens_.size() - 1, "invalid character literal");
            }
         }
         else
         {
            die(tokens_.size() - 1, "unexpected end of file");
         }
      }
      else if (c == '(')
      {
         tokens_.push_back(Token(line, LEFTPAR));
      }
      else if (c == ')')
      {
         tokens_.push_back(Token(line, RIGHTPAR));
      }
      else if (c == '+')
      {
         tokens_.push_back(Token(line, PLUS));
      }
      else if (c == ',')
      {
         tokens_.push_back(Token(line, SEPARATOR));
      }
      else if (c == '-')
      {
         c = fgetc(f);
         if (c == '>')
         {
            tokens_.push_back(Token(line, IN));
         }
         else
         {
            ungetc(c, f);
            tokens_.push_back(Token(line, MINUS));
         }
      }
      else if (c == ':')
      {
         tokens_.push_back(Token(line, DECLARE));
      }
      else if (c == ';')
      {
         tokens_.push_back(Token(line, STATEMENTEND));
      }
      else if (c == '<')
      {
         c = fgetc(f);
         if (c == '-')
         {
            tokens_.push_back(Token(line, OUT));
         }
         else if (c == '>')
         {
            tokens_.push_back(Token(line, INOUT));
         }
         else
         {
            ungetc(c, f);
            tokens_.push_back(Token(line, LESSTHAN));
         }
      }
      else if (c == '=')
      {
         tokens_.push_back(Token(line, ASSIGN));
      }
      else if (isalpha(c))
      {
         std::string ident(1, c);
         while (isalnum(c = fgetc(f)))
         {
            ident.append(1, c);
         }
         ungetc(c, f);
         if (ident == "alt")
         {
            tokens_.push_back(Token(line, ALTSTART));
         }
         else if (ident == "block")
         {
            tokens_.push_back(Token(line, BLOCKSTART));
         }
         else if (ident == "elsealt")
         {
            tokens_.push_back(Token(line, ALTELSE));
         }
         else if (ident == "endalt")
         {
            tokens_.push_back(Token(line, ALTEND));
         }
         else if (ident == "endblock")
         {
            tokens_.push_back(Token(line, BLOCKEND));
         }
         else if (ident == "endpar")
         {
            tokens_.push_back(Token(line, PAREND));
         }
         else if (ident == "endwhile")
         {
            tokens_.push_back(Token(line, WHILEEND));
         }
         else if (ident == "par")
         {
            tokens_.push_back(Token(line, PARSTART));
         }
         else if (ident == "while")
         {
            tokens_.push_back(Token(line, WHILESTART));
         }
         else if (ident == "true" || ident == "false")
         {
            tokens_.push_back(Token(line, LITERAL, "bool", ident));
         }
         else if (ident == "not")
         {
            tokens_.push_back(Token(line, NOT));
         }
         else
         {
            tokens_.push_back(Token(line, IDENTIFIER, ident));
         }
      }
      else if (isdigit(c))
      {
         std::string num(1, c);
         while (isdigit(c = fgetc(f)))
         {
            num.append(1, c);
         }
         ungetc(c, f);
         tokens_.push_back(Token(line, LITERAL, "int", num));
      }
      else
      {
         printf("about to choke on character '%c'\n", c);
         die(tokens_.size() - 1, "syntax error");
      }
   }
}

Token TokenStream::at(size_t index) const
{
   if (index < tokens_.size())
   {
      return tokens_.at(index);
   }
   else
   {
      return Token(0, NONE);
   }
}

void TokenStream::die(size_t index, const std::string& msg) const
{
   if (index < tokens_.size())
   {
      tokens_.at(index).die(msg);
   }
   else
   {
      tokens_.at(tokens_.size() - 1).die(msg);
   }
}

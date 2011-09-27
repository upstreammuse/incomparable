TEMPLATE = app
CONFIG -= app_bundle qt
QMAKE_CXXFLAGS += -Wextra -Werror
SOURCES = \
   alt.cpp \
   assign.cpp \
   block.cpp \
   call.cpp \
   expression.cpp \
   inc.cpp \
   par.cpp \
   program.cpp \
   statement.cpp \
   sync.cpp \
   term.cpp \
   token.cpp \
   tokenstream.cpp \
   variabletable.cpp \
   while.cpp
TARGET = inc

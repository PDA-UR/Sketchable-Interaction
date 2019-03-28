/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_HOME_JUERGEN_1_DEV_PROJECTS_SKETCHABLE_INTERACTION_SILANG_COMPILER_C_CMAKE_BUILD_DEBUG_PARSER_H_INCLUDED
# define YY_YY_HOME_JUERGEN_1_DEV_PROJECTS_SKETCHABLE_INTERACTION_SILANG_COMPILER_C_CMAKE_BUILD_DEBUG_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    identifier = 258,
    number = 259,
    assign_property = 260,
    assign_value = 261,
    region = 262,
    shape = 263,
    type = 264,
    present = 265,
    blueprint = 266
  };
#endif
/* Tokens.  */
#define identifier 258
#define number 259
#define assign_property 260
#define assign_value 261
#define region 262
#define shape 263
#define type 264
#define present 265
#define blueprint 266

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 18 "/home/juergen/1_dev/projects/Sketchable-Interaction/silang-compiler/c/parser.y" /* yacc.c:1921  */
 char* strval; int num; int ivar;

#line 83 "/home/juergen/1_dev/projects/Sketchable-Interaction/silang-compiler/c/cmake-build-debug/parser.h" /* yacc.c:1921  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_HOME_JUERGEN_1_DEV_PROJECTS_SKETCHABLE_INTERACTION_SILANG_COMPILER_C_CMAKE_BUILD_DEBUG_PARSER_H_INCLUDED  */

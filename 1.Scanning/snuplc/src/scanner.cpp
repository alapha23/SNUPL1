//------------------------------------------------------------------------------
/// @brief SnuPL/1 scanner
/// @author Bernhard Egger <bernhard@csap.snu.ac.kr>
/// @section changelog Change Log
/// 2012/09/14 Bernhard Egger created
/// 2013/03/07 Bernhard Egger adapted to SnuPL/0
/// 2014/09/10 Bernhard Egger assignment 1: scans SnuPL/-1
/// 2016/03/13 Bernhard Egger assignment 1: adapted to modified SnuPL/-1 syntax
///
/// @section license_section License
/// Copyright (c) 2012-2017, Computer Systems and Platforms Laboratory, SNU
/// All rights reserved.
///
/// Redistribution and use in source and binary forms,  with or without modifi-
/// cation, are permitted provided that the following conditions are met:
///
/// - Redistributions of source code must retain the above copyright notice,
///   this list of conditions and the following disclaimer.
/// - Redistributions in binary form must reproduce the above copyright notice,
///   this list of conditions and the following disclaimer in the documentation
///   and/or other materials provided with the distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,  BUT NOT LIMITED TO,  THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY  AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER  OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT,  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSE-
/// QUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF  SUBSTITUTE
/// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
/// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT
/// LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY WAY
/// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
/// DAMAGE.
//------------------------------------------------------------------------------

#include <iostream>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdio>

#include "scanner.h"
using namespace std;

//------------------------------------------------------------------------------
// token names
//
#define TOKEN_STRLEN 16

char ETokenName[][TOKEN_STRLEN] = {
  "tIdent",                         ///< ident
  "tNumber",                          ///< number
  "tBoolConst",                       ///< boolean constant
  "tCharConst",                       ///< character constant
  "tString",                          ///< string constant
  "tPlusMinus",                       ///< '+' or '-'
  "tMulDiv",                          ///< '*' or '/'
  "tOr",                              ///< '||'
  "tAnd",                             ///< '&&'
  "tNot",                             ///< '!'
  "tRelOp",                           ///< relational operator
  "tAssign",                          ///< assignment operator
  "tComma",                           ///< a comma
  "tSemicolon",                       ///< a semicolon
  "tColon",                           ///< a colon
  "tDot",                             ///< a dot
  "tLParens",                         ///< a left parenthesis
  "tRParens",                         ///< a right parenthesis
  "tLBrak",                           ///< a left bracket
  "tRBrak",                           ///< a right bracket

  "tModule",                          ///< 'module'
  "tProcedure",                       ///< 'procedure'
  "tFunction",                        ///< 'function'
  "tVarDecl",                         ///< 'var'
  "tInteger",                         ///< 'integer'
  "tBoolean",                         ///< 'boolean'
  "tChar",                            ///< 'char'
  "tBegin",                           ///< 'begin'
  "tEnd",                             ///< 'end'
  "tIf",                              ///< 'if'
  "tThen",                            ///< 'then'
  "tElse",                            ///< 'else'
  "tWhile",                           ///< 'while'
  "tDo",                              ///< 'do'
  "tReturn",                          ///< 'return'

  "tComment",                         ///< comment ('// .... \n')
  "tEOF",                             ///< end of file
  "tIOError",                         ///< I/O error
  "tUndefined",                       ///< undefined
  
};


//------------------------------------------------------------------------------
// format strings used for printing tokens
//

char ETokenStr[][TOKEN_STRLEN] = {
  "tIdent (%s)",                         ///< ident
  "tNumber (%s)",                          ///< number
  "tBoolConst (%s)",                       ///< boolean constant
  "tCharConst (%s)",                       ///< character constant
  "tString (%s)",                          ///< string constant
  "tPlusMinus (%s)",                       ///< '+' or '-'
  "tMulDiv (%s)",                          ///< '*' or '/'
  "tOr",                              ///< '||'
  "tAnd",                             ///< '&&'
  "tNot",                             ///< '!'
  "tRelOp (%s)",                           ///< relational operator
  "tAssign",                          ///< assignment operator
  "tComma",                           ///< a comma
  "tSemicolon",                       ///< a semicolon
  "tColon",                           ///< a colon
  "tDot",                             ///< a dot
  "tLParens",                         ///< a left parenthesis
  "tRParens",                         ///< a right parenthesis
  "tLBrak",                           ///< a left bracket
  "tRBrak",                           ///< a right bracket

  "tModule",                          ///< 'module'
  "tProcedure",                       ///< 'procedure'
  "tFunction",                        ///< 'function'
  "tVarDecl",                         ///< 'var'
  "tInteger",                         ///< 'integer'
  "tBoolean",                         ///< 'boolean'
  "tChar",                            ///< 'char'
  "tBegin",                           ///< 'begin'
  "tEnd",                             ///< 'end'
  "tIf",                              ///< 'if'
  "tThen",                            ///< 'then'
  "tElse",                            ///< 'else'
  "tWhile",                           ///< 'while'
  "tDo",                              ///< 'do'
  "tReturn",                          ///< 'return'

  "tComment",                         ///< comment ('// .... \n')
  "tEOF",                             ///< end of file
  "tIOError",                         ///< I/O error
  "tUndefined (%s)",                       ///< undefined
    
};


//------------------------------------------------------------------------------
// reserved keywords
//
pair<const char*, EToken> Keywords[] =
{
};



//------------------------------------------------------------------------------
// CToken
//
CToken::CToken()
{
  _type = tUndefined;
  _value = "";
  _line = _char = 0;
}

CToken::CToken(int line, int charpos, EToken type, const string value)
{
  _type = type;
  _value = escape(value);
  _line = line;
  _char = charpos;
}

CToken::CToken(const CToken &token)
{
  _type = token.GetType();
  _value = token.GetValue();
  _line = token.GetLineNumber();
  _char = token.GetCharPosition();
}

CToken::CToken(const CToken *token)
{
  _type = token->GetType();
  _value = token->GetValue();
  _line = token->GetLineNumber();
  _char = token->GetCharPosition();
}

const string CToken::Name(EToken type)
{
  return string(ETokenName[type]);
}

const string CToken::GetName(void) const
{
  return string(ETokenName[GetType()]);
}

ostream& CToken::print(ostream &out) const
{
  int str_len = _value.length();
  str_len = TOKEN_STRLEN + (str_len < 64 ? str_len : 64);
  char *str = (char*)malloc(str_len);
  snprintf(str, str_len, ETokenStr[GetType()], _value.c_str());
  out << dec << _line << ":" << _char << ": " << str;
  free(str);
  return out;
}

string CToken::escape(const string text)
{
  const char *t = text.c_str();
  string s;

  while (*t != '\0') {
    switch (*t) {
      case '\n': s += "\\n";  break;
      case '\t': s += "\\t";  break;
      case '\0': s += "\\0";  break;
      case '\'': s += "\\'";  break;
      case '\"': s += "\\\""; break;
      case '\\': s += "\\\\"; break;
      default :  s += *t;
    }
    t++;
  }

  return s;
}

ostream& operator<<(ostream &out, const CToken &t)
{
  return t.print(out);
}

ostream& operator<<(ostream &out, const CToken *t)
{
  return t->print(out);
}


//------------------------------------------------------------------------------
// CScanner
//
map<string, EToken> CScanner::keywords;

CScanner::CScanner(istream *in)
{
  InitKeywords();
  _in = in;
  _delete_in = false;
  _line = _char = 1;
  _token = NULL;
  _good = in->good(); 
  _temp = "";
  NextToken();
}

CScanner::CScanner(string in)
{
  InitKeywords();
  _in = new istringstream(in);
  _delete_in = true;
  _line = _char = 1;
  _token = NULL;
  _good = true;
  _temp = "";
  NextToken();
}

CScanner::~CScanner()
{
  if (_token != NULL) delete _token;
  if (_delete_in) delete _in;
}

void CScanner::InitKeywords(void)
{
  if (keywords.size() == 0) {
    int size = sizeof(Keywords) / sizeof(Keywords[0]);
    for (int i=0; i<size; i++) {
      keywords[Keywords[i].first] = Keywords[i].second;
    }
  }
}

CToken CScanner::Get()
{
  CToken result(_token);

  EToken type = _token->GetType();
  _good = !(type == tIOError);

  NextToken();
  return result;
}

CToken CScanner::Peek() const
{
  return CToken(_token);
}

void CScanner::NextToken()
{
  if (_token != NULL) delete _token;

  _token = Scan();  
}

void CScanner::RecordStreamPosition()
{
  _saved_line = _line;
  _saved_char = _char;
}

void CScanner::GetRecordedStreamPosition(int *lineno, int *charpos)
{
  *lineno = _saved_line;
  *charpos = _saved_char;
}

CToken* CScanner::NewToken(EToken type, const string token)
{
  return new CToken(_saved_line, _saved_char, type, token);
}

CToken* CScanner::Scan()
{
  EToken token;
  string tokval;
  char c;

  while (_in->good() && IsWhite(_in->peek())) GetChar();

  RecordStreamPosition();

  if (_in->eof()) return NewToken(tEOF);
  if (!_in->good()) return NewToken(tIOError);

  c = GetChar();
  tokval = c;
  token = tUndefined;

__REPARSE:
  if ((c>0x2f) && (c < 0x3a)){
  	token = tNumber;
	tokval = c;
	do{
	if(_in->peek()>0x39 || _in->peek() < 0x30)
		return NewToken(token, tokval);
	c = GetChar();
	tokval += c;
	}while( c>0x2f && c<0x3a);	
        return NewToken(token, tokval);
  }
  switch (c) {
    
    case 'm':
	if(_in->peek() == 'o')
	{
		tokval = "";		
		tokval += GetChar(5);
	}else goto __DEFAULT;
	if (tokval.compare("odule") == 0)	
		token = tModule;
	while (_in->peek() == ' ') GetChar();

	tokval = "";
	while (_in->peek() != '\n') tokval +=GetChar();

		break;
    case 'p':
	if(_in->peek() == 'r')
	{		
		tokval = "";				
		tokval += GetChar(8);
	}else goto __DEFAULT;
	if (tokval.compare("rocedure") == 0)
		token = tProcedure;
	else{
		_temp = "p";
		_temp += tokval;
		goto __DEFAULT;
	}
	tokval = "";
	if (_in->peek() == 32) GetChar();
	break;

    case 'f':
        if(_in->peek() == 'a')
		goto __FALSE;
	if(_in->peek() == 'u')
	{
		tokval = "";		
		tokval += GetChar(7);
	}else goto __DEFAULT;
	if (tokval.compare("unction") == 0)
		token = tFunction;
	while (_in->peek() == ' ') GetChar();
	tokval = "";
	while (_in->peek() != '\n') tokval +=GetChar();
	break;

__FALSE:
	tokval = "";
	tokval += GetChar(4);
	if (tokval.compare("alse")==0)
		token = tBoolConst;
	while(_in->peek() == ' ') GetChar();
	tokval = "";
	while(_in->peek() != '\n') tokval += GetChar();
	tokval = "false";
	break;

    case 'v':
	if(_in->peek() == 'a')
	{
		tokval = "";		
		tokval += GetChar(2);
	}else goto __DEFAULT;
	if (tokval.compare("ar") == 0)
		token = tVarDecl;		
	while (_in->peek() == ' ') GetChar();

	tokval = "";

	break;

    case 'i':
    	if(_in->peek() == 'f')
	{
		token = tIf;
		GetChar();
		break;
	}else if(_in->peek() == 'n')
	{
		tokval = "";		
		tokval += GetChar(6);
	}else goto __DEFAULT; 
	if (tokval.compare("nteger") == 0)
		token = tInteger;
	while (_in->peek() == ' ') GetChar();
	tokval = "";
	break;

     case 'b':
	if(_in->peek() == 'o')
	{
		tokval = "";		
		tokval += GetChar(6);
		goto __BOOLEAN;
	}else if (_in->peek() == 'e')
	{
		tokval = "";
		tokval += GetChar(4);
		goto __BEGIN;
	}else {
		goto __DEFAULT;
	}

__BOOLEAN:
	if (tokval.compare("oolean") == 0)
		token = tBoolean;
	while (_in->peek() == ' ') GetChar();
	tokval = "";
	break;
__BEGIN:
	if (tokval.compare("egin") == 0)
		token = tBegin;
	while (_in->peek() == ' ') GetChar();
	tokval = "";
	while (_in->peek() != '\n') tokval +=GetChar();
	break;

    case 'c':
	if(_in->peek() == 'h')
	{
		tokval = "";		
		tokval += GetChar(3);
	}else goto __DEFAULT;
	if (tokval.compare("har") == 0)
		token = tChar;
	while (_in->peek() == ' ') GetChar();

	tokval = "";
//	while (_in->peek() != '\n') tokval +=GetChar();
	break;


    case 'e':
        if(_in->peek() == 'l')
		goto __ELSE;
	if(_in->peek() == 'n')
	{
		tokval = "";		
		tokval += GetChar(2);
	}else goto __DEFAULT;
	if (tokval.compare("nd") == 0)
		token = tEnd;
	while (_in->peek() == ' ') GetChar();
	tokval = "";
//	while (_in->peek() != '\n') tokval +=GetChar();
	break;
__ELSE:
	tokval ="";
	tokval += GetChar(3);
	if (tokval.compare("lse") == 0)
		token = tElse;
/*	while (_in->peek() == ' ') GetChar();
	tokval = "";
	while (_in->peek() != '\n') tokval +=GetChar();*/
	break;

     case 't':
     	if(_in->peek() == 'r')
		goto __TRUE;	
	if(_in->peek() == 'h')
	{
		tokval = "";		
		tokval += GetChar(3);
	}else goto __DEFAULT;
	if (tokval.compare("hen") == 0)
		token = tThen;
	break;
/*	while (_in->peek() == ' ') GetChar();
	tokval = "";
	while (_in->peek() != '\n') tokval +=GetChar();
	break;*/
__TRUE:
	tokval ="";
	tokval += GetChar(3);
	if (tokval.compare("rue") == 0)
		token = tBoolConst;
	while (_in->peek() == ' ') GetChar();
	tokval = "";
	while (_in->peek() != '\n') tokval +=GetChar();
	tokval = "true";
	break;


     case 'w':
	if(_in->peek() == 'h')
	{
		tokval = "";		
		tokval += GetChar(4);
	}else goto __DEFAULT; 
	if (tokval.compare("hile") == 0)
		token = tWhile;
	while (_in->peek() == ' ') GetChar();
	tokval = "";
	while (_in->peek() != '\n') tokval +=GetChar();
	break;


     case 'r':
	if(_in->peek() == 'e')
	{
		tokval = "";		
		tokval += GetChar(5);
	}else goto __DEFAULT; 
	if (tokval.compare("eturn") == 0)
		token = tReturn;
	while (_in->peek() == ' ') GetChar();
	tokval = "";
	while (_in->peek() != '\n') tokval +=GetChar();
	break;

    case 'd':
	if (_in->peek() == 'o')
	{
		GetChar();
		token = tDo;
		break;
	}

    case ':':
      if (_in->peek() == '=') {
        tokval += GetChar();
        token = tAssign;
      }else token = tColon;
      break;
	
    case '!':
    	token = tNot;
	break;

    case '|':
    	if (_in->peek() != '|'){
	goto __DEFAULT;
	  }
	else {		
		tokval = GetChar();
		tokval += '|';
	}
	token = tOr;
	break;
    case '+':
    case '-':
      token = tPlusMinus;
      break;

    case '&':
    	if (_in->peek() != '&'){
	goto __DEFAULT;
	  }
	else {
		tokval = GetChar();
		tokval += '&';
	}
	token = tAnd;
	break;
    case '*':
      token = tMulDiv;
      break;
    case '/':
    	if (_in->peek() == '/'){
		token = tComment;
		do{
		GetChar();
		}while(_in->peek() != '\n');
		GetChar();
	}else {
		token = tMulDiv;
		tokval = "/";
	}
    break;

    case '=':
    	tokval = "=";
	token = tRelOp;
	break;
    case '#':
    	tokval = "#";
	token = tRelOp;
	break;
    case '<':
    	if (_in->peek() == '=')
		tokval = "<=";
	else tokval = "<";
	token = tRelOp;
	break;
    case '>':
    	if (_in->peek() == '=')
		tokval = ">=";
	else tokval = ">";    
      token = tRelOp;
      break;

    case ';':
      token = tSemicolon;
      break;
    case ',':
      token = tComma;
      break;
    case '.':
      token = tDot;
      break;
    
    case '(':
      token = tLParens;
      break;

    case ')':
      token = tRParens;
      break;

    case '[':
      token = tLBrak;
      break;

    case ']':
      token = tRBrak;
      break;

    case '\"':
    	tokval = "";
	token = tString;
    	while(_in->peek() != '\"'){


		if(_in->peek()=='\\'){
			GetChar();
			char _c = GetChar();		
			switch(_c){
				case 'n':
					tokval += '\n';
					goto __L2;
				case 't':				
					tokval += '\t';			
					goto __L2;
				case '\"':	
					tokval += '\"';		
					goto __L2;
				case '\'':	
					tokval += '\'';	
					goto __L2;
				case '\0':	
					tokval += '\0';
					goto __L2;
				case '\\':			
					tokval += '\\';			
					goto __L2;
				default:
					tokval = "illegal modifier after escape character";
					break;
			}
		}




		tokval += GetChar();

__L2:		
		if(_in->peek() == '\n'){
			tokval = "string const not closed";
			break;
		}
	}
	GetChar();
	break;

    case '\'':
    	tokval = "";
	token = tCharConst;
    	while(_in->peek() != '\''){
		if(_in->peek()=='\\'){
			GetChar();
			char _c = GetChar();		
			switch(_c){
				case 'n':
					tokval += '\n';
					goto __L1;
				case 't':				
					tokval += '\t';
					goto __L1;
				case '\"':	
					tokval += '\"';		
					goto __L1;
				case '\'':	
					tokval += '\'';	
					goto __L1;
				case '\0':	
					tokval += '\0';
					goto __L1;
				case '\\':			
					tokval += '\\';			
					goto __L1;
				default:
					tokval = "illigal modifier after escape character";
					break;
			}
		}
		tokval += GetChar();
		if(_in->peek() != '\''){
			tokval = "length exceed char const";
			do{
				GetChar();
			}while(_in->peek()!='\'' && _in->peek()!='\n' && _in->peek()!=EOF );
			GetChar();
			break;}
__L1:		
		
		if(_in->peek() != '\''){
			tokval = "char const not closed";
			while(_in->peek()!=' ' && _in->peek()!='\n')
				GetChar();
			break;
		}else {
			GetChar();
			break;
		}
	}
	break;
    default:
__DEFAULT:
	if(_temp!=""){
		const char * re_parse = _temp.c_str();
		_temp = _temp.substr(1, _temp.length());
		goto __REPARSE;
	}else{
		tokval = "";
		bool b;
		b = ((c >= 'a') && (c <= 'z')) || (( c >= 'A') && (c <= 'Z') ) || (c=='_') || (( c <= 0x39) && (c >= 0x30)) ;
		if(b){	
			token = tIdent;
			do{
				tokval += c;
				b = ((_in->peek()>=  'a') && (_in->peek() <= 'z')) || (( _in->peek() >= 'A') && ( _in->peek() <= 'Z') ) || (_in->peek()=='_') ||( ( _in->peek() <= 0x39)&& (_in->peek()>= 0x30)) ;
				if (b)	c = GetChar();
				else break;
			}while(1);
		token = tIdent;
		}else{
			token = tUndefined;
			tokval = c;
		}
	}
  }
  return NewToken(token, tokval);
}

char CScanner::GetChar()
{
  char c = _in->get();
  if (c == '\n') { _line++; _char = 1; } else _char++;
  return c;
}

string CScanner::GetChar(int n)
{
  string str;
  for (int i=0; i<n; i++) str += GetChar();
  return str;
}

bool CScanner::IsWhite(char c) const
{
  return ((c == ' ') || (c == '\n'));
}

void CScanner::error(const char *c)
{
	do
	{
	cout << *c;
	c++;
	}while(*c != 0);
	cout << endl;
}

char NEXT(void)
{
	char _next;
	if (_temp=="")
		return _in->peek();
	else 

}


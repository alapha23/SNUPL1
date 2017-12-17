//------------------------------------------------------------------------------
/// @brief SnuPL/0 parser
/// @author Bernhard Egger <bernhard@csap.snu.ac.kr>
/// @section changelog Change Log
/// 2012/09/14 Bernhard Egger created
/// 2013/03/07 Bernhard Egger adapted to SnuPL/0
/// 2014/11/04 Bernhard Egger maintain unary '+' signs in the AST
/// 2016/04/01 Bernhard Egger adapted to SnuPL/1 (this is not a joke)
/// 2016/09/28 Bernhard Egger assignment 2: parser for SnuPL/-1
/// 2017/12/16 Gao Zhiyuan Assignment Finished
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

#include <limits.h>
#include <cassert>
#include <errno.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <exception>

#include "parser.h"
using namespace std;



//------------------------------------------------------------------------------
// CParser
//
CParser::CParser(CScanner *scanner)
{
  _scanner = scanner;
  _module = NULL;
}

CAstNode* CParser::Parse(void)
{
  _abort = false;

  if (_module != NULL) { delete _module; _module = NULL; }

  try {
    if (_scanner != NULL) _module = module();
    if (_module != NULL) {
      CToken t;
      string msg;
      if (!_module->TypeCheck(&t, &msg)) SetError(t, msg);
    }
  } catch (...) {
    _module = NULL;
  }

  return _module;
}

const CToken* CParser::GetErrorToken(void) const
{
  if (_abort) return &_error_token;
  else return NULL;
}

string CParser::GetErrorMessage(void) const
{
  if (_abort) return _message;
  else return "";
}

void CParser::SetError(CToken t, const string message)
{
  _error_token = t;
  _message = message;
  _abort = true;
  throw message;
}

bool CParser::Consume(EToken type, CToken *token)
{
  if (_abort) return false;

  CToken t = _scanner->Get();

  if (t.GetType() != type) {
    SetError(t, "expected '" + CToken::Name(type) + "', got '" +
             t.GetName() + "'");
  }
  if (token != NULL) *token = t;

  return t.GetType() == type;
}

void CParser::InitSymbolTable(CSymtab *s)
{
  CTypeManager *tm = CTypeManager::Get();
  CSymProc *myfun;
 // function DIM(array: pointer to array; dim: integer): integer
  myfun = new CSymProc("DIM", tm->GetInt());
  myfun->AddParam(new CSymParam(0, "arr", tm->GetPointer(tm->GetNull())));
  myfun->AddParam(new CSymParam(1, "dim", tm->GetInt()));
  s->AddSymbol(myfun);
  
  // function DOFS(array: pointer to array): integer;
  myfun = new CSymProc("DOFS", tm->GetInt());
  myfun->AddParam(new CSymParam(0, "arr", tm->GetPointer(tm->GetNull())));
  s->AddSymbol(myfun);
  
  // function ReadInt() : integer;
  myfun = new CSymProc("ReadInt", tm->GetInt());
  s->AddSymbol(myfun);
  
  // procedure WriteInt(i: integer);
  myfun = new CSymProc("WriteInt", tm->GetNull());
  myfun->AddParam(new CSymParam(0, "i", tm->GetInt()));
  s->AddSymbol(myfun);
  
  // procedure WriteChar(c: char);
  myfun = new CSymProc("WriteChar", tm->GetNull());
  myfun->AddParam(new CSymParam(0, "c", tm->GetChar()));
  s->AddSymbol(myfun);
  
  // procedure WriteStr(string: char[]);
  myfun = new CSymProc("WriteStr", tm->GetNull());
  myfun->AddParam(new CSymParam(0, "str", tm->GetPointer(tm->GetArray(CArrayType::OPEN, tm->GetChar()))));
  s->AddSymbol(myfun);
  
  // procedure WriteLn();
  myfun = new CSymProc("WriteLn", tm->GetNull());
  s->AddSymbol(myfun);

  // main;
//  CSymbol* mymain;
//  mymain = new CSymbol("main", stReserved, tm->GetNull());
//  s->AddSymbol(mymain);
}

// added subroutineBody_e
CAstModule* CParser::module(void)
{
  //
  // assignment ::= "module" ident ";" [varDeclartion] {funcDeclaration} 	"begin" statSequence "end" ident "."
  //
  CToken t1;
  CToken t2;
  CToken t3;

  Consume(tModule, &t1);
  Consume(tIdent, &t2);
  Consume(tSemicolon);

  CAstModule *m = new CAstModule(&t1, t2.GetValue());
  InitSymbolTable(m->GetSymbolTable());

  //stat_var(m);
  varDeclaration_e(m);
  while(_scanner->Peek().GetType() == tProcedure || _scanner->Peek().GetType() == tFunction) {
     subroutineDecl(m);
	 //subroutineBody_e(m);	 
  }
  Consume(tBegin);
  CAstStatement *statseq = NULL;
  if(_scanner->Peek().GetType() != tEnd)
  {
    statseq = statSequence(m);
  }
  Consume(tEnd);
  t3 = _scanner->Get();
  Consume(tDot);
  assert(!t2.GetValue().compare(t3.GetValue()));

  m->SetStatementSequence(statseq);
  return m;
}

////////////////////////////////////////////////
void CParser::varDeclaration_e(CAstScope *s)
{
  //
  // varDeclaration ::= [ "var" varDeclSequence ";" ].
  // varDeclSequence ::= varDecl { ";" varDecl }.
  //

  // varDeclaration -> "var" ...
  EToken tt = _scanner->Peek().GetType();
  if (tt == tVarDecl) {
    Consume(tVarDecl);

    // varDeclaration -> ... varDeclSequence ...
    vector<string> allVars;

    // add declared symbol's name
    vector<CSymbol*> symbols = s->GetSymbolTable()->GetSymbols();
    for (const auto &symbol : symbols)
      allVars.push_back(symbol->GetName());

    do {
      vector<string> l;
      CAstType *ttype;

      // varDeclSequence -> ... varDecl ...
      varDecl_e(l, ttype, allVars);

      for (const auto &str : l) {
        CSymbol *var = s->CreateVar(str, ttype->GetType());
        s->GetSymbolTable()->AddSymbol(var);
      }

      // varDeclaration -> ... ";"
      // varDeclSequence -> ... ";" ...
      Consume(tSemicolon);
      tt = _scanner->Peek().GetType();
    } while (tt != tProcedure && tt != tFunction && tt != tBegin);
  }
}

void CParser::varDecl_e(vector<string> &vars, CAstType* &ttype, vector<string> &allVars)
{
  //
  // varDecl ::= ident { "," ident } ":" type.
  //

  // vardecl -> ident { "," ident } ":" ...
  varDeclInternal_e(vars, allVars);

  // varDecl -> ... type
  ttype = type_e(false);
}

void CParser::varDeclParam_e(vector<string> &vars, CAstType* &ttype, vector<string> &allVars)
{
  //
  // varDecl ::= ident { "," ident } ":" type.
  //

  // vardecl -> ident { "," ident } ":" ...
  varDeclInternal_e(vars, allVars);

  // varDecl -> ... type
  ttype = type_e(true);
}

void CParser::varDeclInternal_e(vector<string> &vars, vector<string> &allVars)
{
  //
  // varDecl ::= ident { "," ident } ":" type.
  //

  while (!_abort) {
    // varDecl -> ident ...
    CToken e = _scanner->Get();
    if (e.GetType() != tIdent)
      SetError(e, "invalid identifier");

    for (const auto &var : allVars) {
      if (var == e.GetValue()) {
        SetError(e, "re-declaration variable \"" + e.GetValue() + "\"");
        return;
      }
    }
    vars.push_back(e.GetValue());
    allVars.push_back(e.GetValue());

    e = _scanner->Peek();
    if (e.GetType() == tColon) break;
    else if (e.GetType() != tComma)
      SetError(e, "\":\" or \",\" expected");

    // varDecl -> ... "," ...
    Consume(tComma);
  }

  // varDecl -> ... ":" type
  Consume(tColon);
}

CAstType* CParser::type_e(bool isParam)
{
  //
  // type ::= basetype | type "[" [ number ] "]".
  // basetype ::= "boolean" | "char" | "integer".
  //
  CToken t = _scanner->Peek();
  const CType *ttype = NULL;
  vector<long long> index;
  // varDecl -> ... type
  // functionDecl -> ... type ...
  // type -> basetype
  // basetype -> "boolean" | "char" | "integer"
  if (t.GetValue() == "boolean")
  {
	  Consume(tBoolean);
    ttype = CTypeManager::Get()->GetBool();
  }
  else if (t.GetValue() == "char")
  {
	  Consume(tChar);
    ttype = CTypeManager::Get()->GetChar();
  }
  else if (t.GetValue() == "integer")
  {
	  Consume(tInteger);
    ttype = CTypeManager::Get()->GetInt();
  }
  else
    SetError(t, "invalid base type: " + t.GetValue());

  while (_scanner->Peek().GetType() == tLBrak) {
    // type -> ... "[" ...
    Consume(tLBrak);

    // type -> ... number ...
    if (_scanner->Peek().GetType() != tRBrak) {
      long long indexSize = number()->GetValue();

      if (indexSize < 0 || indexSize >= (1LL << 31))
        SetError(t, "invalid array size: " + indexSize);
      else
        index.push_back(indexSize);
    }
    else if (!isParam)
      SetError(t, "expected 'tNumber', got 'tRBrak'");
    else
      index.push_back(CArrayType::OPEN);
    // type -> ... "]"
    Consume(tRBrak);
  }
  // construct array type
  if (!index.empty()) {
    const CType* innertype = ttype;
    for(int i = (int) index.size() - 1; i >= 0; i--)
      innertype = CTypeManager::Get()->GetArray(index[i], innertype);
    ttype = innertype;
  }
  return new CAstType(t, ttype);
}

/////////////////////////////////////////////////////////////////////////

/********************************************************************/
void CParser::stat_var(CAstScope *m)
{
  // 
  // stat_var ::= ["var" varDeclSequence ";"]
  // varDeclSequence ::= varDecl { ";" varDecl }
  // varDecl  ::= ident { "," ident } ":" type
  //
  if(_scanner->Peek().GetType() == tBegin ||_scanner->Peek().GetType() == tProcedure ||_scanner->Peek().GetType() == tFunction )
    return ;
  Consume(tVarDecl); 
  
  varDeclSequence(m);
  //Consume(tSemicolon);    
  //}while(_scanner->Peek().GetType() == tIdent);
}
void CParser::varDeclSequence(CAstScope *s)
{
  // varDeclSequence ::= varDecl { ";" varDecl }
  do{
      varDecl(s);
    if(_scanner->Peek().GetType() == tSemicolon)
      Consume(tSemicolon);
  } while(_scanner->Peek().GetType() == tIdent);
  
}
void CParser::varDecl(CAstScope *s)
{
  // varDecl  ::= ident { "," ident } ":" type
  // idents are added to the scope

  vector <CToken> t = ident(s);
  Consume(tColon);
  const CType* var_type = type(s);

  for(int i=0; i < t.size(); i++)
  {  
    if( s->GetSymbolTable()->FindSymbol(t.at(i).GetValue(), sLocal) != NULL)
      SetError(_scanner->Peek(), "Redefined vars\n");
    CAstType*  add_type = new CAstType(t.at(i), var_type);
    CSymbol*   add_symbol = s->CreateVar(t.at(i).GetValue(), add_type->GetType());
    s->GetSymbolTable()->AddSymbol(add_symbol);
  }   
}
void CParser::procedureDecl(CAstScope *s)
{
    CToken t;
    Consume(tProcedure, &t);
    CToken proc_name;
    Consume(tIdent, &proc_name);
    // procedure do not have vars
    if(s->GetSymbolTable()->FindSymbol(proc_name.GetValue(), sGlobal))
      SetError(proc_name, "procedure redefinition\n");

    // create and add procedure symbol to parent symbol table
    CSymProc *symbol = new CSymProc(proc_name.GetValue(), CTypeManager::Get()->GetNull());
    s->GetSymbolTable()->AddSymbol(symbol);

    CAstProcedure * ast_proc = new CAstProcedure(t, proc_name.GetValue(), s, symbol);

    // read param

    if(_scanner->Peek().GetType() != tSemicolon)
    {
      Consume(tLParens);
      if(_scanner->Peek().GetType() != tRParens)
      {
        paramDeclSequence(ast_proc);
      }
      Consume(tRParens);
    }
    Consume(tSemicolon); 
    
    stat_var(ast_proc);
    // begin proc sequence
    Consume(tBegin);
    CAstStatement * statseq = statSequence(ast_proc);

    Consume(tEnd);
    if(!(proc_name.GetValue().compare(_scanner->Peek().GetValue())))
    {
      Consume(tIdent);
      Consume(tSemicolon);
    }
    ast_proc->SetStatementSequence(statseq);
}

void CParser::paramDeclSequence(CAstProcedure *s)
{
  do
  {
    paramDecl(s);
    if(_scanner->Peek().GetType() == tSemicolon)
      Consume(tSemicolon); 
/* for(int i = 0; i < s->GetSymbol()->GetNParams(); i++)
    {    // add params to the symbol table
      CSymProc* temp = s->GetSymbol();
      CSymbol* add_symbol = new CSymProc(temp);
      s->GetSymbolTable()->AddSymbol(add_symbol);
    }*/
  }while(_scanner->Peek().GetType() == tIdent);
}


void CParser::paramDecl(CAstProcedure *s)
{
  vector<CToken> t = ident(s);
  Consume(tColon);
  const CType* var_type = type(s);

// check duplication inside param

  for(int i; i < t.size()-1; i++)
  {
    for(int j = i+1; j < t.size(); j++)
    {
        if(!t.at(i).GetValue().compare(t.at(j).GetValue()))
	  SetError(_scanner->Peek(), "Parameters redefined \n");  
    }
  }
  for(int i=0; i < t.size(); i++)
  {  
    CAstType*  add_type = new CAstType(t.at(i), var_type);
    CSymParam*   add_param = new CSymParam(i, t.at(i).GetValue(), add_type->GetType());
//    CSymParam*   add_param = new CSymParam(i, t.at(i).GetValue(), CTypeManager::Get()->GetPointer(var_type));

    s->GetSymbol()->AddParam(add_param);
    // add params to procedure symbol table
    s->GetSymbolTable()->AddSymbol(add_param);
  }   
}

const CType* CParser::type(CAstScope *s)
{
// type ::= basetype | type "[" [number]  "]"
// Arrays need to be implemented
  CToken tt = _scanner->Peek();
  const CType* ttype;
  CToken size;

  switch(tt.GetType())
  {
    case tBoolean:
      Consume(tBoolean);
      ttype =  CTypeManager::Get()->GetBool();
      if(_scanner->Peek().GetType() == tSemicolon)
        return ttype;
      else break;
    case tChar:
      Consume(tChar);
      ttype = CTypeManager::Get()->GetChar();
      if(_scanner->Peek().GetType() == tSemicolon)
        return ttype;
      else break;
    case tInteger:
      Consume(tInteger);
      ttype = CTypeManager::Get()->GetInt();
      if(_scanner->Peek().GetType() == tSemicolon)
        return ttype;
      else break;
    default:
      SetError(tt, "Var basetype are limited to boolean, char or integer\n");
  }
  while(_scanner->Peek().GetType() ==tLBrak)
  {
  Consume(tLBrak);
    if(_scanner->Peek().GetType() == tNumber)
    {
      Consume(tNumber, &size);
      Consume(tRBrak);
      ttype = CTypeManager::Get()->GetArray(stoi(size.GetValue()), ttype);
    }
    else
    {
      Consume(tRBrak);
      SetError(_scanner->Peek(), "Array size not specified\n");
      ttype = CTypeManager::Get()->GetArray(CArrayType::OPEN, ttype);
    }
  }
  return ttype;
}
/*******************************************************************/

void CParser::subroutineDecl(CAstScope *m)
{
  //
  //  subroutineDecl = (procedureDecl | functionDecl) subroutineBody ident ";"
  //  not this one ident "(" [ expression { "," expression} ] ")"
  //

  if(_scanner->Peek().GetType() == tProcedure)
  {
    procedureDecl(m);
  }

  else if(_scanner->Peek().GetType() == tFunction)
  {
    functionDecl(m);
  }
  else if(_scanner->Peek().GetType() == tBegin) return ; 
  else SetError(_scanner->Peek(), "Expected procedure or function");

}

void CParser::functionDecl(CAstScope *s)
{
    CToken t;
    Consume(tFunction, &t);
    CToken proc_name;
    const CType* return_type;
    Consume(_scanner->Peek().GetType(), &proc_name);
    // procedure do not have vars
    if(s->GetSymbolTable()->FindSymbol(proc_name.GetValue(), sGlobal))
      SetError(proc_name, "function redefinition\n");

    // create and add procedure symbol
    CSymProc *symbol = new CSymProc(proc_name.GetValue(), return_type);
    s->GetSymbolTable()->AddSymbol(symbol);
    CAstProcedure * ast_proc = new CAstProcedure(t, proc_name.GetValue(), s, symbol);
   
    // read vars

    if(_scanner->Peek().GetType() != tSemicolon)
    {
      Consume(tLParens);
      if(_scanner->Peek().GetType() != tRParens)
      {
        paramDeclSequence(ast_proc);
      }
      Consume(tRParens);
    }
    Consume(tColon);
    return_type = type(s);
    Consume(tSemicolon);

    stat_var(ast_proc);
    // change procedure symbol with return type
    ast_proc->GetSymbol()->SetDataType(return_type);

    // begin proc sequence
    Consume(tBegin);
    CAstStatement * statseq = statSequence(ast_proc);

    Consume(tEnd);
    // check if the end name is same as the procedure name
    if(!(_scanner->Peek().GetValue().compare(proc_name.GetValue())))
    {
      Consume(tIdent);
      Consume(tSemicolon);
    }
    ast_proc->SetStatementSequence(statseq);
}



// changed: stat_call-> functionCall_e
CAstStatement* CParser::statSequence(CAstScope *s)
{
  //
  // statSequence ::= [ statement { ";" statement } ].
  // statement ::= assignment | subroutineCall | ifStatement | whileStatement | returnStatement
  // 
  //
  CAstStatement *head = NULL; 
  CAstStatement *tail = NULL;
  CAstStatement *st = NULL;
  CToken t;
  EToken tt;

  do {
    t = _scanner->Peek();
    tt = _scanner->Peek().GetType();
      switch (tt) {
        // statement ::= assignment
        case tIdent:
	{
	  const CSymbol *temp = s->GetSymbolTable()->FindSymbol(t.GetValue(), sLocal);
	  if(!temp) temp = s->GetSymbolTable()->FindSymbol(t.GetValue(), sGlobal);	 

	  if(!temp) SetError(t, "Illegal Var");
	  ESymbolType temp_type = temp->GetSymbolType();
	  if(temp_type == stProcedure) st = (subroutineCall_e(s));
	  else st  = assignment(s);
	  //cout << st << endl;
          break;
	}
	// statement ::= stat_while
	case tWhile:
	  st = stat_while(s);
	  break;
	case tReturn:
	  st = stat_return(s);
	  assert(st!=NULL);
          if (head == NULL) head = st;
          else tail->SetNext(st);
          tail = st;
          if (_scanner->Peek().GetType() == tEnd  ) 
          {
	    return head;
          }else 
          {
	   while(! s->GetName().compare(_scanner->Peek().GetValue()))
	     _scanner->Get();
           // perror("Expected end token after return \n");
          }
	case tIf:
	  st = stat_if(s);
	  break;
	case tEnd : 
	  return NULL;

        default:	  
	  cout << t << endl;
          SetError(_scanner->Peek(), "statement expected. This is default");
      }
      assert(st != NULL);
      if (head == NULL) head = st;
      else tail->SetNext(st);
      tail = st;

      tt = _scanner->Peek().GetType();
      if (tt == tDot) break;
      if (_scanner->Peek().GetType() == tEnd || _scanner->Peek().GetType() == tElse ) 
      {
        break;
      }else 
      {
        Consume(tSemicolon);
      }
    }while (!_abort && (tt != tEnd) && (tt != tElse) ) ;
  
  return head;
}

CAstStatIf* 	CParser::stat_if(CAstScope *s)
{
  CToken t;
  CAstExpression* cond = NULL;
  CAstStatement* ifBody = NULL;
  CAstStatement* elseBody = NULL;
  Consume(tIf, &t);
  Consume(tLParens);

  cond = expression(s);

  Consume(tRParens);
  Consume(tThen);
  ifBody = statSequence(s);

  if(_scanner->Peek().GetType() == tElse)
  {
    Consume(tElse);
    elseBody = statSequence(s);
  }
  Consume(tEnd);
  return new CAstStatIf(t, cond, ifBody, elseBody);
}
CAstStatReturn* CParser::stat_return(CAstScope *s)
{
  CToken t;
  CAstExpression *express = NULL;
  Consume(tReturn, &t);

  EToken tt = _scanner->Peek().GetType();
  if(tt == tEnd || tt == tElse)
    SetError(_scanner->Peek(), "Expression expected at return statement\n");
  if(tt!= tEnd && tt != tSemicolon)
    express = expression(s);
  return new CAstStatReturn(t, s, express);
}

CAstStatWhile* CParser::stat_while(CAstScope *s)
{
  //
  // while_stat ::= "while" "(" expression ")" "do" statSequence "end"
  //

  CToken t;
  CAstExpression *cond = NULL;
  CAstStatement *body = NULL;

  Consume(tWhile, &t);
  Consume(tLParens);
  cond = expression(s);
  Consume(tRParens);
  Consume(tDo);
  body = statSequence(s);
  Consume(tEnd);

  return new CAstStatWhile(t, cond, body);
}

// changed
CAstStatAssign* CParser::assignment(CAstScope *s)
{
  //
  // assignment ::= qualident ":=" expression.
  //

  CToken t;
  CAstDesignator *lhs = qualident_e(s);
  Consume(tAssign, &t);
  CAstExpression *rhs = expression(s);

  return new CAstStatAssign(t, lhs, rhs);
}

/**************************************************/
CAstDesignator* CParser::qualident(CAstScope *s)
{
  //
  // qualident ::= ident { "[" expression "]" }
  //
  CAstDesignator* desig;
  CToken t;
  CSymtab *symtab = s->GetSymbolTable();
  Consume(tIdent, &t);

  if( _scanner->Peek().GetType() == tLBrak)
  { 
    Consume(tLBrak);
    const CSymbol* arr_sym =  symtab->FindSymbol(t.GetValue(), sLocal);
    if(arr_sym == NULL)
      arr_sym = symtab->FindSymbol(t.GetValue());
    if(arr_sym == NULL) SetError(t, "token not found in symbol table\n");
    CAstArrayDesignator* arr_parm = new CAstArrayDesignator(t, arr_sym);    

    arr_parm->AddIndex(expression(s));
    Consume(tRBrak);
    
    if(_scanner->Peek().GetType() == tLBrak)
    {
      Consume(tLBrak);
      arr_parm->AddIndex(expression(s));
      Consume(tRBrak);
    }
    arr_parm->IndicesComplete();
    return arr_parm;
  }else
  {  
    const CSymbol *symbol = symtab->FindSymbol(t.GetValue(), sLocal);
    if(!symbol) symbol = symtab->FindSymbol(t.GetValue(), sGlobal);
    assert(symbol!=NULL);
    return new CAstDesignator(t, symbol);
  }
}
CAstExpression* CParser::expression(CAstScope* s)
{
  //
  // expression ::= simpleexpr [ relOp simpleexpr ].
  //
  CToken t;
  EOperation relop;
  CAstExpression *left = NULL, *right = NULL;

  left = simpleexpr(s);
  if (_scanner->Peek().GetType() == tRelOp) {
    Consume(tRelOp, &t);
    right = simpleexpr(s);


    if (t.GetValue() == "=")       relop = opEqual;
    else if (t.GetValue() == "#")  relop = opNotEqual;
    else if (t.GetValue() == "<")  relop = opLessThan;
    else if (t.GetValue() == "<=") relop = opLessEqual;
    else if (t.GetValue() == ">")  relop = opBiggerThan;
    else if (t.GetValue() == ">=") relop = opBiggerEqual;
    else SetError(t, "invalid relation.");

    if(!left->GetType()->Compare(right->GetType()))
    {
      SetError(_scanner->Peek(), "Expression type error\n");
    }
    cout << "left" << left->GetType() << "  right" << right->GetType() << endl;
    return new CAstBinaryOp(t, relop, left, right);
  } else {
    return left;
  }
}

vector <CToken> CParser::ident(CAstScope *s)
{
// ident ::= letter {letter | digit}
// idents are added to a vector
  vector<CToken> t;
  CToken tt;

  do
  {
    Consume(tIdent, &tt);
    t.push_back(tt);
    if(_scanner->Peek().GetType() == tComma)
      Consume(tComma);
  }while(_scanner->Peek().GetType() == tIdent);

  return t;
}

CAstExpression* CParser::simpleexpr(CAstScope *s)
{
  //
  // simpleexpr ::= ["+"|"-"] term { termOp term }.
  //
  CAstExpression *n = NULL;
  if(_scanner->Peek().GetType() == tPlusMinus )  
  {
    CToken t = _scanner->Get();
    n = term(s);
    n = new CAstUnaryOp(t, t.GetValue() == "+"? opPos:opNeg, n);
  }
  else n = term(s);

  while (_scanner->Peek().GetType() == tPlusMinus ||_scanner->Peek().GetType() == tOr)  {
    CToken t;
    CAstExpression *l = n, *r;
    t = _scanner->Peek();
    if(t.GetType() == tOr)
      Consume(tOr, &t);
    else Consume(tPlusMinus, &t);

    r = term(s);

    n = new CAstBinaryOp(t, t.GetValue() == "+" ? opAdd : t.GetType() == tOr ? opOr : opSub, l, r);
  }
  return n;
}

CAstStatCall*  CParser::stat_call(CAstScope *s)
{
  CToken t;
  Consume(tIdent, &t);

  const CSymbol *symbol = s->GetSymbolTable()->FindSymbol(t.GetValue());
  assert(symbol!=NULL);

  CAstFunctionCall* ffff = new CAstFunctionCall(t, dynamic_cast<const CSymProc*>(symbol));  
  Consume(tLParens);
  while(_scanner->Peek().GetType() != tRParens)
  {
    ffff->AddArg(expression(s));
    if(_scanner->Peek().GetType() == tComma)
      Consume(tComma);
  }
  Consume(tRParens);

  if( (dynamic_cast<const CSymProc*>(symbol))->GetNParams() != ffff->GetNArgs())
    SetError(_scanner->Peek(), "Number of arguments mismatched!\n");
  return new CAstStatCall(t, ffff);
}

/**************************************************/

////////////////////////////////////////////////////////////
CAstDesignator* CParser::qualident_e(CAstScope *s)
{  
  // qualident ::= ident { "[" expression "]" }
  //
  CToken t;

  // qualident -> ident ...
  CAstDesignator *id = ident_e(s);

  t = _scanner->Peek();
  if (t.GetType() != tLBrak)
    return id;

  const CToken saveToken = id->GetToken();
  const CSymbol* saveSymbol = id->GetSymbol();
  CAstArrayDesignator *arrayId = new CAstArrayDesignator(saveToken, saveSymbol);

  if (!arrayId->GetType() || !arrayId->GetType()->IsArray()) {
    SetError(t, "access with index which is actually not an array");
    return id;
  }
  free(id);
  while (t.GetType() == tLBrak) {
    // qualident -> ... "[" ...
    Consume(tLBrak);
    // qualident -> ... expression ...
    arrayId->AddIndex(expression_e(s));
    // qualident -> ... "]" ...
    Consume(tRBrak);
    t = _scanner->Peek();
  }
  arrayId->IndicesComplete();
  return arrayId;
}

CAstDesignator* CParser::ident_e(CAstScope *s)
{
  //
  // ident ::= letter { letter | digit }.
  //
  CToken t;

  Consume(tIdent, &t);

  CSymtab *symtab = s->GetSymbolTable();
  const CSymbol *symbol = symtab->FindSymbol(t.GetValue(), sLocal);
  if (!symbol) symbol = symtab->FindSymbol(t.GetValue(), sGlobal);
  if (!symbol) SetError(t, "undeclared variable \"" + t.GetValue() + "\"");

  return new CAstDesignator(t, symbol);
}

CAstExpression* CParser::expression_e(CAstScope* s)
{
  //
  // expression ::= simpleexpr [ relOp simpleexpr ].
  // relOp ::= "=" | "#" | "<" | "<=" | ">" | ">=".
  //
  CToken t;
  EOperation relop;
  CAstExpression *left = NULL, *right = NULL;

  // expression -> simpleexpr ...
  left = simpleexpr_e(s);

  // expression -> ... relOp simpleexpr ...
  if (_scanner->Peek().GetType() == tRelOp) {
    Consume(tRelOp, &t);
    right = simpleexpr(s);

    if (t.GetValue() == "=") relop = opEqual;
    else if (t.GetValue() == "#") relop = opNotEqual;
    else if (t.GetValue() == "<") relop = opLessThan;
    else if (t.GetValue() == "<=") relop = opLessEqual;
    else if (t.GetValue() == ">") relop = opBiggerThan;
    else if (t.GetValue() == ">=") relop = opBiggerEqual;
    else SetError(t, "invalid relation.");

    return new CAstBinaryOp(t, relop, left, right);
  }
  else
    return left;
}

CAstExpression* CParser::simpleexpr_e(CAstScope *s)
{
  //
  // simpleexpr ::= [ "+" | "-" ] term { termOp term }.
  // termOp ::= "+" | "-" | "||".
  //
  CToken t;

  // simpleexpr -> [ "+" | "-" ] ...
  bool hasUnary = false;
  if (_scanner->Peek().GetType() == tPlusMinus) {
    hasUnary = true;
    Consume(tPlusMinus, &t);
  }

  // simpleexpr -> ... term ...
  CAstExpression *n = term(s);
  if (hasUnary)
    n = new CAstUnaryOp(t, t.GetValue() == "+" ? opPos : opNeg, n);

  for (CToken tt = _scanner->Peek(); ; tt = _scanner->Peek()) {
    // termOp -> "+" | "-" | "||"
    EOperation eop;
    if (tt.GetValue() == "||") {		
      Consume(tOr, &tt);
      eop = opOr;
    }
    else if (tt.GetType() == tPlusMinus) {
      Consume(tPlusMinus, &tt);
      eop = tt.GetValue() == "+" ? opAdd : opSub;
    }
    else break;

    // simpleexpr -> ... term ...
    CAstExpression *l = n, *r = term(s);

    n = new CAstBinaryOp(tt, eop, l, r);
  }

  return n;
}

void CParser::subroutineBody_e(CAstScope *s)
{
  //
  // subroutineBody ::= varDeclaration "begin" statSequence "end".
  //
  varDeclaration_e(s);
  Consume(tBegin);  
  CAstStatement *statseq = statSequence(s);
  Consume(tEnd);

  s->SetStatementSequence(statseq);
}

// it is called by procedureDecl
void CParser::formalParam_e
  (vector<string> &paramNames, vector<CAstType*> &paramTypes)
{
  //
  // formalParam ::= "(" [ varDeclSequence ] ")".
  //

  // formalParam -> "(" ...
  Consume(tLParens);

  // formalParam -> ... [ varDeclSequence ] ...
  // varDeclSequence -> varDecl { ";" varDecl }
  CToken e = _scanner->Peek();
  if (e.GetType() == tIdent) {
    do {
      vector<string> l;
      CAstType *ttype;
      varDeclParam_e(l, ttype, paramNames);

      for (int i = 0; i < (int) l.size() ; i++) {
        if (ttype->GetType()->IsArray()) {
          const CPointerType *ptrtype =
            CTypeManager::Get()->GetPointer(ttype->GetType());
          ttype = new CAstType(ttype->GetToken(), ptrtype);
        }
        paramTypes.push_back(ttype);
      }

      e = _scanner->Peek();
      if (e.GetType() == tRParens)
        break;
      else Consume(tSemicolon);
    } while (!_abort);
  }

  // formalParam -> ... ")";
  Consume(tRParens);
}

CAstProcedure* CParser::procedureDecl_e(CAstScope *s)
{
  //
  // procedureDecl ::= "procedure" ident [ formalParam ] ";".
  //
  CToken t;
  // procedureDecl -> "procedure" ...
  Consume(tProcedure, &t);
  // procedureDecl -> ... ident ...
  CToken e = _scanner->Get();
  if (e.GetType() != tIdent)
    SetError(e, "procedure identifier expected");
  // procedureDecl -> ... [ formalParam ] ...
  const string &procedureName = e.GetValue();
  if (s->GetSymbolTable()->FindSymbol(procedureName, sGlobal))
    SetError(e, "re-declaration procedure \"" + procedureName + "\"" );

  vector<string> paramNames;
  vector<CAstType*> paramTypes;

  e = _scanner->Peek();
  switch (e.GetType()) {
    case tLParens:
      formalParam_e(paramNames, paramTypes);
      break;
    case tSemicolon:
      break;
    default:
      SetError(e, "\"(\" or \";\" expected");
      break;
  }
  // procedureDecl -> ... ";"
  Consume(tSemicolon);
  CSymProc *symbol =
    new CSymProc(procedureName, CTypeManager::Get()->GetNull());
  s->GetSymbolTable()->AddSymbol(symbol);
  CAstProcedure *ret = new CAstProcedure(t, procedureName, s, symbol);
  AddParameters_e(ret, symbol, paramNames, paramTypes);
  return ret;
}

void CParser::AddParameters_e  (CAstScope *s, CSymProc *symbol, vector<string> &paramNames, vector<CAstType*> &paramTypes)
{
  int cnt = 0;

  // create parameter symbols and add them to symbol table for subroutine
  for (int i = 0; i < (int) paramNames.size() ; i++) {
    string &paramName = paramNames[i];
    CAstType* paramType = paramTypes[i];

    CSymParam* param = new CSymParam(cnt++, paramName, paramType->GetType());
    symbol->AddParam(param);
    s->GetSymbolTable()->AddSymbol(param);
  }
}

CAstFunctionCall* CParser::functionCall_e(CAstScope *s)
{
  //
  // subroutineCall ::= ident "(" [ expression { "," expression } ] ")".
  //
  CToken t;

  // subroutineCall -> ident ...
  Consume(tIdent, &t);
  const CSymbol *symbol = s->GetSymbolTable()->FindSymbol(t.GetValue(), sGlobal);
  if (!symbol)
    SetError(t, "undeclared subroutine name");

  CAstFunctionCall *func =
    new CAstFunctionCall(t, dynamic_cast<const CSymProc*>(symbol));

  // subroutineCall -> ... "(" ...
  Consume(tLParens);

  while (_scanner->Peek().GetType() != tRParens) {
    // subroutineCall -> ... expression ...
    func->AddArg(addressExpression(s));

    // subroutineCall -> ... "," ...
    if (_scanner->Peek().GetType() == tComma)
        Consume(tComma);
  }

  // subroutineCall -> ... ")"
  Consume(tRParens);

  return func;
}

CAstStatCall* CParser::subroutineCall_e(CAstScope *s)
{
  //
  // subroutineCall ::= ident "(" [ expression { "," expression } ] ")".
  //
  CToken t = _scanner->Peek();
  if (t.GetType() != tIdent)
    SetError(t, "invalid subroutine call");

  CAstFunctionCall *func = functionCall_e(s);

  return new CAstStatCall(t, func);
}

CAstExpression* CParser::addressExpression(CAstScope* s)
{
  //
  // addressExpression ::= "&" expression
  // implicit type casting: array to pointer
  //
  CToken t = _scanner->Peek();
  CAstExpression *e = expression_e(s);

  if (!e->GetType())
    SetError(t, "NULL type");
  else if (e->GetType()->IsArray())
    return new CAstSpecialOp(t, opAddress, e, NULL);

  return e;
}


///////////////////////////////////////////////


CAstExpression* CParser::term(CAstScope *s)
{
  //
  // term ::= factor { ("*"|"/") factor }.
  //
  CAstExpression *n = NULL;

  n = factor(s);

  EToken tt = _scanner->Peek().GetType();
  while ((tt == tMulDiv || tt == tAnd)) {
    CToken t;
    CAstExpression *l = n, *r;
    if(tt == tMulDiv)
      Consume(tMulDiv, &t);
    else Consume(tAnd, &t);
    r = factor(s);
    
    n = new CAstBinaryOp(t, t.GetValue() == "*" ? opMul : t.GetType() == tAnd ? opAnd : opDiv, l, r);
   tt = _scanner->Peek().GetType();
  }

  return n;
}
//changed: stat_call -> functionCall_e
//changed: ident
CAstExpression* CParser::factor(CAstScope *s)
{
  //
  // factor ::= number | "(" expression ")"
  //

  CToken t;
  EToken tt = _scanner->Peek().GetType();
  CAstExpression *unary = NULL, *n = NULL;

  switch (tt) {
    // factor ::= qualident | subroutine
    case tIdent:
	  {
        const CSymbol* sym = s->GetSymbolTable()->FindSymbol(_scanner->Peek().GetValue());
        if (sym) {
          ESymbolType stype = sym->GetSymbolType();
          if (stype == stProcedure)
            n = functionCall_e(s);
          else
            n = qualident_e(s);
        }
        else
          SetError(_scanner->Peek(), "undeclared variable \"" + _scanner->Peek().GetValue() + "\"");
      }
      break;
/*
    {
      t = _scanner->Peek();
      const CSymbol *temp = s->GetSymbolTable()->FindSymbol(t.GetValue(), sGlobal);
      if (temp == NULL)
       temp = s->GetSymbolTable()->FindSymbol(t.GetValue(), sLocal);

      if(!temp) temp=s->GetSymbolTable()->FindSymbol(t.GetValue(), sGlobal);
      if(!temp) SetError(t, "Illegal Var");
      ESymbolType temp_type = temp->GetSymbolType();
      if(temp_type == stProcedure) 
        n = functionCall_e(s)->GetCall();
      else
      {
        n = qualident(s);
      }
      break;
    }
*/
	
    // factor ::= number
    case tNumber:
      n = number();
      break;
    case tBoolConst:
      n = boolean();
      break;
    case tCharConst:
      n = character();
      break;
    case tString:
      n = stringC(s);
      break;
    case tNot:
    {
      Consume(tNot, &t);
      CAstExpression *fuckIhavebeendoingthisfortoolong = factor(s);
      n = new CAstUnaryOp(t, opNot, fuckIhavebeendoingthisfortoolong);
      break;
    }
    // factor ::= "(" expression ")"
    case tLParens:
      Consume(tLParens);
      n = expression(s);
      Consume(tRParens);
      break;

    default:
      SetError(_scanner->Peek(), "factor expected.");
      break;
  }

  return n;
}

CAstConstant* CParser::number(int posflag)
{
  //
  // number ::= digit { digit }.
  //
  // "digit { digit }" is scanned as one token (tNumber)
  //

  CToken t;

  Consume(tNumber, &t);
  
  errno = 0;
  long long v = strtoll(t.GetValue().c_str(), NULL, 10);
  long long max = 2147483647;
  // is number is neg
  if(posflag != 1)
    max++;
  if(v >= max)
    SetError(t, "integer constant outside valid range");
  if (errno != 0) SetError(t, "invalid number.");


  return new CAstConstant(t, CTypeManager::Get()->GetInt(), v);
}

CAstConstant* CParser::boolean(void)
{
  CToken t;
  Consume(tBoolConst, &t);
  long b = t.GetValue().compare("false") ? 1 : 0;
  return new CAstConstant(t, CTypeManager::Get()->GetBool(), b);
}

CAstConstant* CParser::character(void)
{
  CToken c;
  Consume(tCharConst, &c);
  long long character = (long long) CToken::unescape(c.GetValue())[0];
  return new CAstConstant(c, CTypeManager::Get()->GetChar(), character);
}

CAstStringConstant* CParser::stringC(CAstScope* s)
{
  CToken str;
  Consume(tString, &str);
  return new CAstStringConstant(str, str.GetValue(), s);
}


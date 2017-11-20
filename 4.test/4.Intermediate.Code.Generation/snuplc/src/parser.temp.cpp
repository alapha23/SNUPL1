//------------------------------------------------------------------------------
/// @brief SnuPL/0 parser
/// @author Bernhard Egger <bernhard@csap.snu.ac.kr>
/// @section changelog Change Log
/// 2012/09/14 Bernhard Egger created
/// 2013/03/07 Bernhard Egger adapted to SnuPL/0
/// 2014/11/04 Bernhard Egger maintain unary '+' signs in the AST
/// 2016/04/01 Bernhard Egger adapted to SnuPL/1 (this is not a joke)
/// 2016/09/28 Bernhard Egger assignment 2: parser for SnuPL/-1
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
  	printf("Module() complete\n");
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

  // TODO: add predefined functions here
}

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
  cout << m->GetSymbolTable() << endl;
  stat_var(m);
  cout << m->GetSymbolTable() << endl;

  Consume(tBegin);  	
cout << "Consume begin token\n" << endl;
  CAstStatement *statseq = NULL;
  statseq = statSequence(m);
cout << "\n\nstat Sequence parse complete\n\n" << endl;
  Consume(tEnd);
  t3 = _scanner->Get();
  Consume(tDot);
  cout << t3 << endl;
  cout << "t2: " << t2 << endl;
  assert(!t2.GetValue().compare(t3.GetValue()));

  m->SetStatementSequence(statseq);
  return m;
}

void CParser::stat_var(CAstModule *m)
{
  // 
  // stat_var ::= ["var" varDeclSequence ";"]
  // varDeclSequence ::= varDecl { ";" varDecl }
  // varDecl  ::= ident { "," ident } ":" type
  //
  vector<CToken> t;
  EToken type;
VarDecl:
  if(_scanner->Peek().GetType() != tVarDecl)
  	return ;
  Consume(tVarDecl);

  // varDeclSequence
  // varDecl
  CToken t2 = _scanner->Get();
  t.push_back(t2);
  while( _scanner->Peek().GetType() != tColon )
  {
   if( _scanner->Peek().GetType() == tComma)
   	Consume(tComma);
    CToken t3 = _scanner->Peek();
    // Check if redefined
    for(int i = 0; i < t.size(); i++)
    {
    	CToken temp = t.at(i);
	if (!t3.GetValue().compare(temp.GetValue()))
	{
	  SetError(t3,"Redefined vars\n");
	}
    }
    if( _scanner->Peek().GetType() == tComma)
   	Consume(tComma);
    t.push_back(_scanner->Get());
  }
  // get var type
  Consume(tColon);
  // add to symbol table
  // t is the vector
  //CAstType *add_type = stat_type(t, m);
  stat_type(t, m);
  Consume(tSemicolon);
  // execute it again if there is a second line
  if(_scanner->Peek().GetType() == tIdent)
  {	
    goto VarDecl;
  }
  return ;
}

CAstType* CParser::stat_type(vector<CToken> t, CAstModule* m)
{
  //
  // stat_type ::= basetype | type "[" [ number ] "]"
  // basetype ::= "boolean" | "char" | "integer"
  //
  CToken ct;
  CAstType *add_type = NULL;
  CSymbol *add_symbol;

  // varDecl -> ... type
  // function -> .. type ...
  for(int i = 0; i < t.size(); i++ ){
    ct = t.at(i);
    cout << "i = " << i << endl;
    cout << ct << endl;
    cout << "peek token" << _scanner->Peek() << endl;
    switch(_scanner->Peek().GetType()){
      case tInteger:	    
        cout << "Got an int" << endl;
	assert(CTypeManager::Get()->GetInt());
        add_type = new CAstType(ct, CTypeManager::Get()->GetInt());
	assert(add_type->GetType());
	add_symbol = m->CreateVar(ct.GetValue(), add_type->GetType());
        break;
      case tBoolean:
        add_type = new CAstType(ct, CTypeManager::Get()->GetBool());
	add_symbol = m->CreateVar("boolean", add_type->GetType());
        break;
      case tChar:        
	add_type = new CAstType(ct, CTypeManager::Get()->GetChar());
	add_symbol = m->CreateVar("char", add_type->GetType());
        break;
      default:
        SetError(_scanner->Peek(), "Var type not found\n ");
    }
    m->GetSymbolTable()->AddSymbol(add_symbol);
  }
  Consume(_scanner->Peek().GetType());
  while(_scanner->Peek().GetType()==tLBrak)
  {
    Consume(tLBrak);
    Consume(tNumber);
    Consume(tRBrak);    
  }

cout << "Return type "<< endl;
return NULL;
//  return new CAstType(t, add_type);
}

CAstStatement* CParser::statSequence(CAstScope *s)
{
  //
  // statSequence ::= [ statement { ";" statement } ].
  // statement ::= assignment | subroutineCall | ifStatement | whileStatement | returnStatement
  // FIRST(statSequence) = { tNumber }
  // FOLLOW(statSequence) = { tDot }
  // 
  //
cout << "Start sequence\n" << endl;
  CAstStatement *head = NULL; 
  CAstStatement *tail = NULL;
  CAstStatement *st = NULL;
  CToken t;
  EToken tt;

  do {
    cout << "Before switch" << endl;
    t = _scanner->Peek();
    tt = _scanner->Peek().GetType();
    cout << tt << endl;
      switch (tt) {
        // statement ::= assignment
        case tIdent:
	{
	  cout << "Search local table" << endl;
	  cout << s->GetSymbolTable() ;
	  const CSymbol *temp = s->GetSymbolTable()->FindSymbol(t.GetValue(), sLocal);
	  assert(temp);

	  cout << "Search global table" << endl;
	  if(!temp) temp = s->GetSymbolTable()->FindSymbol(t.GetValue(), sGlobal);	 
	  
	  if(!temp) SetError(t, "Illegal Var");
	  ESymbolType temp_type = temp->GetSymbolType();
	  if(temp_type == stProcedure) st = stat_call(s);
	  else st  = assignment(s);
          break;
	}
	// statement ::= stat_while
	case tWhile:
	  st = stat_while(s);
	  break;

        default:	  
          SetError(_scanner->Peek(), "statement expected. This is default");
      }
      
      assert(st != NULL);
      cout << "Got out of the switch" << endl;
      if (head == NULL) head = st;
      else tail->SetNext(st);
      tail = st;

      tt = _scanner->Peek().GetType();
      if (tt == tDot) break;
      cout << "Parsed one line in stat sequence" << endl;
      if (_scanner->Peek().GetType() == tEnd ) 
      {
        cout << "Reach the end of stat sequence" << endl;
        break;
      }else 
      {
        Consume(tSemicolon);
	cout << "Consumed one line and semicolon\n\n\n" << endl;
      }
    }while (!_abort && (tt != tEnd) && (tt != tElse) ) ;
  
  return head;
}

CAstStatCall*  CParser::stat_call(CAstScope *s)
{
  return NULL;
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
  Consume(tEnd);

  return new CAstStatWhile(t, cond, body);
}

CAstStatAssign* CParser::assignment(CAstScope *s)
{
  //
  // assignment ::= qualident ":=" expression.
  //

  CToken t;
  CAstDesignator *lhs = qualident(s);
  Consume(tAssign, &t);
  CAstExpression *rhs = expression(s);

  return new CAstStatAssign(t, lhs, rhs);
}

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
    cout << "left square brace not implemented in qualident" << endl;
    return NULL;
  }else
  {  
    cout << "start qualident recognize" << endl;
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
    else SetError(t, "invalid relation.");

    return new CAstBinaryOp(t, relop, left, right);
  } else {
    return left;
  }
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

  while (_scanner->Peek().GetType() == tPlusMinus) {
    CToken t;
    CAstExpression *l = n, *r;

    Consume(tPlusMinus, &t);

    r = term(s);

    n = new CAstBinaryOp(t, t.GetValue() == "+" ? opAdd : opSub, l, r);
  }


  return n;
}

CAstExpression* CParser::term(CAstScope *s)
{
  //
  // term ::= factor { ("*"|"/") factor }.
  //
  CAstExpression *n = NULL;
  cout << "in term" << _scanner->Peek() <<endl;

  n = factor(s);
  cout << "Bnag" << endl;
  cout << "in term" << _scanner->Peek() <<endl;

  EToken tt = _scanner->Peek().GetType();
cout << tt << endl << endl;
  while ((tt == tMulDiv)) {
    CToken t;
    CAstExpression *l = n, *r;

    Consume(tMulDiv, &t);

    r = factor(s);

    n = new CAstBinaryOp(t, t.GetValue() == "*" ? opMul : opDiv, l, r);

    tt = _scanner->Peek().GetType();
  }

  return n;
}

CAstExpression* CParser::factor(CAstScope *s)
{
  //
  // factor ::= number | "(" expression ")"
  //
  // FIRST(factor) = { tNumber, tLBrak }
  //

  CToken t;
  EToken tt = _scanner->Peek().GetType();
  CAstExpression *unary = NULL, *n = NULL;

  switch (tt) {
    // factor ::= qualident | subroutine
    case tIdent:
    {
      t = _scanner->Peek();
      cout << "Search table for the factor" << endl;
      cout << s->GetSymbolTable();
      const CSymbol *temp = s->GetSymbolTable()->FindSymbol(t.GetValue(), sLocal);
      assert(temp);
      if(!temp) temp=s->GetSymbolTable()->FindSymbol(t.GetValue(), sGlobal);
      if(!temp) SetError(t, "Illegal Var");
      ESymbolType temp_type = temp->GetSymbolType();
      if(temp_type == stProcedure) 
        cout << "procedure factor not implemented" << endl;
      else
      {
        cout << "call qualident in factor recognization" << endl;	
        n = qualident(s);
	cout << "qualident done" << endl;
      }
      break;
    }
    // factor ::= number
    case tNumber:
      n = number();
      break;
    case tBoolConst:
    case tChar:
    case tString:
     cout << "not implemented stub. " << endl;
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
      cout << "got " << _scanner->Peek() << endl;
      SetError(_scanner->Peek(), "factor expected.");
      break;
  }

  return n;
}

CAstConstant* CParser::number(void)
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
  if (errno != 0) SetError(t, "invalid number.");

  return new CAstConstant(t, CTypeManager::Get()->GetInt(), v);
}


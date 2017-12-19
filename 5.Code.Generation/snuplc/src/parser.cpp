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
/// Copyright (c) 2012-2017, Bernhard Egger
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

void CParser::InitSymbolTable(CSymtab *scope)
{
  CTypeManager *tm = CTypeManager::Get();
  CSymProc *myfunction;

  myfunction = new CSymProc("DIM", tm->GetInt());
  myfunction->AddParam(new CSymParam(0, "array", tm->GetPointer(tm->GetNull())));
  myfunction->AddParam(new CSymParam(1, "dimension", tm->GetInt()));
  scope->AddSymbol(myfunction);

  myfunction = new CSymProc("DOFS", tm->GetInt());
  myfunction->AddParam(new CSymParam(0, "array", tm->GetPointer(tm->GetNull())));
  scope->AddSymbol(myfunction);

  myfunction = new CSymProc("WriteStr", tm->GetNull());
  myfunction->AddParam(new CSymParam(0, "str", tm->GetPointer(tm->GetArray(CArrayType::OPEN, tm->GetChar()))));
  scope->AddSymbol(myfunction);

  myfunction = new CSymProc("ReadInt", tm->GetInt());
  scope->AddSymbol(myfunction);

  myfunction = new CSymProc("WriteInt", tm->GetNull());
  myfunction->AddParam(new CSymParam(0, "i", tm->GetInt()));
  scope->AddSymbol(myfunction);

  myfunction = new CSymProc("WriteChar", tm->GetNull());
  myfunction->AddParam(new CSymParam(0, "c", tm->GetChar()));
  scope->AddSymbol(myfunction);

  myfunction = new CSymProc("WriteLn", tm->GetNull());
  scope->AddSymbol(myfunction);
}

CAstModule* CParser::module(void)
{
  //
  // module ::= statSequence  ".".
  //
  CToken t;

  Consume(tModule, &t);
  CToken tModuleIdent = _scanner->Get();
  if (tModuleIdent.GetType() != tIdent)
    SetError(tModuleIdent, "module identifier expected");
  Consume(tSemicolon);

  CAstModule *module = new CAstModule(t, tModuleIdent.GetValue());
  InitSymbolTable(module->GetSymbolTable());

  varDeclaration(module);

  CAstProcedure *sub = NULL;
  EToken tt = _scanner->Peek().GetType();
  while (tt != tBegin) {
    switch (tt) {
      case tProc:
        sub = procedureDecl(module);
        break;

      case tFunc:
        sub = functionDecl(module);
        break;

      default:
        SetError(_scanner->Peek(), "subroutine declaration is invalid!");
        break;
    }

    CAstScope *scope = sub;
    varDeclaration(scope);
    Consume(tBegin);
    CAstStatement *statseq = statSequence(scope);
    Consume(tEnd);

    scope->SetStatementSequence(statseq);

    CToken t = _scanner->Peek();
    if (t.GetType() != tIdent || t.GetValue() != sub->GetName()) {
      string msg = "subroutine identifier is mismatched (\"" + sub->GetName()
                    + "\" != \"" + t.GetValue() + "\")";
      SetError(t, msg);
    }
    Consume(tIdent);
    Consume(tSemicolon);

    tt = _scanner->Peek().GetType();
  }

  Consume(tBegin);
  CAstStatement *statseq = statSequence(module);
  module->SetStatementSequence(statseq);
  Consume(tEnd);

  CToken tModuleIdentClose = _scanner->Get();
  if (tModuleIdent.GetValue() != tModuleIdentClose.GetValue()) {
    string msg = "module identifier is not matched (\"" + tModuleIdent.GetValue()
                + "\" != \"" + tModuleIdentClose.GetValue() + "\")";
    SetError(tModuleIdentClose, msg);
  }
  Consume(tDot);

  return module;
}

void CParser::varDeclaration(CAstScope *scope)
{

  EToken tt = _scanner->Peek().GetType();
  if (tt == tVar) {
    Consume(tVar);

    vector<string> allVars;
    vector<CSymbol*> symbols;
    vector<string> line;
    CAstType *ttype;

    symbols = scope->GetSymbolTable()->GetSymbols();

    for (const auto &symbol : symbols)
      allVars.push_back(symbol->GetName());

    do {

      varDecl(line, ttype, allVars);

      for (const auto &str : line) {
        CSymbol *var = scope->CreateVar(str, ttype->GetType());
        scope->GetSymbolTable()->AddSymbol(var);
      }

      Consume(tSemicolon);
      tt = _scanner->Peek().GetType();
    } while (tt != tProc && tt != tFunc && tt != tBegin);
  }
}

void CParser::varDecl(vector<string> &a, CAstType* &ttype, vector<string> &b)
{

vector<string> &vars = a;
vector<string> &allVars = b;

  CToken element;

  while (!_abort) {
    element = _scanner->Get();

    if (element.GetType() != tIdent)
      SetError(element, "identifier is invalid!");
    for (const auto &var : allVars) {
      if (var == element.GetValue()) {
        SetError(element, "variable needs to be redecalration! \"" + element.GetValue() + "\"");
        return;
      }
    }
    vars.push_back(element.GetValue());
    allVars.push_back(element.GetValue());

    element = _scanner->Peek();
    if (element.GetType() == tColon) break;
    else if (element.GetType() != tComma)
      SetError(element, "\":\" or \",\" expected");

    Consume(tComma);
  }

  Consume(tColon);

  ttype = type(false);
}

void CParser::varDeclParam(vector<string> &a, CAstType* &ttype, vector<string> &b)
{
  
vector<string> &vars = a;
vector<string> &allVars = b;

  CToken element;

  while (!_abort) {
    element = _scanner->Get();

    if (element.GetType() != tIdent)
      SetError(element, "identifier is invalid!");
    for (const auto &var : allVars) {
      if (var == element.GetValue()) {
        SetError(element, "variable needs to be redecalration! \"" + element.GetValue() + "\"");
        return;
      }
    }
    vars.push_back(element.GetValue());
    allVars.push_back(element.GetValue());

    element = _scanner->Peek();
    if (element.GetType() == tColon) break;
    else if (element.GetType() != tComma)
      SetError(element, "\":\" or \",\" expected");

    Consume(tComma);
  }

  Consume(tColon);

  ttype = type(true);
}


CAstProcedure* CParser::procedureDecl(CAstScope *scope)
{
  CToken t;

  Consume(tProc, &t);
  vector<string> paramNames;
  vector<CAstType*> paramTypes;

  CToken element = _scanner->Get();
  if (element.GetType() != tIdent)
    SetError(element, "procedure ident error!");

  const string &procedureName = element.GetValue();
  if (scope->GetSymbolTable()->FindSymbol(procedureName, sGlobal))
    SetError(element, "redeclaration the procedure \"" + procedureName + "\"" );

  element = _scanner->Peek();
  switch (element.GetType()) {
    case tLParen:
      parameter_org(paramNames, paramTypes);
      break;
    case tSemicolon:
      break;
    default:
      SetError(element, "\"(\" or \";\" expected");
      break;
  }

  Consume(tSemicolon);

  CSymProc *symbol = new CSymProc(procedureName, CTypeManager::Get()->GetNull());
  scope->GetSymbolTable()->AddSymbol(symbol);

  CAstProcedure *ret = new CAstProcedure(t, procedureName, scope, symbol);
  plusparameter(ret, symbol, paramNames, paramTypes);

  return ret;
}

CAstProcedure* CParser::functionDecl(CAstScope *scope)
{
  CToken t;
  vector<string> paramNames;
  vector<CAstType*> paramTypes;
  CAstType* returnType;

  Consume(tFunc, &t);

  CToken element = _scanner->Get();
  if (element.GetType() != tIdent)
    SetError(element, "function ident error!");

  const string &functionName = element.GetValue();
  if (scope->GetSymbolTable()->FindSymbol(functionName, sGlobal))
    SetError(element, "redeclare function \"" + functionName + "\"");

  element = _scanner->Peek();
  switch (element.GetType()) {
    case tLParen:
      parameter_org(paramNames, paramTypes);
      break;
    case tColon:
      break;
    default:
      SetError(element, "\"(\" or \":\" expected");
      break;
  }

  Consume(tColon);
  returnType = type(false);
  if (returnType->GetType()->IsArray())
    SetError(returnType->GetToken(), "function can not return array type.");
  Consume(tSemicolon);

  CSymProc *symbol = new CSymProc(functionName, returnType->GetType());
  scope->GetSymbolTable()->AddSymbol(symbol);

  CAstProcedure *ret = new CAstProcedure(t, functionName, scope, symbol);
  plusparameter(ret, symbol, paramNames, paramTypes);

  return ret;
}

void CParser::parameter_org
  (vector<string> &paramNames, vector<CAstType*> &paramTypes)
{

  Consume(tLParen);

  CToken e = _scanner->Peek();
  if (e.GetType() == tIdent) {
    do {
      vector<string> l;
      CAstType *ttype;
      varDeclParam(l, ttype, paramNames);

      for (int i = 0; i < (int) l.size() ; i++) {
        if (ttype->GetType()->IsArray()) {
          const CPointerType *ptrtype =
            CTypeManager::Get()->GetPointer(ttype->GetType());
          ttype = new CAstType(ttype->GetToken(), ptrtype);
        }
        paramTypes.push_back(ttype);
      }

      e = _scanner->Peek();
      if (e.GetType() == tRParen)
        break;
      else Consume(tSemicolon);
    } while (!_abort);
  }

  Consume(tRParen);
}

void CParser::plusparameter
  (CAstScope *scope, CSymProc *symbol, vector<string> &paramNames, vector<CAstType*> &paramTypes)
{
  int cnt = 0;

  for (int i = 0; i < (int) paramNames.size() ; i++) {
    string &paramName = paramNames[i];
    CAstType* paramType = paramTypes[i];

    CSymParam* param = new CSymParam(cnt++, paramName, paramType->GetType());
    symbol->AddParam(param);
    scope->GetSymbolTable()->AddSymbol(param);
  }
}


CAstStatement* CParser::statSequence(CAstScope *scope)
{
  CAstStatement *head = NULL;
  CAstStatement *tail = NULL;

  CToken tt = _scanner->Peek();
  while (!_abort && tt.GetType() != tEnd && tt.GetType() != tElse) {
    CAstStatement *statement = NULL;

    tt = _scanner->Peek();
    switch (tt.GetType()) {

      case tIf:
        statement = ifStatement(scope);
        break;

      case tWhile:
        statement = whileStatement(scope);
        break;

      case tIdent:
        {
          const CSymbol *sym = scope->GetSymbolTable()->FindSymbol(tt.GetValue(), sLocal);
          if (!sym) sym = scope->GetSymbolTable()->FindSymbol(tt.GetValue(), sGlobal);
          if (!sym) SetError(tt, "variable is undeclared ! \"" + tt.GetValue() + "\"");

          ESymbolType stype = sym->GetSymbolType();
          if (stype == stProcedure)
	  {
	    statement = subroutineCall(scope);
	  }
          else statement = assignment(scope);
        }
        break;

      case tReturn:
        statement = returnStatement(scope);
        break;

      default:
        SetError(_scanner->Peek(), "statement expected.");
        break;
    }

    assert(statement);

    if (!head) head = statement;
    else tail->SetNext(statement);

    tail = statement;

    tt = _scanner->Peek();
    if (tt.GetType() == tEnd || tt.GetType() == tElse) break;

    Consume(tSemicolon);
  }

  return head;
}

CAstStatAssign* CParser::assignment(CAstScope *scope)
{
  CToken t;

  CAstDesignator *lhs = qualident(scope);

  Consume(tAssign, &t);

  CAstExpression *rhs = expression(scope);

  return new CAstStatAssign(t, lhs, rhs);
}

CAstStatCall* CParser::subroutineCall(CAstScope *scope)
{
  CToken t = _scanner->Peek();
  if (t.GetType() != tIdent)
    SetError(t, "subroutine call is invalid!");

  CAstFunctionCall *func = functionCall(scope);

  return new CAstStatCall(t, func);
}

CAstFunctionCall* CParser::functionCall(CAstScope *scope)
{
  CToken t;

  Consume(tIdent, &t);
  const CSymbol *symbol = scope->GetSymbolTable()->FindSymbol(t.GetValue(), sGlobal);
  if (!symbol)
    SetError(t, "subroutine name is undeclared!");

  CAstFunctionCall *functioncall = new CAstFunctionCall(t, dynamic_cast<const CSymProc*>(symbol));

  Consume(tLParen);

  while (_scanner->Peek().GetType() != tRParen) {
    functioncall->AddArg(addressExpression(scope));

    if (_scanner->Peek().GetType() == tComma)
        Consume(tComma);
  }

  Consume(tRParen);

  return functioncall;
}

CAstStatIf* CParser::ifStatement(CAstScope *scope)
{
  CToken t;

  Consume(tIf, &t);
  Consume(tLParen);
  CAstExpression *cond = expression(scope);
  Consume(tRParen);
  Consume(tThen);
  CAstStatement *ifBody = statSequence(scope);

  CAstStatement *elseBody = NULL;
  if (_scanner->Peek().GetType() == tElse) {
    Consume(tElse);
    elseBody = statSequence(scope);
  }
  Consume(tEnd);

  return new CAstStatIf(t, cond, ifBody, elseBody);
}

CAstStatWhile* CParser::whileStatement(CAstScope *scope)
{
  CToken t;

  Consume(tWhile, &t);
  Consume(tLParen);
  CAstExpression *cond = expression(scope);
  Consume(tRParen);
  Consume(tDo);
  CAstStatement *body = statSequence(scope);
  Consume(tEnd);

  return new CAstStatWhile(t, cond, body);
}

CAstStatReturn* CParser::returnStatement(CAstScope *scope)
{
  CToken t;
  CAstExpression *expr = NULL;

  Consume(tReturn, &t);
  EToken tt = _scanner->Peek().GetType();
  if (tt != tEnd && tt != tSemicolon && tt != tElse)
    expr = expression(scope);

  return new CAstStatReturn(t, scope, expr);
}

CAstExpression* CParser::expression(CAstScope* scope)
{
  CToken t;
  EOperation relop;
  CAstExpression *left = NULL, *right = NULL;

  left = simpleexpr(scope);

  if (_scanner->Peek().GetType() == tRelOp) {
    Consume(tRelOp, &t);
    right = simpleexpr(scope);

    if (t.GetValue() == "=") relop = opEqual;
    else if (t.GetValue() == "<") relop = opLessThan;
    else if (t.GetValue() == ">") relop = opBiggerThan;
    else if (t.GetValue() == "<=") relop = opLessEqual;
    else if (t.GetValue() == ">=") relop = opBiggerEqual;
    else if (t.GetValue() == "#") relop = opNotEqual;
    else SetError(t, "relation is invalid!.");

    return new CAstBinaryOp(t, relop, left, right);
  }
  else
    return left;
}

CAstExpression* CParser::addressExpression(CAstScope* scope)
{
  CAstExpression *expr = expression(scope);
  CToken t = _scanner->Peek();

  if (!expr->GetType())
    SetError(t, "This is a NULL type!");
  else if (expr->GetType()->IsArray())
    return new CAstSpecialOp(t, opAddress, expr, NULL);

  return expr;
}

CAstExpression* CParser::simpleexpr(CAstScope *scope)
{
  CToken t;

  bool existUnary = false;
  if (_scanner->Peek().GetType() == tPlusMinus) {
    existUnary = true;
    Consume(tPlusMinus, &t);
  }

  CAstExpression *n = term(scope);
  if (existUnary)
    n = new CAstUnaryOp(t, t.GetValue() == "+" ? opPos : opNeg, n);

  for (CToken tt = _scanner->Peek(); ; tt = _scanner->Peek()) {
    EOperation eop;
    if (tt.GetValue() == "||") {
      Consume(tAndOr, &tt);
      eop = opOr;
    }
    else if (tt.GetType() == tPlusMinus) {
      Consume(tPlusMinus, &tt);
      eop = tt.GetValue() == "+" ? opAdd : opSub;
    }
    else break;

    CAstExpression *l = n, *r = term(scope);

    n = new CAstBinaryOp(tt, eop, l, r);
  }

  return n;
}

CAstExpression* CParser::term(CAstScope *scope)
{
  CAstExpression *n = NULL;
  n = factor(scope);

  for (CToken t = _scanner->Peek(); ; t = _scanner->Peek()) {
    EOperation eop;
    if (t.GetValue() == "&&") {
      Consume(tAndOr, &t);
      eop = opAnd;
    }
    else if (t.GetType() == tMulDiv) {
      Consume(tMulDiv, &t);
      eop = t.GetValue() == "*" ? opMul : opDiv;
    }
    else break;

    CAstExpression *l = n, *r = factor(scope);

    n = new CAstBinaryOp(t, eop, l, r);
  }

  return n;
}

CAstExpression* CParser::factor(CAstScope *scope)
{
  CToken t;
  CToken tt = _scanner->Peek();
  CAstExpression *unary = NULL, *n = NULL;

  switch (tt.GetType()) {
    case tNumber:
      n = mynumfunction();
      break;

    case tBool:
      n = myboolfunction();
      break;

    case tChar:
      n = mycharfunction();
      break;

    case tString:
      n = stringConst(scope);
      break;

    case tLParen:
      Consume(tLParen);
      n = expression(scope);
      Consume(tRParen);
      break;

    case tIdent:
      {
        const CSymbol* sym = scope->GetSymbolTable()->FindSymbol(tt.GetValue());
        if (sym) {
          ESymbolType stype = sym->GetSymbolType();
          if (stype == stProcedure)
            n = functionCall(scope);
          else
            n = qualident(scope);
        }
        else
          SetError(tt, "undeclared variable \"" + tt.GetValue() + "\"");
      }
      break;

    case tNot:
      {
        Consume(tNot, &t);
        CAstExpression *e = factor(scope);
        n = new CAstUnaryOp(t, opNot, e);
        break;
      }

    default:
      SetError(_scanner->Peek(), "factor expected.");
      break;
  }

  return n;
}

CAstType* CParser::type(bool isParam)
{
  //
  // type ::= basetype | type "[" [ number ] "]".
  // basetype ::= "boolean" | "char" | "integer".
  //
  CToken t;
  const CType *ttype = NULL;
  vector<long long> index;

  Consume(tType, &t);

  if (t.GetValue() == "boolean")
    ttype = CTypeManager::Get()->GetBool();
  else if (t.GetValue() == "char")
    ttype = CTypeManager::Get()->GetChar();
  else if (t.GetValue() == "integer")
    ttype = CTypeManager::Get()->GetInt();
  else
    SetError(t, "invalid base type: " + t.GetValue());

  while (_scanner->Peek().GetType() == tLBrak) {
    // type -> ... "[" ...
    Consume(tLBrak);

    // type -> ... number ...
    if (_scanner->Peek().GetType() != tRBrak) {
      long long indexSize = mynumfunction()->GetValue();

      if (indexSize < 0 || indexSize >= (1LL << 31))
        SetError(t, "invalid array size: " + indexSize);
      else
        index.push_back(indexSize);
    }
    else if (!isParam)
      SetError(t, "expected 'tNumber', got 'tRBrak'");
    else
      index.push_back(CArrayType::OPEN);


    Consume(tRBrak);
  }

  if (!index.empty()) {
    const CType* innertype = ttype;
    for(int i = (int) index.size() - 1; i >= 0; i--)
      innertype = CTypeManager::Get()->GetArray(index[i], innertype);
    ttype = innertype;
  }

  return new CAstType(t, ttype);
}

CAstDesignator* CParser::qualident(CAstScope *scope)
{
  CToken t;

  CAstDesignator *id = ident(scope);

  t = _scanner->Peek();
  if (t.GetType() != tLBrak)
    return id;

  const CToken saveToken = id->GetToken();
  const CSymbol* saveSymbol = id->GetSymbol();
  CAstArrayDesignator *arrayId = new CAstArrayDesignator(saveToken, saveSymbol);

  if (!arrayId->GetType() || !arrayId->GetType()->IsArray()) {
    SetError(t, " without array index!");
    return id;
  }

  free(id);

  while (t.GetType() == tLBrak) {
    Consume(tLBrak);

    arrayId->AddIndex(expression(scope));

    Consume(tRBrak);
    t = _scanner->Peek();
  }

  arrayId->IndicesComplete();
  return arrayId;
}

CAstDesignator* CParser::ident(CAstScope *scope)
{
  CToken t;

  Consume(tIdent, &t);

  CSymtab *symtab = scope->GetSymbolTable();
  const CSymbol *symbol = symtab->FindSymbol(t.GetValue(), sLocal);
  if (!symbol) symbol = symtab->FindSymbol(t.GetValue(), sGlobal);
  if (!symbol) SetError(t, "variable is undeclared! \"" + t.GetValue() + "\"");

  return new CAstDesignator(t, symbol);
}

CAstConstant* CParser::mynumfunction(void)
{
  CToken t;

  Consume(tNumber, &t);

  errno = 0;
  long long v = strtoll(t.GetValue().c_str(), NULL, 10);
  if (errno != 0) SetError(t, "number is invalid!. (" + t.GetValue() + ")");

  long long absv = (v > 0 ? v : (-v));
  if (absv > (1LL << 31)) SetError(t, "number is invalid!. (" + t.GetValue() + ")");

  return new CAstConstant(t, CTypeManager::Get()->GetInt(), v);
}

CAstConstant* CParser::myboolfunction(void)
{
  CToken t;

  Consume(tBool, &t);
  long long b = (t.GetValue() == "false") ? 0 : 1;

  return new CAstConstant(t, CTypeManager::Get()->GetBool(), b);
}

CAstConstant* CParser::mycharfunction(void)
{
  CToken t;

  Consume(tChar, &t);
  long long ch = (long long) CToken::unescape(t.GetValue())[0];

  return new CAstConstant(t, CTypeManager::Get()->GetChar(), ch);
}

CAstStringConstant* CParser::stringConst(CAstScope* scope)
{
  CToken t;

  Consume(tString, &t);

  return new CAstStringConstant(t, t.GetValue(), scope);
}


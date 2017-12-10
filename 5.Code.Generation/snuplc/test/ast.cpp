//------------------------------------------------------------------------------
/// @brief SnuPL abstract syntax tree
/// @author Bernhard Egger <bernhard@csap.snu.ac.kr>
/// @section changelog Change Log
/// 2012/09/14 Bernhard Egger created
/// 2013/05/22 Bernhard Egger reimplemented TAC generation
/// 2013/11/04 Bernhard Egger added typechecks for unary '+' operators
/// 2016/03/12 Bernhard Egger adapted to SnuPL/1
/// 2014/04/08 Bernhard Egger assignment 2: AST for SnuPL/-1
/// 2017/09/23 Bernhard Egger assignment 2: minor bugfixes
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
#include <cassert>
#include <cstring>

#include <typeinfo>

#include "ast.h"
using namespace std;


//------------------------------------------------------------------------------
// CAstNode
//
int CAstNode::_global_id = 0;

CAstNode::CAstNode(CToken token)
  : _token(token), _addr(NULL)
{
  _id = _global_id++;
}

CAstNode::~CAstNode(void)
{
  if (_addr != NULL) delete _addr;
}

int CAstNode::GetID(void) const
{
  return _id;
}

CToken CAstNode::GetToken(void) const
{
  return _token;
}

const CType* CAstNode::GetType(void) const
{
  return CTypeManager::Get()->GetNull();
}

string CAstNode::dotID(void) const
{
  ostringstream out;
  out << "node" << dec << _id;
  return out.str();
}

string CAstNode::dotAttr(void) const
{
  return " [label=\"" + dotID() + "\"]";
}

void CAstNode::toDot(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << dotID() << dotAttr() << ";" << endl;
}

CTacAddr* CAstNode::GetTacAddr(void) const
{
  return _addr;
}

ostream& operator<<(ostream &out, const CAstNode &t)
{
  return t.print(out);
}

ostream& operator<<(ostream &out, const CAstNode *t)
{
  return t->print(out);
}

//------------------------------------------------------------------------------
// CAstScope
//
CAstScope::CAstScope(CToken t, const string name, CAstScope *parent)
  : CAstNode(t), _name(name), _symtab(NULL), _parent(parent), _statseq(NULL),
    _cb(NULL)
{
  if (_parent != NULL) _parent->AddChild(this);
}

CAstScope::~CAstScope(void)
{
  delete _symtab;
  delete _statseq;
  delete _cb;
}

const string CAstScope::GetName(void) const
{
  return _name;
}

CAstScope* CAstScope::GetParent(void) const
{
  return _parent;
}

size_t CAstScope::GetNumChildren(void) const
{
  return _children.size();
}

CAstScope* CAstScope::GetChild(size_t i) const
{
  assert(i < _children.size());
  return _children[i];
}

CSymtab* CAstScope::GetSymbolTable(void) const
{
  assert(_symtab != NULL);
  return _symtab;
}

void CAstScope::SetStatementSequence(CAstStatement *statseq)
{
  _statseq = statseq;
}

CAstStatement* CAstScope::GetStatementSequence(void) const
{
  return _statseq;
}

bool CAstScope::TypeCheck(CToken *t, string *msg) const
{
  bool result = true;

  try {
    CAstStatement *s = _statseq;
    while(result && (s!=NULL)){
      result = s->TypeCheck(t, msg);
      s = s->GetNext();
    }
    vector<CAstScope*>::const_iterator itr = _children.begin();
    while(result && (itr!= _children.end())){
      result = (*itr)->TypeCheck(t, msg);
      itr++;
    }  
  }catch(...)
  {
    result = false;
  }
  return result;
}

ostream& CAstScope::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << "CAstScope: '" << _name << "'" << endl;
  out << ind << "  symbol table:" << endl;
  _symtab->print(out, indent+4);
  out << ind << "  statement list:" << endl;
  CAstStatement *s = GetStatementSequence();
  if (s != NULL) {
    do {
      s->print(out, indent+4);
      s = s->GetNext();
    } while (s != NULL);
  } else {
    out << ind << "    empty." << endl;
  }

  out << ind << "  nested scopes:" << endl;
  if (_children.size() > 0) {
    for (size_t i=0; i<_children.size(); i++) {
      _children[i]->print(out, indent+4);
    }
  } else {
    out << ind << "    empty." << endl;
  }
  out << ind << endl;

  return out;
}

void CAstScope::toDot(ostream &out, int indent) const
{
  string ind(indent, ' ');

  CAstNode::toDot(out, indent);

  CAstStatement *s = GetStatementSequence();
  if (s != NULL) {
    string prev = dotID();
    do {
      s->toDot(out, indent);
      out << ind << prev << " -> " << s->dotID() << " [style=dotted];" << endl;
      prev = s->dotID();
      s = s->GetNext();
    } while (s != NULL);
  }

  vector<CAstScope*>::const_iterator it = _children.begin();
  while (it != _children.end()) {
    CAstScope *s = *it++;
    s->toDot(out, indent);
    out << ind << dotID() << " -> " << s->dotID() << ";" << endl;
  }

}

CTacAddr* CAstScope::ToTac(CCodeBlock *cb)
{
  assert(cb);
  CAstStatement *s = GetStatementSequence();
  while(s)
  {
    CTacLabel *n = cb->CreateLabel();
    s->ToTac(cb, n);
    cb->AddInstr(n);
    // add label to the statement
    s = s->GetNext();
    // parse all the statements till end of the scope
  }
  cb->CleanupControlFlow();
  return NULL;
}

CCodeBlock* CAstScope::GetCodeBlock(void) const
{
  return _cb;
}

void CAstScope::SetSymbolTable(CSymtab *st)
{
  if (_symtab != NULL) delete _symtab;
  _symtab = st;
}

void CAstScope::AddChild(CAstScope *child)
{
  _children.push_back(child);
}


//------------------------------------------------------------------------------
// CAstModule
//
CAstModule::CAstModule(CToken t, const string name)
  : CAstScope(t, name, NULL)
{
  SetSymbolTable(new CSymtab());
}

CSymbol* CAstModule::CreateVar(const string ident, const CType *type)
{
  return new CSymGlobal(ident, type);
}

string CAstModule::dotAttr(void) const
{
  return " [label=\"m " + GetName() + "\",shape=box]";
}



//------------------------------------------------------------------------------
// CAstProcedure
//
CAstProcedure::CAstProcedure(CToken t, const string name,
                             CAstScope *parent, CSymProc *symbol)
  : CAstScope(t, name, parent), _symbol(symbol)
{
  assert(GetParent() != NULL);
  SetSymbolTable(new CSymtab(GetParent()->GetSymbolTable()));
  assert(_symbol != NULL);
}

CSymProc* CAstProcedure::GetSymbol(void) const
{
  return _symbol;
}

CSymbol* CAstProcedure::CreateVar(const string ident, const CType *type)
{
  return new CSymLocal(ident, type);
}

const CType* CAstProcedure::GetType(void) const
{
  return GetSymbol()->GetDataType();
}

string CAstProcedure::dotAttr(void) const
{
  return " [label=\"p/f " + GetName() + "\",shape=box]";
}


//------------------------------------------------------------------------------
// CAstType
//
CAstType::CAstType(CToken t, const CType *type)
  : CAstNode(t), _type(type)
{
  assert(type != NULL);
}

const CType* CAstType::GetType(void) const
{
  return _type;
}

ostream& CAstType::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << "CAstType (" << _type << ")" << endl;
  return out;
}


//------------------------------------------------------------------------------
// CAstStatement
//
CAstStatement::CAstStatement(CToken token)
  : CAstNode(token), _next(NULL)
{
}

CAstStatement::~CAstStatement(void)
{
  delete _next;
}

void CAstStatement::SetNext(CAstStatement *next)
{
  _next = next;
}

CAstStatement* CAstStatement::GetNext(void) const
{
  return _next;
}

CTacAddr* CAstStatement::ToTac(CCodeBlock *cb, CTacLabel *next)
{
  
  cb->AddInstr(new CTacInstr(opGoto, next));
  return NULL;
}


//------------------------------------------------------------------------------
// CAstStatAssign
//
CAstStatAssign::CAstStatAssign(CToken t,
                               CAstDesignator *lhs, CAstExpression *rhs)
  : CAstStatement(t), _lhs(lhs), _rhs(rhs)
{
  assert(lhs != NULL);
  assert(rhs != NULL);
}

CAstDesignator* CAstStatAssign::GetLHS(void) const
{
  return _lhs;
}

CAstExpression* CAstStatAssign::GetRHS(void) const
{
  return _rhs;
}

bool CAstStatAssign::TypeCheck(CToken *t, string *msg) const
{
  CAstDesignator *lhs = GetLHS();
  CAstExpression *rhs = GetRHS();

  if(!lhs->GetType() || !lhs->GetType()->IsScalar() || !rhs->GetType() ||!rhs->GetType()->IsScalar())
  {
    perror("Left or right hand side of statement cannot be scalar\n");
    *t = lhs->GetToken();
    return false;  
  }
 
/*  if(rhs->GetType()){
    perror("Left hand and right hand side statement type not balanced\n");
    *t = lhs->GetToken();
    return false;
  }*/

  if(!rhs->GetType()->Match(lhs->GetType())){
    perror("Left hand and right hand side statement type not balanced\n");
    *t = lhs->GetToken();
    return false;
  }


  return true;
}

const CType* CAstStatAssign::GetType(void) const
{
  return _lhs->GetType();
}

ostream& CAstStatAssign::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << ":=" << " ";

  const CType *t = GetType();
  if (t != NULL) out << t; else out << "<INVALID>";

  out << endl;

  _lhs->print(out, indent+2);
  _rhs->print(out, indent+2);

  return out;
}

string CAstStatAssign::dotAttr(void) const
{
  return " [label=\":=\",shape=box]";
}

void CAstStatAssign::toDot(ostream &out, int indent) const
{
  string ind(indent, ' ');

  CAstNode::toDot(out, indent);

  _lhs->toDot(out, indent);
  out << ind << dotID() << "->" << _lhs->dotID() << ";" << endl;
  _rhs->toDot(out, indent);
  out << ind << dotID() << "->" << _rhs->dotID() << ";" << endl;
}

CTacAddr* CAstStatAssign::ToTac(CCodeBlock *cb, CTacLabel *next)
{
  CTacAddr *r = GetRHS()->ToTac(cb);
  CTacAddr *l = GetLHS()->ToTac(cb);

  cb->AddInstr(new CTacInstr(opAssign, l, r));
  // add assign instruction to the code block
//    cb->AddInstr(new CTacInstr(opGoto, next));

  return NULL;
}


//------------------------------------------------------------------------------
// CAstStatCall
//
CAstStatCall::CAstStatCall(CToken t, CAstFunctionCall *call)
  : CAstStatement(t), _call(call)
{
  assert(call != NULL);
}

CAstFunctionCall* CAstStatCall::GetCall(void) const
{
  return _call;
}

bool CAstStatCall::TypeCheck(CToken *t, string *msg) const
{
  return GetCall()->TypeCheck(t, msg);
}

ostream& CAstStatCall::print(ostream &out, int indent) const
{
  _call->print(out, indent);

  return out;
}

string CAstStatCall::dotID(void) const
{
  return _call->dotID();
}

string CAstStatCall::dotAttr(void) const
{
  return _call->dotAttr();
}

void CAstStatCall::toDot(ostream &out, int indent) const
{
  _call->toDot(out, indent);
}

CTacAddr* CAstStatCall::ToTac(CCodeBlock *cb, CTacLabel *next)
{
  CTacTemp *t = NULL;  
  CAstFunctionCall *c = GetCall();
  int m = c->GetNArgs();

  // every argument instruction is added
  for(int i = 0; i<m ; i++)
  {
    CTacAddr * arg = c->GetArg(m-1-i)->ToTac(cb);
    CTacInstr *in = new CTacInstr(opParam, new CTacConst(m-i-1), arg, NULL);
    cb->AddInstr(in);  
  }

  // Create a temporary value
  if( c->GetType() != CTypeManager::Get()->GetNull() )
    t = cb->CreateTemp(c->GetType());

  // add the call instruction
  cb->AddInstr(new CTacInstr(opCall, t, new CTacName(c->GetSymbol()), NULL));
  cb->AddInstr(new CTacInstr(opGoto, next, NULL, NULL));
  return NULL;
}


//------------------------------------------------------------------------------
// CAstStatReturn
//
CAstStatReturn::CAstStatReturn(CToken t, CAstScope *scope, CAstExpression *expr)
  : CAstStatement(t), _scope(scope), _expr(expr)
{
  assert(scope != NULL);
}

CAstScope* CAstStatReturn::GetScope(void) const
{
  return _scope;
}

CAstExpression* CAstStatReturn::GetExpression(void) const
{
  return _expr;
}

bool CAstStatReturn::TypeCheck(CToken *t, string *msg) const
{

  if(GetScope()->GetType()->Match((CTypeManager::Get()->GetNull()))){
  // it should not be returning
  	if(GetExpression() != NULL)
	{
	// But it returns something
	  perror("Do not return at procedure\n");
	  *t = GetToken();
	  return false;
	}
  }else
  // it is supposed to return something
    if(GetExpression() == NULL)    
    {
    // but nothing is returned
      perror("Function should be returning something\n");
      *t = GetToken();
      return false;
    }

// check the type
// and check the return type
  if(!GetScope()->GetType()->Match(GetExpression()->GetType()))
  {
    perror("Type mismatch in return statement\n");
    *t = GetExpression()->GetToken();
    return false;
  }

  return true;
}

const CType* CAstStatReturn::GetType(void) const
{
  const CType *t = NULL;

  if (GetExpression() != NULL) {
    t = GetExpression()->GetType();
  } else {
    t = CTypeManager::Get()->GetNull();
  }

  return t;
}

ostream& CAstStatReturn::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << "return" << " ";

  const CType *t = GetType();
  if (t != NULL) out << t; else out << "<INVALID>";

  out << endl;

  if (_expr != NULL) _expr->print(out, indent+2);

  return out;
}

string CAstStatReturn::dotAttr(void) const
{
  return " [label=\"return\",shape=box]";
}

void CAstStatReturn::toDot(ostream &out, int indent) const
{
  string ind(indent, ' ');

  CAstNode::toDot(out, indent);

  if (_expr != NULL) {
    _expr->toDot(out, indent);
    out << ind << dotID() << "->" << _expr->dotID() << ";" << endl;
  }
}

CTacAddr* CAstStatReturn::ToTac(CCodeBlock *cb, CTacLabel *next)
{
  CTacAddr *r = NULL;

  // return the expression tac if it exists
  if(GetExpression())
    r = GetExpression()->ToTac(cb);

  cb->AddInstr(new CTacInstr(opReturn, NULL, r, NULL));
  cb->AddInstr(new CTacInstr(opGoto, next, NULL, NULL));

  return NULL;
}


//------------------------------------------------------------------------------
// CAstStatIf
//
CAstStatIf::CAstStatIf(CToken t, CAstExpression *cond,
                       CAstStatement *ifBody, CAstStatement *elseBody)
  : CAstStatement(t), _cond(cond), _ifBody(ifBody), _elseBody(elseBody)
{
  assert(cond != NULL);
}

CAstExpression* CAstStatIf::GetCondition(void) const
{
  return _cond;
}

CAstStatement* CAstStatIf::GetIfBody(void) const
{
  return _ifBody;
}

CAstStatement* CAstStatIf::GetElseBody(void) const
{
  return _elseBody;
}

bool CAstStatIf::TypeCheck(CToken *t, string *msg) const
{
  CAstStatement* ifBody = GetIfBody();
  CAstExpression *cond = GetCondition();
  CAstStatement* elseBody = GetElseBody();

  if(!cond->TypeCheck(t, msg))
  {
    perror("if condition type error\n");
    *t = cond->GetToken();
    return false;
  }

// cond type must be boolean
  if(!cond->GetType() || !cond->GetType()->Match(CTypeManager::Get()->GetBool()))
  {
  perror("if condition must be a bool\n");
  *t = cond->GetToken();
  return false;
  }


  while(elseBody!=NULL)
  {
   if(!elseBody->TypeCheck(t, msg))
    return false;
    elseBody = elseBody->GetNext();   
  }
  while(ifBody !=NULL)
  {
    if(!ifBody->TypeCheck(t, msg))
    {
      perror("If body type errpr\n");
      *t = ifBody->GetToken();
      return false;
    }
  ifBody = ifBody->GetNext();
  }

  return true;
}

ostream& CAstStatIf::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << "if cond" << endl;
  _cond->print(out, indent+2);
  out << ind << "if-body" << endl;
  if (_ifBody != NULL) {
    CAstStatement *s = _ifBody;
    do {
      s->print(out, indent+2);
      s = s->GetNext();
    } while (s != NULL);
  } else out << ind << "  empty." << endl;
  out << ind << "else-body" << endl;
  if (_elseBody != NULL) {
    CAstStatement *s = _elseBody;
    do {
      s->print(out, indent+2);
      s = s->GetNext();
    } while (s != NULL);
  } else out << ind << "  empty." << endl;

  return out;
}

string CAstStatIf::dotAttr(void) const
{
  return " [label=\"if\",shape=box]";
}

void CAstStatIf::toDot(ostream &out, int indent) const
{
  string ind(indent, ' ');

  CAstNode::toDot(out, indent);

  _cond->toDot(out, indent);
  out << ind << dotID() << "->" << _cond->dotID() << ";" << endl;

  if (_ifBody != NULL) {
    CAstStatement *s = _ifBody;
    if (s != NULL) {
      string prev = dotID();
      do {
        s->toDot(out, indent);
        out << ind << prev << " -> " << s->dotID() << " [style=dotted];"
            << endl;
        prev = s->dotID();
        s = s->GetNext();
      } while (s != NULL);
    }
  }

  if (_elseBody != NULL) {
    CAstStatement *s = _elseBody;
    if (s != NULL) {
      string prev = dotID();
      do {
        s->toDot(out, indent);
        out << ind << prev << " -> " << s->dotID() << " [style=dotted];" 
            << endl;
        prev = s->dotID();
        s = s->GetNext();
      } while (s != NULL);
    }
  }
}

CTacAddr* CAstStatIf::ToTac(CCodeBlock *cb, CTacLabel *next)
{
  CTacLabel *if_t = cb->CreateLabel("if_true");
  CTacLabel *if_f = cb->CreateLabel("if_false");
  CAstStatement *if_b = GetIfBody();
  CAstStatement *else_b = GetElseBody();

  // if true, goto true
  // goto false
  // true:
  //     xxxx
  //     goto done
  // false:
  //     xxxx
  //     
  // done:

  // both instructions are added to the intermediate code
  GetCondition()->ToTac(cb, if_t, if_f);
  cb->AddInstr(if_t);
  while(if_b)
  {
    CTacLabel *n = cb->CreateLabel();
    if_b->ToTac(cb, n);
    // moves on to the next statement
    if_b = if_b->GetNext();
  }
  // goto next label
  cb->AddInstr(new CTacInstr(opGoto, next, NULL, NULL));

  cb->AddInstr((if_f));
  while(else_b)
  {
    CTacLabel *n = cb->CreateLabel();
    else_b->ToTac(cb, n);
    cb->AddInstr(n);  
    else_b = else_b->GetNext();
  }
  cb->AddInstr(new CTacInstr(opGoto, next, NULL, NULL));
  return NULL;
}


//------------------------------------------------------------------------------
// CAstStatWhile
//
CAstStatWhile::CAstStatWhile(CToken t,
                             CAstExpression *cond, CAstStatement *body)
  : CAstStatement(t), _cond(cond), _body(body)
{
  assert(cond != NULL);
}

CAstExpression* CAstStatWhile::GetCondition(void) const
{
  return _cond;
}

CAstStatement* CAstStatWhile::GetBody(void) const
{
  return _body;
}

bool CAstStatWhile::TypeCheck(CToken *t, string *msg) const
{
  CAstStatement* while_body = GetBody();
  CAstExpression* while_cond = GetCondition();

  if(!while_cond->TypeCheck(t, msg))
    return false;
// cond check itself
// check cond is bool
  if(!while_cond->GetType() || !while_cond->GetType()->Match(CTypeManager::Get()->GetBool()))
  {
    perror("condition should be type boolean\n");
    *t = while_cond->GetToken();
    return false;
  }

// check the body
//
  while(while_body != NULL)
  {
    if(!while_body->TypeCheck(t, msg))
    {
      perror("While body type error\n");
      *t = while_body->GetToken();
      return false;
    }
    while_body = while_body->GetNext();
  }
  return true;
}

ostream& CAstStatWhile::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << "while cond" << endl;
  _cond->print(out, indent+2);
  out << ind << "while-body" << endl;
  if (_body != NULL) {
    CAstStatement *s = _body;
    do {
      s->print(out, indent+2);
      s = s->GetNext();
    } while (s != NULL);
  }
  else out << ind << "  empty." << endl;

  return out;
}

string CAstStatWhile::dotAttr(void) const
{
  return " [label=\"while\",shape=box]";
}

void CAstStatWhile::toDot(ostream &out, int indent) const
{
  string ind(indent, ' ');

  CAstNode::toDot(out, indent);

  _cond->toDot(out, indent);
  out << ind << dotID() << "->" << _cond->dotID() << ";" << endl;

  if (_body != NULL) {
    CAstStatement *s = _body;
    if (s != NULL) {
      string prev = dotID();
      do {
        s->toDot(out, indent);
        out << ind << prev << " -> " << s->dotID() << " [style=dotted];"
            << endl;
        prev = s->dotID();
        s = s->GetNext();
      } while (s != NULL);
    }
  }
}

CTacAddr* CAstStatWhile::ToTac(CCodeBlock *cb, CTacLabel *next)
{
  /*
   * while cond:
   *   if true goto body
   *   goto done
   *
   * body:
   *    xxx
   *    goto whilecond
   * done:
   */
  CTacLabel *condition = cb->CreateLabel("_while_cond"); 
  CTacLabel *while_body = cb->CreateLabel("_while_body"); 
  CAstStatement *while_b = GetBody();

  cb->AddInstr(condition);
  GetCondition()->ToTac(cb, while_body, next);

  cb->AddInstr(while_body);
  while(while_b)
  {
    CTacLabel *n = cb->CreateLabel();
    while_b->ToTac(cb, n);
    cb->AddInstr(n);
    while_b = while_b->GetNext();
  }
  
  cb->AddInstr(new CTacInstr(opGoto, condition, NULL, NULL));
  cb->AddInstr(new CTacInstr(opGoto, next, NULL, NULL));

  return NULL;
}


//------------------------------------------------------------------------------
// CAstExpression
//
CAstExpression::CAstExpression(CToken t)
  : CAstNode(t)
{
}

void CAstExpression::SetParenthesized(bool parenthesized)
{
  _parenthesized = parenthesized;
}
/*
bool CAstExpression::TypeCheck(t, msg)
{
  
  return true;
}*/

bool CAstExpression::GetParenthesized(void) const
{
  return _parenthesized;
}

CTacAddr* CAstExpression::ToTac(CCodeBlock *cb)
{
  return NULL;
}

CTacAddr* CAstExpression::ToTac(CCodeBlock *cb,
                                CTacLabel *ltrue, CTacLabel *lfalse)
{
  return NULL;
}


//------------------------------------------------------------------------------
// CAstOperation
//
CAstOperation::CAstOperation(CToken t, EOperation oper)
  : CAstExpression(t), _oper(oper)
{
}

EOperation CAstOperation::GetOperation(void) const
{
  return _oper;
}


//------------------------------------------------------------------------------
// CAstBinaryOp
//
CAstBinaryOp::CAstBinaryOp(CToken t, EOperation oper,
                           CAstExpression *l,CAstExpression *r)
  : CAstOperation(t, oper), _left(l), _right(r)
{
  // these are the only binary operation we support for now
  assert((oper == opAdd)        || (oper == opSub)         ||
         (oper == opMul)        || (oper == opDiv)         ||
         (oper == opAnd)        || (oper == opOr)          ||
         (oper == opEqual)      || (oper == opNotEqual)    ||
         (oper == opLessThan)   || (oper == opLessEqual)   ||
         (oper == opBiggerThan) || (oper == opBiggerEqual)
        );
  assert(l != NULL);
  assert(r != NULL);
}

CAstExpression* CAstBinaryOp::GetLeft(void) const
{
  return _left;
}

CAstExpression* CAstBinaryOp::GetRight(void) const
{
  return _right;
}

bool CAstBinaryOp::TypeCheck(CToken *t, string *msg) const
{
  CAstExpression* lhs = GetLeft();
  CAstExpression* rhs = GetRight();

// check separately
  if(!lhs->TypeCheck(t, msg) || !rhs->TypeCheck(t, msg))
  {
    perror("Type error with BINARY OPeration left or right hand side\n");
    *t = lhs->GetToken();
    return false;
  }
// they should be scalar
  if(lhs->GetType() == NULL || !lhs->GetType()->IsScalar() || rhs->GetType() == NULL || !rhs->GetType()->IsScalar())
  {
    perror("Binary op should have scalar on both sides\n");
    *t = lhs->GetToken();
    return false;
  }
  if(!lhs->GetType()->Match(rhs->GetType()))
  {
    *t = GetToken();
    perror("Binary op dont match\n");
    return false;
  }
  return true;
}

const CType* CAstBinaryOp::GetType(void) const
{
  CToken temp = GetToken();
  switch(temp.GetType()){
    case tAnd:
    case tOr:
    case tRelOp:
      return CTypeManager::Get()->GetBool();
    default:
       return CTypeManager::Get()->GetInt();
  }
}

ostream& CAstBinaryOp::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << GetOperation() << " ";

  const CType *t = GetType();
  if (t != NULL) out << t; else out << "<INVALID>";

  out << endl;

  _left->print(out, indent+2);
  _right->print(out, indent+2);

  return out;
}

string CAstBinaryOp::dotAttr(void) const
{
  ostringstream out;
  out << " [label=\"" << GetOperation() << "\",shape=box]";
  return out.str();
}

void CAstBinaryOp::toDot(ostream &out, int indent) const
{
  string ind(indent, ' ');

  CAstNode::toDot(out, indent);

  _left->toDot(out, indent);
  out << ind << dotID() << "->" << _left->dotID() << ";" << endl;
  _right->toDot(out, indent);
  out << ind << dotID() << "->" << _right->dotID() << ";" << endl;
}

CTacAddr* CAstBinaryOp::ToTac(CCodeBlock *cb)
{
  EOperation op = GetOperation();
  
  if( op <=opDiv || op>= opAdd)
  {
    CAstExpression *l = GetLeft();
    CAstExpression *r = GetRight();
    CTacAddr *ll = l->ToTac(cb);
    CTacAddr *rr = r->ToTac(cb);
    CTacTemp *temp = cb->CreateTemp(CTypeManager::Get()->GetInt());
    cb->AddInstr(new CTacInstr(op, temp, ll, rr));
    // check: do we need to return temp here?
    return temp;
  }

  CTacLabel *t = cb->CreateLabel("true");
  CTacLabel *f = cb->CreateLabel("false");
  CTacLabel *end = cb->CreateLabel();
  ToTac(cb, t, f);

  CTacTemp *temp = cb->CreateTemp(CTypeManager::Get()->GetBool());
  cb->AddInstr(t);
  cb->AddInstr(new CTacInstr(opAssign, temp, new CTacConst(1), NULL));
  cb->AddInstr(new CTacInstr(opGoto, end, NULL, NULL));

  cb->AddInstr(f);
  cb->AddInstr(new CTacInstr(opAssign, temp, new CTacConst(0), NULL));
  // check we need new instr or just an end
  cb->AddInstr(end);
  return NULL;
}

CTacAddr* CAstBinaryOp::ToTac(CCodeBlock *cb,
                              CTacLabel *ltrue, CTacLabel *lfalse)
{
  EOperation op = GetOperation();
  CAstExpression *l = GetLeft();
  CAstExpression *r = GetRight();
  CTacLabel *n = cb->CreateLabel();

  // need to check the logic here
  switch(op)
  {
    case opEqual:
    case opNotEqual:
    case opLessThan:
    case opLessEqual:
    case opBiggerThan:
    case opBiggerEqual:
	cb->AddInstr(new CTacInstr(op, ltrue, l->ToTac(cb), r->ToTac(cb)));
  	cb->AddInstr(new CTacInstr(opGoto, lfalse));
	break;

    case opAnd:
      l->ToTac(cb, n, lfalse);
      cb->AddInstr(n);
      break;
    default:
      l->ToTac(cb, ltrue, n);
      cb->AddInstr(n);
      r->ToTac(cb, ltrue, lfalse);
  }

  return NULL;
}


//------------------------------------------------------------------------------
// CAstUnaryOp
//
CAstUnaryOp::CAstUnaryOp(CToken t, EOperation oper, CAstExpression *e)
  : CAstOperation(t, oper), _operand(e)
{
  assert((oper == opNeg) || (oper == opPos) || (oper == opNot));
  assert(e != NULL);
}

CAstExpression* CAstUnaryOp::GetOperand(void) const
{
  return _operand;
}

bool CAstUnaryOp::TypeCheck(CToken *t, string *msg) const
{
  if(!GetOperand())
  {
    perror("Operand is NULL\n");
    *t = GetToken();
    return false;
  }
  if( !GetOperand()->TypeCheck(t, msg))
  {
    perror("Operand type check failed\n");
    *t = GetToken();
    return false;
  }
  
  return true;
}

const CType* CAstUnaryOp::GetType(void) const
{
  if(GetToken().GetType() == tNot)
	  return CTypeManager::Get()->GetBool();
  else return CTypeManager::Get()->GetInt();
}

ostream& CAstUnaryOp::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << GetOperation() << " ";

  const CType *t = GetType();
  if (t != NULL) out << t; else out << "<INVALID>";
  out << endl;

  _operand->print(out, indent+2);

  return out;
}

string CAstUnaryOp::dotAttr(void) const
{
  ostringstream out;
  out << " [label=\"" << GetOperation() << "\",shape=box]";
  return out.str();
}

void CAstUnaryOp::toDot(ostream &out, int indent) const
{
  string ind(indent, ' ');

  CAstNode::toDot(out, indent);

  _operand->toDot(out, indent);
  out << ind << dotID() << "->" << _operand->dotID() << ";" << endl;
}

CTacAddr* CAstUnaryOp::ToTac(CCodeBlock *cb)
{
  EOperation op = GetOperation();
  
  CTacTemp *re = NULL;

  switch(op)
  {
    case opPos:
    case opNeg:
    {
    // this is an integer
      CAstConstant *num = dynamic_cast<CAstConstant *>(GetOperand());
      if(num== NULL)
      {
	CTacAddr *Tac_op = GetOperand()->ToTac(cb);
	re = cb->CreateTemp(CTypeManager::Get()->GetInt());
	cb->AddInstr(new CTacInstr(op, re, Tac_op));
      }
      else
      {
        long long value = num->GetValue();
	if(op == opNeg)
	  value = -value;
	CTacConst *Tac_num = new CTacConst(value);
	return Tac_num;
      }
      break;
    }
    default:
    // create true label and false label
      CTacLabel *t = cb->CreateLabel();
      CTacLabel *f = cb->CreateLabel();
      CTacLabel *end = cb->CreateLabel();
      ToTac(cb, t, f);

    // return value is a bool
      re = cb->CreateTemp(CTypeManager::Get()->GetBool());

      cb->AddInstr(t);
      cb->AddInstr(new CTacInstr(opAssign, re, new CTacConst(0), NULL));
      cb->AddInstr(end);
  }  
  // check
  return re;
}

CTacAddr* CAstUnaryOp::ToTac(CCodeBlock *cb,
                             CTacLabel *ltrue, CTacLabel *lfalse)		
{
  // check
  EOperation op = GetOperation();
  CAstExpression *expr_operand = GetOperand();
  if(op == opNot)
  {
    perror("opNot in unary operation\n");
    exit(0);
  }
  expr_operand->ToTac(cb, lfalse, ltrue);
  return NULL;
}


//------------------------------------------------------------------------------
// CAstSpecialOp
//
CAstSpecialOp::CAstSpecialOp(CToken t, EOperation oper, CAstExpression *e,
                             const CType *type)
  : CAstOperation(t, oper), _operand(e), _type(type)
{
  assert((oper == opAddress) || (oper == opDeref) || (oper = opCast));
  assert(e != NULL);
  assert(((oper != opCast) && (type == NULL)) ||
         ((oper == opCast) && (type != NULL)));
}

CAstExpression* CAstSpecialOp::GetOperand(void) const
{
  return _operand;
}

bool CAstSpecialOp::TypeCheck(CToken *t, string *msg) const
{
  return false;
}

const CType* CAstSpecialOp::GetType(void) const
{
  return NULL;
}

ostream& CAstSpecialOp::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << GetOperation() << " ";

  const CType *t = GetType();
  if (t != NULL) out << t; else out << "<INVALID>";
  out << endl;

  _operand->print(out, indent+2);

  return out;
}

string CAstSpecialOp::dotAttr(void) const
{
  ostringstream out;
  out << " [label=\"" << GetOperation() << "\",shape=box]";
  return out.str();
}

void CAstSpecialOp::toDot(ostream &out, int indent) const
{
  string ind(indent, ' ');

  CAstNode::toDot(out, indent);

  _operand->toDot(out, indent);
  out << ind << dotID() << "->" << _operand->dotID() << ";" << endl;
}

CTacAddr* CAstSpecialOp::ToTac(CCodeBlock *cb)
{
  CAstExpression *op = GetOperand();

  CTacAddr *ptr = op->ToTac(cb);
  CTacTemp *temp = cb->CreateTemp(CTypeManager::Get()->GetPointer(op->GetType()));

  cb->AddInstr(new CTacInstr(opAddress,temp, ptr, NULL));
  return temp;
}


//------------------------------------------------------------------------------
// CAstFunctionCall
//
CAstFunctionCall::CAstFunctionCall(CToken t, const CSymProc *symbol)
  : CAstExpression(t), _symbol(symbol)
{
  assert(symbol != NULL);
}

const CSymProc* CAstFunctionCall::GetSymbol(void) const
{
  return _symbol;
}

void CAstFunctionCall::AddArg(CAstExpression *arg)
{
  _arg.push_back(arg);
}

int CAstFunctionCall::GetNArgs(void) const
{
  return (int)_arg.size();
}

CAstExpression* CAstFunctionCall::GetArg(int index) const
{
  assert((index >= 0) && (index < _arg.size()));
  return _arg[index];
}

bool CAstFunctionCall::TypeCheck(CToken *t, string *msg) const
{
  // number of arguments is checked in the parser  
  return true;
}

const CType* CAstFunctionCall::GetType(void) const
{
  return GetSymbol()->GetDataType();
}

ostream& CAstFunctionCall::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << "call " << _symbol << " ";
  const CType *t = GetType();
  if (t != NULL) out << t; else out << "<INVALID>";
  out << endl;

  for (size_t i=0; i<_arg.size(); i++) {
    _arg[i]->print(out, indent+2);
  }

  return out;
}

string CAstFunctionCall::dotAttr(void) const
{
  ostringstream out;
  out << " [label=\"call " << _symbol->GetName() << "\",shape=box]";
  return out.str();
}

void CAstFunctionCall::toDot(ostream &out, int indent) const
{
  string ind(indent, ' ');

  CAstNode::toDot(out, indent);

  for (size_t i=0; i<_arg.size(); i++) {
    _arg[i]->toDot(out, indent);
    out << ind << dotID() << "->" << _arg[i]->dotID() << ";" << endl;
  }
}

CTacAddr* CAstFunctionCall::ToTac(CCodeBlock *cb)
{

  for(int i = 0; i<GetNArgs(); i++ )
  {
    CTacAddr *argument = GetArg(GetNArgs()-1-i)->ToTac(cb);
    CTacInstr *argu_tac = new CTacInstr(opParam, new CTacConst(1), argument, NULL);
    cb->AddInstr(argu_tac);  
  }
// check
  CTacTemp *re = cb->CreateTemp(GetType());
  cb->AddInstr(new CTacInstr(opCall, re, new CTacName(GetSymbol()), NULL));
  return re;
}

CTacAddr* CAstFunctionCall::ToTac(CCodeBlock *cb,
                                  CTacLabel *ltrue, CTacLabel *lfalse)
{
  CTacAddr *re = ToTac(cb);

  cb->AddInstr(new CTacInstr(opEqual, ltrue, re, new CTacConst(1)));
  cb->AddInstr(new CTacInstr(opGoto, lfalse, NULL, NULL));
  return NULL;
}



//------------------------------------------------------------------------------
// CAstOperand
//
CAstOperand::CAstOperand(CToken t)
  : CAstExpression(t)
{
}


//------------------------------------------------------------------------------
// CAstDesignator
//
CAstDesignator::CAstDesignator(CToken t, const CSymbol *symbol)
  : CAstOperand(t), _symbol(symbol)
{
  assert(symbol != NULL);
}

const CSymbol* CAstDesignator::GetSymbol(void) const
{
  return _symbol;
}

bool CAstDesignator::TypeCheck(CToken *t, string *msg) const
{
  return true;
}

const CType* CAstDesignator::GetType(void) const
{
  return GetSymbol()->GetDataType();
}

ostream& CAstDesignator::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << _symbol << " ";

  const CType *t = GetType();
  if (t != NULL) out << t; else out << "<INVALID>";

  out << endl;

  return out;
}

string CAstDesignator::dotAttr(void) const
{
  ostringstream out;
  out << " [label=\"" << _symbol->GetName();
  out << "\",shape=ellipse]";
  return out.str();
}

void CAstDesignator::toDot(ostream &out, int indent) const
{
  string ind(indent, ' ');

  CAstNode::toDot(out, indent);
}

CTacAddr* CAstDesignator::ToTac(CCodeBlock *cb)
{
  return new CTacName(GetSymbol());
}

CTacAddr* CAstDesignator::ToTac(CCodeBlock *cb,
                                CTacLabel *ltrue, CTacLabel *lfalse)
{
  // check
  CTacAddr *re = ToTac(cb);

  cb->AddInstr(new CTacInstr(opEqual, ltrue, re, new CTacConst(1)));
  cb->AddInstr(new CTacInstr(opGoto, lfalse, NULL, NULL));
  return NULL;
}


//------------------------------------------------------------------------------
// CAstArrayDesignator
//
CAstArrayDesignator::CAstArrayDesignator(CToken t, const CSymbol *symbol)
  : CAstDesignator(t, symbol), _done(false), _offset(NULL)
{
}

void CAstArrayDesignator::AddIndex(CAstExpression *idx)
{
  assert(!_done);
  _idx.push_back(idx);
}

void CAstArrayDesignator::IndicesComplete(void)
{
  assert(!_done);
  _done = true;
}

int CAstArrayDesignator::GetNIndices(void) const
{
  return (int)_idx.size();
}

CAstExpression* CAstArrayDesignator::GetIndex(int index) const
{
  assert((index >= 0) && (index < _idx.size()));
  return _idx[index];
}

bool CAstArrayDesignator::TypeCheck(CToken *t, string *msg) const
{
  bool result = true;

  assert(_done);

  return result;
}

const CType* CAstArrayDesignator::GetType(void) const
{
  return ((const CArrayType*)GetSymbol()->GetDataType())->GetBaseType();
}

ostream& CAstArrayDesignator::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << _symbol << " ";

  const CType *t = GetType();
  if (t != NULL) out << t; else out << "<INVALID>";

  out << endl;

  for (size_t i=0; i<_idx.size(); i++) {
    _idx[i]->print(out, indent+2);
  }

  return out;
}

string CAstArrayDesignator::dotAttr(void) const
{
  ostringstream out;
  out << " [label=\"" << _symbol->GetName() << "[]\",shape=ellipse]";
  return out.str();
}

void CAstArrayDesignator::toDot(ostream &out, int indent) const
{
  string ind(indent, ' ');

  CAstNode::toDot(out, indent);

  for (size_t i=0; i<_idx.size(); i++) {
    _idx[i]->toDot(out, indent);
    out << ind << dotID() << "-> " << _idx[i]->dotID() << ";" << endl;
  }
}

CTacAddr* CAstArrayDesignator::ToTac(CCodeBlock *cb)
{
  CSymtab *st = cb->GetOwner()->GetSymbolTable();

  const CSymbol *dim = st->FindSymbol("DIM");
  CAstConstant *constant = new CAstConstant(GetToken(), CTypeManager::Get()->GetInt(), 0);

  const CSymbol *dofs = st->FindSymbol("DOFS");

  // array identifier
  CTacAddr *ident = new CTacName(GetSymbol());
  CAstExpression *expr_ident = new CAstDesignator(GetToken(), GetSymbol());
  const CArrayType *type;

  if(GetSymbol()->GetDataType()->IsPointer())
  {
    type = dynamic_cast<const CArrayType*>((dynamic_cast<const CPointerType*>(GetSymbol()->GetDataType()))->GetBaseType());    
  }
  else{
    CTacTemp *ptr = cb->CreateTemp(CTypeManager::Get()->GetPointer(GetSymbol()->GetDataType()));
    cb->AddInstr(new CTacInstr(opAddress, ptr,ident, NULL));
    ident = ptr;
    
    expr_ident = new CAstSpecialOp(GetToken(), opAddress, expr_ident, NULL);
    type = dynamic_cast<const CArrayType*>(GetSymbol()->GetDataType());
  }

  int size = type->GetBaseType()->GetSize();
  CTacAddr *dim_ident = NULL;
  int i = 0;
  for(i=0; i < type->GetNDim() - 1; i++)
  {
    if(!dim_ident) 
      dim_ident = GetIndex(i)->ToTac(cb); // how many rows we go over
    else
    {
      CTacAddr * prev = new CTacConst(0);

      if(i < GetNIndices())
      {
        prev = GetIndex(i)->ToTac(cb);
      }
      CTacAddr *next = cb->CreateTemp(CTypeManager::Get()->GetInt());
      cb->AddInstr(new CTacInstr(opAdd, next, dim_ident, prev));
      dim_ident = next;
    }
    
    // dimension size
    CAstFunctionCall *dim_function = new CAstFunctionCall(GetToken(), dynamic_cast<const CSymProc*>(dim));

    dim_function->AddArg(expr_ident);
    constant->SetValue(i+2);
    dim_function->AddArg(constant);
    CTacAddr *entry = dim_function->ToTac(cb);

    CTacAddr *next = cb->CreateTemp(CTypeManager::Get()->GetInt());
    cb->AddInstr(new CTacInstr(opMul, next, dim_ident, entry));
    dim_ident = next;
  }
    if(!dim_ident)
      dim_ident = GetIndex(i)->ToTac(cb);
    else
    {
      CTacAddr * prev = new CTacConst(0);

      if(i < GetNIndices())
      {
        prev = GetIndex(i)->ToTac(cb);
      }
      CTacAddr *next = cb->CreateTemp(CTypeManager::Get()->GetInt());
      cb->AddInstr(new CTacInstr(opAdd, next, dim_ident, prev));
      dim_ident = next;
    }

// data size
  CTacTemp *temp = cb->CreateTemp(CTypeManager::Get()->GetInt());
  cb->AddInstr(new CTacInstr(opMul, temp, dim_ident, new CTacConst(size)));
  dim_ident = temp;
  
  // calculate array offset
  CAstFunctionCall *dofs_function = new CAstFunctionCall(GetToken(), dynamic_cast<const CSymProc*>(dofs));
  dofs_function->AddArg(expr_ident);
  CTacAddr *offset = dofs_function->ToTac(cb);

  temp = cb->CreateTemp(CTypeManager::Get()->GetInt());
  cb->AddInstr(new CTacInstr(opAdd, temp, dim_ident, offset));

  dim_ident = temp;
  temp = cb->CreateTemp(CTypeManager::Get()->GetInt());
  cb->AddInstr(new CTacInstr(opAdd, temp, ident, dim_ident));
  // check deref
  return new CTacReference(temp->GetSymbol(), temp->GetSymbol());
}

CTacAddr* CAstArrayDesignator::ToTac(CCodeBlock *cb,
                                     CTacLabel *ltrue, CTacLabel *lfalse)
{
  CTacAddr *re = ToTac(cb);
  cb->AddInstr(new CTacInstr(opEqual, ltrue, re, new CTacConst(1)));
  cb->AddInstr(new CTacInstr(opGoto, lfalse, NULL, NULL));
  return re;
}


//------------------------------------------------------------------------------
// CAstConstant
//
CAstConstant::CAstConstant(CToken t, const CType *type, long long value)
  : CAstOperand(t), _type(type), _value(value)
{
}

void CAstConstant::SetValue(long long value)
{
  _value = value;
}

long long CAstConstant::GetValue(void) const
{
  return _value;
}

string CAstConstant::GetValueStr(void) const
{
  ostringstream out;

  if (GetType() == CTypeManager::Get()->GetBool()) {
    out << (_value == 0 ? "false" : "true");
  } else {
    out << dec << _value;
  }

  return out.str();
}

bool CAstConstant::TypeCheck(CToken *t, string *msg) const
{
  return true;
}

const CType* CAstConstant::GetType(void) const
{
  return _type;
}

ostream& CAstConstant::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << GetValueStr() << " ";

  const CType *t = GetType();
  if (t != NULL) out << t; else out << "<INVALID>";

  out << endl;

  return out;
}

string CAstConstant::dotAttr(void) const
{
  ostringstream out;
  out << " [label=\"" << GetValueStr() << "\",shape=ellipse]";
  return out.str();
}

CTacAddr* CAstConstant::ToTac(CCodeBlock *cb)
{
  return new CTacConst(GetValue());
}

CTacAddr* CAstConstant::ToTac(CCodeBlock *cb,
                                CTacLabel *ltrue, CTacLabel *lfalse)
{
 // check why long long
  long long condition = GetValue();
  if(condition)
    cb->AddInstr(new CTacInstr(opGoto, ltrue, NULL, NULL));
  else
    cb->AddInstr(new CTacInstr(opGoto, lfalse, NULL, NULL));

  return new CTacConst(GetValue());
}


//------------------------------------------------------------------------------
// CAstStringConstant
//
int CAstStringConstant::_idx = 0;

CAstStringConstant::CAstStringConstant(CToken t, const string value,
                                       CAstScope *s)
  : CAstOperand(t)
{
  CTypeManager *tm = CTypeManager::Get();
  CSymtab *st = s->GetSymbolTable();

  _type = tm->GetArray(strlen(CToken::unescape(value).c_str())+1,
                       tm->GetChar());
  _value = new CDataInitString(value);

  // in case of name clashes we simply iterate until we find a
  // name that has not yet been used
  _sym = NULL;
  do {
    ostringstream o;
    o << "_str_" << ++_idx;
    if (st->FindSymbol(o.str(), sGlobal) == NULL) {
      _sym = new CSymGlobal(o.str(), _type);
    }
  } while (_sym == NULL);

  _sym->SetData(_value);
  st->AddSymbol(_sym);
}

const string CAstStringConstant::GetValue(void) const
{
  return _value->GetData();
}

const string CAstStringConstant::GetValueStr(void) const
{
  return GetValue();
}

bool CAstStringConstant::TypeCheck(CToken *t, string *msg) const
{
  return true;
}

const CType* CAstStringConstant::GetType(void) const
{
  return NULL;
}

ostream& CAstStringConstant::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << '"' << GetValueStr() << '"' << " ";

  const CType *t = GetType();
  if (t != NULL) out << t; else out << "<INVALID>";

  out << endl;

  return out;
}

string CAstStringConstant::dotAttr(void) const
{
  ostringstream out;
  // the string is already escaped, but dot requires double escaping
  out << " [label=\"\\\"" << CToken::escape(GetValueStr())
      << "\\\"\",shape=ellipse]";
  return out.str();
}

CTacAddr* CAstStringConstant::ToTac(CCodeBlock *cb)
{
  return new CTacName(_sym);
}

CTacAddr* CAstStringConstant::ToTac(CCodeBlock *cb,
                                CTacLabel *ltrue, CTacLabel *lfalse)
{
  return new CTacName(_sym);
}



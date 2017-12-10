//------------------------------------------------------------------------------
/// @brief SNUPL type system
/// @author Bernhard Egger <bernhard@csap.snu.ac.kr>
/// @section changelog Change Log
/// 2012/09/14 Bernhard Egger created
/// 2016/03/12 Bernhard Egger adapted to SnuPL/1
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

#include <cassert>

#include "type.h"
using namespace std;


//------------------------------------------------------------------------------
// CType
//
CType::CType(void)
{
}

CType::~CType(void)
{
}

unsigned int CType::GetDataSize(void) const
{
  return GetSize();
}

bool CType::Compare(const CType *t) const
{
  return Match(t);
}

ostream& operator<<(ostream &out, const CType &t)
{
  return t.print(out);
}

ostream& operator<<(ostream &out, const CType *t)
{
  return t->print(out);
}


//------------------------------------------------------------------------------
// CScalarType
//
bool CScalarType::Match(const CType *t) const
{
  // types are singletons and not implicit type conversions are allowed, 
  // i.e., for scalar types we can simply compare the pointers
  return (t == this);
}


//------------------------------------------------------------------------------
// CNullType
//
CNullType::CNullType(void)
  : CScalarType()
{
}

ostream& CNullType::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << "<NULL>";
  return out;
}

//------------------------------------------------------------------------------
// CIntType
//
CIntType::CIntType(void)
  : CScalarType()
{
}

ostream& CIntType::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << "<" << "int";
  //out << "," << GetSize() << "," << GetAlign();
  out << ">";
  return out;
}


//------------------------------------------------------------------------------
// CCharType
//
CCharType::CCharType(void)
  : CScalarType()
{
}

ostream& CCharType::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << "<" << "char";
  //out << "," << GetSize() << "," << GetAlign();
  out << ">";
  return out;
}


//------------------------------------------------------------------------------
// CBoolType
//
CBoolType::CBoolType(void)
  : CScalarType()
{
}

ostream& CBoolType::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << "<" << "bool";
  //out << "," << GetSize() << "," << GetAlign();
  out << ">";
  return out;
}


//------------------------------------------------------------------------------
// CPointerType
//
CPointerType::CPointerType(const CType *basetype)
  : CScalarType(), _basetype(basetype)
{
}

bool CPointerType::Match(const CType *t) const
{
  // check whether t is a pointer
  if ((t == NULL) || !t->IsPointer()) return false;

  const CPointerType *pt = dynamic_cast<const CPointerType*>(t);
  assert(pt != NULL);

  // match if:
  // - this is a void pointer or
  // - the types are compatible with respect to Match()
  // TODO
  return false;
}

bool CPointerType::Compare(const CType *t) const
{
  // check whether t is a pointer
  if ((t == NULL) || !t->IsPointer()) return false;

  const CPointerType *pt = dynamic_cast<const CPointerType*>(t);
  assert(pt != NULL);

  // comparison: match if
  // - this is a void pointer or
  // - the types are compatible with respect to Compare()
  // TODO
  return false;
}

ostream& CPointerType::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << "<" << "ptr" << "(" << GetSize() << ") to ";
  if (_basetype != NULL) out << _basetype; else out << "void";
  out << ">";
  return out;
}


//------------------------------------------------------------------------------
// CArrayType
//
CArrayType::CArrayType(int nelem, const CType *innertype)
  : CType(), _nelem(nelem), _innertype(innertype)
{
  assert((_nelem > 0) || (_nelem == OPEN));
  assert(_innertype != NULL);
}

CArrayType::~CArrayType(void)
{
  // do not free _innertype; all types (also inner types) are
  // owned by the type manager
}

unsigned int CArrayType::GetSize(void) const
{
  return 4 + 4*GetNDim() + GetDataSize();
}

unsigned int CArrayType::GetDataSize(void) const
{
  int n = GetNElem();
  return n == OPEN ? 0 : n*GetInnerType()->GetDataSize();
}

int CArrayType::GetAlign(void) const
{
  // arrays must be 4-aligned since we have integer meta-data at the beginning
  return 4;
}

const CType* CArrayType::GetBaseType(void) const
{
  const CType *t = GetInnerType();
  assert(t != NULL);

  if (t->IsArray()) return dynamic_cast<const CArrayType*>(t)->GetBaseType();
  else return t;
}

int CArrayType::GetNDim(void) const
{
  const CType *t = GetInnerType();
  assert(t != NULL);

  if (t->IsArray()) return dynamic_cast<const CArrayType*>(t)->GetNDim() + 1;
  else return 1;
}

bool CArrayType::Match(const CType *t) const
{
  // check whether t is an array
  if ((t == NULL) || !t->IsArray()) return false;

  const CArrayType *at = dynamic_cast<const CArrayType*>(t);
  assert(at != NULL);

  // match if:
  // - (this is an open array or the number of elements match) and
  // - the inner types are compatible with respect to Match()
  // TODO
  return false;
}

bool CArrayType::Compare(const CType *t) const
{
  // check whether t is an array
  if ((t == NULL) || !t->IsArray()) return false;

  const CArrayType *at = dynamic_cast<const CArrayType*>(t);
  assert(at != NULL);

  // comparison: match if
  // - the number of elements match and
  // - the inner types are compatible with respect to Compare()
  // TODO
  return false;
}

ostream& CArrayType::print(ostream &out, int indent) const
{
  string ind(indent, ' ');
  int n = GetNElem();

  out << ind << "<array ";
  if (n != OPEN) out << n << " ";
  out << "of "; GetInnerType()->print(out);
  //out << "," << GetSize() << "," << GetAlign();
  out << ">";
  return out;
}


//------------------------------------------------------------------------------
// CTypeManager
//
CTypeManager* CTypeManager::_global_tm = NULL;

CTypeManager::CTypeManager(void)
{
  _null = new CNullType();
  _integer = new CIntType();
  _char = new CCharType();
  _boolean = new CBoolType();
  _voidptr = new CPointerType(_null);
  _ptr.push_back(_voidptr);
}

CTypeManager::~CTypeManager(void)
{
  delete _null;
  delete _integer;
  delete _char;
  delete _boolean;
  // _voidptr is a member of _pointer and will get deleted just below
  for (size_t i=0; i<_ptr.size(); i++) delete _ptr[i];
  for (size_t i=0; i<_array.size(); i++) delete _array[i];
}

CTypeManager* CTypeManager::Get(void)
{
  if (_global_tm == NULL) _global_tm = new CTypeManager();

  return _global_tm;
}

const CNullType* CTypeManager::GetNull(void) const
{
  return _null;
}

const CIntType* CTypeManager::GetInt(void) const
{
  return _integer;
}

const CCharType* CTypeManager::GetChar(void) const
{
  return _char;
}

const CBoolType* CTypeManager::GetBool(void) const
{
  return _boolean;
}

const CPointerType* CTypeManager::GetVoidPtr(void) const
{
  return _voidptr;
}

const CPointerType* CTypeManager::GetPointer(const CType *basetype)
{
  for (size_t i=0; i<_ptr.size(); i++) {
    if ((_ptr[i]->GetBaseType()->Compare(basetype))) {
      return _ptr[i];
    }
  }

  CPointerType *p = new CPointerType(basetype);
  _ptr.push_back(p);

  return p;
}

const CArrayType* CTypeManager::GetArray(int nelem, const CType *innertype)
{
  if (innertype == NULL) return NULL;

  for (size_t i=0; i<_array.size(); i++) {
    if ((_array[i]->GetNElem() == nelem) &&
        (_array[i]->GetInnerType()->Compare(innertype))) {
      return _array[i];
    }
  }

  unsigned long long size = innertype->GetDataSize();
  size = size * nelem + 8;
  if (size > CArrayType::MAX_SIZE) return NULL;

  CArrayType *a = new CArrayType(nelem, innertype);
  _array.push_back(a);

  return a;
}

ostream& CTypeManager::print(ostream &out, int indent) const
{
  string ind(indent, ' ');

  out << ind << "[[ type manager" << endl
      << ind << "  base types:" << endl
      << ind << "    " << _null << endl
      << ind << "    " << _integer << endl
      << ind << "    " << _char << endl
      << ind << "    " << _boolean << endl
      << ind << "    " << _voidptr << endl
      << ind << "  pointer types:" << endl;
  for (size_t i=0; i<_ptr.size(); i++) {
    out << ind << "    " << _ptr[i] << endl;
  }
  out << ind << "  array types:" << endl;
  for (size_t i=0; i<_array.size(); i++) {
    out << ind << "    " << _array[i] << endl;
  }
  out << ind << "]]" << endl;

  return out;
}

//------------------------------------------------------------------------------
/// @brief SnuPL backend
/// @author Bernhard Egger <bernhard@csap.snu.ac.kr>
/// @section changelog Change Log
/// 2012/11/28 Bernhard Egger created
/// 2013/06/09 Bernhard Egger adapted to SnuPL/0
/// 2016/04/04 Bernhard Egger adapted to SnuPL/1
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

#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>

#include "backend.h"
using namespace std;


//------------------------------------------------------------------------------
// CBackend
//
CBackend::CBackend(ostream &out)
  : _out(out)
{
}

CBackend::~CBackend(void)
{
}

bool CBackend::Emit(CModule *m)
{
  assert(m != NULL);
  _m = m;

  if (!_out.good()) return false;

  bool res = true;

  try {
    EmitHeader();
    EmitCode();
    EmitData();
    EmitFooter();

    res = _out.good();
  } catch (...) {
    res = false;
  }

  return res;
}

void CBackend::EmitHeader(void)
{
}

void CBackend::EmitCode(void)
{
}

void CBackend::EmitData(void)
{
}

void CBackend::EmitFooter(void)
{
}


//------------------------------------------------------------------------------
// CBackendx86
//
CBackendx86::CBackendx86(ostream &out)
  : CBackend(out), _curr_scope(NULL)
{
  _ind = string(4, ' ');
}

CBackendx86::~CBackendx86(void)
{
}

void CBackendx86::EmitHeader(void)
{
  _out << "##################################################" << endl
       << "# " << _m->GetName() << endl
       << "#" << endl
       << endl;
}

void CBackendx86::EmitCode(void)
{
  _out << _ind << "#-----------------------------------------" << endl
       << _ind << "# text section" << endl
       << _ind << "#" << endl
       << _ind << ".text" << endl
       << _ind << ".align 4" << endl
       << endl
       << _ind << "# entry point and pre-defined functions" << endl
       << _ind << ".global main" << endl
       << _ind << ".extern DIM" << endl
       << _ind << ".extern DOFS" << endl
       << _ind << ".extern ReadInt" << endl
       << _ind << ".extern WriteInt" << endl
       << _ind << ".extern WriteStr" << endl
       << _ind << ".extern WriteChar" << endl
       << _ind << ".extern WriteLn" << endl
       << endl;

  // TODO: done 
  // forall s in subscopes do
  //   EmitScope(s)
  // EmitScope(program)
//  CScope * s = GetScope();  
  const vector<CScope*> & sub = _m->GetSubscopes();
  for(int i=0; i < sub.size(); i++)
  {
    EmitScope(sub.at(i));    
  }
  SetScope(_m);
  EmitScope(_m);

  _out << _ind << "# end of text section" << endl
       << _ind << "#-----------------------------------------" << endl
       << endl;
}

void CBackendx86::EmitData(void)
{
  _out << _ind << "#-----------------------------------------" << endl
       << _ind << "# global data section" << endl
       << _ind << "#" << endl
       << _ind << ".data" << endl
       << _ind << ".align 4" << endl
       << endl;

  EmitGlobalData(_m);

  _out << _ind << "# end of global data section" << endl
       << _ind << "#-----------------------------------------" << endl
       << endl;
}

void CBackendx86::EmitFooter(void)
{
  _out << _ind << ".end" << endl
       << "##################################################" << endl;
}

void CBackendx86::SetScope(CScope *scope)
{
  _curr_scope = scope;
}

CScope* CBackendx86::GetScope(void) const
{
  return _curr_scope;
}

void CBackendx86::EmitScope(CScope *scope)
{
  assert(scope != NULL);

  string label;

  if (scope->GetParent() == NULL) label = "main";
  else label = scope->GetName();

  // label
  _out << _ind << "# scope " << scope->GetName() << endl
       << label << ":" << endl;

  // TODO: done
  // ComputeStackOffsets(scope)
  CSymtab *symtab = scope->GetSymbolTable();

  size_t offset =  ComputeStackOffsets(symtab, 8, -12);
  _out << endl;
  _out << _ind << "# stack offset " << offset << endl;
  _out << _ind << "# function prologue " << offset << endl;

  // emit function prologue
  EmitInstruction("pushl", "%ebp");
  EmitInstruction("movl", "%esp, %ebp");
  // callee saved
  EmitInstruction("pushl", "%ebx");
  EmitInstruction("pushl", "%esi");
  EmitInstruction("pushl", "%edi");
  EmitInstruction("subl", "$"+to_string(offset)+", %esp");

  EmitInstruction("cld");
  EmitInstruction("xorl", "%eax, %eax");
  EmitInstruction("movl", "$"+to_string(offset/4)+ ", %ecx");
  EmitInstruction("mov", "%esp, %edi");
  EmitInstruction("rep", "stosl");

  if(scope->GetParent())
  EmitLocalData(scope);
  _out<<endl;
  // forall i in instructions do
  //   EmitInstruction(i)
  CCodeBlock* codeblock = scope->GetCodeBlock();
  list<CTacInstr*> instr = codeblock->GetInstr();
  for(list<CTacInstr*>::iterator it = instr.begin(); it != instr.end(); it++)
  {
    EmitInstruction(*it);
  }
  _out << endl;

  // emit function epilogue
  _out << Label("exit") << ":" << endl
       << _ind << "# epilogue " << endl;

  EmitInstruction("addl", "$" + to_string(offset) + ", %esp");
  EmitInstruction("popl", "%edi");
  EmitInstruction("popl", "%esi");
  EmitInstruction("popl", "%ebx");
  EmitInstruction("popl", "%ebp");
  EmitInstruction("ret");

  _out << endl;
}

void CBackendx86::EmitGlobalData(CScope *scope)
{
  assert(scope != NULL);

  // emit the globals for the current scope
  CSymtab *st = scope->GetSymbolTable();
  assert(st != NULL);

  bool header = false;

  vector<CSymbol*> slist = st->GetSymbols();

  _out << dec;

  size_t size = 0;

  for (size_t i=0; i<slist.size(); i++) {
    CSymbol *s = slist[i];
    const CType *t = s->GetDataType();

    if (s->GetSymbolType() == stGlobal) {
      if (!header) {
        _out << _ind << "# scope: " << scope->GetName() << endl;
        header = true;
      }

      // insert alignment only when necessary
      if ((t->GetAlign() > 1) && (size % t->GetAlign() != 0)) {
        size += t->GetAlign() - size % t->GetAlign();
        _out << setw(4) << " " << ".align "
             << right << setw(3) << t->GetAlign() << endl;
      }

      _out << left << setw(36) << s->GetName() + ":" << "# " << t << endl;

      if (t->IsArray()) {
        const CArrayType *a = dynamic_cast<const CArrayType*>(t);
        assert(a != NULL);
        int dim = a->GetNDim();

        _out << setw(4) << " "
          << ".long " << right << setw(4) << dim << endl;

        for (int d=0; d<dim; d++) {
          assert(a != NULL);

          _out << setw(4) << " "
            << ".long " << right << setw(4) << a->GetNElem() << endl;

          a = dynamic_cast<const CArrayType*>(a->GetInnerType());
        }
      }

      const CDataInitializer *di = s->GetData();
      if (di != NULL) {
        const CDataInitString *sdi = dynamic_cast<const CDataInitString*>(di);
        assert(sdi != NULL);  // only support string data initializers for now

        _out << left << setw(4) << " "
          << ".asciz " << '"' << sdi->GetData() << '"' << endl;
      } else {
        _out  << left << setw(4) << " "
          << ".skip " << dec << right << setw(4) << t->GetDataSize()
          << endl;
      }

      size += t->GetSize();
    }
  }

  _out << endl;

  // emit globals in subscopes (necessary if we support static local variables)
  vector<CScope*>::const_iterator sit = scope->GetSubscopes().begin();
  while (sit != scope->GetSubscopes().end()) EmitGlobalData(*sit++);
}

void CBackendx86::EmitLocalData(CScope *scope)
{
  assert(scope != NULL);

  // TODO: done InitializeLocalScope
  // forall arrays a belongsto local variables in scope do
  //     initialize meta-data for a on stack
  CSymtab *symtab = scope->GetSymbolTable();
  vector<CSymbol*> symbols = symtab->GetSymbols();

  _out << dec;

  for(int i = 0; i < symbols.size(); i++)
  {    
    CSymbol *s = symbols.at(i);
    const CType *type = s->GetDataType();

    if(type->IsArray())
    {
    // get type of array
      const CArrayType *arr = dynamic_cast<const CArrayType*>(type);
      assert(arr != NULL);  
      
      int dimension = arr->GetNDim();
      string reg = s->GetBaseRegister();
      int offset = s->GetOffset();

      string arg = Imm(dimension) + ", " + to_string(offset) + "(" + reg + ")";
      string comment = "local array '" + s->GetName() + "':" + to_string(dimension) + "dimensions";
      EmitInstruction("movl", arg, comment);

      for(int j=0; j < dimension; j++)
      {
        int nElem = arr->GetNElem();
        offset += 4;

        string arg = Imm(nElem) + ", " + to_string(offset) + "(" + reg + ")";
        string comment = "  dimension " + to_string(j + 1) + ": " + to_string(nElem) + " elements";
        EmitInstruction("movl", arg, comment);
        arr = dynamic_cast<const CArrayType*>(arr->GetInnerType());
      }
    }
  }
}

void CBackendx86::EmitCodeBlock(CCodeBlock *cb)
{
  assert(cb != NULL);

  const list<CTacInstr*> &instr = cb->GetInstr();
  list<CTacInstr*>::const_iterator it = instr.begin();

  while (it != instr.end()) EmitInstruction(*it++);
}

void CBackendx86::EmitInstruction(CTacInstr *i)
{
  assert(i != NULL);

  ostringstream cmt;
  string mnm;
  cmt << i;

  EOperation op = i->GetOperation();

  switch (op) {
    // binary operators
    // dst = src1 op src2
    // TODO: done
    case opAdd:
         Load(i->GetSrc(1), "%eax", cmt.str());
         Load(i->GetSrc(2), "%ebx");
         EmitInstruction("addl", "%ebx, %eax");
         Store(i->GetDest(), 'a');
         break;

    case opSub:
         Load(i->GetSrc(1), "%eax", cmt.str());
         Load(i->GetSrc(2), "%ebx");
         EmitInstruction("subl", "%ebx, %eax");
         Store(i->GetDest(), 'a');
         break;

    case opMul:
         Load(i->GetSrc(1), "%eax", cmt.str());
         Load(i->GetSrc(2), "%ebx");
         EmitInstruction("imull", "%ebx");
         Store(i->GetDest(), 'a');
         break;

    case opDiv:
         Load(i->GetSrc(1), "%eax", cmt.str());
         Load(i->GetSrc(2), "%ebx");
         EmitInstruction("cdq");
         EmitInstruction("idivl", "%ebx");
         Store(i->GetDest(), 'a');
         break;

    case opAnd:
         Load(i->GetSrc(1), "%eax", cmt.str());
         Load(i->GetSrc(2), "%ebx");
         EmitInstruction("andl", "%eax, %ebx");
         Store(i->GetDest(), 'a');
         break;

    case opOr:
         Load(i->GetSrc(1), "%eax", cmt.str());
         Load(i->GetSrc(2), "%ebx");
         EmitInstruction("orl", "%eax, %ebx");
         Store(i->GetDest(), 'a');
         break;
    // unary operators
    // dst = op src1
    // TODO
    case opPos:
         Load(i->GetSrc(1), "%eax", cmt.str());
         Store(i->GetDest(), 'a');
         break;
    case opNeg:
         Load(i->GetSrc(1), "%eax", cmt.str());
         EmitInstruction("negl", "%eax");
         Store(i->GetDest(), 'a');
         break;
    case opNot:
         Load(i->GetSrc(1), "%eax", cmt.str());
         EmitInstruction("notl", "%eax");
         Store(i->GetDest(), 'a');
         break;

    // memory operations
    // dst = src1
    // TODO
    case opAssign:
         Load(i->GetSrc(1), "%eax", cmt.str());
         Store(i->GetDest(), 'a');
         break;
    // pointer operations
    // dst = &src1
    // TODO
    // dst = *src1
    case opAddress:
         EmitInstruction("leal", Operand(i->GetSrc(1)) + ", %eax", cmt.str());
         Store(i->GetDest(), 'a');
         break;
    case opDeref:
      // opDeref not generated for now
      EmitInstruction("# opDeref", "not implemented", cmt.str());
      break;


    // unconditional branching
    // goto dst
    // TODO
    case opGoto:
      EmitInstruction("jmp", Label(dynamic_cast<const CTacLabel*>(i->GetDest())), cmt.str());
      break;

    // conditional branching
    // if src1 relOp src2 then goto dst
    // TODO
    case opEqual:
    case opNotEqual:
    case opLessEqual:
    case opLessThan:
    case opBiggerThan:
    case opBiggerEqual:
      Load(i->GetSrc(1), "%eax", cmt.str());
      Load(i->GetSrc(2), "%ebx");
      EmitInstruction("cmpl", "%ebx, %eax");
      EmitInstruction("j" + Condition(op), Label(dynamic_cast<const CTacLabel*>(i->GetDest())));
      break;


    // function call-related operations
    // TODO
   case opCall:
    {
      const CTacName *function = dynamic_cast<const CTacName*>(i->GetSrc(1));
      const CSymProc *symproc = dynamic_cast<const CSymProc*>(function->GetSymbol());
      assert(function != NULL);
      assert(symproc != NULL);

      EmitInstruction("call", symproc->GetName(), cmt.str());
      if(symproc->GetNParams() > 0)
        EmitInstruction("addl", "$" + to_string(4 * symproc->GetNParams()) + ", %esp");
      if(i->GetDest())
        Store(i->GetDest(), 'a');
      break;
    }
    case opReturn:
      if(i->GetSrc(1))
      {
        Load(i->GetSrc(1), "%eax", cmt.str());
        EmitInstruction("jmp", Label("exit"), cmt.str());
      }
      else
        EmitInstruction("jmp", Label("exit"), cmt.str());
      break;

    case opParam:
      Load(i->GetSrc(1), "%eax", cmt.str());
      EmitInstruction("pushl", "%eax");
      break;
    // special
    case opLabel:
      _out << Label(dynamic_cast<CTacLabel*>(i)) << ":" << endl;
      break;

    case opNop:
      EmitInstruction("nop", "", cmt.str());
      break;


    default:
      EmitInstruction("# ???", "not implemented", cmt.str());
  }
}

void CBackendx86::EmitInstruction(string mnemonic, string args, string comment)
{
  _out << left
       << _ind
       << setw(7) << mnemonic << " "
       << setw(23) << args;
  if (comment != "") _out << " # " << comment;
  _out << endl;
}

int count = 0;

void CBackendx86::Load(CTacAddr *src, string dst, string comment)
{
  assert(src != NULL);

  string mnm = "mov";
  string mod = "l";

  // set operator modifier based on the operand size
  switch (OperandSize(src)) {
    case 1: mod = "zbl"; break;
    case 2: mod = "zwl"; break;
    case 4: mod = "l"; break;
  }
 // emit the load instruction
  EmitInstruction(mnm + mod, Operand(src) + ", " + dst, comment);
}


void CBackendx86::Store(CTac *dst, char src_base, string comment)
{
  assert(dst != NULL);

  string mnm = "mov";
  string mod = "l";
  string src = "%";

  // compose the source register name based on the operand size
  switch (OperandSize(dst)) {
    case 1: mod = "b"; src += string(1, src_base) + "l"; break;
    case 2: mod = "w"; src += string(1, src_base) + "x"; break;
    case 4: mod = "l"; src += "e" + string(1, src_base) + "x"; break;
  }

  cout << *dst;
  printf("  %d: size:%d, mod= ", ++count , OperandSize(dst));
  cout << mod << endl;
  fflush(stdout);
 
  // emit the store instruction
  EmitInstruction(mnm + mod, src + ", " + Operand(dst), comment);
}

string CBackendx86::Operand(const CTac *op)
{
  string operand;

  // TODO: done
  // param: op, a three address code
  // return a string representing op
  // hint: take special care of references (op of type CTacReference)

  // *op is possibly a CTacReference, a CTacName, a CTacConst
  const CTacReference *ref = dynamic_cast<const CTacReference*>(op);
  const CTacName *name = dynamic_cast<const CTacName*>(op);
  const CTacConst *tacconst = dynamic_cast<const CTacConst *>(op);
  if(ref!=NULL)
  {
    const CSymbol *symbol = ref->GetSymbol();
    EmitInstruction("movl", to_string(symbol->GetOffset())+ "(" + symbol->GetBaseRegister() + "), %edi");
    operand = "(%edi)";
    return operand;
  }

  if(tacconst !=NULL)
  {
    operand = "$" + to_string(tacconst->GetValue());  
    return operand;
  }


  if(name !=NULL)
  {
    const CSymbol *symbol = name->GetSymbol();
    ESymbolType type = symbol->GetSymbolType();
    if(type == stGlobal || type == stProcedure)
      operand = symbol->GetName();
    else 
      // no need for braces
      operand = to_string(symbol->GetOffset())+"(" + symbol->GetBaseRegister()+")" ;
    return operand;
  }
  return operand;
}

string CBackendx86::Imm(int value) const
{
  ostringstream o;
  o << "$" << dec << value;
  return o.str();
}

string CBackendx86::Label(const CTacLabel* label) const
{
  CScope *cs = GetScope();
  assert(cs != NULL);

  ostringstream o;
  o << "l_" << cs->GetName() << "_" << label->GetLabel();
  return o.str();
  return "l_" + cs->GetName() + "_" + label->GetLabel();
}

string CBackendx86::Label(string label) const
{
  CScope *cs = GetScope();
  assert(cs != NULL);

  return "l_" + cs->GetName() + "_" + label;
}

string CBackendx86::Condition(EOperation cond) const
{
  switch (cond) {
    case opEqual:       return "e";
    case opNotEqual:    return "ne";
    case opLessThan:    return "l";
    case opLessEqual:   return "le";
    case opBiggerThan:  return "g";
    case opBiggerEqual: return "ge";
    default:            assert(false); break;
  }
}

int CBackendx86::OperandSize(CTac *t) const
{
  int size = 4;
  // compute the size for operand t of type CTacName
  // Hint: you need to take special care of references (incl. references to pointers!)
  //       and arrays. Compare your output to that of the reference implementation
  CTacName *name = dynamic_cast<CTacName*>(t);
  CTacReference *ref = dynamic_cast<CTacReference*>(t);

  if(ref != NULL)
  {
    const CSymbol *deref = ref->GetDerefSymbol();
    const CType *type = deref->GetDataType();
    if(type->IsPointer())
    {
      type = dynamic_cast<const CPointerType*>(type)->GetBaseType();
      const CType *base = dynamic_cast<const CArrayType*>(type)->GetBaseType();
      size = base->GetDataSize();
    }else if(type->IsArray())
    {
      const CType *base = dynamic_cast<const CArrayType*>(type)->GetBaseType();
      size = base->GetDataSize();
    }else
    {
      size = type->GetDataSize();
    }
    cout << "refsize: " << size << endl;
    return size;
  }
 
  if(name != NULL)
  {
    size = name->GetSymbol()->GetDataType()->GetDataSize();
    return size;
  }

 return size;
}




size_t CBackendx86::ComputeStackOffsets(CSymtab *symtab,
                                        int param_ofs,int local_ofs)
{
  assert(symtab != NULL);
  vector<CSymbol*> slist = symtab->GetSymbols();

  size_t size = 0;

  /* foreach local symbol l in slist do
   *   compute aligned offset on stack and store in symbol l
   *   set base register to %ebp
   */
  for (const auto &l : slist) {
    if (l->GetSymbolType() != stLocal)
      continue;

    const CType* datatype = l->GetDataType();
    if (datatype->IsInt() || datatype->IsPointer()) {
      if (local_ofs % 4) {
        int padding = 4 + local_ofs % 4; /* local_ofs < 0 */
        size += padding;
        local_ofs += -padding;
      }

      size += 4;
      local_ofs += -4;
      l->SetOffset(local_ofs);
      l->SetBaseRegister("%ebp");
    }
    else if (datatype->IsChar() || datatype->IsBoolean()) {
      size += 1;
      local_ofs += -1;
      l->SetOffset(local_ofs);
      l->SetBaseRegister("%ebp");
    }
    else if (datatype->IsArray()) {
      if ((local_ofs - datatype->GetSize()) % 4) {
        int padding = 4 + (local_ofs - datatype->GetSize()) % 4;
        size += padding;
        local_ofs += -padding;
      }

      size += datatype->GetSize();
      local_ofs += -datatype->GetSize();
      l->SetOffset(local_ofs);
      l->SetBaseRegister("%ebp");
    }
  }

  /* foreach parameter p in slist do
   *   compute offset on stack and store in symbol p
   *   set base register to %ebp
   */
  for (const auto &p : slist) {
    if (p->GetSymbolType() != stParam)
      continue;

    /* parameter doesn't require alignment rule.
     * its offset is always (start_param_ofs + 4 * index)
     */
    int offset = param_ofs + 4 * dynamic_cast<CSymParam*>(p)->GetIndex();
    p->SetOffset(offset);
    p->SetBaseRegister("%ebp");
  }

  /* align size */
  if (local_ofs % 4) {
    int padding = 4 + local_ofs % 4;
    size += padding;
    local_ofs += -padding;
  }

  /* dump stack frame to assembly file */
  for (const auto &s : slist) {
    ESymbolType stype = s->GetSymbolType();
    if (stype != stLocal && stype != stParam)
      continue;

    _out << _ind << "#" << setw(7) << std::right << s->GetOffset() << "(" << s->GetBaseRegister() << ")"
         << setw(4) << std::right << s->GetDataType()->GetSize() << setw(2) << s << endl;
  }

  return size;
}









/*
size_t CBackendx86::ComputeStackOffsets(CSymtab *symtab,
                                        int param_ofs,int local_ofs)
{
  // we set param_ofs =8, local_ofs=-12 in EmitScope
  // param_ofs is the offset to parameters from base pointer(ebp) after epilogue, 
  // local_ofs is offset to local vars  from base pointer(ebp) after epilogue
  int size = 0;
  // this is the total size of the stack frame
  assert(symtab != NULL);
  vector<CSymbol*> slist = symtab->GetSymbols();

  // TODO: done
  // symbol in the table
  CSymbol *symbol;
  // data type
  const CType *type;

  // foreach local symbol l in slist do
  //   compute aligned offset on stack and store in symbol l
  //   set base register to %ebp
  for(int i=0; i < slist.size(); i++)
  {
    symbol = slist.at(i);
    if(symbol->GetSymbolType() != stLocal)
      // global symbols are not handled here
      continue;
    type = symbol->GetDataType();

    
    if(type->IsChar() || type->IsBoolean())
      // both size = 1
    {
      size++;
      local_ofs--;
      // for each symbol, its offset must be set
      symbol->SetOffset(local_ofs);
      symbol->SetBaseRegister("%ebp");
    }
    else
    // In this machine pointer size is the same as an int
    if(type->IsInt() || type->IsPointer())
    {
      if(local_ofs % 4)
      {
        int pad = 4 + local_ofs % 4;
        size += pad;
        local_ofs -= pad;
      }
      size +=4;
      local_ofs -= 4;
      symbol->SetBaseRegister("%ebp");

      symbol->SetOffset(local_ofs);
    }
    else
    if(type->IsArray())
    {
      int pad = (local_ofs - type->GetSize()) % 4;
      if(pad)
      {
        pad = local_ofs % 4 + 4;
        size += pad;
        local_ofs -= pad;
      }
      local_ofs -= type->GetSize();  
      size += type->GetSize();
      symbol->SetOffset(local_ofs);
      symbol->SetBaseRegister("%ebp");
   }
  }
 // foreach parameter p in slist do
  //   compute offset on stack and store in symbol p
  //   set base register to %ebp
  for(int i=0; i < slist.size(); i++)
  {
    symbol = slist.at(i);
    if(symbol->GetSymbolType() != stParam)
      // ignore anything other than parameter
      continue;
    // offset of a parameter is easy to measure
    int ofs = param_ofs + dynamic_cast<CSymParam*>(symbol)->GetIndex() * 4;
    symbol->SetOffset(ofs);
    symbol->SetBaseRegister("%ebp");
  }
  // align size
  int pad = local_ofs % 4;
  if(pad!=0)
  {
    pad = local_ofs%4 +4;
    size += pad;
    local_ofs -= pad;
  }
  // dump stack frame to assembly file
  for(int i=0; i < slist.size(); i++)
  {
    symbol = slist.at(i);
    ESymbolType symboltype = symbol->GetSymbolType();
    if(symboltype != stLocal && (symboltype != stParam))
      continue;

    _out << _ind << "#" << setw(7) << std::right << symbol->GetOffset() << "(" << symbol->GetBaseRegister() << ")" << setw(4) << std::right << symbol->GetDataType()->GetSize() << setw(2) << symbol << endl;
  }
  return size;
}


*/



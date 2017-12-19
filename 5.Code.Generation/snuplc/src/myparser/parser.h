//------------------------------------------------------------------------------
/// @brief SnuPL/0 parser
/// @author Bernhard Egger <bernhard@csap.snu.ac.kr>
/// @section changelog Change Log
/// 2012/09/14 Bernhard Egger created
/// 2013/03/07 Bernhard Egger adapted to SnuPL/0
/// 2016/03/09 Bernhard Egger adapted to SnuPL/1
/// 2016/04/08 Bernhard Egger assignment 2: parser for SnuPL/-1
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

#ifndef __SnuPL_PARSER_H__
#define __SnuPL_PARSER_H__

#include "scanner.h"
#include "symtab.h"
#include "ast.h"


//------------------------------------------------------------------------------
/// @brief parser
///
/// parses a module
///
class CParser {
  public:
    /// @brief constructor
    ///
    /// @param scanner  CScanner from which the input stream is read
    CParser(CScanner *scanner);

    /// @brief parse a module
    /// @retval CAstNode program node
    CAstNode* Parse(void);

    /// @name error handling
    ///@{

    /// @brief indicates whether there was an error while parsing the source
    /// @retval true if the parser detected an error
    /// @retval false otherwise
    bool HasError(void) const { return _abort; };

    /// @brief returns the token that caused the error
    /// @retval CToken containing the error token
    const CToken* GetErrorToken(void) const;

    /// @brief returns a human-readable error message
    /// @retval error message
    string GetErrorMessage(void) const;
    ///@}

  private:
    /// @brief sets the token causing a parse error along with a message
    /// @param t token causing the error
    /// @param message human-readable error message
    void SetError(CToken t, const string message);

    /// @brief consume a token given type and optionally store the token
    /// @param type expected token type
    /// @param token If not null, the consumed token is stored in 'token'
    /// @retval true if a token has been consumed
    /// @retval false otherwise
    bool Consume(EToken type, CToken *token=NULL);


    /// @brief initialize symbol table @a s with predefined procedures and
    ///        global variables
    void InitSymbolTable(CSymtab *s);

    /// @name methods for recursive-descent parsing
    /// @{

    /// @brief parse var declaration
    /// 	global variables
    void stat_var(CAstScope *m);

    /// @brief subroutine
    /// @param m the module where subroutine is defined
    ///		global variables
    void subroutineDecl(CAstScope *s);   
    void procedureDecl(CAstScope *s);
    void functionDecl(CAstScope *s);

    void varDeclSequence(CAstScope *);
    void varDecl(CAstScope *);
    vector <CToken> ident(CAstScope *);
    const CType* type(CAstScope *);

    void paramDeclSequence(CAstProcedure *s);
    void paramDecl(CAstProcedure *s);


    CAstModule*       module(void);


    CAstStatement*    statSequence(CAstScope *s);

    CAstStatWhile*    stat_while(CAstScope *s);
    CAstStatIf*       stat_if(CAstScope *s);
    CAstStatCall*     stat_call(CAstScope *s);
    CAstStatReturn*   stat_return(CAstScope *s);
//    CAstType*	      stat_type(vector <CToken>, CAstScope*);
    CAstStatAssign*   assignment(CAstScope *s);
    CAstDesignator*   qualident(CAstScope *s);
    
    CAstExpression*   expression(CAstScope *s);
    CAstExpression*   simpleexpr(CAstScope *s);
    CAstExpression*   term(CAstScope *s);
    CAstExpression*   factor(CAstScope *s);
    

    CAstConstant*     number(int=0);
    CAstConstant*     boolean(void);
    CAstConstant*     character(void);
    CAstStringConstant*     stringC(CAstScope* s);
    /// @}

	CAstType* type_e(bool isParam);
	void varDeclInternal_e(vector<string> &vars, vector<string> &allVars);
	void varDeclParam_e(vector<string> &vars, CAstType* &ttype, vector<string> &allVars);
	void varDecl_e(vector<string> &vars, CAstType* &ttype, vector<string> &allVars);
	void varDeclaration_e(CAstScope *s);
	CAstDesignator* qualident_e(CAstScope *s);
	CAstDesignator* ident_e(CAstScope *s);	
	void formalParam_e(vector<string> &paramNames, vector<CAstType*> &paramTypes);
	CAstProcedure* procedureDecl_e(CAstScope *s);
	void AddParameters_e(CAstScope *s, CSymProc *symbol, vector<string> &paramNames, vector<CAstType*> &paramTypes);
	CAstExpression* expression_e(CAstScope* s);
	CAstExpression* simpleexpr_e(CAstScope *s);
	CAstFunctionCall* functionCall_e(CAstScope *s);
	CAstStatCall* subroutineCall_e(CAstScope *s);
	CAstExpression* addressExpression(CAstScope* s);
	void subroutineBody_e(CAstScope *s);
	
    CScanner     *_scanner;       ///< CScanner instance
    CAstModule   *_module;        ///< root node of the program
    CToken        _token;         ///< current token

    /// @name error handling
    CToken        _error_token;   ///< error token
    string        _message;       ///< error message
    bool          _abort;         ///< error flag

};


#endif // __SnuPL_PARSER_H__
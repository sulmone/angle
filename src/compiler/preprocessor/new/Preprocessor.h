//
// Copyright (c) 2011 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

#ifndef COMPILER_PREPROCESSOR_PREPROCESSOR_H_
#define COMPILER_PREPROCESSOR_PREPROCESSOR_H_

#include "DirectiveParser.h"
#include "Macro.h"
#include "MacroExpander.h"
#include "Tokenizer.h"

namespace pp
{

class Diagnostics;

class Preprocessor
{
  public:
    Preprocessor(Diagnostics* diagnostics);

    // count: specifies the number of elements in the string and length arrays.
    // string: specifies an array of pointers to strings.
    // length: specifies an array of string lengths.
    // If length is NULL, each string is assumed to be null terminated.
    // If length is a value other than NULL, it points to an array containing
    // a string length for each of the corresponding elements of string.
    // Each element in the length array may contain the length of the
    // corresponding string or a value less than 0 to indicate that the string
    // is null terminated.
    bool init(int count, const char* const string[], const int length[]);

    void lex(Token* token);

  private:
    PP_DISALLOW_COPY_AND_ASSIGN(Preprocessor);

    Diagnostics* mDiagnostics;

    MacroSet mMacroSet;
    Tokenizer mTokenizer;
    DirectiveParser mDirectiveParser;
    MacroExpander mMacroExpander;
};

}  // namespace pp
#endif  // COMPILER_PREPROCESSOR_PREPROCESSOR_H_


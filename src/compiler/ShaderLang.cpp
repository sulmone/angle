//
// Copyright (c) 2002-2010 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

//
// Implement the top-level of interface to the compiler,
// as defined in ShaderLang.h
//

#include "GLSLANG/ShaderLang.h"

#include "compiler/Initialize.h"
#include "compiler/InitializeDll.h"
#include "compiler/ParseHelper.h"
#include "compiler/ShHandle.h"
#include "compiler/SymbolTable.h"

//
// This is the platform independent interface between an OGL driver
// and the shading language compiler.
//

//
// Driver must call this first, once, before doing any other
// compiler operations.
//
int ShInitialize()
{
    if (!InitProcess())
        return 0;

    return 1;
}

//
// Cleanup symbol tables
//
int ShFinalize()
{
    if (!DetachProcess())
        return 0;

    return 1;
}

//
// Initialize built-in resources with minimum expected values.
//
void ShInitBuiltInResource(TBuiltInResource* resources)
{
    // Constants.
    resources->MaxVertexAttribs = 8;
    resources->MaxVertexUniformVectors = 128;
    resources->MaxVaryingVectors = 8;
    resources->MaxVertexTextureImageUnits = 0;
    resources->MaxCombinedTextureImageUnits = 8;
    resources->MaxTextureImageUnits = 8;
    resources->MaxFragmentUniformVectors = 16;
    resources->MaxDrawBuffers = 1;

    // Extensions.
    resources->OES_standard_derivatives = 0;
}

//
// Driver calls these to create and destroy compiler objects.
//
ShHandle ShConstructCompiler(EShLanguage language, EShSpec spec, const TBuiltInResource* resources)
{
    if (!InitThread())
        return 0;

    TShHandleBase* base = static_cast<TShHandleBase*>(ConstructCompiler(language, spec));
    TCompiler* compiler = base->getAsCompiler();
    if (compiler == 0)
        return 0;

    // Generate built-in symbol table.
    if (!compiler->Init(*resources)) {
        ShDestruct(base);
        return 0;
    }

    return reinterpret_cast<void*>(base);
}

void ShDestruct(ShHandle handle)
{
    if (handle == 0)
        return;

    TShHandleBase* base = static_cast<TShHandleBase*>(handle);

    if (base->getAsCompiler())
        DeleteCompiler(base->getAsCompiler());
}

//
// Do an actual compile on the given strings.  The result is left 
// in the given compile object.
//
// Return:  The return value of ShCompile is really boolean, indicating
// success or failure.
//
int ShCompile(
    const ShHandle handle,
    const char* const shaderStrings[],
    const int numStrings,
    int compileOptions)
{
    if (!InitThread())
        return 0;

    if (handle == 0)
        return 0;

    TShHandleBase* base = reinterpret_cast<TShHandleBase*>(handle);
    TCompiler* compiler = base->getAsCompiler();
    if (compiler == 0)
        return 0;

    GlobalPoolAllocator.push();

    bool success = compiler->compile(shaderStrings, numStrings, compileOptions);

    //
    // Throw away all the temporary memory used by the compilation process.
    //
    GlobalPoolAllocator.pop();

    return success ? 1 : 0;
}

void ShGetInfo(const ShHandle handle, EShInfo pname, int* params)
{
    if (!handle || !params)
        return;

    TShHandleBase* base = static_cast<TShHandleBase*>(handle);
    TCompiler* compiler = base->getAsCompiler();
    if (!compiler) return;

    switch(pname)
    {
    case SH_INFO_LOG_LENGTH:
        *params = compiler->getInfoSink().info.size() + 1;
        break;
    case SH_OBJECT_CODE_LENGTH:
        *params = compiler->getInfoSink().obj.size() + 1;
        break;
    case SH_ACTIVE_UNIFORMS:
        UNIMPLEMENTED();
        break;
    case SH_ACTIVE_UNIFORM_MAX_LENGTH:
        UNIMPLEMENTED();
        break;
    case SH_ACTIVE_ATTRIBUTES:
        UNIMPLEMENTED();
        break;
    case SH_ACTIVE_ATTRIBUTE_MAX_LENGTH:
        UNIMPLEMENTED();
        break;

    default: UNREACHABLE();
    }
}

//
// Return any compiler log of messages for the application.
//
void ShGetInfoLog(const ShHandle handle, char* infoLog)
{
    if (!handle || !infoLog)
        return;

    TShHandleBase* base = static_cast<TShHandleBase*>(handle);
    TCompiler* compiler = base->getAsCompiler();
    if (!compiler) return;

    TInfoSink& infoSink = compiler->getInfoSink();
    strcpy(infoLog, infoSink.info.c_str());
}

//
// Return any object code.
//
void ShGetObjectCode(const ShHandle handle, char* objCode)
{
    if (!handle || !objCode)
        return;

    TShHandleBase* base = static_cast<TShHandleBase*>(handle);
    TCompiler* compiler = base->getAsCompiler();
    if (!compiler) return;

    TInfoSink& infoSink = compiler->getInfoSink();
    strcpy(objCode, infoSink.obj.c_str());
}

void ShGetActiveAttrib(const ShHandle handle,
                       int index,
                       int* length,
                       int* size,
                       EShDataType* type,
                       char* name)
{
    UNIMPLEMENTED();
}

void ShGetActiveUniform(const ShHandle handle,
                        int index,
                        int* length,
                        int* size,
                        EShDataType* type,
                        char* name)
{
    UNIMPLEMENTED();
}

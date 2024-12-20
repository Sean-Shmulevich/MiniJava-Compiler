// stdlib headers
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <unordered_map>

// Declarations of LLVM global objects like TheContext, TheBuilder, TheModule
#include "llvm/LLVMGlobals.h"

int getIndex(const char *string);
const char *getString(int i);

static llvm::Value *AddExpr(uintptr_t a);
static llvm::Value *AddSimpleExpr(uintptr_t a);
static llvm::Value *AddFactor(uintptr_t a);
static void createFormatStr(std::vector<llvm::Value *> &arguments, llvm::Constant *&formatStr);
static llvm::Value *AddTerm(uintptr_t a);
static llvm::Value *AddVarInit(uintptr_t a);
static void AddArrInit(uintptr_t a);

extern "C"
{
#include "proj2.h"
}
#include "proj3.h"

static llvm::Function *funcPrintf = nullptr;

// Global map to store string constants and avoid duplicates
static std::map<std::string, llvm::Constant *> ConstMap;
// maps string -> symbol table index
// maybe better, string -> string table index.
static std::map<std::string, int> STMap;
static std::map<int, std::string> STInvertedMap;
static llvm::StructType *currClass;

static llvm::BasicBlock *bb;
// Helper function to get or create a string constant
static void setConstants()
{
    int i = 0;
    int wordStart = 0;
    int strLen = 1;
    const char *currChar = getString(i);
    const char *programStr = getString(GetAttr(0, NAME_ATTR));

    // add newline into map for the "ln" in println lol.
    std::string newLine = "\n";
    llvm::Constant *strConstant = TheBuilder.CreateGlobalString(newLine);
    ConstMap[newLine] = strConstant;

    // I always do shitty logic like this when im tired.
    while (currChar != NULL)
    {
        if (*(currChar) == '\0')
        {
            auto fullWord = getString(wordStart);
            auto tempStr = llvm::StringRef(fullWord);

            // key exists in symbol table, do not add to the constants.
            if (STMap.count(fullWord) == 0 && ConstMap.count(tempStr) == 0 && strcmp(programStr, fullWord) != 0)
            {
                llvm::Constant *strConstant = TheBuilder.CreateGlobalString(tempStr);
                ConstMap[fullWord] = strConstant;
            }

            wordStart = i;
        }
        // todo: check if its in the symbol table or not
        // printf("%s\n", currChar);
        currChar = getString(i);

        strLen += 1;
        i += 1;
    }
}

// map the actual string to the st_index
static void makeStMap()
{
    for (int st_index = 1; st_index < GetSymbolTableSize(); st_index++)
    {
        int stringTableLoc = GetAttr(st_index, NAME_ATTR);
        std::string st_string = getString(stringTableLoc);
        STMap[st_string] = st_index;
        STInvertedMap[st_index] = st_string;
    }
}

std::map<std::string, tree> getMethodProperties(tree treenode)
{
    std::map<std::string, tree> infoMap;

    // Populate the map with keys as variable names and values as uintptr_t
    infoMap["method_block"] = RightChild(treenode);
    infoMap["method_stmts"] = RightChild(RightChild(treenode));
    infoMap["method_decl_list"] = LeftChild(RightChild(treenode));
    infoMap["method_first_decl"] = RightChild(LeftChild(RightChild(treenode)));

    tree head_op = LeftChild(treenode);
    infoMap["method_id"] = LeftChild(head_op);

    tree spec_op = RightChild(head_op);
    // spec op is part of the formal parameter list.
    // formal parameter list could contain the type of another class.
    // but besides that its pretty boring
    // Maybe you can initialize default values here?
    infoMap["arg_list"] = spec_op;
    infoMap["method_type"] = RightChild(spec_op);
    infoMap["method_args"] = LeftChild(spec_op);

    return infoMap;
}

// static void addCallOp(std::vector<llvm::Value *, std::allocator<llvm::Value *>> callParams, llvm::BasicBlock bb)
// {
//     llvm::CallInst *callInst = llvm::CallInst::Create(funcPrintf, callParams, "call", bb);
//     // return 0;
//     TheBuilder.CreateRet(llvm::ConstantInt::get(TheContext, llvm::APInt(32, 0)));
// }
// this could maybe be made into a more generic thing other than a function.
static llvm::Function *makeFuncHeader(std::string name, tree typeNode)
{

    tree typeId = LeftChild(typeNode);
    // no examples have this.
    tree subtype = RightChild(typeNode);

    int typeKind = NodeKind(typeId);
    llvm::FunctionType *funcType;
    if (typeKind == STNode)
    {
        // TODO: make it the type of the enclosing class.
        // we need to make sure we are in the class.
        // no we are in the program scope, we need to make sure its in the program scope.
        // then lookup that value in scope.
        funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(TheContext), currClass, true);
    }
    else if (typeKind == INTEGERTNode)
    {
        funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(TheContext), false);
    }
    else
    {
        funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(TheContext), false);
    }
    llvm::Function *func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, llvm::StringRef(name), TheModule.get());

    // create basic block for function.
    return func;
}

static llvm::BasicBlock *initFuncBlock(llvm::Function *func)
{
    llvm::BasicBlock *bb = llvm::BasicBlock::Create(TheContext, "entry", func);
    TheBuilder.SetInsertPoint(bb);
    return bb;
}

// NOT RUNNING
static std::vector<llvm::Value *> AddCallArgs(tree argsList)
{
    // printf("here in add callargs")t
    std::vector<llvm::Value *> callParams;
    tree exprTree = LeftChild(argsList);
    // this might need to return something
    // this is maybe a const
    llvm::Value *argValue = AddExpr((uintptr_t)exprTree);

    if (argValue)
    {
        callParams.push_back(argValue);
    }

    if (!IsNull(RightChild(argsList)))
    {
        std::vector<llvm::Value *> additionalParams = AddCallArgs(RightChild(argsList));

        // Append additional parameters to the existing list
        callParams.insert(
            callParams.end(),
            additionalParams.begin(),
            additionalParams.end());
    }
    return callParams;
}

// returns the fn (for now).
static llvm::Value *AddField(tree treenode)
{
    tree fieldOp = LeftChild(treenode);
    tree fieldId = LeftChild(LeftChild(treenode));
    tree nextSelect = RightChild(treenode);

    llvm::Value *llvmFunc = (llvm::Value *)GetAttr(IntVal(fieldId), OBJECT_ATTR);
    // printf("HERE?\n");
    // todo, recurse right for more selectOP,s
    return llvmFunc;
}

static llvm::Value *AddRoutineCall(tree theStmt)
{
    // printf("Add routine call\n");

    // Extract select operator and the method/function identifier
    tree selectOp = RightChild(LeftChild(theStmt));
    tree argumentTree = RightChild(theStmt);
    tree objectVar = LeftChild(LeftChild(theStmt)); // e.g., "system"

    uintptr_t funcValue;
    std::vector<llvm::Value *> arguments;

    // Case 1: Check for method call with object (system.println)
    if ((NodeOp(selectOp) == SelectOp && STInvertedMap[IntVal(objectVar)].compare("system") == 0))
    {
        // printf("SYSTEM");
        arguments = AddCallArgs(argumentTree);

        llvm::Constant *formatStr = ConstMap["\n"];
        // createFormatStr(arguments, formatStr);
        // llvm::Value *data = TheBuilder.CreateLoad(arguments[0]->getType()->getPointerElementType(), arguments[0], "yo");

        // arguments.push_(formatStr);
        // arguments.insert(arguments.begin(), formatStr);
        // llvm::CallInst *callInst = llvm::CallInst::Create(funcPrintf, arguments, "println", bb);

        // arguments.clear();
        // llvm::Constant *newlineConst = ConstMap["\n"];
        // arguments.push_back(newlineConst);
        return llvm::CallInst::Create(funcPrintf, arguments, "print", bb);
        // llvm::ConstantInt::get(TheContext, llvm::APInt(32, 0));
    }

    else if (NodeOp(selectOp) == SelectOp)
    {
        tree fieldVar = LeftChild(LeftChild(selectOp));
        // tree methodVar = RightChild(selectOp); // e.g., "println"

        // printf("the one with\n");
        int stIndex = IntVal(fieldVar);

        // printf("%s\n", STInvertedMap[stIndex].c_str());
        llvm::Value *llvmFunc = (llvm::Value *)GetAttr(IntVal(fieldVar), OBJECT_ATTR);
        // Process arguments for the call
        arguments = AddCallArgs(argumentTree);

        llvm::CallInst *callInst = llvm::CallInst::Create(llvmFunc, arguments, "ELLLO", bb);
        return callInst;
    }
    // Case 2: Simple function call without SelectOp
    else if (!IsNull(theStmt) && IsNull(selectOp))
    {
        // printf("the one without params\n");
        int stIndex = IntVal(LeftChild(LeftChild(theStmt)));

        funcValue = GetAttr(stIndex, OBJECT_ATTR);
        if (!funcValue)
        {
            fprintf(stderr, "Error: Function %s not found!\n", STInvertedMap[stIndex].c_str());
            return nullptr;
        }
        llvm::Value *llvmFunc = (llvm::Value *)funcValue;

        arguments = AddCallArgs(argumentTree);
        llvm::CallInst *callInst = llvm::CallInst::Create(llvmFunc, arguments, "ELLLO", bb);
        return callInst;
    }
    else
    {
        fprintf(stderr, "Error: Unsupported call format\n");
        return nullptr;
    }

    // Generate LLVM function call instruction
    return nullptr;
}

void createFormatStr(std::vector<llvm::Value *> &arguments, llvm::Constant *&formatStr)
{
    auto arg = arguments[0];
    if (arguments[0]->getType()->isIntegerTy())
    {
        // If it's an integer, use "%d" format specifier
        formatStr = TheBuilder.CreateGlobalStringPtr("%d\n", "printf.num");
    }
    else if (arguments[0]->getType()->isPointerTy())
    {
        // If it's a string (pointer), use "%s" format specifier
        formatStr = TheBuilder.CreateGlobalStringPtr("%s\n", "printf.str");
    }
    else
    {
        // Default fallback
        formatStr = TheBuilder.CreateGlobalStringPtr("\n", "printf.newline");
    }
}

// dont really need recursion here directly.
static llvm::Value *AddFactor(uintptr_t factorTree)
{
    tree factor = (tree)factorTree;
    // TODO emit simple expression with proper comparison
    int kind = NodeKind(factor);
    if (kind == EXPRNode)
    {
        if (NodeOp(factor) == VarOp)
        {
            tree factorId = LeftChild(factor);
            std::string factorStr = STInvertedMap[IntVal(factorId)];
            // printf("Here in ada %s\n", getString(IntVal(factor)));
            llvm::Value *fieldOps = AddField(factor);
            // if the value is a class.
            // is it a STNode?
            // what is the condition for class?
            // printf("%d %s\n", GetAttr(IntVal(factorId), KIND_ATTR) == CLASS, factorStr.c_str());
            // lookup X in current scope before creating.
            // the class ones will be created before the function yes.

            // If I make sure that all of the decl are created before I come here,
            // I can translate OBJECT_ATTR -> llvm::Value, and check if that value is a function or a class
            // it has pops on it. like ->getType()
            // DO AN extra pass to add in the Decls.
            llvm::GlobalVariable *globalStr = TheModule->getGlobalVariable("Variables.global");
            // check if something exists, where? in globals?
            // this is where we get variable value.
            llvm::Value *opInit = (llvm::Value *)GetAttr(IntVal(factorId), OBJECT_ATTR);
            // printf("when we tried to find the declaratino var we found, %d", IntVal(factorId));
            // we have a variable?
            // we have a var if the type of the node is alloc

            // opInit->getType();
            // .getName -> lookup in StMap mapping ->
            if (opInit)
            {

                if (llvm::AllocaInst *alloca = llvm::dyn_cast<llvm::AllocaInst>(opInit))
                {
                    // It is an AllocaInst
                    llvm::Value *store = TheBuilder.CreateStore(llvm::ConstantInt::get(TheContext, llvm::APInt(32, 4)), opInit);
                    return store;
                }
                else
                {

                    // llvm::Value *classUsage = TheBuilder.CreateStructGEP(TheModule->getNamedGlobal("Variables.global"), 0, "point.x");
                    // also
                    llvm::Value *classLoad = TheBuilder.CreateLoad(opInit->getType()->getPointerElementType(), opInit, "point.x.val");
                    return classLoad;
                }
            }
            // else /* if (NodeKind(factor) == VAR)*/
            // {
            //     // llvm::ConstantInt *varInit = llvm::ConstantInt::get(TheContext, llvm::APInt(32, 4));
            //     // llvm::AllocaInst *iVar = TheBuilder.CreateAlloca(llvm::Type::getInt32Ty(TheContext), nullptr, factorStr);
            //     // TheBuilder.CreateStore(llvm::ConstantInt::get(TheContext, llvm::APInt(32, 4)), iVar);

            //     // llvm::Value *varLoad = TheBuilder.CreateLoad(varInit, iVar, "y");
            //     // get iVar from looking up the factor str.
            //     // llvm::Value *iVal = TheBuilder.CreateLoad(iVar->getAllocatedType(), iVar, factorStr);

            //     // // llvm::Value *varStore = TheBuilder.CreateStore(varLoad, varStore);
            //     // return iVal;
            // }

            // Assume this is the varop for the class (for now)

            // get the value at that name of class.
            // basically the name of var should either be created or looked up.

            // llvm::Value *classStore = TheBuilder.CreateStore(classUsage, classLoad);

            // return
            // return a variable node.
            // return;
            // emit its usage?
        }
        else if (NodeOp(factor) == RoutineCallOp)
        {
            // printf("JAWWN\n");
            std::vector<llvm::Value *> callParams;
            return AddRoutineCall(factor);
        }
        else if (NodeOp(factor) == GTOp || NodeOp(factor) == GEOp || NodeOp(factor) == LTOp || NodeOp(factor) == LEOp || NodeOp(factor) == EQOp || NodeOp(factor) == NEOp)
        {
            // printf("Here in fud %s\n", getString(IntVal(factor)));
            return AddExpr(factorTree);
        }
    }
    else if (kind == NUMNode)
    {
        int numValue = IntVal(factor);
        // TheBuilder.CreateRet(llvm::ConstantInt::get(TheContext, llvm::APInt(32, 0)));
        return llvm::ConstantInt::get(TheContext, llvm::APInt(32, numValue, true));
    }
    else if (kind == STRINGNode)
    {
        // printf("Here in stingnode %s\n", getString(IntVal(factor)));
        if (ConstMap.count(getString(IntVal(factor))) == 1)
        {
            // printf("WE FOUND IT", getString(IntVal(factor)));

            // auto strConst = ConstMap["\n"];
            // llvm::CallInst *callInst = TheBuilder.CreateCall(funcPrintf, callParams, "call");
            // std::vector<llvm::Value *> callParams;
            auto strConst = ConstMap[getString(IntVal(factor))];
            // callParams.push_back(strConst);
            // llvm::CallInst::Create(funcPrintf, callParams, "print", bb);

            return strConst;
            // callParams.clear();

            // llvm::Constant *newlineConst = ConstMap["\n"];
            // callParams.push_back(newlineConst);
            // llvm::CallInst::Create(funcPrintf, callParams, "print", bb);
        }
        // llvm::IRBuilder<> builder(currentBB);

        // this is lowkey the call for the system class?
        // llvm::CallInst::Create(funcPrintf, callParams, "call0", bb);
    }
    return nullptr;
}

static llvm::Value *AddTerm(uintptr_t termTree)
{
    tree term = (tree)termTree;
    if (IsNull(term))
    {
        return nullptr;
    }

    int opType = NodeOp(term);
    // if (opType == MultOp)
    // {
    //     // emit something and keep traversing
    // }
    // else if (opType == DivOp)
    // {
    //     // emit something and keep traversing
    // }
    // else if (opType == AndOp)
    // {
    //     // emit something and keep traversing
    // }
    // else
    // {
    //     // its just a factor
    //     printf("Here in term\n");
    // }
    return AddFactor(termTree);
}

// TODO: addop should be two types i think,
// one at head and N ones following.
static llvm::Value *AddSimpleExpr(uintptr_t exprTree)
{
    tree expr = (tree)exprTree;
    // printf("Here in simple expression\n");
    if (NodeOp(expr) == AddOp || NodeOp(expr) == OrOp || NodeOp(expr) == SubOp)
    {
        // Left side could be a term or another simple expression
        int opCode = NodeOp(expr);
        tree leftTree = LeftChild(expr);
        llvm::Value *leftValue = AddSimpleExpr((uintptr_t)leftTree);
        // Right side is always a single term
        tree rightTree = RightChild(expr);
        llvm::Value *rightValue = AddTerm((uintptr_t)rightTree);

        // after might make sense.
        // the right side goes deeper.
        // I want to build the rightmost first.
        // so recurse all the way down first
        // looking at the tree it might be the other way.
        // Perform the correct operation based on the opcode
        if (opCode == AddOp)
        {
            return TheBuilder.CreateAdd(leftValue, rightValue, "add_result");
        }
        else if (opCode == SubOp)
        {
            return TheBuilder.CreateSub(leftValue, rightValue, "sub_result");
        }
        else if (opCode == OrOp)
        {
            return TheBuilder.CreateOr(leftValue, rightValue, "or_result");
        }
    }
    // this will only ever be the top.
    else if (NodeOp(expr) == UnaryNegOp)
    {
        // this is always null.
        // tree rightSide = RightChild(expr);
        // left is either factor/term, or another addOp, maybe even unarynegop
        // tree leftSide = LeftChild(expr);
        // tree term = LeftChild(expr);

        // Negation applies to a single term
        tree termTree = LeftChild(expr);
        llvm::Value *termValue = AddTerm((uintptr_t)termTree);

        if (!termValue)
        {
            // fprintf(stderr, "Error: Null value encountered in UnaryNegOp\n");
            return nullptr;
        }

        // Return the negated value (zero - termValue)
        llvm::Value *zero = llvm::ConstantInt::get(TheContext, llvm::APInt(32, 0));
        return TheBuilder.CreateSub(zero, termValue, "neg_result");

        // llvm::Value *rest = AddSimpleExpr((uintptr_t)leftSide);
        // llvm::Value *rightTerm = AddTerm((uintptr_t)term);
        // todo this will overflow
    }
    else
    {
        // printf("Here in simple\n");
        return AddTerm(exprTree);
        // return TheBuilder.CreateAdd(rest, rightTerm, "add_result");
    }
    // printf("here");
    return nullptr;
}

static llvm::Value *AddExpr(uintptr_t exprTree)
{
    tree expr = (tree)exprTree;
    // TODO emit simple expression with proper comparison
    // if (NodeOp(expr) == LTOp)
    // {
    // }
    // else if (NodeOp(expr) == LEOp)
    // {
    // }
    // else if (NodeOp(expr) == GTOp)
    // {
    // }
    // else if (NodeOp(expr) == GEOp)
    // {
    // }
    // else if (NodeOp(expr) == EQOp)
    // {
    // }
    // else if (NodeOp(expr) == NEOp)
    // {
    // }
    // else
    // {
    // printf("Here in expression\n");
    return AddSimpleExpr(exprTree);
    // }
}

static void AddStmt(tree theStmt)
{

    if (IsNull(theStmt) || NodeKind(theStmt) != EXPRNode)
    {
        return;
    }
    switch (NodeOp(theStmt))
    {
    case AssignOp:
    {
        // this variable should be available in the current context already
        // it was defined as a register already, when we processed declop.
        // even method params maybe you could assign to also, (probably).

        // left side of equals sign in an assignment op
        tree varTree = RightChild(LeftChild(theStmt));
        tree exprTree = RightChild(theStmt);
        // std::string varSub = STInvertedMap[];
    }
    case RoutineCallOp:
    {
        // printf("wrrrds\n");
        // AddRoutineCall(theStmt);
        llvm::Value *routineCall = AddRoutineCall(theStmt);
        break;
    }
    case IfElseOp:
    {
        break;
    }
    case ReturnOp:
    {
        // left of a return op is the subtree for expression.
        tree exprTree = LeftChild(theStmt);
        llvm::Value *expr = AddExpr((uintptr_t)exprTree);
        TheBuilder.CreateRet(expr);
        break;
    }
    case LoopOp:
    {
        break;
    }
    default:
    {
        AddStmt(LeftChild(theStmt));
        AddStmt(RightChild(theStmt));
    }
    }
}

static llvm::Value *AddVarInit(uintptr_t tnode)
{
    tree treenode = (tree)tnode;
    // if (IsNull(treenode))
    // {
    //     return;
    // }

    // if (NodeOp(treenode) == ArrayTypeOp)
    // {
    //     // return AddArrInit((uintptr_t)treenode);
    // }
    // else
    // {
    // }
    return AddExpr((uintptr_t)treenode);
    // AddVarInit(RightChild(treenode));
    // AddVarInit(LeftChild(treenode));
}

static void AddArrInit(uintptr_t tnode)
{
    tree treenode = (tree)tnode;
    if (IsNull(treenode))
    {
        return;
    }
    tree type = RightChild(treenode);
    // create empty array in LLVM
    // possibly 2d

    if (NodeKind(type) == INTEGERTNode)
    {
        // this one {}
        // AddArrInit(LeftChild(treenode));
    }
    else
    {
        // right could be expression or varinit
        AddExpr((uintptr_t)treenode);
        // this one int[69420]
    }
    AddArrInit((uintptr_t)LeftChild(treenode));
    AddArrInit((uintptr_t)RightChild(treenode));
}

static llvm::Value *AddDeclVar(tree treenode)
{
    tree varDeclId = LeftChild(treenode);
    std::string variableName = STInvertedMap[IntVal(varDeclId)];
    tree type = LeftChild(RightChild(treenode));
    tree varRightSide = LeftChild(RightChild(treenode));

    // store if initialized
    // allocate if the variable is associated with a method decl.
    // if its a class do what?

    llvm::ConstantInt *varInit = llvm::ConstantInt::get(TheContext, llvm::APInt(32, 4));
    // is this variable is associated with a value, do not create a nullptr as the value for the initial allocation.
    llvm::Value *iVar = TheBuilder.CreateAlloca(llvm::Type::getInt32Ty(TheContext), nullptr, variableName);
    llvm::Value *varLoad = TheBuilder.CreateLoad(varInit, iVar, "y");
    printf("%d name: %s\n", IntVal(varDeclId), variableName.c_str());
    SetAttr(IntVal(varDeclId), OBJECT_ATTR, (uintptr_t)iVar);
    return varLoad;
}

static void AddDeclLine(tree treenode)
{
    if (IsNull(treenode))
    {
        return;
    }
    tree currVar = RightChild(treenode);
    AddDeclVar(currVar);
    AddDeclLine(LeftChild(treenode));
}

static void AddDeclList(tree treenode)
{
    if (IsNull(treenode))
    {
        return;
    }
    tree currVar = RightChild(treenode);
    AddDeclLine(currVar);
    AddDeclLine(LeftChild(treenode));
}

static void AddMethodArgs(tree treenode)
{
    if (IsNull(treenode))
    {
        return;
    }

    int opType = NodeOp(treenode);
    if (opType == RArgTypeOp || VArgTypeOp)
    {
        // todo, add to function stack
        tree opData = LeftChild(treenode);
        tree argId = LeftChild(opData);
        tree integerT = RightChild(opData);
        tree nextArg = RightChild(treenode);

        AddMethodArgs(nextArg);
        return;
    }
    // else if(opType == VArgTypeOp)
}
static void AddMethod(tree treenode)
{
    // base case.
    if (IsNull(treenode) || NodeKind(treenode) != EXPRNode)
    {
        return;
    }

    // get the rVargTypeOp
    int opType = NodeOp(treenode);
    if (opType == MethodOp)
    {
        tree methodOp = treenode;
        std::map<std::string, tree> props = getMethodProperties(methodOp);

        // get the string from the number of this symbol in the symbol table
        // inverted mapping gets ST_INDEX -> symbol STRING
        auto idString = STInvertedMap[IntVal(props["method_id"])];
        llvm::Function *fnHeader = makeFuncHeader(idString, props["method_type"]);

        AddDeclList(props["method_decl_list"]);
        // add arglist to the context of this function definition.
        // make vars in the function we are currently in.
        AddMethodArgs(props["method_args"]);
        // inside of the method block.
        // there are a list of semicolon separated statements.
        // AddMethod(props["method_block"]);
        AddStmt(props["method_stmts"]);
        return;
    }
}

static void AddClass(tree treenode)
{
    if (IsNull(treenode) || NodeKind(treenode) != EXPRNode)
    {
        return;
    }

    // get the rVargTypeOp
    if (NodeOp(treenode) == DeclOp)
    {
        // AddDecl(treenode, false);
        return;
    }

    AddClass(RightChild(treenode));
    AddClass(LeftChild(treenode));
}

static void emitSymbol(int idx)
{
    // TODO: Emit symbol in index idx in symbol table.
    int kind = GetAttr(idx, KIND_ATTR);
    // // get name from the string table?
    // // this is not actually a string
    std::string name = getString(GetAttr(idx, NAME_ATTR));

    switch (kind)
    {
    case VAR:
    {
        // LookUp the name and add it to ST.
        // It should have already been added in class or function.
        // llvm::GlobalVariable *globalVar = new llvm::GlobalVariable(
        //     *TheModule,                         // Module
        //     varType,                            // Type
        //     false,                              // Is constant?
        //     llvm::GlobalValue::ExternalLinkage, // Linkage type
        //     initialValue,                       // Initial value
        //     name                                // Variable name
        // );
        break;
    }
    case PROGRAM:
    {
        break;
    }
    case CLASS:
    {

        if (name.compare("system") == 0)
        {
            SetAttr(idx, OBJECT_ATTR, (uintptr_t)NULL);
            break;
        }
        else
        {
            llvm::StructType *classType = llvm::StructType::create(TheContext, {llvm::Type::getInt32Ty(TheContext)}, name);
            currClass = classType;
            std::string globalName = name.append(".global");
            TheModule->getOrInsertGlobal(globalName, classType);
            // get the global var version of this type that I just made.
            llvm::GlobalVariable *classTypeVar = TheModule->getNamedGlobal(globalName);

            classTypeVar->setInitializer(llvm::ConstantStruct::get(classType, {llvm::ConstantInt::get(TheContext, llvm::APInt(32, -1))}));
            // get it cause he's the teacher and its his class so its also his node
            if (IsAttr(idx, INIT_ATTR) == 1)
            {
                tree ahnsNode = (tree)GetAttr(idx, INIT_ATTR);
                AddClass(ahnsNode);
            }

            llvm::Value *typeAsVal = classTypeVar;

            // TheModule->getOrInsertGlobal(name, classType);
            SetAttr(idx, OBJECT_ATTR, (uintptr_t)typeAsVal);
        }

        // std::vector<llvm::Type *> classFields;

        // if (!classFields.empty())
        // {
        //     classType->setBody(classFields);
        //     tree classNode = (tree)GetAttr(idx, INIT_ATTR);
        //     // SetAttr(idx, OBJECT_ATTR, (llvm::Value)(uintptr_t)classNode);
        //     // AddMethod(classNode);
        // }

        // // Store the class type in the symbol table or module
        // TheModule->getOrInsertGlobal(name, classType);
        // bb = initFuncBlock();
        // llvm::Constant *strConstant = TheBuilder.CreateGlobalString(newLine);
        break;
    }
    // if its func its always function definition.
    // not every function has a block necessarily.
    // they all probably have INIT_ATTR and TYPE_ATTR
    //
    case FUNC:
    {
        // Set insertion point
        // Create a function.
        // with a call inside of it.
        // printf("%s\n", name.c_str());
        // printf("%d\n", IsAttr(idx, TYPE_ATTR));
        if (IsAttr(idx, TYPE_ATTR) == 1)
        {
            // todo, do this after the first function.
            if (name.compare("main") == 0)
            {
                // main is in its own block?
                llvm::FunctionType *funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(TheContext), false);

                llvm::Function *func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, llvm::StringRef(name), TheModule.get());
                bb = initFuncBlock(func);
                setConstants();
                // TODO : MID 5AM type logic
                if (IsAttr(idx, INIT_ATTR) == 1)
                {
                    llvm::Value *funcVal = (llvm::Value *)func;
                    // TODO: i think this needs to be after.
                    SetAttr(idx, OBJECT_ATTR, (uintptr_t)funcVal);

                    tree methNode = (tree)GetAttr(idx, INIT_ATTR);

                    AddMethod(methNode);
                    // after doing this I want to set the LLLVM object.
                }
            }
            else if (IsAttr(idx, INIT_ATTR) == 1)
            {
                llvm::Function *func = makeFuncHeader(name, (tree)GetAttr(idx, TYPE_ATTR));
                bb = initFuncBlock(func);

                llvm::Value *funcVal = (llvm::Value *)func;
                // TODO: i think this needs to be after.
                SetAttr(idx, OBJECT_ATTR, (uintptr_t)funcVal);
                tree methNode = (tree)GetAttr(idx, INIT_ATTR);
                AddMethod(methNode);
                // after doing this I want to set the LLLVM object.
            }
        }
        // create printf call
        // else if (name.compare("println"))
        // {
        // }
        break;
    }
    default:
    {
        break;
    }
    }
}

static void initPrintF()
{
    funcPrintf = TheModule->getFunction("printf");
    if (!funcPrintf)
    {
        int printfIntex = STMap["println"];
        llvm::FunctionType *funcPrintfType = llvm::FunctionType::get(llvm::IntegerType::get(TheContext, 32), true);
        funcPrintf = llvm::Function::Create(funcPrintfType, llvm::Function::ExternalLinkage, "printf", TheModule.get());
        funcPrintf->setCallingConv(llvm::CallingConv::C);

        SetAttr(printfIntex, OBJECT_ATTR, (uintptr_t)funcPrintf);
    }
}

void codegen()
{
    // llvm::BasicBlock *program_block = llvm::BasicBlock::Create(TheContext, "entry");
    // TheBuilder.SetInsertPoint(program_block);
    makeStMap();
    initPrintF();
    // setConstants();
    for (int i = 1; i < GetSymbolTableSize(); i++)
    {
        // printf("%d \n", i);
        emitSymbol(i);
    }
}

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
            if (STMap.count(fullWord) == 0 && ConstMap.count(tempStr.str()) == 0 && strcmp(programStr, fullWord) != 0)
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
// Count parameters in the arg tree (RArgTypeOp/VArgTypeOp chain)
static int countArgs(tree argNode) {
    if (IsNull(argNode)) return 0;
    if (NodeOp(argNode) == RArgTypeOp || NodeOp(argNode) == VArgTypeOp) {
        return 1 + countArgs(RightChild(argNode));
    }
    return 0;
}

static llvm::Function *makeFuncHeader(std::string name, tree typeNode, tree argsNode = nullptr)
{
    tree typeId = LeftChild(typeNode);
    // no examples have this.
    tree subtype = RightChild(typeNode);

    // Determine return type
    int typeKind = NodeKind(typeId);
    llvm::Type *retType = llvm::Type::getInt32Ty(TheContext);

    // Count and build parameter types
    int nArgs = countArgs(argsNode);
    std::vector<llvm::Type*> paramTypes(nArgs, llvm::Type::getInt32Ty(TheContext));

    llvm::FunctionType *funcType;
    if (typeKind == STNode)
    {
        // Class return type — prepend class pointer param
        paramTypes.insert(paramTypes.begin(), currClass);
        funcType = llvm::FunctionType::get(retType, paramTypes, false);
    }
    else
    {
        funcType = llvm::FunctionType::get(retType, paramTypes, false);
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

    // Case 1: Check for method call with object (system.println / system.readln)
    if ((NodeOp(selectOp) == SelectOp && STInvertedMap[IntVal(objectVar)].compare("system") == 0))
    {
        // Determine which system method is being called
        std::string methodName = STInvertedMap[IntVal(LeftChild(LeftChild(selectOp)))];

        if (methodName == "readln")
        {
            // system.readln(x) → scanf("%d", &x)
            // Get the variable being read into
            tree argExpr = LeftChild(argumentTree);
            tree argId = LeftChild(argExpr);
            int stIdx = IntVal(argId);
            llvm::Value *varPtr = (llvm::Value *)GetAttr(stIdx, OBJECT_ATTR);

            // Create scanf format string and call
            static llvm::Function *funcScanf = nullptr;
            if (!funcScanf) {
                llvm::FunctionType *scanfType = llvm::FunctionType::get(
                    llvm::IntegerType::get(TheContext, 32),
                    {llvm::PointerType::getUnqual(TheContext)},
                    true);
                funcScanf = llvm::Function::Create(scanfType, llvm::Function::ExternalLinkage, "scanf", TheModule.get());
            }
            llvm::Value *scanfFmt = TheBuilder.CreateGlobalString("%d", "scanf.int");
            return TheBuilder.CreateCall(funcScanf, {scanfFmt, varPtr}, "readln");
        }

        // system.println — use printf
        arguments = AddCallArgs(argumentTree);

        // printf needs a format string as first arg
        llvm::Constant *formatStr = nullptr;
        createFormatStr(arguments, formatStr);
        arguments.insert(arguments.begin(), formatStr);

        return TheBuilder.CreateCall(funcPrintf, arguments, "print");
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

        llvm::Function *llvmFn = llvm::dyn_cast<llvm::Function>(llvmFunc);
        return TheBuilder.CreateCall(llvmFn, arguments, "ELLLO");
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
        llvm::Function *llvmFn2 = llvm::dyn_cast<llvm::Function>(llvmFunc);
        llvm::CallInst *callInst = TheBuilder.CreateCall(llvmFn2, arguments, "ELLLO");
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
        formatStr = TheBuilder.CreateGlobalString("%d\n", "printf.num");
    }
    else if (arguments[0]->getType()->isPointerTy())
    {
        // If it's a string (pointer), use "%s" format specifier
        formatStr = TheBuilder.CreateGlobalString("%s\n", "printf.str");
    }
    else
    {
        // Default fallback
        formatStr = TheBuilder.CreateGlobalString("\n", "printf.newline");
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
                    // Load the value from the local variable
                    return TheBuilder.CreateLoad(alloca->getAllocatedType(), alloca, factorStr);
                }
                else
                {

                    // llvm::Value *classUsage = TheBuilder.CreateStructGEP(TheModule->getNamedGlobal("Variables.global"), 0, "point.x");
                    // also
                    // With opaque pointers, we need to figure out the pointee type.
                    // For global variables, use getValueType(); otherwise default to i32.
                    llvm::Type *loadTy = llvm::Type::getInt32Ty(TheContext);
                    if (auto *GV = llvm::dyn_cast<llvm::GlobalVariable>(opInit))
                        loadTy = GV->getValueType();
                    llvm::Value *classLoad = TheBuilder.CreateLoad(loadTy, opInit, "point.x.val");
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
    if (opType == MultOp || opType == DivOp || opType == AndOp)
    {
        tree leftTree = LeftChild(term);
        tree rightTree = RightChild(term);
        if (IsNull(leftTree) || IsNull(rightTree)) return nullptr;
        llvm::Value *leftValue = AddTerm((uintptr_t)leftTree);
        llvm::Value *rightValue = AddFactor((uintptr_t)rightTree);
        if (!leftValue || !rightValue) return nullptr;

        if (opType == MultOp)
            return TheBuilder.CreateMul(leftValue, rightValue, "mul_result");
        else if (opType == DivOp)
            return TheBuilder.CreateSDiv(leftValue, rightValue, "div_result");
        else // AndOp
            return TheBuilder.CreateAnd(leftValue, rightValue, "and_result");
    }
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
    int op = NodeOp(expr);

    // Comparison operators
    if (op == LTOp || op == LEOp || op == GTOp || op == GEOp || op == EQOp || op == NEOp)
    {
        llvm::Value *left = AddSimpleExpr((uintptr_t)LeftChild(expr));
        llvm::Value *right = AddSimpleExpr((uintptr_t)RightChild(expr));

        llvm::Value *cmp = nullptr;
        if (op == LTOp)
            cmp = TheBuilder.CreateICmpSLT(left, right, "lt");
        else if (op == LEOp)
            cmp = TheBuilder.CreateICmpSLE(left, right, "le");
        else if (op == GTOp)
            cmp = TheBuilder.CreateICmpSGT(left, right, "gt");
        else if (op == GEOp)
            cmp = TheBuilder.CreateICmpSGE(left, right, "ge");
        else if (op == EQOp)
            cmp = TheBuilder.CreateICmpEQ(left, right, "eq");
        else if (op == NEOp)
            cmp = TheBuilder.CreateICmpNE(left, right, "ne");

        // Extend i1 to i32 for use in expressions
        return TheBuilder.CreateZExt(cmp, llvm::Type::getInt32Ty(TheContext), "cmp_ext");
    }

    return AddSimpleExpr(exprTree);
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
        // AssignmentStatement grammar: AssignOp(AssignOp(Null, Variable), Expression)
        // So left child is AssignOp wrapping the variable, right child is the expression
        tree leftAssign = LeftChild(theStmt);
        tree exprTree = RightChild(theStmt);
        
        // If left child is another AssignOp, the variable is its right child
        tree varNode;
        if (!IsNull(leftAssign) && NodeKind(leftAssign) == EXPRNode && NodeOp(leftAssign) == AssignOp)
            varNode = RightChild(leftAssign);
        else
            varNode = leftAssign;

        // Navigate to the STNode id
        tree varId;
        if (!IsNull(varNode) && NodeKind(varNode) == EXPRNode && NodeOp(varNode) == VarOp)
            varId = LeftChild(varNode);
        else
            varId = varNode;

        if (!IsNull(varId) && !IsNull(exprTree))
        {
            int stIdx = IntVal(varId);
            llvm::Value *varPtr = (llvm::Value *)GetAttr(stIdx, OBJECT_ATTR);
            llvm::Value *val = AddExpr((uintptr_t)exprTree);

            if (varPtr && val)
            {
                TheBuilder.CreateStore(val, varPtr);
            }
        }
        break;
    }
    case RoutineCallOp:
    {
        llvm::Value *routineCall = AddRoutineCall(theStmt);
        break;
    }
    case IfElseOp:
    {
        llvm::Function *func = TheBuilder.GetInsertBlock()->getParent();
        llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(TheContext, "ifcont", func);

        // Emit an if/else-if/else chain.
        // elseBody is the trailing else statements (passed down from parent root node).
        std::function<void(tree, llvm::BasicBlock*, tree)> emitIfChain =
            [&](tree node, llvm::BasicBlock *afterBB, tree elseBody) {

            if (IsNull(node) || NodeKind(node) != EXPRNode)
                return;

            tree rightChild = RightChild(node);
            tree leftChild = LeftChild(node);

            bool isCondNode = !IsNull(rightChild) && NodeKind(rightChild) == EXPRNode
                              && NodeOp(rightChild) == CommaOp;

            if (isCondNode)
            {
                // This node has condition on the right: CommaOp(condition, then_stmts)
                // But left child may contain earlier conditions that should be checked first
                
                if (!IsNull(leftChild) && NodeKind(leftChild) == EXPRNode && NodeOp(leftChild) == IfElseOp)
                {
                    // Left has earlier conditions — process them first
                    // This node's condition becomes an else-if after the left chain
                    tree condition = LeftChild(rightChild);
                    tree thenStmts = RightChild(rightChild);
                    
                    // Process the left chain; its then-blocks merge to afterBB
                    // but its else fallthrough should come to this else-if condition
                    // So we create the elseif block and pass it as the else destination
                    llvm::BasicBlock *thisElseIfBB = llvm::BasicBlock::Create(TheContext, "elseif", func);
                    
                    // For the left chain: set elseBody to a marker that causes
                    // the else to branch to thisElseIfBB. We achieve this by
                    // processing the chain, then replacing the else terminator.
                    
                    // Process left chain normally
                    emitIfChain(leftChild, afterBB, NullExp());
                    // The insert point is at the innermost else block
                    // Replace its terminator (which goes to afterBB) with a branch to thisElseIfBB
                    llvm::BasicBlock *curBB = TheBuilder.GetInsertBlock();
                    if (curBB->getTerminator())
                        curBB->getTerminator()->eraseFromParent();
                    TheBuilder.CreateBr(thisElseIfBB);
                    
                    // Now emit this else-if condition
                    TheBuilder.SetInsertPoint(thisElseIfBB);
                    llvm::Value *condVal = AddExpr((uintptr_t)condition);
                    llvm::Value *condBool = TheBuilder.CreateICmpNE(
                        condVal, llvm::ConstantInt::get(TheContext, llvm::APInt(32, 0)), "ifcond");
                    
                    llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(TheContext, "then", func);
                    llvm::BasicBlock *elseBB = llvm::BasicBlock::Create(TheContext, "else", func);
                    
                    TheBuilder.CreateCondBr(condBool, thenBB, elseBB);
                    
                    TheBuilder.SetInsertPoint(thenBB);
                    AddStmt(thenStmts);
                    if (!TheBuilder.GetInsertBlock()->getTerminator())
                        TheBuilder.CreateBr(afterBB);
                    
                    TheBuilder.SetInsertPoint(elseBB);
                    if (!IsNull(elseBody))
                    {
                        AddStmt(elseBody);
                        if (!TheBuilder.GetInsertBlock()->getTerminator())
                            TheBuilder.CreateBr(afterBB);
                    }
                    else
                    {
                        if (!TheBuilder.GetInsertBlock()->getTerminator())
                            TheBuilder.CreateBr(afterBB);
                    }
                }
                else
                {
                    // No left chain — this is the first (or only) if
                    tree condition = LeftChild(rightChild);
                    tree thenStmts = RightChild(rightChild);

                    llvm::Value *condVal = AddExpr((uintptr_t)condition);
                    llvm::Value *condBool = TheBuilder.CreateICmpNE(
                        condVal, llvm::ConstantInt::get(TheContext, llvm::APInt(32, 0)), "ifcond");

                    llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(TheContext, "then", func);
                    llvm::BasicBlock *elseBB = llvm::BasicBlock::Create(TheContext, "else", func);

                    TheBuilder.CreateCondBr(condBool, thenBB, elseBB);

                    TheBuilder.SetInsertPoint(thenBB);
                    AddStmt(thenStmts);
                    if (!TheBuilder.GetInsertBlock()->getTerminator())
                        TheBuilder.CreateBr(afterBB);

                    TheBuilder.SetInsertPoint(elseBB);
                    if (!IsNull(elseBody))
                    {
                        AddStmt(elseBody);
                        if (!TheBuilder.GetInsertBlock()->getTerminator())
                            TheBuilder.CreateBr(afterBB);
                    }
                    else
                    {
                        // No else body — branch to afterBB unless caller will redirect
                        if (!TheBuilder.GetInsertBlock()->getTerminator())
                            TheBuilder.CreateBr(afterBB);
                    }
                }
            }
            else
            {
                // Root node: left = if/else-if chain, right = trailing else stmts
                emitIfChain(leftChild, afterBB, rightChild);
            }
        };

        emitIfChain(theStmt, mergeBB, NullExp());

        TheBuilder.SetInsertPoint(mergeBB);
        bb = mergeBB;
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
        // while (condition) { body }
        // LoopOp: left = condition, right = body
        llvm::Function *func = TheBuilder.GetInsertBlock()->getParent();

        llvm::BasicBlock *condBB = llvm::BasicBlock::Create(TheContext, "loop.cond", func);
        llvm::BasicBlock *bodyBB = llvm::BasicBlock::Create(TheContext, "loop.body", func);
        llvm::BasicBlock *endBB  = llvm::BasicBlock::Create(TheContext, "loop.end", func);

        // Branch from current block to condition check
        TheBuilder.CreateBr(condBB);

        // Emit condition
        TheBuilder.SetInsertPoint(condBB);
        tree condExpr = LeftChild(theStmt);
        llvm::Value *cond = AddExpr((uintptr_t)condExpr);
        // If AddExpr returns an i32, convert to i1
        if (cond->getType()->isIntegerTy(32)) {
            cond = TheBuilder.CreateICmpNE(cond, llvm::ConstantInt::get(TheContext, llvm::APInt(32, 0)), "tobool");
        }
        TheBuilder.CreateCondBr(cond, bodyBB, endBB);

        // Emit body
        TheBuilder.SetInsertPoint(bodyBB);
        AddStmt(RightChild(theStmt));
        // After body, branch back to condition (only if no terminator yet)
        if (!TheBuilder.GetInsertBlock()->getTerminator()) {
            TheBuilder.CreateBr(condBB);
        }

        // Continue after the loop
        TheBuilder.SetInsertPoint(endBB);
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
    int stIdx = IntVal(varDeclId);
    std::string variableName;
    // STNode uses symbol table index, IDNode uses string table index
    if (NodeKind(varDeclId) == STNode && STInvertedMap.count(stIdx))
        variableName = STInvertedMap[stIdx];
    else
        variableName = getString(GetAttr(stIdx, NAME_ATTR));
    tree type = LeftChild(RightChild(treenode));
    tree varRightSide = LeftChild(RightChild(treenode));

    // store if initialized
    // allocate if the variable is associated with a method decl.
    // if its a class do what?

    llvm::ConstantInt *varInit = llvm::ConstantInt::get(TheContext, llvm::APInt(32, 4));
    // is this variable is associated with a value, do not create a nullptr as the value for the initial allocation.
    llvm::Value *iVar = TheBuilder.CreateAlloca(llvm::Type::getInt32Ty(TheContext), nullptr, variableName);
    llvm::Value *varLoad = TheBuilder.CreateLoad(llvm::Type::getInt32Ty(TheContext), iVar, "y");
    // fprintf(stderr, "DEBUG AddDeclVar: stIdx=%d name='%s' nodeKind=%d\n", stIdx, variableName.c_str(), NodeKind(varDeclId));
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

    // Recursively walk the tree looking for DeclOp nodes
    if (NodeKind(treenode) == EXPRNode && NodeOp(treenode) == DeclOp)
    {
        AddDeclLine(treenode);
    }
    else
    {
        // BodyOp or other container — recurse both children
        AddDeclList(LeftChild(treenode));
        AddDeclList(RightChild(treenode));
    }
}

static void AddMethodArgs(tree treenode)
{
    if (IsNull(treenode))
    {
        return;
    }

    int opType = NodeOp(treenode);
    if (opType == RArgTypeOp || opType == VArgTypeOp)
    {
        tree opData = LeftChild(treenode);
        tree argId = LeftChild(opData);
        tree integerT = RightChild(opData);
        tree nextArg = RightChild(treenode);

        // Get the ST index for this arg and create an alloca for it
        int stIdx = IntVal(argId);
        std::string argName = STInvertedMap[stIdx];
        llvm::AllocaInst *alloca = TheBuilder.CreateAlloca(
            llvm::Type::getInt32Ty(TheContext), nullptr, argName);
        SetAttr(stIdx, OBJECT_ATTR, (uintptr_t)alloca);

        // Recurse first so args are numbered correctly (left to right)
        AddMethodArgs(nextArg);
        return;
    }
}

// Store the function's actual argument values into the allocas created by AddMethodArgs
static void StoreMethodArgs(tree treenode, llvm::Function *func, unsigned argIdx = 0)
{
    if (IsNull(treenode)) return;

    int opType = NodeOp(treenode);
    if (opType == RArgTypeOp || opType == VArgTypeOp)
    {
        tree opData = LeftChild(treenode);
        tree argId = LeftChild(opData);
        int stIdx = IntVal(argId);

        llvm::Argument *llvmArg = func->getArg(argIdx);
        llvm::Value *alloca = (llvm::Value *)GetAttr(stIdx, OBJECT_ATTR);
        TheBuilder.CreateStore(llvmArg, alloca);

        StoreMethodArgs(RightChild(treenode), func, argIdx + 1);
    }
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
        llvm::Function *fnHeader = makeFuncHeader(idString, props["method_type"], props["method_args"]);
        bb = initFuncBlock(fnHeader);

        AddDeclList(props["method_decl_list"]);
        AddMethodArgs(props["method_args"]);
        StoreMethodArgs(props["method_args"], fnHeader);
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

            classTypeVar->setInitializer(llvm::ConstantStruct::get(classType, {llvm::ConstantInt::get(TheContext, llvm::APInt(32, -1, true))}));
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
                if (IsAttr(idx, INIT_ATTR) == 1)
                {
                    tree methNode = (tree)GetAttr(idx, INIT_ATTR);
                    llvm::FunctionType *funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(TheContext), false);
                    llvm::Function *func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, llvm::StringRef(name), TheModule.get());
                    bb = initFuncBlock(func);
                    setConstants();

                    llvm::Value *funcVal = (llvm::Value *)func;
                    SetAttr(idx, OBJECT_ATTR, (uintptr_t)funcVal);

                    if (!IsNull(methNode) && NodeOp(methNode) == MethodOp) {
                        std::map<std::string, tree> props = getMethodProperties(methNode);
                        AddDeclList(props["method_decl_list"]);
                        AddMethodArgs(props["method_args"]);
                        AddStmt(props["method_stmts"]);
                    }
                }
            }
            else if (IsAttr(idx, INIT_ATTR) == 1)
            {
                tree methNode = (tree)GetAttr(idx, INIT_ATTR);
                if (!IsNull(methNode) && NodeOp(methNode) == MethodOp) {
                    std::map<std::string, tree> props = getMethodProperties(methNode);

                    llvm::Function *func = makeFuncHeader(name, props["method_type"], props["method_args"]);
                    bb = initFuncBlock(func);
                    llvm::Value *funcVal = (llvm::Value *)func;
                    SetAttr(idx, OBJECT_ATTR, (uintptr_t)funcVal);

                    AddDeclList(props["method_decl_list"]);
                    AddMethodArgs(props["method_args"]);
                    StoreMethodArgs(props["method_args"], func);
                    AddStmt(props["method_stmts"]);
                }
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
        llvm::FunctionType *funcPrintfType = llvm::FunctionType::get(
            llvm::IntegerType::get(TheContext, 32),
            {llvm::PointerType::getUnqual(TheContext)},
            true);
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
        emitSymbol(i);
    }
}

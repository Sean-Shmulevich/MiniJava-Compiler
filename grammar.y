%{
/** Code by @author Wonsun Ahn, Fall 2024
 * 
 * The parser implementation file.
 */

#include <stdlib.h>
#include <stdio.h>
#include "proj2.h"

extern int yylex();
static void yyerror(char *str);

tree SyntaxTree;
tree methodDeclType;
tree declType;

%}

%union {
  int intg;
  tree tptr;
}

/* Terminal (token) declarations */
%token <intg>  IDnum DECLARATIONSnum ENDDECLARATIONSnum
%token <intg>  EQUALnum VOIDnum
%token <intg>  INTnum METHODnum VALnum DOTnum ASSGNnum RETURNnum IFnum ELSEnum WHILEnum
%token <intg>  LTnum LEnum EQnum NEnum GEnum GTnum PLUSnum MINUSnum ORnum TIMESnum DIVIDEnum ANDnum
%token <intg>  NOTnum ICONSTnum SCONSTnum



/* Non-terminal (tree node) declarations */
/* %type  <tptr>  Program */
%type <tptr> Expression SimpleExpression Term Factor UnsignedConstant Variable ExprListBrac ExprList SignedTerm TypeKind
%type <tptr> VariableInitializer EmptyBracList VariableDeclId VarInitList ArrInitList ArrayInitializer ArrayCreationExpression
/* %type <tptr> VarList VarEq */

/* %type <type> Type    // Declares Type rule returns a type value */
%type <tptr> FieldDecl VarList VarEq Type FieldDeclList Decls StatementList StatementListHelper TypeIndexed
%type <tptr> AssignmentStatement MethodCallStatement ReturnStatement IfStatement WhileStatement Statement ElseIfList Class_op
%type <tptr> ClassDecl ClassDeclList Program ClassBody MethodDecl MethodDeclTypeHelper FormalParameterList Block MethodDeclList IdList FormalParameterListTypeHelper
%left <intg> LPARENnum CLASSnum LBRACEnum LBRACnum COMMAnum PROGRAMnum 
%right <intg> SEMInum RPARENnum RBRACEnum RBRACnum 
/* %type <tptr> Program  */
/* %type <tptr> ClassDecl  */
/* StatementList StatementListHelper ClassBody MethodDecl Class_op MethodDeclList MethodDeclTypeHelper FormalParameterList Block  */
%start Program

/* TODO: Add more non-terminal definitons as needed. */

%%

/* TODO: Fix this production rule so that it matches language specification. */
// { $$ = MakeTree(ProgramOp, NullExp(), MakeLeaf(IDNode, $2)); SyntaxTree = $$; }

Program
  : PROGRAMnum IDnum { $<tptr>-1 = MakeLeaf(IDNode, $2); } SEMInum ClassDeclList {$$ = MakeTree(ProgramOp, $5, $<tptr>-1); SyntaxTree = $$; };
  /* : PROGRAMnum IDnum SEMInum {$$ = MakeTree(ProgramOp, NullExp(), MakeLeaf(IDNode, $1)); SyntaxTree = $$; }; */

ClassDecl
  : CLASSnum IDnum { $<tptr>-2 = MakeLeaf(IDNode, $2); } ClassBody {$$ = MakeTree(ClassDefOp, $4, $<tptr>-2);};

ClassDeclList
  : ClassDecl {$$ = MakeTree(ClassOp, NullExp(), $1);};
  | ClassDeclList ClassDecl {$$ = MakeTree(ClassOp, $1, $2);};

ClassBody
  : LBRACEnum Class_op RBRACEnum {$$ = $2;}
  | LBRACEnum RBRACEnum {$$ = NullExp();};

Class_op
  : Decls MethodDeclList {
      // both of the right subtrees are meant for 
      $$ = MkLeftC($1, $2); // make decls, the end of the methodDeclList.
  }
  | MethodDeclList {
      // decls is at the end, add body op to end of other body op's, decls is the last one.
      $$ = $1;
  }
  | Decls {
      $$ = $1;
  };

MethodDecl
  : METHODnum MethodDeclTypeHelper {methodDeclType = $2;} IDnum {$<tptr>-3 = MakeLeaf(IDNode, $4);} LPARENnum FormalParameterList RPARENnum Block {
    $$ = MakeTree(MethodOp, MakeTree(HeadOp, $<tptr>-3, $7), $9);
  }
  /* : METHODnum MethodDeclTypeHelper {methodDeclType = $2;} IDnum LPARENnum RPARENnum Block {
      tree emptyParamList = MakeTree(SpecOp, NullExp(), methodDeclType);
      $$ = MakeTree(MethodOp, MakeTree(HeadOp, MakeLeaf(IDNode, $4), emptyParamList), $7);
    } // subtree for param, may not be Null, have to check */

MethodDeclTypeHelper
  : Type {$$ = $1;}
  | VOIDnum {$$ = NullExp();};

// I messed up this rule.
MethodDeclList
  : MethodDecl { $$ = MakeTree(BodyOp, NullExp(), $1); }
  | MethodDeclList MethodDecl { $$ = MakeTree(BodyOp, $1, $2); };

// todo, type needs to be passed down from method decl.
FormalParameterList
  : FormalParameterListTypeHelper IdList {$$ = MakeTree(SpecOp, $2, methodDeclType);}
  | FormalParameterList SEMInum FormalParameterListTypeHelper IdList { MkRightC($4, LeftChild($1)); $$ = $1;}
  | {$$ = MakeTree(SpecOp, NullExp(), methodDeclType);};
/* FormalParameterList
  : VALnum INTnum {$<tptr>$ = MakeTree(VArgTypeOp, MakeTree(CommaOp, NullExp(), MakeLeaf(INTEGERTNode, $2)), NullExp()); } IDnum {
      MakeTree(SpecOp, $4, methodDeclType);
    }
  | FormalParameterListTypeHelper IdList SEMInum FormalParameterList { MkRightC($4, LeftChild($1)); $$ = $1;}; */

FormalParameterListTypeHelper
  : VALnum INTnum {$$ = MakeTree(VArgTypeOp, MakeTree(CommaOp, NullExp(), MakeLeaf(INTEGERTNode, $2)), NullExp()); $<tptr>$ = $$;}
  | INTnum {$$ = MakeTree(RArgTypeOp, MakeTree(CommaOp, NullExp(), MakeLeaf(INTEGERTNode, $1)), NullExp()); $<tptr>$ = $$;};


// this node may be hardcoded
// it for sure is because you can only declare int values in this case
// no way this works.
IdList
  : IDnum {
      tree nameAndType = LeftChild($<tptr>0);
      MkLeftC(MakeLeaf(IDNode, $1), nameAndType);
      $$ = $<tptr>0;
    }
  | IdList COMMAnum IDnum {
      tree prevType = RightChild(LeftChild($1));
      int opType = NodeOp($1);
      tree nextId = MakeTree(CommaOp, MakeLeaf(IDNode, $3), prevType);
      // $$ = MakeTree(opType, nextId, $1); // this will go the opposite way.
      tree nextOp = MakeTree(opType, nextId, NullExp());
      $$ = MkRightC(nextOp, $1);
  }

Block
  : Decls { $$ = MakeTree(BodyOp, $1, NullExp()); }
  | Decls StatementList { $$ = MakeTree(BodyOp, $1, $2); }
  | StatementList { $$ = MakeTree(BodyOp, NullExp(), $1); };

// I could have been doing this the whole time
StatementList
  : LBRACEnum StatementListHelper RBRACEnum { $$ = $2; };
  | LBRACEnum RBRACEnum { $$ = MakeTree(StmtOp, NullExp(), NullExp());};

StatementListHelper
  : Statement SEMInum { $$ = MakeTree(StmtOp, NullExp(), $1); }
  // NOTE, todo: switching the order of these may have been wrong.
  | StatementListHelper Statement SEMInum {$$ = MakeTree(StmtOp, $1, $2) ;};

Statement
  : AssignmentStatement {$$ = $1;}
  | ReturnStatement {$$ = $1;}
  | IfStatement {$$ = $1;};
  | WhileStatement {$$ = $1;};
  | MethodCallStatement {$$ = $1;}

AssignmentStatement
  : Variable ASSGNnum Expression {$$ = MakeTree(AssignOp, MakeTree(AssignOp, NullExp(), $1), $3);};

/* ExprListParen
  : LPARENnum ExprList RPARENnum {$$ = MakeTree(CommaOp, $2, NullExp());}; 
*/

// Todo, this solution is a hardcoded HACK for the 6th example file
// its still quite wrong
// BAD
MethodCallStatement
  : Variable LPARENnum RPARENnum {$$ = MakeTree(RoutineCallOp, $1, NullExp()); }
  /* | Variable LPARENnum Expression COMMAnum Expression COMMAnum Expression RPARENnum {
      tree startList = MakeTree(CommaOp, $7, NullExp());
      tree startList1 = MakeTree(CommaOp, $5, startList);
      tree startList2 = MakeTree(CommaOp, $3, startList1);
      // tree startList3 = MakeTree(CommaOp, $5, startList2);
      $$ = MakeTree(RoutineCallOp, $1, startList2); 
    } */
  | Variable LPARENnum ExprList RPARENnum {
    $$ = MakeTree(RoutineCallOp, $1, $3);
  }

ReturnStatement
  : RETURNnum {$$ = MakeTree(ReturnOp, NullExp(), NullExp());}
  | RETURNnum Expression {$$ = MakeTree(ReturnOp, $2, NullExp());};

WhileStatement
  : WHILEnum Expression StatementList {$$ = MakeTree(LoopOp, $2, $3);};
// if else 
// right of root is statement list for trailing else
// left of root is if else op
IfStatement
  : IFnum Expression StatementList {
      tree rightSub = MakeTree(CommaOp, $2, $3);
      tree fullIfTree = MakeTree(IfElseOp, NullExp(), rightSub);
      // $$ = MkLeftC(fullIfTree, $3);
      // $$ = MakeTree(IfElseOp, fullIfTree, $3);
      $$ = fullIfTree;
    }
  // if else
  | IFnum Expression StatementList ELSEnum StatementList {
      tree rightSub = MakeTree(CommaOp, $2, $3);
      tree subIf = MakeTree(IfElseOp, NullExp(), rightSub);
      tree rootIf = MakeTree(IfElseOp, subIf, $5);

      $$ = rootIf;
  }
  // if + else if
  | IFnum Expression StatementList ElseIfList {
      // this one is root
      // issue right now, the first if else should 
      // the subtree for elseif, needs to be built the other way
      tree ifSub = MakeTree(CommaOp, $2, $3);
      tree elseIfList = $4;
      tree ifTree = MakeTree(IfElseOp, elseIfList, ifSub);

      $$ = ifTree;
  }
  // if + else if + else
  // for this case I need to make more nodes. like one level higher.
  | IFnum Expression StatementList ElseIfList ELSEnum StatementList {
    tree elseStatementList = $6;
    tree elseIfList = $4;
    // 
    tree rightSubIf = MakeTree(CommaOp, $2, $3);
    // link to the rest of the elseif statements.
    // flip this around somehow?
    // make root var, set new to end.
    tree mainIf = MakeTree(IfElseOp, NullExp(), rightSubIf);
    tree all = MkLeftC(mainIf, elseIfList);

    tree rootIf = MakeTree(IfElseOp, all, elseStatementList);

    // $$ = MkLeftC(newElseIf, $1);
    $$ = rootIf;
  };

ElseIfList
  : ELSEnum IFnum Expression StatementList {
      tree rightSub = MakeTree(CommaOp, $3, $4);
      $$ = MakeTree(IfElseOp, NullExp(), rightSub);
  }
  | ElseIfList ELSEnum IFnum Expression StatementList {
      // else if list is gonna be a if else op that contains the tree so far.
      // set the leftmost of the ElseIfList to be The new if else op.
      tree rightSub = MakeTree(CommaOp, $4, $5);
      // create new if else tree
      // 2 ways to go
      // make old ElseIfList (which is an IfElseOp) the root and set the new one to its leftmostchild.
      // make the new else if statement the root, set its left child to the old root (ElseIfList ($1)).
      tree newElseIf = MakeTree(IfElseOp, $1, rightSub);
      $$ = newElseIf;
      // $$ = MkLeftC(newElseIf, $1);
  };

// can have 0 field decl
Decls
  : DECLARATIONSnum FieldDeclList ENDDECLARATIONSnum { $$ = $2; }
  | DECLARATIONSnum ENDDECLARATIONSnum {$$ = MakeTree(BodyOp, NullExp(), NullExp()); };// VARIABLE

// Variable declaration structure.
// DeclOp left tree is Null unless there is a comma in the declaration.
// if there is then set the left of the existing declOp to the new one
// MakeTree(CurrentRoot, NullExp(), Variable? possibly wrap in comma)

// FieldDecl: Declaration of variable 
// Examples:
// int a = 6, b, c;
// int a = 6, b, c = 6;
// Grammar Desc
// Every FieldDecl has one Type
// Type is followed by at least one variable declaration (VariableDeclId)
// after a variable declaration (VariableDeclId)
//    you could have a comma and at least one more VariableDeclId
//    you could have a semicolon, end
//    you could have '=' with a variable assignment after it (VariableInitializer) after it
// after every VariableInitializer
//    you can have a comma and then another VariableDeclId
//    you can have a semicolon, end
// Node Linking Description
// this rule took 10 hours at least.
FieldDecl
  : Type { $<tptr>$ = $1; declType = $1; } VarList SEMInum { $$ = $3; };

// list helper for multiple decl's 
FieldDeclList
  : FieldDecl {$$ =  MakeTree(BodyOp, NullExp(), $1);}
  // the new body op goes left and inside
  // the next one goes inside the previous one on the left.
  | FieldDeclList FieldDecl { $$ = MkLeftC($1, MakeTree(BodyOp, NullExp(), $2)); };

// Variable declaration int x = 10, x, z=3; (any combination)

// csv variable declaration shape
// the right of this must point to array typeop?
// we might return array type op.
// did we have some arr indexing in the type? in VariableDeclId
VarEq
  : VariableDeclId EQUALnum VariableInitializer {
      tree entireVarTree = MakeTree(CommaOp, $1, MakeTree(CommaOp, NullExp(), $3));
      // tree entireVarTree = MakeTree(CommaOp, $1, MakeTree(CommaOp, NullExp(), $3));
      $$ = entireVarTree;
  }
  | VariableDeclId {
      tree varRightSub = MakeTree(CommaOp, NullExp(), NullExp());
      // tree varRightSub = MakeTree(CommaOp, currentType, NullExp());
      tree entireVarTree = MakeTree(CommaOp, $1, varRightSub);
      $$ = entireVarTree;
  };

// this rule MUST be right recursive, in order to keep the saved type value.
// VarList Regex: (VarEq,VarEq)*(VarEq,VarEq) (concat at least one.)
VarList
  : VarEq {
      tree varRightSub = RightChild($1);
      tree varWithType = MkLeftC($<tptr>0, varRightSub);
      $$ = MakeTree(DeclOp, NullExp(), $1);
    }
  | VarList COMMAnum VarEq {
      tree varRightSub = RightChild($3);
      tree varWithType = MkLeftC($<tptr>0, varRightSub);
      $$ = MakeTree(DeclOp, $1, $3);
  };

// after idnum and intnum is always [] or . or end
// after a dot you NEED to have at least one more identifier
// after [] you can only have a dot or a finish
// this is a good rule, well written ❤️ but then I had to change it :( 
// good thought process
  /* : TypeKind {
      tree typeTree = MakeTree(TypeIdOp, $1 , NullExp());
      $$ = typeTree;
    }Type
 */
Type
  : TypeIndexed { $$ = $1;}
  | Type LBRACnum RBRACnum {

    // tree indexing = MakeTree(IndexOp, NullExp(), NullExp());
    // tree indexing = MakeTree(IndexOp, NullExp(), NullExp());
    // $$ = MkRightC(indexing, $1);
    $$ = MkRightC(MakeTree(IndexOp, NullExp(), NullExp()), $1);
  };

TypeKind
  : INTnum {$$ = MakeLeaf(INTEGERTNode, $1);}
  | IDnum {$$ = MakeLeaf(IDNode, $1);};

TypeIndexed
  : TypeKind { $$ = MakeTree(TypeIdOp, $1, NullExp()); }
  | TypeKind DOTnum TypeIndexed {
    tree field = MakeTree(FieldOp, $1, NullExp());
    $$ = MkRightC(field, $3);
  };

// name of variable that comes after type
// todo, emptybrac list might need to return a tree
// how would you know if the amount of brakets corrisponds?
// what does it even mean to have brackets in your variable name declaration
// You should at least have them be a part of the string
// but the functionality seems weird in general?
// empty indexing a variable that you just declared?
VariableDeclId
  : IDnum {$$ = MakeLeaf(IDNode, $1);}
  | IDnum EmptyBracList {$$ = MakeLeaf(IDNode, $1);};

// list of empty brackets
EmptyBracList
  : LBRACnum RBRACnum {}
  | LBRACnum RBRACnum EmptyBracList {};

// // Right side of equal sign
// set the root syntax tree node to be variable init.
VariableInitializer
  : Expression {$$ = $1;}
  | ArrayInitializer {$$ = $1;}
  | ArrayCreationExpression {$$ = $1;};

// ArrayTypeOp has typeIdOp as its right tree
// ArrayTypeOp has bound op or comma op as its right tree
// the right should be num/expr, the left should be a comma node

// Regex: INT([Expression])^+ (at least one then kleene)
ArrayCreationExpression
  : INTnum ArrInitList {$$ = $2;};

/* int[] ia[]={3, 5, 7}; */

//bound op is root and also left subtree, expression is right subtree.
//create new bound op with null Left and Expression right,
//make it the leftmost of the existing ArrInitList tree (which is of type boundOp) 
//there should be a null node in the left where to place it.
/*List helper for concat Array greation expression.*/
ArrInitList 
  : LBRACnum Expression RBRACnum {$$ = MakeTree(ArrayTypeOp, MakeTree(BoundOp, NullExp(), $2), MakeLeaf(INTEGERTNode, INTnum));}
  | LBRACnum Expression RBRACnum ArrInitList {
      tree leftSub = MakeTree(BoundOp, NullExp(), $2);
      // tree root = MakeTree(ArrayTypeOp, NullExp(), $2);
      $$ = MkLeftC(leftSub, $4);
    };

// Csv variableInitializer's 
ArrayInitializer
  : LBRACEnum VarInitList RBRACEnum {$$ = MakeTree(ArrayTypeOp, $2, declType);};

// left node is always comma OP
// varinitlist is always comma op unless its varinitializer
// variable initializers separated by comma's
VarInitList
  : VariableInitializer {$$ = MakeTree(CommaOp, NullExp(), $1);}
  | VarInitList COMMAnum VariableInitializer {$$ = MakeTree(CommaOp, $1, $3);};



// Simple Expression with comparison op, Simple Expression
Expression
  : SimpleExpression {$$ = $1; }
  // GtoP should be linking to NUMNode
  // simple expression should not return a tree, just a Node, or maybe a tree, depending on the expression.
  /* | SimpleExpression Comp_op SimpleExpression { tree opNode = MakeTree($2, $1, $3); $$ = opNode; SyntaxTree = $$; }; */
  | SimpleExpression LTnum SimpleExpression { $$ = MakeTree(LTOp, $1, $3); };
  | SimpleExpression LEnum SimpleExpression { $$ = MakeTree(LEOp, $1, $3); };
  | SimpleExpression GTnum SimpleExpression { $$ = MakeTree(GTOp, $1, $3); };
  | SimpleExpression GEnum SimpleExpression { $$ = MakeTree(GEOp, $1, $3); };
  | SimpleExpression EQnum SimpleExpression { $$ = MakeTree(EQOp, $1, $3); };
  | SimpleExpression NEnum SimpleExpression { $$ = MakeTree(NEOp, $1, $3); };

SimpleExpression
  : SignedTerm { $$ = $1; } // signed term should not return a tree
  | SimpleExpression ORnum Term { $$ = MakeTree(OrOp, $1, $3); }
  | SimpleExpression PLUSnum Term { $$ = MakeTree(AddOp, $1, $3); }
  | SimpleExpression MINUSnum Term { $$ = MakeTree(SubOp, $1, $3); }

SignedTerm
  : Term { $$ = $1; }
  | PLUSnum Term { $$ = MakeTree(AddOp, $2, NullExp()); }
  | MINUSnum Term { $$ = MakeTree(UnaryNegOp, $2, NullExp()); };

// do not link to factor actually, factor is represented by the things that makes it up
// its not a part of the actual tree. 
Term
  : Factor { $$ = $1; }
  | Factor TIMESnum Term { $$ = MakeTree(MultOp, $1, $3); }
  | Factor DIVIDEnum Term { $$ = MakeTree(DivOp, $1, $3); }
  | Factor ANDnum Term { $$ = MakeTree(AndOp, $1, $3); };


Factor
  : UnsignedConstant { $$ = $1; }
  | Variable { $$ = $1; }
  | MethodCallStatement { $$ = $1; }
  //todo, this might be wrong but for now I think its right
  | LPARENnum Expression RPARENnum { $$ = $2; }
  | NOTnum Factor { $$ = MakeTree(NotOp, $2, NullExp()); };

// todo: makeleaf might be wrong, make sure I am passing in the value
UnsignedConstant
  : ICONSTnum {$$ = MakeLeaf(NUMNode, $1);}
  | SCONSTnum {$$ = MakeLeaf(STRINGNode, $1);};

// probably a conflict here too
// id[3].a
// id.id.id
// id[7777777].id.id[8]
// id.id.id[8]
// id[][][].hello[].hi
// var always has id as leftTree
// right tree is either dummy or selectOp
Variable
  : IDnum { $$ = MakeTree(VarOp, MakeLeaf(IDNode, $1), NullExp());}
  /* | IDnum ExprListList { $$ = MakeTree(IndexOp, MakeLeaf(IDNode, $1), $2); } */
  | Variable ExprListBrac {
      tree select = MakeTree(SelectOp, $2, NullExp());
      $$ = MkRightC(select, $1);
    }
  /* | Variable DOTnum {$$ = MakeTree()} */
  | Variable DOTnum IDnum {
      // the first one is always var and stays var.
      // eveything after is select / field
      // just add a new select to the rightmost leaf.
      tree fieldOpNode = MakeTree(FieldOp, MakeLeaf(IDNode, $3), NullExp()); 
      tree select = MakeTree(SelectOp, fieldOpNode, NullExp());
      MkRightC(select, $1);
      $$ = $1;
    };


// [expr,...][expr,...][expr,...]
// todo: this is probably a bad name, I might need it somewhere else.
  //todo: I may need to return dummy for the $4, if we are a the end of the list.
ExprListBrac
  : LBRACnum Expression RBRACnum {$$ = MakeTree(IndexOp, $2, NullExp());}
  | LBRACnum Expression RBRACnum ExprListBrac { $$ = MakeTree(IndexOp, $2, $4); };
  // | LBRACnum ExprList RBRACnum ExprListList {MkLeftC($4,$2); $$ = MkRightC(NullExp(), $2);};

  //todo: I may need to return dummy for the $4, if we are a the end of the list.
  // | LBRACnum ExprList RBRACnum ExprListList {MkLeftC($4,$2); $$ = MkRightC(NullExp(), $2);};
// expr,expr,...

// Try flipping the tree building order here first
// then flip the recursion?
ExprList
  : Expression {$$ = MakeTree(CommaOp, $1, NullExp());}
  | Expression COMMAnum ExprList {$$ = MakeTree(CommaOp, $1, $3);};
/* ExprList
  : Expression {$$ = MakeTree(CommaOp, $1, NullExp());}
  | Expression COMMAnum ExprList {$$ = MkLeftC(MakeTree(CommaOp, $1, NullExp()), $3);}; */



%%

int yycolumn = 1, yyline = 1;
void yyerror(char *str)
{
  printf("yyerror: %s at line %d\n", str, yyline);
  exit(0);
}

#include "lex.yy.c"
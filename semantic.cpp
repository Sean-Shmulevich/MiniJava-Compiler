#include <vector>
#include <map>
#include <string>
#include <variant>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
extern "C"
{
#include "proj2.h"
}
int getIndex(const char *string);
#include "proj3.h"
extern tree SyntaxTree;
int offset = 0;
int offset1 = 0;
int selectOpClass = -1;

// is it possible to free this by passing reference?
// id node in this function def
// input leaf
// output, side effects: id_node is replaced with STNode
// If I pass in the pointer it will for sure free the right thing.
void makeSTLeafRight(tree curr_tree, int st_idx)
{
	tree id_node = RightChild(curr_tree);
	free(id_node);
	SetRightChild(curr_tree, MakeLeaf(STNode, st_idx));
}

uintptr_t getDimentions(tree assignment)
{
	tree initializer = LeftChild(assignment);
	int val = 0;
	if (NodeOp(initializer) == BoundOp)
	{
		val = IntVal(RightChild(initializer));
	}
	// if (NodeOp(initializer) == CommaOp)
	else
	{
		while (!IsNull(initializer))
		{
			val += 1;
			initializer = LeftChild(initializer);
		}
	}
	// Create a new vector to store dimensions
	std::vector<int> *dimensions = new std::vector<int>();
	dimensions->push_back(val);

	return (uintptr_t)dimensions;
}

// Define the variant type to hold either `tree` or `uintptr_t`

std::map<std::string, uintptr_t> initMethod(tree treenode)
{
	std::map<std::string, uintptr_t> infoMap;

	// Populate the map with keys as variable names and values as uintptr_t
	infoMap["method_op"] = (uintptr_t)treenode;
	infoMap["method_body"] = (uintptr_t)RightChild(treenode);

	tree head_op = LeftChild(treenode);
	infoMap["head_op"] = (uintptr_t)head_op;
	infoMap["method_name"] = (uintptr_t)LeftChild(head_op);

	tree spec_op = RightChild(head_op);
	infoMap["spec_op"] = (uintptr_t)spec_op;
	infoMap["method_type"] = (uintptr_t)RightChild(spec_op);
	infoMap["method_args"] = (uintptr_t)LeftChild(spec_op);

	return infoMap;
}
void mkArgEntry(tree node, int arg_kind)
{
	tree var_info = LeftChild(node);
	tree param_type = RightChild(var_info);
	tree val = LeftChild(var_info);
	int symbol_table_index = InsertEntry(IntVal(val), arg_kind, val->LineNo);
	SetAttr(symbol_table_index, TYPE_ATTR, (uintptr_t)param_type);
	SetAttr(symbol_table_index, OFFSET_ATTR, offset);
	SetLeftChild(var_info, MakeLeaf(STNode, symbol_table_index));
}

int countMethodArgs(tree method_args)
{
	tree tempTree = method_args;
	int num_args = 0;
	while (!IsNull(tempTree))
	{
		num_args += 1;
		// NOTE: Could I make a method or a macro for this?
		// A class is probably the simplest.
		tempTree = RightChild(tempTree);
	}
	return num_args;
}

void checkCustomType(tree decl_type)
{
	if (NodeKind(LeftChild(decl_type)) == IDNode)
	{
		int custom_type_index;
		if (LookUp(IntVal(LeftChild(decl_type))) != -1)
		{
			custom_type_index = LookUp(IntVal(LeftChild(decl_type)));
		}
		else
		{
			custom_type_index = InsertEntry(IntVal(LeftChild(decl_type)), VAR, LeftChild(decl_type)->LineNo);
		}
		SetLeftChild(decl_type, MakeLeaf(STNode, custom_type_index));
		// it will still be idnode, I will need to set it to the existing one.
	}
}

void initST(tree treenode)
{
	int symbol_table_index = InsertEntry(IntVal(RightChild(treenode)), PROGRAM, RightChild(treenode)->LineNo);
	/* Replace IDNode with STNode */
	makeSTLeafRight(treenode, symbol_table_index);
}

void MkST(tree treenode)
{
	if (treenode == SyntaxTree)
	{
		/* Open global scope for the entire source file. */
		OpenBlock();

		initST(treenode);

		OpenBlock();
		STAddPredefined();
		MkST(LeftChild(treenode)); // classlist
		CloseBlock();

		CloseBlock();

		/* Close global scope for the entire source file. */
		return;
	}

	/* Recursive traversal for all other nodes */
	if (!IsNull(treenode))
	{
		/* Recursive tree traversal strategy */
		switch (NodeOp(treenode))
		{
		// only one program op per program, if we see it again, its likely an error, and it should have been caught in the parsing phase.
		case ClassDefOp:
		{
			// Insert class name into symbol table
			int symbol_table_index = InsertEntry(IntVal(RightChild(treenode)), CLASS, RightChild(treenode)->LineNo);
			// Replace IDNode with STNode for class name

			tree id_node = RightChild(treenode);
			free(id_node);
			SetRightChild(treenode, MakeLeaf(STNode, symbol_table_index));

			// body of the class
			OpenBlock();
			MkST(LeftChild(treenode));
			CloseBlock();

			break;
		}
		case MethodOp:
		{
			tree method_op = treenode;
			auto method = initMethod(treenode);

			if (NodeKind((tree)method["method_name"]) == IDNode)
			{

				// Insert method name into symbol table
				int idLineNo = ((tree)method["method_name"])->LineNo;
				int symbol_table_index = InsertEntry(IntVal((tree)method["method_name"]), FUNC, idLineNo);

				// Set method type attribute
				SetAttr(symbol_table_index, TYPE_ATTR, method["method_type"]);

				// Set method initialization (body) attribute
				SetAttr(symbol_table_index, INIT_ATTR, (uintptr_t)treenode);

				int num_args = countMethodArgs((tree)method["method_args"]);
				SetAttr(symbol_table_index, ARGNUM_ATTR, num_args);

				// Replace IDNode with STNode for method name
				tree id_node = (tree)method["method_name"];
				free(id_node);
				SetLeftChild((tree)method["head_op"], MakeLeaf(STNode, symbol_table_index));
			}

			OpenBlock(); // Open function scope

			// Process method body
			MkST((tree)method["spec_op"]);
			MkST(RightChild(treenode));

			CloseBlock(); // Close function scope
			break;
		}
		case RArgTypeOp:
		{
			mkArgEntry(treenode, REF_ARG);
			offset += 1;
			MkST(RightChild(treenode));
			offset += 0;
			break;
		}
		case VArgTypeOp:
		{
			mkArgEntry(treenode, VALUE_ARG);
			offset += 1;
			MkST(RightChild(treenode));
			offset = 0;
			break;
		}
		case DeclOp:
		{
			// this is actually comma op.
			tree decl_op = RightChild(treenode);
			tree decl_id = LeftChild(decl_op);
			tree decl_type = LeftChild(RightChild(decl_op));
			tree assignment = RightChild(RightChild(decl_op));
			// recurse to the deepest declaration in a declOp line. (Variable assignment)
			// decl's on different lines also have offset.

			// Check and set, this breaks the single responsiblity principal.
			checkCustomType(decl_type);

			int symbol_table_index;
			int isArrayTypeOp = NodeOp(assignment) == ArrayTypeOp ? ARR : VAR;
			symbol_table_index = InsertEntry(IntVal(decl_id), isArrayTypeOp, decl_id->LineNo);

			SetAttr(symbol_table_index, TYPE_ATTR, (uintptr_t)decl_type);

			// Set method initialization (body) attribute

			if (!IsNull(assignment) && NodeOp(assignment) == ArrayTypeOp)
			{
				// if comma op to the left of array type op
				auto dimensions = getDimentions(assignment);
				// Set dimensions attribute
				SetAttr(symbol_table_index, INIT_ATTR, (uintptr_t)assignment);
				SetAttr(symbol_table_index, DIMEN_ATTR, (uintptr_t)dimensions);
			}
			else if (!IsNull(assignment))
			{
				SetAttr(symbol_table_index, INIT_ATTR, (uintptr_t)assignment);
			}
			SetAttr(symbol_table_index, OFFSET_ATTR, offset);

			free(decl_id);
			SetLeftChild(decl_op, MakeLeaf(STNode, symbol_table_index));

			offset += 1;
			// left child will be body op if its in a csv decl on one line,
			// else, it will be a bodyOp. with more decl-ops inside.
			MkST(LeftChild(treenode));
			// I forgot how this works but it works well.
			if (IsNull(LeftChild(treenode)))
			{
				offset = 0;
			}

			break;
		}
		case BodyOp:
		{
			if (NodeOp(RightChild(treenode)) == DeclOp)
			{
				MkST(LeftChild(treenode)); // another body op
				if (!IsNull(LeftChild(treenode)))
				{
					offset1 += 1;
					offset = offset1;
				}
				else
				{
					offset1 = 0;
					offset = offset1;
				}
				// only one declaration.
				MkST(RightChild(treenode)); // a declOp
				break;
			}
			MkST(LeftChild(treenode));
			MkST(RightChild(treenode));
			break;
		}
		case VarOp:
		{
			tree var_op = treenode;
			tree var_id = LeftChild(var_op);
			// printf("var op kind: %d\n", NodeKind(var_op));
			if (NodeKind(var_id) == IDNode)
			{

				// if I find system here, I want to look it up in the system class
				// whatever the nesting level of the result of this lookup is.
				// printf("LOOKUP: %d\n", symbol_table_index);
				int symbol_table_index = LookUp(IntVal(var_id));
				// TODO: add and not != to the case below.
				if (NodeOp(RightChild(treenode)) == SelectOp)
				{
					// if lookup is true, the var is within the scope.
					// selectOpClass = symbol_table_index;
					SetLeftChild(var_op, MakeLeaf(STNode, symbol_table_index));
					// printf("%d \n", NodeKind(LeftChild((tree)GetAttr(symbol_table_index, TYPE_ATTR))) == STNode);
					int var_class_index;
					// Type is not an attribute.
					if (IsAttr(symbol_table_index, TYPE_ATTR))
					{
						var_class_index = IntVal(LeftChild((tree)GetAttr(symbol_table_index, TYPE_ATTR)));
						// if (var_class_index != 0)
						// {
						selectOpClass = var_class_index;
						// }
					}
					// What is this code for?
					// I think its revlevant when symbol_table index is -1?
					// no thats not possible.
					else if (IsAttr(symbol_table_index, NAME_ATTR))
					{
						// get type from
						int i = 0;
						while (GetAttr(i, NAME_ATTR) != GetAttr(symbol_table_index, NAME_ATTR))
						{
							i++;
						}
						selectOpClass = i;
					}
				}
				else if (symbol_table_index != -1)
				{
					// SetLeftChild(var_op, MakeLeaf(STNode, symbol_table_index));
					free(var_id);
					SetLeftChild(var_op, MakeLeaf(STNode, symbol_table_index));
				}
				else
				{
					// this should always be found because it was declared. If not ERROR!
				}
			}
			MkST(RightChild(treenode));
			break;
		}
		// assign is more like a routine then a select.
		case SelectOp:
		{
			tree sel_op = treenode;
			tree field_op = LeftChild(treenode); // could also be indexop
			tree field_id = LeftChild(field_op);

			int def_within_class;
			int foundOp = -1;

			if (NodeKind(field_id) == IDNode)
			{
				int i = 1;
				int initType;
				while (GetAttr(selectOpClass + i, KIND_ATTR) != CLASS)
				{
					if (IntVal(field_id) == GetAttr(selectOpClass + i, NAME_ATTR))
					{
						foundOp = i + selectOpClass;
						if (IsAttr(foundOp, TYPE_ATTR))
						{
							int typeLoc = IntVal(LeftChild((tree)GetAttr(foundOp, TYPE_ATTR)));
							// if this is zero, then we have gotten a primitive type back.
							if (typeLoc != 0)
							{
								selectOpClass = typeLoc;
							}
						}
						free(field_id);
						SetLeftChild(field_op, MakeLeaf(STNode, foundOp));
						break;
					}
					i++;
				}
				// printf("selectopclass op: %d: found? %d hellp: %d\n", selectOpClass, foundOp, IntVal(field_id));
			}

			MkST(RightChild(treenode)); // expression here?
			MkST(LeftChild(treenode));	// expression here?
			break;
		}

		default:
			MkST(LeftChild(treenode));
			MkST(RightChild(treenode));
			/* Recursively process children for other node types */
			break;
		}
	}
}
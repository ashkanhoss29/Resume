/**
 * cs541 Compiler Design
 * @author Ashkan Hosseini
 *
 * Specification of nodes for the CSX Abstract Syntax Tree
 * For this program, the nodes contain Unparse(), which is
 *   used to reconstruct the CSX test program for output
 *   in order to show the correct parse
 *
 */

class ASTNode {
// abstract superclass; only subclasses are actually created

	int linenum;
	int colnum;

	static void genIndent(final int indent) {
		for (int i = 1; i <= indent; i += 1) {
			System.out.print("\t");
		}
	} // genIndent

	ASTNode() {
		linenum = -1;
		colnum = -1;
	} // ASTNode()

	ASTNode(final int line, final int col) {
		linenum = line;
		colnum = col;
	} // ASTNode(line, col)

	boolean isNull() {
		return false; // often redefined in a subclass
	} // isNull()

	void Unparse(final int indent) {
		// This routine is normally redefined in a subclass
	} // Unparse()
} // class ASTNode

class nullNode extends ASTNode {
// This class definition probably doesn't need to be changed
	nullNode() {
		super();
	}

	boolean isNull() {
		return true;
	}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullNode

class classNode extends ASTNode {
	classNode(final identNode id, final memberDeclsNode memb, final int line, final int col) {
		super(line, col);
		className = id;
		members = memb;
	} // classNode

	@Override
	void Unparse(final int ident) {
    	System.out.print(linenum + ":" + " class ");
		className.Unparse(0);
		System.out.println(" {");
		members.Unparse(1);
		System.out.println(linenum + ":" + " } EOF");
	}

	private final identNode className;
	private final memberDeclsNode members;
} // class classNode

class memberDeclsNode extends ASTNode {
	memberDeclsNode() {
		super();
	}
	memberDeclsNode(final declNode f, final methodDeclsNode m, final memberDeclsNode ms, final int line, final int col) {
		super(line, col);
		field = f;
		methods = m;
		moreMembers = ms;
	}

	@Override
	void Unparse(final int ident) {
		field.Unparse(ident);
		methods.Unparse(ident);
		moreMembers.Unparse(ident);
	}

	static nullMemberDeclsNode NULL = new nullMemberDeclsNode();
	private declNode field;
	private methodDeclsNode methods;
	private memberDeclsNode moreMembers;
} // class memberDeclsNode

class nullMemberDeclsNode extends memberDeclsNode {
	nullMemberDeclsNode() {
		// no action
	}
	boolean isNull() {
		return true;
	}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullMemberDeclsNode

class fieldDeclsNode extends ASTNode {
	fieldDeclsNode() {
		super();
	}
	fieldDeclsNode(final declNode d, final fieldDeclsNode f, final int line, final int col) {
		super(line, col);
		thisField = d;
		moreFields = f;
	}

	@Override
	void Unparse(final int indent) {
		thisField.Unparse(indent);
		moreFields.Unparse(indent);
	}

	static nullFieldDeclsNode NULL = new nullFieldDeclsNode();
	private declNode thisField;
	private fieldDeclsNode moreFields;
} // class fieldDeclsNode

class nullFieldDeclsNode extends fieldDeclsNode {
	nullFieldDeclsNode() {
		// no action
	}
	boolean isNull() {
		return true;
	}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullFieldDeclsNode

// abstract superclass; only subclasses are actually created
class declNode extends ASTNode {
	declNode() {
		super();
	}
	declNode(final int l, final int c) {
		super(l, c);
	}

    static nullDeclNode NULL = new nullDeclNode();
} // class declNode

class nullDeclNode extends declNode {
	nullDeclNode() {
		// no action
	}
	boolean isNull() {
		return true;
	}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullDeclNode

class varDeclNode extends declNode {
	varDeclNode(final identNode id, final typeNode t, final exprNode e, final int line, final int col) {
		super(line, col);
		varName = id;
		varType = t;
		initValue = e;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print(linenum + ":");
		for(int i = 0; i < indent; i++) {
			System.out.print("\t");
		}
		varType.Unparse(0);
		varName.Unparse(0);
		if(!initValue.isNull()) {
			System.out.print(" = ");
			initValue.Unparse(0);
		}
		System.out.println(";");
	}
		

	private final identNode varName;
	private final typeNode varType;
	private final exprNode initValue;
} // class varDeclNode

class constDeclNode extends declNode {
	constDeclNode(final identNode id, final exprNode e, final int line, final int col) {
		super(line, col);
		constName = id;
		constValue = e;
	}
	
	@Override
	void Unparse(final int indent) {
		System.out.print(linenum + ":");
		genIndent(indent);
		System.out.print("const ");
		constName.Unparse(0);
		System.out.print(" = ");
		constValue.Unparse(0);
		System.out.println(";");
	}

	private final identNode constName;
	private final exprNode constValue;
} // class constDeclNode

class arrayDeclNode extends declNode {
	arrayDeclNode(final identNode id, final typeNode t, final intLitNode lit, final int line, final int col) {
		super(line, col);
		arrayName = id;
		elementType = t;
		arraySize = lit;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print(linenum + ":");
		genIndent(indent);
		elementType.Unparse(0);
		arrayName.Unparse(0);
		System.out.print("[");
		arraySize.Unparse(0);
		System.out.println("];");
	}

	private final identNode arrayName;
	private final typeNode elementType;
	private final intLitNode arraySize;
} // class arrayDeclNode

abstract class typeNode extends ASTNode {
// abstract superclass; only subclasses are actually created
	typeNode() {
		super();
	}
	typeNode(final int l, final int c) {
		super(l, c);
	}
	static nullTypeNode NULL = new nullTypeNode();
} // class typeNode

class nullTypeNode extends typeNode {
	nullTypeNode() {
		// no action
	}
	boolean isNull() {
		return true;
	}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullTypeNode

class intTypeNode extends typeNode {
	intTypeNode(final int line, final int col) {
		super(line, col);
	}

	@Override
	void Unparse(final int indent) {
		System.out.print("int ");
	}
} // class intTypeNode

class boolTypeNode extends typeNode {
	boolTypeNode(final int line, final int col) {
		super(line, col);
	}

	@Override
	void Unparse(final int indent) {
		System.out.print("bool ");
	}
} // class boolTypeNode

class charTypeNode extends typeNode {
	charTypeNode(final int line, final int col) {
		super(line, col);
	}

	@Override
	void Unparse(final int indent) {
		System.out.print("char ");
	}
} // class charTypeNode

class voidTypeNode extends typeNode {
	voidTypeNode(final int line, final int col) {
		super(line, col);
	}

	@Override
	void Unparse(final int indent) {
		System.out.print("void ");
	}
} // class voidTypeNode

class methodDeclsNode extends ASTNode {
	methodDeclsNode() {
		super();
	}
	methodDeclsNode(final methodDeclNode m, final methodDeclsNode ms, final int line, final int col) {
		super(line, col);
		thisDecl = m;
		moreDecls = ms;
	}

	@Override
	void Unparse(final int indent) {
		thisDecl.Unparse(indent);
		moreDecls.Unparse(indent);
	}

	static nullMethodDeclsNode NULL = new nullMethodDeclsNode();
	private methodDeclNode thisDecl;
	private methodDeclsNode moreDecls;
} // class methodDeclsNode 

class nullMethodDeclsNode extends methodDeclsNode {
	nullMethodDeclsNode() {
		// no action
	}
	boolean   isNull() {return true;}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullMethodDeclsNode 

class methodDeclNode extends ASTNode {
	methodDeclNode(final identNode id, final argDeclsNode a, final typeNode t,
			final fieldDeclsNode f, final stmtsNode s, final int line, final int col) {
		super(line, col);
		name = id;
		args = a;
		returnType = t;
		decls = f;
		stmts = s;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print("\n" + linenum + ":");
		genIndent(indent);
		returnType.Unparse(0);
		name.Unparse(0);
		System.out.print(" (");
		args.Unparse(0);
		System.out.println(") {");
		decls.Unparse(indent+1);
		stmts.Unparse(indent+1);
		System.out.print(linenum + ":");
		for(int i = 0; i < indent; i++) {
			System.out.print("\t");
		}
		System.out.println("}");
	}

	private final identNode name;
	private final argDeclsNode args;
	private final typeNode returnType;
	private final fieldDeclsNode decls;
	private final stmtsNode stmts;
} // class methodDeclNode 

// abstract superclass; only subclasses are actually created
abstract class argDeclNode extends ASTNode {
	argDeclNode() {
		super();
	}
	argDeclNode(final int l, final int c) {
		super(l, c);
	}
}

class argDeclsNode extends ASTNode {
	argDeclsNode() {
		// no action
	}
	argDeclsNode(final argDeclNode arg, final argDeclsNode args,
			final int line, final int col) {
		super(line, col);
		thisDecl = arg;
		moreDecls = args;
	}

	@Override
	void Unparse(final int indent) {
		thisDecl.Unparse(0);
		if(!moreDecls.isNull()) {
			System.out.print(", ");
			moreDecls.Unparse(0);
		}
	}

	static nullArgDeclsNode NULL = new nullArgDeclsNode();
	private argDeclNode thisDecl;
	private argDeclsNode moreDecls;
} // class argDeclsNode 

class nullArgDeclsNode extends argDeclsNode {
	nullArgDeclsNode() {
		// no action
	}
	boolean   isNull() {return true;}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullArgDeclsNode 

class arrayArgDeclNode extends argDeclNode {
	arrayArgDeclNode(final identNode id, final typeNode t, final int line, final int col) {
		super(line, col);
		argName = id;
		elementType = t;
	}

	@Override
	void Unparse(final int indent) {
		elementType.Unparse(0);
		argName.Unparse(0);
		System.out.print("[]");
	}

	private final identNode argName;
	private final typeNode elementType;
} // class arrayArgDeclNode 

class valArgDeclNode extends argDeclNode {
	valArgDeclNode(final identNode id, final typeNode t, final int line, final int col) {
		super(line, col);
		argName = id;
		argType = t;
	}

	@Override
	void Unparse(final int indent) {
		argType.Unparse(0);
		argName.Unparse(0);
	}

	private final identNode argName;
	private final typeNode argType;
} // class valArgDeclNode 

// abstract superclass; only subclasses are actually created
abstract class stmtNode extends ASTNode {
	stmtNode() {
		super();
	}
	stmtNode(final int l, final int c) {
		super(l, c);
	}
	static nullStmtNode NULL = new nullStmtNode();
}

class nullStmtNode extends stmtNode {
	nullStmtNode() {
		// no action
	}
	boolean   isNull() {return true;}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullStmtNode 

class stmtsNode extends ASTNode {
	stmtsNode() {
		// no action
	}
	stmtsNode(final stmtNode stmt, final stmtsNode stmts, final int line, final int col) {
		super(line, col);
		thisStmt = stmt;
		moreStmts = stmts;
	}

	@Override
	void Unparse(final int indent) {
		thisStmt.Unparse(indent);
		moreStmts.Unparse(indent);
	} 

	static nullStmtsNode NULL = new nullStmtsNode();
	private stmtNode thisStmt;
	private stmtsNode moreStmts;
} // class stmtsNode 

class nullStmtsNode extends stmtsNode {
	nullStmtsNode() {
		// no action
	}
	boolean isNull() {return true;}

	@Override
	void Unparse(final int indent) {
		// no action
	}

} // class nullStmtsNode 

class asgNode extends stmtNode {
	asgNode(final nameNode n, final exprNode e, final int line, final int col) {
		super(line, col);
		target = n;
		source = e;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print(linenum + ":");
		genIndent(indent);
		target.Unparse(0);
		System.out.print(" = ");
		source.Unparse(0);
		System.out.println(";");
	}

	private final nameNode target;
	private final exprNode source;
} // class asgNode 

class ifThenNode extends stmtNode {
	ifThenNode(final exprNode e, final stmtNode s1, final stmtNode s2, final int line, final int col) {
		super(line, col);
		condition = e;
		thenPart = s1;
		elsePart = s2;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print(linenum + ":");
		genIndent(indent);
		System.out.print("if (");
		condition.Unparse(0);
		System.out.println(")");
		thenPart.Unparse(indent+1);
		if(!elsePart.isNull()) {
			System.out.print(linenum + ":");
			genIndent(indent);
			System.out.println("else");
			elsePart.Unparse(indent+1);
		}
		
	}

	private final exprNode condition;
	private final stmtNode thenPart;
	private final stmtNode elsePart;
} // class ifThenNode 

class whileNode extends stmtNode {
	whileNode(final exprNode e, final stmtNode s, final int line, final int col) {
		super(line, col);
		condition = e;
		loopBody = s;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print(linenum + ":");
		genIndent(indent);
		System.out.print("while (");
		condition.Unparse(0);
		System.out.println(")");
		loopBody.Unparse(indent+1);
	}

	private final exprNode condition;
	private final stmtNode loopBody;
} // class whileNode 

class readNode extends stmtNode {
	//readNode() {}
	readNode(final readListNode rn, final int line, final int col) {
		super(line, col);
		reads = rn;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print(linenum + ":");
		genIndent(indent);
		System.out.print("read (");
		reads.Unparse(0);
		System.out.println(");");
	}

	private final readListNode reads;
} // class readNode

class readListNode extends stmtNode {
	readListNode() {
		// no action
	}
	readListNode(final nameNode n, final readListNode rn, final int line, final int col) {
		super(line, col);
		 targetVar = n;
		 moreReads = rn;
	}
	
	@Override
	void Unparse(final int indent) {
		targetVar.Unparse(0);
		if(!moreReads.isNull()) {
			System.out.print(", ");
			moreReads.Unparse(0);
		}
	}

	static nullReadListNode NULL = new nullReadListNode();
	private nameNode targetVar;
	private readListNode moreReads;
} // class readListNode 

class nullReadListNode extends readListNode {
	nullReadListNode() {
		// no action
	}
	boolean   isNull() {return true;}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullReadListNode 

class printNode extends stmtNode {
	//printNode() {}
	printNode(final printListNode rn, final int line, final int col) {
		super(line, col);
		reads = rn;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print(linenum + ":");
		genIndent(indent);
		System.out.print("print (");
		reads.Unparse(0);
		System.out.println(");");
	}

	private final printListNode reads;
} // class readNode

class printListNode extends stmtNode {
	printListNode() {
		// no action
	}
	printListNode(final exprNode val, final printListNode pn, final int line, final int col) {
		super(line, col);
		outputValue = val;
		morePrints = pn;
	}

	@Override
	void Unparse(final int indent) {
		outputValue.Unparse(0);
		if(!morePrints.isNull()) {
			System.out.print(", ");
			morePrints.Unparse(0);
		}
	}

	static nullPrintListNode NULL = new nullPrintListNode();
	private exprNode outputValue;
	private printListNode morePrints;
} // class printListNode 

class nullPrintListNode extends printListNode {
	nullPrintListNode() {
		// no action
	}
	boolean   isNull() {return true;}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullPrintListNode 

class callNode extends stmtNode {
	callNode(final identNode id, final argsNode a, final int line, final int col) {
		super(line, col);
		methodName = id;
		args = a;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print(linenum + ":");
		genIndent(indent);
		methodName.Unparse(0);
		System.out.print("(");
		args.Unparse(0);
		System.out.println(");");
	}

	private final identNode methodName;
	private final argsNode args;
} // class callNode 

class returnNode extends stmtNode {
	returnNode(final exprNode e, final int line, final int col) {
		super(line, col);
		returnVal = e;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print(linenum + ":");
		genIndent(indent);
		System.out.print("return ");
		returnVal.Unparse(0);
		System.out.println(";");
	}

	private final exprNode returnVal;
} // class returnNode 

class blockNode extends stmtNode {
	blockNode(final fieldDeclsNode f, final stmtsNode s, final int line, final int col) {
		super(line, col);
		decls = f;
		stmts = s;
	}
	
	@Override
	void Unparse(final int indent) {
		System.out.print(linenum + ":");
		genIndent(indent-1);
		System.out.println("{");
		decls.Unparse(indent);
		stmts.Unparse(indent);
		System.out.print(linenum + ":");
		genIndent(indent-1);
		System.out.println("}");
	}

	private final fieldDeclsNode decls;
	private final stmtsNode stmts;
} // class blockNode 

/* break and continue currently not supported
class breakNode extends stmtNode {
	breakNode(final identNode i, final int line, final int col) {
		super(line, col);
		label = i;
	}

	private final identNode label;
} // class breakNode 

class continueNode extends stmtNode {
	continueNode(final identNode i, final int line, final int col) {
		super(line, col);
		label = i;
	}

	private final identNode label;
} // class continueNode 
*/

class argsNode extends ASTNode {
	argsNode() {
		// no action
	}
	argsNode(final exprNode e, final argsNode a, final int line, final int col) {
		super(line, col);
		argVal = e;
		moreArgs = a;
	}

	@Override
	void Unparse(final int indent) {
		argVal.Unparse(0);
		if(!moreArgs.isNull()) {
			System.out.print(", ");
			moreArgs.Unparse(0);
		}
	}

	static nullArgsNode NULL = new nullArgsNode();
	private exprNode argVal;
	private argsNode moreArgs;
} // class argsNode 

class nullArgsNode extends argsNode {
	nullArgsNode() {
		// empty constructor
	}
	boolean   isNull() {return true;}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullArgsNode 

// abstract superclass; only subclasses are actually created
abstract class exprNode extends ASTNode {
	exprNode() {
		super();
	}
	exprNode(final int l, final int c) {
		super(l, c);
	}
	static nullExprNode NULL = new nullExprNode();
}

class nullExprNode extends exprNode {
	nullExprNode() {
		super();
	}
	boolean   isNull() {return true;}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullExprNode 

class binaryOpNode extends exprNode {
	binaryOpNode(final exprNode e1, final int op, final exprNode e2, final int line, final int col) {
		super(line, col);
		operatorCode = op;
		leftOperand = e1;
		rightOperand = e2;
	}

	static void printOp(final int op) {
		switch (op) {
			case sym.COR:
				System.out.print(" || ");
				break;
			case sym.CAND:
				System.out.print(" && ");
				break;
			case sym.LT:
				System.out.print(" < ");
				break;
			case sym.GT:
				System.out.print(" > ");
				break;
			case sym.LEQ:
				System.out.print(" <= ");
				break;
			case sym.GEQ:
				System.out.print(" >= ");
				break;
			case sym.EQ:
				System.out.print(" == ");
				break;
			case sym.NOTEQ:
				System.out.print(" != ");
				break;
			case sym.PLUS:
				System.out.print(" + ");
				break;
			case sym.MINUS:
				System.out.print(" - ");
				break;
			case sym.TIMES:
				System.out.print(" * ");
				break;
			case sym.SLASH:
				System.out.print(" / ");
				break;
			default:
				throw new Error("printOp: case not found");
		}
	}

	@Override
	void Unparse(final int indent) {
		System.out.print("(");
		leftOperand.Unparse(0);
		printOp(operatorCode);
		rightOperand.Unparse(0);
		System.out.print(")");
	}

	private final exprNode leftOperand;
	private final exprNode rightOperand;
	private final int operatorCode; // Token code of the operator
} // class binaryOpNode 

class unaryOpNode extends exprNode {
	unaryOpNode(final exprNode e, final int line, final int col) {
		super(line, col);
		operand = e;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print("!");
		operand.Unparse(0);
	}

	private final exprNode operand;
} // class unaryOpNode 

class castNode extends exprNode {
	castNode(final typeNode t, final exprNode e, final int line, final int col) {
		super(line, col);
		operand = e;
		resultType = t;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print("(");
		resultType.Unparse(0);
		System.out.print(")");
		operand.Unparse(0);
	}

	private final exprNode operand;
	private final typeNode resultType;
} // class castNode 

class fctCallNode extends exprNode {
	fctCallNode(final identNode id, final argsNode a, final int line, final int col) {
		super(line, col);
		methodName = id;
		methodArgs = a;
	}

	@Override
	void Unparse(final int indent) {
		methodName.Unparse(0);
		System.out.print("(");
		methodArgs.Unparse(0);
		System.out.print(")");
	}

	private final identNode methodName;
	private final argsNode methodArgs;
} // class fctCallNode 

class identNode extends exprNode {
	identNode(final String identname, final int line, final int col) {
		super(line, col);
		idname   = identname;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print(idname);
	}

	private final String idname;
} // class identNode 

class nameNode extends exprNode {
	nameNode(final identNode id, final exprNode expr, final int line, final int col) {
		super(line, col);
		varName = id;
		subscriptVal = expr;
	}

	@Override
	void Unparse(final int indent) {
		varName.Unparse(0);
		if(!subscriptVal.isNull()) {
			System.out.print("[");
			subscriptVal.Unparse(0);
			System.out.print("]");
		}
	}

	private final identNode varName;
	private final exprNode subscriptVal;
} // class nameNode 

class intLitNode extends exprNode {
	intLitNode(final int val, final int line, final int col) {
		super(line, col);
		intval = val;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print(intval);
	}

	private final int intval;
} // class intLitNode 

class charLitNode extends exprNode {
	charLitNode(final String val, final int line, final int col) {
		super(line, col);
		charval = val;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print(charval);
	}

	private final String charval;
} // class charLitNode 

class strLitNode extends exprNode {
	strLitNode(final String stringval, final int line, final int col) {
		super(line, col);
		strval = stringval;
	}

	@Override
	void Unparse(final int indent) {
		System.out.print(strval);
	}

	private final String strval;
} // class strLitNode 

class trueNode extends exprNode {
	trueNode(final int line, final int col) {
		super(line, col);
	}

	@Override
	void Unparse(final int indent) {
		System.out.print("true");
	}
} // class trueNode 

class falseNode extends exprNode {
	falseNode(final int line, final int col) {
		super(line, col);
	}

	@Override
	void Unparse(final int indent) {
		System.out.print("false");
	}
} // class falseNode 

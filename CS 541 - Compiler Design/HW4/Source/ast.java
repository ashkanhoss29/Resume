/**
 * cs541 Compiler Design
 * @author Ashkan Hosseini
 *
 * Specification of nodes for the CSX Abstract Syntax Tree
 * For this program, the nodes contain checkTypes(), which is
 *   used to step through the AST of the test program to check
 *   for any type errors.
 *
 */

class ASTNode {
// abstract superclass; only subclasses are actually created

	int linenum;
	int colnum;

	static int typeErrors = 0; // Total number of type errors found
	public static SymbolTable st = new SymbolTable();

	ASTNode() {
		linenum = -1;
		colnum = -1;
	} // ASTNode()

	ASTNode(final int line, final int col) {
		linenum = line;
		colnum = col;
	} // ASTNode(line, col)

	static void mustBe(boolean assertion) {
		if (! assertion) {
			throw new RuntimeException();
		}
	} // mustBe()

	static void typeMustBe(int testType, int requiredType, String errorMsg) {
		if ((testType != Types.Error) && (testType != requiredType)) {
			System.out.println(errorMsg);
			typeErrors++;
		}
	} // typeMustBe()
                                                                               
	static void typesMustBeEqual(int type1, int type2, String errorMsg) {
		if ((type1 != Types.Error) && (type2 != Types.Error) 
				&& (type1 != type2)) {
			System.out.println(errorMsg);
			typeErrors++;
		}
	} // typesMustBeEqual

	String error() {
		return "Error (line " + linenum + "): ";
	} // error()

	boolean isNull() {
		return false; // often redefined in a subclass
	} // isNull()

	void checkTypes() {
		// This routine is normally redefined in a subclass
	} // checkTypes()

	static int getErrorCount () {
		return typeErrors;
	}

	void Unparse(final int indent) {
		// This routine is normally redefined in a subclass
	} // Unparse()

	static void genIndent(final int indent) {
		for (int i = 1; i <= indent; i += 1) {
			System.out.print("\t");
		}
	} // genIndent

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
	void checkTypes() {
		// no action
	}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullNode

class classNode extends ASTNode {
	classNode(final identNode id, final memberDeclsNode memb, 
			final int line, final int col) {
		super(line, col);
		className = id;
		members = memb;
	} // classNode

	@Override
	void checkTypes() {
		// className.checkTypes(); // only one class in CSX, so not needed
		members.checkTypes();
	}

	boolean isTypeCorrect() {
		checkTypes();
		return (typeErrors == 0);
	}

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
	memberDeclsNode(final declNode f, final methodDeclsNode m, 
					final memberDeclsNode ms, final int line, final int col) {
		super(line, col);
		field = f;
		methods = m;
		moreMembers = ms;
	}

	@Override
	void checkTypes() {
		field.checkTypes();
		methods.checkTypes();
		moreMembers.checkTypes();
		/*
		if (!methods.isNull()) {
			methods.checkTypes();
		}
		else {
			field.checkTypes();
			moreMembers.checkTypes();
		}*/
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
	void checkTypes() {
		// no action
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
	fieldDeclsNode(final declNode d, final fieldDeclsNode f, 
			final int line, final int col) {
		super(line, col);
		thisField = d;
		moreFields = f;
	}

	@Override 
	void checkTypes() {
		thisField.checkTypes();
		moreFields.checkTypes();
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
	void checkTypes() {
		// no action
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
	void checkTypes() {
		// no action
	}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullDeclNode

class varDeclNode extends declNode {
	varDeclNode(final identNode id, final typeNode t, 
			final exprNode e, final int line, final int col) {
		super(line, col);
		varName = id;
		varType = t;
		initValue = e;
	}

	@Override
	void checkTypes() {
        SymbolInfo id = null;
		try {
			id = (SymbolInfo) st.localLookup(varName.idname);
		}
		catch (EmptySTException e) {
			System.out.println(error() + "No scope open in symbol table.");
		}
		// Check already declared variable
		if (id == null) {
			id = new SymbolInfo(varName.idname, new Kinds(Kinds.Var), 
								varType.type);
			try {
				st.insert(id);
			}
			catch (DuplicateException d) {
				// can't happen
			}
			catch (EmptySTException e) {
				// can't happen
			}
		}
		else {
			System.out.println( error() + varName.idname 
										+ " is already declared.");
			typeErrors++;
		}
		// Check initial assignment
		if (!initValue.isNull()) {
			initValue.checkTypes();
			typesMustBeEqual(id.type.val, initValue.type.val,
				error() + "Initial value for " + varName.idname 
						+ " is wrong type.");
		}
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
	constDeclNode(final identNode id, final exprNode e, 
			final int line, final int col) {
		super(line, col);
		constName = id;
		constValue = e;
	}

	@Override
	void checkTypes() {
		// Check if assignment value is type void (could be call to void method)
		if (constValue.type.val == Types.Void) {
			typeMustBe(constValue.type.val, Types.Error, 
				error() + "Const assign value cannot be void.");
		}
		// Check already declared variable
        SymbolInfo id = null;
		try {
			id = (SymbolInfo) st.localLookup(constName.idname);
		}
		catch (EmptySTException e) {
			System.out.println(error() + "No scope open in symbol table.");
		}
		if (id == null) {
			id = new SymbolInfo(constName.idname, new Kinds(Kinds.Value), 
								constValue.type);
			try {
				st.insert(id);
			}
			catch (DuplicateException d) {
				// can't happen
			}
			catch (EmptySTException e) {
				// can't happen
			}
		}
		else {
			System.out.println(error() + id.name() + " is already declared.");
			typeErrors++;
		}
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
	arrayDeclNode(final identNode id, final typeNode t, 
			final intLitNode lit, final int line, final int col) {
		super(line, col);
		arrayName = id;
		elementType = t;
		arraySize = lit;
	}

	@Override
	void checkTypes() {
		// Check already declared variable
        SymbolInfo id = null;
		try {
			id = (SymbolInfo) st.localLookup(arrayName.idname);
		}
		catch (EmptySTException e) {
			System.out.println(error() + "No scope open in symbol table.");
		}
		if (id == null) {
			id = new SymbolInfo(arrayName.idname, new Kinds(Kinds.Array), 
								elementType.type);
			try {
				st.insert(id);
			}
			catch (DuplicateException d) {
				// can't happen
			}
			catch (EmptySTException e) {
				// can't happen
			}
		}
		else {
			System.out.println(error() + id.name() + " is already declared.");
			typeErrors++;
		}
		// Check array size type
		arraySize.checkTypes();
		if (arraySize.type.val != Types.Integer 
				&& arraySize.type.val != Types.Character) {
			typeMustBe(arraySize.type.val, Types.Error,
				error() + "Array size type is " + arraySize.type.toString()
						+ " but must be int or char.");
		}
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

	typeNode(final int l, final int c, Types t) {
		super(l, c);
		type = t;
	}

	static nullTypeNode NULL = new nullTypeNode();
	Types type; // used for typechecking
} // class typeNode

class nullTypeNode extends typeNode {
	nullTypeNode() {
		// no action
	}

	boolean isNull() {
		return true;
	}

	@Override
	void checkTypes() {
		// no action
	}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullTypeNode

class intTypeNode extends typeNode {
	intTypeNode(final int line, final int col) {
		super(line, col, new Types(Types.Integer));
	}

	@Override
	void Unparse(final int indent) {
		System.out.print("int ");
	}
} // class intTypeNode

class boolTypeNode extends typeNode {
	boolTypeNode(final int line, final int col) {
		super(line, col, new Types(Types.Boolean));
	}

	@Override
	void Unparse(final int indent) {
		System.out.print("bool ");
	}
} // class boolTypeNode

class charTypeNode extends typeNode {
	charTypeNode(final int line, final int col) {
		super(line, col, new Types(Types.Character));
	}

	@Override
	void Unparse(final int indent) {
		System.out.print("char ");
	}
} // class charTypeNode

class voidTypeNode extends typeNode {
	voidTypeNode(final int line, final int col) {
		super(line, col, new Types(Types.Void));
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
	methodDeclsNode(final methodDeclNode m, final methodDeclsNode ms, 
			final int line, final int col) {
		super(line, col);
		thisDecl = m;
		moreDecls = ms;
	}

	@Override
	void checkTypes() {
		thisDecl.checkTypes();
		moreDecls.checkTypes();
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
	void checkTypes() {
		// no action
	}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullMethodDeclsNode 

class methodDeclNode extends ASTNode {
	methodDeclNode(final identNode id, final argDeclsNode a, 
			final typeNode t, final fieldDeclsNode f, 
			final stmtsNode s, final int line, final int col) {
		super(line, col);
		name = id;
		args = a;
		returnType = t;
		decls = f;
		stmts = s;
	}

	@Override
	void checkTypes() {
		
		// Methods are only declared in top most scope, so do globalLookup
        SymbolInfo id = null;
		id = (SymbolInfo) st.globalLookup(name.idname);

		if (id == null) {
			SymbolInfoMethod idM = null;
			idM = new SymbolInfoMethod(name.idname, new Kinds(Kinds.Method), 
										returnType.type);
			
			try {
				st.insert(idM);
			}
			catch (DuplicateException d) {
				// can't happen
			}
			catch (EmptySTException e) {
				// can't happen
			}
			
			st.openScope(); // open scope for method internals
			args.checkTypes(idM); // check args and store arg info

			// Check method body
			decls.checkTypes();
			stmts.checkTypes(returnType.type);

			// Close method scope
			try {
				st.closeScope();
			}
			catch (EmptySTException e) {
				System.out.println( error() + "No scope in symbol"
											+ " table at end of method");
			}

		}
		else {
			System.out.println( error() + "Method " + id.name() 
										+ " is already declared.");
			typeErrors++;

			//Check method body anyway
			decls.checkTypes();
			stmts.checkTypes(returnType.type);
		}

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

	void checkTypes(SymbolInfoMethod mId) {
		// Redefined in subclass. Need method info for checking argument lists
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

	//@Override
	void checkTypes(SymbolInfoMethod mId) {
		thisDecl.checkTypes(mId);
		moreDecls.checkTypes(mId);
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
	void checkTypes() {
		// no action
	}

	@Override
	void checkTypes(SymbolInfoMethod mId) {
		// no action
	}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullArgDeclsNode 

class arrayArgDeclNode extends argDeclNode {
	arrayArgDeclNode(final identNode id, final typeNode t, 
			final int line, final int col) {
		super(line, col);
		argName = id;
		elementType = t;
	}

	@Override
	void checkTypes(SymbolInfoMethod mId) {
        SymbolInfo id = null;
		try {
			id = (SymbolInfo) st.localLookup(argName.idname);
		}
		catch (EmptySTException e) {
			System.out.println(error() + "No scope open in symbol table.");
		}
		// Check already declared variable
		if (id == null) {
			id = new SymbolInfo(argName.idname, new Kinds(Kinds.ArrayParm), 
								elementType.type);
			mId.addArgument(new Kinds(Kinds.ArrayParm), elementType.type);
			try {
				st.insert(id);
			}
			catch (DuplicateException d) {
				// can't happen
			}
			catch (EmptySTException e) {
				// can't happen
			}
		}
		else {
			System.out.println(error() + id.name() + " is already declared.");
			typeErrors++;
		}
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
	valArgDeclNode(final identNode id, final typeNode t, 
			final int line, final int col) {
		super(line, col);
		argName = id;
		argType = t;
	}

	@Override
	void checkTypes(SymbolInfoMethod mId) {
        SymbolInfo id = null;
		try {
			id = (SymbolInfo) st.localLookup(argName.idname);
		}
		catch (EmptySTException e) {
			System.out.println(error() + "No scope open in symbol table.");
		}
		// Check already declared variable
		if (id == null) {
			id = new SymbolInfo(argName.idname, new Kinds(Kinds.ScalarParm), 
								argType.type);
			mId.addArgument(new Kinds(Kinds.ScalarParm), argType.type);
			try {
				st.insert(id);
			}
			catch (DuplicateException d) {
				// can't happen
			}
			catch (EmptySTException e) {
				// can't happen
			}
		}
		else {
			System.out.println(error() + id.name() + " is already declared.");
			typeErrors++;
		}
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
	
	void checkTypes(Types returnType) {
		// Redefined in subclass. Used for keeping track of method return type.
	}

	@Override
	void checkTypes() {
		System.out.println( error() + "Type checker error: " 
									+ "calling checkTypes() for stmt node");
	}

	static nullStmtNode NULL = new nullStmtNode();
}

class nullStmtNode extends stmtNode {
	nullStmtNode() {
		// no action
	}
	boolean   isNull() {return true;}

	@Override
	void checkTypes() {
		// no action
	}

	@Override
	void checkTypes(Types returnType) {
		// no action
	}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullStmtNode 

class stmtsNode extends ASTNode {
	stmtsNode() {
		// no action
	}
	stmtsNode(final stmtNode stmt, final stmtsNode stmts, 
				final int line, final int col) {
		super(line, col);
		thisStmt = stmt;
		moreStmts = stmts;
	}

	void checkTypes(Types returnType) {
		thisStmt.checkTypes(returnType);
		moreStmts.checkTypes(returnType);
	}

	@Override
	void checkTypes() {
		System.out.println( error() + "Type checker error: "
									+"calling checkTypes() for stmts node");
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
	void checkTypes() {
		// no action
	}

	@Override
	void checkTypes(Types returnType) {
		// no action
	}

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
	void checkTypes(Types returnType) {
		target.checkTypes();
		source.checkTypes();
		SymbolInfo id = (SymbolInfo) st.globalLookup(target.varName.idname);
		if(id != null) {
			if (id.kind.val == Kinds.Var || id.kind.val == Kinds.Array) {
				typeMustBe(id.type.val, source.type.val,
					error() + "Left hand side of assignment is " 
							+ id.type.toString()
							+ ", but right hand side is " 
							+ source.type.toString() + ".");
			}
			else {
				System.out.println( error() + "Left hand side of" 
											+ " assign cannot be of type "
											+ id.kind.toString());
				typeErrors++;
			}
		}
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
	ifThenNode(final exprNode e, final stmtNode s1, 
			final stmtNode s2, final int line, final int col) {
		super(line, col);
		condition = e;
		thenPart = s1;
		elsePart = s2;
	}

	@Override
	void checkTypes(Types returnType) {
		condition.checkTypes();
		typeMustBe(condition.type.val, Types.Boolean,
			error() + "If statement control type must be bool.");
		thenPart.checkTypes(returnType);
		if(!elsePart.isNull()) {
			elsePart.checkTypes(returnType);
		}
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
	whileNode(final exprNode e, final stmtNode s, 
			final int line, final int col) {
		super(line, col);
		condition = e;
		loopBody = s;
	}

	@Override
	void checkTypes(Types returnType) {
		condition.checkTypes();
		typeMustBe(condition.type.val, Types.Boolean,
			error() + "While statement control type must be bool.");
		loopBody.checkTypes(returnType);
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
	void checkTypes(Types returnType) {
		reads.checkTypes(returnType);
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
	readListNode(final nameNode n, final readListNode rn, 
			final int line, final int col) {
		super(line, col);
		 targetVar = n;
		 moreReads = rn;
	}

	@Override
	void checkTypes(Types returnType) {
		targetVar.checkTypes();
		SymbolInfo id;
		id = (SymbolInfo) st.globalLookup(targetVar.varName.idname);
		if (id != null) {
			if (id.type.val != Types.Integer && id.type.val != Types.Character) {
				System.out.println( error() + "Read argument is " 
											+ id.type.toString()
											+ " but must be int or char.");
				typeErrors++;
			}
			if (id.kind.val != Kinds.Var && id.kind.val != Kinds.Array) {
				System.out.println( error() + "Read argument is "
											+ id.kind.toString()
											+ " but must be variable.");
				typeErrors++;
			}
		}
		else {
			System.out.println( error() + "Read argument variable not found.");
			typeErrors++;
		}
		if(!moreReads.isNull()) {
			moreReads.checkTypes(returnType);
		}
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
	void checkTypes() {
		// no action
	}

	@Override
	void checkTypes(Types returnType) {
		// no action
	}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullReadListNode 

class printNode extends stmtNode {
	//printNode() {}
	printNode(final printListNode rn, final int line, final int col) {
		super(line, col);
		prints = rn;
	}

	@Override
	void checkTypes(Types returnType) {
		prints.checkTypes(returnType);
	}

	@Override
	void Unparse(final int indent) {
		System.out.print(linenum + ":");
		genIndent(indent);
		System.out.print("print (");
		prints.Unparse(0);
		System.out.println(");");
	}

	private final printListNode prints;
} // class readNode

class printListNode extends stmtNode {
	printListNode() {
		// no action
	}
	printListNode(final exprNode val, final printListNode pn, 
			final int line, final int col) {
		super(line, col);
		outputValue = val;
		morePrints = pn;
	}

	@Override
	void checkTypes(Types returnType) {
		outputValue.checkTypes();
		if (outputValue.type.val == Types.Void) {
			System.out.println( error() + "print() argument is type void but "
										+ "must be int, char, bool, or string.");
			typeErrors++;
		}
		if(!morePrints.isNull()) {
			morePrints.checkTypes(returnType);
		}
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
	void checkTypes() {
		// no action
	}

	@Override
	void checkTypes(Types returnType) {
		// no action
	}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullPrintListNode 

class callNode extends stmtNode {
	callNode(final identNode id, final argsNode a, 
			final int line, final int col) {
		super(line, col);
		methodName = id;
		args = a;
	}

	@Override
	void checkTypes(Types returnType) {
		SymbolInfo id;
		id = (SymbolInfo) st.globalLookup(methodName.idname);
		if (id != null) {
			if (id.kind.val == Kinds.Method) {
				SymbolInfoMethod mId; // method decl
				SymbolInfoMethod cId; // method call
				mId = (SymbolInfoMethod) st.globalLookup(methodName.idname);
				if (mId.type.val == Types.Void) {
					cId = new SymbolInfoMethod(methodName.idname, mId.kind, mId.type);
					args.checkTypes(cId);
					if (mId.kindList.size() == cId.kindList.size()) {
						for (int i = 0; i < mId.kindList.size(); i++) {
							typeMustBe( mId.kindList.get(i).val, 
									cId.kindList.get(i).val, 
									error() + "Argument " + i + " has kind " 
										+ cId.kindList.get(i).toString()
										+ " but matching parameter has kind " 
										+ mId.kindList.get(i).toString() + ".");
							typeMustBe( mId.typeList.get(i).val, 
									cId.typeList.get(i).val, 
									error() + "Argument " + i + " has type " 
										+ cId.typeList.get(i).toString()
										+ " but matching parameter has type " 
										+ mId.typeList.get(i).toString() + ".");
						}
					}
					else {
						System.out.println( error() 
											+ "Wrong number of arguments"
											+ " to parameters");
						typeErrors++;
					}
				}
				else {
					System.out.println( error() + "Not a procedure.");
					typeErrors++;
				}
			}
			else {
				System.out.println(error() + "Not a method.");
				typeErrors++;
			}
		}
		else {
			System.out.println( error() + "Method " + methodName.idname 
										+ " not found for function call.");
			typeErrors++;
		}
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
	void checkTypes(Types returnType) {
		if(!returnVal.isNull()) {
			typeMustBe(returnType.val, Types.Void,
				error() + "Attempting \'return;\' but method type is not Void");
		}
		else {
			returnVal.checkTypes();
			if(!returnVal.isNull()) {
				typesMustBeEqual(returnVal.type.val, returnType.val,
					error() + "Return type is " + returnVal.type.toString()
						+ " but method type is " + returnType.toString() + ".");
			}
		}
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
	blockNode(final fieldDeclsNode f, final stmtsNode s, 
			final int line, final int col) {
		super(line, col);
		decls = f;
		stmts = s;
	}

	@Override
	void checkTypes(Types returnType) {
		st.openScope();
		decls.checkTypes();
		stmts.checkTypes(returnType);
		try {
			st.closeScope();
		}
		catch (EmptySTException e) {
			System.out.println( error() + "Failed to close scope. "
										+ "No scope open.");
		}
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

	void checkTypes(SymbolInfoMethod cId) {
		argVal.checkTypes();
		cId.addArgument(argVal.kind, argVal.type);
		moreArgs.checkTypes(cId);
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
	void checkTypes() {
		// no action
	}

	@Override
	void checkTypes(SymbolInfoMethod mId) {
		// no action
	}

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
	protected Types type; // Used for typechecking
	protected Kinds kind; // Used for typechecking
}

class nullExprNode extends exprNode {
	nullExprNode() {
		super();
	}
	boolean   isNull() {return true;}

	@Override
	void checkTypes() {
		// no action
	}

	@Override
	void Unparse(final int indent) {
		// no action
	}
} // class nullExprNode 

class binaryOpNode extends exprNode {
	binaryOpNode(final exprNode e1, final int op, 
			final exprNode e2, final int line, final int col) {
		super(line, col);
		operatorCode = op;
		leftOperand = e1;
		rightOperand = e2;
	}

	static void printOp(final int op) {
		System.out.print(opString(op));
	}

	static public String opString(final int op) {
		switch (op) {
			case sym.COR:
				return " || ";
			case sym.CAND:
				return " && ";
			case sym.LT:
				return " < ";
			case sym.GT:
				return " > ";
			case sym.LEQ:
				return " <= ";
			case sym.GEQ:
				return " >= ";
			case sym.EQ:
				return " == ";
			case sym.NOTEQ:
				return " != ";
			case sym.PLUS:
				return " + ";
			case sym.MINUS:
				return " - ";
			case sym.TIMES:
				return " * ";
			case sym.SLASH:
				return " / ";
			default:
				throw new Error("printOp: case not found");
		}
	}

	@Override
	void checkTypes() {
		// Check types for arithmetic operators
		if(operatorCode == sym.PLUS || operatorCode == sym.MINUS
				|| operatorCode == sym.TIMES || operatorCode == sym.SLASH) {
			leftOperand.checkTypes();
			rightOperand.checkTypes();
			int ltype = leftOperand.type.val;
			int rtype = rightOperand.type.val;
			if(ltype != Types.Integer && ltype != Types.Character) {
				typeMustBe(ltype, Types.Error, 
					error() + "Left operand of" + opString(operatorCode) 
							+ "must be an int or char.");
			}
			if(rtype != Types.Integer && rtype != Types.Character) {
				typeMustBe(rtype, Types.Error, 
					error() + "Right operand of" + opString(operatorCode) 
							+ "must be an int or char.");
			}
			type = new Types(Types.Integer);
		}
		// Check types of logical operators
		else if (operatorCode == sym.COR || operatorCode == sym.CAND) {
			leftOperand.checkTypes();
			rightOperand.checkTypes();
			typeMustBe(leftOperand.type.val, Types.Boolean,
				error() + "Left operand of" + opString(operatorCode)
						+ "must be a bool.");
			typeMustBe(rightOperand.type.val, Types.Boolean,
				error() + "Right operand of" + opString(operatorCode)
						+ "must be a bool.");
			type = new Types(Types.Boolean);
		}
		// Check types for relational operators
		else if (operatorCode == sym.LT || operatorCode == sym.GT
				|| operatorCode == sym.LEQ || operatorCode == sym.GEQ
				|| operatorCode == sym.EQ || operatorCode == sym.NOTEQ) {
			leftOperand.checkTypes();
			rightOperand.checkTypes();
			int ltype = leftOperand.type.val;
			int rtype = rightOperand.type.val;
			if (ltype == Types.Boolean) {
				typeMustBe(rtype, Types.Boolean,
					error() + "Left operand of" + opString(operatorCode)
							+ "is bool but right operand is " 
							+ rightOperand.type.toString());
			}
			if (rtype == Types.Boolean) {
				typeMustBe(ltype, Types.Boolean,
					error() + "Right operand of" + opString(operatorCode)
							+ "is bool but left operand is " 
							+ leftOperand.type.toString());
			}
			/*
			else if (ltype == Types.Integer || ltype == Types.Char) {
				if (rtype == Types.Boolean) {
					typeMustBe(1, 2,
						error() + "Left operand of" + toString(operatorCode)
								+ "is " + ltype.toString()
								+ " but right operand is bool");
				}
			}*/
			kind = new Kinds(Kinds.Value);
			type = new Types(Types.Boolean);
		}
		// Unkown operators
		else {
			mustBe(false);
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
	void checkTypes() {
		operand.checkTypes();
		typeMustBe(operand.type.val, Types.Boolean,
			error() + " unary operation type is " + operand.type.toString()
					+ " but most be boolean.");
		kind = new Kinds(Kinds.Value);
		type = new Types(Types.Boolean);
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
	void checkTypes() {
		operand.checkTypes();
		if (operand.type.val != Types.Integer 
				&& operand.type.val != Types.Character
				&& operand.type.val != Types.Boolean) {
			typeMustBe(operand.type.val, Types.Error,
				error() + " Cast operand is type " + operand.type.toString()
						+ " but must be int, char, or bool.");
		}
		if (resultType.type.val != Types.Integer 
				&& resultType.type.val != Types.Character
				&& resultType.type.val != Types.Boolean) {
			typeMustBe(resultType.type.val, Types.Error,
				error() + " Cast type is " + resultType.type.toString()
						+ " but must be int, char, or bool.");
			type = new Types(Types.Error);
		}
		else {
			type = resultType.type;
		}
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
	fctCallNode(final identNode id, final argsNode a, 
			final int line, final int col) {
		super(line, col);
		methodName = id;
		args = a;
	}

	@Override
	void checkTypes() {
		SymbolInfo id;
		id = (SymbolInfo) st.globalLookup(methodName.idname);
		if (id != null) {
			if (id.kind.val == Kinds.Method) {
				SymbolInfoMethod mId; // method decl
				SymbolInfoMethod cId; // method call
				mId = (SymbolInfoMethod) st.globalLookup(methodName.idname);
				cId = new SymbolInfoMethod(methodName.idname, mId.kind, mId.type);
				args.checkTypes(cId);
				if (mId.kindList.size() == cId.kindList.size()) {
					for (int i = 0; i < mId.kindList.size(); i++) {
						typeMustBe( mId.kindList.get(i).val, 
									cId.kindList.get(i).val, 
									error() + "Argument " + i + " has kind " 
									+ cId.kindList.get(i).toString()
									+ " but matching parameter has kind " 
									+ mId.kindList.get(i).toString() + ".");
						typeMustBe( mId.typeList.get(i).val, 
									cId.typeList.get(i).val, 
									error() + "Argument " + i + " has type " 
									+ cId.typeList.get(i).toString()
									+ " but matching parameter has type " 
									+ mId.typeList.get(i).toString() + ".");
					}
				}
				else {
					System.out.println( error() + "Wrong number of arguments "
												+ "to parameters");
					typeErrors++;
				}
			}
			else {
				System.out.println( error() + "Method not found for function "
                                           + "call.");
				typeErrors++;
			}
			kind = id.kind;
			type = id.type;
		}
		else {
			System.out.println(error() + "Method not found for function call.");
			typeErrors++;
			kind = new Kinds(Kinds.Value);
			type = new Types(Types.Error);
		}
	}

	@Override
	void Unparse(final int indent) {
		methodName.Unparse(0);
		System.out.print("(");
		args.Unparse(0);
		System.out.print(")");
	}

	private final identNode methodName;
	private final argsNode args;
} // class fctCallNode 

class identNode extends exprNode {
	identNode(final String identname, final int line, final int col) {
		super(line, col);
		idname   = identname;
	}

	@Override
	void checkTypes() {
		SymbolInfo id;
		id = (SymbolInfo) st.globalLookup(idname);
		if (id == null) {
			System.out.println(error() + idname + " is not declared.");
			typeErrors++;
			kind = new Kinds(Kinds.Value);
			type = new Types(Types.Error);
		}
		else {
			kind = id.kind;
			type = id.type;
			//idinfo = id; // Save reference to correct symbol table entry
		}
	}

	@Override
	void Unparse(final int indent) {
		System.out.print(idname);
	}

	public final String idname;
} // class identNode 

class nameNode extends exprNode {
	nameNode(final identNode id, final exprNode expr, 
			final int line, final int col) {
		super(line, col);
		varName = id;
		subscriptVal = expr;
	}

	@Override
	void checkTypes() {
		varName.checkTypes();
		// Check array index type
		if (!subscriptVal.isNull()) {
			SymbolInfo id;
			id = (SymbolInfo) st.globalLookup(varName.idname);
			if (id != null) {
				if (id.kind.val != Kinds.Array 
						&& id.kind.val != Kinds.ArrayParm) {
					System.out.println( error() + "Index attempt on a " 
										+ id.kind.toString() + ".");
					typeErrors++;
				}
			}
			subscriptVal.checkTypes();
			if (subscriptVal.type.val != Types.Integer 
					&& subscriptVal.type.val != Types.Character) {
				typeMustBe(subscriptVal.type.val, Types.Error,
					error() + "Array index type is " 
							+ subscriptVal.type.toString()
							+ " but must be Integer or Character.");
			}
		}
		kind = varName.kind;
		type = varName.type;
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

	public final identNode varName;
	private final exprNode subscriptVal;
} // class nameNode 

class intLitNode extends exprNode {
	intLitNode(final int val, final int line, final int col) {
		super(line, col);
		intval = val;
		kind = new Kinds(Kinds.Value);
		type = new Types(Types.Integer);
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
		kind = new Kinds(Kinds.Value);
		type = new Types(Types.Character);
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
		kind = new Kinds(Kinds.Value);
		type = new Types(Types.String);
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
		kind = new Kinds(Kinds.Value);
		type = new Types(Types.Boolean);
	}

	@Override
	void Unparse(final int indent) {
		System.out.print("true");
	}
} // class trueNode 

class falseNode extends exprNode {
	falseNode(final int line, final int col) {
		super(line, col);
		kind = new Kinds(Kinds.Value);
		type = new Types(Types.Boolean);
	}

	@Override
	void Unparse(final int indent) {
		System.out.print("false");
	}
} // class falseNode 

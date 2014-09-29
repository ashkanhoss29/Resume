import java.io.*;

/**
 * cs541 Compiler Design
 * @author Ashkan Hosseini
 *
 * This is a driver for testing the CSX scanner.
 * The input is a CSX file.
 *  This driver will use the scanner to process the input CSX file
 *  and output the tokens withs associated line and column information.
 */
class P2 {

  public static void main(String args[]) throws java.io.IOException {

    // Open test file
    if (args.length != 1) {
      System.out.println(
        "Error: Input file must be named on command line.");
      System.exit(-1);
    }

    java.io.FileInputStream yyin = null;

    try {
      yyin = new java.io.FileInputStream(args[0]);
    }
    catch (FileNotFoundException notFound) {
      System.out.println("Error: unable to open input file.");
      System.exit(-1);
    }

    // lex is a JLex-generated scanner that reads from yyin
    final Yylex lex = new Yylex(yyin);

    System.out.println("Begin test of CSX scanner.");

    Symbol token = lex.yylex();

    // CSX Scanner Test
    while (token.sym != sym.EOF) {

      System.out.print(token.value.linenum + ":"
              + token.value.colnum + " ");

      // Identify the token type and output appropriate token info
      switch (token.sym) {

        case sym.IDENTIFIER:
          System.out.println("\tIdentifier ("
            + ((CSXIdentifierToken) token.value).identifierText + ")");
          break;

        case sym.INTLIT:
          System.out.println("\tInteger literal ("
            + ((CSXIntLitToken) token.value).intValue + ")");
          break;
          
        case sym.STRLIT:
          System.out.println("\tString literal ("
            + ((CSXStringLitToken) token.value).stringText + ")");
          break;

        case sym.CHARLIT:
          System.out.println("\tChar literal ("
            + ((CSXCharLitToken) token.value).charText + ")");
          break;

        case sym.rw_BOOL:
          System.out.println("\tbool");
          break;

        case sym.rw_CHAR:
          System.out.println("\tchar");
          break;

        case sym.rw_CLASS:
          System.out.println("\tclass");
          break;

        case sym.rw_CONST:
          System.out.println("\tconst");
          break;

        case sym.rw_ELSE:
          System.out.println("\telse");
          break;

        case sym.rw_FALSE:
          System.out.println("\tfalse");
          break;

        case sym.rw_IF:
          System.out.println("\tif");
          break;

        case sym.rw_INT:
          System.out.println("\tint");
          break;

        case sym.rw_READ:
          System.out.println("\tread");
          break;

        case sym.rw_RETURN:
          System.out.println("\treturn");
          break;

        case sym.rw_TRUE:
          System.out.println("\ttrue");
          break;

        case sym.rw_VOID:
          System.out.println("\tvoid");
          break;

        case sym.rw_WHILE:
          System.out.println("\twhile");
          break;

        case sym.rw_PRINT:
          System.out.println("\tprint");
          break;

        case sym.LPAREN:
          System.out.println("\t(");
          break;

        case sym.RPAREN:
          System.out.println("\t)");
          break;

        case sym.LBRACKET:
          System.out.println("\t[");
          break;

        case sym.RBRACKET:
          System.out.println("\t]");
          break;

        case sym.ASSIGN:
          System.out.println("\t=");
          break;

        case sym.SEMI:
          System.out.println("\t;");
          break;

        case sym.PLUS:
          System.out.println("\t+");
          break;

        case sym.MINUS:
          System.out.println("\t-");
          break;

        case sym.TIMES:
          System.out.println("\t*");
          break;

        case sym.SLASH:
          System.out.println("\t/");
          break;

        case sym.EQ:
          System.out.println("\t==");
          break;

        case sym.NOTEQ:
          System.out.println("\t!=");
          break;

        case sym.CAND:
          System.out.println("\t&&");
          break;

        case sym.COR:
          System.out.println("\t||");
          break;

        case sym.LT:
          System.out.println("\t<");
          break;

        case sym.GT:
          System.out.println("\t>");
          break;

        case sym.LEQ:
          System.out.println("\t<=");
          break;

        case sym.GEQ:
          System.out.println("\t>=");
          break;

        case sym.COMMA:
          System.out.println("\t,");
          break;

        case sym.NOT:
          System.out.println("\t!");
          break;

        case sym.LBRACE:
          System.out.println("\t{");
          break;

        case sym.RBRACE:
          System.out.println("\t}");
          break;

        case sym.COLON:
          System.out.println("\t:");
          break;

        case sym.ERROR:
          System.out.println("\tInvalid token ("
            + ((CSXInvalidToken) token.value).invalidToken + ")");
          break;

        default:
          System.out.println("unrecognized token type: " + token.value);

      } // switch(token.sym)
      token = lex.yylex(); // get next token

    } // not at EOF
    System.out.println("End test of CSX scanner.");

  } // main()
} // class P2

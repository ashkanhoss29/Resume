import java_cup.runtime.*;
import java.io.*;

/**
 * cs541 Compiler Design
 * @author Ashkan Hosseini
 *
 * FILE NOT CHANGED
 *
 * This class is used to feed the parser tokens generated by the JLEX scanner
 *
 */

class Scanner {
	private	static Yylex lex = null;

	public static void init(final java.io.FileInputStream yyin) {
		if (lex == null) {
			lex = new Yylex(yyin);
		} else {
			System.err.println("Scanner is already initialized.");
		}
	} // init

	public static Symbol next_token() throws IOException {
		if (lex == null) {
			System.err.println("Scanner is not yet initialized.");
			System.exit(-1);
		} else {
			return lex.yylex();
		}
		return null; // To appease javac
	} // next_token

} // class Scanner

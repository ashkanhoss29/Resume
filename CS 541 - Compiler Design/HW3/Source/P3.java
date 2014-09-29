import java.io.*;
import java_cup.runtime.*;

/**
 * cs541 Compiler Design
 * @author Ashkan Hosseini
 *
 * Driver
 * The primary goal of this driver is to link the scanner to the parser 
 *   so that tokens from the input can be fed into the parser. The driver
 *   will have the parser parse the input test CSX program and then call
 *   the Unparse methods of the AST to reconstruct and output the test program
 *
 */

class P3 {

	public static void main(String args[]) throws java.io.IOException {
		if (args.length != 1) {
			System.out.println(
				"Error: Input file must be named on command line." );
			System.exit(-1);
		}
		java.io.FileInputStream yyin = null;
		try {
			yyin = new java.io.FileInputStream(args[0]);
		} catch (FileNotFoundException notFound){
			System.out.println ("Error: unable to open input file.");
			System.exit(-1);
		}
		Scanner.init(yyin); // Initialize Scanner class for parser
		final parser csxParser = new parser();
		Symbol root = null;
		try {
			// root = csxParser.debug_parse(); // do the parse
			root = csxParser.parse(); // do the parse
			System.out.println ("CSX program parsed correctly.");
		} catch (Exception e){
			System.out.println ("Compilation terminated due to syntax errors.");
			e.printStackTrace();
			System.exit(0);
		}
		System.out.println ("Here is its unparsing:");
		((classNode)root.value).Unparse(0);
	} // main

} // class P3

import java.io.*;
import java_cup.runtime.*;

class P5 {

	public static void
	main(String args[]) throws java.io.IOException,  Exception {

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
		parser csxParser = new parser();
		System.out.println ("\n\n" + "CSX compilation of " + args[0]);
		Symbol root=null;
		try {
			root = csxParser.parse(); // do the parse
			System.out.println ("Program parsed correctly.");
		} catch (SyntaxErrorException e){
			System.out.println ("Compilation terminated due to syntax errors.");
			System.exit(0);
		}
		if (!((ASTNode)root.value).isTypeCorrect()) {
			System.out.println("Compilation halted due to type errors.");
			return;
		}
		java.io.PrintStream outFile = null;
		String outFileName = "test.j";
		try {
			outFile = new java.io.PrintStream(
				new java.io.FileOutputStream(outFileName));
		} catch (FileNotFoundException notFound){
			System.out.println ("Error: unable to open output file " + outFileName);
			System.exit(-1);
		}
		if (((ASTNode)root.value).codeGen(outFile)) {
			System.out.println ("Program translated; result is in " + outFileName);
		} else {
			System.out.println ("Error in translating CSX program");
		}
	} // main
} // class P5
	

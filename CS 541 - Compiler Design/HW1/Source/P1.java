import java.util.InputMismatchException;
import java.util.Scanner;

/**
 * cs541 Compiler Design
 * @author Ashkan Hosseini
 * 
 * This is a driver for testing the SymbolTable class. It uses the Scanner class
 * to get input from the user. There are 7 predefined commands:
 *  insert - will ask the user for name and value to be stored into symbol table
 *  open - opens a new scope in the symbol table
 *  close - closes the front scope in the symbol table
 *  lookup - asks for a name to look up in the front scope of the symbol table
 *  global - asks for a name to look up in the symbol table
 *  quit - Ends the program
 */
public class P1
{

  /**
   * @param args the command line arguments 
   */
  public static void main(final String[] args) {

    final SymbolTable symbolTable = new SymbolTable();
    final Scanner scanner = new Scanner(System.in);
    String input;

    // Get input from the user for testing SymbolTable
    while(scanner.hasNext()) {
      input = scanner.next();
      
      // Test insert of symbol into front scope of symbol table
      if("insert".equals(input)) {
        
        // Ask user for name and integer value
        System.out.print("Enter symbol: ");
        final String name = scanner.next();
        final int integer;
        
        try {
          System.out.print("Enter associated integer: ");
          integer = scanner.nextInt();
        }
        catch(InputMismatchException inputMismatchException) {
          System.out.println("The input is not an integer");
          scanner.next(); // flush incorrect input
          continue;
        }

        // Create and attempt insert of given name and integer into symbol table
        try {
          final TestSymb testSymb = new TestSymb(name, integer);
          symbolTable.insert(testSymb);
          System.out.println(testSymb.toString() + " entered into symbol table.");
        }
        catch(DuplicateException duplicateException) {
          System.out.println(name + " already entered into scope");
        }
        catch(EmptySTException emptyStException) {
          System.out.println("There is no scope open");
        }
      }
      // Test open new scope
      else if("open".equals(input)) {
        symbolTable.openScope();
        System.out.println("New scope opened.");
      }
      // Test close of front scope
      else if("close".equals(input)) {
        try {
          symbolTable.closeScope();
          System.out.println("Top scope closed.");
        }
        catch(Exception ex) {
          System.out.println("There is no scope open.");
        }
      }
      // Test lookup in front scope
      else if("lookup".equals(input)) {
        try {
          System.out.print("Enter symbol: ");
          final String name = scanner.next();
          final TestSymb testSymb = (TestSymb) symbolTable.localLookup(name);

          if(testSymb == null) {
            System.out.println(name + " not found in top scope");
          }
          else {
            System.out.println(testSymb.toString() + " found in top scope");
          }
        }
        catch(EmptySTException emptyStException) {
          System.out.println("There is no scope open");
        }
      }
      // Test global lookup in symbol table
      else if("global".equals(input)) {
        System.out.print("Enter symbol: ");
        final String name = scanner.next();
        final TestSymb testSymb = (TestSymb) symbolTable.globalLookup(name);

        if(testSymb == null) {
          System.out.println(name + " not found in symbol table");
        }
        else {
          System.out.println(testSymb.toString() + " found in symbol table");
        }
      }
      // Test dump of symbol table string representation
      else if("dump".equals(input)) {
        symbolTable.dump(System.out);
      }
      // End the program
      else if("quit".equals(input)) {
        break;
      }
      // Tell the user if a command is not found
      else {
        System.out.println("Command not found");
      }
    }

    System.out.println("Testing done");
  }
}

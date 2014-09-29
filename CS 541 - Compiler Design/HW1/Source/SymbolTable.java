import java.io.*;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

/**
 * cs541 Compiler Design
 * @author Ashkan Hosseini
 * 
 * A block-structured implementation of a symbol table.
 */
public class SymbolTable
{

  /**
   * A linked list of Maps. Each Map in the list represents a new SymbolTable
   * scope. The implementation is specified here (rather than interface) to
   * avoid constant casting
   */
  // private final LinkedList<Map> scopes_;
// grader: Map is not a complete type.  I fixed here and elsewhere. -2
  private final LinkedList<Map<String, Symbol>> scopes_;
// grader: why the underscore?  It hurts readability.  -1

  /**
   * Initialize SymbolTable to contain a single scope that is initially empty.
   */
  SymbolTable() {
    // scopes_ = new LinkedList<Map>();
    scopes_ = new LinkedList<Map<String, Symbol>>();
    openScope();
  }

  /**
   * Add a new, initially empty scope to the list of scopes contained in this
   * SymbolTable.
   * Note that this is made final to allow safe call from constructor.
   */
  public final void openScope() {
    final Map<String, Symbol> myMap = new HashMap<String, Symbol>();
    scopes_.add(myMap);
  }

  /**
   * Remove the current (front) scope from the list of scopes contained in this
   * SymbolTable.
   *
   * @throws EmptySTException if the list of scopes is empty
   */
  public void closeScope() 
          throws EmptySTException {
    if(scopes_.isEmpty()) {
      throw new EmptySTException();
    }
    
    scopes_.removeLast();
  }

  /**
   * Inserts the specified Symbol element into the current (front) scope. 
   * If the current (front) scope contains a Symbol whose name is the same 
   * as that of symbol, throw a DuplicateException. If the list of scopes in
   * this SymbolTable is empty, throw an EmptySTException.
   *
   * @param symbol the element to insert into the current scope
   * @throws DuplicateException if the current scope contains symbol
   * @throws EmptySTException if the list of scopes is empty
   */
  public void insert(final Symbol symbol) 
          throws DuplicateException, EmptySTException {
    if(scopes_.isEmpty()) {
      throw new EmptySTException();
    }
    
    final Map<String, Symbol> currentScope = scopes_.getLast();
    if(currentScope.containsKey(symbol.name())) {
      throw new DuplicateException();
    }
    currentScope.put(symbol.name(), symbol);
  }

  /**
   * If the current (front) scope contains a Symbol whose name matches
   * symbolName (ignoring case) return that Symbol, otherwise return null. 
   * If the list of scopes is empty, return null.
   *
   * @param symbolName the associated name of the Symbol to look up
   * @return the associated Symbol whose name matches symbolName in the 
   * current scope or null if the current scope does not contain a Symbol 
   * associated with symbolName or if the list of scopes is empty
   * @throws EmptySTException if the list of scopes is empty
   */
  public Symbol localLookup(final String symbolName) 
          throws EmptySTException {
    if(scopes_.isEmpty()) {
      throw new EmptySTException();
    }
    
    final Map<String, Symbol> currentScope = scopes_.getLast();
    return currentScope.get(symbolName.toLowerCase(Locale.ENGLISH));
  }

  /**
   * If any scope contains a Symbol whose name matches symbolName 
   * (ignoring case) return that Symbol (in the scope nearest to the front
   * of the list), otherwise return null. 
   * If the list of scopes is empty, return null.
   *
   * @param symbolName the associated name of the Symbol to look up
   * @return the associated Symbol whose name is symbolName in the nearest 
   * scope or null if the symbol table does not contain a Symbol associated
   * with symbolName
   */
  public Symbol globalLookup(final String symbolName) {
    // We need to look through the scopes from front to back, so we use Iterator
    //  and call the descendingIterator method to obtain front to back mapping.
    // final Iterator<Map> iterator = scopes_.descendingIterator();
    final Iterator<Map<String, Symbol>> iterator = scopes_.descendingIterator();
    while(iterator.hasNext()) {
      final Map<String, Symbol> scope = iterator.next();
      final Set<String> names = scope.keySet();

      if(names.contains(symbolName.toLowerCase(Locale.ENGLISH))) {
        return scope.get(symbolName.toLowerCase(Locale.ENGLISH));
      }
    } // for each scope in order from front to back

    return null; // Symbol not found, return null
  }

  /**
   * @return the string representation of this symbol table
   */
  @Override
  public String toString() {
    final StringBuilder symbolTableString = new StringBuilder();

    // Iterate each scope in linked list from front to back.
    // Because we need to traverse the scopes from front to back, the
    //  descendingIterator() method is called to get an iterator with
    //  reverse mapping.
    // final Iterator<Map> iterator = scopes_.descendingIterator();
    final Iterator<Map<String, Symbol>> iterator = scopes_.descendingIterator();
// grader: instead of iterator say: for (Map<String, Symbol> aScope : scopes) ...  -0
    while(iterator.hasNext()) {
      symbolTableString.append('{');
      final Map<String, Symbol> scope = iterator.next();
      final Set<String> nameSet = scope.keySet();
      final StringBuilder scopeString = new StringBuilder();
      
      // Iterate the names of the scope
      for(String name : nameSet) {
        final Symbol symbol = scope.get(name);
        if(scopeString.length() > 0) {
          scopeString.append(", ");
        } // add separator if not first element
        scopeString.append(name);
        scopeString.append('=');
        scopeString.append(symbol.toString());
      } // for each name in the scope

      symbolTableString.append(scopeString);
      symbolTableString.append("}\n");
    } // for each scope in order of front to back

    return symbolTableString.toString();
  }

  /**
   * The contents of this SymbolTable are written to printStream. 
   * For debugging purposes.
   *
   * @param printStream the PrintStream to write to
   */
  void dump(final PrintStream printStream) {
    printStream.print(toString());
// grader: or: printStream.print(this).  automatically calls toString().  -0
  }
}

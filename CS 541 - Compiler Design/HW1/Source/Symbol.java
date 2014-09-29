import java.util.Locale;

/**
 * cs541 Compiler Design
 * @author Ashkan Hosseini
 * 
 * Symbol implementation used in SymbolTable class
 */
public class Symbol
{
  private final String name_;
  
  /**
   * Initializes name_ to the passed in value
   *
   * @param name the name of this Symbol
   */
  Symbol(final String name) {
    name_ = name.toLowerCase(Locale.ENGLISH);
  }

  /**
   * @return the name of this Symbol
   */
  public String name() {
    return name_;
  }

  /**
   * @param name the string representation of this Symbol
   */
  @Override
  public String toString() {
    return name_;
  }
}

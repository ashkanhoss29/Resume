/** 
 * cs541 Compiler Design
 * @author Ashkan Hosseini
 * 
 * Extends Symbol by providing an associated integer value alongside the name.
 * Used for testing purposes.
 */
public class TestSymb extends Symbol
{
  private final int value_;

  /**
   * Initializes name_ and value_ to the specified parameters
   *
   * @param name the name to set for this TestSymb
   * @param value the value to set for this TestSymb
   */
  TestSymb(final String name, final int value) {
    super(name);
    value_ = value;
  }

  /**
   * @return the value of this TestSymb
   */
  public int value() {
    return value_;
  }

  /**
   * @return the string representation for this TestSymb
   */
  @Override
  public String toString() {
    return "(" + name() + ":" + value_ + ")";
  }
}

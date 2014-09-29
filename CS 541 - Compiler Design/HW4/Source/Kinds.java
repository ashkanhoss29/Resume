/**
 * cs541 Compiler Design
 * @author Ashkan Hosseini
 * 
 * Contains allowable Kinds in CSX language.
 * Currently used in ast.java for type checking.
 *
 */

class Kinds{
 public static final int Var = 0;
 public static final int Value = 1;
 public static final int Array = 2;
 public static final int ScalarParm = 3;
 public static final int ArrayParm = 4;
 public static final int Method = 5;
 public static final int Other = 2;

 Kinds(int i){val = i;}
 Kinds(){val = Other;}

 public String toString() {
        switch(val){
          case 0: return "Var";
          case 1: return "Value";
          case 2: return "Array";
          case 3: return "ScalarParm";
          case 4: return "ArrayParm";
          case 5: return "Method";
          //case 2: return "Other";
          default: throw new RuntimeException();
        }
 }

 int val;
}

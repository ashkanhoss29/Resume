/**************************************************
*  class used to hold information associated w/
*  Symbs (which are stored in SymbolTables)
*
****************************************************/

class SymbolInfo extends Symb {
	public Kinds kind;
	public Types type;
	public int varIndex;

	public SymbolInfo(String id, Kinds k, Types t){
		super(id);
		kind = k; type = t;
	}

	public SymbolInfo(String id, int k, int t){
		super(id);
		kind = new Kinds(k); type = new Types(t);
	}

	public String genJasminArg() {
		// Override in subclass
		throw new Error();
	}

	public String toString(){
		return "("+name()+": kind=" + kind+ ", type="+  type+")";
	}
}


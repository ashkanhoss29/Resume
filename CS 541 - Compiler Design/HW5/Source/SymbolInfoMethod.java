/**
 * cs541 Compiler Design
 * @author Ashkan Hosseini
 * 
 * Extends SymbolInfo class to contain method
 *   parameter listing. This is used by the 
 *   type checker to check for matching method
 *   calls.
 *
 */

import java.util.ArrayList;

class SymbolInfoMethod extends SymbolInfo {
	public ArrayList<Kinds> kindList;
	public ArrayList<Types> typeList;

	public SymbolInfoMethod(String id, Kinds k, Types t) {
		super(id, k, t);
		kindList = new ArrayList<Kinds>();
		typeList = new ArrayList<Types>();
	}

	public SymbolInfoMethod(String id, int k, int t) {
		super(id, k, t);
		kindList = new ArrayList<Kinds>();
		typeList = new ArrayList<Types>();
	}

	public void addArgument(Kinds k, Types t) {
		kindList.add(k);
		typeList.add(t);
	}

	@Override
	public String genJasminArg() {
		String result = "(";
		for(int i = 0; i < typeList.size(); i++) {
			if (kindList.get(i).val == Kinds.Array 
					|| kindList.get(i).val == Kinds.ArrayParm) {
				result = result + "[";
			}
			result = result + typeList.get(i).getDescriptor();
		}
		result = result + ")";
		return result;
	}

	@Override
	public String toString() {
		return "(" + name() + " : kind=" + kind + ", type=" + type + ")";
	}
}

// This class contains library routines used by CSX program

public class CSXLib{
	private static java.io.PushbackInputStream in =
		new java.io.PushbackInputStream(System.in);
	private static char getChar(){
		int c;
		try {
			c=  in.read();
			if (c == -1)
				throw new java.lang.RuntimeException();
		} catch (java.io.IOException e){
			throw new java.lang.RuntimeException();
		}
		return (char) c;
	}
	private static void ungetChar(char c){
		try {
			in.unread(c);
		} catch (java.io.IOException e){
			throw new java.lang.RuntimeException();
		}
	}
	public static int readInt(){
		char c=  getChar();
		double val = 0;
		int sign = 1;
		do {
			while ( c != '-' && c != '~' &&
				!java.lang.Character.isDigit(c)){
				c=  getChar();
			}
			if ( c == '-' || c == '~'){
				sign = -1;
				c=  getChar();
			}
		} while (!java.lang.Character.isDigit(c));

		while ( java.lang.Character.isDigit(c)){
			val=10*val+(c-'0');
			c=  getChar();
		}
		ungetChar(c);
		val = val*sign;
		if (val > Integer.MAX_VALUE)
			return Integer.MAX_VALUE;
		else if (val < Integer.MIN_VALUE)
			return Integer.MIN_VALUE;
		else return (int) val;
	}

	public static char readChar(){
		return  getChar();
	}

	public static void printString(String s){
		System.out.print(s);
		System.out.flush();
	}

	public static void printChar(char c){
		System.out.print(c);
		System.out.flush();
	}

	public static void printInt(int i){
		System.out.print(i);
		System.out.flush();
	}

	public static void printBool(boolean b){
// grader: you added printBool; you shouldn't have needed it.  -1
		System.out.print(b);
		System.out.flush();
	}

	public static void printIBool(boolean b){
		System.out.print(!b);
		System.out.flush();
	}

	public static void printCharArray(char c[]){
		for (int i=0;i<c.length;i++) {
			System.out.print(c[i]);
			System.out.flush();
		}
	}

	public static int[] cloneIntArray(int i[]){
		return i.clone();
	}

	public static char[] cloneCharArray(char i[]){
		return i.clone();
	}

	public static boolean[] cloneBoolArray(boolean i[]){
		return i.clone();
	}

	public static char[] convertString(String s){
		return  s.toCharArray();
	}


	public static int[] checkIntArrayLength(int[] target, int source[]){
		if (target.length != source.length)
			throw new ArraySizeException();
		else return source;
	}

	public static char[] checkCharArrayLength(char[] target, char source[]){
		if (target.length != source.length)
			throw new ArraySizeException();
		else return source;
	}

	public static boolean[] checkBoolArrayLength(boolean[] target,
						boolean source[]){
		if (target.length != source.length)
			throw new ArraySizeException();
		else return source;
	}


	public static void main(){
		int a[] = new int[100];
		int z[] = new int[100];
		a[1] = 1;
		a[2] = 123;
		int b[] = checkIntArrayLength(z,cloneIntArray(a));
		b[1]=-1;
		printInt(a[1]);
		printChar(' ');
		printInt(b[1]);
		printChar(' ');
		printInt(a[2]);
		printChar(' ');
		printInt(b[2]);
		printChar('\n');
		char aa[] = new char[100];
		char zz[] = new char[100];
		aa[1] = 'a';
		aa[2] = 'A';
		char bb[] = checkCharArrayLength(zz,cloneCharArray(aa));
		bb[1]='z';
		printChar(aa[1]);
		printChar(' ');
		printChar(bb[1]);
		printChar(' ');
		printChar(aa[2]);
		printChar(' ');
		printChar(bb[2]);
		printChar('\n');
		boolean aaa[] = new boolean[100];
		boolean zzz[] = new boolean[100];
		aaa[1] = true;
		aaa[2] = true;
		boolean bbb[] = checkBoolArrayLength(zzz,cloneBoolArray(aaa));
		bbb[1]=false;
		printBool(aaa[1]);
		printChar(' ');
		printBool(bbb[1]);
		printChar(' ');
		printBool(aaa[2]);
		printChar(' ');
		printBool(bbb[2]);
		printChar('\n');
		char qq[] = convertString("AbCdE");
		printChar(qq[0]);
		printChar(' ');
		printChar(qq[1]);
		printChar(' ');
		printChar(qq[2]);
		printChar(' ');
		printChar(qq[3]);
		printChar('\n');
	}

	public static void main(String args[]){
		main();
	}

}

class ArraySizeException extends RuntimeException {
	static final long serialVersionUID = 1L;
	// no other content
}

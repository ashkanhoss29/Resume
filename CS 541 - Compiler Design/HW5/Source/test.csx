class program {

	int a = 5;
	char c = 'a';
	int b;

	void hello(int e) {
		print("\nhello hello ", e);
		return;
	}

	int mult(int f, int g) {
		return f * g;
	}

	int echo(int i) {
		return i;
	}

	void main() {
		int b;
		a  = 123;
		b  = 456;

		//if (a == 0) {
			//print(4);
		//}

		print(a == 123, '\n', false);

		print("\nhello compiler");
	
		while (a == 123) {
			print('\n', 'a', '\n');
			a = a + 1;
		}

		//read(c);
		//print(c, '\n', b);

		//print('\n', mult(3, 5), " ", a);
		//hello(3);

		if (122 >= 123) {
			print("Success");
		}

//		a= a+a;
//		print( a+b-100);
		return;
	}


}

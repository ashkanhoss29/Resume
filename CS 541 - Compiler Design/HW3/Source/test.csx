##
 # cs541 Compiler Design
 # @authoer Ashkan Hosseini
 #
 # This is a test CSX program used for testing the CSX parser
 #
##

class Hello {
    int var = 5;
	const var2 = 10;
	void hello () {
		return;
	}

	int test (int input, int input2) {
		char myChar = 'a';
		bool myBool;
		myBool = true;
		print(input + "output");
		read(one, two);
		return input;
	}
	## testing
		comments
	##
	char test2 (char input, int input2, bool input3) {
		bool varArray[10]; // this is a comment!
		varArray[0] = false;
		test (input2);
		if(input2 > 1) {
			while(input2 < 10) {
				input2 = input2 + 1 * 2;
			}
			if(input2 || (input && input3))
				return 'b';
		}
		else {
			return 'w';
		}
		return '\t';
	}

	bool test3() {
		if(1 < 2 || 3 > 4 || 5 <= 6 || 7 >= 8 || 9 == 10 || 11 != 12) {
			hello();
			test (one, two);
		}
	};
}

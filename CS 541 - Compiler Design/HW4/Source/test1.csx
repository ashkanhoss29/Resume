class main {
	int var1;
	int var2 = 5;
	char var3;

	int hello() {
		return 5;
	}

	void test(int input, char inArray[]) {
		int member = input;

		if(5 < 1 || true) {
			member = var1 + 1;
		}

		while(member == var1 && 'a' < 5) {
			var2 = hello() + member + (int) inArray[5];
		}

		var3 = (char) var2;

		test(member, inArray);
	
		return;
	}

}

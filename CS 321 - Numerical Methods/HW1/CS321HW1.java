// Name: Ashkan Hosseini
// Date: Spring 2013

package cs321.hw1;

public class CS321HW1
{
	public static void main(String[] args)
	{
		computeEpsilon();
		
		/*
		computeExponential(0);
		computeExponential(1);
		computeExponential(-1);
		computeExponential(0.5);
		computeExponential(-0.123);
		computeExponential(-25.5);
		computeExponential(-1776);
		computeExponential(3.14159);
		*/
	}
	
	
	public static void computeEpsilon()
	{
		double epsilon = 1.0 / 2.0;

		while((1.0 + epsilon) != 1.0)
		{
			epsilon = epsilon / 2.0;
		}
		
		System.out.println("Epsilon: " + epsilon);
	}
	
	
	public static void computeExponential(double x)
	{
		// Check for negative x
		boolean negative = false;
		if(x < 0.0){
			// If x is negative, make positive
			x = x * -1.0;
			negative = true;
		}
		
		// Compute using taylor expansion
		double result = 1.0 + x;
		int n = 2;
		while(n <= 25){
			double nFactorial = 1.0;
			for(int i = 2; i <= n; i++){
				nFactorial = nFactorial * (double)i;
			}
			
			double term = Math.pow(x, (double)n)/nFactorial;
			
			if((!negative && term < 0.0000001) || (negative && term > 1000000)){
				break;
			}
			
			result = result + term;
			
			n++;
		}
		
		// If x was negative take reciprocal of result and make x negative again
		if(negative){
			result = 1.0 / result;
			x = x * -1.0;
		}
		
		// Compute errors
		double computerResult = Math.pow(Math.E, x);
		double absoluteError = Math.abs(result - computerResult);
		double relativeError = absoluteError / Math.abs(computerResult);
		
		// Output results
		System.out.println("X: " + x);
		System.out.println("Result: " + result);
		System.out.println("Computer Result: " + computerResult);
		System.out.println("Absolute Error: " + absoluteError);
		System.out.println("Relative Error: " + relativeError + "\n");
	}
}


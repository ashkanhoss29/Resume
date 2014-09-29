package cs321.hw5;

/*
 * Iterative methods for solving linear system of equations
 * 
 * ie. For a given system Ax = b, find x
 * 
 * Main method contains driver for testing and output of the different methods
 *  with the same numbers in order to see the different results of each
 *  approximation.
 */

public class CS321HW5 {

	public static void main(String[] args) {
		
		double e = 0.0001;
		
		double a[][] = new double[][]
		{
			{7, 1, -1, 2},
			{1, 8, 0, -2},
			{-1, 0, 4, -1},
			{2, -2, -1, 6},
		};
		
		double b[] = {3, -5, 4, -3};
		
		Jacobi(a, b, e);
		Gauss_Seidel(a, b, e);
		SOR(a, b, e);
	}
	
  private static void Jacobi(double a[][], double b[], double e) {
		
		double x[] = {2,2,2,2};
		int c[] = {0, 0, 0, 0};
		
		int k = 0;
		while (k < 100) {
			
			if((c[0] + c[1] + c[2] + c[3]) == 4) {
				break;
			}
			
			for(int i = 0; i < a.length; i++) {
				
				double sum = b[i];
				double diag = a[i][i];
				for(int j = 0; j < a[i].length; j++) {
					
					if(i != j) {
						sum = sum - a[i][j]*x[j];
					}
				}
				
				if (c[i] == 0) {
					double newX = sum / diag;
					if (Math.abs(newX - x[i]) >= e) {
						x[i] = newX;
					}
					else {
						double round = x[i] * 10000;
						round = Math.round(round);
						x[i] = round / 10000;
						c[i] = 1;
					}
				}
			}
			
			k++;
		}
		
		System.out.println(
						"k: " + k + " | " + x[0] + " " + x[1] + " " + x[2] + " " + x[3]);
	}
  
	private static void SOR(double a[][], double b[], double e) {
		
		double x[] = {400,400,400,400};
		int c[] = {0, 0, 0, 0};
		
		double w = 1.1;
		
		int k = 0;
		while (k < 100) {
			
			if((c[0] + c[1] + c[2] + c[3]) == 4) {
				break;
			}
			
			for(int i = 0; i < a.length; i++) {
				
				double sum = b[i];
				double diag = a[i][i];
				
				for(int j = 0; j < i-1; j++) {
					sum = sum - a[i][j]*x[j];
				}
				for(int j = i+1; j < a.length; j++) {
					sum = sum - a[i][j]*x[j];
				}
				
				if (c[i] == 0) {
					double newX = sum / diag;
					newX = w*newX + (1-w)*x[i];
					if (Math.abs(newX - x[i]) >= e) {
						x[i] = newX;
					}
					else {
						double round = x[i] * 10000;
						round = Math.round(round);
						x[i] = round / 10000;
						c[i] = 1;
					}
				}
			}
			
			k++;
		}
		
		System.out.println(
						"k: " + k + " | " + x[0] + " " + x[1] + " " + x[2] + " " + x[3]);
	}
	
	private static void Gauss_Seidel(double a[][], double b[], double e) {
		
		double x[] = {2,2,2,2};
		int c[] = {0, 0, 0, 0};
		
		int k = 0;
		while (k < 100) {
			
			if((c[0] + c[1] + c[2] + c[3]) == 4) {
				break;
			}
			
			for(int i = 0; i < a.length; i++) {
				
				double sum = b[i];
				double diag = a[i][i];
				
				for(int j = 0; j < i-1; j++) {
					sum = sum - a[i][j]*x[j];
				}
				for(int j = i+1; j < a.length; j++) {
					sum = sum - a[i][j]*x[j];
				}
				
				if (c[i] == 0) {
					double newX = sum / diag;
					if (Math.abs(newX - x[i]) >= e) {
						x[i] = newX;
					}
					else {
						double round = x[i] * 10000;
						round = Math.round(round);
						x[i] = round / 10000;
						c[i] = 1;
					}
				}
			}
			
			k++;
		}
		
		System.out.println(
						"k: " + k + " | " + x[0] + " " + x[1] + " " + x[2] + " " + x[3]);
	}
}

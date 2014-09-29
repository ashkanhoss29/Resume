package bouncingball;

/**
 * @author Ashkan
 */
public class BouncingBall
{
	public static void main(String[] args)
	{
		Thread thread1 = new Thread(new BallThread());
		thread1.start();
	}
}

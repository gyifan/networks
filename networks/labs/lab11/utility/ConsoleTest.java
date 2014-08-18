/**
 * ConsoleTest: Demonstrate how Java console can turn off
 * the input-echo feature. <p>
 *
 * The program generates a random number and asks the
 * user re-type the generated number with echo-off until
 * the number is correctly entered.<p>
 *
 * For CSCI 363 lab work.<p>
 *
 * @author Xiannong Meng
 * @date 2013-04-22
 *
 */
import java.io.BufferedReader;
import java.io.Console;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Random;

public class ConsoleTest  {   

    public static void main(String[] args)     {   

        Console console = System.console();
	Random generator = new Random( 1234 ); // the parameter is a seed
	int r, number;

	do {
	    r = generator.nextInt(Integer.MAX_VALUE) % 1000000;
	    System.out.println("Random number generated : " + r);

	    console.printf("Please type the number seen above: ");

	    // the readPassword() method turns off the input echo
	    char[] charsTyped = console.readPassword();
	    String numString = new String(charsTyped);
	    number = Integer.parseInt(numString);

	    if (number == r)
		System.out.println("Great!");
	    else
		System.out.println("Number typed is incorrect, try again.");
	} while (number != r);
    }  // end of main()
}  // end of class

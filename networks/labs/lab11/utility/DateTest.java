/**
 * DateTest: Demonstrate how Java Date class works.<p>
 *
 * For CSCI 363 lab work.<p>
 *
 * @author Xiannong Meng
 * @date 2013-04-22
 *
 */
import java.util.Date;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

public class DateTest {

    public static void main(String[] args) {
	
	Date date = new Date();
	DateFormat df = new SimpleDateFormat("MM/dd/yyyy");
	String formattedDate = df.format(date);

	System.out.println("Date in mm/dd/yyyy format: ");
	System.out.println(formattedDate);

	System.out.println("Date in calendar format : ");
	System.out.println(date);
	
    }
}

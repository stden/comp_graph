package datetime;

import org.junit.Assert;
import org.junit.Test;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;

import static java.lang.System.out;

//
// Все возможности форматирования даты:
// https://docs.oracle.com/javase/7/docs/api/java/text/SimpleDateFormat.html
public class A_DateTest extends Assert {

    @SuppressWarnings("deprecation") // Класс Date deprecated
    @Test
    public void testDate() throws ParseException {
        //
        // Форматирование даты:
        // * yyyy - год 4 цифры
        // * MM - месяц 2 цифры
        // * dd - день в месяце 2 цифры
        // * HH - час 2 цифры
        // * mm - минута 2 цифры
        // * ss - секунда 2 цифры
        //-->
        Date date = new Date();
        out.println("date = " + date);
        SimpleDateFormat dateFormat =
                new SimpleDateFormat("'Дата и время:' dd.MM.yyyy HH:mm:ss");
        dateFormat.setTimeZone(java.util.TimeZone.getTimeZone("UTC"));
        out.println(dateFormat.format(date));

        SimpleDateFormat russianDate = new SimpleDateFormat("dd.MM.yyyy");
        Date date2 = russianDate.parse("11.10.2014");
        out.println(date2);
        // A year y is represented by the integer y - 1900
        assertEquals("Год начиная с 1900", 114, date2.getYear());
        // A month is represented by an integer from 0 to 11; 0 is January, 1 is February,
        // and so forth; thus 11 is December
        assertEquals("Номер месяца - 1", 9, date2.getMonth());
        // A date (day of month) is represented by an integer from 1 to 31 in the usual manner.
        assertEquals("День месяца", 11, date2.getDate());
        //
        assertEquals("День недели", 6, date2.getDay());

        out.println(dateFormat.format(date2));

        // Изменяю время, ставлю 10 часов
        date.setHours(10);

        // Разбор дата + время
        Date result = dateFormat.parse("Дата и время: 17.01.2017 01:02:03");
        Calendar cal = Calendar.getInstance(java.util.TimeZone.getTimeZone("UTC"), Locale.forLanguageTag("RU"));
        cal.setTime(result);
        assertEquals("2017 год", 2017, cal.get(Calendar.YEAR));
        assertEquals("Январь", Calendar.JANUARY, cal.get(Calendar.MONTH));
        assertEquals("17-ое число", 17, cal.get(Calendar.DAY_OF_MONTH));
        // WEEK_OF_MONTH зависит от локали и первого дня недели (в разных JDK может быть 3 или 4)
        int weekOfMonth = cal.get(Calendar.WEEK_OF_MONTH);
        assertTrue("Неделя месяца должна быть 3 или 4", weekOfMonth == 3 || weekOfMonth == 4);
        assertEquals("Вторник", Calendar.TUESDAY, cal.get(Calendar.DAY_OF_WEEK));
        assertEquals(3, cal.get(Calendar.DAY_OF_WEEK_IN_MONTH));
        assertEquals(17, cal.get(Calendar.DAY_OF_YEAR));
        // WEEK_OF_YEAR зависит от локали и первого дня недели (в разных JDK может быть 3 или 4)
        int weekOfYear = cal.get(Calendar.WEEK_OF_YEAR);
        assertTrue("Неделя года должна быть 3 или 4", weekOfYear == 3 || weekOfYear == 4);
        assertEquals("01:02:03", 1, cal.get(Calendar.HOUR_OF_DAY));
        assertEquals("01:02:03", 2, cal.get(Calendar.MINUTE));
        assertEquals("01:02:03", 3, cal.get(Calendar.SECOND));
        //<--
    }
}

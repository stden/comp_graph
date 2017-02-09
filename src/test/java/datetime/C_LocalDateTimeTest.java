package datetime;


import org.junit.Assert;
import org.junit.Test;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.time.*;
import java.time.chrono.Era;
import java.time.format.DateTimeFormatter;
import java.time.format.FormatStyle;
import java.time.format.TextStyle;
import java.util.Date;
import java.util.Locale;

import static java.lang.System.out;

public class C_LocalDateTimeTest extends Assert {
    private static DateTimeFormatter dateFormat =
            DateTimeFormatter.ofPattern("'Дата и время:' dd.MM.yyyy HH:mm:ss G");
    private static DateTimeFormatter dateFormat2 =
            DateTimeFormatter.ofPattern("'Date & time:' dd.MM.yyyy HH:mm:ss G", Locale.US);
    private static SimpleDateFormat russianDate = new SimpleDateFormat("dd.MM.yyyy");

    @Test
    public void testDate() {
        //-->
        LocalDate nowDate = LocalDate.now();
        LocalDateTime now = LocalDateTime.now();
        out.println(dateFormat.format(now));
        System.out.println("День месяца: " + now.getDayOfMonth());
        System.out.println("День недели: " + now.getDayOfWeek().getDisplayName(TextStyle.FULL, Locale.forLanguageTag("RU")));
        out.println("Через 10 дней: " + dateFormat.format(now.plusDays(10)));
        out.println(now.minusMonths(4).plusDays(3));

        out.println("Через 3 месяца и 10 дней и 2 часа: " +
                dateFormat.format(now.plusMonths(3).plusDays(10).plusHours(2)));

        out.println("Верхний предел: " + dateFormat.format(now.plusYears(100000)));
        out.println("Нижний предел: " + dateFormat.format(now.minusYears(100000)));

        System.out.println("US: " + dateFormat2.format(now.minusYears(100000)));

        // Печать даты
        DateTimeFormatter russianDate = DateTimeFormatter.ofPattern("dd.MM.yyyy");
        out.println(russianDate.format(now));

        LocalDate date = LocalDate.of(2013, 5, 3);
        assertEquals("Год", 2013, date.getYear());
        assertEquals("Месяц: май", Month.MAY, date.getMonth());
        assertEquals("3 мая", 3, date.getDayOfMonth());
        assertEquals(DayOfWeek.FRIDAY, date.getDayOfWeek());

        out.println(russianDate.format(date));
        //<--
    }

    @Test
    public void testParseDate() {
        DateTimeFormatter germanFormatter = DateTimeFormatter.ofLocalizedDate(FormatStyle.MEDIUM).withLocale(Locale.GERMAN);

        LocalDate xmas = LocalDate.parse("24.12.2014", germanFormatter);
        Era era = xmas.getEra();
        assertEquals("Anno Domini", era.getDisplayName(TextStyle.FULL, Locale.US));
        assertEquals("AD", era.getDisplayName(TextStyle.SHORT, Locale.US));
        assertEquals("A", era.getDisplayName(TextStyle.NARROW, Locale.US));
        assertEquals("н.э.", era.getDisplayName(TextStyle.FULL, Locale.forLanguageTag("RU")));
        assertEquals("Год", 2014, xmas.getYear());
        assertEquals("Месяц: декабрь", Month.DECEMBER, xmas.getMonth());
        assertEquals("24 декабря", 24, xmas.getDayOfMonth());
        assertEquals("Среда", DayOfWeek.WEDNESDAY, xmas.getDayOfWeek());
        System.out.println(xmas);

        // Много лет назад...
        LocalDate longTimeAgo = xmas.minusYears(5000);
        Era e = longTimeAgo.getEra();
        assertEquals("Before Christ", e.getDisplayName(TextStyle.FULL, Locale.US));
        assertEquals("BC", e.getDisplayName(TextStyle.SHORT, Locale.US));
        assertEquals("B", e.getDisplayName(TextStyle.NARROW, Locale.US));
        assertEquals("до н.э.", e.getDisplayName(TextStyle.FULL, Locale.forLanguageTag("RU")));
        assertEquals("Год", -2986, longTimeAgo.getYear());
        assertEquals("Месяц: декабрь", Month.DECEMBER, longTimeAgo.getMonth());
        assertEquals("24 декабря", 24, longTimeAgo.getDayOfMonth());
        assertEquals("Суббота", DayOfWeek.SATURDAY, longTimeAgo.getDayOfWeek());

    }

    @Test
    public void testParseRussianDate() throws ParseException {
        Date date = russianDate.parse("17.01.2017");
        LocalDate ld = date.toInstant().atZone(ZoneId.systemDefault()).toLocalDate();
        assertEquals("Год", 2017, ld.getYear());
        assertEquals("Месяц: январь", Month.JANUARY, ld.getMonth());
        assertEquals("17 января", 17, ld.getDayOfMonth());
        assertEquals("Вторник", DayOfWeek.TUESDAY, ld.getDayOfWeek());
    }

    @Test
    public void testTime() {
        LocalDate d = LocalDate.of(2015, 5, 30);
        assertEquals("Год", 2015, d.getYear());
        assertEquals("Месяц", 5, d.getMonth().getValue());
        assertEquals("День", 30, d.getDayOfMonth());

        assertEquals("День", 30, d.getDayOfMonth());
        assertEquals("День недели", DayOfWeek.SATURDAY, d.getDayOfWeek());
        assertEquals("День в году", 150, d.getDayOfYear());
        assertEquals("30.05.2015", russianDate.format(java.sql.Date.valueOf(d)));
    }
}

class birthDays {

public enum days {
    // Defined with properties - which day of the week and whether a weekday or not
    MONDAY (2, true),
    TUESDAY (3, true),
    WEDNESDAY (4, true),
    THURSDAY (5, true),
    FRIDAY (6, true),
    SATURDAY (7, false),
    SUNDAY (1, false);
    
    final int birthday;
    final boolean weekday;
    
    days(int bday, boolean wkday) {
        birthday = bday;
        weekday = wkday;
    }
    
    public int getBirthday() {
        return birthday;
    }
    
    public boolean getWeekday() {
        return weekday;
    }
}

   public birthDays(String args[]) {
      days d;
      if (args[0].equals("Sunday")) 
         d = days.SUNDAY;
      else if (args[0].equals("Monday"))
         d = days.MONDAY;
      else if (args[0].equals("Tuesday"))
         d = days.TUESDAY;
      else if (args[0].equals("Wednesday"))
         d = days.WEDNESDAY;
      else if (args[0].equals("Thursday"))
         d = days.THURSDAY;
      else if (args[0].equals("Friday"))
         d = days.FRIDAY;
      else
         d = days.SATURDAY;
         
      System.out.printf("My birthday is on a %s: the %d day of the week, which is a weekday: %b%n", d, d.getBirthday(), d.getWeekday());
   }

   public static void main(String args[]) {
      new birthDays(args);
   }
}

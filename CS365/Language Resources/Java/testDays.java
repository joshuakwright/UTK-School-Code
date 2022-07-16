class testDays {
   public testDays(String args[]) {
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
      new testDays(args);
   }
}

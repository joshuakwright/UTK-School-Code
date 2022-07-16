public class enumTest2 {
   public enumTest2() {
      for (months Month : months.values()) {
         switch (Month) {
            // case months.MARCH:  switch know the type
            case MARCH:
            case APRIL:
            case MAY:
               System.out.printf("Spring month: %s%n", Month);
               break;
            case DECEMBER:
            case JANUARY:
            case FEBRUARY:
               System.out.printf("Winter month: %s%n", Month);
               break;
            case JUNE:
            case JULY:
            case AUGUST:
               System.out.printf("Summer month: %s%n", Month);
               break;
            default:
            case SEPTEMBER:
            case OCTOBER:
            case NOVEMBER:
               System.out.printf("Fall month: %s%n", Month);
         }
      }
   }

   public static void main(String[] args) {
       new enumTest2();
   }
}

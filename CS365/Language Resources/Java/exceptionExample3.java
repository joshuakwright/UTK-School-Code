public class exceptionExample3 {
   public static void main(String args []) {
      try {
         // code that may raise an exception
         int data = 100 / 0;
      // } catch (ArithmeticException e) {
      } catch (Exception e) {
         System.out.println(e);
      }
    
      // rest of program code
      System.out.println("rest of the code ...");
   }
}

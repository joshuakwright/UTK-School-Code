public class throwExample2 {
   int age = 0;

   public throwExample2(int Age) {
      this.age = Age;
   }

   void votingCheck() {
      if (age < 18) 
         throw new ArithmeticException("Not Eligible for voting");
      else
         System.out.println("Eligible for voting");
   }

   public static void main(String args[]) {                                                                      
      System.out.println("Starting throwExample");                                                           

      int age = Integer.parseInt(args[0]);
      throwExample2 tEx = new throwExample2(age);
    
      try {
          tEx.votingCheck();
      } catch (ArithmeticException e) {
         System.out.println("Caught the Exception");
         System.out.println(e.getMessage());
      }

      System.out.println("Ending throwExample");    
   }
}

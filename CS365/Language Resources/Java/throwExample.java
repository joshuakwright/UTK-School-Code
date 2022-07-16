public class throwExample {
   int age = 0;

   public throwExample(int Age) {
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
      throwExample tEx = new throwExample(age);                                                                              
      tEx.votingCheck();

      System.out.println("Ending throwExample");    
   }
}

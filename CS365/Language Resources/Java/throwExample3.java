// MyException class - must extend Exception class
class MyException extends Exception {
   String str1;

   // Constructor takes message we will use for an exception
   MyException(String str2) {
      str1 = str2;
   }

   // What to display when exception thrown
   public String toString() {
      return("MyException Occurred:  " + str1);
   }
}

public class throwExample3 {
   int age = 0;

   public throwExample3(int Age) {
      this.age = Age;
   }

   void votingCheck() {
      try {
         if (age < 18) 
            throw new MyException("Not Eligible for voting");
         else
            System.out.println("Eligible for voting");
      } catch (MyException e) {
         System.out.println("Caught the Exception");
         System.out.println(e.toString());
      }
   }

   public static void main(String args[]) {                                                                      
      System.out.println("Starting throwExample");

      int age = Integer.parseInt(args[0]);                                                                     
      throwExample3 tEx = new throwExample3(age);

      tEx.votingCheck();

      System.out.println("Ending throwExample");    
   }
}

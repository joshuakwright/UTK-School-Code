class multiExceptionExample {
   int [] Array = {3,6,9,12,15};

   public multiExceptionExample(int index) {
      try {
         System.out.print("Array at index " + index + " : " + Array[index] + "\n");
         Array[0] = Array[0] / 0;
      } catch (ArithmeticException e) {
         // Your code optional
         System.out.println(e);
      } catch (ArrayIndexOutOfBoundsException e) {
         // Your code optional
         System.out.println(e);
      } finally {
	System.out.println("The finally block is always executed.");
      }
   }

   public static void main(String args[]) {
      System.out.println("Starting multiExceptionExample");
      int index = Integer.parseInt(args[0]); 
      new multiExceptionExample(index);
      System.out.println("Ending multiExceptionExample");
   }
}

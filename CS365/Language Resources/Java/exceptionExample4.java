class exceptionExample4 {
   int [] Array = {3,6,9,12,15};

   public exceptionExample4(int index) {
      try {
         System.out.print("Array at index " + index + " : " + Array[index] + "\n");
      } catch (ArrayIndexOutOfBoundsException e) {
         // Your code optional
         System.out.println(e.toString());
      } finally {
	System.out.println("The finally block is always executed.");
      }
   }

   public static void main(String args[]) {
      System.out.println("Starting exceptionExample");
      int index = Integer.parseInt(args[0]); 
      new exceptionExample4(index);
      System.out.println("Ending exceptionExample");
   }
}

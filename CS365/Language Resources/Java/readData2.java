import java.util.Scanner;
import java.io.Console;

class Student {
   String name;
   int age;
   float height;
   boolean isLocal;

   Student(String name, int age, float height, boolean isLocal){
      this.name = name;
      this.age = age;
      this.height = height;
      this.isLocal = isLocal;
   }

   public void displayDetails(){
      System.out.println("Details..............");
      System.out.println("Name: "+this.name);
      System.out.println("Age: "+this.age);
      System.out.println("Height in inches: "+this.height);
      if(this.isLocal) {
         System.out.println("Knoxvillian");
      } else {
         System.out.println("Not a Knoxvillian");
      }
   }
}

public class readData2 {
   public readData2 (String args[]) {
      String inputLine;
      Console inputReader = System.console();
      String name = "";
      int age = 0;
      float height = (float) 0.0;
      boolean isLocal = false;

      // exit if console not provided
      if (inputReader == null) {
         System.err.println("No console.");
         System.exit(1);
      }

      System.out.println("Please enter your name, age, height, and locality (stop to exit):");
      while(true) {
         inputLine = inputReader.readLine();
         if (inputLine.compareTo("stop") == 0) break;

         try (Scanner lineTokenizer = new Scanner(inputLine)) {
            if (lineTokenizer.hasNext())
               name = lineTokenizer.next();
            else continue;
            if (lineTokenizer.hasNextInt()) 
               age = lineTokenizer.nextInt();
            else continue;
            if (lineTokenizer.hasNextFloat()) 
               height = lineTokenizer.nextFloat();
            else continue; 
            if (lineTokenizer.hasNextBoolean()) 
               isLocal = lineTokenizer.nextBoolean();
            else continue; 
            lineTokenizer.close();
         }
      }

      Student std = new Student(name, age, height, isLocal);
      std.displayDetails();
   }

   public static void main(String args[]){
      new readData2(args);
   }
}

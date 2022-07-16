public class Subclass extends Superclass {
    int x = 40;
	
    // overrides printMethod in Superclass
    public void printMethod() {
        super.printMethod();
        System.out.println("Printed in Subclass");

	System.out.printf("Super x = %d\n", super.x);
	System.out.printf("Sub x = %d\n", x);
    }
    public static void main(String[] args) {
        Subclass s = new Subclass();
        s.printMethod();    
    }
}

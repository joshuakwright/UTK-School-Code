public class Horse implements Animal {
    private int numLegs = 4;
	
    public void eats() {
	System.out.println("Horses eat.  They are herbivores.");
    }

    public void travels(){
	String str = Integer.toString(numLegs);
	System.out.printf("Horses travel.  They walk and run on %d legs.\n", numLegs);
	System.out.print("Horses travel.  They walk and run on " + str + " legs.\n");
    }
}

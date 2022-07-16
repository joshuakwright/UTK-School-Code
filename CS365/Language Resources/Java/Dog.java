public class Dog implements Animal {
    private int numLegs = 4;
	
    public void eats() {
	    System.out.println("Dogs eat.  They are carnivores.");
    }

    public void travels(){
	    System.out.printf("Dogs travel.  They walk and run on %d legs.\n", numLegs);
    }
}

public class Mammal {
    public Mammal() {
        Dog d = new Dog();
        Horse h = new Horse();

        System.out.println("Do dogs eat?");
        d.eats();
        System.out.println("Do horses eat?");
        h.eats();
        System.out.println("Do dogs travel?");
        d.travels();
        System.out.println("Do horses travel?");
        h.travels();
    }

    public static void main (String args[]) {
        new Mammal();
    }	
}

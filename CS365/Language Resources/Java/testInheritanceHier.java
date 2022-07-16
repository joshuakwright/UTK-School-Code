class Animal{  
    int type;

    void eat(String str, int type) {
	if (type == 1) 
 	    System.out.println(str + " are carnivores");
	else if (type == 2) 
 	    System.out.println(str + " are herbivores");
	else if (type == 3) 
 	    System.out.println(str + " are omnivores");
    }  
}  

class Dog extends Animal{  
    void bark(){
	System.out.println("Dogs bark");
    }  
}  
   
class Cat extends Animal{  
    void meow(){
	System.out.println("Cats meow");
    }  
}  

class testInheritanceHier{  
    testInheritanceHier() {
	Dog d = new Dog();
	d.bark();
	d.eat("Dogs", 1);
	Cat c = new Cat();
	c.meow();
	c.eat("Cats", 1);
    }

    public static void main(String args[]){  
	new testInheritanceHier();
    }
}  

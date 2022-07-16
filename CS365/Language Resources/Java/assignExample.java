class foo {
	public int a = 4;
}
class assignExample {                                                                                                    
    // constructor                                                                               
    public assignExample() {
	foo f = new foo();
	foo g = new foo();
	g.a = 8;

	System.out.println(f.a + " ");
	System.out.println(g.a);

	f = g;
	System.out.println(f.a + " ");
        System.out.println(g.a);
    }

    public static void main(String args[]) {
	new assignExample();
    }
}


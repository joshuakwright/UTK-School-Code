public class employee {
    int id;
    String name;
    address Address;

    public employee (int id, String name, address addr) {
	this.id = id;
	this.name = name;
	this.Address = addr;
    }

    void display() {
	System.out.println(id + " " + name);
	System.out.println(Address.city + " " + Address.state + " " + Address.country);
    }

    public static void main(String[] args) {
	address address1 = new address("Knoxville", "TN", "USA");
	address address2 = new address("Nashville", "TN", "USA");

   	employee e = new employee(100, "brad", address1);
	employee e2 = new employee(101, "kelsey", address2);

	e.display();
	e2.display();
    }
}

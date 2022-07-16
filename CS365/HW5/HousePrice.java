//Joshua Wright jwrig117

class HousePrice {
    static class Person {
        int price;
        HouseColor color;
        String name;

        public Person(String n, int p, HouseColor c) {
            name = new String(n);
            price = p;
            color = c;
        }
    }

    public enum HouseColor {
        RED (2010, "red"),
        BLUE (2011, "blue"),
        GREEN (2012, "green"),
        WHITE (2013, "white");

        final int ColorYear;
        final String Color;

        HouseColor(int cYear, String c) {
            ColorYear = cYear;
            Color = c;
        }

        public int getYear() {
            return ColorYear;
        }

        public String getColor() {
            return Color;
        }
    }

    public static void main (String[] args) {
        if ((args.length) % 3 != 0 || args.length < 4) {
            System.out.printf("Invalid input. Needs to be of the form: color name price%n");
            return;
        }

        Person personList[] = new Person[args.length / 3];
        float totalPrice = 0;

        for (int i = 0, j = 0; j < args.length/3; i++, j++) {
            String name = new String(args[i]);
            i++;
            int price = Integer.parseInt(args[i]);
            totalPrice += (float)price;
            i++;
            HouseColor color = args[i].equals("red") ? HouseColor.RED : args[i].equals("blue") ? HouseColor.BLUE : args[i].equals("green") ? HouseColor.GREEN : HouseColor.WHITE;
            personList[j] = new Person(name, price, color);
        }

        float averagePrice = totalPrice / (args.length/3);
        System.out.printf("%naverage house price = %-,10.2f", averagePrice);
        System.out.printf("%n%n");

        for (int i = 0; i < args.length / 3; i++) {
            System.out.printf("%-15s ", personList[i].name);
            System.out.printf("%,10d ", personList[i].price);
            System.out.printf("%b%n", personList[i].price > averagePrice);
            System.out.printf("This %s house was built in the year %d%n", personList[i].color.getColor(), personList[i].color.getYear());
        }
        System.out.printf("%n");
    }
}
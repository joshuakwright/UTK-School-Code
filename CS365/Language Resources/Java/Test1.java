class Test1 {
        static int x;
        int k;

        // constructor with 2 args
        public Test1(int n, int m) {
                x = n;
                k = m;
        }

        public static void main(String args[]) {
                Test1 t1 = new Test1(10, 20);
                Test1 t2 = new Test1(30, 40);
                System.out.print(t1.x + " ");
                System.out.print(t1.k + " ");
                System.out.print(t2.x + " ");
                System.out.println(t2.k);
        }
}

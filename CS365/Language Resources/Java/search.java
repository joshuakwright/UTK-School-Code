class search { 
    public search(String args[]) {
        int n = Integer.parseInt(args[0]); // parseInt converts String to int 
        String searchList[] = new String[n]; 
        int i; 
        for (i = 0; i < n; i++) { 
            searchList[i] = args[i+1]; 
        } 

        // %b is the formatting string for a boolean 
        for (i = n+1; i < args.length; i++) { 
            System.out.printf("%s %b\n", args[i], find(searchList, args[i])); 
        } 
    } 
 
    boolean find(String list[], String key) { 
        for (String value : list) 
            if (value.equals(key)) 
                return true; 
        return false; 
    } 

    public static void main(String args[]) { 
        new search(args);
    }
}

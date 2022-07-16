class assignEarly {   
    int nums[] = new int[10];

    // Intialization block
    {
       for (int i = 0; i < nums.length; i++)
       {   nums[i] = i+1;   }
    }

    // constructor                                                                               
    public assignEarly() {
	System.out.println(nums[0]);
	System.out.println(nums[9]);
    }

    public static void main(String args[]) {
	new assignEarly();
    }
}


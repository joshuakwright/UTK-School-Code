//Joshua Wright jwrig117

import java.util.*;
import java.io.*;
import java.util.Scanner;

interface Queue {
    void push(Object o);
    Object pop();
    Object peek();
    void loop();
    boolean isEmpty();
    int length();
}

class ArrayQueue implements Queue {
    ArrayList<Object> arrayQueue = new ArrayList<Object>();

    public void push(Object num) {
        arrayQueue.add(num);
    }    

    public Object pop() {
        return arrayQueue.size() != 0 ? arrayQueue.remove(0) : null;
    }

    public Object peek() {
        return arrayQueue.get(0);
    }

    public void loop() {
        arrayQueue.add(arrayQueue.remove(0));
    }

    public boolean isEmpty() {
        return arrayQueue.size() == 0 ? true : false;
    }

    public int length() {
        return arrayQueue.size();
    }

}

class LinkListQueue implements Queue {
    class Node {
        Object data;
        Node next;

        Node(Object _data) {
            data = _data;
        }
    }

    Node head;
    int length = 0;

    public void push(Object data) {
        if (head == null) {
            head = new Node(data);
            length++;
            return;
        }

        Node temp = head;
        while (temp.next != null) {
            temp = temp.next;
        }
        temp.next = new Node(data);
        length++;
    }

    public Object pop() {
        if(length > 0) {
            Object temp = head.data;
            head = head.next;
            length--;
            return temp;
        }
        return null;
    }

    public Object peek() {
        return head.data;
    }

    public void loop(){ 
        Node temp = new Node(head.data);
        head = head.next;
        Node looper = head;
        while (looper.next != null) {
            looper = looper.next;
        }
        looper.next = temp;
    }

    public boolean isEmpty(){
        return length == 0 ? true : false;
    }

    public int length(){
        return length;
    }

}

public class InterfaceQueue {
    public static void main(String args[]) {

        String file = new File("Randomnumbers.txt").getAbsolutePath();
        Scanner scanner;
        try{ 
            scanner = new Scanner(new File(file));
        }
        catch (FileNotFoundException ex){
            System.out.print(ex);
            return;
        }

        ArrayQueue validInts = new ArrayQueue();
        LinkListQueue garbageInts = new LinkListQueue();
        int validIntCount = 0, garbageIntCount = 0;

        while(scanner.hasNext()) {
            String temp = scanner.nextLine();
            try {
                int number = Integer.parseInt(temp);
                validInts.push(number);
                validIntCount++;
            }
            catch (NumberFormatException ex) {
                garbageInts.push(temp);
                garbageIntCount++;
            }
        }

        System.out.printf("Number of Integers: %d\n", validIntCount);
        System.out.printf("Number of Non-Integers: %d\n", garbageIntCount);

        try {
            PrintWriter writer = new PrintWriter("output.txt", "ascii");
        
            int loopCount = 1;
            while (validInts.length() > 0) {
                String digits = Integer.toString(((Integer)validInts.peek()).intValue());
                int loop = 0;
                for(int i = 0; i < digits.length(); i++) {
                    loop += Character.getNumericValue(digits.charAt(i));
                }

                for (int i = 0; i < loop; i++) {
                    validInts.loop();
                }

                loop = ((Integer)validInts.pop()).intValue();
                loop = loop / 1000;

                for (int i = 0; i < loop; i++) {
                    garbageInts.loop();
                }

                writer.print(garbageInts.peek());
                writer.print("\n");

                for (int i = 0; i < loopCount; i++) {
                    validInts.pop();
                }
                loopCount++;
            }   

            writer.close();
        }

        catch(IOException ex) {
            System.out.print("An error occured while creating or writing to file.");
            ex.printStackTrace();
        }
    }
}

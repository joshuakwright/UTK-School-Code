//Joshua Wright jwrig117

import java.io.*;
import java.util.Comparator;
import java.util.Scanner;
import java.util.Vector;

class Personnel {
    String name;
    String socialNum;
    char gender;

    Personnel(String[] personVals) {
        name = personVals[0];
        socialNum = personVals[1];
        gender = personVals[2].charAt(0);
    }

    public String GetSocialNum() {
        return socialNum;
    }
}

class Student extends Personnel {
    float gpa;
    int year;

    Student(String[] personVals) {
        super(personVals);
        gpa = Float.parseFloat(personVals[3]);
        year = Integer.parseInt(personVals[4]);
    }
}

class Employee extends Personnel {
    String position;
    double salary;

    Employee(String[] personVals) {
        super(personVals);
        position = personVals[3];
        salary = Double.parseDouble(personVals[4]);
    }

    public double GetSalary() {
        return salary;
    }
}

class Staff extends Employee {
    int vacationLeave;

    Staff(String[] personVals) {
        super(personVals);
        vacationLeave = Integer.parseInt(personVals[5]);
    }

    public int GetVacationLeave() {
        return vacationLeave;
    }
}

class Faculty extends Employee {
    String rank;

    Faculty(String[] personVals) {
        super(personVals);
        rank = personVals[5];
    }
}

class PersonnelTracker {
    public static void main (String[] args) {
        String file = new File("PersonnelEntries-1.csv").getAbsolutePath();
        Scanner scanner;
        try{ 
            scanner = new Scanner(new File(file));
        }
        catch (FileNotFoundException ex){
            return;
        }

        Vector<Faculty> allFaculty = new Vector<Faculty>();
        Vector<Staff> allStaff = new Vector<Staff>();
        Vector<Student> allStudents = new Vector<Student>();

        int facultyCount = 0, staffCount = 0, studentCount = 0;
        int femaleStudentCount = 0, maleStudentCount = 0;
        float femaleGPATotal = 0, maleGPATotal = 0;
        int fourFemaleGPACount = 0, fourMaleGPATotal = 0;

        while (scanner.hasNext()) {
            String temp = scanner.nextLine();
            String[] tempArr = temp.split(",");
            String gender = tempArr[2];

            if (tempArr[3].equals("Faculty")) {
                Faculty faculty = new Faculty(tempArr);
                allFaculty.add(faculty);
                facultyCount++;
            }
            else if(tempArr[3].equals("Staff")) {
                Staff staff = new Staff(tempArr);
                allStaff.add(staff);
                staffCount++;
            }
            else {
                Student student = new Student(tempArr);
                allStudents.add(student);
                studentCount++;

                float gpa = Float.parseFloat(tempArr[3]);
                if (gender.equals("F")) {
                    femaleStudentCount++;
                    femaleGPATotal += gpa;
                    if(gpa == 4) {
                        fourFemaleGPACount++;
                    }
                }
                else {
                    maleStudentCount++;
                    maleGPATotal += gpa;
                    if (gpa == 4) {
                        fourMaleGPATotal++;
                    }
                }
            }
        }

        scanner.close();

        float avgFemaleGPA = femaleGPATotal / femaleStudentCount;
        float avgMaleGPA = maleGPATotal / maleStudentCount;

        try {
            PrintWriter writer = new PrintWriter("Answer.txt", "UTF-8");
            writer.printf("A. The number of students is : %d\n", studentCount);
            writer.printf("B. The number of staff is : %d\n", staffCount);
            writer.printf("C. The number of faculty is : %d\n", facultyCount);
            writer.printf("D. The number of female students is : %d\n", femaleStudentCount);
            writer.printf("E. The average female student's GPA is : %.3f\n", avgFemaleGPA);
            writer.printf("F. The number of female students with a 4.0 GPA is : %d\n", fourFemaleGPACount);
            writer.printf("G. The number of male students is : %d\n", maleStudentCount);
            writer.printf("H. The average male student's GPA is : %.3f\n", avgMaleGPA);
            writer.printf("I. The number of male students with a 4.0 GPA is : %d\n", fourMaleGPATotal);
            writer.close();
        }
        catch (IOException ex) {
            System.out.print("An error occured while creating or writing to file.");
            ex.printStackTrace();
        }

        allStudents.sort(Comparator.comparing(Student::GetSocialNum));
        try {
            PrintWriter writer = new PrintWriter("Students.csv");
            StringBuilder output = new StringBuilder();
            for (Student student : allStudents) {
                String[] outputArr = new String[5];
                outputArr[0] = student.name;
                outputArr[1] = student.socialNum;
                outputArr[2] = Character.toString(student.gender);
                outputArr[3] = Float.toString(student.gpa);
                outputArr[4] = Integer.toString(student.year);
                String studentOutput = String.join(",", outputArr);
                output.append(studentOutput);
                output.append("\n");
            }
            writer.write(output.toString());
            writer.close();
        }
        catch(IOException ex) {
            System.out.print("An error occured while creating or writing to file.");
            ex.printStackTrace();
        }

        allFaculty.sort(Comparator.comparing(Faculty::GetSalary));
        try {
            PrintWriter writer = new PrintWriter("Faculty.csv");
            StringBuilder output = new StringBuilder();
            for (Faculty faculty : allFaculty) {
                String[] outputArr = new String[6];
                outputArr[0] = faculty.name;
                outputArr[1] = faculty.socialNum;
                outputArr[2] = Character.toString(faculty.gender);
                outputArr[3] = faculty.position;
                String roundedSalaray = String.format("%.2f", faculty.salary);
                outputArr[4] = roundedSalaray;
                outputArr[5] = faculty.rank;
                String facultyOutput = String.join(",", outputArr);
                output.append(facultyOutput);
                output.append("\n");
            }
            writer.write(output.toString());
            writer.close();
        }
        catch(IOException ex) {
            System.out.print("An error occured while creating or writing to file.");
            ex.printStackTrace();
        }

        allStaff.sort(Comparator.comparing(Staff::GetVacationLeave).thenComparing(Staff::GetSocialNum));
        try {
            PrintWriter writer = new PrintWriter("Staff.csv");
            StringBuilder output = new StringBuilder();
            for (Staff staff : allStaff) {
                String[] outputArr = new String[6];
                outputArr[0] = staff.name;
                outputArr[1] = staff.socialNum;
                outputArr[2] = Character.toString(staff.gender);
                outputArr[3] = staff.position;
                String roundedSalaray = String.format("%.2f", staff.salary);
                outputArr[4] = roundedSalaray;
                outputArr[5] = Integer.toString(staff.vacationLeave);
                String facultyOutput = String.join(",", outputArr);
                output.append(facultyOutput);
                output.append("\n");
            }
            writer.write(output.toString());
            writer.close();
        }
        catch(IOException ex) {
            System.out.print("An error occured while creating or writing to file.");
            ex.printStackTrace();
        }
    }
}
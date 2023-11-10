#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <memory>
#include "Student.h"

std::vector<std::string> readLinesFromFile(const std::string& fileName) {
    std::vector<std::string> lines;
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << fileName << std::endl;
        std::exit(-1);
    }
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    return lines;
}
std::vector<int> gradeCalculator(float result, std::vector<int> letterGrades){
    if (result >=95){
        letterGrades[0]++;
    }else if(result >=90){
        letterGrades[1]++;
    }else if(result >=85){
        letterGrades[2]++;
    }else if(result >=80){
        letterGrades[3]++;
    }else if(result >=75){
        letterGrades[4]++;
    }else if(result >=70){
        letterGrades[5]++;
    }else if(result >=65){
        letterGrades[6]++;
    }else if(result >=60){
        letterGrades[7]++;
    }else if(result >=55){
        letterGrades[8]++;
    }else if(result >=50){
        letterGrades[9]++;
    }else{
        letterGrades[12]++;
    }
    return letterGrades;
}
std::vector<std::vector<int>> gradeReader(std::string path){
    std::vector<std::string> lines = readLinesFromFile(path);
    std::vector<std::vector<int>> grades;
    for (std::string line :lines){
        std::stringstream strStream(line);
        std::vector<int> temp;
        int gradeOne;
        while(strStream>>gradeOne){
            temp.push_back(gradeOne);
        }
        grades.push_back(temp);
    }
    return grades;
}
template <typename T>
void writer(std::string path, T text, int precision){
    std::fstream file;
    file.open(path, std::ios_base::app);
    if (!file.is_open()){
        std::cerr<<"File cannot be opened!";
        exit(-1);
    }
    if constexpr (std::is_floating_point_v<T>) {
        file << std::fixed << std::setprecision(precision) << text;
    } else {
        file << text;
    }
}
void part1(){
    int studentCount=0;
    std::vector<std::vector<int>> grades = gradeReader("fileio/part1_input.txt");
    std::vector<float> results;
    int A1=0,A2=0,A3=0,B1=0,B2=0,B3=0,C1=0,C2=0,C3=0,D=0,F1=0,F2=0,F3=0;
    std::vector<int> letterGrades = {A1,A2,A3,B1,B2,B3,C1,C2,C3,D,F1,F2,F3};
    double max =0,min=100;
    double resultSum=0;
    for (std::vector<int> grade : grades){
        studentCount++;
        float result = grade[0]*0.4 + grade[1]*0.6;
        results.push_back(result);
        resultSum+=result;
        letterGrades= gradeCalculator(result, letterGrades);
        if(result>max){
            max=result;
        }if (result<min){
            min=result;
        }
    }
    writer("part1_output.txt", "- - - - - - - - - -\nBBM201 Statistics (Part 1)\n- - - - - - - - - -",0);
    std::vector<std::string> letters = {"A1","A2","A3","B1","B2","B3","C1","C2","C3","D","F1","F2","F3"};
    int i =0;
    while (i < 13){
        writer("part1_output.txt", "\n"+letters[i]+":\t",0);
        if (letterGrades[i] ==0){
            writer("part1_output.txt", "-",0);
        }else{
            writer("part1_output.txt", letterGrades[i],0);
        }
        i++;
    }
    writer("part1_output.txt", "\n--------------------\nTotal Number of Students: "+std::to_string(studentCount)+"\nMean value of average grades: ",0);
    writer("part1_output.txt", resultSum/studentCount,2);
    writer("part1_output.txt", "\nMax Score: ",0);
    writer("part1_output.txt", max,1);
    writer("part1_output.txt", "\nMin Score: ",0);
    writer("part1_output.txt", min,1);
    writer("part1_output.txt", "\n--------------------\n",0);
}
void part2NoCurve(){
    std::vector<std::vector<int>> grades = gradeReader("fileio/part2_input.txt");
    int studentCount=0;
    std::vector<float> results;
    int A1=0,A2=0,A3=0,B1=0,B2=0,B3=0,C1=0,C2=0,C3=0,D=0,F1=0,F2=0,F3=0;
    std::vector<int> letterGrades = {A1,A2,A3,B1,B2,B3,C1,C2,C3,D,F1,F2,F3};
    double max =0,min=100;
    double resultSum=0;
    for (std::vector<int> grade : grades){
        studentCount++;
        float result = grade[0]*0.3 + grade[1]*0.3 + grade[2]*0.4;
        results.push_back(result);
        resultSum+=result;
        letterGrades= gradeCalculator(result, letterGrades);
        if(result>max){
            max=result;
        }if (result<min){
            min=result;
        }
    }
    writer("part2_output.txt", "- - - - - - - - - -\nBBM201 Statistics\n(Without Curve)\n- - - - - - - - - -",0);
    std::vector<std::string> letters = {"A1","A2","A3","B1","B2","B3","C1","C2","C3","D","F1","F2","F3"};
    int i =0;
    while (i < 13){
        writer("part2_output.txt", "\n"+letters[i]+":\t",0);
        if (letterGrades[i] ==0){
            writer("part2_output.txt", "-",0);
        }else{
            writer("part2_output.txt", letterGrades[i],0);
        }
        i++;
    }
    writer("part2_output.txt", "\n--------------------\nTotal Number of Students: "+std::to_string(studentCount)+"\nMean value of average grades: ",0);
    writer("part2_output.txt", resultSum/studentCount,2);
    writer("part2_output.txt", "\nMax Score: ",0);
    writer("part2_output.txt", max,1);
    writer("part2_output.txt", "\nMin Score: ",0);
    writer("part2_output.txt", min,1);
    writer("part2_output.txt", "\n--------------------\n",0);
}
void part2Curve1(){
    std::vector<std::vector<int>> grades = gradeReader("fileio/part2_input.txt");
    int studentCount=0;
    std::vector<float> results;
    int A1=0,A2=0,A3=0,B1=0,B2=0,B3=0,C1=0,C2=0,C3=0,D=0,F1=0,F2=0,F3=0;
    std::vector<int> letterGrades = {A1,A2,A3,B1,B2,B3,C1,C2,C3,D,F1,F2,F3};
    float max =0,min=100;
    float resultSum=0;
    for (std::vector<int> grade : grades){
        studentCount++;
        float result = grade[0]*0.3 + grade[1]*0.3 + grade[2]*0.4;
        results.push_back(result);
        resultSum+=result;
    }
    float mean = resultSum/studentCount;
    float curve=0;
    if(mean<50){
        curve=50-mean;
    }
    for(float result : results){
        result += curve;
        letterGrades= gradeCalculator(result, letterGrades);
        if(result>max){
            max=result;
        }if (result<min){
            min=result;
        }
    }
    writer("part2_curveType1_output.txt", "- - - - - - - - - - \nBBM201 Statistics\n(Curve Type 1 - Increase Average)\n- - - - - - - - - - ",1);
    std::vector<std::string> letters = {"A1","A2","A3","B1","B2","B3","C1","C2","C3","D","F1","F2","F3"};
    int i =0;
    while (i < 13){
        writer("part2_curveType1_output.txt", "\n"+letters[i]+":\t",0);
        if (letterGrades[i] ==0){
            writer("part2_curveType1_output.txt", "-",0);
        }else{
            writer("part2_curveType1_output.txt", letterGrades[i],0);
        }
        i++;
    }
    writer("part2_curveType1_output.txt", "\n--------------------\nTotal Number of Students: "+std::to_string(studentCount)+"\nMean value of average grades: ",0);
    writer("part2_curveType1_output.txt", (resultSum+(curve*studentCount))/studentCount,1);
    writer("part2_curveType1_output.txt", "\nMax Score: ",0);
    writer("part2_curveType1_output.txt", max,2);
    writer("part2_curveType1_output.txt", "\nMin Score: ",0);
    writer("part2_curveType1_output.txt", min,2);
    writer("part2_curveType1_output.txt", "\n--------------------\n",0);
}
void part2Curve2(){
    std::vector<std::vector<int>> grades = gradeReader("fileio/part2_input.txt");
    int studentCount=0;
    std::vector<float> results;
    int A1=0,A2=0,A3=0,B1=0,B2=0,B3=0,C1=0,C2=0,C3=0,D=0,F1=0,F2=0,F3=0;
    std::vector<int> letterGrades = {A1,A2,A3,B1,B2,B3,C1,C2,C3,D,F1,F2,F3};
    float firstMax =0,min=100, secondMax =0;
    float resultSum=0;
    for (std::vector<int> grade : grades){
        studentCount++;
        float result = grade[0]*0.3 + grade[1]*0.3 + grade[2]*0.4;
        results.push_back(result);
        if(result>firstMax){
            firstMax=result;
        }
        resultSum+=result;
    }
    float curve=0;
    if(firstMax<100){
        curve=100-firstMax;
    }
    for(float result : results){
        result += curve;
        letterGrades= gradeCalculator(result, letterGrades);
        if(result>secondMax){
            secondMax=result;
        }if (result<min){
            min=result;
        }
    }
    writer("part2_curveType2_output.txt", "- - - - - - - - - - \nBBM201 Statistics\n(Curve Type 2 - Increase Max Score)\n- - - - - - - - - - ",0);
    std::vector<std::string> letters = {"A1","A2","A3","B1","B2","B3","C1","C2","C3","D","F1","F2","F3"};
    int i =0;
    while (i < 13){
        writer("part2_curveType2_output.txt", "\n"+letters[i]+":\t",0);
        if (letterGrades[i] ==0){
            writer("part2_curveType2_output.txt", "-",0);
        }else{
            writer("part2_curveType2_output.txt", letterGrades[i],0);
        }
        i++;
    }
    writer("part2_curveType2_output.txt", "\n--------------------\nTotal Number of Students: "+std::to_string(studentCount)+"\nMean value of average grades: ",0);
    writer("part2_curveType2_output.txt", (resultSum+(curve*studentCount))/studentCount,2);
    writer("part2_curveType2_output.txt", "\nMax Score: ",0);
    writer("part2_curveType2_output.txt", secondMax,1);
    writer("part2_curveType2_output.txt", "\nMin Score: ",0);
    writer("part2_curveType2_output.txt", min,1);
    writer("part2_curveType2_output.txt", "\n--------------------\n",0);
}
void part2Curve3(){
    std::vector<std::vector<int>> grades = gradeReader("fileio/part2_input.txt");
    int studentCount=0;
    std::vector<float> results;
    int A1=0,A2=0,A3=0,B1=0,B2=0,B3=0,C1=0,C2=0,C3=0,D=0,F1=0,F2=0,F3=0;
    std::vector<int> letterGrades = {A1,A2,A3,B1,B2,B3,C1,C2,C3,D,F1,F2,F3};
    float max =0,min=100;
    float resultSum=0;
    for (std::vector<int> grade : grades){
        studentCount++;
        float result = grade[0]*0.3 + grade[1]*0.3 + grade[2]*0.4;
        results.push_back(result);
        resultSum+=result;
    }
    int failNumber= studentCount/4;
    float curve=0;
    std::sort(results.begin(), results.end());
    if (results[failNumber]<50){
        curve = 50-results[failNumber];
    }
    for(float result : results) {
        result += curve;
        letterGrades= gradeCalculator(result, letterGrades);
        if(result>max){
            max=result;
        }if (result<min){
            min=result;
        }
    }
    writer("part2_curveType3_output.txt", "- - - - - - - - - - \nBBM201 Statistics\n(Curve Type 3 - Fail %25)\n- - - - - - - - - - ",0);
    std::vector<std::string> letters = {"A1","A2","A3","B1","B2","B3","C1","C2","C3","D","F1","F2","F3"};
    int i =0;
    while (i < 13){
        writer("part2_curveType3_output.txt", "\n"+letters[i]+":\t",0);
        if (letterGrades[i] ==0){
            writer("part2_curveType3_output.txt", "-",0);
        }else{
            writer("part2_curveType3_output.txt", letterGrades[i],0);
        }
        i++;
    }
    writer("part2_curveType3_output.txt", "\n--------------------\nTotal Number of Students: "+std::to_string(studentCount)+"\nMean value of average grades: ",0);
    writer("part2_curveType3_output.txt", (resultSum+(curve*studentCount))/studentCount,2);
    writer("part2_curveType3_output.txt", "\nMax Score: ",0);
    writer("part2_curveType3_output.txt", max,1);
    writer("part2_curveType3_output.txt", "\nMin Score: ",0);
    writer("part2_curveType3_output.txt", min,1);
    writer("part2_curveType3_output.txt", "\n--------------------\n",0);
}
void part3(){
    int studentCount=0;
    std::vector<std::shared_ptr<Student>> students;
    std::vector<std::string> lines = readLinesFromFile("fileio/part3_input.txt");
    for (std::string line:lines){
        std::istringstream iss(line);
        std::vector<std::string> studentData;
        std::string part;
        while (std::getline(iss, part, '\t')) {
            studentData.push_back(part);
        }
        std::istringstream iss2(studentData[4]);
        std::vector<float> hmwGrades;
        int num;
        while (iss2 >> num) {
            hmwGrades.push_back(num);
        }
        Student student(studentData[0], std::stoi(studentData[1]), std::stof(studentData[2]), std::stof(studentData[3]), hmwGrades);
        students.push_back(std::make_shared<Student>(student));
        studentCount++;
    }
    std::vector<float> grades;
    std::vector<std::shared_ptr<Student>> f1Students;
    int A1=0,A2=0,A3=0,B1=0,B2=0,B3=0,C1=0,C2=0,C3=0,D=0,F1=0,F2=0,F3=0;
    int newF3=0;
    double resultSum=0;
    double max =0,min=100;
    for (std::shared_ptr<Student> student : students) {
        float hmwGradeSum=0;
        int undoneHmwCount=0;
        for (float grade : student->getHmwGrades()) {
            hmwGradeSum += grade;
            if (grade == 0) {
                undoneHmwCount++;
            }
        }
        student->setGpa(student->getFinal()*0.4+student->getMidterm()*0.3+hmwGradeSum*0.06);
        resultSum += student->getGpa();
        if(student->getGpa()>max){
            max=student->getGpa();
        }if (student->getGpa()<min){
            min=student->getGpa();
        }
        if (undoneHmwCount > 1) {
            student->setFailed(true);
            student->setLetter("F1");
            f1Students.push_back(student);
            F1++;
        }
        else if (student->getFinal()==0){
            student->setLetter("F2");
            F2++;
            student->setFailed(true);
        }else if (student->getFinal()<50){
            student->setLetter("F3");
            student->setFailed(true);
            F3++;
        }
        if(!student->isFailed()){
            grades.push_back(student->getGpa());
        }
    }
    std::vector<int> letterGrades = {A1,A2,A3,B1,B2,B3,C1,C2,C3,D,F1,F2,F3};
    for (std::shared_ptr<Student> student : students){
        if (student->getGpa() >=95){
            if(student->getLetter() ==""){
                student->setLetter("A1");
                letterGrades[0]++;
            }
        }else if(student->getGpa() >=90){
            if(student->getLetter() ==""){
                student->setLetter("A2");
                letterGrades[1]++;
            }
        }else if(student->getGpa() >=85){
            if(student->getLetter() ==""){
                student->setLetter("A3");
                letterGrades[2]++;
            }
        }else if(student->getGpa() >=80){
            if(student->getLetter() ==""){
                student->setLetter("B1");
                letterGrades[3]++;
            }
        }else if(student->getGpa() >=75){
            if(student->getLetter() ==""){
                student->setLetter("B2");
                letterGrades[4]++;
            }
        }else if(student->getGpa() >=70){
            if(student->getLetter() ==""){
                student->setLetter("B3");
                letterGrades[5]++;
            }
        }else if(student->getGpa() >=65){
            if(student->getLetter() ==""){
                student->setLetter("C1");
                letterGrades[6]++;
            }
        }else if(student->getGpa() >=60){
            if(student->getLetter() ==""){
                student->setLetter("C2");
                letterGrades[7]++;
            }
        }else if(student->getGpa() >=55){
            if(student->getLetter() ==""){
                letterGrades[8]++;
                student->setLetter("C3");
            }
        }else if(student->getGpa() >=50){
            if(student->getLetter() ==""){
                student->setLetter("D");
                letterGrades[9]++;
            }
        }else if(student->getGpa() <50){
            if(student->getLetter() ==""){
                student->setLetter("F3");
                letterGrades[12]++;
            }
        }
    }
    writer("part3_output.txt", "- - - - - - - - - - \nBBM201 Statistics (Part 3)\n- - - - - - - - - - ",0);
    std::vector<std::string> letters = {"A1","A2","A3","B1","B2","B3","C1","C2","C3","D","F1","F2","F3"};
    int i =0;
    while (i < 13){
        writer("part3_output.txt", "\n"+letters[i]+"\t",0);
        if (letterGrades[i] ==0){
            writer("part3_output.txt", "-",0);
        }else{
            writer("part3_output.txt", letterGrades[i],0);
        }
        i++;
    }
    writer("part3_output.txt", "\n--------------------\nTotal Number of Students: "+std::to_string(studentCount)+"\nMean value of average grades: ",0);
    writer("part3_output.txt", resultSum/studentCount,2);
    writer("part3_output.txt", "\nMax Score: ",0);
    writer("part3_output.txt", max,2);
    writer("part3_output.txt", "\nMin Score: ",0);
    writer("part3_output.txt", min,2);
    writer("part3_output.txt", "\n--------------------\n",0);
    //F1 List
    writer("part3_F1_list.txt", "Students who will fail BBM201 with F1 Grade:\n--------------------",0);
    for (std::shared_ptr<Student> student : f1Students){
        writer("part3_F1_list.txt", "\n"+student->getName()+"\t",0);
        writer("part3_F1_list.txt", student->getId(),0);
    }
    //topten list
    writer("part3_top_10_list.txt", "Top 10 Students of BBM201 by GPA:\n--------------------",0);
    int j = 0;
    std::sort(grades.begin(), grades.end(), std::greater<float>());
    for (float grade : grades) {
        j++;
        for (std::shared_ptr<Student> student: students){
            if (grade == student->getGpa()){
                writer("part3_top_10_list.txt", "\n"+student->getName()+"\t",0);
                writer("part3_top_10_list.txt", student->getId(),0);
                writer("part3_top_10_list.txt", "\t"+student->getLetter()+"\t",0);
                writer("part3_top_10_list.txt", student->getGpa(),2);
            }
        }
        if (j==10){
            break;
        }
    }
    //name letter list
    writer("part3_names_and_letters_list.txt", "Letters List of BBM201 Course:\n--------------------",0);
    std::sort(students.begin(), students.end(), Student::compareByName);
    for (std::shared_ptr<Student> student: students){
        writer("part3_names_and_letters_list.txt", "\n"+student->getName()+"\t",0);
        writer("part3_names_and_letters_list.txt", student->getId(),0);
        writer("part3_names_and_letters_list.txt", "\t"+student->getLetter(),0);
    }
}

int main() {
    //part1();
    //part2NoCurve();
    //part2Curve1();
    //part2Curve2();
    //part2Curve3();
    part3();
}

#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>

class Student {
private:
    std::string name;
    int id;
    float midterm;
    float final;
    std::vector<float> hmwGrades;
    float gpa;
    bool failed=false;
    std::string letter;
public:
    Student(const std::string name, int id, float midterm, float final, std::vector<float> hmwGrades);
    static bool compareByName(const std::shared_ptr<Student>& a, const std::shared_ptr<Student>& b);
    std::string getName() const;
    int getId() const;
    std::vector<float> getHmwGrades();
    float getMidterm();
    float getFinal();
    bool isFailed();
    std::string getLetter();
    float getGpa();
    void setLetter(std::string letter);
    void setFailed(bool failed);
    void setGpa(float gpa);
};


#endif //STUDENT_H

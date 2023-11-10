#include <string>
#include <vector>
#include <memory>
#include "Student.h"

Student::Student(const std::string name, int id, float midterm, float final, std::vector<float> hmwGrades)
        : name(name), id(id), midterm(midterm), final(final), hmwGrades(hmwGrades) {
}
bool Student::compareByName(const std::shared_ptr<Student> &a, const std::shared_ptr<Student> &b){
    if (a->getName() != b->getName()) {
        return a->getName() < b->getName();
    }
    return a->getId() < b->getId();
}
std::string Student::getName() const {
    return name;
}
int Student::getId() const {
    return id;
}
std::vector<float> Student::getHmwGrades() {
    return hmwGrades;
}
float Student::getMidterm() {
    return midterm;
}
float Student::getFinal() {
    return final;
}bool Student::isFailed(){
    return failed;
}std::string Student::getLetter(){
    return letter;
}float Student::getGpa(){
    return gpa;
}
void Student::setLetter(std::string letter){
    this->letter = letter;
}
void Student::setFailed(bool failed){
    this->failed = failed;
}
void Student::setGpa(float gpa){
    this->gpa = gpa;
}



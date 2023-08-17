#ifndef STRUCT_H
#define STRUCT_H

#include <string>
#include <vector>
using namespace std;

struct Employee
{
    string  id,  fam_name,  first_name,  unit,  position,  
            dob,  hometown,  address,  email,  phone_number,  
            start_of_work, parent_unit;
    vector<string>date_time;

    Employee()
    {
        id = "null_id";
    } 
    Employee(string id, string fam_name, string first_name, string unit,
             string position, string dob, string hometown, string address,
             string email, string phone_number, string start_of_work)
    {
        this -> id = id;
        this -> fam_name = fam_name;
        this -> first_name = first_name;
        this -> unit = unit;
        this -> position = position;
        this -> dob = dob;
        this -> hometown = hometown;
        this -> address = address;
        this -> email = email;
        this -> phone_number = phone_number;
        this -> start_of_work = start_of_work;   
    }
};

struct Department
{
    string name;
    vector<Employee>leader_list;
    vector<Employee>vice_leader_list;
    vector<Employee>employee_list;
};

struct Subsidiary
{
    string name;
    vector<Employee>director_list;
    vector<Employee>deputy_director_list;
    vector<Department>department_list;
    vector<Employee>employee_list;
};

struct Corporation
{
    string name = "BKcorporation";
    vector<Employee>manage;
    vector<Employee>chairman_list;
    vector<Employee>vice_chairman_list;
    vector<Subsidiary>subsidiary_list;
};


bool operator == (Employee& person1, Employee& person2) {
    return(person1.id == person2.id && person1.fam_name == person2.fam_name && person1.first_name == person2.first_name
           && person1.unit == person2.unit && person1.position == person2.position && person1.dob == person2.dob
           && person1.hometown == person2.hometown && person1.address == person2.address
           && person1.email == person1.email && person1.phone_number == person2.phone_number && person1.start_of_work == person2.start_of_work);
}


void displayE(Employee person) {
    cout << person.id << endl << person.fam_name + " " + person.first_name << endl << person.unit << endl << person.position << endl << person.dob << endl << person.hometown << endl
         << person.address << endl << person.email << endl << person.phone_number << endl << person.start_of_work << endl;
}


#endif
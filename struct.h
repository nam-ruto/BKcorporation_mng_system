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

    Employee(){} 
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
    string leader;
    string vice_leader;
    vector<Employee>employee;
    
    Department()
    {
        name = "Unappointed";
        leader = "Unappointed";
        vice_leader = "Unappointed";
    }
};

struct Subsidiary
{
    string name;
    string director;
    string deputy_director;
    vector<Department>department;
    vector<Employee>employee;

    Subsidiary()
    {
        name = "Unappointed";
        director = "Unappointed";
        deputy_director = "Unappointed";
    }
};

struct Corporation
{
    string name;
    string chairman;
    string vice_chairman;
    vector<Subsidiary>subsidiary;

    Corporation()
    {
        name = "Unappointed";
        chairman = "Unappointed";
        vice_chairman = "Unappointed";
    }
};

#endif
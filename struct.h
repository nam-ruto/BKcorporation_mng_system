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
    Employee leader;
    Employee vice_leader;
    vector<Employee>employee_list;
    
    Department()
    {
        name = "Unappointed";
        leader.fam_name = "";
        leader.first_name = "Unappointed";
        vice_leader.fam_name = "";
        vice_leader.first_name = "Unappointed";
    }
};

struct Subsidiary
{
    string name;
    Employee director;
    Employee deputy_director;
    vector<Department>department_list;
    vector<Employee>employee_list;

    Subsidiary()
    {
        name = "Unappointed";
        director.fam_name = "";
        director.first_name = "Unappointed";
        deputy_director.fam_name = "";
        deputy_director.first_name = "Unappointed";
    }
};

struct Corporation
{
    string name;
    Employee chairman;
    Employee vice_chairman;
    vector<Subsidiary>subsidiary_list;

    Corporation()
    {
        name = "Unappointed";
        chairman.fam_name = "";
        chairman.first_name = "Unappointed";
        vice_chairman.fam_name = "";
        vice_chairman.first_name = "Unappointed";
    }
};

#endif
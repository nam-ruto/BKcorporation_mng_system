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

#endif
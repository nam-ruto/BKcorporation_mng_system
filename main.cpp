#include <iostream> // Luồng vào ra 
#include <string> // String
#include <vector> // Lưu trữ dữ liệu
#include <fstream> // Đọc file
#include <iomanip> // Thiết lập định dạng
#include <cctype>   
#include "struct.h" // File header chứa struct

// Bộ tiền xử lý này để xóa màn hình console
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

//--------MODULE 1: ĐỌC DỮ LIỆU TỪ FILE TEXT | H.D.NAM----------//
//--------Có 4 hàm, trong đó có 1 hàm chính + 3 hàm phụ--------//
Corporation initCorp;
void corp_devision(Employee person)
{
    bool is_chairman = ( person.position == "Chu tich" || person.position == "chu tich");
    bool is_vice_chairman = (person.position ==  "Pho chu tich" || person.position == "pho chu tich");
    
    if(is_chairman)
    {
        initCorp.name = person.unit;
        initCorp.chairman = person;
    }
    
    else if (is_vice_chairman)
    {
        initCorp.name = person.unit;
        initCorp.vice_chairman = person;
    }
}

void sub_devision(Employee person)
{
    bool found = false;
    bool is_director = (person.position == "Giam doc" || person.position == "giam doc");
    bool is_vice_director = (person.position == "Pho giam doc" || person.position == "pho giam doc");

    for(int i = 0; i < initCorp.subsidiary.size(); i++)
    {
        if(person.unit == initCorp.subsidiary[i].name)
        {
            if(is_director)
                initCorp.subsidiary[i].director = person;
            else if(is_vice_director)
                initCorp.subsidiary[i].deputy_director = person;  
            else
            {
                initCorp.subsidiary[i].employee.push_back(person);
            }
            found = true;
        }
    }

    if(!found)
    {
        Subsidiary new_sub;
        new_sub.name = person.unit;
        if(is_director)
            new_sub.director = person;
        else if(is_vice_director)
            new_sub.deputy_director = person;
        else
            new_sub.employee.push_back(person); 

        initCorp.subsidiary.push_back(new_sub);
    }
}

void depart_devision(Employee person)
{
    bool found = false;
    bool flag = false;
    bool is_leader = (person.position == "Truong phong" || person.position == "truong phong");
    bool is_vice_leader = (person.position == "Pho phong" || person.position == "pho phong");
    for(int i = 0; i < initCorp.subsidiary.size(); i++)
    {
        if(person.parent_unit == initCorp.subsidiary[i].name)
        {
            for(int j = 0; j < initCorp.subsidiary[i].department.size(); j++)
            {
                if(initCorp.subsidiary[i].department[j].name == person.unit)
                {
                    if(is_leader)
                        initCorp.subsidiary[i].department[j].leader = person;
                    else if(is_vice_leader)
                        initCorp.subsidiary[i].department[j].vice_leader = person; 
                    else
                        initCorp.subsidiary[i].department[j].employee.push_back(person);
                    found = true;
                    flag = true;
                    break;
                }
            }

            if(!found)
            {
                Department new_depart;
                new_depart.name = person.unit;
                if(is_leader)
                    new_depart.leader = person;
                else if(is_vice_leader)
                    new_depart.vice_leader = person; 
                else
                    new_depart.employee.push_back(person);
                initCorp.subsidiary[i].department.push_back(new_depart);
                flag = true;
                break;
            }
        }
    }
    if(!flag)
    {
        Subsidiary new_sub;
        new_sub.name = person.parent_unit;
        initCorp.subsidiary.push_back(new_sub);
        depart_devision(person);
    }
}

int upload(vector<Employee>&manage_emp)
{
    string  id, fam_name, first_name, unit, position, dob, hometown, 
            address, email, phone_number, working_start_date;

    fstream input;
    input.open("data.txt", ios::in);

    if(!input.is_open())
    {
        cout << "FILE ERROR. Can not access, please try again!\n";
        return 0;
    }

    else
    {
        while(!input.eof())
        {
            begining:
                getline(input, id); getline(input, fam_name); getline(input, first_name);
                getline(input, unit); getline(input, position); getline(input, dob);
                getline(input, hometown); getline(input, address);getline(input, email); 
                getline(input, phone_number);getline(input, working_start_date); 
                
                // Xử lý chuỗi - unit
                int slash = 0;
                string parent_unit;
                for(int i = 0; i < unit.size(); i++)
                {
                    if(unit[i] == '/')
                    {
                        slash += 1;
                        parent_unit = unit.substr(0, i);
                        unit = unit.substr(i+1, unit.size());
                        i = 0;
                    }
                }

                // Khởi tạo struct nhân viên, có các trường dữ liệu vừa nạp ở trên
                Employee person(id, fam_name, first_name, unit, position, dob, hometown, address, email, phone_number, working_start_date);
                
                // Lấy dữ liệu phần chấm công
                string tmp;
                getline(input, tmp);
                while(tmp != "")
                {
                    person.date_time.push_back(tmp);
                    getline(input, tmp);
                }
                person.parent_unit = parent_unit;

                // Ném thằng này vào vecto manage_emp
                manage_emp.push_back(person);

                // Phân bổ thằng này vào vị trí Corp
                if(slash == 0)
                {
                    corp_devision(person);
                }

                // Phân bổ thằng này vào vị trí Subsidiary
                else if(slash == 1)
                {
                    sub_devision(person);
                }

                // Phân bổ thằng này vào vị trí Department
                else if(slash == 2)
                {
                    depart_devision(person);
                }

        }
    }
    input.close();
}

// Module 2
// Module 3
// Module 4
// Module 5
// Module 6
// Module 7

int main()
{
    // Khai báo vecto để lưu trữ toàn bộ nhân sự
    vector<Employee>manage;

    // Tải thông tin từ file text lên, nếu không tải được thì dừng chương trình
    if(upload(manage))
    {
        // Menu-base
        //..........

    }

    return 0;
}
#include <iostream> // Luồng vào ra 
#include <string> // String
#include <vector> // Lưu trữ dữ liệu
#include <fstream> // Đọc file
#include <iomanip> // Thiết lập định dạng
#include "struct.h" // File header chứa struct

// Bộ tiền xử lý này để xóa màn hình console
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

//Table format//
void menu_line(int n)
{
    ios init(NULL);
    init.copyfmt(cout);
    cout << "+" << setw(n) << setfill('-') << "" << "+" << endl;
    cout.copyfmt(init); // Reset fill character to the default
}

void line(int n)
{
    ios init(NULL);
    init.copyfmt(cout);
    cout << "\t+" << setw(n) << setfill('-') << "" << "+" << endl;
    cout.copyfmt(init); // Reset fill character to the default 
}

//--------MODULE 1: ĐỌC DỮ LIỆU TỪ FILE TEXT | H.D.NAM----------//
Corporation initCorp;
void corp_devision(Employee person)
{
    bool is_chairman = ( person.position == "Chu tich" || person.position == "chu tich");
    bool is_vice_chairman = (person.position ==  "Pho chu tich" || person.position == "pho chu tich");
    
    if(is_chairman)
        initCorp.chairman_list.push_back(person);
    
    else if (is_vice_chairman)
        initCorp.vice_chairman_list.push_back(person);

}

void sub_devision(Employee person)
{
    bool found = false;
    bool is_director = (person.position == "Giam doc" || person.position == "giam doc");
    bool is_vice_director = (person.position == "Pho giam doc" || person.position == "pho giam doc");

    for(int i = 0; i < initCorp.subsidiary_list.size(); i++)
    {
        if(person.unit == initCorp.subsidiary_list[i].name)
        {
            if(is_director)
                initCorp.subsidiary_list[i].director_list.push_back(person);
            else if(is_vice_director)
                initCorp.subsidiary_list[i].deputy_director_list.push_back(person);  
            else
            {
                initCorp.subsidiary_list[i].employee_list.push_back(person);
            }
            found = true;
        }
    }

    if(!found)
    {
        Subsidiary new_sub;
        new_sub.name = person.unit;
        if(is_director)
            new_sub.director_list.push_back(person);
        else if(is_vice_director)
            new_sub.deputy_director_list.push_back(person); 
        else
            new_sub.employee_list.push_back(person); 

        initCorp.subsidiary_list.push_back(new_sub);
    }
}

void depart_devision(Employee person)
{
    bool found = false;
    bool flag = false;
    bool is_leader = (person.position == "Truong phong" || person.position == "truong phong");
    bool is_vice_leader = (person.position == "Pho phong" || person.position == "pho phong");
    for(int i = 0; i < initCorp.subsidiary_list.size(); i++)
    {
        if(person.parent_unit == initCorp.subsidiary_list[i].name)
        {
            for(int j = 0; j < initCorp.subsidiary_list[i].department_list.size(); j++)
            {
                if(initCorp.subsidiary_list[i].department_list[j].name == person.unit)
                {
                    if(is_leader)
                        initCorp.subsidiary_list[i].department_list[j].leader_list.push_back(person);
                    else if(is_vice_leader)
                        initCorp.subsidiary_list[i].department_list[j].vice_leader_list.push_back(person); 
                    else
                        initCorp.subsidiary_list[i].department_list[j].employee_list.push_back(person);
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
                    new_depart.leader_list.push_back(person);
                else if(is_vice_leader)
                    new_depart.vice_leader_list.push_back(person);
                else
                    new_depart.employee_list.push_back(person);
                initCorp.subsidiary_list[i].department_list.push_back(new_depart);
                flag = true;
                break;
            }
        }
    }
    if(!flag)
    {
        Subsidiary new_sub;
        new_sub.name = person.parent_unit;
        initCorp.subsidiary_list.push_back(new_sub);
        depart_devision(person);
    }
}

bool upload()
{
    string  id, fam_name, first_name, unit, position, dob, hometown, 
            address, email, phone_number, working_start_date;

    fstream input;
    input.open("data.txt", ios::in);

    if(!input.is_open())
    {
        cout << "FILE ERROR. Can not open txt file, please try again!\n";
        return false;
    }

    else
    {
        while(!input.eof())
        {
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
            initCorp.manage.push_back(person);

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
    return true;
}


//--------MODULE 2: HIỂN THỊ DỮ LIỆU | N.T.D.HANG----------//
void display_corp_data()
{
    int max_size;
    if(initCorp.chairman_list.size() == 0)
        {
            Employee new_emp;
            initCorp.chairman_list.push_back(new_emp);
        }
    if(initCorp.vice_chairman_list.size() == 0)
        {
            Employee new_emp;
            initCorp.vice_chairman_list.push_back(new_emp);
        }
    if(initCorp.chairman_list.size() >= initCorp.vice_chairman_list.size())
        max_size = initCorp.chairman_list.size();
    else
        max_size = initCorp.vice_chairman_list.size();

    cout << "\t" << initCorp.name << endl;
    line(44);
    cout << "\t|" << left << setw(20) << "CHAIRMAN" << " | " << setw(20) << "VICE CHAIRMAN" << " |" << endl;
    line(44);
    for(int i = 0; i < max_size; i++)
    {
        if(i == initCorp.chairman_list.size())
        {
            string full_name_vice_chairman = initCorp.vice_chairman_list[i].fam_name + " " + initCorp.vice_chairman_list[i].first_name;
            cout << "\t|" << left << setw(20) << "" << " | " << setw(20) << full_name_vice_chairman << " |" << endl;
        }
        else if(i == initCorp.vice_chairman_list.size())
        {
            string full_name_chairman = initCorp.chairman_list[i].fam_name + " " + initCorp.chairman_list[i].first_name;
            cout << "\t|" << left << setw(20) << full_name_chairman << " | " << setw(20) << "" << " |" << endl;
        }
        else
        {
            string full_name_chairman = initCorp.chairman_list[i].fam_name + " " + initCorp.chairman_list[i].first_name;
            string full_name_vice_chairman = initCorp.vice_chairman_list[i].fam_name + " " + initCorp.vice_chairman_list[i].first_name;
            cout << "\t|" << left << setw(20) << full_name_chairman << " | " << setw(20) << full_name_vice_chairman << " |" << endl;
        }
    }
    line(44);
    int n = initCorp.subsidiary_list.size();
    cout << "\tOwns " << n << " subsidiary companies" << endl;

    for(int i = 0; i < n; i++)
    {
        int no_dept = initCorp.subsidiary_list[i].department_list.size();
        int dir = initCorp.subsidiary_list[i].director_list.size();
        int dep_dir = initCorp.subsidiary_list[i].deputy_director_list.size();

        if(dir == 0)
        {
            Employee new_emp;
            initCorp.subsidiary_list[i].director_list.push_back(new_emp);
        }
        if(dep_dir == 0)
        {
            Employee new_emp;
            initCorp.subsidiary_list[i].deputy_director_list.push_back(new_emp);
        }

        cout << "\t" << i+1 << "." << initCorp.subsidiary_list[i].name << "\n";
        line(44);
        cout << "\t|" << left << setw(20) << "DIRECTOR" << " | " << setw(20) << "DEPUTY DIRECTOR" << " |" << endl;
        line(44);

        int max_size_1;
        if(dir >= dep_dir)
            max_size_1 = dir;
        else
            max_size_1 = dep_dir;

        for(int j = 0; j < max_size_1; j++)
        {
            if(j == dir)
            {
                string full_name_dep_dir = initCorp.subsidiary_list[i].deputy_director_list[j].fam_name + " " + initCorp.subsidiary_list[i].deputy_director_list[j].first_name;
                cout << "\t|" << left << setw(20) << "" << " | " << setw(20) << full_name_dep_dir << " |" << endl;
            }
            else if(j == dep_dir)
            {
                string full_name_dir = initCorp.subsidiary_list[i].director_list[j].fam_name + " " + initCorp.subsidiary_list[i].director_list[j].first_name;
                cout << "\t|" << left << setw(20) << full_name_dir << " | " << setw(20) << "" << " |" << endl;
            }
            else
            {
                string full_name_dep_dir = initCorp.subsidiary_list[i].director_list[j].fam_name + " " + initCorp.subsidiary_list[i].director_list[j].first_name;
                string full_name_dir = initCorp.subsidiary_list[i].deputy_director_list[j].fam_name + " " + initCorp.subsidiary_list[i].deputy_director_list[j].first_name;
                cout << "\t|" << left << setw(20) << full_name_dir << " | " << setw(20) << full_name_dep_dir << " |" << endl;
            }
        }
        line(44);
        cout << "\t\tIncluding: " << no_dept << " departments\n";
        line(83);
        cout << "\t|" << left << setw(20) << "Department" << " | " << setw(20) << "Leader" << " | " << setw(20) << "Vice Leader" << " | " << setw(13)  << "No.employee"  << " | " << endl;
        line(83);
        for(int k = 0; k < initCorp.subsidiary_list[i].department_list.size(); k++)
        {
            int led = initCorp.subsidiary_list[i].department_list[k].leader_list.size();
            int vice_led = initCorp.subsidiary_list[i].department_list[k].vice_leader_list.size();
            int no_emp = initCorp.subsidiary_list[i].department_list[k].employee_list.size();
            
            if(led == 0)
            {
                Employee new_emp;
                initCorp.subsidiary_list[i].department_list[k].leader_list.push_back(new_emp);
            }
            else if(vice_led == 0)
            {
                Employee new_emp;
                initCorp.subsidiary_list[i].department_list[k].vice_leader_list.push_back(new_emp);
            }

            int max_size_2;

            if(led >= vice_led)
                max_size_2 = led;
            else
                max_size_2 = vice_led;

            for(int t = 0; t < max_size_2; t++)
            {
                if(t == 0 && led != 0 && vice_led != 0)
                    cout << "\t|" << left << setw(20) << initCorp.subsidiary_list[i].department_list[k].name << " | " 
                                  << setw(20) << initCorp.subsidiary_list[i].department_list[k].leader_list[t].fam_name + " " + initCorp.subsidiary_list[i].department_list[k].leader_list[t].first_name << " | "
                                  << setw(20) << initCorp.subsidiary_list[i].department_list[k].vice_leader_list[t].fam_name + " " + initCorp.subsidiary_list[i].department_list[k].vice_leader_list[t].first_name << " | "
                                  << setw(13) << no_emp << " | " << endl;
                else if(t == 0 && led != 0 && vice_led == 0)
                    cout << "\t|" << left << setw(20) << initCorp.subsidiary_list[i].department_list[k].name << " | " 
                                  << setw(20) << initCorp.subsidiary_list[i].department_list[k].leader_list[t].fam_name + " " + initCorp.subsidiary_list[i].department_list[k].leader_list[t].first_name << " | "
                                  << setw(20) << "" << " | "
                                  << setw(13) << no_emp << " | " << endl;
                else if(t == 0 && led == 0 && vice_led != 0)
                    cout << "\t|" << left << setw(20) << initCorp.subsidiary_list[i].department_list[k].name << " | " 
                                  << setw(20) << "" << " | "
                                  << setw(20) << initCorp.subsidiary_list[i].department_list[k].vice_leader_list[t].fam_name + " " + initCorp.subsidiary_list[i].department_list[k].vice_leader_list[t].first_name << " | "
                                  << setw(13) << no_emp << " | " << endl;
                else if(t == vice_led)
                    cout << "\t|" << left << setw(20) << "" << " | " 
                                  << setw(20) << initCorp.subsidiary_list[i].department_list[k].leader_list[t].fam_name + " " + initCorp.subsidiary_list[i].department_list[k].leader_list[t].first_name << " | "
                                  << setw(20) << "" << " | "
                                  << setw(13) << "" << " | " << endl;
                else if(t == led)
                    cout << "\t|" << left << setw(20) << "" << " | " 
                                  << setw(20) << "" << " | "
                                  << setw(20) << initCorp.subsidiary_list[i].department_list[k].vice_leader_list[t].fam_name + " " + initCorp.subsidiary_list[i].department_list[k].vice_leader_list[t].first_name << " | "
                                  << setw(13) << "" << " | " << endl;
                else
                    cout << "\t|" << left << setw(20) << "" << " | " 
                                  << setw(20) << initCorp.subsidiary_list[i].department_list[k].leader_list[t].fam_name + " " + initCorp.subsidiary_list[i].department_list[k].leader_list[t].first_name << " | "
                                  << setw(20) << initCorp.subsidiary_list[i].department_list[k].vice_leader_list[t].fam_name + " " + initCorp.subsidiary_list[i].department_list[k].vice_leader_list[t].first_name << " | "
                                  << setw(13) << "" << " | " << endl;
            }
            line(83);
        }
        cout << endl;
    }
}


//--------MODULE 3: TÌM KIẾM NHÂN VIÊN | L.M.HOANG----------//
void readInfo(Employee person )
{
    cout << setw(15) <<left << "ID: "           << "| "<<person.id << endl;
    cout << setw(15) <<left << "First Name: "   << "| "<<person.fam_name << endl;
    cout << setw(15) <<left << "Name: "         << "| "<<person.first_name << endl;
    cout << setw(15) <<left << "Position: "     << "| "<<person.position << endl;
    cout << setw(15) <<left << "Date of Birth: "<< "| "<<person.dob <<endl ;
    cout << setw(15) <<left << "Hometown: "     << "| "<<person.hometown <<endl;
    cout << setw(15) <<left << "Adress:"        << "| "<<person.address <<endl;
    cout << setw(15) <<left << "Email: "        << "| "<<person.email <<endl;
    cout << setw(15) <<left << "Phone number:"  << "| "<<person.phone_number <<endl;
    cout << setw(15) <<left << "Start of work: "<< "| "<<person.start_of_work <<endl;
    cout << setw(15) <<left << "Day Work:" << endl;
    for (string dt : person.date_time)
        cout << dt<< endl; 
}
vector<Employee> findInfo()
{   
    string find_first_name, find_fam_name ,name;
    cin.ignore();
    cout <<"Please enter name: "<<endl;
    getline(cin,name);
    // Kiểm tra HỌ VÀ TÊN
    for (int i = name.length() - 1; i > -1; i--) {
        if (name[i] ==' ')
        {
            find_first_name = name.substr(i+1);
            find_fam_name = name.substr(0,i);
            break;
        }
    }
    vector<Employee> count;
    bool isReal = false ;
    for (const Employee & person: initCorp.manage)
    {
        if (find_first_name == person.first_name and find_fam_name == person.fam_name)
        {  
            isReal = true ;
            count.push_back(person);
        }
    }
    if (count.size()==1)
        cout << "There are " << count.size() << " valid result";
    else
        cout << "There are " << count.size() << " valid results";
    if(isReal == false)
        cout <<"DATA NOT FOUND. PLEASE TRY AGAIN!";
    return count;   
}


//--------MODULE 4: CHAM CONG | T.D.MINH----------//
int isEarly(string t)
{
    string defaultt = "08:00";
    vector<int> ans;
    for (int i = 0; i < 5; i++)
        ans.push_back((t[i] - 'a') - (defaultt[i] - 'a'));
    int anss = 0;
    for (int i = 0; i < 5; i++)
    {
        if (i == 0)
            anss += ans[i] * 600;
        else if (i == 1)
            anss += ans[i] * 60;
        else if (i == 3)
            anss += ans[i] * 10;
        else if (i == 4)
            anss += ans[i];
    }
    return anss;
}

int isLate(string t)
{
    string defaultt = "17:00";
    vector<int> ans;
    for (int i = 0; i < 5; i++)
        ans.push_back((t[i] - 'a') - (defaultt[i] - 'a'));
    int anss = 0;
    for (int i = 0; i < 5; i++)
    {
        if (i == 0)
            anss += ans[i] * 600;
        else if (i == 1)
            anss += ans[i] * 60;
        else if (i == 3)
            anss += ans[i] * 10;
        else if (i == 4)
            anss += ans[i];
    }
    return anss;
}

bool isDayOff(string day)
{
    string sat[52] = { "07/01/2023","14/01/2023","21/01/2023","28/01/2023","04/02/2023","11/02/2023","18/02/2023",
        "25/02/2023","04/03/2023","11/03/2023","18/03/2023","25/03/2023","01/04/2023","08/04/2023",
        "15/04/2023","22/04/2023","29/04/2023","06/05/2023","13/05/2023","20/05/2023","27/05/2023",
        "03/06/2023","10/06/2023","17/06/2023","24/06/2023","01/07/2023","08/07/2023","15/07/2023","22/07/2023"
        ,"29/07/2023","05/08/2023","12/08/2023","19/08/2023","26/08/2023","02/09/2023","09/09/2023"};
    string sun[52] = { "08/02/2023","15/01/2023","22/01/2023","29/01/2023","05/02/2023","12/02/2023","19/02/2023"
         ,"26/02/2023","05/03/2023","12/03/2023","19/03/2023","26/03/2023","02/04/2023","09/04/2023",
         "16/04/2023","23/04/2023","30/04/2023","07/05/2023","14/05/2023","21/05/2023","28/05/2023",
         "04/06/2023","11/06/2023","18/06/2023","25/06/2023","02/07/2023","09/07/2023","16/07/2023","23/07/2023"
         ,"30/07/2023","06/08/2023","13/08/2023","20/08/2023","27/08/2023","03/09/2023","10/09'2023" };
    for (int i = 0; i < 52; i++)
        if (day == sat[i])
            return true;
    return false;
}

string getDay(string t)
{
    string day = "";
    for (int i = 0; i < 10; i++)
        day += t[i];
    return day;
}

string calculateArrive(string t, string day)
{
    //t = "18/05/2003,08:00,09:00"
    string arrival_time = "";
    for (int i = 11; i < 16; i++)
        arrival_time += t[i];
    //print
    string status = "";
    if (isEarly(arrival_time) > 0)
    {
        int timeLate = isEarly(arrival_time);
        timeLate % 60 == 0 ? timeLate /= 60 : timeLate = (timeLate / 60) + 1;
        status += "late " + to_string(timeLate) + " hours";
    }
    else
    {
        status += "on time";
    }
    if (isDayOff(day))
        status = "on time";
    return status;
}

string calculateLeave(string t, string day)
{
    //t = "18/05/2003,08:00,09:00"
    string leave_time = "";
    for (int i = 17; i < 22; i++)
        leave_time += t[i];
    //print
    string status = "";
    if (isLate(leave_time) < 0)
    {
        int timeLeave = abs(isLate(leave_time));
        timeLeave % 60 == 0 ? timeLeave /= 60 : timeLeave = (timeLeave / 60) + 1;
        status += "leave early " + to_string(timeLeave) + " hours";
    }
    else
    {
        status += "on time";
    }

    if (isDayOff(day))
        status = "on time";
    return status;
}

void displayModule4()
{
    cout << setw(10) << "date" << setw(15) << "|" << setw(20) << "arrive time" << setw(23) << "|" << setw(25) << "leave time" << "\n";
}

void displayModule4(string date, string status1, string status2)
{
    cout << setw(10) << date << setw(15) << "|" << setw(20) << status1 << setw(23) << "|" << setw(25) << status2 << "\n";
}

void module4(Employee e)
{
    cout << "Cham cong cua nhan vien voi id la : " << e.id << "\n";
    displayModule4();
    for (int i = 0; i < e.date_time.size(); i++)
    {
        string day = getDay(e.date_time[i]);
        string status1 = calculateArrive(e.date_time[i], day);
        string status2 = calculateLeave(e.date_time[i], day);
        displayModule4(day, status1, status2);
    }
}

bool find_emp_id(string id)
{
    for(int i = 0; i < initCorp.manage.size(); i++)
    {
        if(id == initCorp.manage[i].id)
        {
            module4(initCorp.manage[i]);
            return true;
        }
    }
    return false;
}


//--------MODULE 5: HIEN THI THONG TIN SUB/DEPT | L.M.HOANG----------//


//--------MODULE 6: ADD EMPLOYEE| TRAN VAN DUC----------//


//--------MODULE 7: UPDATE EMPLOYEE| NGUYEN DUY THANH----------//

int main()
{
    // Tải thông tin từ file text lên, nếu không tải được thì dừng chương trình
    if(upload() == true)
    {
        // Menu-base
        while(true)
        {
            string option;
            menu_line(50);
            cout << left << setw(50) << "| OPTION LIST | TYPE 1 ~ 7 TO SELECT THE OPTION:" << " |" << endl;
            menu_line(50);
            cout << left << setw(50) << "| 1 - Display Corporation's Data." << " |" << endl;
            cout << left << setw(50) << "| 2 - Search Employee's Data." << " |" << endl;
            cout << left << setw(50) << "| 3 - Display data" << " |" << endl;
            cout << left << setw(50) << "| 4 - Display Subsidiary's Data." << " |" << endl;
            cout << left << setw(50) << "| 5 - Add New Employee" << " |" << endl;
            cout << left << setw(50) << "| 6 - Update Data" << " |" << endl;
            cout << left << setw(50) << "| 7 - Exit Program." << " |" << endl;
            menu_line(50);
            cout << "Input your option: ";
            cin >> option;

            if(option == "1") 
                {display_corp_data();}

            else if(option == "2")
            {
                vector<Employee> count = findInfo();
                for (const Employee & person: count)
                {
                    cout << " -----------------------"<<endl;
                    readInfo(person);
                }
            }

            else if(option == "3")
            {
                string id_emp;
                cout << "Input employee's ID: ";
                cin >> id_emp;
                bool emp = find_emp_id(id_emp);
                if(!emp)
                    cout << "DATA NOT FOUND\n";
            }

            else if(option == "4"){}
            else if(option == "5"){}
            else if(option == "6"){}

            else if(option == "7")
            {
                cout << "ENDING PROGRAM!" << endl;
                break;
            }
            else cout << "ERROR INPUT - PLEASE TRY AGAIN";
            cout << "Press any key to continue......\n";

            cin.ignore();
            getline(cin, option);
            system("cls");
            }
    }

    return 0;
}
#include <iostream> // Luồng vào ra 
#include <string> // String
#include <vector> // Lưu trữ dữ liệu
#include <fstream> // Đọc file
#include <iomanip> // Thiết lập định dạng
#include "struct.h" // File header chứa struct

// Bộ tiền xử lý này để xóa màn hình console
// #ifdef _WIN32
// #include <windows.h>
// #else
// #include <unistd.h>
// #endif

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
void corp_division(Employee person)
{
    bool is_chairman = ( person.position == "Chu tich" || person.position == "chu tich");
    bool is_vice_chairman = (person.position ==  "Pho chu tich" || person.position == "pho chu tich");
    
    if(is_chairman)
        initCorp.chairman_list.push_back(person);
    
    else if (is_vice_chairman)
        initCorp.vice_chairman_list.push_back(person);

}

void sub_division(Employee person)
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

void depart_division(Employee person)
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
        depart_division(person);
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
                corp_division(person);
            }

            // Phân bổ thằng này vào vị trí Subsidiary
            else if(slash == 1)
            {
                sub_division(person);
            }

            // Phân bổ thằng này vào vị trí Department
            else if(slash == 2)
            {
                depart_division(person);
            }
        }
    }
    input.close();
    return true;
}


//--------MODULE 2: HIỂN THỊ DỮ LIỆU | N.T.D.HANG------------//
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

    //Hiển thị thông tin chủ tịch và pct
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
    // In số lượng công ty con
    cout << "\tOwns " << n << " subsidiary companies" << endl;

    //Hiển thị thông tin CÔNG TY CON
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
                string full_name_dir = initCorp.subsidiary_list[i].director_list[j].fam_name + " " + initCorp.subsidiary_list[i].director_list[j].first_name;
                string full_name_dep_dir = initCorp.subsidiary_list[i].deputy_director_list[j].fam_name + " " + initCorp.subsidiary_list[i].deputy_director_list[j].first_name;
                cout << "\t|" << left << setw(20) << full_name_dir << " | " << setw(20) << full_name_dep_dir << " |" << endl;
            }
        }
        line(44);
        cout << "\t\tIncluding: " << no_dept << " departments\n";
        line(83);
        cout << "\t|" << left << setw(20) << "Department" << " | " << setw(20) << "Leader" << " | " << setw(20) << "Vice Leader" << " | " << setw(13)  << "No.employee"  << " | " << endl;
        line(83);

        // In thông tin phòng ban
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

            // In ra trưởng phòng và ptp
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
    string emp_unit = person.parent_unit + "/" + person.unit;
    cout << "\t| " << setw(15) <<left << "ID: "           << "| " << setw(24)<<person.id << "|" << endl;
    cout << "\t| " << setw(15) <<left << "First Name: "   << "| " << setw(24)<<person.fam_name << "|" << endl;
    cout << "\t| " << setw(15) <<left << "Name: "         << "| " << setw(24)<<person.first_name << "|" << endl;
    cout << "\t| " << setw(15) <<left << "Unit: "         << "| " << setw(24)<<emp_unit << "|" << endl;
    cout << "\t| " << setw(15) <<left << "Position: "     << "| " << setw(24)<<person.position << "|" << endl;
    cout << "\t| " << setw(15) <<left << "Date of Birth: "<< "| " << setw(24)<<person.dob << "|" <<endl ;
    cout << "\t| " << setw(15) <<left << "Hometown: "     << "| " << setw(24)<<person.hometown << "|" <<endl;
    cout << "\t| " << setw(15) <<left << "Adress:"        << "| " << setw(24)<<person.address << "|" <<endl;
    cout << "\t| " << setw(15) <<left << "Email: "        << "| " << setw(24)<<person.email << "|" <<endl;
    cout << "\t| " << setw(15) <<left << "Phone number:"  << "| " << setw(24)<<person.phone_number << "|" <<endl;
    cout << "\t| " << setw(15) <<left << "Start of work: "<< "| " << setw(24)<<person.start_of_work << "|" <<endl;
    cout << "\t| " << setw(41) <<left << "Day Work:" << "|" << endl;
    for (string dt : person.date_time)
        cout << "\t| " << setw(41) << dt << "|"<< endl; 
}
vector<Employee> findInfo()
{   
    string find_first_name, find_fam_name ,name;
    cin.ignore();
    cout <<"Please enter name: ";
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
    for (int i = 0; i < initCorp.manage.size(); i++)
    {
        if (name == initCorp.manage[i].fam_name + " " + initCorp.manage[i].first_name)
        {  
            isReal = true ;
            count.push_back(initCorp.manage[i]);
        }
    }
    if (count.size()==1)
        cout << "There is " << count.size() << " valid result";
    else
        cout << "There are " << count.size() << " valid results";
    if(isReal == false)
        cout <<"DATA NOT FOUND. PLEASE TRY AGAIN!\n";
    return count;   
}


//--------MODULE 4: CHAM CONG | T.D.MINH-------------------//
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
    cout << setw(20) << "   DATE" << setw(15) << "|" << setw(20) << "ARRIVE TIME" << setw(23) << "|" << setw(25) << "LEAVE TIME" << "\n";
}

void displayModule4(string date, string status1, string status2)
{
    cout << setw(20) << date << setw(15) << "|" << setw(20) << status1 << setw(23) << "|" << setw(25) << status2 << "\n";
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
void displaySub(vector<Subsidiary>&manage_sub)
{
    string find_sub;
    
    cout <<"This is the subsidiary list: "<<endl;
    // Show the list of subsidiary
    for(const Subsidiary sub: manage_sub)
    {   
        cout<<"- "<< sub.name <<endl;
    }
    cout <<" "<<endl;
    cout <<"Please enter the subsidiary: ";
    cin.ignore();
    getline(cin,find_sub);
    bool isReal = false ;
    // Find the sub
    for(const Subsidiary sub: manage_sub)
    {
        if(find_sub == sub.name)
        {   
            isReal = true;
            
            cout << left << setw(20) <<"Director" << " : ";
            // if list empty, leave a blank
            if(sub.director_list.empty())
            {
                cout <<"none"<<endl;
            }
            else
            {
                for(const Employee person: sub.director_list)
                {
                    string full_name = person.fam_name + " " + person.first_name;
                    cout << full_name << " . ";
                }
                cout << endl;
            }
            cout << left << setw(20) <<"Deputy Director" << " : ";
            if(sub.deputy_director_list.empty())
            {
                cout <<"none"<<endl;
            }
            else
            {
                for(const Employee person: sub.deputy_director_list)
                {
                    string full_name = person.fam_name + " " + person.first_name;
                    cout << full_name << " . ";
                }
                cout << endl;
            }
            

            line(39);
            cout << "\t| " << left << setw(20) << "DEPARTMENT" << " | " << setw(15) << "No.employee" << "|" << endl;
            line(39);
            for(int i = 0; i < sub.department_list.size(); i++)
            {
                int no_emp = sub.department_list[i].employee_list.size();
                cout << "\t| " << left << setw(20) << sub.department_list[i].name << " | " << setw(15) << no_emp << "|" << endl;
            }
            line(39);

            string option;
            cout << "Do you want to see the employee list?"<<endl;
            cout << "Input (yes) to continue or any thing to end: " ;
            cin >> option;
            if(option == "yes" || option == "y" || option == "Yes")
            {   
                string department;
                cout << "Please enter the department: "<<endl;
                
                cin >>department ;
                // Flag 
                bool isFound = false;
                for(const Department depart: sub.department_list)
                {
                    if(department == depart.name)
                    {   
                        isFound = true;

                        // Print leader's name
                        cout << left << setw(20) << "Leader" << " : ";
                        if(depart.leader_list.empty())
                            cout << "none" << endl;
                        else
                        {
                            for(const Employee person: depart.leader_list)
                            {
                                string full_name = person.fam_name + " " + person.first_name;
                                cout << full_name << " . " ;
                            }
                            cout << endl;
                        }

                        // Print vice's leader name
                        cout << left  << setw(20) << "Vice-leader" << " : ";
                        if(depart.vice_leader_list.empty())
                            cout << "none" << endl;
                        else
                        {
                            for(const Employee person: depart.vice_leader_list)
                            {
                                string full_name = person.fam_name + " " + person.first_name;
                                cout << full_name << " . "<<endl;
                            }
                            cout <<endl;
                        }
                
                        // Print information of department's employee 
                        cout <<endl;
                        line(48);
                        cout << "\t| " << left << setw(10) << "ID" << " | " << setw(34) << "EMPLOYEE NAME" << "|" << endl;
                        line(48);
                        for(const Employee person: depart.employee_list)
                        {
                            string full_name =  person.fam_name + " " + person.first_name;
                            cout << "\t| " << left << setw(10) << person.id << " | " << setw(34) << full_name << "|" << endl;
                        }
                        line(48);
                    }
                }
                if(isFound==false)
                    cout <<"Please enter again."<<endl;
            }
            else
                cout<<"End the function! ";
        }
    }  
    if(isReal==false)
        cout <<"Please enter again. ";
}


//--------MODULE 6: ADD EMPLOYEE| TRAN VAN DUC----------//
// Hàm set nhân viên vào vị trí 
void set_position(Employee employee, int option, string position, string subsidiary, string department)
{
    string position_input = position;
    // Phân bổ thằng này vào vị trí Corp
    if(option == 1)
    {
        bool is_chairman = ( position_input == "Chu tich" || position_input == "chu tich");
        bool is_vice_chairman = (position_input ==  "Pho chu tich" || position_input == "pho chu tich");

        if(is_chairman)
        {
            if(initCorp.chairman_list[0].id == "null_id")
                initCorp.chairman_list[0] = employee;
            else
                initCorp.chairman_list.push_back(employee);
        }
            
        else if (is_vice_chairman)
            if(initCorp.vice_chairman_list[0].id == "null_id")
                initCorp.vice_chairman_list[0] = employee;
            else
                initCorp.vice_chairman_list.push_back(employee);
    }

    // Phân bổ thằng này vào vị trí Subsidiary
    else if(option == 2)
    {
        bool found = false;
        bool is_director = (position_input == "Giam doc" || position_input == "giam doc");
        bool is_vice_director = (position_input == "Pho giam doc" || position_input == "pho giam doc");

        for(int i = 0; i < initCorp.subsidiary_list.size(); i++)
        {
            if(subsidiary == initCorp.subsidiary_list[i].name)
            {
                if(is_director)
                {
                    if(initCorp.subsidiary_list[i].director_list[0].id == "null_id")
                        initCorp.subsidiary_list[i].director_list[0] = employee;
                    else
                        initCorp.subsidiary_list[i].director_list.push_back(employee);
                }
                    
                else if(is_vice_director)
                {
                    if(initCorp.subsidiary_list[i].deputy_director_list[0].id == "null_id")
                        initCorp.subsidiary_list[i].deputy_director_list[0] = employee;
                    else
                        initCorp.subsidiary_list[i].deputy_director_list.push_back(employee);
                }
                    
                else
                {
                    initCorp.subsidiary_list[i].employee_list.push_back(employee);
                }
                found = true;
            }
        }

        if(!found)
        {
            Subsidiary new_sub;
            new_sub.name = subsidiary;
            if(is_director)
                new_sub.director_list.push_back(employee);
            else if(is_vice_director)
                new_sub.deputy_director_list.push_back(employee);
            else
                new_sub.employee_list.push_back(employee);

            initCorp.subsidiary_list.push_back(new_sub);
        }
    }

    // Phân bổ thằng này vào vị trí Department
    else if(option == 3)
    {
        bool found = false;
        bool flag = false;
        bool is_leader = (position_input == "Truong phong" || position_input == "truong phong");
        bool is_vice_leader = (position_input == "Pho phong" || position_input == "pho phong");
        for(int i = 0; i < initCorp.subsidiary_list.size(); i++)
        {
            if(subsidiary == initCorp.subsidiary_list[i].name)
            {
                for(int j = 0; j < initCorp.subsidiary_list[i].department_list.size(); j++)
                {
                    if(initCorp.subsidiary_list[i].department_list[j].name == department)
                    {
                        if(is_leader)
                        {
                            if(initCorp.subsidiary_list[i].department_list[j].leader_list.size() == 0)
                                initCorp.subsidiary_list[i].department_list[j].leader_list[0] = employee;
                            else
                                initCorp.subsidiary_list[i].department_list[j].leader_list.push_back(employee);
                        }
                            
                        else if(is_vice_leader)
                            if(initCorp.subsidiary_list[i].department_list[j].vice_leader_list[0].id == "null_id")
                                initCorp.subsidiary_list[i].department_list[j].vice_leader_list[0] = employee;
                            else
                                initCorp.subsidiary_list[i].department_list[j].vice_leader_list.push_back(employee);
                        else
                            initCorp.subsidiary_list[i].department_list[j].employee_list.push_back(employee);
                        found = true;
                        flag = true;
                        break;
                    }
                }

                if(!found)
                {
                    Department new_depart;
                    new_depart.name = department;
                    if(is_leader)
                        new_depart.leader_list.push_back(employee);
                    else if(is_vice_leader)
                        new_depart.vice_leader_list.push_back(employee);
                    else
                        new_depart.employee_list.push_back(employee);
                    initCorp.subsidiary_list[i].department_list.push_back(new_depart);
                    flag = true;
                    break;
                }
            }
        }
        if(!flag)
        {
            Subsidiary new_sub;
            new_sub.name = subsidiary;
            initCorp.subsidiary_list.push_back(new_sub);
            depart_division(employee);
        }
    }

    initCorp.manage.push_back(employee);
}

//Hàm đưa ra vị trí nhân viên 
string display_position(int option)
{
    string position;
    int opt;
    bool to_continue = true;
    cout << "Choose the position you want to add:\n";
    if (option == 1)
    {   
        do
        {
            cout << "1-Chu tich\n";
            cout << "2-Pho chu tich\n";
            cin >> opt;
            if (opt == 1)
            {
                position = "Chu tich";
                to_continue = false;
            }
            else if (opt == 2)
            {
                position = "Pho chu tich";
                to_continue = false;
            }
            else 
            {
                cout << "Please enter number 1 or 2.\n";
            }
        } while (to_continue);
    }
    else if (option == 2)
    {
        do
        {
            cout << "1-Giam doc\n";
            cout << "2-Pho giam doc\n";
            cin >> opt;
            if (opt == 1)
            {
                position = "Giam doc";
                to_continue = false;
            }
            else if (opt == 2)
            {
                position = "Pho giam doc";
                to_continue = false;
            }
            else 
            {
                cout << "Please enter number 1 or 2.\n";
            }
        } while (to_continue);
    }
    else if (option == 3)
    {
        do
        {
            cout << "1-Truong phong\n";
            cout << "2-Pho phong\n";
            cout << "3-Nhan vien\n";
            cin >> opt;
            if (opt == 1)
            {
                position = "Truong phong";
                to_continue = false;
            }
            else if (opt == 2)
            {
                position = "Pho phong";
                to_continue = false;
            }
            else if (opt == 3)
            {
                position = "Nhan vien";
                to_continue = false;
            }
             else 
            {
                cout << "Please enter number 1 or 2 or 3\n";
            }
        } while (to_continue);
        
    }
    return position;
}

//Hàm đưa ra các subsidiary 
string display_subsidiary()
{
    string subsidiary;
    cout << endl;
    cout << "Here is all subsidiary in our corporation:\n";
    cout << endl << "List of subsidiary | ";
    for (int i = 0; i < initCorp.subsidiary_list.size(); i++)
    {
        cout << initCorp.subsidiary_list[i].name << "  ";
    }
    cout << endl << "Please enter subsidiary you want to add employee: ";
    cin >> subsidiary;
    return subsidiary;
}

//Hàm đưa ra các department
string display_department(string sub)
{
    int argument_sub = 0;
    string department;

    for (int i = 0; i < initCorp.subsidiary_list.size(); i++)
    {
        if (sub == initCorp.subsidiary_list[i].name)
        {
            cout << endl << "Here is all department you can add employee:\n";
            cout << "List of department | ";
            argument_sub = i;
        }
    }

    for (int i = 0; i < initCorp.subsidiary_list[argument_sub].department_list.size(); i++)
    {
        cout << initCorp.subsidiary_list[argument_sub].department_list[i].name << " ";
    }
    cout << endl << "Please enter the department you want to add emplyee: ";
    cin >> department;
    return department;
}

//Hàm check subsidiary nhập vào
string check_subsidiary(string sub )
{
    bool sub_true = true;

    do
    {
        sub = display_subsidiary();
        for (int i = 0; i < initCorp.subsidiary_list.size(); i++)
        {
            if (sub == initCorp.subsidiary_list[i].name)
            {
                sub_true = false;
                break;
            }
        }
        if (sub_true)
        {
            cout << "Subsidiary not found. Please enter again.\n" ;
        }
    } while (sub_true);

    return sub;
}

//Hàm check department nhập vào 
string check_department(string sub, string dept)
{
    bool dept_true = true;
    int argument_sub = 0;
    for (int i = 0; i < initCorp.subsidiary_list.size(); i++)
    {
        if(sub == initCorp.subsidiary_list[i].name)
        {
            argument_sub = i;
        }
    }

    do
    {
        dept = display_department(sub);
        for (int i = 0; i < initCorp.subsidiary_list[argument_sub].department_list.size(); i++)
        {
            if (dept == initCorp.subsidiary_list[argument_sub].department_list[i].name)
            {
                dept_true = false;
                break;
            }
        }
        if (dept_true)
        {
            cout << "Department not found. Please enter again.\n";
        }
    } while (dept_true);
    return dept;
}

//Hàm điền thông tin nhân viên
void add_employee(Employee &employee)
{
    cout << endl;
    cout << "Please enter the employee information following the order below.\n";
    cout << "Please enter the employee id: ";
    cin >> employee.id;
    cin.ignore();
    cout << "Please enter the employee family name: ";
    getline(cin,employee.fam_name);
    cout << "Please enter the employee first name: ";
    getline(cin,employee.first_name);
    cout << "Please enter the employee date of birth: ";
    getline(cin,employee.dob);
    cout << "Please enter the employee hometown: ";
    getline(cin,employee.hometown);
    cout << "Please enter the employee adress: ";
    getline(cin, employee.address);
    cout << "Please enter the employee email: ";
    getline(cin,employee.email);
    cout << "Please enter the employee phone number: ";
    getline(cin,employee.phone_number);
    cout << "Please enter day start of work: ";
    getline(cin,employee.start_of_work);
}

//Hàm lưu thông tin nhân viên mới vào file demodata
void save_demodata(Employee employee, string sub, string dept, string position, int option)
{
    ofstream data;
    data.open("demodata.txt", ios::app);

    if(!data)
    {
        cout << "Error: file not opened." << endl;
    }
    else
    {
        data << endl;
        data << employee.id << endl;
        data << employee.fam_name << endl;
        data << employee.first_name << endl;

        if (option == 1)
            data << "Bkcorporation" << endl;
        else if (option == 2)
            data << "Bkcorporation/" + sub << endl;
        else if (option == 3)
            data << "Bkcorporation/" + sub + "/" + dept << endl;

        data << position << endl;
        data << employee.dob << endl;
        data << employee.hometown << endl;
        data << employee.address << endl;
        data << employee.email << endl;
        data << employee.phone_number << endl;
        data << employee.start_of_work << endl;
    }

}

//Hàm lưu thông tin nhân viên mới vào file data chính 
void save_data()
{
    string line;

    //Open file
    ifstream demo_data{ "demodata.txt",ios::in};
    ofstream data{ "data.txt",ios::app};

    //Copy data from demodata.txt to data.txt
    if (demo_data && data)
    {
        while (getline(demo_data, line))
        {
            data << line << endl;
        }
        cout << "Copy Finished." << endl;
    }
    else
    {
        cout << "Cannot read File" << endl;
    }

    //Close file
    demo_data.close();
    data.close();
}

//Hàm chính 
void display_menu()
{
    int option;
    string subsidiary_input, department_input,position;
    Employee employee_add;
    bool to_continue = true;

    //Menu base(Choose option)
    while(true)
    {
        //Hiển thị Menu
        cout << endl;
        cout << "//-- HERE IS THE MENU TO ADD NEW EMPLOYEE(ENTER NUMBER 1 TO 6) --//" << endl;
        cout << "1 - Add employees to the executive position of the corporation." << endl;
        cout << "2 - Add employees to the executive position of the subsidiary." << endl;
        cout << "3 - Add employees in a department." << endl;
        cout << "4 - Save the data into the data.txt file." << endl;
        cout << "5 - Clear all data in the demodata.txt file." << endl;
        cout << "6 - End the module." << endl;
        cout << "Please choose your option:" << endl;
        cin >> option;
            //Add vị trí điều hành tổng công ty(Chủ tịch,phó chủ tịch)
            if(option == 1)
            {
                position = display_position(option); //chu tich
                add_employee(employee_add);
                set_position(employee_add, option, position, subsidiary_input, department_input);
                save_demodata(employee_add, subsidiary_input, department_input,position, option);
                cout << endl;
            }

            //Add vị trí điều hành công ty con(Giám đốc công ty, phó giám đốc công ty)
            else if(option == 2) 
            {
                subsidiary_input = check_subsidiary(subsidiary_input);
                position = display_position(option);
                add_employee(employee_add);
                set_position(employee_add, option, position, subsidiary_input, department_input);
                save_demodata(employee_add, subsidiary_input, department_input,position, option);
            }

            //Add vị trí trong phòng ban(Trưởng phòng, phó phòng, nhân viên)
            else if(option == 3) 
            {
                subsidiary_input = check_subsidiary(subsidiary_input);
                department_input = check_department(subsidiary_input, department_input);
                position = display_position(option);
                add_employee(employee_add);
                set_position(employee_add, option, position, subsidiary_input, department_input);
                save_demodata(employee_add, subsidiary_input, department_input,position, option);
                cout << endl;
            }

            //Save data into data.txt
            else if(option == 4) 
            {
                save_data();
                cout << endl;
            }

            //Clear demodata
            else if(option == 5) 
            {
                ofstream demo_data;
                demo_data.open("demodata.txt",ios::trunc);
                cout << "Clear finished." << endl;
                demo_data.close();
            }

            // End module
            else if(option == 6) 
            {
                to_continue = false;
                break;
            }

            else
                cout << "INVALID INPUT. PLEASE TRY AGAIN\n";
        
    }

}


//--------MODULE 7: UPDATE EMPLOYEE| NGUYEN DUY THANH----------//
void readInfoNoDateTime(Employee person)
{
    cout << setw(15) <<left << "ID: "           << "| "<<person.id << endl;
    cout << setw(15) <<left << "First Name: "   << "| "<<person.fam_name << endl;
    cout << setw(15) <<left << "Name: "         << "| "<<person.first_name << endl;
    cout << setw(15) <<left << "Unit: "     << "| "<<person.unit << endl;
    cout << setw(15) <<left << "Position: "     << "| "<<person.position << endl;
    cout << setw(15) <<left << "Date of Birth: "<< "| "<<person.dob <<endl ;
    cout << setw(15) <<left << "Hometown: "     << "| "<<person.hometown <<endl;
    cout << setw(15) <<left << "Adress:"        << "| "<<person.address <<endl;
    cout << setw(15) <<left << "Email: "        << "| "<<person.email <<endl;
    cout << setw(15) <<left << "Phone number:"  << "| "<<person.phone_number <<endl;
    cout << setw(15) <<left << "Start of work: "<< "| "<<person.start_of_work <<endl;

}

vector<int> find_employee(string name) {
    vector<int> order;
    int count = 1;
    for (int i = 0; i < initCorp.manage.size(); i++)
    {
        if (initCorp.manage[i].fam_name + " " + initCorp.manage[i].first_name == name)
        {
            order.push_back(i);
            cout << "Employee " << count++ << ": \n";
            readInfoNoDateTime(initCorp.manage[i]);
        }
    }
    return order;
}

void find_employee_order(Employee person, bool& is_main_role, bool& is_part_role, int& order_sub, int& order_dep, int& order_subE, int& order_depE, int& slash, int& order_corE) {
    order_sub = order_dep = order_subE = order_depE = order_corE = 0;
    string parent_unit = person.parent_unit;
    string unit = person.unit;
    if (parent_unit == "")
        slash = 0;
    else if (parent_unit == "BKcorporation")
        slash = 1;
    else
        slash = 2;
    if (slash == 0)
    {
        for (order_corE = 0; order_corE < initCorp.chairman_list.size(); order_corE++)
        {
            if (person == initCorp.chairman_list[order_corE])
            {
                is_main_role = true;
                return;
            }
        }
        if (!is_main_role)
        {
            for (order_corE = 0; order_corE < initCorp.vice_chairman_list.size(); order_corE++)
            {
                if (person == initCorp.vice_chairman_list[order_corE])
                {
                    is_part_role = true;
                    return;
                }
            }
        }
    }
    else if (slash == 1)
    {
        // Tìm công ty con
        for (order_sub; order_sub < initCorp.subsidiary_list.size(); order_sub++)
        {
            if (unit == initCorp.subsidiary_list[order_sub].name)
            {
                for (order_subE = 0; order_subE < initCorp.subsidiary_list[order_sub].director_list.size(); order_subE++)
                {
                    if (person == initCorp.subsidiary_list[order_sub].director_list[order_subE])
                    {
                        is_main_role = true;
                        break;
                    }
                }
                if (!is_main_role)
                {
                    for (order_subE = 0; order_subE < initCorp.subsidiary_list[order_sub].deputy_director_list.size(); order_subE++)
                    {
                        if (person == initCorp.subsidiary_list[order_sub].deputy_director_list[order_subE])
                        {
                            is_part_role = true;
                            break;
                        }
                    }
                }
                if (!is_main_role && !is_part_role)
                {
                    // Tìm nhân viên trong công ty
                    for (order_subE = 0; order_subE < initCorp.subsidiary_list[order_sub].employee_list.size(); order_subE++)
                    {
                        if (person == initCorp.subsidiary_list[order_sub].employee_list[order_subE])
                            break;
                    }
                }
                return;
            }
        }
    }
    else {
        // Tìm công ty
        for (order_sub = 0; order_sub < initCorp.subsidiary_list.size(); order_sub++)
        {
            if (parent_unit == initCorp.subsidiary_list[order_sub].name)
            {
                // Tìm phòng ban
                for (order_dep = 0; order_dep < initCorp.subsidiary_list[order_sub].department_list.size(); order_dep++)
                {
                    if (unit == initCorp.subsidiary_list[order_sub].department_list[order_dep].name)
                    {
                        for (order_depE = 0; order_depE <
                                         initCorp.subsidiary_list[order_sub].department_list[order_dep].leader_list.size(); order_depE++)
                        {
                            if (person == initCorp.subsidiary_list[order_sub].department_list[order_dep].leader_list[order_depE])
                            {
                                is_main_role = true;
                                break;
                            }
                        }
                        if (!is_main_role)
                            for (order_depE = 0; order_depE < initCorp.subsidiary_list[order_sub].department_list[order_dep].vice_leader_list.size(); order_depE++)
                            {
                                if (person == initCorp.subsidiary_list[order_sub].department_list[order_dep].vice_leader_list[order_depE])
                                {
                                    is_main_role = true;
                                    break;
                                }
                            }

                        if (!is_main_role && !is_part_role)
                        {
                            // Tìm nhân viên trong phòng ban
                            for (order_depE = 0; order_depE < initCorp.subsidiary_list[order_sub].department_list[order_dep].employee_list.size(); order_depE++)
                            {
                                if (person == initCorp.subsidiary_list[order_sub].department_list[order_dep].employee_list[order_depE])
                                    break;
                            }
                        }
                        return;
                    }
                }
            }
        }
    }
}

void delete_employee (int tmp, int order_sub, int order_dep, int order_subE, int order_depE, int order_corE, bool is_main_role, bool is_part_role, int slash) {
    if (slash == 0)
    {
        if (is_main_role)
            initCorp.chairman_list.erase(initCorp.chairman_list.begin() + order_corE);
        else
            initCorp.vice_chairman_list.erase(initCorp.chairman_list.begin() + order_corE);
    }
    else if (slash == 1)
    {
        if (is_main_role)
            initCorp.subsidiary_list[order_sub].director_list.erase(initCorp.subsidiary_list[order_sub].director_list.begin() + order_subE);
        else if (is_part_role)
            initCorp.subsidiary_list[order_sub].deputy_director_list.erase(initCorp.subsidiary_list[order_sub].deputy_director_list.begin() + order_subE);
        else
            initCorp.subsidiary_list[order_sub].employee_list.erase(initCorp.subsidiary_list[order_sub].employee_list.begin() + order_subE);
    }
    else
    {
        if (is_main_role)
            initCorp.subsidiary_list[order_sub].department_list[order_dep].leader_list.erase(initCorp.subsidiary_list[order_sub].department_list[order_dep].leader_list.begin() + order_depE);
        else if (is_part_role)
            initCorp.subsidiary_list[order_sub].department_list[order_dep].vice_leader_list.erase(initCorp.subsidiary_list[order_sub].department_list[order_dep].vice_leader_list.begin() + order_depE);
        else
            initCorp.subsidiary_list[order_sub].department_list[order_dep].employee_list.erase(initCorp.subsidiary_list[order_sub].department_list[order_dep].employee_list.begin() + order_depE);
    }
    initCorp.manage.erase(initCorp.manage.begin() + tmp);
}

void add_employee_1 (Employee person)
{
    int slash = 0;
    string parent_unit = person.parent_unit;
    string unit = person.unit;
    if (parent_unit.empty())
        slash = 0;
    else if (parent_unit == "BKcorporation")
        slash = 1;
    else
        slash = 2;

    string position_input = person.position;
    if(slash == 0)
    {
        bool is_chairman = ( position_input == "Chu tich" || position_input == "chu tich");
        bool is_vice_chairman = (position_input ==  "Pho chu tich" || position_input == "pho chu tich");

        if(is_chairman)
        {
            if(initCorp.chairman_list[0].id == "null_id")
                initCorp.chairman_list[0] = person;
            else
                initCorp.chairman_list.push_back(person);
        }
            
        else if (is_vice_chairman)
            if(initCorp.vice_chairman_list[0].id == "null_id")
                initCorp.vice_chairman_list[0] = person;
            else
                initCorp.vice_chairman_list.push_back(person);
    }
        
    else if(slash == 1)
    {
        bool found = false;
        bool is_director = (position_input == "Giam doc" || position_input == "giam doc");
        bool is_vice_director = (position_input == "Pho giam doc" || position_input == "pho giam doc");

        for(int i = 0; i < initCorp.subsidiary_list.size(); i++)
        {
            if(person.unit == initCorp.subsidiary_list[i].name)
            {
                if(is_director)
                {
                    if(initCorp.subsidiary_list[i].director_list[0].id == "null_id")
                        initCorp.subsidiary_list[i].director_list[0] = person;
                    else
                        initCorp.subsidiary_list[i].director_list.push_back(person);
                }
                    
                else if(is_vice_director)
                {
                    if(initCorp.subsidiary_list[i].deputy_director_list[0].id == "null_id")
                        initCorp.subsidiary_list[i].deputy_director_list[0] = person;
                    else
                        initCorp.subsidiary_list[i].deputy_director_list.push_back(person);
                }
                    
                else
                {
                    initCorp.subsidiary_list[i].employee_list.push_back(person);
                }
                found = true;
            }
        }
    }
        // Phân bổ thằng này vào vị trí Department
    else if(slash == 2)
    {
        bool found = false;
        bool flag = false;
        bool is_leader = (position_input == "Truong phong" || position_input == "truong phong");
        bool is_vice_leader = (position_input == "Pho phong" || position_input == "pho phong");
        for(int i = 0; i < initCorp.subsidiary_list.size(); i++)
        {
            if(person.parent_unit == initCorp.subsidiary_list[i].name)
            {
                for(int j = 0; j < initCorp.subsidiary_list[i].department_list.size(); j++)
                {
                    if(initCorp.subsidiary_list[i].department_list[j].name == person.unit)
                    {
                        if(is_leader)
                        {
                            if(initCorp.subsidiary_list[i].department_list[j].leader_list.size() == 0)
                                initCorp.subsidiary_list[i].department_list[j].leader_list[0] = person;
                            else
                                initCorp.subsidiary_list[i].department_list[j].leader_list.push_back(person);
                        }
                            
                        else if(is_vice_leader)
                            if(initCorp.subsidiary_list[i].department_list[j].vice_leader_list[0].id == "null_id")
                                initCorp.subsidiary_list[i].department_list[j].vice_leader_list[0] = person;
                            else
                                initCorp.subsidiary_list[i].department_list[j].vice_leader_list.push_back(person);
                        else
                            initCorp.subsidiary_list[i].department_list[j].employee_list.push_back(person);
                        found = true;
                        flag = true;
                        break;
                    }
                }
            }
        }    
    initCorp.manage.push_back(person);
}
}

int menu_update(Employee *person, int& tmp, int order_sub, int order_dep, int order_subE, int order_depE, int order_corE, bool is_main_role, bool is_part_role, int slash)
{
    bool flag = true;
    string old_info;
    string new_info;
    flag = true;
    //In menu
    int choice;
    cout << endl << "Choose type of information you want to update: " << endl;
    cout << "1. ID \n" << "2. Family name \n" << "3. First name \n" << "4. Unit \n" << "5. Position \n"
         << "6. Date of birth \n" << "7. Hometown \n" << "8. Address \n" << "9. Email \n"
         << "10. Phone Number \n"
         << "11. Start Of Work \n" << "12. Delete Employee \n" << "13. Exit \n";
    cin >> choice;
    cin.ignore();
    switch (choice) {
        case 1: {
            string id;
            cout << "Enter new ID: ";
            old_info = person->id;
            getline(cin, id);
            initCorp.manage[tmp].id = id;
            person->id = id;
            new_info = person->id;
            break;;
        }
        case 2: {
            string name;
            cout << "Enter new family name: ";
            old_info = person->fam_name;
            getline(cin, name);
            initCorp.manage[tmp].fam_name = name;
            person->fam_name = name;
            new_info = person->fam_name;
            break;;
        }
        case 3: {
            string name;
            cout << "Enter new first name: ";
            old_info = person->first_name;
            getline(cin, name);
            initCorp.manage[tmp].first_name = name;
            person->first_name = name;
            new_info = person->first_name;
            break;;
        }
        case 4: {
            string unit,
                    position,
                    parent_unit;
            char chosen_unit;
            Employee new_employee = *person;
            // Xóa thông tin cũ
            delete_employee(tmp, order_sub, order_dep, order_subE, order_depE, order_corE, is_main_role,
                            is_part_role,
                            slash);
            do {
                cout << "Enter your choosen: \n" << "1. Change unit in the same level \n"
                     << "2. Change unit in different level \n";
                cin >> chosen_unit;
                cin.ignore();
                flag = true;
                switch (chosen_unit) {
                    case '1':
                        if (slash == 1) {
                            cout << "Subsidiary list: ";
                            for (Subsidiary sub: initCorp.subsidiary_list)
                                cout << sub.name << " ";
                        }
                        else {
                            for (Subsidiary sub: initCorp.subsidiary_list)
                                if (sub.name == new_employee.parent_unit) {
                                    cout << "Department list: ";
                                    for (Department dep: sub.department_list)
                                        cout << dep.name;
                                }
                        }
                        if (slash == 0)
                            return choice;
                        cout << endl << "Enter new unit: ";
                        getline(cin, unit);
                        new_employee.unit = unit;
                        break;
                    case '2':
                        cout << "Choose level: \n" << "1. Corporation level \n" << "2. Subsidiary level \n"
                             << "3. Department level \n";
                        char choice_level;
                        do {
                            cin >> choice_level;
                            cin.ignore();
                            switch (choice_level) {
                                case '1':
                                    unit = "BKcorporation";
                                    person->unit = unit;
                                    break;
                                case '2':
                                    cout << "Subsidiary list: \n";
                                    for (Subsidiary sub: initCorp.subsidiary_list) {
                                        cout << sub.name << " ";
                                    }
                                    cout << endl;
                                    cout << "Enter new unit: \n";
                                    getline(cin, unit);
                                    new_employee.unit = unit;
                                    new_employee.parent_unit = "BKcorporation";
                                    break;
                                case '3':
                                    cout << "Subsidiary list: ";
                                    for (Subsidiary sub: initCorp.subsidiary_list) {
                                        cout << sub.name << " ";
                                    }
                                    cout << endl;
                                    cout << "Enter new parent unit: ";
                                    getline(cin, parent_unit);
                                    new_employee.parent_unit = parent_unit;
                                    for (Subsidiary sub: initCorp.subsidiary_list) {
                                        if (sub.name == parent_unit) {
                                            cout << "Department list: ";
                                            for (Department dep: sub.department_list) {
                                                cout << dep.name << " ";
                                            }
                                            cout << endl;
                                            cout << "Enter new unit: \n";
                                            getline(cin, unit);
                                            new_employee.unit = unit;
                                        }
                                    }
                                    break;
                                default:
                                    flag = false;
                                    cout << "Please choose correct number \n";
                            }
                        } while (!flag);
                        if (unit != "BKcorporation") {
                            if (new_employee.parent_unit == "BKcorporation")
                                slash = 1;
                            else
                                slash = 2;
                        } else {
                            slash = 0;
                        }
                        if (slash == 0) {
                            flag = true;
                            char chosen_position;
                            do {
                                cout << "Choose new position: \n" << "1. Chu tich \n" << "2. Pho chu tich \n";
                                cin >> chosen_position;
                                switch (chosen_position) {
                                    case '1':
                                        position = "Chu tich";
                                        break;
                                    case '2':
                                        position = "Pho chu tich";
                                        break;
                                    default:
                                        flag = false;
                                        cout << "Please choose correct number /n";
                                }
                            } while (!flag);
                        } else if (slash == 1) {
                            flag = true;
                            char chosen_position;
                            do {
                                cout << "Choose new position: \n" << "1. Giam doc \n" << "2. Pho giam doc \n"
                                     << "3. Nhan vien \n";
                                cin >> chosen_position;
                                switch (chosen_position) {
                                    case '1':
                                        position = "Giam doc";
                                        break;
                                    case '2':
                                        position = "Pho giam doc";
                                        break;
                                    case '3':
                                        position = "Nhan vien";
                                        break;
                                    default:
                                        flag = false;
                                        cout << "Please choose correct number /n";
                                }
                            } while (!flag);
                        } else {
                            flag = true;
                            char chosen_position;
                            do {
                                cout << "Choose new position: \n" << "1. Truong phong \n" << "2. Pho phong \n"
                                     << "3. Nhan vien \n";
                                cin >> chosen_position;
                                switch (chosen_position) {
                                    case '1':
                                        position = "Truong phong";
                                        break;
                                    case '2':
                                        position = "Pho phong";
                                        break;
                                    case '3':
                                        position = "Nhan vien";
                                        break;
                                    default:
                                        flag = false;
                                        cout << "Please choose correct number \n";
                                }
                            } while (!flag);
                        }
                        new_employee.position = position;
                        break;
                    default:
                        flag = false;
                        cout << "Please enter correct number \n";
                }
            } while (!flag);
            add_employee_1(new_employee);
            tmp = initCorp.manage.size() - 1;
            break;;
        }
        case 5: {
            string position;
            char choice_position;
            if (slash == 0) {
                do {
                    cout << "Choose new position: \n";
                    cout << "1. Chu tich \n" << "2. Pho chu tich \n";
                    flag = true;
                    cin >> choice_position;
                    switch (choice_position) {
                        case '1':
                            position = "Chu tich";
                            break;
                        case '2':
                            position = "Pho chu tich";
                            break;
                        default:
                            flag = false;
                            cout << "Please enter correct number \n";
                    }
                } while (!flag);

            } else if (slash == 1) {
                do {
                    flag = true;
                    cout << "Choose new position: \n";
                    cout << "1. Giam doc \n" << "2. Pho giam doc \n" << "3. Nhan vien\n";
                    cin >> choice_position;
                    switch (choice_position) {
                        case '1':
                            position = "Giam doc";
                            break;
                        case '2':
                            position = "Pho giam doc";
                            break;
                        case '3':
                            position = "Nhan vien";
                            break;
                        default:
                            flag = false;
                            cout << "Please enter correct number \n";
                    }
                } while (!flag);
            } else {
                do {
                    flag = true;
                    cout << "Choose new position: \n";
                    cout << "1. Truong phong \n" << "2. Pho phong \n" << "3. Nhan vien\n";
                    cin >> choice_position;
                    switch (choice_position) {
                        case '1':
                            position = "Truong phong";
                            break;
                        case '2':
                            position = "Pho phong";
                            break;
                        case '3':
                            position = "Nhan vien";
                            break;
                        default:
                            flag = false;
                            cout << "Please enter correct number \n";
                    }
                } while (!flag);
            }

            Employee new_employee = *person;
            // Xóa nhân viên
            delete_employee(tmp, order_sub, order_dep, order_subE, order_depE, order_corE, is_main_role,
                            is_part_role,
                            slash);
            // Đổi vị trí
            new_employee.position = position;
            // Thêm nhân viên
            add_employee_1(new_employee);
            tmp = initCorp.manage.size() - 1;
            break;;
        }
        case 6: {
            string dob;
            cout << "Enter new date of birth: ";
            old_info = person->dob;
            getline(cin, dob);
            initCorp.manage[tmp].dob = dob;
            person->dob = dob;
            new_info = person->dob;
            break;;
        }
        case 7: {
            string hometown;
            cout << "Enter new hometown: ";
            old_info = person->hometown;
            getline(cin, hometown);
            initCorp.manage[tmp].hometown = hometown;
            person->hometown = hometown;
            new_info = person->hometown;
            break;;
        }
        case 8: {
            string address;
            cout << "Enter new adress: ";
            old_info = person->address;
            getline(cin, address);
            initCorp.manage[tmp].address = address;
            person->address = address;
            new_info = person->address;
            break;;
        }
        case 9: {
            string email;
            cout << "Enter new email: ";
            old_info = person->email;
            getline(cin, email);
            initCorp.manage[tmp].email = email;
            person->email = email;
            new_info = person->email;
            break;;
        }
        case 10: {
            string phone_number;
            cout << "Enter new phone number: ";
            old_info = person->phone_number;
            getline(cin, phone_number);
            initCorp.manage[tmp].phone_number = phone_number;
            person->phone_number = phone_number;
            new_info = person->phone_number;
            break;;
        }
        case 11: {
            string start_of_work;
            cout << "Enter new start_of_work: ";
            old_info = person->start_of_work;
            getline(cin, start_of_work);
            initCorp.manage[tmp].start_of_work = start_of_work;
            person->start_of_work = start_of_work;
            new_info = person->start_of_work;
            break;;
        }
        case 12: {
            delete_employee(tmp, order_sub, order_dep, order_subE, order_depE, order_corE, is_main_role,
                            is_part_role, slash);
            break;;
        }
        case 13:
            return choice;
        default:
            cout << "Please choose correct number";
    }
    ifstream input("data.txt");
    ofstream output("data_temp.txt");
    string line;
    string lineChanged;
    if (choice == 1)
        lineChanged = old_info;
    else
        lineChanged = person->id;
    bool found = false;
        while (getline(input, line)) {
            if (line == lineChanged)
                found = true;
            if(found) {
                if (line == old_info) {
                    output << new_info << endl; // Kết thúc sau khi tìm thấy dòng cần sửa
                    getline(input, line);
                }
            }
            output << line << endl;
        }
    input.close();
    output.close();
    remove("data.txt");
    rename("data_temp.txt", "data.txt");
    return choice;

}

void update_employee(string name) {
    int tmp;
    vector<int> order = find_employee(name);
    // Kiểm tra nhân viên có tồn tại hay không
    if (order.size() == 0) {
        cout << "Cannot find employee named " + name;
        return;
    }
    else
    {
        cout << "Choose employee you want to update \n";
        int choice_employee;
        cin >> choice_employee;
        tmp = order[--choice_employee];
    }

    // Tìm chỉ số nhân viên trong danh sách
    bool is_main_role = false;
    bool is_part_role = false;
    int order_sub,
            order_dep,
            order_subE,
            order_depE,
            order_corE;
    order_sub = order_dep = order_subE = order_depE = order_corE = 0;
    int slash = 0;
    int choice = -1;
    Employee *person = nullptr;
    do {
        if (choice == 4 || choice == 5 || choice == -1) {
            find_employee_order(initCorp.manage[tmp], is_main_role, is_part_role, order_sub, order_dep, order_subE,
                                order_depE,
                                slash,
                                order_corE);

            // Tham chiếu đến employee cần thay đổi
            if (slash == 0) {
                if (is_main_role)
                    person = &initCorp.chairman_list[order_corE];
                else
                    person = &initCorp.vice_chairman_list[order_corE];;
            } else if (slash == 1) {
                if (is_main_role)
                    person = &initCorp.subsidiary_list[order_sub].director_list[order_subE];
                else if (is_part_role)
                    person = &initCorp.subsidiary_list[order_sub].deputy_director_list[order_subE];
                else
                    person = &initCorp.subsidiary_list[order_sub].employee_list[order_subE];
            } else {
                if (is_main_role)
                    person = &initCorp.subsidiary_list[order_sub].department_list[order_dep].leader_list[order_depE];
                else if (is_part_role)
                    person = &initCorp.subsidiary_list[order_sub].department_list[order_dep].vice_leader_list[order_depE];
                else
                    person = &initCorp.subsidiary_list[order_sub].department_list[order_dep].employee_list[order_depE];
            }
        }
        //Menu
        choice = menu_update(person, tmp, order_sub, order_dep, order_subE, order_depE, order_corE, is_main_role, is_part_role,
                    slash);
    } while (choice != 13);


}


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
            cout << left << setw(50) << "| 3 - Display employee working status." << " |" << endl;
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
                cout << endl;
                for (const Employee & person: count)
                {
                    line(42);
                    readInfo(person);
                    line(42);
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

            else if(option == "4")
            {
                displaySub(initCorp.subsidiary_list);
            }

            else if(option == "5"){display_menu();}

            else if(option == "6")
            {
                string name;
                cout << "Iput employee name: ";
                cin.ignore();
                getline(cin, name);
                update_employee(name);
            }

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
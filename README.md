#BKcorporation
##A. PROBLEM
Programming an “Employee Management System” of a corporation – “BKCorporation”.
The organizational structure of the company is hierarchical as follows: 
- Corporation
- Subsidiaries
- Departments
- Employees
1. Build a program to read employee information from the saved text file.
2. The program should be able to perform the following functions:
- Option 1: Display basic information about the corporation.
- Option 2: Search Employee’s information.
- Option 3: Display working status.
- Option 4: Display information about any Subsidiaries.
- Option 5: Add a new employee.
- Option 6: Update Employee’s information.
- Option 7: Exit program.
##B. SOLUTION and WORK DIVISION
###I. Overview
_What knowledge will be applied in this project?_
1. Structure organization: struct
2. Data base: text file – read file
_Why text file? Not other database system?_
There are many types of databases can be used in this management project, such as MySQL, PostgreSQL, Oracle, … 
However, because of the limitation of this course and for simplifying the problem, we will only use “read file” method to address this problem.
3. Data structure: vector (stl container)
_Why “vector, not others?_
There are many types of data structures can be used like linked list, array, tree, vector, …
However, for simple we use vector which is a class template that contains the vector container, and its member functions allow us easily to manage the data.

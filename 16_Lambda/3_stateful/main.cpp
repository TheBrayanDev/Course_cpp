/*
#Stateful Lambda expressions

[captured_variables]() -> return_type specifiers { //code };

[captured_variables]() mutable -> return_type specifiers { //code };

! Default captures

[=] By value
[&] By reference
[this] this obj by &reference

*/

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <cctype>
#include <string>
#include <vector>
#include <cmath>
#include <memory>
#include <fstream>
#include <algorithm>
#include <numeric>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

int global_x{1000};

// # Capture by value
// ? The global variable global_x cannot be captured because it is not
// !  within the reaching scope of the lambda. It can however still be accessed
// from within the lambda using normal name lookup rules.
void test1()
{
    cout << "\n---Test1 --------------------------" << endl;

    int local_x{100};

    auto l = [local_x]() // # captures local_x by value
    {
        cout << local_x << endl;
        cout << global_x << endl;
    };
    l();
}

// # Capture by value - mutable
//? The captured variable x can only be modified within the lambda by
// using the mutable keyword.
//! Note that this modifiability only pertains to the captured variable within the lambda body and not the actual variable.
void test2()
{
    cout << "\n---Test2 --------------------------" << endl;

    int x{100};

    auto l = [x]() mutable
    {
        x += 100;
        cout << x << endl;
    };

    l();
    cout << x << endl;

    l(); //! the x of the obj is now 200
    cout << x << endl;
}

// # Capture by reference
// Any changes made to the captured reference variable within the lambda body
// will change the actual variable.
void test3()
{
    cout << "\n---Test3 --------------------------" << endl;

    int x{100};

    auto l = [&x]() mutable
    {
        x += 100;
        cout << x << endl;
    };

    l();
    cout << x << endl;
}

// # Default capture by value - mutable
//! Only variables used within the lambda body are captured by value.
//? The variable z is not captured by the lambda.
void test4()
{
    cout << "\n---Test4 --------------------------" << endl;

    int x{100};
    int y{200};
    int z{300}; //? Note that z is not captured in the lambda since it is not used.
                //? This should produce an 'unused variable' warning when compiling.

    auto l = [=]() mutable
    {
        x += 100;
        y += 100;
        cout << x << endl;
        cout << y << endl;
    };
    l();

    cout << "\n";
    cout << x << endl;
    cout << y << endl;
}

// # Default capture by reference
// ! This time because all three variables - x,y,z are used within
// the lambda body, all three of them will be captured by reference.
void test5()
{
    cout << "\n---Test5 --------------------------" << endl;

    int x{100};
    int y{200};
    int z{300};

    auto l = [&]()
    {
        x += 100;
        y += 100;
        z += 100; //! z is used and captured
        cout << x << endl;
        cout << y << endl;
        cout << z << endl;
    };
    l();

    cout << "\n";
    cout << x << endl;
    cout << y << endl;
    cout << z << endl;
}

// # Default capture by value, capture y by reference
void test6()
{
    cout << "\n---Test6 --------------------------" << endl;

    int x{100};
    int y{200};
    int z{300};

    auto l = [=, &y]() mutable
    {
        x += 100;
        y += 100; // changes by reference
        z += 100;
        cout << x << endl;
        cout << y << endl;
        cout << z << endl;
    };
    l();

    cout << "\n";
    cout << x << endl;
    cout << y << endl;
    cout << z << endl;
}

// # Default capture by reference, capture x and z by value
// Equivalent to test6
void test7()
{
    cout << "\n---Test7 --------------------------" << endl;

    int x{100};
    int y{200};
    int z{300};

    auto l = [&, x, z]() mutable
    {
        x += 100;
        y += 100; // # y was captured by reference
        z += 100;
        cout << x << endl;
        cout << y << endl;
        cout << z << endl;
    };
    l();

    cout << "\n";
    cout << x << endl;
    cout << y << endl;
    cout << z << endl;
}

// Used for test8
class Person
{
    friend std::ostream &operator<<(std::ostream &os, const Person &rhs);

private:
    string name;
    int age;

public:
    Person() = default;
    Person(string name, int age) : name{name}, age{age} {}
    Person(const Person &p) = default;
    ~Person() = default;
    string get_name() const { return name; }
    void set_name(string name) { this->name = name; };
    int get_age() const { return age; }
    void set_age(int age) { this->age = age; }

    auto change_person1() //! returns lambda
    {
        return [this](string new_name, int new_age)
        {name = new_name; age = new_age; };
    }
    auto change_person2() //! returns lambda
    {
        return [=](string new_name, int new_age)
        {name = new_name; age = new_age; };
    }
    auto change_person3()
    {
        return [&](string new_name, int new_age)
        {name = new_name; age = new_age; };
    }
};

std::ostream &operator<<(std::ostream &os, const Person &rhs)
{
    os << "[Person: " << rhs.name << " : " << rhs.age << "]";
    return os;
}

// # Default capture by reference of the current object using [this]
// ? [=], [&], and [this] are equivalent when capturing an object's member variables - all are captured by reference
// ! Note that capturing this with [=] has been deprecated in C++20
//@ Also, C++20 allows [*this], which captures by value!
void test8()
{
    cout << "\n---Test8 --------------------------" << endl;

    Person person("Larry", 18);
    cout << person << endl;

    // # Default capture [this]
    //! This is the preferred way
    auto change_person1 = person.change_person1();
    change_person1("Moe", 30);
    cout << person << endl;

    // Default capture [=]
    // This has been deprecated in C++20
    auto change_person2 = person.change_person2();
    change_person2("Curly", 25);
    cout << person << endl;

    // Default capture [&]
    auto change_person3 = person.change_person3();
    change_person3("Frank", 34);
    cout << person << endl;
}

// Used for test9
// ! This is the lambda class generated
class Lambda
{
private:
    int y;

public:
    Lambda(int y) : y{y} {};

    void operator()(int x) const
    {
        cout << x + y << endl;
    };
};

// Lambda class equivalence example
void test9()
{
    cout << "\n---Test9 --------------------------" << endl;

    int y{100};

    Lambda lambda1(y);
    auto lambda2 = [y](int x)
    { cout << x + y << endl; };

    lambda1(200);
    lambda2(200);
}

// Class used in the test10() example
class People
{
    vector<Person> people;
    int max_people;

public:
    People(int max = 10) : max_people(max) {}
    People(const People &p) = default;
    void add(string name, int age)
    {
        people.emplace_back(name, age);
    }
    void set_max_people(int max)
    {
        max_people = max;
    }
    int get_max_people() const
    {
        return max_people;
    }

    // This method returns a vector of Person objects in
    // people whose age > max_age AND it limits the number of
    // persons returned to be <= the member variable max_people.
    // Note that the lambda in this example captures this, by reference, and by value
    vector<Person> get_people(int max_age)
    {
        vector<Person> result;
        int count{0};
        std::copy_if(people.begin(),
                     people.end(),
                     std::back_inserter(result),
                     [this, &count, max_age](const Person &p) //! this access the obj and gets max_people
                     { return p.get_age() > max_age && ++count <= max_people; });
        return result;
    }
};

void test10()
{
    cout << "\n---Test10 --------------------------" << endl;

    People friends;
    friends.add("Larry", 18);
    friends.add("Curly", 25);
    friends.add("Moe", 35);
    friends.add("Frank", 28);
    friends.add("James", 65);

    auto result = friends.get_people(17); // all are +17

    cout << endl;
    for (const auto &p : result)
        cout << p << endl;

    friends.set_max_people(3);

    result = friends.get_people(17); //! max is 3 now

    cout << endl;
    for (const auto &p : result)
        cout << p << endl;

    result = friends.get_people(50); //! people over 50

    cout << endl;
    for (const auto &p : result)
        cout << p << endl;
}

int main()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    test10();

    cout << "\n";
    return 0;
}
/*
# VIRTUAL DESTRUCTORS

• Problems can happen when we destroy polymorphic objects
• If a derived class is destroyed by deleting its storage via the base class pointer
    and the class a non-virtual destructor. Then the behavior is undefined in the C++ standard.
! Derived objects must be destroyed in the correct order starting at the correctdestructor

# Solution/Rule:
    ? If a class has virtual functions
    ! ALWAYS provide a public virtual destructor
    ! If base class destructor is virtual then all derived class destructors are also virtual

class Checking : public Account
{
public:
    virtual void withdraw(double amount);
    virtual ~Checking();
    - - - - - - -
}

*/
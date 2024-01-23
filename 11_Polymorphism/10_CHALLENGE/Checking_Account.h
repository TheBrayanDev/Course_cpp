#ifndef CHECKING_ACCOUNT_H
#define CHECKING_ACCOUNT_H

#pragma once
#include "Account.h"
#include <string>
#include <iostream>

/* Account son with a static withdrawal fee of $1.50  */
class Checking_Account : public Account
{
private:
    static constexpr const char *def_name = "Unnamed Checking Account";
    static constexpr double def_balance = 0.0;
    static constexpr double withdrawal_fee = 1.50;

public:
    Checking_Account(std::string name = def_name, double balance = def_balance);
    virtual bool withdraw(double) override;
    virtual bool deposit(double) override;

    virtual void print(std::ostream &os) const override;

    virtual ~Checking_Account() = default;
};

#endif
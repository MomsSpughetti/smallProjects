
#include "client.h"

int client::getClient_id() const
{
    return this->client_id;
}

void client::setClient_id(int val)
{
    this->client_id = val;
}

bool client::getIsMember() const
{
    return this->isMember;
}

void client::setIsMember(bool val)
{
    this->isMember = val;
}

int client::getPhone() const
{
    return this->phone;
}

void client::setPhone(int val)
{
    this->phone = val;
}

int client::getExpenses() const
{
    return this->expenses;
}

void client::setExpenses(int val)
{
    this->expenses = val;
}

void client::reset()
{
    this->expenses = 0;
}

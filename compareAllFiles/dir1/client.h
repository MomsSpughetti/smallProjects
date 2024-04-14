
#ifndef _CLIENT
#define _CLIENT

#define INVALID -1
#define ZERO 0
class client
{
private:
    int client_id;
    int phone;
    bool isMember;
    int expenses;
public:
    client() = default;
    client(int client_id, int Phone, bool isMember = false, int exps = ZERO) : 
        client_id(client_id), 
        phone(Phone), 
        isMember(isMember), 
        expenses(exps){}
    ~client() = default;

    /*getter and setters*/
    int getClient_id() const;
    void setClient_id(int val);
    bool getIsMember() const;
    void setIsMember(bool val);
    int getPhone() const;
    void setPhone(int val);
    int getExpenses() const;
    void setExpenses(int val);

    /*special*/
    void reset();
};

#endif
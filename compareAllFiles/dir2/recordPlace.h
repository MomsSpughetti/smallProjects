#ifndef _RP_
#define _RP_
#include <ostream>
#define Zero 0
#define NOTEXIST -1
class recordPlace
{
private:
    int r_id;
    int helperToCalcHeightOfRecord;
    //next members are valid for the root only
    int columnHeight;
    int columnIndex;
public:
    recordPlace(int r_id = NOTEXIST, int colH = NOTEXIST, int colIn = NOTEXIST) : helperToCalcHeightOfRecord(Zero), columnHeight(colH), columnIndex(colIn){}
    ~recordPlace() = default;
    void extraCalc(recordPlace&, bool);
    void getStaticInfo(recordPlace& rp);
    recordPlace& operator+=(const recordPlace& obj);
    recordPlace& operator-=(const recordPlace& obj);
    recordPlace& operator=(const recordPlace& obj) = default;
    friend recordPlace operator+(const recordPlace& obj1, const recordPlace& obj2);
    friend recordPlace operator-(const recordPlace& obj1, const recordPlace& obj2);
    friend recordPlace operator%(const recordPlace& obj1, const recordPlace& obj2);
    friend std::ostream& operator<<(std::ostream&, const recordPlace&);
    friend class RecordsCompany;
};

#endif
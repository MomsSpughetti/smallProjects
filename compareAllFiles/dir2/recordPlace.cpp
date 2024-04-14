#include "recordPlace.h"

void recordPlace::extraCalc(recordPlace& NewRootRecord, bool is_B_BiggerThan_A)
{
    if (is_B_BiggerThan_A)
    {
        this->helperToCalcHeightOfRecord += (NewRootRecord.columnHeight - NewRootRecord.helperToCalcHeightOfRecord);
        NewRootRecord.columnHeight += this->columnHeight;
    } else {
        NewRootRecord.helperToCalcHeightOfRecord += this->columnHeight;
        this->helperToCalcHeightOfRecord -= NewRootRecord.helperToCalcHeightOfRecord;
        NewRootRecord.columnHeight += this->columnHeight;
        NewRootRecord.columnIndex = this->columnIndex;
    }
    
}

recordPlace& recordPlace::operator-=(const recordPlace& obj)
{
    this->helperToCalcHeightOfRecord -= obj.helperToCalcHeightOfRecord;
    return *this;
}

recordPlace& recordPlace::operator+=(const recordPlace& obj)
{
    this->helperToCalcHeightOfRecord += obj.helperToCalcHeightOfRecord;
    return *this;
}

recordPlace operator+(const recordPlace& obj1, const recordPlace& obj2)
{
    recordPlace temp;
    temp = obj1;
    temp += obj2;
    return temp;
}

recordPlace operator-(const recordPlace& obj1, const recordPlace& obj2)
{
    recordPlace temp;
    temp = obj1;
    temp -= obj2;
    return temp;
}
recordPlace operator%(const recordPlace& obj1, const recordPlace& obj2)
{
    recordPlace temp;
    temp = obj1;
    return temp;
}

std::ostream& operator<<(std::ostream& os, const recordPlace& obj){
    os << obj.helperToCalcHeightOfRecord;
    return os;
}

void recordPlace::getStaticInfo(recordPlace& rp){
    this->columnHeight = rp.columnHeight;
    this->columnIndex = rp.columnIndex;
}
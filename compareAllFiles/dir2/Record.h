#ifndef _REC_
#define _REC_



class Record
{
private:
    int id;
    int copies;
    int purchases;
public:
    ~Record() = default;
    Record(int id = -1, int copies = 0, int purs = 0): id(id),copies(copies), purchases(purs){}
    friend class RecordsCompany;
};

#endif

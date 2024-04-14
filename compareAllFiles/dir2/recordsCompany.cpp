
#include "recordsCompany.h"

RecordsCompany::RecordsCompany() : 
    recordsInfo(nullptr), 
    recordsNum(ZERO), 
    clients(), members(), 
    stock(new UnionFindExtra<int, int, recordPlace>()){}

RecordsCompany::~RecordsCompany()
{
    client ** clients_ = clients.get_data();
    for (int i = 0; i < clients.size(); i++)
    {
        delete clients_[i];
    }
    delete [] clients_;
    delete [] recordsInfo;
    delete stock;
}

void RecordsCompany::resetStock(){
    delete stock;
    stock = new UnionFindExtra<int, int, recordPlace>();
}

void RecordsCompany::resetExpenses()
{
    int N = 0;
    client ** clients_ = members.TreeNodesToDataArray(N);
    for (int i = 0; i < N; i++)
    {
        clients_[i]->reset();
    }
    delete [] clients_;
}

void RecordsCompany::resetPrizes()
{
    this->members.resetExtras();
}

StatusType RecordsCompany:: newMonth(int *records_stocks, int number_of_records)
{
    if(number_of_records<0)
    {
        return StatusType::INVALID_INPUT;
    }
     try
     {
        resetStock();
        delete [] this->recordsInfo;
        recordsInfo = new Record[number_of_records];
        for (int i = 0; i < number_of_records; i++)
        {
            recordsInfo[i].copies = records_stocks[i];
            this->stock->Insert(i, recordsInfo[i].copies, recordPlace(i, records_stocks[i], i));
        }
        resetPrizes();
        resetExpenses();
        this->recordsNum = number_of_records;
     }
     catch(const std::bad_alloc& w)
     {
        return StatusType::ALLOCATION_ERROR;
     }
     catch(const std::exception& e)
     {
        std::cerr << e.what() << '\n';
     }
    return StatusType::SUCCESS;
}

StatusType RecordsCompany::addCostumer(int c_id, int phone)
{
    
    if(c_id<0||phone<0)
    {
        return StatusType::INVALID_INPUT;
    }
    
       try
       {
              if(clients.contains(c_id))
                {
                   return StatusType::ALREADY_EXISTS;
                }

            client* c1 = new client(c_id, phone);
            clients.put(c_id,c1);
    }
        catch(const std::bad_alloc& e)
        {
           return StatusType::ALLOCATION_ERROR;
        }
        catch(const std::exception& e)
        {
          std::cerr << e.what() << '\n';
        }

    return StatusType:: SUCCESS;
    
}

Output_t<int> RecordsCompany:: getPhone(int c_id)
{
    if(c_id<0)
    {
	   return StatusType::INVALID_INPUT;
    }

    client** c1=clients.get(c_id);
    return (c1) ? (*c1)->getPhone() : Output_t<int>(StatusType::DOESNT_EXISTS);

}

StatusType RecordsCompany::makeMember(int c_id)
{
    if(c_id<0)
    {
          return StatusType::INVALID_INPUT;
    }

    try
    {
       if(!clients.contains(c_id))
       {
        return StatusType::DOESNT_EXISTS;
       }

       if(members.Find(c_id))
       {
        return StatusType::ALREADY_EXISTS;
       }

       client** c1 = clients.get(c_id);
       (*c1)->setIsMember(true);
       members.Insert(c_id,(*c1));

    }
    catch(const std::bad_alloc& e)
        {
           return StatusType::ALLOCATION_ERROR;
        }
    catch(const std::exception& e)
        {
          std::cerr << e.what() << '\n';
        }
    return StatusType::SUCCESS;
}

Output_t<bool> RecordsCompany:: isMember(int c_id)
{

    if(c_id<0)
    {
        return Output_t<bool>(StatusType::INVALID_INPUT);
    }
    try
    {
        client** c1 = clients.get(c_id);
        return (c1) ? (*c1)->getIsMember() : Output_t<bool>(StatusType::DOESNT_EXISTS);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return StatusType::FAILURE;
}

StatusType RecordsCompany::buyRecord(int c_id, int r_id)
{
    if(c_id<0||r_id<0){
        return StatusType::INVALID_INPUT;
    }

    try
    {

    if(recordsNum<=r_id || !(clients.contains(c_id)))
    {
        return StatusType::DOESNT_EXISTS;
    }

    //commit purchase
    int numOfPurchases = recordsInfo[r_id].purchases;
    recordsInfo[r_id].purchases++;
    client** c1 = clients.get(c_id);
    if((*c1)->getIsMember()){
        (*c1)->setExpenses(numOfPurchases + 100 + (*c1)->getExpenses());
    }

    }
    catch(const std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
  return StatusType::SUCCESS;  

}

StatusType RecordsCompany:: addPrize(int c_id1, int c_id2, double  amount)
{
    if(c_id1<0||c_id2<c_id1||amount<=0)
    {
        return StatusType::INVALID_INPUT;
    }
    
    try
    {
    members.updateRange(c_id2-1,amount);
    members.updateRange(c_id1-1,(-amount));
     return  StatusType::SUCCESS;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return  StatusType::SUCCESS;

}

 Output_t<double> RecordsCompany::getExpenses(int c_id)
 {
    if(c_id == 206){
        std::cout << "";
    }
    if(c_id<0)
    {
	  return StatusType::INVALID_INPUT;
    }
    try
    {
        client ** _client = members.Find(c_id);
        if(!_client)
        {
            return StatusType::DOESNT_EXISTS;
        }
    
        int memberExpenses = (*_client)->getExpenses() - members.getValidExtra(c_id);
        return memberExpenses;  
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return StatusType::SUCCESS;
 }

StatusType RecordsCompany::putOnTop(int r_id1, int r_id2){
    if(r_id1 < 0 
    || r_id2 < 0){
        return StatusType::INVALID_INPUT;
    }

    if(r_id1 >= this->recordsNum
    || r_id2 >= this->recordsNum){
        return StatusType::DOESNT_EXISTS;
    }

    try
    {
        if(stock->areInSameGroup(r_id1, r_id2)){
            return StatusType::FAILURE;
        }
        stock->Union(r_id1, r_id2);
    }
    catch(const std::bad_alloc&){
        return StatusType::ALLOCATION_ERROR;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return StatusType::SUCCESS;
}
StatusType RecordsCompany::getPlace(int r_id, int *column, int *height){
    //std::cout << std::endl;
    //std::cout << *(this->stock) << std::endl;
    if(r_id < 0
    || !column
    || !height){
        return StatusType::INVALID_INPUT;
    }
    if(r_id >= this->recordsNum){
        return StatusType::DOESNT_EXISTS;
    }

    try
    {
        recordPlace placeInfo = this->stock->get_validExtra(r_id);
        *column = placeInfo.columnIndex;
        *height = placeInfo.helperToCalcHeightOfRecord;
    }
    catch(const std::bad_alloc&){
        return StatusType::ALLOCATION_ERROR;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return StatusType::SUCCESS;
}
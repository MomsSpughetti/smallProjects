#ifndef _HASH_TABLE_
#define _HASH_TABLE_

#include "RankedAvlTreeExtra.h"

/*
Hash table

supports size changing!

with AVL trees
*/
template <typename Key, typename Data>
class HashTable {
 public:
  HashTable(int size = 16) : size_(size), table_(new RankedAVLTreeExtra<Key,Data,int>[size]()){}

  ~HashTable() {
    delete[] table_;
  }

  void put(Key key, Data data) 
  {
    unsigned int index = hash(key, size_);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
    RankedAVLTreeExtra<Key,Data,int>& targetTree = table_[index];
    targetTree.Insert(key, data);
    if (++count_ > size_) 
    {
      //int new_size = closer_prime(size_ * 2);
      int new_size = size_ * 2;
      resize(new_size);
    }
  }

  Data* get(Key key) 
  {
    unsigned int index = hash(key, size_);
    RankedAVLTreeExtra<Key,Data,int>& targetTree = table_[index];
    Data* dataToReturn = targetTree.Find(key);
    return dataToReturn;
  }

  void remove(Key key) 
  {
    unsigned int index = hash(key, size_);
    RankedAVLTreeExtra<Key,Data,int>& targetTree = table_[index];
    int treeSizeBeforeDeletion = targetTree.get_size();
    targetTree.Remove(key);
    if (treeSizeBeforeDeletion > targetTree.get_size() && --count_ < size_ / 4) 
    {
      int new_size = size_ / 2;
      //if(!is_prime(new_size))
        //new_size = closer_prime(new_size);
      resize(new_size);
    }
  }

  bool contains(Key key) 
  {
    unsigned int index = hash(key, size_);
    RankedAVLTreeExtra<Key,Data,int>& targetTree = table_[index];
    return targetTree.FindNode(key) ? true : false;
  }

  int size() 
  {
    return count_;
  }

  int Table_size()
  {
    return size_;
  }


  void Table_Diagram() const
  {
    for (int i = 0; i < size_; i++)
    {
      std::cout << std::endl << i << " <+> ";
      RankedAVLTreeExtra<Key,Data,int>& targetTree = table_[i];
      std::cout << targetTree.get_size() << std::endl;

    }
    
  }

  //do not forget to free the returned array
  Data* get_data()
  {
    Data* valid = new Data[this->count_];
    int validArrayIndex = 0;
    for (int i = 0; i < this->size_; i++)
    {
      //store the data that table_[i] contains, in the array 'valid'.
    RankedAVLTreeExtra<Key,Data,int>& targetTree = table_[i];
    int DataAmount = 0;
    Data *dataInTree = targetTree.TreeNodesToDataArray(DataAmount);
    for (int j = 0; j < DataAmount; j++)
    {
      valid[validArrayIndex++] = dataInTree[j];
    }
    delete[] dataInTree;
    }
    return valid;
  }

 private:

  int size_;
  RankedAVLTreeExtra<Key,Data,int> *table_;
  int count_ = 0;


  void resize(int new_size) 
  {
    RankedAVLTreeExtra<Key,Data,int> *new_table = new RankedAVLTreeExtra<Key,Data,int>[new_size]();

    for (int i = 0; i < size_; i++) {
      //get tree in place i
      RankedAVLTreeExtra<Key,Data,int>& currTree = table_[i];
      //get all nodes in tree
      Node<Key,Data,int> **nodesInTree = currTree.TreeNodesToArray();
      //move to nodes to the new table
      for (int i = 0; i < currTree.get_size(); i++)
      {
        unsigned int index = hash(nodesInTree[i]->key, new_size) % new_size;
        RankedAVLTreeExtra<Key,Data,int>& newTree = new_table[index];
        Node<Key,Data,int> *node = nodesInTree[i];
        newTree.Insert(node->key, node->data);
      }
      delete[] nodesInTree;
    }

    delete[] table_;
    table_ = new_table;
    size_ = new_size;
  }

  std::size_t Nhash(const Key& key, std::size_t table_size) {
    std::size_t multiplier = 2654435761;
    std::size_t shift = 32;
    std::size_t index = static_cast<std::size_t>(static_cast<std::size_t>(multiplier * key) >> shift) % table_size;;
    //return static_cast<std::size_t>(static_cast<std::size_t>(multiplier * key)) % table_size;
    return index;
  }

//FNV hash (better than the previous one)
unsigned int hash(const Key& key, std::size_t table_size) {
  unsigned int hash = 2166136261u; // Initialize the hash to a prime value
  const unsigned char *p = reinterpret_cast<const unsigned char*>(&key);
  size_t size = sizeof(Key);
  for (size_t i = 0; i < size; ++i) {
    hash = (hash ^ p[i]) * 16777619u; // Multiply by a prime value
  }
  //return hash % table_size; // Return the index in the range [0, table_size-1]
  return hash & (table_size - 1);
}

};

#endif

#ifndef _OUR_TREE
#define _OUR_TREE


/*
first version of an AVL tree
supports basic functions
key should have:
  operators:
    - <
    - >
    - ==
    - <<

Data should have:
  operators:
    - <<

operators << are used in function printInOrder()
    
*/

#include <iostream>
#include <stdlib.h>
#include <math.h>

template <typename Key, typename Data, typename Extra>
class Node {
 public:
  Key key;
  Data data;
  Extra extra;
  int height;
  Node<Key, Data, Extra>  *left;
  Node<Key, Data, Extra>  *right;
  Node<Key, Data, Extra>  *parent;
  int size;

  explicit Node(const Key &key, const Data &data)
      : key(key), data(data), extra(),height(1), left(nullptr), right(nullptr), parent(nullptr), size(1) {}

  ~Node() = default;

};

template <typename Key, typename Data, typename Extra>
class RankedAVLTreeExtra {
 public:
 RankedAVLTreeExtra() : root(nullptr) {}

  ~RankedAVLTreeExtra() {
    DeleteTree(root);
  }

  bool Insert(const Key &key, const Data &data) {
    if(!FindNode(root, key)){
      root = InsertNode(root, key, data);
      return true;
    }
    return false;
  }

  bool Remove(const Key &key) {
    if(FindNode(root, key)){
      root = Remove(root, key);
      return true;
    }
    return false;
  }


  void InsertNode(Node<Key, Data, Extra> * node){
    if(!FindNode(root, node->key))
      root = InsertNode(root, node->key, node->data, node);
  }

//update has bugs
  void update(Key& prevKey, Key& newkey, Data& newData)
  {
    Node<Key, Data, Extra> * temp = FindNode(root, prevKey);

    if(!temp) {return;} 

    if(!(temp->key == prevKey)) {return;}
    
    Node<Key, Data, Extra> * removed;
    root = Remove(root, prevKey, 0, &removed);
    if(removed){
    (removed)->key = newkey;
    (removed)->data = newData;
    }

    this->InsertNode(removed);
  }
  
  Data* Find(const Key &key) {
    return Find(root, key);
  }

int GetRank(const Key &key) {
  return GetRank(root, key);
}

Data* Max() const{
  Node<Key, Data, Extra> * maxNode = getMaxNode();
  if(!maxNode){
    return nullptr;
  }

  return &(maxNode->data);
}
Node<Key, Data, Extra> * FindByRank(int k) {
  return FindByRank(root, k);
}

  void printInOrder()
  {
    std::cout << std::endl;
    printInOrder_aux(this->root);
    std::cout << std::endl;
  }

  Node<Key, Data, Extra> ** TreeNodesToArray()
  {
    if(!this->root)
      return nullptr;
    Node<Key, Data, Extra> ** array = new Node<Key, Data, Extra> *[this->root->size];
    int place = 0;
    TreeNodesToNodesArray(this->root, array, place);
    return array;
  }

  Data* TreeNodesToDataArray(int &N)
  {
    if(!this->root)
      return nullptr;
    Data* array = new Data[this->root->size];
    int place = 0;
    N = this->root->size;
    TreeNodesToDataArray(this->root, array, place);
    return array;
  }

  int get_size() const{
    return (!root) ? 0 : root->size;
  }

  Key& getIdByReference(const Key& key)
  {
    return FindNode(root, key)->key;
  }

  Node<Key, Data, Extra> * FindNode(const Key &key){
    return FindNode(root, key);
  }

  /*
  *returns the key of the given node's predecessor

  *Key should have operator ==

  *There should be no node having a key equal to the 
  default value of Key objects (default constructor values).

  *If there is no predecessor, the function returns the default value of a Key object.
  */
  Key getPredecessor(Key key, bool& changed){
    Key predecessorKey;
    changed = false;
    getPredecessor_aux(key, root, &predecessorKey, changed);
    return predecessorKey;
  }

  /*
  Extra += extra for each node that maintains node.key <= key
  O(log(n)) complexity
  */
  void updateRange(Key key, Extra extra){
    if(!FindNode(key)){
      bool changed;
      key = getPredecessor(key, changed);
      if(!changed){return;}
    }
    //if Predecessor is not in the tree (in case 
    //the target key not found and is the smallest in the tree)
    //updateRange_aux will do nothing (why?)
    updateRange_aux(root, key, extra, false);
  }
  
  /*get the correct value that the node(key) should has after updateRange was called*/
  Extra getValidExtra(Key key){
    return getSumOfExtras(root, key);
  }

  void resetExtras(){
    resetExtras_aux(this->root);
  }

 private:
  Node<Key, Data, Extra>  *root;

    Node<Key, Data, Extra> * InsertNode(Node<Key, Data, Extra>  *no, const Key &key, const Data &data, Node<Key, Data, Extra>  *nodeToInsert = nullptr) {
        if (!no) {
            if(!nodeToInsert)
              return new Node<Key, Data, Extra> (key, data);
            else {/*
              nodeToInsert->parent = nullptr;
              nodeToInsert->left = nullptr;
              nodeToInsert->right = nullptr;*/
              return nodeToInsert;
            }
        }
        if (key < no->key) 
        {
          Node<Key, Data, Extra> *noLeftOld = no->left;
          no->left = InsertNode(no->left, key, data, nodeToInsert);
          (no->left)->parent = no;
          //make validExtra = default value, for the new node
          if(!noLeftOld){no->left->extra = no->left->extra - getSumOfExtras(root, no->key);}
        } else if (key > no->key) 
        {
          Node<Key, Data, Extra> *noRightOld = no->right;
          no->right = InsertNode(no->right, key, data, nodeToInsert);
          (no->right)->parent = no;
          //make validExtra = default value, for the new node
          if(!noRightOld){no->right->extra = no->right->extra - getSumOfExtras(root, no->key);}
        } else 
        {
            return nullptr;
        }
        no->size = 1 + GetSize(no->left) + GetSize(no->right);
        no->height = 1 + std::max(GetHeight(no->left), GetHeight(no->right));
        return Balance(no);
        }

void swapSonParent(Node<Key, Data, Extra>  *son, Node<Key, Data, Extra>  *father, Node<Key, Data, Extra>  sonMirror, Node<Key, Data, Extra>  fatherMirror){

  int sonIsLeft = 0, sonIsRight = 0;
  if((son->parent && son->parent->key == father->key)){
    if(father->left && father->left->key == son->key){
      sonIsLeft = 1;
    } else {
      sonIsRight = 1;
    }
  }

  if(father->parent){
    if(father->parent->left && father->parent->left->key == father->key)
      father->parent->left = son;
    else
      father->parent->right = son;
  }

    if(sonIsLeft){
      son->right = father->right;
      son->left = father;
      father->right = sonMirror.right;
      father->left = sonMirror.left;
    }
    if(sonIsRight){
      son->left = father->left;
      son->right = father;
      father->left = sonMirror.left;
      father->right = sonMirror.right;
    }
      son->parent = father->parent;
      father->parent = son;

      if(root->key == father->key)
        root = son;
}

  void copyNodePointers(Node<Key, Data, Extra> * source, Node<Key, Data, Extra> * dst){
    if(!dst || !source){return;}
    dst->left = source->left;
    dst->right = source->right;
    if(dst->right)
      dst->right->parent = dst;
    if(dst->left)
      dst->left->parent = dst;
    dst->parent = source->parent;
    source->parent = dst->parent;
  }

  void replaceParents(Node<Key, Data, Extra> * Water, Node<Key, Data, Extra> * Fire){
    if(!Fire || !Water){return;}

    //water parent to be Fire parent
    if(Water->parent){
      if(Water->parent->left && Water->parent->left->key == Water->key){
      //water is left son
        Water->parent->left = Fire;
      } else {
        //water is right son
        Water->parent->right = Fire; 
      }
    }

    //Fire parent to be Water parent
    if(Fire->parent){
      if(Fire->parent->left && Fire->parent->left->key == Fire->key){
      //Fire is left son
        Fire->parent->left = Water;
      } else {
        //Fire is right son
        Fire->parent->right = Water; 
      }
    }
  }

//swaps the nodes themselves
void swapNodes(Node<Key, Data, Extra>  *Fire, Node<Key, Data, Extra>  *Water){

  Node<Key, Data, Extra>  WaterMirror(Water->key, Water->data);
  WaterMirror.left = Water->left;
  WaterMirror.right = Water->right;
  WaterMirror.parent = Water->parent;

  Node<Key, Data, Extra>  FireMirror(Water->key, Water->data);
  FireMirror.left = Fire->left;
  FireMirror.right = Fire->right;
  FireMirror.parent = Fire->parent;

  //if the are related parent - son
  if((Water->parent && Water->parent->key == Fire->key)){
    swapSonParent(Water, Fire, WaterMirror, FireMirror);
    return;
    }
  if((Fire->parent && Fire->parent->key == Water->key)){
    swapSonParent(Fire, Water, FireMirror, WaterMirror);
    return;
    }

  //their parents
  replaceParents(Water, Fire);

  //Water to be Fire
  copyNodePointers(Fire, Water);

  //Fire to be Water
  copyNodePointers(&WaterMirror, Fire);

  if(root->key == Fire->key)
    root = Water;
  if(root->key == Water->key)
    root = Fire;
}

void ModifyParentOfDeleted(Node<Key, Data, Extra>  *no){
  if(no && no->parent){
    if(no->parent->left && no->parent->left->key == no->key){
      no->parent->left = nullptr;
    } else {
      no->parent->right = nullptr;
    }
  }
}
//has bugs
Node<Key, Data, Extra> * RemoveKeepingPointers(Node<Key, Data, Extra>  *no, const Key &key, bool IsToDelete = 1, Node<Key, Data, Extra> ** removed = nullptr) {
  if (!no) {
    return nullptr;
  }
  if (key > no->key) 
  {
    no->right = RemoveKeepingPointers(no->right, key, IsToDelete, removed);
  } else if (key < no->key) 
  {
    no->left = RemoveKeepingPointers(no->left, key, IsToDelete, removed);
  } else 
  {
    if (!no->left || !no->right) 
    {
      Node<Key, Data, Extra>  *temp = no->left ? no->left : no->right;
      if (!temp) 
      {
        //is there a need that parent points now to null in the deleted child ptr?
        if(IsToDelete){
          //ModifyParentOfDeleted(no);
        if(no->parent){
          if(no->parent->left && no->parent->left->key == no->key){
            no->parent->left = nullptr;
          } else {
            no->parent->right = nullptr;
          }
        }
          delete no;
        }
        else
          (*removed) = no;
        return nullptr;
      }
      swapNodes(temp, no);
      Node<Key, Data, Extra>  *toDelete = no;
      no = temp;
      temp->left = nullptr;
      temp->right = nullptr;

      if(IsToDelete){
          //ModifyParentOfDeleted(toDelete);
          delete toDelete;
      }
      else
        (*removed) = toDelete;

    } else 
    {
      Node<Key, Data, Extra>  *temp = FindMin(no->right);
      swapNodes(temp, no);
      temp->right = Remove(temp->right, no->key, IsToDelete, removed);
      no = temp;
    }
  }
  no->size = 1 + GetSize(no->left) + GetSize(no->right);
  no->height = 1 + std::max(GetHeight(no->left), GetHeight(no->right));
  return Balance(no);
}

/*
  set the pointer - of the parent of the given node - that points to node to nullptr
*/
void makeParentChildNull(Node<Key, Data, Extra>  *no){
  if(no
  && no->parent 
  && no->parent->right
  && no->parent->right->key == no->key){
    no->parent->right = nullptr;
  }

  if(no
  && no->parent 
  && no->parent->left
  && no->parent->left->key == no->key){
    no->parent->left = nullptr;
  }

}

Node<Key, Data, Extra> * Remove(Node<Key, Data, Extra>  *no, const Key &key, bool IsToDelete = 1, Node<Key, Data, Extra> ** removed = nullptr) {
  if (!no) {
    return nullptr;
  }
  if (key > no->key) 
  {
    no->right = Remove(no->right, key);
  } else if (key < no->key) 
  {
    no->left = Remove(no->left, key);
  } else 
  {
    if (!no->left || !no->right) 
    {
      Node<Key, Data, Extra>  *temp = no->left ? no->left : no->right;
      if (!temp) 
      {
        makeParentChildNull(no);
        delete no;
        return nullptr;
      }
      no->key = temp->key;
      no->data = temp->data; //swap between only child and father
      Key parentKey = no->parent ? no->parent->key : Key();
      no->extra = no->extra + temp->extra - getValidExtra(parentKey);//helper value + child extra = child valid extra

      makeParentChildNull(temp);
      delete temp;
    } else 
    {
      Node<Key, Data, Extra>  *temp = FindMin(no->right);
      Key nodeToDeleteParentKey;
      //get valid extras
      Extra tempValidExtra = getValidExtra(temp->key);
      if(no->parent){nodeToDeleteParentKey = no->parent->key;}
      Extra nodeToDeleteParentValidExtra = getValidExtra(nodeToDeleteParentKey);
      Extra rightSonValidExtra = getValidExtra(no->right->key);
      Extra leftSonValidExtra = getValidExtra(no->left->key);
      //swap
      no->key = temp->key;
      no->data = temp->data;
      //correct extras accordingly
      no->extra = tempValidExtra - nodeToDeleteParentValidExtra;
      no->right->extra = rightSonValidExtra - tempValidExtra;
      no->left->extra = leftSonValidExtra - tempValidExtra;
      //remove the swapped father
      temp->extra = tempValidExtra; //save a helper info in the node to be deleted, 
      //                                      so that its children can use it later
      no->right = Remove(no->right, temp->key);
    }
  }
  no->size = 1 + GetSize(no->left) + GetSize(no->right);
  no->height = 1 + std::max(GetHeight(no->left), GetHeight(no->right));
  return Balance(no);
}


Data* Find(Node<Key, Data, Extra>  *node, const Key &key) {
  if (!node) {
    return nullptr;
  }
  if (key == node->key) 
  {
    return &node->data;
  } else if (key < node->key) 
  {
    return Find(node->left, key);

  } else 
  {

    return Find(node->right, key);
  }
}

Node<Key, Data, Extra> * FindNode(Node<Key, Data, Extra>  *no, const Key &key) {
  if (!no) {
    return nullptr;
  }
  if (key == no->key){
    return no;
  } else if (key < no->key){
    return FindNode(no->left, key);
  } else {
    return FindNode(no->right, key);
  }
  return nullptr;
}
Node<Key, Data, Extra> * FindByRank(Node<Key, Data, Extra>  *no, int k) {
  if (!no) {
    return nullptr;
  }
  int leftSize = GetSize(no->left);
  if (k < leftSize) {
    return FindByRank(no->left, k);
  } else if (k > leftSize) {
    return FindByRank(no->right, k - leftSize - 1);
  } else {
    return no;
  }
}

int GetSize(Node<Key, Data, Extra>  *node) {
  return node ? node->size : 0;
}

int GetRank(Node<Key, Data, Extra>  *no, const Key &key) {
  if (!no) {
    return 0;
  }
  if (key < no->key) {
    return GetRank(no->left, key);
  } else if (key > no->key) {
    return 1 + GetSize(no->left) + GetRank(no->right, key);
  } else {
    return GetSize(no->left);
  }
}

Node<Key, Data, Extra> * FindMin(Node<Key, Data, Extra>  *node) {
  if(!node){
    return nullptr;
  }
  if (!node->left) {
    return node;
  }
  return FindMin(node->left);
}

int GetHeight(Node<Key, Data, Extra>  *node) {
  return node ? node->height : 0;
}

int GetBalanceFactor(Node<Key, Data, Extra>  *node) {
  return GetHeight(node->right) - GetHeight(node->left);
}

Node<Key, Data, Extra> * Balance(Node<Key, Data, Extra>  *node) {
  node->height = 1 + std::max(GetHeight(node->left), GetHeight(node->right));
  node->size = 1 + GetSize(node->left) + GetSize(node->right);

  int balance_factor = GetBalanceFactor(node);
  if (balance_factor < -1 && GetBalanceFactor(node->left) <= 0) {
    return RotateRight(node);
  }
  if (balance_factor > 1 && GetBalanceFactor(node->right) >= 0) {
    return RotateLeft(node);
  }
  if (balance_factor < -1 && GetBalanceFactor(node->left) > 0) {
    node->left = RotateLeft(node->left);
    return RotateRight(node);
  }
  if (balance_factor > 1 && GetBalanceFactor(node->right) < 0) {
    node->right = RotateRight(node->right);
    return RotateLeft(node);
  }
  return node;
}

Node<Key, Data, Extra>  *RotateLeft(Node<Key, Data, Extra>  *node) {
  Node<Key, Data, Extra>  *temp = node->right;
  node->right = temp->left;
  if(temp->left)
    temp->left->parent = node;
  temp->left = node;
  Node<Key, Data, Extra>  *nodeParent = node->parent;
  node->parent = temp;
  temp->parent = nodeParent;
  node->height = 1 + std::max(GetHeight(node->left), GetHeight(node->right));
  temp->height = 1 + std::max(GetHeight(temp->left), GetHeight(temp->right));
  node->size = 1 + GetSize(node->left) + GetSize(node->right);
  temp->size = 1 + GetSize(temp->left) + GetSize(temp->right);

  /*NEW*/
  Extra prevTempExtra = temp->extra;
  temp->extra = temp->extra + node->extra;
  node->extra = node->extra - temp->extra;
  if(node->right){node->right->extra = node->right->extra + prevTempExtra;}

  return temp;
}

Node<Key, Data, Extra> * RotateRight(Node<Key, Data, Extra>  *node) {
  Node<Key, Data, Extra>  *temp = node->left;
  node->left = temp->right;
  if(temp->right)
    temp->right->parent = node;
  temp->right = node;
  Node<Key, Data, Extra>  *nodeParent = node->parent;
  node->parent = temp;
  temp->parent = nodeParent;
  node->height = 1 + std::max(GetHeight(node->left), GetHeight(node->right));
  temp->height = 1 + std::max(GetHeight(temp->left), GetHeight(temp->right));
  node->size = 1 + GetSize(node->left) + GetSize(node->right);
  temp->size = 1 + GetSize(temp->left) + GetSize(temp->right);

  /*NEW*/
  Extra prevTempExtra = temp->extra;
  temp->extra = temp->extra + node->extra;
  node->extra = node->extra - temp->extra;
  if(node->left){node->left->extra = node->left->extra + prevTempExtra;}

  return temp;
}

void DeleteTree(Node<Key, Data, Extra>  *node) {
  if (node == nullptr) {
    return;
  }
  DeleteTree(node->left);
  DeleteTree(node->right);
  delete node;
}


void printInOrder_aux(Node<Key, Data, Extra> * router)
{
  if(router == nullptr)
  {
    return;
  }
  
  printInOrder_aux(router->left);
  std::cout << std::endl;
  std::cout << "[ " << router->key << ", " << router->data << ", " << getValidExtra(router->key) <<"] ";
  printInOrder_aux(router->right);

}

  void TreeNodesToNodesArray(Node<Key, Data, Extra> * node, Node<Key, Data, Extra> ** arr, int &i)
  {
    if(!node)
    {
      return;
    }

    TreeNodesToNodesArray(node->left, arr, i);
    arr[i] = node;
    i++;
    TreeNodesToNodesArray(node->right, arr, i);
  }

  void TreeNodesToDataArray(Node<Key, Data, Extra> * node, Data* arr, int &i)
  {
    if(!node)
    {
      return;
    }

    TreeNodesToDataArray(node->left, arr, i);
    arr[i] = node->data;
    i++;
    TreeNodesToDataArray(node->right, arr, i);
  }

  Node<Key, Data, Extra> * getMaxNode() const{
    Node<Key, Data, Extra> * max = root;
    while (max && max->right) 
    {
      max = max->right;
    }
    return max;
  }

  /*NEW - Spring 2023*/

  void getPredecessor_aux(Key key, Node<Key, Data, Extra>* curr, Key *predecessor, bool& changed){
    if(!curr){
      return;
    }
    if(curr->key >= key){ //going left
      getPredecessor_aux(key, curr->left, predecessor, changed);
    } else { //going right
      *predecessor =  curr->key;
      changed = true;
      getPredecessor_aux(key, curr->right, predecessor, changed);
    }
  }

    /*
    *adds extra to every node with key less or equal to the given key.

    #How it works:
    *Find the node with key
      *If not there get the ancestor (closest from left).
      *in the route to node:
        in every right-step series add extra to the first node in the series.
        in every left-step series decrease the first node by extra. (unless it's the root)
      *if the target has right son, decrease him there
    */
  void updateRange_aux(Node<Key, Data, Extra>* curr, Key key, Extra extra, bool lastWasRight){
    if(!curr){
      return;
    }

    if(key < curr->key){
      if(lastWasRight){curr->extra = curr->extra - extra;}
      updateRange_aux(curr->left, key, extra, false);
    } else if(key > curr->key){
      if(!lastWasRight){curr->extra = curr->extra + extra;}
      updateRange_aux(curr->right, key, extra, true);
    } else {
      if(!lastWasRight){curr->extra = curr->extra + extra;}
      if(curr->right){curr->right->extra = curr->right->extra - extra;}
      return;
    }
  }

  /*
  *returns the sum of extras from root to node(key)
  */
  Extra getSumOfExtras(Node<Key, Data, Extra>* curr, Key key){
    if(!curr){return Extra();}
    if(key < curr->key){
      return curr->extra + getSumOfExtras(curr->left, key);
    } else if(key > curr->key){
      return curr->extra + getSumOfExtras(curr->right, key);
    } else { //reached the target node
      return curr->extra;
    }
  }

  void resetExtras_aux(Node<Key, Data, Extra>* curr){
    if (!curr){
      return;
    }
    curr->extra = Extra();
    resetExtras_aux(curr->left);
    resetExtras_aux(curr->right);
  }
};


#endif

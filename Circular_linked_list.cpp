//Divyansh 1903112
#include <iostream>
#include <algorithm>
using namespace std;

template <typename Object>

/*
    |Circular_list is a circular linked list which consists of nodes and prev and next pointers.
    |This is a class template for a circular linked list 
    |Iterators : iterators and cons_iterators can be created as for any other STL like vectors etc.
    |Getting special node's iterators :begin() and end() returns first and last insertd element respectively.
    |Inserting: Insertion can be done by insert function after the passed iterator.
    |Inserting/removing elements at special location: push_front push_back pop_front.
        pop_back can be used to insert/remove  element at first/last inserted location as sugested by name.
    |Erase: erase() function can be called to delete a particular node or a range of node as acoording 
        to the passed iterator. clear() erases the whole list
    Size: size() functions return size of CLL
    MOST OPERATONS USING ++ -- OPERATORS OR EQUALITY OPEAROTRS CAN BE USED AS WITH ANY OTHER STL, LISTS CAN BE ALSO  ASSIGNED TO OTHER LISTS
    VIEW SPECIFIC FUNCTIONS AND CLASSES TO KNOW IT'S WORKINGS AND ABOUT FUNCTIONS AND OPERATORS SUPPORTED

*/



/*
For IIT GOA CS 220 Assignment for Lab 4
FULLFILLING ASSIGNMENT REQUIREMENTS
add: 
    insert(iterator, const Object & x) adds at a specific location as decteted by first argument
    push_front(Object) adds before first inserted element
    push_back(Object) adds after the last inserted element

remove:
    erase(iterator itr) removes the node coresponding to passed iterator
    erase(iterator from, iterator to) removes all nodes between the passed iterators
    erase(Object &) it removes all nodes with that data equal to passed parameter
    pop_front() removes the oldest inserted element or the element inserted before oldest inserted element
    pop_back() removes the latest inserted element or the elemnts inserted after that
    clear() deletes the whole list

isEmpty:
    empty() functions returns bool value coresponding to is list empty or not
    size() returns the current size of the list
*/
class Circular_list{
    private:

    struct Node{
        Object data;
        Node *prev;
        Node *next;

        Node(const Object & d = Object{ }, Node* p = nullptr, Node *n = nullptr):data{d},prev{p},next{n} {}

        Node (Object &&d, Node *p = nullptr, Node *n = nullptr):data{move(d)},prev{p},next{n} {}
    };

    public:
    class const_iterator{
        public:
        const_iterator():current{nullptr}{}
        const Object & operator* () const{
            return retrieve();
        }
        const_iterator & operator++(){
            current = current->next;
            return *this;
        }
        const_iterator operator++(int){
            const_iterator old = *this;
            ++(*this);
            return old;
        }

        const_iterator & operator--(){
            current = current->prev;
            return *this;
        }
        const_iterator operator--(int){
            const_iterator old = *this;
            --(*this);
            return old;
        }
        bool operator== (const const_iterator & rhs)const{
            return current == rhs.current;
        }
        bool operator != (const const_iterator & rhs)const{
            return !(*this==rhs);
        }

        protected:
        Node *current;

        Object & retrieve() const{
            return current->data;
        }

        const_iterator(Node* p): current{p}
        {}
        friend class Circular_list<Object>;
    };

    class iterator : public const_iterator{
        public:
        iterator(){
            ;
        }
        Object & operator*(){
            return const_iterator::retrieve();
        }
        const Object & operator* () const{
            return const_iterator::operator*();
        }

        iterator & operator++(){
            this->current = this->current->next;
            return *this;
        }

        iterator operator++(int){
            iterator old = *this;
            ++(*this);
            return old;
        }

        iterator & operator--(){
            this->current = this->current->prev;
            return *this;
        }

        iterator operator--(int){
            iterator old = *this;
            --(*this);
            return old;
        }

        protected:
        iterator (Node *p): const_iterator{p}{}
        friend class Circular_list<Object>;
    };


    public:
    Circular_list(){
        init();
    }

    Circular_list(const Circular_list & rhs){
        init();
        for(auto & x : rhs)
            push_back(x);
    }

    ~Circular_list(){
        clear();
        delete head;
        delete tail;
    }
    Circular_list & operator= (const Circular_list & rhs ){
        auto b = rhs.begin();
        auto c = rhs.begin();
        int counter = 0;
        do{
            this->push_back(*(b++));
        }
        while(b!=c);
        return *this;
        
        
    }
    Circular_list(Circular_list && rhs):theSize{rhs.size()},head{rhs.head},tail{rhs.tail}{
        rhs.theSize = 0;
        rhs.head = nullptr;
        rhs.tail = nullptr;
    }
    Circular_list & operator= (Circular_list && rhs){
        swap( theSize, rhs.theSize );
        swap( head, rhs.head );
        swap( tail, rhs.tail );

        return *this;
    }


    iterator begin(){
        return {head->next};
    }
    const_iterator begin() const{
        return{head->next};
    }
    iterator end(){
        return {tail};
    }
    const_iterator end() const{
        return {tail};
    }

    int size() const{
        return theSize;
    }

    bool empty(){
        return size() == 0;
    }

    void clear(){
        while(!empty())
            pop_front();
    }

    Object & front(){
        return *begin();
    }

    const Object & front() const{
        return *begin();
    }

    Object & back(){
        return *--end();
    }

    const Object & back() const{
        return *--end();
    }

    void push_front(const Object & x){
        insert(begin(),x);
        circularise();
    }
    void push_front(const Object && x){
        insert(begin(),std::move(x));
        circularise();
    }
    void push_back(const Object &x){
        insert(end(),x);
        circularise();
    }

    void push_back(const Object &&x){
        insert(end(),std::move(x));
        circularise();
    }

    int print(){
        cout << "[circular list] ";
        iterator b = begin();
        if(empty())return 0;
        cout << "..." ;
        do{
        cout << *(b++) <<" ";
        }
        while(b!=begin());
        cout << "..." << endl;
        return size();
    }

    void pop_front(){
        erase(begin());
        circularise();
    }
    void pop_back(){
        erase(--end());
        circularise();
    }

    iterator insert(iterator itr, const Object & x)
    {
        Node *p = itr.current;
        theSize++;
        
        p->prev = p->prev->next = new Node{x,p->prev,p};
        circularise();
        return p->prev;
        
    }

    iterator insert(iterator itr, const Object && x)
    {
        Node *p = itr.current;
        theSize++;
        p->prev = p->prev->next = new Node{move(x),p->prev,p};
        circularise();
        return p->prev;
        
    }


    iterator erase(iterator itr)
    {
        Node *p = itr.current;
        iterator retVal {p->next};
        p -> prev->next = p->next;
        p -> next->prev = p->prev;
        delete p;
        theSize--;
        circularise();
        return retVal;
    }

    iterator find(const Object & x){
        iterator a = begin();
        int count = 0;
        int flag = 0;
        while(*a!=x){
            ++a;
            ++count;
            if(count > size()){
                flag = 1;
                break;
            }
        }
        if(flag ==1){
            return tail;
        }
        return a;
    }

    void erase(const Object & x){
        while(true){
            iterator a  = find(x);
            if(a == tail) break;
            erase(a);
        }
        
    }

    iterator erase(iterator from, iterator to)
    {
        for(iterator itr = from; itr != to;)
            itr = erase (itr);
            circularise();
        return to;
    }


    private:
    int theSize;
    Node * head;
    Node * tail;

    void circularise(){
        //deleting the content of this function will convert the list into linear non circular linked list
        if(size()>=1){
            tail->prev->next = head->next;
            head->prev = tail->prev;
    
        }
    }
        

    void init(){
        theSize = 0;
        head = new Node;
        tail = new Node;
        head->next = tail;
        tail->prev = head;
        circularise();
    }
};


int main(){
    // Circular_list<int> n;
    // n.push_front(5);
    // n.push_front(10);
    // n.push_front(3);
    // n.push_front(3);
    // n.push_front(5);
    // Circular_list<int>::const_iterator a = n.begin();
    // do{
    //     cout << *(a++) <<"  ";
        
    // }
    // while(a!=n.begin());
    // n.erase(3);
    // cout << "-------------------" << endl;
    // Circular_list<int>::const_iterator b = n.begin();
    //  do{
    //     cout << *(b++) <<endl;
        
    // }
    // while(b!=n.begin());
    //creating a linked list for ints;
    Circular_list<int> cl1;
    //pushing elemnts just beside the oldest element and newest elemet
    //all the below insertion takes O(1) time
    cl1.push_front(5);
    cl1.push_front(10);
    cl1.push_front(3);
    cl1.push_back(3);
    cl1.push_front(5);
    cl1.push_front(6);
    cl1.push_back(3);
    cl1.push_front(7);
    cl1.push_back(2);
    cl1.push_back(1);
    //printing the list
    cout << "cl1 is " << endl;
    cl1.print();
    //iterating the list using iterators
    //though circular linked lists have no significance of begin but for 
    // the sake of chosing a starting point a node is selected as begin
    Circular_list<int>::iterator a = cl1.begin();
    int num = cl1.back();
    //using *(dereference operator) with iterators
    cout << "the begin node has value = " << *a << endl;
    //using increment/decrement operator with iterators to move forward(clockwise)
    a++;
    cout << "element just beside the begin node in clockwise direction has value = " <<  *a << endl;
    //finding one of the occurance of an element in linked list 
    Circular_list<int>::iterator f = cl1.find(3);
    cout << "the node found has value ="<<*f << " and the next node(clockwise) has value = " <<*(++f) <<endl;
    //returning to previous f 
    --f;
    //making another list by assigning it already created list
    Circular_list<int> cl2;
    cl2 = cl1;
    cout << "cl2 is ";
    int k = cl2.print();
    //inserting an element at a particular location
    auto f1 = cl2.find(6);
    cl2.insert(f1,-1); //will insert -1 before(counter clockwise) first 6(at its location);
    cl2.print();
    auto in = cl2.begin();
    for(int i=0; i<5; i++){
        in++;
    }
    cl2.insert(in,-2);
    cl2.print();
    //erasing a node
    auto e = cl2.begin();
    for(int i=0; i<3; i++){
        e++;
    }
    cl2.erase(e); //will erase the node pointed by e (here 3 forward of begin)
    cl2.print();
    //erasing a integer
    cl2.erase(3); //will delete all occurances of 3
    cl2.print();
    //getting size
    cout << "now size of cl2 is " << cl2.size() << endl;
    //is a list empty;
    Circular_list <int> cl3;
    cout << "empty() for cl2 returns " <<cl2.empty() << " and for cl3 returns "<<cl3.empty()<<endl; 
    //deleting the whole list
    cl2.clear();
    cout << "now size of cl2 is "<<cl2.size() <<endl;
    //creating list of other data types
    Circular_list <char> cl4;
    cl4.push_back('D');
    cl4.push_back('i');
    cl4.push_back('v');
    cl4.push_back('y');
    cl4.push_back('a');
    cl4.push_back('n');
    cl4.push_back('s');
    cl4.push_back('h');
    cl4.print();

}

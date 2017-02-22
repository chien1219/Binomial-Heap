#include<iostream>
using namespace std;

//每個樹不同LEVEL，否則樹根值較小的會變新樹根
//被吸到變小後若比PARENT小則交換
//If a new person joined the promenade, he could be seen as a new tree with one level

class Person{
public:
	Person();
	Person(int i, int e);
	~Person();

private:
	Person *parent, *child, *sibling; // the pointers to his parent, child, sibling
	int id; // record Person's id
	int energy; // record Person's energy
	int degree;// record how many levels of this Person's children
	friend class Promenade;//you could access all valuables in class Promenade
};

//ctor
Person::Person(){
	parent = child = sibling = NULL;
	id = 0;
	energy = 0;
	degree = 0;
}

//ctor
Person::Person(int i, int e){// define a new Person with id = i and energy = e
	parent = child = sibling = NULL;
	id = i;
	energy = e;
	degree = 0;
}

//dtor
Person::~Person(){

}

class Promenade{

public:
	Promenade();
	~Promenade();
	void one_person_joined(int id, int energy);
	void one_group_joined(Promenade *g);
	void absorb_energy(int id, int energy);
	int  calculate_people_below(int energy);
	int  get_weakest_person();
	int  size();
	/////////////////////////////////////
	void Initializeheap(int,int);
	//Merge
	void mergeTree(Person* ,Person* );
	void Merge(Promenade &);
	void merge(Promenade &);
	//////////////////
	Person* Search(Person*,int);
	void deleteMin();
	/////////////////////////////////////
private:
	Person *head;
	int num;//caculate how many people in the promenade
};

//ctor
Promenade::Promenade(){
	head = NULL;
	num = 0;
}

//dtor
Promenade::~Promenade(){

}
void Promenade::Initializeheap(int i,int e)
{
    Person* np = new Person(i,e);
    	head = np;
	num = 0;
}
void Promenade::mergeTree(Person* y,Person* z){
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;
}
void Promenade::Merge(Promenade &bh){
	
        merge(bh);  
        Person* previous = NULL;
           Person* x = head;
           Person* next = x->sibling;
           while(next){
               if( (x->degree != next->degree) || (next->sibling && next->sibling->degree == x->degree)){
                   previous = x;  
                x = next;  
            }
               else if( x->energy <= next->energy){ 
                x->sibling = next->sibling; 
                   mergeTree(next,x);             
               }
               else{
                if(!previous){                   
                       head = next;           
                }
                   else{
                       previous->sibling = next;       
                   }
                   mergeTree(x,next);           
                x = next;                    
               }
               next = next->sibling;           
           }
       }


void Promenade::merge(Promenade &bh){
	
	Person* head2 = bh.head;
	    Person* head1 = head;
    
	    Person* newHead, *newCurr;

	    if(!head1){          
	     head = head2;
	        return ;
	    }
	  else if(!head2){          
	     return ;
	  }

	 if(head1->degree > head2->degree){
	        newHead = newCurr = head2;
	        head2 = head2->sibling;
	    }
	    else {
	        newHead = newCurr = head1;
	        head1 = head1->sibling;
	    }

	    while(head1 && head2){
	        if(head1->degree < head2->degree){
	            newCurr->sibling = head1;
	            newCurr = head1;
	            head1 = head1->sibling;
	        }
	        else {
	            newCurr->sibling = head2;
	            newCurr = head2;
	            head2 = head2->sibling;
	        }

	    }
	    while(head1){
	        newCurr->sibling = head1;
	        newCurr = head1;
	        head1 = head1->sibling;
	    }
	    while(head2){
	        newCurr->sibling = head2;
	        newCurr = head2;
	        head2 = head2->sibling;
	    }
    
	    head = newHead;
	}
	
	
	


void Promenade::one_person_joined(int id, int energy){
	//TODO: fill in the code to simulate a person with his id and energy to join the Promenade
	Promenade *new_heap = new Promenade();
	new_heap->Initializeheap(id,energy);
	Merge(*new_heap);
	num++;
	
}

void Promenade::one_group_joined(Promenade *g){
	//TODO: fill in the code to simulate one group to join the full promenade
    Person* tmp = g->head;
    int count=0;
    while(tmp){
        count+= (1<<tmp->degree); 
     tmp = tmp->sibling;
    }
    num+=count;
	Merge(*g);	
	
}

void Promenade::absorb_energy(int id, int energy){
	//TODO: Elizabeth absord the energy of the person with this id, this Person's energy will decrease by this number.
	//If his energy becomes lower than or equal to zero, he will die and should be thrown out automatically
	
	//二種情況，刪除後較老爸小就上移，if小於零就刪掉
	Person *x = Search(head,id);
	if(x==NULL)return;
	else   {
	    x->energy -= energy;
	    if(x->energy <= 0){
	
		    while(x->parent!=NULL){
			    int temp = x->parent->energy;
			    x->parent->energy = x->energy;
			    x->energy = temp;
			    int tmp = x->parent->id;
			    x->parent->id = x->id;
			    x->id = tmp;
			    x=x->parent;			    
		    }
		    if(num==1) head = NULL;
		    else  deleteMin();
			 num--;
			 return;
	    }
	    else if(x->energy >0)
	    {
		    while(  (x->parent!=NULL) && x->energy < x->parent->energy ){
			    int bb = x->parent->energy;
			    x->parent->energy = x->energy;
			    x->energy = bb;
			    int aa = x->parent->id;
			    x->parent->id = x->id;
			    x->id = aa;
			    x=x->parent;	
		    }
		    return;
	    }
	    
	  }	
}

int Promenade::calculate_people_below(int energy){
	//TODO: You should calculate how many people's energy are below the number (include the number) and return it
	int number=0;
	Person* y=head;
	Person * tmp;
	while(y){
		
	if(y->energy <= energy){
		
			number++;
	        if(y->child) y=y->child;
		else if(y->sibling) y=y->sibling;
		else if(y->parent) y=y->parent->sibling;
		else return number;
	}
	else if(y->sibling) y=y->sibling;
	else if(y->parent) y=y->parent->sibling;	
	else return number;	
		
	}
	
	return number;
}

int Promenade::get_weakest_person(){
	//TODO: return the id of the one with the least energy left. If there are more than one solution, just return one of them
        Person* x = head;
         int min = INT_MAX;
	 int id; 
         if(x==NULL) 
	 return 0;

	 while(x!=NULL){
           if(x->energy < min){
           min = x->energy;
   	   id = x->id;
		}
           x = x->sibling;
   }
           return id;
   
}

int Promenade::size(){
	return num;

}

Person* Promenade::Search(Person* head,int k)
{
    Person* x = head;
    Person* p = NULL;
    if (x->id == k)
    {
        p = x;
        return p;
    }
    if (x->child != NULL && p == NULL)
        p = Search(x->child, k);
    if (x->sibling != NULL && p == NULL)
        p = Search(x->sibling, k);
    return p;
}



void Promenade::deleteMin(){
    int min = head->energy;
    Person* tmp = head;
    Person* min_previous = NULL;
    Person* min_next = head;
   	
 while(tmp->sibling){
        if(tmp->sibling->energy< min){
            min = tmp->sibling->energy;
            min_previous = tmp;
            min_next = tmp->sibling;
        }
        tmp = tmp->sibling;
    }
    if(!min_previous && min_next) //最小棵是第一個
     head = min_next->sibling;
    else if(min_previous && min_next)
        min_previous->sibling = min_next->sibling;
    

    Person *pre,*curr;
    
 pre = tmp = NULL;
    curr = min_next->child; 
    while(curr){
        tmp = curr->sibling;
        curr->sibling = pre;
        curr->parent = NULL;
        pre = curr;
        curr = tmp;
    }
  Promenade *bh=new Promenade();
    bh->head = pre;
 Merge(*bh);

}
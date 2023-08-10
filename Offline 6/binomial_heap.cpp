#include <bits/stdc++.h>
using namespace std;

class Node{
public:
    int data, degree;
    Node *Lchild, *Rsibling;
    Node(int d){
        data = d;
        degree = 0;
        Lchild = Rsibling = nullptr;
    }
};

class binomial_heap{
    Node* root;
    Node* min_node;
public:
    int getDegree(Node* h)
    {
        if(h == nullptr)
            return INT32_MAX;
        return h->degree;
    }
    void calcMin()
    {
        Node* cur;
        min_node = cur = root;
        while(cur != nullptr){
            if(cur->data < min_node->data)
                min_node = cur;
            cur = cur->Rsibling;
        }
    }
    void merge(binomial_heap &h)
    {
        if(h.root== nullptr)
            return;
        Node* prv = nullptr;
        Node* cur1 = root;
        Node* cur2 = h.root;
        //adjusting merged heap's root
        if(getDegree(cur1) <= getDegree(cur2)){
            prv = cur1;
            cur1 = cur1->Rsibling;
        }
        else{
            root = cur2;
            prv = cur2;
            cur2 = cur2->Rsibling;
        }
        //merging 2 heaps
        while(cur1 != nullptr || cur2 != nullptr){
            if(getDegree(cur1) <= getDegree(cur2)){
                prv->Rsibling = cur1;
                prv = cur1;
                cur1 = cur1->Rsibling;
            }
            else{
                prv->Rsibling = cur2;
                prv = cur2;
                cur2 = cur2->Rsibling;
            }
        }
        h.root = nullptr;
    }
    binomial_heap(){
        min_node = root = nullptr;
    }

    int FindMin()
    {
        return min_node->data;
    }

    void Union(binomial_heap &h)
    {
        merge(h);
        Node* prv = nullptr;
        Node* cur = root;
        while (cur->Rsibling != nullptr)
        {
            if(getDegree(cur)!=getDegree(cur->Rsibling) || getDegree(cur->Rsibling) == getDegree(cur->Rsibling->Rsibling))
            {
                prv = cur;
                cur = cur->Rsibling;
            }
            else if(cur->data <= cur->Rsibling->data)
            {
                Node* child = cur->Lchild;
                cur->Lchild = cur->Rsibling;
                cur->Rsibling = cur->Rsibling->Rsibling;
                cur->Lchild->Rsibling = child;
                cur->degree++;
            }
            else
            {
                Node* temp = cur->Rsibling;
                if(prv == nullptr)
                    root = temp;
                else
                    prv->Rsibling = temp;
                cur->Rsibling = temp->Lchild;
                temp->Lchild = cur;
                temp->degree++;
                cur = temp;
            }
            if(cur == nullptr)
                break;
        }

        //updating min_node
        calcMin();
    }

    void Insert(int d)
    {
        if(root == nullptr){
            min_node = root = new Node(d);
            return;
        }
        Node* temp = new Node(d);
        binomial_heap h;
        h.root = temp;
        Union(h);
    }

    int ExtractMin()
    {
        int retval = min_node->data;
        Node* prv = nullptr;
        Node* cur = root;
        while(cur != min_node){
            prv = cur;
            cur = cur->Rsibling;
        }
        if(prv == nullptr && cur->Rsibling == nullptr)
        {
            root = cur->Lchild;
            calcMin();
            return retval;
        }
        else if(prv == nullptr)
            root = cur->Rsibling;
        else
            prv->Rsibling = cur->Rsibling;
        cur = cur->Lchild;
        if(cur == nullptr)
        {
            calcMin();
            return retval;
        }
        stack<Node*> nodstk;
        while(cur != nullptr)
        {
            nodstk.push(cur);
            cur = cur->Rsibling;
        }
        Node* tmp = nodstk.top();
        cur = tmp;
        nodstk.pop();
        while(!nodstk.empty())
        {
            tmp->Rsibling = nodstk.top();
            nodstk.pop();
            tmp = tmp->Rsibling;
        }
        tmp->Rsibling = nullptr;
        binomial_heap temp;
        temp.root = cur;
        Union(temp);
        return retval;
    }
    vector<int> ncrvec(int n)
    {
        vector<int> vec;
        vec.push_back(1);
        for(int i=1; i<=n; i++)
        {
            vec.push_back((vec[i-1]*(n-i+1))/i);
        }
        return vec;
    }
    void Print()
    {
        cout<<"Printing Binomial Heap... \n";
        Node* cur = root;
        while(cur != nullptr)
        {
            cout<<"Binomial Tree, B"<<cur->degree<<endl;
            vector<int> nodlvl = ncrvec(cur->degree);
            cout<<"Level 0 : "<<cur->data<<"\n";
            if(cur->Lchild == nullptr)
            {
                cur = cur->Rsibling;
                continue;
            }
            cout<<"Level 1 : ";
            int level = 1, cnt= 0;
            queue<Node *> nodq;
            nodq.push(cur);
            while (!nodq.empty())
            {
                Node* temp = nodq.front()->Lchild;
                nodq.pop();
                if(temp == nullptr)
                    continue;
                nodq.push(temp);
                while (temp!= nullptr)
                {
                    cout<<temp->data<<" ";
                    cnt++;
                    if(temp->Rsibling != nullptr)
                        nodq.push(temp->Rsibling);
                    temp = temp->Rsibling;
                }
                if(level <= cur->degree && cnt == nodlvl[level])
                {
                    cnt = 0;
                    level++;
                    cout<<endl;
                    if(level <= cur->degree)
                        cout<<"Level "<<level<<" : ";
                }
            }
            cur = cur->Rsibling;
        }
    }
};

int main()
{
    binomial_heap h1, h2;
    h1.Insert(5);
    cout<<h1.FindMin()<<endl; //5
    h1.Insert(2);
    cout<<h1.FindMin()<<endl; //2
    h1.Insert(10);
    cout<<h1.FindMin()<<endl;
    h1.Print();//2
    h1.Insert(15);
    h1.Print();
    h1.Insert(9);
    h1.Print();
    h1.Insert(1);
    h1.Print();
    cout<<"Find-Min returned "<<h1.FindMin()<<endl; //1
    cout<<"Extract-Min returned "<<h1.ExtractMin()<<endl; //1
    h1.Print();
    cout<<h1.FindMin()<<endl; //2
    cout<<h1.ExtractMin()<<endl; //2
    cout<<h1.FindMin()<<endl; //5
    return 0;
}
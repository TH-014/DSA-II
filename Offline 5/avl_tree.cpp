#include<bits/stdc++.h>
using namespace std;

#define _clock chrono::high_resolution_clock::now()
#define nano_t chrono::duration_cast<std::chrono::nanoseconds>
#define micro_t chrono::duration_cast<std::chrono::microseconds>
typedef long long ll;
typedef long double ld;

template<class T>
class Node{
public:
    Node<T> *left, *right, *parent;
    T data;
    int height;
    Node(T d=0, Node<T> *p=NULL, Node<T> *lc=NULL, Node<T> *rc=NULL)
    {
        parent = p;
        left = lc;
        right = rc;
        data = d;
        height=1;
    }
    bool operator==(Node &p)
    {
        if(p.data == data)
            return true;
        return false;
    }
};

template<class T>
class AVL{
protected:
    Node<T> *root;
    bool flag;
    void delNode(Node<T> *p)
    {
        p->left = p->right = p->parent = NULL;
        delete p;
    }
    void print(Node<T> *p, ostream &stream)
    {
        if(p==NULL)
            return;
        stream<<p->data<<" ";
        if(p->left==NULL && p->right==NULL)
            return;
        stream<<"(";
        print(p->left, stream);
        stream<<",";
        print(p->right, stream);
        stream<<") ";
    }
    void setHeight(Node<T> *p)
    {
        if(p==NULL)
            return;
        p->height = max(getHeight(p->left), getHeight(p->right)) +1;
    }
    int getHeight(Node<T> *p)
    {
        if(p == NULL)
            return 0;
        return p->height;
    }
    int balance_factor(Node<T> *p)
    {
        // cerr<<"balance factor of "<<p->data<<" is "<<getHeight(p->left)<<" - "<<getHeight(p->right)<<endl;
        return getHeight(p->left) - getHeight(p->right);
    }

    void left_rotate(Node<T> *p)
    {
        Node<T> *child = p->right;
        Node<T> *par = p->parent;
        child->parent = par;
        if(par!=NULL && par->left==p)
            par->left = child;
        else if(par!=NULL && par->right==p)
            par->right = child;
        p->right = child->left;
        if(child->left!=NULL)
            child->left->parent = p;
        child->left = p;
        p->parent = child;
        if(p==root)
            root = child;
        setHeight(p->right);
        setHeight(p);
        setHeight(child);
    }


    void right_rotate(Node<T> *p)
    {
        Node<T> *child = p->left;
        Node<T> *par = p->parent;
        child->parent = par;
        if(par!=NULL && par->left==p)
            par->left = child;
        else if(par!=NULL && par->right==p)
            par->right = child;
        p->left = child->right;
        if(child->right!=NULL)
            child->right->parent = p;
        child->right = p;
        p->parent = child;
        if(p==root)
            root = child;
        setHeight(p->left);
        setHeight(p);
        setHeight(child);
    }

    void balance_insert(Node<T> *p, T val)
    {
        //balance
        if(flag==true || p==NULL) //after root is balanced
            return;
        setHeight(p);
        // cerr << "Inserting "<<val<<endl;
        //cerr<<"height of "<<p->data<<" is "<<getHeight(p)<<endl;
        int bfactor = balance_factor(p);
        // cerr<<"bfactor of "<<p->data<<" is "<<bfactor<<endl;
        if(bfactor>1) //left-left or left-right case
        {
            flag=true;
            if(val>p->left->data) //left-right case
            {
                // cerr<<"rotating left\n";
                left_rotate(p->left);
            }
                
            //left-left case
            // cerr<<"rotating right\n";
            right_rotate(p);
        }
        else if(bfactor<-1) //right-right or right-left case
        {
            flag=true;
            if(val<p->right->data) //right-left case
            {
                // cerr<<"rotating right\n";
                right_rotate(p->right);
            }
            
            //right-right case
            // cerr<<"rotating left\n";
            left_rotate(p);
        }
        balance_insert(p->parent, val);
    }

    void balance_delete(Node<T>* p)
    {
        if(p==NULL) //after root is balanced
            return;
        Node<T>* next = p->parent;
        setHeight(p);
        //cerr<<"height of "<<p->data<<" is "<<getHeight(p)<<endl;
        int bfactor = balance_factor(p);
        //cerr<<"bfactor of "<<p->data<<" is "<<bfactor<<endl;
        if(bfactor>1) //left-left or left-right case
        {
            if(balance_factor(p->left)<0) //left-right case
            {
                //cerr<<"rotating left\n";
                left_rotate(p->left);
            }
                
            //left-left case
            //cerr<<"rotating right\n";
            right_rotate(p);
        }
        else if(bfactor<-1) //right-right or right-left case
        {
            if(balance_factor(p->right)>0) //right-left case
            {
                //cerr<<"rotating right\n";
                right_rotate(p->right);
            }
            
            //right-right case
            //cerr<<"rotating left\n";
            left_rotate(p);
        }
        balance_delete(next);
    }


    Node<T> *search(T d) //returns address of 'd' if found //otherwise returns address of a node whose child could be 'd'
    {
        Node<T> *p = root;
        Node<T> *q = NULL; //returns null only if root is NULL
        while (p!=NULL)
        {
            if(p->data==d)
                return p;
            else if(p->data>d)
            {
                q = p;
                p = p->left;
            }
            else
            {
                q = p;
                p = p->right;
            }
        }
        return q;
    }
    void del(Node<T> *p)
    {
        T data = p->data;
        //cerr<<"deletion of "<<data<<endl;
        if(p==NULL) return;
        if(p->left==NULL && p->right==NULL) //deleting a leaf-node
        {
            Node<T> *q = p->parent;
            if(q==NULL) //while deleting the root
                root = NULL;
            else if(p==q->left) //p is the left child of q
                q->left = NULL;
            else //p is the right child of q
                q->right = NULL;
            delNode(p); 
            balance_delete(q);
        }

        else if(p->left==NULL) //p has a right child only
        {
            Node<T>* q = p;
            p = p->right;
            if(p->left==NULL)
            {
                p->parent = q->parent;
                if(q->parent==NULL) //if root is to be deleted
                    root = p;
                else{
                    if(q->parent->left==q)
                        q->parent->left=p;
                    else
                        q->parent->right=p;
                }
                balance_delete(p);
                delNode(q);
            }
            else{
                while(p->left!=NULL)
                    p = p->left;
                q->data = p->data;
                p->parent->left = p->right;
                if(p->right!=NULL)
                    p->right->parent = p->parent;
                balance_delete(p->parent);
                delNode(p);
            }
        }

        else //p has a left child (may or may not have a right child)
        {
            Node<T>* q = p;
            p = p->left;
            if(p->right==NULL)
            {
                p->parent = q->parent;
                p->right = q->right;
                if(q->right!=NULL)
                    q->right->parent = p;
                if(q->parent==NULL)
                    root = p;
                else if(q->parent->left==q)
                    q->parent->left = p;
                else
                    q->parent->right = p;
                balance_delete(p); 
                delNode(q);
            }
            else{
                while(p->right!=NULL)
                    p = p->right;
                q->data = p->data;
                p->parent->right = p->left;
                if(p->left!=NULL)
                    p->left->parent = p->parent;
                balance_delete(p->parent);
                delNode(p);
            }
        }
    }
    void inorder(Node<T> *p)
    {
        if(p==NULL)
            return;
        inorder(p->left);
        cout<<p->data<<" ";
        inorder(p->right);
    }
    void preorder(Node<T> *p)
    {
        if(p==NULL)
            return;
        cout<<p->data<<" ";
        preorder(p->left);
        preorder(p->right);
    }
    void postorder(Node<T> *p)
    {
        if(p==NULL)
            return;
        postorder(p->left);
        postorder(p->right);
        cout<<p->data<<" ";
    }
public:
    AVL(){
        root = NULL;
    }
    void Insert(T data)
    {
        Node<T> *p;
        if(root==NULL) //in case of empty tree
            root = new Node<T>(data);
        else{
            Node<T> *q = search(data); // q is the parent of p
            p = new Node<T>(data , q);
            if(data>q->data)
                q->right = p;
            else
                q->left = p;
            setHeight(q);
            flag = false;
            balance_insert(q, data);
        }
        // cerr<<data<<" inserted into AVL\n";
    }
    void Delete(T data)
    {
        //cerr<<"Deleting "<<data<<endl;
        Node<T> *p = search(data);
        if(p==NULL || p->data!=data)
        {
            //cerr<<"data not found\n";
            print(root, cout);
            cout<<"\n";
            return;
        }
        //cerr<<"del calling for "<<p->data<<endl;
        del(p);
        //cerr<<"del returning\n";
        print(root, cout);
        cout<<"\n";
        //cerr<<"node deleted successfully\n";
    }
    bool Find(T data)
    {
        //cout<<"Searching for "<<data<<endl;
        Node<T> *p = search(data);
        if(p==NULL)
            return false;
        if(p->data == data)
            return true;
        return false;
    }
    void Traversal()
    {
        inorder(root);
        cout<<endl;
    }
    friend ostream &operator <<(ostream &stream , AVL &ob){
        stream<<"";
        ob.print(ob.root, stream);
        stream<<"";
    }
};

int main()
{
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    // freopen("err.txt", "w", stderr);
    ld dur_insert=0, dur_delete=0, dur_traversal=0, dur_find=0, dur_total;
    
    AVL<int> avl_tree;
    string str;
    while (getline(cin, str))
    {
        auto tbegin = _clock;
        auto tend = _clock;
        ld duration;
        int num=0;
        char cmd = str[0];
        for(int i=2; i<str.length();i++)
            num = (str[i]-'0')+ num*10;
        //cerr<<cmd<<" "<<num<<endl;
        switch (cmd)
        {
        case 'I':
            tbegin = _clock;
            avl_tree.Insert(num);
            cout<<avl_tree<<endl;
            tend = _clock;
            duration = nano_t (tend-tbegin).count();
            dur_insert+=duration;
            break;
        case 'D':
            tbegin = _clock;
            avl_tree.Delete(num);
            tend = _clock;
            duration = nano_t (tend-tbegin).count();
            dur_delete+=duration;   
            break;
        case 'F':
            tbegin = _clock;
            if(avl_tree.Find(num))
                cout<<"found\n";
            else
                cout<<"not found\n";
            tend = _clock;
            duration = nano_t (tend-tbegin).count();
            dur_find+=duration;
            break;
        case 'T':
            tbegin = _clock;
            avl_tree.Traversal();
            tend = _clock;
            duration = nano_t (tend-tbegin).count();
            dur_insert+=duration;
            break;
        default:
            break;
        }
    }
    fclose(stdout);
    freopen("report_avl.txt", "w", stdout);

    cout<<"operation time(ms)\n";
    dur_total = dur_delete+dur_insert+dur_find+dur_traversal;
    cout<<"insert "<<setprecision(16)<<(dur_insert/1e6)<<endl;
    cout<<"delete "<<setprecision(16)<<(dur_delete/1e6)<<endl;
    cout<<"search "<<setprecision(16)<<(dur_find/1e6)<<endl;
    cout<<"trav "<<setprecision(16)<<(dur_traversal/1e6)<<endl;
    cout<<"total "<<setprecision(16)<<(dur_total/1e6)<<endl;

    return 0;
}
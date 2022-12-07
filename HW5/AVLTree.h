#define stature(p) ((p)?(p->height):-1)
#define IsRoot(x) (!x->parent)
#define IsLchild(x) (!IsRoot(x)&&(x==x->parent->left))
#define IsRchild(x) (!IsRoot(x)&&(x==x->parent->right))
#define tallerChild(x) ( \
    stature(x->left) > stature(x->right)?x->left:(\
    stature(x->left) < stature(x->right)?x->right:(\
    IsLchild(x)?x->left:x->right\
    )\
    )\
)
struct AVLNode
{
    int key;
    int value;
    AVLNode* parent;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(int tkey,int tvalue,AVLNode* tparent):key(tkey),value(tvalue),parent(tparent)
    {
        left=nullptr;
        right=nullptr;
        height=0;
    }
};


class AVLTree
{
private:
    AVLNode* root;
    AVLNode* nowparent;
    int size;
    int updateheight(AVLNode* x);
    AVLNode* & binsearch(int key,AVLNode* & v,AVLNode* & parent);
    AVLNode* connect34(
        AVLNode* p1,AVLNode* p2,AVLNode* p3,
        AVLNode* c1,AVLNode* c2,AVLNode* c3,AVLNode* c4
    );
    AVLNode* rotateAt(AVLNode* v);
    AVLNode* &gettnode(AVLNode* x);
public:
    AVLTree()
    {
        size = 0;
        root = nullptr;
        nowparent = nullptr;
    }
    ~AVLTree()
    {
    }
    AVLNode* & search(int key);
    void Insert(int key,int value);
};
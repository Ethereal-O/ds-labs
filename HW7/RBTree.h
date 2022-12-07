#define IsBlack(p) (!(p)||(p->color==RB_BLACK))
#define IsRed(p) (!IsBlack(p))
#define stature(p) ((p)?(p->height):-1)
#define IsRoot(x) (!x->parent)
#define IsLchild(x) (!IsRoot(x)&&(x==x->parent->left))
#define IsRchild(x) (!IsRoot(x)&&(x==x->parent->right))
#define uncle(x)\
(IsLchild(x->parent)?x->parent->right:x->parent->left)
#define tallerChild(x) ( \
    stature(x->left) > stature(x->right)?x->left:(\
    stature(x->left) < stature(x->right)?x->right:(\
    IsLchild(x)?x->left:x->right\
    )\
    )\
)
#define BlackHeightUpdatedz(x)( \
    (stature(x->left)==stature(x->right))&& \
    ((x)->height ==IsRed(x)?stature(x->left):stature(x->left)+1) \
)


typedef enum {RB_RED, RB_BLACK} RBColor;

struct RBNode
{
    int key;
    int value;
    RBNode* parent;
    RBNode* left;
    RBNode* right;
    RBColor color;
    int height;
    RBNode(int tkey,int tvalue,RBNode* tparent,int theight=0,RBColor tcolor=RB_RED):key(tkey),value(tvalue),parent(tparent),height(theight),color(tcolor)
    {
        left=nullptr;
        right=nullptr;
    }
};

class RBTree
{
private:
    RBNode* root;
    RBNode* nowparent;
    int size;
    int updateheight(RBNode* x);
    RBNode* & binsearch(int key,RBNode* & v,RBNode* & parent);
    void solveDoubleRed(RBNode* x);
    RBNode* connect34(
        RBNode* p1,RBNode* p2,RBNode* p3,
        RBNode* c1,RBNode* c2,RBNode* c3,RBNode* c4
    );
    RBNode* rotateAt(RBNode* v);
    RBNode* &gettnode(RBNode* x);
public:
    int rotatenum=0;
    RBTree()
    {
        size=0;
        root=nullptr;
        nowparent=nullptr;
    }
    ~RBTree()
    {
    }
    RBNode* & Search(int key);
    void Insert(int key, int value);
};
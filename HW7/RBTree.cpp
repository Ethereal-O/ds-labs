#include "RBTree.h"
#include <algorithm>

int RBTree::updateheight(RBNode*x)
{
    x->height = std::max(stature(x->left),stature(x->right));
    return IsBlack(x)?x->height++:x->height;
}
RBNode* & RBTree::binsearch(int key,RBNode* & v,RBNode* & parent)
{
    if (!v||v->key==key) return v;
    parent =v;
    return binsearch(key,((key<v->key)?v->left:v->right),parent);
}

RBNode* RBTree::connect34(
    RBNode* p1,RBNode* p2,RBNode* p3,
    RBNode* c1,RBNode* c2,RBNode* c3,RBNode* c4)
{
    p1->left = c1; if (c1) c1->parent = p1;
    p1->right = c2; if (c2) c2->parent = p1;
    p3->left = c3; if (c3) c3->parent = p3;
    p3->right = c4; if (c4) c4->parent = p3;
    p2->left = p1; p1->parent=p2;
    p2->right = p3; p3->parent=p2;
    return p2;
}

RBNode* RBTree::rotateAt(RBNode* v)
{
    rotatenum++;
    RBNode* p=v->parent;
    RBNode* g=p->parent;
    if (IsLchild(p))
    {
        if (IsLchild(v))
        {
            p->parent = g->parent;
            return connect34(v,p,g,v->left,v->right,p->right,g->right);
        }else{
            v->parent = g->parent;
            return connect34(p,v,g,p->left,v->left,v->right,g->right);
        }
    }else{
        if (IsRchild(v))
        {
            p->parent=g->parent;
            return connect34(g,p,v,g->left,p->left,v->left,v->right);
        }else{
            v->parent=g->parent;
            return connect34(g,v,p,g->left,v->left,v->right,p->right);
        }
    }
}
RBNode* & RBTree::Search(int key)
{
    return binsearch(key,root,nowparent=nullptr);
}

RBNode* & RBTree::gettnode(RBNode* x)
{
    if (!IsRoot(x)) return x->parent->key>x->key?x->parent->left:x->parent->right;
    return root;
}

void  RBTree::solveDoubleRed(RBNode* x)
{
    if (IsRoot(x))
    {
        root->color=RB_BLACK;
        root->height++;
        return;
    }
    RBNode* p=x->parent; if (IsBlack(p)) return;
    RBNode* g=p->parent;
    RBNode* u=uncle(x);
    if(IsBlack(u)){
        if(IsLchild(x)==IsLchild(p)){
            p->color=RB_BLACK;
        }else{
            x->color=RB_BLACK;
        }
        g->color=RB_RED;
        RBNode* gg=g->parent;
        RBNode* r=gettnode(g)=rotateAt(x);
        r->parent=gg;
    }else{
        p->color=RB_BLACK; p->height++;
        u->color=RB_BLACK; u->height++;
        if(!IsRoot(g)){
            g->color = RB_RED;
        }
        solveDoubleRed(g);
    }
}

void RBTree::Insert(int key, int value)
{
    RBNode* &nownode =Search(key);
    if (nownode) return;
    nownode = new RBNode(key,value, nowparent,-1);
    size++;
    solveDoubleRed(nownode);
    return;
}
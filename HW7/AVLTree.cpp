#include "AVLTree.h"
#include <algorithm>


int AVLTree::updateheight(AVLNode* x)
{
    return x->height=std::max(stature(x->left),stature(x->right))+1;
}

AVLNode* & AVLTree::binsearch(int key,AVLNode* & v,AVLNode* & parent)
{
    if (!v||v->key==key) return v;
    parent =v;
    return binsearch(key,((key<v->key)?v->left:v->right),parent);
}

AVLNode* AVLTree::connect34(
    AVLNode* p1,AVLNode* p2,AVLNode* p3,
    AVLNode* c1,AVLNode* c2,AVLNode* c3,AVLNode* c4)
{
    p1->left = c1; if (c1) c1->parent = p1;
    p1->right = c2; if (c2) c2->parent = p1;
    p3->left = c3; if (c3) c3->parent = p3;
    p3->right = c4; if (c4) c4->parent = p3;
    p2->left = p1; p1->parent=p2;
    p2->right = p3; p3->parent=p2;
    return p2;
}

AVLNode* AVLTree::rotateAt(AVLNode* v)
{
    rotatenum++;
    AVLNode* p=v->parent;
    AVLNode* g=p->parent;
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


AVLNode* & AVLTree::Search(int key)
{
    return binsearch(key,root,nowparent=nullptr);
}

AVLNode* & AVLTree::gettnode(AVLNode* x)
{
    if (!IsRoot(x)) return x->parent->key>x->key?x->parent->left:x->parent->right;
    return root;
}

void AVLTree::Insert(int key,int value)
{
    AVLNode* &nownode=Search(key);
    if (nownode) return;
    nownode = new AVLNode(key,value,nowparent);
    AVLNode* tnode = nowparent;
    while(tnode)
    {
        if((stature(tnode->left)-stature(tnode->right)>2)||(stature(tnode->left)-stature(tnode->right)<-2))
        {
            gettnode(tnode) = rotateAt(tallerChild(tallerChild(tnode)));
            break;
        }else{
            updateheight(tnode);
        }
        tnode=tnode->parent;
    }
}
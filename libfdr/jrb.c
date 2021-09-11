/* Revision 1.2.  Jim Plank */

/* Original code by Jim Plank (plank@cs.utk.edu) */
/* modified for THINK C 6.0 for Macintosh by Chris Bartley */
 
/* Revision 1.2.  Jim Plank */

/* Original code by Jim Plank (plank@cs.utk.edu) */
/* modified for THINK C 6.0 for Macintosh by Chris Bartley */
 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "jrb.h"
 
static void mk_new_int(JRB l, JRB r, JRB p, int il);
static JRB lprev(JRB n);
static JRB rprev(JRB n);
static void recolor(JRB n);
static void single_rotate(JRB y, int l);
static void jrb_print_tree(JRB t, int level);
static void jrb_iprint_tree(JRB t, int level);
 
#define isred(n) (n->red)
#define isblack(n) (!isred(n))
#define isleft(n) (n->left)
#define isright(n) (!isleft(n))
#define isint(n) (n->internal)
#define isext(n) (!isint(n))
#define ishead(n) (n->roothead & 2)
#define isroot(n) (n->roothead & 1)
#define getlext(n) ((struct jrb_node *)(n->key.v))
#define setlext(node, val) node->key.v = (void *) (val)
#define getrext(n) ((struct jrb_node *)(n->val.v))
#define setrext(node, value) node->val.v = (void *) (value)
#define setred(n) n->red = 1
#define setblack(n) n->red = 0
#define setleft(n) n->left = 1
#define setright(n) n->left = 0
#define sethead(n) (n->roothead |= 2)
#define setroot(n) (n->roothead |= 1)
#define setint(n) n->internal = 1
#define setext(n) n->internal = 0
#define setnormal(n) n->roothead = 0
#define sibling(n) ((isleft(n)) ? n->parent->blink : n->parent->flink)
#define INFINITY_VALUE 10000000
static void insert(JRB item, JRB list)	/* Inserts to the end of a list */
{
  JRB last_node;
 
  last_node = list->blink;
 
  list->blink = item;
  last_node->flink = item;
  item->blink = last_node;
  item->flink = list;
}
 
static void delete_item(JRB item)		/* Deletes an arbitrary iterm */
{
  item->flink->blink = item->blink;
  item->blink->flink = item->flink;
}

#define mk_new_ext(new, kkkey, vvval) {\
  new = (JRB) malloc(sizeof(struct jrb_node));\
  new->val = vvval;\
  new->key = kkkey;\
  setext(new);\
  setblack(new);\
  setnormal(new);\
}
 
static void mk_new_int(JRB l, JRB r, JRB p, int il)
{
  JRB newnode;
 
  newnode = (JRB) malloc(sizeof(struct jrb_node));
  setint(newnode);
  setred(newnode);
  setnormal(newnode);
  newnode->flink = l;
  newnode->blink = r;
  newnode->parent = p;
  setlext(newnode, l);
  setrext(newnode, r);
  l->parent = newnode;
  r->parent = newnode;
  setleft(l);
  setright(r);
  if (ishead(p)) {
    p->parent = newnode;
    setroot(newnode);
  } else if (il) {
    setleft(newnode);
    p->flink = newnode;
  } else {
    setright(newnode);
    p->blink = newnode;
  }
  recolor(newnode);
}

JRB lprev(JRB n)
{
  if (ishead(n)) return n;
  while (!isroot(n)) {
    if (isright(n)) return n->parent;
    n = n->parent;
  }
  return n->parent;
}
 
JRB rprev(JRB n)
{
  if (ishead(n)) return n;
  while (!isroot(n)) {
    if (isleft(n)) return n->parent;
    n = n->parent;
  }
  return n->parent;
}
 
JRB make_jrb()
{
  JRB head;
 
  head = (JRB) malloc (sizeof(struct jrb_node));
  head->flink = head;
  head->blink = head;
  head->parent = head;
  head->key.s = "";
  sethead(head);
  return head;
}
 
JRB jrb_find_gte_str(JRB n, char *key, int *fnd)
{
  int cmp;
 
  *fnd = 0;
  if (!ishead(n)) {
    fprintf(stderr, "jrb_find_gte_str called on non-head 0x%x\n", n);
    exit(1);
  }
  if (n->parent == n) return n;
  cmp = strcmp(key, n->blink->key.s);
  if (cmp == 0) {
    *fnd = 1;
    return n->blink; 
  }
  if (cmp > 0) return n; 
  else n = n->parent;
  while (1) {
    if (isext(n)) return n;
    cmp = strcmp(key, getlext(n)->key.s);
    if (cmp == 0) {
      *fnd = 1;
      return getlext(n);
    }
    if (cmp < 0) n = n->flink ; else n = n->blink;
  }
}
 
JRB jrb_find_str(JRB n, char *key)
{
  int fnd;
  JRB j;
  j = jrb_find_gte_str(n, key, &fnd);
  if (fnd) return j; else return NULL;
}
 
JRB jrb_find_gte_int(JRB n, int ikey, int *fnd)
{
  *fnd = 0;
  if (!ishead(n)) {
    fprintf(stderr, "jrb_find_gte_int called on non-head 0x%x\n", n);
    exit(1);
  }
  if (n->parent == n) return n;
  if (ikey == n->blink->key.i) {
    *fnd = 1;
    return n->blink; 
  }
  if (ikey > n->blink->key.i) return n; 
  else n = n->parent;
  while (1) {
    if (isext(n)) return n;
    if (ikey == getlext(n)->key.i) {
      *fnd = 1;
      return getlext(n);
    }
    n = (ikey < getlext(n)->key.i) ? n->flink : n->blink;
  }
}
 
JRB jrb_find_int(JRB n, int ikey)
{
  int fnd;
  JRB j;

  j = jrb_find_gte_int(n, ikey, &fnd);
  if (fnd) return j; else return NULL;
}
 
JRB jrb_find_gte_dbl(JRB n, double dkey, int *fnd)
{
  *fnd = 0;
  if (!ishead(n)) {
    fprintf(stderr, "jrb_find_gte_dbl called on non-head 0x%x\n", n);
    exit(1);
  }
  if (n->parent == n) return n;
  if (dkey == n->blink->key.d) {
    *fnd = 1;
    return n->blink; 
  }
  if (dkey > n->blink->key.d) return n; 
  else n = n->parent;
  while (1) {
    if (isext(n)) return n;
    if (dkey == getlext(n)->key.d) {
      *fnd = 1;
      return getlext(n);
    }
    n = (dkey < getlext(n)->key.d) ? n->flink : n->blink;
  }
}
 
JRB jrb_find_dbl(JRB n, double dkey)
{
  int fnd;
  JRB j;

  j = jrb_find_gte_dbl(n, dkey, &fnd);
  if (fnd) return j; else return NULL;
}
 
JRB jrb_find_gte_gen(JRB n, Jval key,int (*fxn)(Jval, Jval), int *fnd)
{
  int cmp;
 
  *fnd = 0;
  if (!ishead(n)) {
    fprintf(stderr, "jrb_find_gte_str called on non-head 0x%x\n", n);
    exit(1);
  }
  if (n->parent == n) return n;
  cmp = (*fxn)(key, n->blink->key);
  if (cmp == 0) {
    *fnd = 1;
    return n->blink; 
  }
  if (cmp > 0) return n; 
  else n = n->parent;
  while (1) {
    if (isext(n)) return n;
    cmp = (*fxn)(key, getlext(n)->key);
    if (cmp == 0) {
      *fnd = 1;
      return getlext(n);
    }
    if (cmp < 0) n = n->flink ; else n = n->blink;
  }
}
 
JRB jrb_find_gen(JRB n, Jval key, int (*fxn)(Jval, Jval))
{
  int fnd;
  JRB j;

  j = jrb_find_gte_gen(n, key, fxn, &fnd);
  if (fnd) return j; else return NULL;
}
 
static JRB jrb_insert_b(JRB n, Jval key, Jval val)
{
  JRB newleft, newright, newnode, p;
 
  if (ishead(n)) {
    if (n->parent == n) {         /* Tree is empty */
      mk_new_ext(newnode, key, val);
      insert(newnode, n);
      n->parent = newnode;
      newnode->parent = n;
      setroot(newnode);
      return newnode;
    } else {
      mk_new_ext(newright, key, val);
      insert(newright, n);
      newleft = newright->blink;
      setnormal(newleft);
      mk_new_int(newleft, newright, newleft->parent, isleft(newleft));
      p = rprev(newright);
      if (!ishead(p)) setlext(p, newright);
      return newright;
    }
  } else {
    mk_new_ext(newleft, key, val);
    insert(newleft, n);
    setnormal(n);
    mk_new_int(newleft, n, n->parent, isleft(n));
    p = lprev(newleft);
    if (!ishead(p)) setrext(p, newleft);
    return newleft;    
  }
}
 
static void recolor(JRB n)
{  
  JRB p, gp, s;
  int done = 0;
 
  while(!done) {
    if (isroot(n)) {
      setblack(n);
      return;
    }
 
    p = n->parent;
 
    if (isblack(p)) return;
    
    if (isroot(p)) {
      setblack(p);
      return;
    }
 
    gp = p->parent;
    s = sibling(p);
    if (isred(s)) {
      setblack(p);
      setred(gp);
      setblack(s);
      n = gp;
    } else {
      done = 1;
    }
  }
  /* p's sibling is black, p is red, gp is black */
  
  if ((isleft(n) == 0) == (isleft(p) == 0)) {
    single_rotate(gp, isleft(n));
    setblack(p);
    setred(gp);
  } else {
    single_rotate(p, isleft(n));
    single_rotate(gp, isleft(n));
    setblack(n);
    setred(gp);
  }
}
 
static void single_rotate(JRB y, int l)
{
  int rl, ir;
  JRB x, yp;
 
  ir = isroot(y);
  yp = y->parent;
  if (!ir) {
    rl = isleft(y);
  }
  
  if (l) {
    x = y->flink;
    y->flink = x->blink;
    setleft(y->flink);
    y->flink->parent = y;
    x->blink = y;
    setright(y);  
  } else {
    x = y->blink;
    y->blink = x->flink;
    setright(y->blink);
    y->blink->parent = y;
    x->flink = y;
    setleft(y);  
  }
 
  x->parent = yp;
  y->parent = x;
  if (ir) {
    yp->parent = x;
    setnormal(y);
    setroot(x);
  } else {
    if (rl) {
      yp->flink = x;
      setleft(x);
    } else {
      yp->blink = x;
      setright(x);
    }
  }
}
    
void jrb_delete_node(JRB n)
{
  JRB s, p, gp;
  char ir;
 
  if (isint(n)) {
    fprintf(stderr, "Cannot delete an internal node: 0x%x\n", n);
    exit(1);
  }
  if (ishead(n)) {
    fprintf(stderr, "Cannot delete the head of an jrb_tree: 0x%x\n", n);
    exit(1);
  }
  delete_item(n); /* Delete it from the list */
  p = n->parent;  /* The only node */
  if (isroot(n)) {
    p->parent = p;
    free(n);
    return;
  } 
  s = sibling(n);    /* The only node after deletion */
  if (isroot(p)) {
    s->parent = p->parent;
    s->parent->parent = s;
    setroot(s);
    free(p);
    free(n);
    return;
  }
  gp = p->parent;  /* Set parent to sibling */
  s->parent = gp;
  if (isleft(p)) {
    gp->flink = s;
    setleft(s);
  } else {
    gp->blink = s;
    setright(s);
  }
  ir = isred(p);
  free(p);
  free(n);
  
  if (isext(s)) {      /* Update proper rext and lext values */
    p = lprev(s); 
    if (!ishead(p)) setrext(p, s);
    p = rprev(s);
    if (!ishead(p)) setlext(p, s);
  } else if (isblack(s)) {
    fprintf(stderr, "DELETION PROB -- sib is black, internal\n");
    exit(1);
  } else {
    p = lprev(s);
    if (!ishead(p)) setrext(p, s->flink);
    p = rprev(s);
    if (!ishead(p)) setlext(p, s->blink);
    setblack(s);
    return;
  }
 
  if (ir) return;
 
  /* Recolor */
  
  n = s;
  p = n->parent;
  s = sibling(n);
  while(isblack(p) && isblack(s) && isint(s) && 
        isblack(s->flink) && isblack(s->blink)) {
    setred(s);
    n = p;
    if (isroot(n)) return;
    p = n->parent;
    s = sibling(n);
  }
  
  if (isblack(p) && isred(s)) {  /* Rotation 2.3b */
    single_rotate(p, isright(n));
    setred(p);
    setblack(s);
    s = sibling(n);
  }
    
  { JRB x, z; char il;
    
    if (isext(s)) {
      fprintf(stderr, "DELETION ERROR: sibling not internal\n");
      exit(1);
    }
 
    il = isleft(n);
    x = il ? s->flink : s->blink ;
    z = sibling(x);
 
    if (isred(z)) {  /* Rotation 2.3f */
      single_rotate(p, !il);
      setblack(z);
      if (isred(p)) setred(s); else setblack(s);
      setblack(p);
    } else if (isblack(x)) {   /* Recoloring only (2.3c) */
      if (isred(s) || isblack(p)) {
        fprintf(stderr, "DELETION ERROR: 2.3c not quite right\n");
        exit(1);
      }
      setblack(p);
      setred(s);
      return;
    } else if (isred(p)) { /* 2.3d */
      single_rotate(s, il);
      single_rotate(p, !il);
      setblack(x);
      setred(s);
      return;
    } else {  /* 2.3e */
      single_rotate(s, il);
      single_rotate(p, !il);
      setblack(x);
      return;
    }
  }
}
 
 
void jrb_print_tree(JRB t, int level)
{
  int i;
  if (ishead(t) && t->parent == t) {
    printf("tree 0x%x is empty\n", t);
  } else if (ishead(t)) {
    printf("Head: 0x%x.  Root = 0x%x\n", t, t->parent);
    jrb_print_tree(t->parent, 0);
  } else {
    if (isext(t)) {
      for (i = 0; i < level; i++) putchar(' ');
      printf("Ext node 0x%x: %c,%c: p=0x%x, k=%s\n", 
              t, isred(t)?'R':'B', isleft(t)?'l':'r', t->parent, t->key.s);
    } else {
      jrb_print_tree(t->flink, level+2);
      jrb_print_tree(t->blink, level+2);
      for (i = 0; i < level; i++) putchar(' ');
      printf("Int node 0x%x: %c,%c: l=0x%x, r=0x%x, p=0x%x, lr=(%s,%s)\n", 
              t, isred(t)?'R':'B', isleft(t)?'l':'r', t->flink, 
              t->blink, 
              t->parent, getlext(t)->key.s, getrext(t)->key.s);
    }
  }
}
 
void jrb_iprint_tree(JRB t, int level)
{
  int i;
  if (ishead(t) && t->parent == t) {
    printf("tree 0x%x is empty\n", t);
  } else if (ishead(t)) {
    printf("Head: 0x%x.  Root = 0x%x, < = 0x%x, > = 0x%x\n", 
            t, t->parent, t->blink, t->flink);
    jrb_iprint_tree(t->parent, 0);
  } else {
    if (isext(t)) {
      for (i = 0; i < level; i++) putchar(' ');
      printf("Ext node 0x%x: %c,%c: p=0x%x, <=0x%x, >=0x%x k=%d\n", 
              t, isred(t)?'R':'B', isleft(t)?'l':'r', t->parent, 
              t->blink, t->flink, t->key.i);
    } else {
      jrb_iprint_tree(t->flink, level+2);
      jrb_iprint_tree(t->blink, level+2);
      for (i = 0; i < level; i++) putchar(' ');
      printf("Int node 0x%x: %c,%c: l=0x%x, r=0x%x, p=0x%x, lr=(%d,%d)\n", 
              t, isred(t)?'R':'B', isleft(t)?'l':'r', t->flink, 
              t->blink, 
              t->parent, getlext(t)->key.i, getrext(t)->key.i);
    }
  }
}
      
int jrb_nblack(JRB n)
{
  int nb;
  if (ishead(n) || isint(n)) {
    fprintf(stderr, "ERROR: jrb_nblack called on a non-external node 0x%x\n",
            n);
    exit(1);
  }
  nb = 0;
  while(!ishead(n)) {
    if (isblack(n)) nb++;
    n = n->parent;
  }
  return nb;
}
 
int jrb_plength(JRB n)
{
  int pl;
  if (ishead(n) || isint(n)) {
    fprintf(stderr, "ERROR: jrb_plength called on a non-external node 0x%x\n",
            n);
    exit(1);
  }
  pl = 0;
  while(!ishead(n)) {
    pl++;
    n = n->parent;
  }
  return pl;
}
 
void jrb_free_tree(JRB n)
{
  if (!ishead(n)) {
    fprintf(stderr, "ERROR: Rb_free_tree called on a non-head node\n");
    exit(1);
  }
 
  while(jrb_first(n) != jrb_nil(n)) {
    jrb_delete_node(jrb_first(n));
  }
  free(n);
}
 
Jval jrb_val(JRB n)
{
  return n->val;
}
 
static JRB jrb_insert_a(JRB nd, Jval key, Jval val)
{
  return jrb_insert_b(nd->flink, key, val);
}

JRB jrb_insert_str(JRB tree, char *key, Jval val)
{
  Jval k;
  int fnd;

  k.s = key;
  return jrb_insert_b(jrb_find_gte_str(tree, key, &fnd), k, val);
}

JRB jrb_insert_int(JRB tree, int ikey, Jval val)
{
  Jval k;
  int fnd;

  k.i = ikey;
  return jrb_insert_b(jrb_find_gte_int(tree, ikey, &fnd), k, val);
}

JRB jrb_insert_dbl(JRB tree, double dkey, Jval val)
{
  Jval k;
  int fnd;

  k.d = dkey;
  return jrb_insert_b(jrb_find_gte_dbl(tree, dkey, &fnd), k, val);
}

JRB jrb_insert_gen(JRB tree, Jval key, Jval val,
                          int (*func)(Jval, Jval))
{ 
  int fnd;

  return jrb_insert_b(jrb_find_gte_gen(tree, key, func, &fnd), key, val);
}

JRB createGraph()
{
  JRB graph;
  graph=make_jrb();
  return graph;
}

void addEdge_int(JRB graph, int v1,int v2)//Thêm cạnh không trọng số
{
  JRB node,tree,node2;
  node=jrb_find_int(graph,v1);
  if(node==NULL)
    {
      tree=make_jrb();
      jrb_insert_int(graph,v1,new_jval_v(tree));
      if(jrb_find_int(tree,v2)==NULL)
      jrb_insert_int(tree,v2,new_jval_i(1));
    }
  else
    {
      tree=make_jrb();
      tree=(JRB)jval_v(node->val);
      node2=jrb_find_int(tree,v2);
      if(node2==NULL)
	{
	  jrb_insert_int(tree,v2,new_jval_i(1));
	}
      // jrb_insert_int(tree,v2,new_jval_i(1));
      node->val=new_jval_v(tree);
    }
}
void addEdge_str(JRB graph, char* v1,char* v2)//Thêm cạnh không trọng số
{
  JRB node,tree,node2;
  node=jrb_find_str(graph,v1);
  if(node==NULL)
    {
      tree=make_jrb();
      jrb_insert_str(graph,v1,new_jval_v(tree));
      jrb_insert_str(tree,v2,new_jval_i(1));
    }
  else
    {
      tree=make_jrb();
      tree=(JRB)jval_v(node->val);
      node2=jrb_find_str(tree,v2);
      if(node2==NULL)
	{
	  jrb_insert_str(tree,v2,new_jval_i(1));
	}
      //jrb_insert_str(tree,v2,new_jval_i(1));
      node->val=new_jval_v(tree);
    }
  node=jrb_find_str(graph,v2);
  if(node==NULL)
    {
      tree=make_jrb();
      jrb_insert_str(graph,v2,new_jval_v(tree));
      jrb_insert_str(tree,v1,new_jval_i(1));
    }
  else
    {
      tree=make_jrb();
      tree=(JRB)jval_v(node->val);
      node2=jrb_find_str(tree,v1);
      if(node2==NULL)
	{
	  jrb_insert_str(tree,v1,new_jval_i(1));
	}
      node->val=new_jval_v(tree);
    }
  // addEdge(graph,v2,v1);
  //printf("Da add thanh cong\n");
}

int getAdjacentVerties_int(JRB graph,int v,int *output)//
{
  JRB node,tree;
  int total;
  node=jrb_find_int(graph,v);
  if(node==NULL)
    return 0;
  else
    {
      tree=(JRB) jval_v(node->val);
      total=0;
      jrb_traverse(node,tree)
	output[total++]=jval_i(node->key);
      return total;
    }
}
int getAdjacentVerties_str(JRB graph,char *v,char* output[])
{
  JRB node,tree;
  int total;
  node=jrb_find_str(graph,v);
  if(node==NULL)
    return 0;
  else
    {
      tree=(JRB) jval_v(node->val);
      total=0;
      jrb_traverse(node,tree)
	output[total++]=(char*)jval_s(node->key);
      return total;
    }
}
void dropGraph(JRB graph)
{
  JRB node;
  jrb_traverse(node, graph)
    jrb_free_tree(jval_v(node->val));
}
int numVertices(JRB g){ //đếm số lượng đỉnh của đồ thị

  JRB ptr;
  int n = 0;
  jrb_traverse(ptr, g) n++;
  return n;
}
void BFS_Connected(JRB graph,int start, int *id, int lt){
   int n, output[100], i, u, v;
   Dllist node, queue;
   
   queue = new_dllist();
   dll_append(queue, new_jval_i(start));

   while ( !dll_empty(queue) )
   {
      node = dll_first(queue);
      u = jval_i(node->val);
      dll_delete_node(node);
      if (id[u]==-1) 
      {
          id[u]=lt;           
          n = getAdjacentVerties_int(graph, u, output);
          for (i=0; i<n; i++)
          {
              v = output[i];
              if (id[v] == -1){
                 dll_append(queue, new_jval_i(v));
              }
          }
      }
   }
}
void getComponents(JRB graph){// Kiem tra tinh lien thong cua do thi vo huong
  int lt = 0;//luu tru so thanh phan lien thong cua do thi
  int id[1000];
  int n = numVertices(graph);
  printf("So phan tu cua do thi: %d\n",n);
  for(int i = 1;i<=n;i++) id[i]=-1;
  for(int i = 1;i<=n;i++){
    if(id[i] == -1){
      lt++;
      BFS_Connected(graph,i,id,lt);
    }
  }
  if(lt == 1)printf("Do thi vo huong da cho lien thong\n");
  else{
    printf("Do thi vo huong da cho khong lien thong\n");
    printf("So thanh phan lien thong cua do thi: %d\n",lt);
    for(int i = 1;i<=lt;i++){
      printf("Cac dinh thuoc thanh phan lien thong thu %d: ",i);
      for(int j = 1;j<=n;j++){
        if(id[j]==i)printf("%d",j);
      }
      printf("\n");
    }
  }
}
void BFS(JRB graph, int start, int stop, void (*func)(int),int *last)
{
   int visited[1000] = {};
   int n, output[100], i, u, v;
   Dllist node, queue;
   
   queue = new_dllist();
   dll_append(queue, new_jval_i(start));

   while ( !dll_empty(queue) )
   {
      node = dll_first(queue);
      u = jval_i(node->val);
      dll_delete_node(node);
      if (!visited[u]) 
      {
          func(u);
          visited[u] = 1;
          if ( u == stop ) return;           
          n = getAdjacentVerties_int(graph, u, output);
          for (i=0; i<n; i++)
          {
              v = output[i];
              if (!visited[v]){
                last[v]=u;
                 dll_append(queue, new_jval_i(v));
              }
          }
      }
   }                            
}



int DFS(JRB graph, int start, int stop, void (*func)(int),int* last)
{
   int visited[1000] = {};
   int n, output[100], i, u, v;
   Dllist node, stack;
   
   stack = new_dllist();
   dll_append(stack, new_jval_i(start));

   while ( !dll_empty(stack) )
   {
      node = dll_last(stack);
      u = jval_i(node->val);
      dll_delete_node(node);
      if (!visited[u]) 
      {
          func(u);
          visited[u] = 1;
          if ( u == stop ) return 1;           
          n = getAdjacentVerties_int(graph, u, output);
          for (i=0; i<n; i++)
          {
              v = output[i];
              if(v == start) return 0;//ham tra ve 0 neu phat hien ra chu trinh
              if (!visited[v]){
                last[v]=u;
                 dll_append(stack, new_jval_i(v));
              }
          }
      }
   }  
   return 1;//ham tra ve 1 neu khong phat hien ra chu trinh                          
}
int DFS_DAG(JRB graph, int start)//Kiểm tra chu trình
{
   int visited[1000] = {};
   int n, output[100], i, u, v;
   Dllist node, stack;
   
   stack = new_dllist();
   dll_append(stack, new_jval_i(start));

   while ( !dll_empty(stack) )
   {
      node = dll_last(stack);
      u = jval_i(node->val);
      dll_delete_node(node);
      if (!visited[u]) 
      {
          visited[u] = 1;           
          n = getAdjacentVerties_int(graph, u, output);
          for (i=0; i<n; i++)
          {
              v = output[i];
              if(v == start) return 0;//ham tra ve 0 neu phat hien ra chu trinh
              if (!visited[v]){
                 dll_append(stack, new_jval_i(v));
              }
          }
      }
   }  
   return 1;//ham tra ve 1 neu khong phat hien ra chu trinh                          
}
int DAG(JRB graph){//Khong co chu trinh return 1
  int start, notCycle;
  JRB vertex;

  jrb_traverse(vertex,graph){
    start = jval_i(vertex->key);
    printf("Goi DFS xuat phat tu dinh %d\n",start);
    notCycle = DFS_DAG(graph,start);
    if(notCycle == 0)return 0;//co chu trinh
  }
  return 1;//Khong co chu trinh
}

double getEdgeValue(JRB graph, int v1, int v2){//Lấy trọng số của cạnh v1,v2
  JRB node,tree;
  node = jrb_find_int(graph,v1);
  if(node == NULL){
    return INFINITY_VALUE;
  }
  tree = (JRB)jval_v(node->val);
  node = jrb_find_int(tree,v2);
  if(node == NULL){
    return INFINITY_VALUE;
  }else{
    return jval_d(node->val);
  }
}
void addEdge(JRB graph, int v1, int v2, double weight)//Thêm cạnh có trọng số
{
     JRB node, tree;
     if (getEdgeValue(graph, v1, v2)==INFINITY_VALUE)
     {
        node = jrb_find_int(graph, v1);
        if (node==NULL) {
           tree = make_jrb();
           jrb_insert_int(graph, v1, new_jval_v(tree));
        } else
        {
           tree = (JRB) jval_v(node->val);   
        }
        jrb_insert_int(tree, v2, new_jval_d(weight));
     }
}
int indegree (JRB graph, int v, int* output)//Bán bậc vào
{
    JRB tree, node;
    int total = 0;   
    jrb_traverse(node, graph)
    {
       tree = (JRB) jval_v(node->val);
       if (jrb_find_int(tree, v))
       {
          output[total] = jval_i(node->key);
          total++;
       }                
    }
    return total;   
}
int outdegree (JRB graph, int v, int* output)//bán bậc ra
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph, v);
    if (node==NULL)
       return 0;
    tree = (JRB) jval_v(node->val);
    total = 0;   
    jrb_traverse(node, tree)
    {
       output[total] = jval_i(node->key);
       total++;                
    }
    return total;   
}
node CreateNode(int value){
    node temp; // declare a node
    temp = (node)malloc(sizeof(struct LinkedList)); // Cấp phát vùng nhớ dùng malloc()
    temp->next = NULL;// Cho next trỏ tới NULL
    temp->data = value; // Gán giá trị cho Node
    return temp;//Trả về node mới đã có giá trị
}
 
node AddTail(node head, int value){
    node temp,p;// Khai báo 2 node tạm temp và p
    temp = CreateNode(value);//Gọi hàm createNode để khởi tạo node temp có next trỏ tới NULL và giá trị là value
    if(head == NULL){
        head = temp;     //Nếu linked list đang trống thì Node temp là head luôn
    }
    else{
        p  = head;// Khởi tạo p trỏ tới head
        while(p->next != NULL){
            p = p->next;//Duyệt danh sách liên kết đến cuối. Node cuối là node có next = NULL
        }
        p->next = temp;//Gán next của thằng cuối = temp. Khi đó temp sẽ là thằng cuối(temp->next = NULL mà)
    }
    return head;
}
 
 
node AddHead(node head, int value){
    node temp = CreateNode(value); // Khởi tạo node temp với data = value
    if(head == NULL){
        head = temp; // //Nếu linked list đang trống thì Node temp là head luôn
    }else{
        temp->next = head; // Trỏ next của temp = head hiện tại
        head = temp; // Đổi head hiện tại = temp(Vì temp bây giờ là head mới mà)
    }
    return head;
}
 
node AddAt(node head, int value, int position){
    if(position == 0 || head == NULL){
        head = AddHead(head, value); // Nếu vị trí chèn là 0, tức là thêm vào đầu
    }else{
        // Bắt đầu tìm vị trí cần chèn. Ta sẽ dùng k để đếm cho vị trí
        int k = 1;
        node p = head;
        while(p != NULL && k != position){
            p = p->next;
            ++k;
        }
 
        if(k != position){
            // Nếu duyệt hết danh sách lk rồi mà vẫn chưa đến vị trí cần chèn, ta sẽ mặc định chèn cuối
            // Nếu bạn không muốn chèn, hãy thông báo vị trí chèn không hợp lệ
            head = AddTail(head, value);
            // printf("Vi tri chen vuot qua vi tri cuoi cung!\n");
        }else{
            node temp = CreateNode(value);
            temp->next = p->next;
            p->next = temp;
        }
    }
    return head;
}
 
node DelHead(node head){
    if(head == NULL){
        printf("\nCha co gi de xoa het!");
    }else{
        head = head->next;
    }
    return head;
}
int Dijkstra(JRB g, int diemdau, int diemcuoi, int sodinh,int array[])
{
    double *d = (double *)malloc(sodinh * sizeof(double)); //khoang cach tu diem dau
    for(int i = 0; i < sodinh; i++)
    {
        d[i] = 100000;
    }
    d[diemdau] = 0;
    int *diemtruoc = (int *)malloc(sodinh * sizeof(int));
    int *check = (int *)calloc(sodinh, sizeof(int)); //visited
    node q = NULL;
    q = AddTail(q,diemdau);
    int u;
    int n;
    int output[1000];
    JRB tmp, tmp1, tmp2;
    while(q!=NULL)
    {
        u = q->data;
        q = DelHead(q);
        n = outdegree(g, u, output);
        tmp = jrb_find_int(g, u);
        if(check[u]==0)
        {
            for(int i = 0; i < n; i++)
            {
                q = AddTail(q,output[i]);
                tmp1 = (JRB)tmp->val.v;
                tmp2 = jrb_find_int(tmp1, output[i]);
                if(d[output[i]] == 1000)
                {
                    d[output[i]] = tmp2->val.d + d[u];
                    diemtruoc[output[i]] = u;
                }
                else if(((tmp2->val.d + d[u]) < d[output[i]])&&d[output[i]]!=1000)
                {
                    d[output[i]] = tmp2->val.d + d[u];
                    diemtruoc[output[i]] = u;
                }
  
            }
        }
        check[u] = 1;
    }
    int x = 0;
    int m = diemcuoi;
    while(m != diemdau)
    {
        array[x++] = m;
        m = diemtruoc[m];
    }
    array[x++] = diemdau;
    return x-1;
//inDuong(g, diemtruoc, diemdau, diemcuoi);
}// muon in ra man hinh for(i = edgeDijktra;i>=0;i--){
//printf("%s",bangKihieu[i].name v.v...);
//}
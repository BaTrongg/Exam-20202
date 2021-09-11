#ifndef	_JRB_H_
#define	_JRB_H_

#include "jval.h"
#include"dllist.h"
/** Main jrb_node.  You only ever use the fields
    flink
    blink
    k.key or k.ikey
    v.val
 */


typedef struct jrb_node {
  unsigned char red;
  unsigned char internal;
  unsigned char left;
  unsigned char roothead;  /**< Bit 1 is the root. Bit 2 is head. */
  struct jrb_node *flink;
  struct jrb_node *blink;
  struct jrb_node *parent;
  Jval key;
  Jval val;
} *JRB;
struct LinkedList{
    int data;
    struct LinkedList *next;
 };
 
typedef struct LinkedList *node; //Từ giờ dùng kiểu dữ liệu LinkedList có thể thay bằng node cho ngắn gọn

/** Creates a new rb-tree. */
extern JRB make_jrb();


/** Creates a node with key key and val val and inserts it into the tree.
 *  jrb_insert uses strcmp() as comparison funcion.  jrb_inserti uses <>=,
 *  jrb_insertg uses func()
 */

extern JRB jrb_insert_str(JRB tree, char *key, Jval val);
extern JRB jrb_insert_int(JRB tree, int ikey, Jval val);
extern JRB jrb_insert_dbl(JRB tree, double dkey, Jval val);
extern JRB jrb_insert_gen(JRB tree, Jval key, Jval val, int (*func)(Jval,Jval));

/* Returns an external node in t whose value is equal k. Returns NULL if
 * there is no such node in the tree.
 */

extern JRB jrb_find_str(JRB root, char *key);
extern JRB jrb_find_int(JRB root, int ikey);
extern JRB jrb_find_dbl(JRB root, double dkey);
extern JRB jrb_find_gen(JRB root, Jval, int (*func)(Jval, Jval));


/** Returns an external node in t whose value is equal
 * k or whose value is the smallest value greater than k. Sets found to
 * 1 if the key was found, and 0 otherwise.
 */

extern JRB jrb_find_gte_str(JRB root, char *key, int *found);
extern JRB jrb_find_gte_int(JRB root, int ikey, int *found);
extern JRB jrb_find_gte_dbl(JRB root, double dkey, int *found);
extern JRB jrb_find_gte_gen(JRB root, Jval key, 
                              int (*func)(Jval, Jval), int *found);


/** Creates a node with key key and val val and inserts it into the 
 *  tree before/after node nd.  Does not check to ensure that you are 
 *  keeping the correct order.
 */

/** Deletes and free(3) a node but not the key or val. */
extern void jrb_delete_node(JRB node);

/** Deletes and free(3) an entire tree. */
extern void jrb_free_tree(JRB root);

/** Returns node->v.val -- this is to shut lint up. */
extern Jval jrb_val(JRB node);

/** Returns # of black nodes in path from n to the root. */
extern int jrb_nblack(JRB n);

/** Returns # of nodes in path from n to the root. */
int jrb_plength(JRB n);

/** Convenience macros */
#define jrb_first(n) (n->flink)
#define jrb_last(n) (n->blink)
#define jrb_next(n) (n->flink)
#define jrb_prev(n) (n->blink)
#define jrb_empty(t) (t->flink == t)
#ifndef jrb_nil
#define jrb_nil(t) (t)
#endif
 
#define jrb_traverse(ptr, lst) \
  for(ptr = jrb_first(lst); ptr != jrb_nil(lst); ptr = jrb_next(ptr))
#define jrb_rtraverse(ptr, lst) \
  for(ptr = jrb_last(lst); ptr != jrb_nil(lst); ptr = jrb_prev(ptr))
  JRB createGraph();
  void addEdge_int(JRB graph, int v1,int v2);
  void addEdge_str(JRB graph, char* v1,char* v2);
  int getAdjacentVerties_int(JRB graph,int v,int *output);
  int getAdjacentVerties_str(JRB graph,char *v,char* output[]);
  void dropGraph(JRB graph);
  int numVertices(JRB g);
  void BFS_Connected(JRB graph,int start, int *id, int lt);
  void getComponents(JRB graph);
  void BFS(JRB graph, int start, int stop, void (*func)(int),int* last);
  int DFS(JRB graph, int start, int stop, void (*func)(int),int* last);
  int DFS_DAG(JRB graph, int start);
  int DAG(JRB graph);
  double getEdgeValue(JRB graph, int v1, int v2);
  void addEdge(JRB graph, int v1,int v2,double weight);
  int indegree (JRB graph, int v, int* output);
  int outdegree (JRB graph, int v, int* output);
  node CreateNode(int value);
  node AddTail(node head, int value);
  node AddHead(node head, int value);
  node AddAt(node head, int value, int position);
  node DelHead(node head);
  int Dijkstra(JRB g, int diemdau, int diemcuoi, int sodinh,int array[]);
#endif

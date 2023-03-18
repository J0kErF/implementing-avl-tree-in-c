#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

//mohammad yosef
//mohmd.sy2002@gmail.com
//https://www.linkedin.com/in/mohammad-yosef/

int getHeight(AVLNodePtr root);
int getSize(AVLNodePtr root);
int getSum(AVLNodePtr root);
void updateHeight(AVLNodePtr node);
void updateSize(AVLNodePtr node);
void updateSum(AVLNodePtr node);
AVLNodePtr minToRoot (AVLNodePtr root,AVLNodePtr parent, AVLNodePtr child);
void convertToArr(AVLNodePtr root, int arr[],int *i);
AVLNodePtr convertToAVL(int arr[],int s,int e);
AVLNodePtr mergeArr(int arr1[],int arr2[],int len1,int len2);
AVLNodePtr rightRotate(AVLNodePtr root);
AVLNodePtr leftRotate(AVLNodePtr root);
AVLNodePtr balance(AVLNodePtr root);
int route_R_sum(AVLNodePtr root, int x);
int route_L_sum(AVLNodePtr root, int x);


int util_max(int x, int y)
{
    return x > y ? x : y;
}
int util_min(int x, int y)
{
    return x < y ? x : y;
}
int util_abs(int x)
{
    return x > 0 ? x : -x;
}

// the 3 gets is to take care of the null statement to be 0
int getHeight(AVLNodePtr root)
{
    return !root ? -1 : root->height;
}
int getSize(AVLNodePtr root)
{
    return !root ? 0 : root->tree_size;
}
int getSum(AVLNodePtr root)
{
    return !root ? 0 : root->tree_sum;
}
// here we update the values with every edit on the tree - rotations , deletion , insertion ...
void updateHeight(AVLNodePtr node)
{
    if (node)
        node->height = util_max(getHeight(node->child[LEFT]), getHeight(node->child[RIGHT])) + 1;
}
void updateSize(AVLNodePtr node)
{
    if (node)
        node->tree_size = getSize(node->child[LEFT]) + getSize(node->child[RIGHT]) + 1;
}
void updateSum(AVLNodePtr node)
{
    if (node)
        node->tree_sum = getSum(node->child[LEFT]) + getSum(node->child[RIGHT]) + node->key;
}

/* replaces the root with the minimal node -- helps with the deletion*/
AVLNodePtr minToRoot (AVLNodePtr root,AVLNodePtr parent, AVLNodePtr child){
    AVLNodePtr tmp;
    if (child->child[LEFT]) {  /* continue until you reach the minimal node */
        return minToRoot (root, child, child->child[LEFT]);
    } else {
        tmp = (child->child[RIGHT]); /* the min node doesn't have left child then we only need to save the right child */
        if (parent == root) {    /* in case which the root of the sub-tree is the minimum */
            child->child[RIGHT] = root;
            child->child[LEFT] = root->child[LEFT];
        } else {    /* else*/
            parent->child[LEFT] = root;
            child->child[LEFT] = root->child[LEFT];
            child->child[RIGHT] = root->child[RIGHT];
        }
        root->child[LEFT] = NULL;
        root->child[RIGHT] = tmp;
        child->height = root->height;
        updateSum(root);
        updateHeight(root);
        updateSize(root);
        return child;
    }
}
/*here we convert the tree into array (inorder) -- helps with merge*/
void convertToArr(AVLNodePtr root, int arr[],int *i)
{
    if(!root) return;
    convertToArr(root->child[LEFT],arr,i);
    arr[(*i)]=root->key;
    (*i)++;
    convertToArr(root->child[RIGHT],arr,i);
}
/*here we convert the array into avl tree (preorder) -- helps with merge*/
AVLNodePtr convertToAVL(int arr[],int s,int e)
{
    if(s>e) return NULL;
    int m= (s+e)/2;
    AVLNodePtr root=new_avl_node(arr[m]);
    root->child[LEFT]=convertToAVL(arr,s,m-1);
    root->child[RIGHT]=convertToAVL(arr,m+1,e);
    updateHeight(root);
    updateSize(root);
    updateSum(root);

    return root;
}
// this function merge two arrays and return the avl tree of the merged arrays
AVLNodePtr mergeArr(int arr1[],int arr2[],int len1,int len2)
{
    int *arr3=(int*) malloc((len1+len2) * sizeof(int));
    int i=0,x=0,y=0;
    int real=0;//this counter is for the duplicates

    while (i < len1 && x < len2)
    {
        if (arr1[i] < arr2[x])
        {
            arr3[y] = arr1[i];
            i++;
        }
        else if (arr1[i] > arr2[x])
        {
            arr3[y] = arr2[x];
            x++;
        }
        else // taking care of the duplicates
        {
            arr3[y] = arr2[x];
            x++;
            i++;
            real++;
        }
        y++;
    }
    // here we take care of the extentions
    while (i < len1)
    {
        arr3[y] = arr1[i];
        i++; y++;
    }
    while (x < len2)
    {
        arr3[y] = arr2[x];
        x++; y++;
    }
    AVLNodePtr FINAL = convertToAVL(arr3,0,(len1+len2-real-1));
    free(arr3);
    return FINAL;
}
//here we rotate right
AVLNodePtr rightRotate(AVLNodePtr root)
{
    AVLNodePtr final = root->child[LEFT];
    root->child[LEFT] = final->child[RIGHT] ? final->child[RIGHT] : NULL;
    final->child[RIGHT] = root;
    if (final->child[LEFT])
    {
        final->child[LEFT]->height = ((util_max(getHeight(final->child[LEFT]->child[LEFT]), getHeight(final->child[LEFT]->child[RIGHT]))) + 1);
    }
    if (final->child[RIGHT])
    {
        final->child[RIGHT]->height = ((util_max(getHeight(final->child[RIGHT]->child[LEFT]), getHeight(final->child[RIGHT]->child[RIGHT]))) + 1);
    }
    if (final)
    {
        final->height = ((util_max(getHeight(final->child[LEFT]), getHeight(final->child[RIGHT]))) + 1);
    }
    updateSum(root->child[RIGHT]);
    updateSum(root);
    updateHeight(final->child[LEFT]); /*because we rotated the tree we have to update the nodes new heights*/
    updateHeight(final->child[RIGHT]);
    updateHeight(final);
    updateSize(final->child[RIGHT]); /*because we rotated the tree we have to update the nodes new size*/
    updateSize(final);
    return final;
}
//here we rotate left
AVLNodePtr leftRotate(AVLNodePtr root)
{
    AVLNodePtr final = root->child[RIGHT];
    root->child[RIGHT] = final->child[LEFT] ? final->child[LEFT] : NULL;
    final->child[LEFT] = root;
    if (final->child[LEFT])
    {
        final->child[LEFT]->height = ((util_max(getHeight(final->child[LEFT]->child[LEFT]), getHeight(final->child[LEFT]->child[RIGHT]))) + 1);
    }
    if (final->child[RIGHT])
    {
        final->child[RIGHT]->height = ((util_max(getHeight(final->child[RIGHT]->child[LEFT]), getHeight(final->child[RIGHT]->child[RIGHT]))) + 1);
    }
    if (final)
    {
        final->height = ((util_max(getHeight(final->child[LEFT]), getHeight(final->child[RIGHT]))) + 1);
    }
    updateSum(root->child[LEFT]);
    updateSum(root);
    updateHeight(final->child[LEFT]);/*because we rotated the tree we have to update the nodes new heights*/
    updateHeight(final->child[RIGHT]);
    updateHeight(final);
    updateSize(final->child[LEFT]);/*because we rotated the tree we have to update the nodes new size*/
    updateSize(final);
    return final;
}

// here we check which rotations we need and depends on that we rotate
AVLNodePtr balance(AVLNodePtr root)
{
    //right heavier than left
    if (getHeight(root->child[LEFT]) < getHeight(root->child[RIGHT]))
    {
        if (root->child[RIGHT] != NULL && getHeight(root->child[RIGHT]->child[LEFT]) > getHeight(root->child[RIGHT]->child[RIGHT]))
            root->child[RIGHT] = rightRotate(root->child[RIGHT]);
        return leftRotate(root);
    }
    else//left heavier than right
    {
        if (root->child[LEFT] != NULL && getHeight(root->child[LEFT]->child[LEFT]) < getHeight(root->child[LEFT]->child[RIGHT]))
            root->child[LEFT] = leftRotate(root->child[LEFT]);
        return rightRotate(root);
    }
}
//this 2 function calculate the sides of tree from x or to x depends on the side
int route_R_sum(AVLNodePtr root, int x)// here we calculate the right tree sum up to x
{
    if (!root)
        return 0;
    if (root->key < x)// in case we still search for x and we less than x we add the left sum and continue to the right
        return root->child[LEFT]?root->child[LEFT]->tree_sum + root->key+ route_R_sum(root->child[RIGHT], x):(root->key + route_R_sum(root->child[RIGHT], x));
    if (root->key == x)// in case we found the x we get the left sum and return
        return root->child[LEFT]?root->child[LEFT]->tree_sum + root->key:root->key;
    return route_R_sum(root->child[LEFT],x);
}
int route_L_sum(AVLNodePtr root, int x)
{
    if (!root)
        return 0;
    if (root->key > x)// in case we still search for x and we greater than x we add the right sum and continue to the left
        return root->child[RIGHT]?root->child[RIGHT]->tree_sum + root->key+ route_L_sum(root->child[LEFT], x):(root->key + route_L_sum(root->child[LEFT], x));
    if (root->key == x)// in case we found the x we get the right sum and return
        return root->child[RIGHT]?root->child[RIGHT]->tree_sum + root->key:root->key;
     return route_L_sum(root->child[RIGHT],x);
}



AVLNodePtr avl_search(AVLNodePtr root, int x)
{   // normal binary search
    if (!root)
        return root;
    if (root->key > x)
        return avl_search(root->child[LEFT], x);
    if (root->key < x)
        return avl_search(root->child[RIGHT], x);
    return root;
}

AVLNodePtr avl_insert(AVLNodePtr root, int x)
{
    if (!root)
        return new_avl_node(x); // creating new node when we find the right location or creating a new tree
    if (root->key > x)
        root->child[LEFT] = avl_insert(root->child[LEFT], x); // find the right place
    else if (root->key < x)
        root->child[RIGHT] = avl_insert(root->child[RIGHT], x); // find the right place
    else
        return root; // already exists -- no need to re balance the tree
    if (util_abs(getHeight(root->child[LEFT]) - getHeight(root->child[RIGHT])) > 1)
        root = balance(root); // balance the tree in case there is imbalance
    // WE NEED TO UPDATE THE HEIGHT SIZE AND SUM
    updateHeight(root);
    updateSize(root);
    updateSum(root);
    return root;
}

AVLNodePtr avl_delete( AVLNodePtr root, int x )
{
   AVLNodePtr tmp;
    if (!root){  /* x doesn't exist */
        return root;
    }
    else if(root->key > x){ /* if root key is greater than x */
        root->child[LEFT] = avl_delete(root->child[LEFT], x);
    }
    else if(root->key < x){ /* if root key is smaller than x */
        root->child[RIGHT] = avl_delete(root->child[RIGHT], x);
    }
    else if (root->key == x){
        if ((root->child[LEFT])&&(root->child[RIGHT])){ /* x-key-root has two children */
            root = minToRoot (root, root, root->child[RIGHT]);
            root->child[RIGHT] = avl_delete (root->child[RIGHT],x);
        }
        else {  /* either one or no children */
            tmp = root;
            root = root->child[LEFT] ? root->child[LEFT] : root->child[RIGHT];
            free(tmp);
            return root;
        }
    }
     if(abs(getHeight(root->child[LEFT]) - getHeight(root->child[RIGHT])) > 1){ /* if there is imbalance--> call balance */
        root = balance(root);
    }
    updateHeight(root);
    updateSize(root);
    updateSum(root);
    return root;
}

AVLNodePtr avl_merge(AVLNodePtr root1, AVLNodePtr root2)
{   // first we convert the two trees into arrays (sorted because it's converted based on inorder)
    if(!root1||!root2) return !root1?root2:root1;
    int i=0;
    int arr1[getSize(root1)];
    convertToArr(root1,arr1,&i);
    i=0;
    int arr2[getSize(root2)];
    convertToArr(root2,arr2,&i);
    return mergeArr(arr1,arr2,getSize(root1),getSize(root2));// we call the merge function

}

AVLNodePtr avl_LCA(AVLNodePtr root, int x, int y)
{
    if (!root)
        return root;
    if (root->key > x && root->key > y)// in case both the x and y less than the root that means the LCA is on the left
        return avl_LCA(root->child[LEFT], x, y);
    if (root->key < x && root->key < y)// in case both the x and y greater than the root that means the LCA is on the right
        return avl_LCA(root->child[RIGHT], x, y);
    return root;// we found the LCA here


}

int avl_sum(AVLNodePtr root, int x, int y)
{   // first we find the LCA of x and y and then we call the left route sum func and the right route sum func
    // and return the sum of both of them with the LCA root it self
    if (!root)
        return 0;
    root = avl_LCA(root, x, y);
    return root->key + route_R_sum(root->child[RIGHT], util_max(x, y)) + route_L_sum(root->child[LEFT], util_min(x, y));
}

AVLNodePtr new_avl_node(int x)
{
    AVLNodePtr node = (AVLNodePtr)malloc(sizeof(AVLNode));
    if (!node)
        return NULL;
    // fill with the default values.
    node->height = 0;
    node->key = x;
    node->child[LEFT] = NULL;
    node->child[RIGHT] = NULL;
    node->tree_size = 1;
    node->tree_sum = x;

    return node;
}

void avl_delete_all(AVLNodePtr root)
{   // free using post order
    if (!root) return;
    if (root->child[LEFT])avl_delete_all(root->child[LEFT]);
    if (root->child[RIGHT])avl_delete_all(root->child[RIGHT]);
    free (root);
    return;
}

//mohammad yosef
//mohmd.sy2002@gmail.com
//https://www.linkedin.com/in/mohammad-yosef/

# implementing-avl-tree-in-c
this code is about implementing the concept of avl tree using c

The code is fulled with comments very easy to understand

here are some of the functions that used to make it work:

getters and setters:

int getHeight(AVLNodePtr root);
int getSize(AVLNodePtr root);
int getSum(AVLNodePtr root);
void updateHeight(AVLNodePtr node);
void updateSize(AVLNodePtr node);
void updateSum(AVLNodePtr node);


replaces the root with the minimal node -- helps with the deletion:
 
AVLNodePtr minToRoot (AVLNodePtr root,AVLNodePtr parent, AVLNodePtr child);

converting array to avl tree and the opposite:

void convertToArr(AVLNodePtr root, int arr[],int *i);
AVLNodePtr convertToAVL(int arr[],int s,int e);

merging two arrays:

AVLNodePtr mergeArr(int arr1[],int arr2[],int len1,int len2);

for the balancing I used main function named balance and this function used the rightRotate and leftRotate functions:

AVLNodePtr rightRotate(AVLNodePtr root);
AVLNodePtr leftRotate(AVLNodePtr root);
AVLNodePtr balance(AVLNodePtr root);

guess what these two used for 😉:

int route_R_sum(AVLNodePtr root, int x);
int route_L_sum(AVLNodePtr root, int x);



//mohammad yosef
//mohmd.sy2002@gmail.com
//https://www.linkedin.com/in/mohammad-yosef/

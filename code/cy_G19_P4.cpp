#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>

typedef struct QueNode QueNode;
typedef struct TreeNode TreeNode;
struct TreeNode
{
    TreeNode *left,*right;
    int data;
    int symbol;
};
struct QueNode
{
    int data,symbol;
};

TreeNode *Root;
QueNode Que[201];
int QueLen,BaseQueLen,MaxDepth,TotalSymbol,ErrorFlag,TreeNodeNum;
int Weight[201],a[201],Count[201];


/*--------------Allocate Space---------------------------------*/
TreeNode *NewTreeNode(TreeNode *left,TreeNode *right,int data,int symbol);
/*--------------Allocate Space---------------------------------*/

/*--------------Operations About Tree--------------------------*/
int InsertTreeNode(TreeNode *node,int sym,char num[],int depth);
void FreeTree(TreeNode *node);
int TravelTree(TreeNode *node,int depth);
/*--------------Operations About Tree--------------------------*/

TreeNode *NewTreeNode(TreeNode *left,TreeNode *right,int data,int symbol)
{
    TreeNode *node=(TreeNode*)malloc(sizeof(TreeNode));
    node->left=node->right=NULL;
    node->data=data;
    node->symbol=symbol;
    TreeNodeNum++;
    return node;
}
//Allocate space with given elements

void Initialize()
{
    memset(Count,0,sizeof(Count));
    ErrorFlag=0;
    TreeNodeNum=0;
    TotalSymbol=64;
    Root=NewTreeNode(NULL,NULL,0,63);
    MaxDepth=0;
}
//Initialize the corresponding structures

void FreeTree(TreeNode *node)
{
    if(node==NULL) return;
    FreeTree(node->left);
    FreeTree(node->right);
    free(node);
}
//Free the space of the node of the tree after a query

void Clear()
{
    FreeTree(Root);
}
//Clear all the redundancy after a query

int TransferCharToInt(char c)
{
    if(c<='9'&&c>='0') return c-'0';    //char of '0'~'9' will be numbered 0~9
    if(c<='z'&&c>='a') return c-'a'+10; //char of 'a'~'z' will be numbered 10~35
    if(c<='Z'&&c>='A') return c-'A'+36; //char of 'A'~'Z' will be numbered 36~61
    if(c=='_') return 62;               //char of '_'     will be numbered 62
    return 0;
}
//Transfer a char to a corresponding int

int OverallInput(QueNode *Que,int &QueLen)
{
    int i,fre,n;
    char sym;
    scanf("%d",&n);
    for(i=1;i<=n;i++)
    {
        getchar();
        scanf("%c %d",&sym,&fre);
        Que[i].data=fre;
        Que[i].symbol=TransferCharToInt(sym);   //put the elements into Que
        Weight[Que[i].symbol]=fre;              //store the frequency to corresponding symbol
    }
    QueLen=n;
    scanf("%d",&n);
    getchar();
    return n;
}
//Get the frequency of each char and store the given elements into Que array

/*--------------------------------------------------------------------------*/
/*      This function will insert the given char sym into the tree, whose   */
/*  path is represented by string num.                                      */
/*      error happens when 0 has been returneed                             */
/*      node is current node that we touched                                */
/*      sym is the char that we should insert into this tree this time      */
/*      num represent the path of the char should be inserted               */
/*      depth is current depth of this tree.                                */
/*      aimdepth is the goal depth that we should target                    */
/*--------------------------------------------------------------------------*/
int InsertTreeNode(TreeNode *node,int sym,char num[],int depth,int aimdepth)
{
    int ret=0;
    if(TreeNodeNum>2*QueLen-1)
        //if the amount of nodes is larger than 2n-1, this tree must not be Huffman Tree
        ErrorFlag=1;
    if(ErrorFlag==1) return 0;
    if(depth==aimdepth)//if we hit the target, we'll put this sym in this node
    {
        if(node->symbol<63&&node->symbol>-1) return 0;
            //if this node was covered with a valuable sym, an error happens
        node->symbol=sym;
        node->data=Weight[sym];
        return 1;
            //sym has covered this node successfully
    }
    switch(num[depth])
    {
        case '0':   //we'll go to the left child of this node when meet '0'
            if(node->left==NULL)    //if the left child of this node is NULL, we'll create a new node
                node->left=NewTreeNode(NULL,NULL,0,-1);
            ret=InsertTreeNode(node->left,sym,num,depth+1,aimdepth);
            break;
        case '1':   //we'll go to the right child of this node when meet '1'
            if(node->right==NULL)   //if the right child of this node is NULL, we'll create a new node
                node->right=NewTreeNode(NULL,NULL,0,-1);
            ret=InsertTreeNode(node->right,sym,num,depth+1,aimdepth);
            break;
    }
    if(node->symbol==-1) node->symbol=TotalSymbol++;
        //if this node wasn't covered with a symbol, make a symbol and cover it
    return ret;
}

void GetCharAndString(int &sym,char *num,int &len)
{
    char ch;
    ch=getchar();
    sym=TransferCharToInt(ch);
    getchar();
    ch=getchar();
    len=0;
    while(ch=='0'||ch=='1')
    {
        num[len++]=ch;
        ch=getchar();
    }
    num[len]='\0';
}
//this function serve under the Input function,
//get the given sym,num and count the length of the num

int Input(TreeNode *Root,int CharNum)
{
    int sym;
    char num[2001];
    int len,i;
    for(i=0;i<CharNum;i++)
    {
        GetCharAndString(sym,num,len);
        if(ErrorFlag==0)    //if error has happened, we don't need to do anything
        {
            if(InsertTreeNode(Root,sym,num,0,len)==0) ErrorFlag=1;
            if(len>MaxDepth) MaxDepth=len;
        }
    }
    if(ErrorFlag==1) return 0;
    MaxDepth++;
    return 1;
}
//this function is used to get the input information
//and insert the given sym into the tree

void SwapChild(TreeNode *node)
{
    TreeNode *tmp=node->left;
    node->left=node->right;
    node->right=tmp;
}
//this function is used to swap the child of the given node

/*--------------------------------------------------------------------------*/
/*      This function is used to count the amount of nodes of the tree      */
/*  in each depths, judge the single child&prefix condition, and calculate  */
/*  data of the internal nodes.                                             */
/*      depth is current depth of this tree                                 */
/*--------------------------------------------------------------------------*/

int TravelTree(TreeNode *node,int depth)
{
    Count[depth]++;     //Count is used to calculate amount of nodes in each depths
    if(node->symbol<63) //judge prefix condition
    {
        if(node->left!=NULL || node->right!=NULL) return 0;
        return 1;
    }
    if(node->left!=NULL && node->right!=NULL)
    {
        if(TravelTree(node->left,depth+1)==0) return 0;
        if(TravelTree(node->right,depth+1)==0) return 0;
        node->data=node->left->data+node->right->data;
            //data of this node is the sum data of its child
        if(node->left->data > node->right->data) SwapChild(node);
            //Ensure the data of the left child is larger than the right child
        return 1;
    }
    if(node->left!=NULL || node->right!=NULL) return 0;
            //judge single child condition
    return 1;   //travel this tree successfully
}

void GetArray(TreeNode *node,int depth)
{
    if(node==NULL) return;
    a[Count[depth]--]=node->data;
    GetArray(node->left,depth+1);
    GetArray(node->right,depth+1);
}
//Put the treenode into the array

int JudgeTree(TreeNode *Root,int &QueLen)
{
    int i;
    if(TravelTree(Root,1)==0) return 0;
    //Travel this tree
    for(i=2;i<=MaxDepth;i++)
        Count[i]=Count[i-1]+Count[i];
    //get the prefix sum of Count to get the array
    GetArray(Root,1);
    for(i=1;i<QueLen*2-1;i++)
        if(a[i]<a[i+1]) return 0;
    //judge the array if the data is increasing or equal
    return 1;
}

int main()
{
//	char sin[]="data1.in";
//	char sout[]="data1.out";
//	for(int i=1;i<=8;i++)
//	{
//		sin[4]='0'+i;
//		sout[4]='0'+i;
//		freopen(sin,"r",stdin);
//		freopen(sout,"w",stdout);
        int rep;
        rep=OverallInput(Que,QueLen);
        for(;rep>0;rep--)
        {
            Initialize();               //initialize all the elements
            if(Input(Root,QueLen)==1)   //work with the algorithm
            {
                if(JudgeTree(Root,QueLen)==1)
                    printf("Yes\n");
                else printf("No\n");
            } else printf("No\n");
            Clear();                    //clear the redundant elements
        }
//	}
    return 0;
}


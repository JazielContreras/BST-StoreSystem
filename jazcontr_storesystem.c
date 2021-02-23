#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct Node {
  int id;
  char name[50];
  float  balance;
  struct Node *left;
  struct Node *right;
};

typedef struct Node tree_node;
//root is global so it can be accessed anywhere
tree_node *root = NULL;

//creates a node with an id, name and balance
tree_node* add(int num, char name[],float balance) {
  tree_node *temp = (tree_node *)malloc(sizeof(tree_node));
  strcpy(temp->name,name);
  temp->id = num;
  temp->balance = balance;
  temp->left = temp->right = NULL;
  return temp;
}

//inserts nodes in the tree
tree_node* insert(tree_node* tree, tree_node *newNode, int flag) {
 
  if(tree == NULL) {
    return newNode;
  }
  //if the tree's node and the newly made node have the same id
  //then that means that the user input an id that already exists
  else if(tree->id == newNode->id && flag != 0) {
    printf("ID %d already exists in the tree. Please retry your entry.\n",newNode->id);
  }
  //if the tree's node is less than the new node's then move right 
  else if(tree->id < newNode->id && tree->id != newNode->id) {
    tree->right = insert(tree->right, newNode,flag);
  }
  //if the tree's node is greater than the new node's then move left
  else if(tree->id > newNode->id && tree->id != newNode->id) {
    tree->left = insert(tree->left, newNode,flag);
  }
  return tree;
}


void readfromFile() {
  FILE *fp;
  fp = fopen("customers.txt","r");
  char strfinal[30];
  char str[200];
  char fname[50];
  int id;
  float sbalance;
  int i = 0;

  while(fgets(str,200,fp)) {
    char fstr[100][250];
    int i, counter = 0,j = 0;
    for(i = 0; str[i] != '\0';i++) {
        //gets rid of unnecessary characters
        if((str[i] == '{'  || str[i] == '\n' || str[i] == '\"')) {
            continue;
        }
        //comma and close curly bracket act as delimiters
        else if(str[i] == ','|| str[i] == '}') {
            //null character is added to the end of the string
            fstr[counter][j] = '\0';
            //counter is incremented to change word
            counter++;
            i++;
            j = 0;
            continue;
        }

        else {
            //assigns necessary characters to fstr
            fstr[counter][j] = str[i];
            j++;
        }
    }

    for(j = 0; j < counter; j++) {
      if(j == 0) {
        id = atoi(fstr[0]);
      }
      if(j == 1) {
        strcpy(fname,fstr[1]);
      }
      if(j == 2) {
        sbalance = atof(fstr[2]);
      }
    }

    int flag;
   if(root == NULL){
     flag = 0;
      root = add(id,fname,sbalance);
      insert(root, root,flag);
    }
    else{
      flag = 1;
      tree_node* Node = add(id,fname,sbalance);
      insert(root, Node,flag);
    }
  }
}

//creates a customer from user input
void addCustomer() {
  int id;
  char name[25];
  float balance;
  int flag;
  scanf(" %d %s %f",&id,name,&balance);
  //if the root is null then that means the tree has not
  //been created yet
  if(root == NULL){
    flag = 0;
    root = add(id,name,balance);
    insert(root, root,flag);
    }
    //otherwise the tree has been created
  else{
    flag = 1;
    tree_node* Node = add(id,name,balance);
    insert(root, Node,flag);
  }
}

//finds the minimum value in the tree to make it the root
//crt stands for current
tree_node *minValue(tree_node *tree) {
  tree_node *crt = tree;
  //goes through the tree until it reaches the node with smallest id
  while(crt->left != NULL) {
    crt = crt->left;
  }
  return crt;
}



tree_node* deleteNode(tree_node *tree, int id) {
  //if user tries to delete something that isn't in the tree then this error message will pop up.
  if(tree == NULL) {
    printf("ID was not found in the tree.\n");
    return tree;
  }
  //if the tree id is greater than the id then go left
  else if(tree->id > id) {
    tree->left = deleteNode(tree->left,id);
  }
  //go right if id is greater than tree id
  else if(tree->id < id) {
    tree->right = deleteNode(tree->right,id);
  }
  //if they are equal or any other case will go in here
  else {
      //if both children are NULL then return NULL
      if(tree->left == NULL && tree->right == NULL) {
        tree = NULL;
        return tree;
      }
      //if the left child is NULL then free the current position and return the right child
       else if(tree->left == NULL) {
        tree_node *temp = tree->right;
        free(tree);
        return temp;
      }
      //frees current position and is replaced with left child
      else if(tree->right == NULL) {
        tree_node *temp = tree->left;
        free(tree);
        return temp;
      }
      //if a root is deleted then it will go in here
      else {
        //temp is the node with smallest value in the tree
        //reassigns the root with the node pointed to by temp
        tree_node *temp = minValue(tree->right);
        tree->id = temp->id;
        strcpy(tree->name,temp->name);
        tree->balance = temp->balance;
        tree->right = deleteNode(tree->right, temp->id);
      }
    }
    return tree;
}

void deleteCustomer() {
  int id;
  scanf(" %d",&id);
  root = deleteNode(root,id);
}

//prints tree in in-order in file
void fprint_inorder(tree_node * tree, FILE *fp) {
 if(tree != NULL) {
    fprint_inorder(tree->left,fp);
     fprintf(fp,"{%04d, \"%s\", %.2f}\n",tree->id, tree->name,tree->balance);
    fprint_inorder(tree->right,fp);
    }
}

//prints tree in post=order in file
void fprint_postorder(tree_node * tree,FILE *fp) {
  if(tree != NULL) {
    fprint_postorder(tree->left,fp);
    fprint_postorder(tree->right,fp);
    fprintf(fp,"{%04d, \"%s\", %.2f}\n",tree->id,tree->name,tree->balance);
  }
}
//prints the tree in pre-order in file
void fprint_preorder(tree_node * tree, FILE *fp) {
  if(tree != NULL) {
    fprintf(fp,"{%04d, \"%s\", %.2f}\n",tree->id,tree->name,tree->balance);
    fprint_preorder(tree->left,fp);
    fprint_preorder(tree->right,fp);
  }
}

//will quit the menu and print to the file based on user input
void quitFunction() {
  int num = 1;
  char str[10];
  
  FILE *fp;
  fp = fopen("newcustomers.txt","w+");
  while(num != 0) {
    printf("output> Please enter output format: pre, post or in order...\n");
    printf("input> ");
    scanf("%s",str);
    if(strcmp(str,"in") == 0) {
      fprint_inorder(root,fp);
      num = 0;
    }
    else if(strcmp(str,"pre") == 0) {
      fprint_preorder(root,fp);
      num = 0;
    }
    else if(strcmp(str,"post") == 0) {
      fprint_postorder(root,fp);
      num = 0;
    }
    else {
      printf("Please enter a valid output format.\n");
      num = 1;
    }
  }
  printf("output> writing to newcustomers.txt in %s-order, shutting down...\n",str);
}

//helps process find the node with the same id as the id that is given in orders.txt
tree_node* processHelper(tree_node *tree, int id, float balance) {
if(tree == NULL) {
  printf("ID %d was not found in tree.\n",id);
  return tree;
}
 if(tree->id == id) {
    float custbalance = tree->balance;
    tree->balance = tree->balance - balance;
    if(tree->balance < 0) {
      tree->balance = 0;
    }
    printf("Customer %04d, %s, payed %.2f from their store balance and %.2f at the register, %.2f remaining store balance\n",tree->id,tree->name, custbalance, balance,tree->balance);
    return tree;
  }
  //if the id is less than tree id then continue left
  else if(tree->id > id) {
    tree->left = processHelper(tree->left,id,balance);
  }
  //continue right if id is greater than tree id
  else if(tree->id < id) {
    tree->right = processHelper(tree->right,id,balance);
  }
  return tree;
}

//parses string in orders.txt function and passes it to processHelper
void process(char filename[]) {
  char strfinal[30];
  char str[200];
  char fname[50];
  int id;
  float sbalance;
  int i = 0;
FILE *file;
file = fopen(filename,"r");
printf("output> \n");
  while(fgets(str,200,file)) {
    char fstr[100][250];
    int i, counter = 0,j = 0;
    //loops until character at i encounters '\0'
    for(i = 0; str[i] != '\0';i++) {
        //gets rid of unnecessary characters

        if((str[i] == '{'  || str[i] == '\n' || str[i] == '\"')) {
            continue;
        }
        //comma acts as delimiter
        else if(str[i] == ','|| str[i] == '}') {
            //null character is added to the end of the string
            fstr[counter][j] = '\0';
            //counter is incremented to change word
            counter++;
            i++;
            j = 0;
            continue;
        }

        else {
            //assigns necessary characters to fstr
            fstr[counter][j] = str[i];
            j++;
        }
    }

    for(j = 0; j < counter; j++) {
      if(j == 0) {
        id = atoi(fstr[0]);
      }
      if(j == 1) {
        sbalance = atof(fstr[1]);
      }
    }
    root = processHelper(root,id,sbalance);
  }
}

//prints the menu
void printMenu() {
  char str[20];
  int count = 1;
  char filename[40];
  while(count != 0) {
    printf("1: add\n");
    printf("2: delete\n");
    printf("3: process\n");
    printf("4: quit\n");
    printf("input> ");
    scanf("%s",str);
    if(strcmp(str,"add") == 0 || strcmp(str,"1") == 0){
      addCustomer();
      continue;
    }

    else if(strcmp(str,"delete") == 0 || strcmp(str,"2") == 0) {
      deleteCustomer();
      continue;
    }

    else if(strcmp(str,"process") == 0 || strcmp(str,"3") == 0) {
      scanf(" %s",filename);
      process(filename);
      continue;
    }

    else if(strcmp(str,"quit") == 0 || strcmp(str,"4") == 0) {
      count = 0;
      quitFunction();
      continue;
    }

    else {
      printf("Please enter a valid menu option.\n");
    }
  }
}


int main() {
  //reads the customers.txt file and creates the tree
  readfromFile();
  printMenu();
  return 0;
}

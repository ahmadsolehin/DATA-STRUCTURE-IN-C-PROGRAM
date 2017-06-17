#include<stdio.h>
#include<time.h>
#include <stdlib.h>
#include <windows.h>//Used in making gotoxy(int, int) function

char *tarikhbaru[25];
void rectangle(int x,int y,int l,int b);// To make a rectangle starting from (x,y) with length l and breadth b
void gotoxy(int x,int y); //To move the cursor to x column and y row of output screen
COORD coord = {0, 0};

struct staffinfo    // class of staffinfo
{
    int id, duration;       //attribute/field
    int timein, timeout;
    char name[30];
    char ic[30];
    char status[100];
    struct staffinfo *left,*right;
};
struct staffinfo *rootptr,*newptr,*currentptr,*previousptr,*tmpptr;

time_t curtime;
struct tm *loctime;

void gotoxy (int x, int y)
{
    coord.X = x;
    coord.Y = y; // X and Y coordinates
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{
    int tahun, bulan, hari, tarikh;
    tahun = year()*10000;
    bulan = month()*100;
    hari = day();
    tarikh = tahun+bulan+hari;

    sprintf(tarikhbaru,"%d",tarikh);

    int ch;
    do{
        system("cls");

        rectangle(8,6,70,20);
        gotoxy(31,8);  printf("Main menu");
        gotoxy(31,9);  printf("\---------");
        gotoxy(10,11); printf("1 - Admin Login");
        gotoxy(10,13); printf("2 - Staff Login");
        gotoxy(10,15); printf("Esc - Exit");
        gotoxy(10,17); printf("Enter choice: ");
        ch = getche();

		switch(ch)
		{
		    case '1':
                system("cls");
		        adminLogin();
                break;
		    case '2':
                staffLogin();
                break;
		    case 27: //27 is ASCII code of escape key, means program exits when user presses Esc key instead of 1 or 2
		        printf("\n\n\n\n");
		        exit(0);
                break;
		}
    }while(1);//infinite loop incase any key other than Esc, 1, or 2 is pressed.

    return 0;
}

void staffLogin()
{
    system("cls");
    int id;
    rectangle(0,0,80,23);
    gotoxy(27,2);printf("Please enter detail..");
    gotoxy(10,6);printf("Enter staff id : ");
    scanf(" %d", &id);

    tmpptr = SEARCH(rootptr,id);
    if(tmpptr != NULL)
    {
        //printf("%d",tmpptr->timein);
        UPDATE(id); //go to update function
    }
    else
        INSERT(id); //go to insert function

    printf("\n\nAll data is saved..\n");
}

void adminLogin() //user will give attempt only 3 times
{
    char user_name[10] = "admin"; //set username and password
    char password[10] = "admin";
    char name[10],passcode[10];
    int a,b,i=3;

    printf("\n\n-----\nLogin\n-----\n");
    do{
        printf("\nUsername : ");
        scanf(" %s", &name);
        printf("Password : ");
        scanf(" %s", &passcode);

        a = strcmp(user_name,name);    //string compare function.. to compare username and password
        b = strcmp(password,passcode);
        i--;
    }while(a!=0 && b!=0 && i>0);

    if(a==0 && b==0)
    {
        loginsuccess();
        main_menu();         //go to main menu function
    }

    if(i<=0) //if more than 3 times,the user does not have permission to access
    {
        printf("\nAccess Denied!!\n");
        system("pause");    //system will pause
        system("cls");  //clear screen
        return 0;
    }
}

void main_menu()
{
    int id,element;
    char ch;

    do{
        printf("\n\n\n");
        /* Get the current time. */
        curtime = time (NULL);

        /* Convert it to local time representation. */
        loctime = localtime (&curtime);
        /* Print out the date and time in the standard format. */
        fputs (asctime (loctime), stdout);

        printf("\n\n\t\tSTAFF ATTENDANCE SYSTEM");

		printf("\n\nl - List all staff");
		printf("\nd - Delete a staff");
        printf("\ns - Search a staff");
        printf("\np - Print staff data");
		printf("\nx - Exit");
		printf("\n\nEnter choice: ");
		ch = getche();
		ch = tolower(ch);  //Converts input to its lowercase if input is an uppercase letter and has a lowercase.

		switch(ch)
        {
            case 'p':
                printer(rootptr);
                printf("\n\nAll data has been printed..\n");
                system("pause");
                system("cls");
                main_menu();
                break;

            case 'l':
                if(rootptr==NULL)
                {
                    printf("\n\nEmpty list..\n");
                    system("pause");
                    system("cls");
                    break;
                }
                else
                {
                    printf("\n\t\t\tAll Staff Data..\n");
                    printf("\nId\t\tIc\tTime in    Time out     Duration     Status\tName");
                    printf("\n-------------------------------------------------------------------------------------");
                    display(rootptr);
                    printf("\n\n");
                    system("pause");
                    system("cls");
                    break;
                }
            case 'd':
                printf("\nEnter a staff id to delete : ");
                scanf(" %d", &element);
                RemoveStaff(element);
                break;
		case 's':
            printf("\nEnter the searching staff id : ");
            scanf("%d", &element);

            tmpptr = SEARCH(rootptr,element);
            if(tmpptr!= NULL)
            {
                printf("\nThe staff id %d is found..\n\n",tmpptr->id);
                printf("Name\t    Ic\t\tTime in\t Time out   Duration\tStatus\n");
                printf("%s\t%s\t  %d\t  %d\t      %d\t   %s\n", tmpptr->name,tmpptr->ic,tmpptr->timein,tmpptr->timeout,tmpptr->duration,tmpptr->status);
            }
            else
                printf("The staff id %d is not found..\n", element);

            system("pause");
            system("cls");
            break;
		case 'x':
		    printf("\nExit system..\n");
		    break;

		default:
            printf("\nEnter only one from the above\n");
            getch();
            system("pause");
            system("cls");
		}
	}while (ch!='x'); //system will exit

   	getch();
	return;
}

void INSERT(int number)
{
    newptr = (struct staffinfo*) malloc (sizeof(struct staffinfo));
    newptr->left = NULL;
    newptr->right = NULL;
    newptr->id = number;

    rectangle(0,0,80,23);
    gotoxy(10,8);printf("Enter staff name : ");
    scanf(" %s", &newptr->name);
    gotoxy(10,10);printf("Enter staff ic : ");
    scanf(" %s", &newptr->ic);

    int jam=(hour()*100)+minute();
    newptr->timein=jam;
    newptr->timeout=jam;
    newptr->duration = newptr->timeout - newptr->timein;
    if(newptr->duration < 900)
    {
        strcpy(newptr->status,"Bad.."); //strcpy to give value to status
    }
    else
    {
        strcpy(newptr->status,"Good..");
    }

    currentptr =rootptr;
    while (currentptr!=NULL) //search the location to insert
    {
        previousptr = currentptr;
        if(number > currentptr->id)
            currentptr = currentptr->right;
        else
            currentptr = currentptr->left;
    }
    if (rootptr==NULL)
        rootptr = newptr;
    else if(number > previousptr->id)
        previousptr->right = newptr;
    else
        previousptr->left = newptr;
}

void UPDATE(int number)
{
    rectangle(0,0,80,23);

    int jam=(hour()*100)+minute();
    tmpptr->timeout=jam;
    tmpptr->duration = tmpptr->timeout - tmpptr->timein;

    if(tmpptr->duration < 900)
    {
        strcpy(tmpptr->status,"Bad.."); //strcpy to give value to status
    }
    else
    {
        strcpy(tmpptr->status,"Good..");
    }
}

void display(struct staffinfo *node)
{
    if (node!=NULL)
    {
        display(node->left);
        printf("\n\n%d",node->id);
        printf("\t %s",node->ic);
        printf("\t  %d",node->timein);
        printf("\t    %d",node->timeout);
        printf("\t%d",node->duration);
        printf("\t   %s",node->status);
        printf("\t%s",node->name);
        display(node->right);
    }
}

SEARCH(struct staffinfo *node, int number)
{
    if(node!=NULL)// node is not null
    {
    	if(number == node->id)
            return node;
        else if(number > node->id)
            return SEARCH(node->right,number);
        else
            return SEARCH(node->left,number);
    }
    else
        return NULL;
}

COPY (struct staffinfo *node)
{
    struct staffinfo *previousptr2;
    tmpptr=node;

    if(node->right==NULL)//no right node
        node = node->left;
    else if (node->left==NULL)//no left node
        node = node->right;
    else
        {
        tmpptr = node->left;
        previousptr2 =node;
      	while (tmpptr->right!=NULL) //find the rightmost node of the left subtree
      	{
         	previousptr2 = tmpptr;
         	tmpptr = tmpptr->right;
      	}
        node->id = tmpptr->id;//copy the element to node

        if (previousptr2 == node)
            previousptr2->left = tmpptr->left;
        else
            previousptr2->right = tmpptr->left;
    }

    free(tmpptr);
    printf("Data has been deleted\n");
    return node;
}

void RemoveStaff(int number)
{
    currentptr = rootptr;
    while (currentptr!=NULL)//search the node to delete
    {
        if (currentptr->id == number)
            break;

        previousptr = currentptr;//previousptr hold the address of node to delete
        if (number > currentptr->id)
            currentptr = currentptr->right;
        else
            currentptr = currentptr->left;
    }

    if (currentptr==NULL)
    {
        printf ("\nStaff id %d is not in the list!!!\n",number);
        system("pause");
        system("cls");
        return;
    }
    else if(currentptr->id == number)
    {
        if (currentptr == rootptr)
            rootptr=COPY(rootptr);
      	else if(previousptr->left == currentptr)
            previousptr->left=COPY(previousptr->left);
      	else
            previousptr->right=COPY(previousptr->right);
    }
    system("pause");
    system("cls");
}

void printer(struct staffinfo *node)
{
    char *filename;
    filename = tarikhbaru;

    FILE *file = fopen(filename,"w");

    /* Get the current time. */
    curtime = time (NULL);

    /* Convert it to local time representation. */
    loctime = localtime (&curtime);
    /* Print out the date and time in the standard format. */
    fprintf(file,asctime (loctime), stdout);

    fprintf(file,"\n\nAll Staff Data..");
    fprintf(file,"\nId\t\tIc\tTime in    Time out     Duration     Status\tName");
    if (node!=NULL)
    {

        display(node->left);
        fprintf(file,"\n\n%d",node->id);
        fprintf(file,"\t %s",node->ic);
        fprintf(file,"\t  %d",node->timein);
        fprintf(file,"\t    %d",node->timeout);
        fprintf(file,"\t%d",node->duration);
        fprintf(file,"\t   %s",node->status);
        fprintf(file,"\t%s",node->name);
        display(node->right);

    }

    fclose(file);
}

void rectangle(int x,int y,int l,int b)
{
    int i,m;
    gotoxy(x,y); printf("%c",201);
    for(i=x+1;i<l-1;i++)
    {
        gotoxy(i,y);
        printf("%c",205);
    }
    gotoxy(i,y); printf("%c",187);

    for (m=y+1;m<b;m++)
    {
        gotoxy(x,m);
        for(i=x;i<l;i++)
        {
            if(i==x||i==l-1)
            {
                gotoxy(i,m); printf("%c",186);
            }
        }
    }

    gotoxy(x,m); printf("%c",200);
    for(i=x+1;i<l-1;i++)
    {
        gotoxy(i,m);
        printf("%c",205);
    }
    gotoxy(i,m); printf("%c",188);
}

int day()
{
	SYSTEMTIME str_t;
	GetSystemTime(&str_t);

    return(str_t.wDay);
}

int month()
{
	SYSTEMTIME str_t;
	GetSystemTime(&str_t);

    return(str_t.wMonth);
}

int year()
{
	SYSTEMTIME str_t;
	GetSystemTime(&str_t);

    return(str_t.wYear);
}

int hour()
{
	SYSTEMTIME str_t;
	GetSystemTime(&str_t);

	return(str_t.wHour+8);
}

int minute()
{
	SYSTEMTIME str_t;
	GetSystemTime(&str_t);

	return(str_t.wMinute);
}

void wait(int sec)
{
    clock_t end_wait;
    end_wait = clock() + sec * CLK_TCK ;

    while (clock() < end_wait) {}
}

void loginsuccess()
{
    system("cls");

    gotoxy(3,2); printf("20%% Loading.");
    wait(1);	/* Wait for 1 seconds */
    system("cls");

    gotoxy(3,2); printf ("40%% Loading. .");
    wait(1);
    system("cls");

    gotoxy(3,2); printf ("60%% Loading. . .");
    wait(1);
    system("cls");

    gotoxy(3,2); printf ("80%% Loading. . . .");
    wait(1);
    system("cls");

    gotoxy(3,2); printf ("100%% Loading. . . . .");
    wait(1);
    system("cls");
}

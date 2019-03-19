#include<iostream>
#include<time.h>
#include<fstream>
#include<stdlib.h>
#include<math.h>
#include<string.h>

using namespace std;

int sud[9][9], in[9][9],xen,yen,zen,a,b,score=0,config[35],tot;
char x;
time_t beg,eng;
float tim;

void disfull(int sud[][9]);
bool submit(int in[9][9]);  //crosschecks the final submitted array with sud
void ass(); //assigning to  in[][]
void clrscr();
void input();   //taking in values of the position and the no. to be assigned
bool lsearch(int in, int sud[9][9], int i, int j, int opt);
bool sqcheck(int sud[][9],int i,int j,int z);
void con(int config[35]);   //gets clue configuration
void affix(int sud[][9]);   //makes fixed numbers negative
void display(int sud[][9]); // displays only clues(negative numbers)
int generator();
void help();
int decorate();
bool simser(int* a, int in);
void sigma(int &in1, int &in2);
void crecon();
void profiles();

class prof
{
public:
    char name[21];
    int game;
    int serial;
    int coun;
}iles;

class prev
{
    public:
    int x, y, in;

    void undo()
    {
        sud[x][y]=0;
    }

    void redo()
    {
        sud[x][y]=-in;
    }
}p;


void profiles()
{
    int choice;
    char ms;
    prof temp;

    fstream a;
    a.open("progress.dat",ios::binary|ios::in|ios::app);
    fstream b;
    b.open("tutu.dat",ios::binary|ios::in|ios::app);

    if(!a&&!b)
        cout<<"error";
    else
    {
        cout<<"\nAre you an existing user?\n";
        cin>>ms;
        if(ms=='y')
        {

            cout<<"S No.\tName\n\n";
            while(!a.eof())
            {
                a.read((char*)&temp,sizeof(prof));
                if(a.eof())
                break;
                cout<<temp.serial<<'\t'<<temp.name<<'\n';
            }

            cout<<"Choose your S No.";
            cin>>choice;
            a.seekg(choice*sizeof(prof),ios::beg);

            a.read((char*)&iles,sizeof(prof));
            iles.game+=score;

            b.write((char*)&iles,sizeof(iles));
            remove("progress.dat");
            rename("tutu.dat","progress.dat");

            cout<<"Your progress has been saved.";


        }

        else if(ms=='n')
        {
            cout<<"\nEnter your name\n";
            cin.ignore();
            cin.getline(iles.name,21);

            a.seekg(0,ios::end);
            if(a.tellg()==0)
                {
                 cout<<"new";
                 temp.serial=0;
                }

            else
            {
                a.seekg(-(int)(sizeof(temp)),ios::end);
                a.read((char*)&temp,sizeof(temp));
            }

            iles.serial=temp.serial+1;
            iles.game+=score;


            a.seekg(ios::end);
            a.write((char*)&iles,sizeof(prof));

            cout<<"Your progress has been saved.";

        }
        a.close();
        b.close();
    }

}

void displayprofile()
{   cout<<"\nS.NO   NAME";
    prof temp1;
    fstream a;
    a.open("progress.dat",ios::binary|ios::in|ios::app);
    if(!a)
       {cout<<"error";
        exit(0);
       }
    while(!a.eof())
    {
       a.read((char*)&temp1,sizeof(prof));
       if(a.eof())
        break;
       cout<<"\n"<<temp1.serial<<"."<<'\t'<<temp1.name<<'\t'<<"            SCORE:"<<temp1.game;
    }
    a.close();
}



bool simser(int* a, int in)
{
    for(int i=0;i<sizeof(a)/sizeof(int);i++)
        if(a[i]==in)
            return true;
    return false;
}

void sigma(int &in1, int &in2)
{
    int sum1=0, sum2=0, maz=rand()%3 + 3;

    again:
    for(int i=0;i<35;i++)
        {
            if(config[i]/10==in1)
                    sum1++;

            if(config[i]%10==in2)
                    sum2++;
        }

    if(sum1>=maz||sum2>=maz||simser(config,(10*in1 + in2)))
        {
            if(simser(config,(10*in1 + in2)))
                {
                    in2++;
                    if(in2==10)
                        in2=1;
                }

            if(sum1>=maz)
                {
                    in1++;
                    if(in1==10)
                        in1=1;
                }

            if(sum2>=maz)
                {
                    in2++;
                    if(in2==10)
                        in2=1;
                }

            sum1=0;
            sum2=0;
            goto again;
        }
}

void crecon()
{
    int x,y;
    srand(time(NULL));
    tot=rand()%6+27;

    for(int i=0;i<tot;i++)
    {   int k,j;
       start: x=rand()%9+1;
        y=rand()%9+1;
        k=10*x + y;
       {

        for(j=0;config[j]!=0;j++)
        if(config[j]==k)
         {
             goto start;
          }
        }
        sigma(x,y);

        config[i]=k;
    }

}

bool submit(int in[9][9])
{
    for(int i=0;i<9;i++)
       { for(int j=0;j<9;j++)
           { if(in[i][j]!=sud[i][j])
                return false;
           }
	}
    return true;
}

void ass()
{
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)
        {
            if(sud[i][j]<0)
                in[i][j]=abs(sud[i][j]);
            else
                in[i][j]=0;
        }
}


void clrscr()
{
    for(int i=0;i<5;i++)
        cout<<"\n\n\n\n\n\n\n\n\n\n";
}

void input()
{
    while(!(x=='$'||x=='g'))
    {
        clrscr();
        display(in);
        time(&beg);

        cout<<"\nType in the number you want to enter, and enter its coordinates\n";
        cin>>x;

        if(!isdigit(x))
        {
            clrscr();
            switch(x)
            {
            case 'u':
                p.undo();
                break;

            case 'r':
                p.redo();
                break;

            case 'g':
                time(&eng);
                tim=difftime(eng,beg);
                affix(sud);
                disfull(sud);
                cout<<"\nyou took "<<tim/60<<" minutes to give up";
                break;
            case '$':
                if(submit(in))
                    {score=100;
                     time(&eng);
                     tim=difftime(eng,beg);
                     cout<<"CONGRATULATIONS! You solved it in "<<tim/60<<" minutes";
                    }

                else
                    cout<<"\nThe solution submitted is incorrect. \nThank you for playing.\n";

                break;
            default:
                cout<<"Invalid character!";
            }

        }

        else
        {
            zen=x-'0';
            clrscr();
            display(in);
            cout<<zen<<"-->(";
            cin>>xen;
            clrscr();
            display(in);
            cout<<zen<<"-->("<<xen<<",";
            cin>>yen;
            clrscr();
            display(in);
            cout<<zen<<"==>("<<xen<<","<<yen<<")\n";
            system("pause");

            in[yen-1][xen-1]=zen;
            p.x=xen-1;
            p.y=yen-1;
            p.in=zen;

        }
    }
}

bool lsearch(int in, int sud[9][9], int i, int j, int opt)
{
    if(opt==1)                                             //Checks row
    {
    for(int x=0;x<9;x++)
        if(sud[i][x]==in)
            return true;
    return false;
    }

    if(opt==2)                                             //Checks column
    {
     for(int x=0;x<9;x++)
        if(sud[x][j]==in)
            return true;
     return false;
    }
}

bool sqcheck(int sud[][9],int i,int j,int z)
{
    int a, b, c, d;


    if((j+1)%3!=0)
        {
            b = j+1+ (3-((j+1)%3));
            a = j+1 - ((j+1)%3);
        }

    else
        {
            b = j+1;
            a = j - 2;
        }


     if((i+1)%3!=0)
        {
            d = (i+1) + (3-((i+1)%3));
            c = i+1 - ((i+1)%3);
        }

    else
        {
            d = i+1;
            c = i - 2;
        }

    for(int x=c;x<d;x++)
        for(int y=a;y<b;y++)
            if(sud[x][y]==z)
                    return true;
    return false;
}


void con(int config[35])
{
    ifstream a;
    int z,temp,i=0;

    switch(z)
    {
        case 1:
            a.open("easy1.dat",ios::binary);

            if(!a)
                cout<<"Error!";

            while(!a.eof())
            {
                a.read((char*)&temp,sizeof(temp));
                config[i]=temp;
                i++;

            }
        a.close();

    }
}

void affix(int sud[][9])
{
        for(int i=0;i<tot;i++)
        {
            sud[(config[i]/10)-1][(config[i]%10)-1]=-sud[(config[i]/10)-1][(config[i]%10)-1];
        }
}

void display(int sud[][9])
{   cout<<'\n'<<'\n';
    cout<<"   1   2   3   4   5   6   7   8   9 \n   ----------+-----------+-----------"<<'\n';
    for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                if(sud[i][j]>0)
                {
                    if(j==0)
                        cout<<i+1<<" |";

                    if((j+1)%3==0)
                         cout<<abs(sud[i][j])<<" | ";

                    else
                        cout<<abs(sud[i][j])<<"   ";

                }

                else
                {
                    if(j==0)
                        cout<<i+1<<" |";

                    if((j+1)%3==0)
                         cout<<'x'<<" | ";

                    else
                        cout<<'x'<<"   ";

                }

            }
            cout<<'\n';
         if((i+1)%3==0)
         cout<<"   ----------+-----------+-----------"<<'\n';
        }
        cout<<"\n\n\n";
}
void disfull(int sud[][9])
{   cout<<'\n'<<'\n';
    cout<<"   1   2   3   4   5   6   7   8   9 \n   ----------+-----------+-----------"<<'\n';
    for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                    if(j==0)
                        cout<<i+1<<" |";

                    if((j+1)%3==0)
                         cout<<abs(sud[i][j])<<" | ";

                    else
                        cout<<abs(sud[i][j])<<"   ";


            }
            cout<<'\n';
         if((i+1)%3==0)
         cout<<"   ----------+-----------+-----------"<<'\n';
        }
        cout<<"\n\n\n";
}
int generator()
{
 int i=0,coun=1,bt=0,l=0,z,j,ar[88];
    /*
       z holds the value which could take the current position
       i and j are for loops
       coun is for checking if all possibilities have been exhausted (which will be more clear in the context of the code)
    */
    for(int xyz=0;xyz<89;xyz++)
        ar[xyz]=2;

    srand(time(NULL));

        while(i<9)
        {
            for(j=0;j<9;)
            {

                if(coun==1)
                    {
                        z=(rand()%9)+1;
                    }

                    if(!lsearch(z,sud,i,j,1) && !lsearch(z,sud,i,j,2)&&!sqcheck(sud,i,j,z))     //Checks if the number has any conflicts
                    {
                        ar[(i*10)+j]=coun;                                                                     //If there is no conflict, the position takes z and the pointer moves to the next position
                        sud[i][j]=z;
                        coun=1;
                        j++;

                    }

                    else                                                   //If the number has a conflict, the number is incremented by 1
                    {                                                      //It will still be random because if a number x is random, x+1 is also random
                        z++;
                        if(z>9)
                            z=1;

                        coun++;

                        if(coun>9)
                        /*If the number has been incremented more than 9 times, it means all numbers have been tried.
                          That would mean something is wrong with one of the previous numbers.
                          So we go back and change the previous number. If we still have a problem, we change the number before that.
                          We keep doing this until we find a solution.*/
                        {
                            j--;

                            z=sud[i][j]+1;
                            sud[i][j]=0;
                            if(z>9)
                            z=1;
                            coun=ar[(i*10)+j];

                        }
                    }


            if(j==-1)    //This is to go one row up when we have backtracked and reached the beginning of the row
            {
                i--;
                j==8;
            }
        }
     i++;
    }

    return 0;

}

int decorate()
{
    int input;
    cout<<"\n                                                                     \n";
    cout<<"  sssss     uu   uu    ddddd        ooooo     kk   kk    uu   uu\n";
    cout<<" ss   ss    uu   uu    ddddddd     ooooooo    kk  kk     uu   uu\n";
    cout<<" ss         uu   uu    dd    dd    oo   oo    kk kk      uu   uu\n";
    cout<<"  ssss      uu   uu    dd    dd    oo   oo    kkkk       uu   uu\n";
    cout<<"      ss    uu   uu    dd    dd    oo   oo    kk kk      uu   uu\n";
    cout<<" ss   ss    uuuuuuu    ddddddd     ooooooo    kk  kk     uuuuuuu\n";
    cout<<"  sssss      uuuuu     ddddd        ooooo     kk   kk     uuuuu  \n";

    cout<<"\n\n1.Play\n\n2.Help\n\n3.Exit\n";
    cin>>input;

    return input;
}

void help()
{
    clrscr();
    cout<<"\nHELP\n";
    cout<<"\nThe game of Sudoku involves a simple grid of 81 squares, arranged 9x9. The rules of the game are simple: each of the nine blocks has to contain all the numbers 1-9 within its squares. Each number can only appear once in a row, column or box. Furthermore, every nine-square column and row should also contain all the numbers 1-9, again without repitition or omission.\n";

    cout<<"\nIn this game, we use the co-ordinate system to locate and navigate to squares. To enter a number in a particular square, enter the X-coordinate of the square, Y-coordinate, and the number itself, in the same order. To submit the Sudoku once you've solved it, or to perform some other special actions, you'll have to enter its character according to this key:\n";

    cout<<"\nu: Undo";
    cout<<"\nr: Redo";
    cout<<"\ng: Give Up";
    cout<<"\n$: Submit";
}




int main()
{
    int z;

    beg:
    clrscr();
    crecon();
    z=decorate();

    char ex;

    switch(z)
    {
    case 1:{
        generator();
        affix(sud);
        ass();
        input();
        profiles();
        cout<<"\n \n"<<"UPDATED USER PROFILES: \n";
        displayprofile();

           }

    break;
    case 2:char ch;
         help();
         cout<<"\n\n Press m for main menu\n";cin>>ch;
         if(ch=='m')
            goto beg;
    break;

    case 3:
        cout<<"\nAre you sure you want to exit? (y/n)\n";
        cin>>ex;
        if(ex=='y')
            exit(0);
        else if(ex=='n')
            goto beg;
        else
            cout<<"\nPlease enter a valid choice\n";
    }
    cout<<"\n \n";
    return 0;
}

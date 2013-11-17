#include<iostream>
using namespace std;

int main(){
    string buffer;
    string input;
    while (cin>>buffer){
        input+=buffer;
        if (input.size()==81){
            break;
        }
    }
    
    input="";
    while (cin>>buffer){
        input+=buffer;
        if (input.size()==81){
            break;
        }
    }

    int size=input.size();
    if (size==5){
        if (input=="ERROR"){
            cout<<"error"<<endl;
            return 0;
        }
        else{
            cout<<"invalid_size"<<endl;
            return 0;
        }
    }


    if (size!=81){
        cout<<"invalid_size"<<endl;
        return 0;
    }

    int sudoku[9][9];
    for (int i=0;i<81;i++){

        int num=input[i]-48;

        if ((num<1)||(num>9)){
            cout<<"invalid"<<endl;
            return 0;
        }
        sudoku[i/9][i%9]=num;
    }
    
    //rows

    for (int m=0;m<9;m++){
        int sum=0;
        int count=0;
        int a[9]={0,0,0,0,0,0,0,0,0};
        for (int n=0;n<9;n++){
            a[n]=sudoku[m][n];
        }
        for(int mm=1;mm<=9;mm++){
            bool isUnique=1;
            for (int nn=0;nn<9;nn++){
                if(mm==a[nn]){
                    if (isUnique){
                        isUnique=0;
                    }
                    else{
                        cout<<"invalid_ROW "<<m<<endl;
                        return 0;
                    }
                }
            }
        }
    }
    //columns
    for (int u=0;u<9;u++){
        int sum=0;
        int count=0;
        int a[9]={0,0,0,0,0,0,0,0,0};
        for (int v=0;v<9;v++){
            a[v]=sudoku[v][u];
        }
        for(int uu=1;uu<=9;uu++){
            bool isUnique=1;
            for (int vv=0;vv<9;vv++){
                if(uu==a[vv]){
                    if (isUnique){
                        isUnique=0;
                    }
                    else{
                        cout<<"invalid_COL" <<u<<endl;
                        return 0;
                    }
                }
            }
        }
    }
    
    //boxes
    for (int x=0;x<9;x++){
        int sum=0;
        int count=0;
        int a[9]={0,0,0,0,0,0,0,0,0};
        for (int y=0;y<9;y++){
            int xx=y/3+x/3*3;
            int yy=y%3+x%3*3;
            a[y]=sudoku[xx][yy];
        }
        for(int xxx=1;xxx<=9;xxx++){
            bool isUnique=1;
            for (int yyy=0;yyy<9;yyy++){
                if(xxx==a[yyy]){
                    if (isUnique){
                        isUnique=0;
                    }
                    else{
                        cout<<"invalid_BOX "<<x<<endl;
                        return 0;
                    }
                }
            }
        }
    }
    cout<<"valid"<<endl;
    return 0;
    

}

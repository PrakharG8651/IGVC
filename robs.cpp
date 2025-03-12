#include <bits/stdc++.h>
using namespace std;
int n=14,m=11;//n is the number of rows and m is the number of columns
vector<int> position(2);//Position contains the initial position of the bot
vector<int> speedVector(2);//SpeedVector contains the direction of the bot
vector< vector<int> > matrix(n,vector<int>(m));//Matrix contains the input grid
vector< vector<int> > speedGraph(n,vector<int>(m,-1));//SpeedGraph contains the trace of speed of next step at each position
int speed=1,minSteps=-1;//Speed is the speed of the bot and minSteps is the minimum steps to reach the destination
vector< vector<int> > bestGraph(n,vector<int>(m,-1));
bool canFill=0;//An indicator which tells the bot is at starting or ending as the bot can fill the path only when it reaches the end
vector<int> front(2), leftD(2), rightD(2);//front,leftD,rightD denotes front,left and right directions respectively
int frontDistance;//It has the distance of the nearest 1 in front of the bot
int frontNearestBot;//It has the distance of nearest obstacle , in case there is no 2 in front of bot it will be equal to frontdistance

void minimisePath(int,int,int,bool);

void setVectors(int i,int j){// Using initial coordinates to set the vectors
    if(j<4){
        speedVector[0]=front[0]=-1;speedVector[1]=front[1]=0;leftD[0]=0;leftD[1]=-1;rightD[0]=0;rightD[1]=1;
    }
    else if(j<7){
        if(i<7){
            speedVector[0]=front[0]=0;speedVector[1]=front[1]=1;leftD[0]=-1;leftD[1]=0;rightD[0]=1;rightD[1]=0;
        }
        else{
            speedVector[0]=front[0]=0;speedVector[1]=front[1]=-1;leftD[0]=1;leftD[1]=0;rightD[0]=-1;rightD[1]=0;
        }
    }
    else{
        speedVector[0]=front[0]=1;speedVector[1]=front[1]=0;leftD[0]=0;leftD[1]=1;rightD[0]=0;rightD[1]=-1;
    }
}

void print(vector< vector<int> > &mat){//Printing the matrix to trace the movement
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(matrix[i][j]==1){
                cout<<"# ";
            }
            else if(mat[i][j]==-1){
                cout<<". ";
            }
            else{
                cout<<mat[i][j]<<" ";
            }
        }
        cout<<endl;
    }
    cout<<endl;
}

bool isClear(int i1,int i2,int j1,int j2){//Checking if the path is clear
    if(i1<0 or i2<0 or i1>=n or i2>=n or j1<0 or j2<0 or j1>=m or j2>=m){
        return 0;
    }
    for(int i=min(i1,i2);i<=max(i1,i2);i++){
        for(int j=min(j1,j2);j<=max(j1,j2);j++){
            if(matrix[i][j]==1 or matrix[i][j]==2){
                return 0;
            }
        }
    }
    return 1;
}

void processDistances(int i,int j){//Calculates the distances
    frontDistance=1;frontNearestBot=-1;bool find=1;
    while(matrix[i+frontDistance*front[0]][j+frontDistance*front[1]]!=1){
        if(find and matrix[i+frontDistance*front[0]][j+frontDistance*front[1]]==2){
            find=0;frontNearestBot=frontDistance;
        }
        frontDistance++;
    }
    if(frontNearestBot==-1){
        frontNearestBot=frontDistance;
    }
}

void rotateVectorLeft(vector<int> &vc){//Rotates the vector left
    int a=-vc[1],b=vc[0];
    vc[0]=a;vc[1]=b;
}

void rotateVectorRight(vector<int> &vc){//Rotates the vector right
    int a=vc[1],b=-vc[0];
    vc[0]=a;vc[1]=b;
}


bool correctDirection(int i,int j){//Checking whether the direction of the bot is correct
    if(j<4){
        if(speedVector[0]==-1 or abs(speedVector[1])==1){
            return 1;
        }
        return 0;
    }
    else if(j<7){
        if(abs(speedVector[0])==1){
            return 1;
        }
        if((i<7 and speedVector[1]==1)or(i>7 and speedVector[1]==-1)){
            return 1;
        }
        return 0;
    }
    if(speedVector[0]==1 or abs(speedVector[1])==1){
        return 1;
    }
    return 0;
} 

void paste(){//Pasting the bestGraph to the matrix
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            bestGraph[i][j]=speedGraph[i][j];
        }
    }
}

bool isCorrect(int i,int j,int ni,int nj){//Checking whether the movement of the bot is correct
    int t1=ni-i,t2=nj-j;
    if(j<=4){
        return t1<=0;
    }
    if(j<7){
        if(i<7){
            return t2>=0;
        }
        return t2<=0;
    }
    return t1>=0;
}

bool start=0,cross=0,done=0;//start is an indicator which tells the bot has started moving, cross is an indicator which tells the bot has crossed the middle line, done is an indicator which tells the bot has reached the destination

void goRight(int i,int j,int steps){//Moving the bot right
    if(isClear(i,i+speed*speedVector[0]+rightD[0],j,j+speed*speedVector[1]+rightD[1]) and isCorrect(i,j,i+speed*speedVector[0]+rightD[0],j+speed*speedVector[1]+rightD[1])){
        minimisePath(i+speed*speedVector[0]+rightD[0],j+speed*speedVector[1]+rightD[1],steps+1,0);
    }
}

void goStraight(int i,int j,int steps){//Moving the bot straight
    if(isClear(i,i+speed*speedVector[0],j,j+speed*speedVector[1]) and isCorrect(i,j,i+speed*speedVector[0],j+speed*speedVector[1])){
        minimisePath(i+speed*speedVector[0],j+speed*speedVector[1],steps+1,0);
    }
}

void goLeft(int i,int j,int steps){//Moving the bot left
    if(isClear(i,i+speed*speedVector[0]+leftD[0],j,j+speed*speedVector[1]+leftD[1]) and isCorrect(i,j,i+speed*speedVector[0]+leftD[0],j+speed*speedVector[1]+leftD[1])){ 
        minimisePath(i+speed*speedVector[0]+leftD[0],j+speed*speedVector[1]+leftD[1],steps+1,0);
    }
}

void operate(int i,int j,int steps){//Operating the bot in right, straight and left directions. Right is given priority over straight and left  
    goRight(i,j,steps);
    goStraight(i,j,steps);
    goLeft(i,j,steps);
}

void increaseSpeed(int i,int j,int steps){//Increasing the speed of the bot
    if((frontNearestBot>speed)){
        speedGraph[i][j]=++speed;
        operate(i,j,steps+1);
        speed--;
        speedGraph[i][j]=-1;
    }
}

void constantSpeed(int i,int j,int steps){//Keeping the speed of the bot constant
    speedGraph[i][j]=speed;
    operate(i,j,steps+1);
    speedGraph[i][j]=-1;
}

void decreaseSpeed(int i,int j,int steps){//Decreasing the speed of the bot
    if(speed>=2){
        speedGraph[i][j]=--speed;
        operate(i,j,steps+1);
        ++speed;
        speedGraph[i][j]=-1;
    }
}


void rotateRightOrder(int i,int j,int steps){//Rotating the bot right
    if(speed<=1){
        rotateVectorRight(speedVector);
        speedGraph[i][j]=speed;
        if(correctDirection(i,j)){
            rotateVectorRight(front);rotateVectorRight(leftD);rotateVectorRight(rightD);
            minimisePath(i,j,steps,1);
            rotateVectorLeft(front);rotateVectorLeft(leftD);rotateVectorLeft(rightD);
        }
        rotateVectorLeft(speedVector);
        speedGraph[i][j]=-1;
    }
}

void minimisePath(int i,int j,int steps,bool lastRotated){//The operating system of program which minimises the path by taking actions according to their priority and co ordinates
    if(i<0 or i>=n or j<0 or j>=m or matrix[i][j]==1 or matrix[i][j]==2 or speed<0){
        return;
    }
    if(i==position[0] and j==position[1]){
        if(canFill and !done){
            done=1;
            if(start){
                paste();
                return;
            }
        }
        else{
            canFill=1;
        }
    }
    else{
        start=1;
    }
    if(!correctDirection(i,j) or (speedGraph[i][j]!=-1 and start and !lastRotated)){
        return;
    }
    if(i>position[0]){
        cross=1;
    }
    if(i<position[0] and j<4 and cross){
        return ;
    }
    print(speedGraph);
    processDistances(i,j);
    if(i<5 and j<7 and front[1]!=1){
        if(speed>1){
            decreaseSpeed(i,j,steps);
        }
        else if(!lastRotated){
            rotateRightOrder(i,j,steps);
        }
    }
    else if(i<10 and j>=7 and front[0]!=1){
        if(speed>1){
            decreaseSpeed(i,j,steps);
        }
        else if(!lastRotated){
            rotateRightOrder(i,j,steps);
        }
    }
    else if(i>=10 and j>=4 and front[1]!=-1){
        if(speed>1){
            decreaseSpeed(i,j,steps);
        }
        else if(!lastRotated){
            rotateRightOrder(i,j,steps);
        }
    }
    else if(i<5 and j<4 and front[0]!=-1){
        if(speed>1){
            decreaseSpeed(i,j,steps);
        }
        else if(!lastRotated){
            rotateRightOrder(i,j,steps);
        }
    }
    if(done){
        return;
    }
    if(speed<frontNearestBot){
        increaseSpeed(i,j,steps);
    }
    if(done){
        return;
    }
    constantSpeed(i,j,steps);
    if(done){
        return;
    }
    decreaseSpeed(i,j,steps);
}

int main(){
    freopen("input.txt","r",stdin);//The matrix is present in input.txt file
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            cin>>matrix[i][j];
            if(matrix[i][j]==3){
                position[0]=i;position[1]=j;
            }
        }
    }
    setVectors(position[0],position[1]);
    minimisePath(position[0],position[1],0,0);
    print(bestGraph);
}
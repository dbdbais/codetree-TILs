//
// Created by newbi on 2024-04-15.
//
#include <bits/stdc++.h>

#define endl '\n'
#define fastIO() ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
#define For(i,a,b) for(int i=a;i<b;i++)

using namespace std;

struct robot{
    int x;
    int y;
    int score =0;
    bool done = false;

    robot(int x,int y):x(x),y(y){}

    void printInfo(){
        cout <<"X : " <<x <<"  Y : " << y << "  Score : "<< score << endl;
    }
};
vector<robot> rVec;
int visited[80][80];
int rVisited[80][80];
bool position[80][80];
int R,C,K,no;
const int dx[] = {-1,0,1,0};    //상 우 하 좌
const int dy[] = {0,1,0,-1};

bool out(int x,int y){
    return x <1 || x >R || y <1 || y >C;
}

void output(){
    cout << "Visited" << endl;
    For(i,1,R+1){
        For(j,1,C+1){
            cout << visited[i][j]<<" ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "POSITION" << endl;
    For(i,1,R+1){
        For(j,1,C+1){
            cout << position[i][j]<<" ";
        }
        cout << endl;
    }
    cout << endl;

    for(auto elem : rVec){
        elem.printInfo();
    }
}




bool isAround(int x,int y){
    if(visited[x][y]) return true;
    For(i,0,4){
        int qx = x + dx[i];
        int qy = y + dy[i];
        if(out(qx,qy)) return true;
        if(visited[qx][qy]) return true;
    }
    return false;
}

int rotate(int pos, bool left){
    if(left){
        if(pos == 0){
            return 3;
        }
        else{
            return pos -1;
        }
    }
    else{
        return (pos+1)%4;
    }

}
bool check(){
    For(i,1,3){
        For(j,1,C+1){
            if(visited[i][j]) return true;
        }
    }
    return false;
}

void color(int x,int y,int no,int pos){

    visited[x][y] = no;

    For(i,0,4){
        visited[x+dx[i]][y+dy[i]] = no;
    }
    position[x+dx[pos]][y+dy[pos]] = true;
    if(check()){
        no =0;
        memset(visited,0,sizeof(visited));
        memset(position,false,sizeof(position));
    }
    else{
        rVec.push_back(robot(x,y));
    }

}

bool move(int x,int y,bool left){
    if(left){
        y-=1;
    }
    else{
        y+=1;
    }

    if(out(x,y)) return false;
    For(i,0,4){
        int qx = x +dx[i];
        int qy = y + dy[i];
        if(out(qx,qy)||visited[qx][qy]) return false;
    }
    x+=1;
    if(out(x,y)) return false;
    For(i,0,4){
        int qx = x +dx[i];
        int qy = y + dy[i];
        if(out(qx,qy)||visited[qx][qy]) return false;
    }
    return true;
}


int rBFS(int x,int y){
    memset(rVisited,false,sizeof(rVisited));
    int mx = x;
    queue<pair<int,pair<int,int>>> q;
    rVisited[x][y] = true;  //방문처리
    q.push({visited[x][y],{x,y}});

    while(q.size()){
        auto cur = q.front(); q.pop();
        int val = cur.first;
        int tx = cur.second.first;
        int ty = cur.second.second;

        For(i,0,4){
            int qx = tx + dx[i];
            int qy = ty + dy[i];
            if(out(qx,qy)||rVisited[qx][qy]||visited[qx][qy] == 0) continue;
            if(position[qx][qy] && visited[qx][qy] == val){
                if(mx < qx) mx = qx;
                rVisited[qx][qy] = true;
                //cout << qx << qy << endl;
                For(j,0,4){
                    int ax = qx + dx[j];
                    int ay = qy + dy[j];
                    if(out(ax,ay) || rVisited[ax][ay]||visited[ax][ay] == 0) continue;
                    if(mx < ax) mx = ax;
                    q.push({visited[ax][ay],{ax,ay}});
                    rVisited[ax][ay] = true;
                }

            }
            else{
                if(visited[qx][qy] != val) continue;
                rVisited[qx][qy] = true;
                if(mx < qx) mx = qx;
                q.push({val,{qx,qy}});
            }
        }
    }

    /*
    For(i,1,R+1){
        For(j,1,C+1){
            cout << rVisited[i][j]<<" ";
        }
        cout << endl;
    }
    cout<<endl;
*/
    return mx;
}

void robotMove(){
    for(robot & r : rVec){
        if(r.done) continue;
        r.score = max(rBFS(r.x,r.y)-2,0);
        r.done = true;
    }
}

void dropRocket(int col,int pos){
    no++;
    int curX = 1;
    int curY = col;
    bool done = false;

    while(!done){
        int nxtX = curX +1;
        if(!isAround(nxtX,curY)){
            //cout << "down" << endl;
            curX = nxtX;
        }
        else if(move(curX,curY,true)){
            curX +=1;
            curY -=1;
            pos = rotate(pos,true);
            //cout << "left" << endl;
        }
        else if(move(curX,curY,false)){
            curX +=1;
            curY +=1;
            pos = rotate(pos,false);
            //cout << "right" << endl;
        }
        else{
            done = true;
        }
    }
    color(curX,curY,no,pos);
}

void input(){
    int a,b;

    cin >> R >> C >> K;
    R= R+2;
    For(i,0,K){
        cin >> a >> b;
        dropRocket(a,b);
        robotMove();
        //output();
    }
}
int getScore(){
    int sm = 0;
    for(robot r : rVec){
        sm += r.score;
    }
    return sm;
}
int main(){
    fastIO();
    input();

    cout << getScore() << endl;

    return 0;
}
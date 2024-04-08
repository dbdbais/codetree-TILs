//
// Created by newbi on 2024-04-08.
//
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <cstring>
#define endl '\n'
#define fastIO() ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
#define For(i,a,b) for(int i=a;i<b;i++)

using namespace std;

int arr[11][11];
int visited[11][11];
int N,M,K;
int exitX,exitY;

const int dx[] = {-1,1,0,0};    //상 하 좌 우
const int dy[] = {0,0,-1,1};



int getDistance(int x,int y){
    return abs(x - exitX) + abs(y-exitY);
}

struct Player{
    int no;
    int x;
    int y;
    int dist = 0;
    bool done = false;
    Player(int no,int x,int y):x(x),y(y),no(no){}

};
vector<Player> vec;

bool out(int x,int y){
    return x <1 || y < 1 || x > N || y > N;
}
void print(){
    cout << "Arr"<<endl;
    For(i,1,N+1){
        For(j,1,N+1){
            cout << arr[i][j] <<" ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Visited "<<endl;
    For(i,1,N+1){
        For(j,1,N+1){
            cout << visited[i][j] <<" ";
        }
        cout << endl;
    }
}

void input(){
    For(i,1,N+1){
        For(j,1,N+1){
            cin >> arr[i][j];
        }
    }
    int x,y,no=1;
    For(i,0,M){
        cin >> x >> y;
        visited[x][y] += no;
        vec.push_back(Player(no++,x,y));
    }
    cin >> exitX >> exitY;
    visited[exitX][exitY] = -1;
}

bool isIn(int x,int y,int d){
    bool isExit = false;
    bool isPlayer = false;
    For(i,x,x+d){
        For(j,y,y+d){
            if(visited[i][j]> 0) isPlayer = true;
            if(visited[i][j] == -1) isExit = true;
        }
    }
    if(isExit && isPlayer) return true;
    return false;
}
pair<int,pair<int,int>> makeSquare(){
    pair<int,pair<int,int>> ret;
    int rad = 1e9;
    for(Player p : vec){
        if(p.done) continue;
        int tmp = max(abs(exitX- p.x),abs(exitY-p.y))+1;
        if(rad> tmp) rad = tmp; //최소 값으로 갱신
    }
    for(int i=1;i<=N-rad+1;i++){
        for(int j=1;j<=N-rad+1;j++){
            if(isIn(i,j,rad)){
                return{rad,{i,j}};
            }
        }
    }
}

void playerMove(){
    for(Player &p : vec){

        if(p.done) continue;    //p가 끝났다면
        //cout << p.no <<" Turn" <<endl;
        queue<pair<int,int>> ways;
        int curDist = getDistance(p.x,p.y);
        For(i,0,4){
            int qx = p.x + dx[i];
            int qy = p.y + dy[i];
            if(out(qx,qy) || arr[qx][qy] >= 1) continue;    //나갔거나 벽이라면
            if(getDistance(qx,qy) < curDist){
                ways.push({qx,qy});    //갈수 있다면 ㄱ
            }
        }
        if(ways.size()){
            //cout << p.no << "to "<<ways.front().first << ", "<< ways.front().second <<endl;
            visited[p.x][p.y] -= p.no;
            p.x = ways.front().first;
            p.y = ways.front().second;
            p.dist +=1;
            if(p.x == exitX && p.y == exitY){   //끝이라면
                //cout << p.no <<"goal in" <<endl;
                p.done = true;  //종료
            }
            else{
                visited[p.x][p.y] += p.no;  //이동
            }


        }
        else{
            //cout << "NO WAY" << endl;
        }
    }
    //print();
}
void rotate(pair<int,pair<int,int>> sq){
    int x = sq.second.first;    //2
    int y = sq.second.second;   //1
    int d = sq.first;           //2

    int tmp [d+1][d+1];
    int tmpVisited[d+1][d+1];

    memset(tmp,0,sizeof(tmp));
    memset(tmpVisited,0,sizeof(tmpVisited));

    for(int i=1;i<=d;i++){
        for(int j=1;j<=d;j++){
            tmp[i][j] = arr[x+d-j][y+i-1];  //arr 대입
            if(tmp[i][j] > 0) tmp[i][j]-=1; //벽이라면 마모
        }
    }

    for(int i=1;i<=d;i++){
        for(int j=1;j<=d;j++){
            arr[x+i-1][y+j-1] = tmp[i][j];  //덮어 씌우기
        }
    }

    for(int i=1;i<=d;i++){
        for(int j=1;j<=d;j++){
            tmpVisited[i][j] = visited[x+d-j][y+i-1];  //arr 대입
            if(tmpVisited[i][j] > 0){   //방문자 바꾼다.
                for(Player & p : vec){
                    if(p.x == (x+d-j) && p.y == (y+i-1)){
                        p.x = x + i-1;
                        p.y = y + j-1;
                    }
                }
            }
            else if(tmpVisited[i][j] == -1){
                exitX = x + i-1;
                exitY = y + j-1;
            }
        }
    }

    for(int i=1;i<=d;i++){
        for(int j=1;j<=d;j++){
            visited[x+i-1][y+j-1] = tmpVisited[i][j];  //덮어 씌우기
        }
    }
    /*
    cout << "tmp ARR" << endl;
    For(i,1,d+1){
        For(j,1,d+1){
            cout << tmp[i][j] << " ";
        }
        cout << endl;
    }
    cout << "tmpVisited ARR" << endl;
    For(i,1,d+1){
        For(j,1,d+1){
            cout << tmpVisited[i][j] << " ";
        }
        cout << endl;
    }
*/
}

bool check(){
    for(Player p : vec){
        if(!p.done) return false;
    }
    return true;
}
int getScore(){
    int sm = 0;
    for(Player p : vec){
        sm += p.dist;
    }
    return sm;
}

void solve(){
    while(K--){
        playerMove();
        if(check()) break;
        auto ret = makeSquare();
        //cout << ret.first << "<" << ret.second.first <<", " <<ret.second.second << ">"<< endl;
        rotate(ret);
        //print();
    }
    cout << getScore() << endl;
    cout << exitX <<" "<< exitY << endl;
}

int main(){
    fastIO();
    cin >> N >> M >>K;
    input();
    solve();

    return 0;
}
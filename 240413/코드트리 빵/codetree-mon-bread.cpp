//
// Created by newbi on 2024-04-13.
//
#include <iostream>
#include <vector>
#include <algorithm>
#define fastIO() ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
#define For(i,a,b) for(int i=a;i<b;i++)
#define endl '\n'

using namespace std;

struct person{
    int x;
    int y;
    int m;
    int fx;
    int fy;
    bool isBase = false;
    bool done = false;
    person(int fx,int fy,int m):fx(fx),fy(fy),m(m){}

    void printInfo(){
        cout << "x : " << x << "y : "<< y << "Done : " <<done << endl;
    }
};

int N,M,cx,cy;

int getDistance(int x1,int y1,int x2,int y2){
    return abs(x1-x2) + abs(y1-y2);
}

struct facility{
    int x;
    int y;
    bool used = false;
    facility(int x,int y):x(x),y(y){}
    bool operator <(facility f){
        int tDist = getDistance(x,y,cx,cy);
        int fDist = getDistance(f.x,f.y,cx,cy);

        if(tDist == fDist){
            if(x == f.x){
                return y < f.y;
            }
            return x < f.x;
        }
        return tDist < fDist;

    }
    void printInfo(){
        cout << "( "<< x << y <<" ) U : "<< used<<endl;
    }
};

const int dx[] = {-1,0,0,1};    //상 좌 우 하
const int dy[] = {0,-1,1,0};

bool out(int x,int y){
    return x<1 || y < 1 || x > N || y > N;
}



vector<facility> basecamp;
vector<facility> store;
vector<person> pVec;

int arr[18][18];
bool visited[18][18];

void output(){
    cout << "DEBUG"<<endl;
    For(i,1,N+1){
        For(j,1,N+1){
            cout << visited[i][j]<<" ";
        }
        cout << endl;
    }
    cout << endl;

    for(person p : pVec){
        p.printInfo();
    }

    cout <<endl;
}

void firstMove(int t){
    for(auto & p : pVec){
        if(p.m > t || p.done) continue;   //시간이 안맞거나 이미 끝났다면
        int curDist = getDistance(p.x,p.y,p.fx,p.fy);
        int mnDist = curDist;

        For(i,0,4){ //최소 거리 구한다.
            int qx = p.x + dx[i];
            int qy = p.y + dy[i];
            if(out(qx,qy)|| visited[qx][qy]) continue;
            int tmpDist = getDistance(qx,qy,p.fx,p.fy);
            if(tmpDist < mnDist){
                mnDist = tmpDist;
            }
        }

        For(i,0,4){ //우선순위 중에 최소거리인거로 바꾼다.
            int qx = p.x + dx[i];
            int qy = p.y + dy[i];
            if(out(qx,qy)|| visited[qx][qy]) continue;
            int tmpDist = getDistance(qx,qy,p.fx,p.fy);
            if(tmpDist == mnDist){
                /*
                if(p.isBase){
                    p.isBase = false;
                }
                else{
                    visited[p.x][p.y] = false;
                }
                 */
                //cout << "change to " << qx <<", "<<qy <<endl;
                p.x = qx;
                p.y = qy;   //좌표 옮긴다.
                break;
            }
        }
    }
}

void secondMove(int t){
    for(person & p :pVec){
        if(p.m > t || p.done) continue;   //시간이 안맞거나 이미 끝났다면

        if(p.x == p.fx && p.y == p.fy){ //도착지에 도착했다면
            p.done = true;
            visited[p.x][p.y] = true;
        }

    }

}

void thirdMove(int t){
    for(person &p : pVec){
        if(p.m != t) continue;   //시간이 안맞다면
        cx = p.fx;
        cy = p.fy;   //전역변수로 설정하고
        //cout << "c store" << cx << cy << endl;
        //베이스 캠프에 들어간다. 방문처리
        sort(basecamp.begin(),basecamp.end());

        for(auto & bc : basecamp){
            if(bc.used) continue;
            //사용가능하다면
            p.x = bc.x;
            p.y = bc.y;
            bc.used = true;
            //p.isBase = true;
            visited[p.x][p.y] = true;
            //베이스 캠프로 세팅
            break;
        }
    }

}

bool check(){
    for(person p : pVec){
        if(!p.done) return false;
    }
    return true;
}



void solve(){
    int t = 1;
    while(true){
        //cout<<"Time : " << t <<endl;
        firstMove(t);
        secondMove(t);
        thirdMove(t);
        //output();
        if(check()) break;
        t++;
    }
    cout << t << endl;
}

void input(){
    cin >> N >> M;
    For(i,1,N+1){
        For(j,1,N+1){
            cin >> arr[i][j];
            if(arr[i][j] == 1){ //베이스 캠프 전부 집어넣는다.
                basecamp.push_back(facility(i,j));
            }
        }
    }
    int a,b;
    For(i,1,M+1){
        cin >> a >> b;
        pVec.push_back(person(a,b,i));
        store.push_back(facility(a,b));
    }
}

int main(){
    fastIO();
    input();
    solve();

    return 0;
}
//
// Created by newbi on 2024-06-13.
//
#include <iostream>
#include <queue>
#include <cstring>
#include <algorithm>
#include <map>
#include <vector>
#define endl '\n'
#define For(i,a,b) for(int i=a;i<b;i++)
#define fastIO() ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)

using namespace std;

int N,M;

int baseCamp[17][17];
bool visited[17][17];
int dist[17][17];
int dVisited[17][17];

const int dx[4] = {-1,0,0,1};
const int dy[4] = {0,-1,1,0};
//상 좌 우하

struct Customer{
    int x;
    int y;
    bool life = false;
};

struct bDist{
    int dst;
    int x;
    int y;

    bool operator< (const bDist other){
        if(dst == other.dst){
            if(x == other.x){
                return y < other.y;
            }else{
                return x < other.x;
            }
        }
        else{
            return dst < other.dst;
        }
    }
};
map<int,pair<int,int>> sCord;
Customer cArr[32];
vector<pair<int,int>> camps;

bool out(int x,int y){
    return x < 1 || y < 1 || x> N || y > N;
}

void output(){
    cout << "BASECAMP " << endl;
    For(i,1,N+1){
        For(j,1,N+1){
            cout << baseCamp[i][j] <<" ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "VISITED " << endl;
    For(i,1,N+1){
        For(j,1,N+1){
            cout << visited[i][j] <<" ";
        }
        cout << endl;
    }
    cout << endl;

    For(i,1,M+1){
        cout<< "X: " << cArr[i].x << " Y : "<<cArr[i].y <<" LIFE : "<< cArr[i].life << endl;
    }
}

void input(){
    cin >> N >> M;
    For(i,1,N+1){
        For(j,1,N+1){
            cin >> baseCamp[i][j];
            if(baseCamp[i][j]) camps.push_back({i,j});
        }
    }
    int x,y;
    For(i,1,M+1){
        cin >> x >> y;
        sCord.insert({i,{x,y}});
    }
}

void getDist(int x,int y){
    memset(dVisited,false,sizeof(dVisited));
    memset(dist,0,sizeof(dist));

    queue<pair<int,int>> q;

    dVisited[x][y] = true;
    q.push({x,y});

    while(q.size()){
        auto cur = q.front(); q.pop();
        int cx = cur.first;
        int cy = cur.second;
        int val = dist[cx][cy];

        For(i,0,4){
            int qx = cx + dx[i];
            int qy = cy + dy[i];

            if(out(qx,qy) || visited[qx][qy] || dVisited[qx][qy]) continue;
            //나갔거나 못가는 거리, 이미 방문한 곳 스킵

            dist[qx][qy] = val +1;
            dVisited[qx][qy] = true;
            q.push({qx,qy});
        }
    }
}

void firstMove(int t){
    For(i,1,M+1){
        if(t < i || !cArr[i].life) continue;
        //꺼져있으면 스킵
        vector<pair<int,int>> distVec;
        //first 거리 길이 가까운 순, second dir 상좌우하 순

        int x = cArr[i].x;
        int y = cArr[i].y;

        //편의점 위치
        int storeX = sCord[i].first;
        int storeY = sCord[i].second;

        For(a,0,4){
            int qx = x + dx[a];
            int qy = y + dy[a];

            if(out(qx,qy) || visited[qx][qy]) continue;
            getDist(qx,qy);
            //qx qy에 맞게 모든 거리 최소로 갱신됨

            distVec.push_back({dist[storeX][storeY],a});
            //후보지 다 넣고
        }

        sort(distVec.begin(),distVec.end());

        int optimalDirec  = distVec.front().second;

        //그 위치로 이동
        cArr[i].x += dx[optimalDirec];
        cArr[i].y += dy[optimalDirec];

    }
}

void secondMove(int t){
    For(i,1,M+1){
        if(t < i ||!cArr[i].life) continue;

        if(cArr[i].x == sCord[i].first && cArr[i].y == sCord[i].second){
            cArr[i].life = false;
            visited[cArr[i].x][cArr[i].y] = true;
            //아무도 못가게 막는다.
        }
    }
}

pair<int,int> calcDist(pair<int,int> store){
    vector<bDist> calVec;
    for(auto elem : camps){
        if(visited[elem.first][elem.second]) continue;
        int tmpDist = abs(elem.first - store.first) + abs(elem.second - store.second);
        calVec.push_back({tmpDist,elem.first,elem.second});
    }
    sort(calVec.begin(),calVec.end());
    return {calVec.front().x,calVec.front().y};
}

void thirdMove(int t){
    if(t <= M){
        auto stay = calcDist(sCord[t]);
        cArr[t].life = true;
        cArr[t].x = stay.first;
        cArr[t].y = stay.second;
        visited[stay.first][stay.second] = true;
    }
}

bool check(){
    For(i,1,M+1){
        if(!visited[sCord[i].first][sCord[i].second]) return true;
        //아직 방문하지 않았으니 계속 돌려야함
    }
    //모두 방문되었다.
    return false;
}

void solve(){
    int _time =0;
    while(check()){
        _time++;
        firstMove(_time);
        secondMove(_time);
        thirdMove(_time);
        //cout << _time << endl;
        //output();
    }
    cout << _time << endl;
}

int main(){
    fastIO();
    input();
    solve();
    //output();
    return 0;
}
//
// Created by newbi on 2024-04-12.
//
#include <iostream>
#include<vector>
#include <queue>
#include<algorithm>
#define endl '\n'
#define fastIO() ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
#define For(i,a,b) for(int i=a;i<b;i++)

using namespace std;

const int INF = 1e9;
const int dx[] = { 0,1,0,-1 };	//우 하 좌 상
const int dy[] = { 1,0, -1,0 };
const int bombX[] = {0,0,-1,1,1,1,-1,-1};
const int bombY[] = {1,-1,0,0,1,-1,1,-1};
int N, M, K;
int visited[12][12];
int arr[12][12];
int fx, fy;

struct tower {
    int x;
    int y;
    int no;
    int attack;
    int atkTime = 0;	//공격한 시간
    bool touch = false;
    tower(int no, int x,int y,int attack):no(no),x(x),y(y),attack(attack){}

    bool operator <(tower t) {	//공격자 선정 시

        if (attack == t.attack) {
            if (atkTime == t.atkTime) {	//공격 시점이 같다면
                int rcSum = x + y;
                int rcSum2 = t.x + t.y;
                if (rcSum == rcSum2) {	//행 열의 합이 같다면
                    return y > t.y;	//열의 값이 큰게 앞으로
                }
                else {
                    return rcSum > rcSum2;
                }

            }
            else {
                return atkTime > t.atkTime;	//공격이 최근인 시점
            }

        }
        return attack < t.attack;
    }

    void printInfo() {
        cout<<"No. "<< no << "  " << x << ", " << y << "  ATK : " << attack << " ATKTIME : " << atkTime << endl;
    }
};

vector<tower> vec;

void output() {
    For(i, 0, N) {
        For(j, 0, M) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    for (tower t : vec) {
        t.printInfo();
    }
}
bool selTarget(tower a, tower b) {
    if (a.attack == b.attack) {
        if (a.atkTime == b.atkTime) {
            int rcSum = a.x + a.y;
            int rcSum2 = b.x + b.y;
            if (rcSum == rcSum2) {	//행 열의 합이 같다면
                return a.y < b.y;	//열의 값이 작은게 앞으로
            }
            else {
                return rcSum < rcSum2;	//행열의 합이 작은게 앞으로
            }
        }
        else {
            return a.atkTime < b.atkTime;	//공격한지 오래된
        }
    }
    return a.attack > b.attack;

}


bool out(int x, int y) {
    return x < 0 || y < 0 || x >= N || y >= M;
}

pair<int,int> restrict(int x,int y){
    pair<int,int> ret;
    if (x < 0) {
        ret.first=  (x % N + N)% N;
    }
    else if (x >= N) {
        ret.first=  x % N;
    }
    else{
        ret.first = x;
    }
    if (y < 0) {
        ret.second=  (y % M + M)% M;
    }
    else if (y >= M) {
        ret.second=  y % N;
    }
    else{
        ret.second = y;
    }
    return ret;

}
void doAtk(int x,int y,int atk,int tim){
    for(tower & t : vec){
        if(t.x == x && t.y == y){
            t.attack += atk;
            t.touch = true;
            t.atkTime = tim;
            break;
        }
    }
}
void doAtked(int x,int y,int atk){
    for(tower & t : vec){
        if(t.x == x && t.y == y){
            t.attack = max(0,t.attack-atk);
            t.touch = true;
            break;
        }
    }
}
void recover(){
    for(tower & t : vec){
        if(!t.touch){
            t.attack +=1;
            arr[t.x][t.y] +=1;  //증가
        }
        else{
            t.touch = false;
        }
    }
}

void BFS(int x, int y,int t) {
    fill(&visited[0][0], &visited[0][0] + 12 * 12, INF);	//0으로 초기화
    int cnt = 1;
    visited[x][y] = cnt;
    arr[x][y]+= (N+M);  //공격 증가
    doAtk(x,y,N+M,t); //실제 증가
    queue<pair<int, int>> q;  //방향

    q.push({ x,y });

    while (q.size()) {

        auto cord = q.front(); q.pop();

        int px = cord.first;
        int py = cord.second;
        For(i,0,4){
            int qx = px + dx[i];  //현재 위치로
            int qy = py + dy[i];
            if (out(qx, qy)) {	//나갔다면
                auto resCord = restrict(qx,qy);
                qx = resCord.first;
                qy = resCord.second;
            }
            if(arr[qx][qy] == 0) continue;  //0이면 컨티뉴
            if(visited[qx][qy] > visited[cord.first][cord.second]+1){
                visited[qx][qy] = visited[cord.first][cord.second] + 1;
                q.push({ qx,qy });	//큐 안에 넣는다.
            }
        }
    }
/*
    For(i, 0, N) {
        For(j, 0, M) {
            if(visited[i][j] == INF){
                cout << "I ";
            }
            else{
                cout << visited[i][j] << " ";
            }

        }
        cout << endl;
    }
    cout << endl;
*/
    vector<pair<int,int>> path;
    q.push({fx,fy});
    int rdir = 3;
    while(q.size()){
        auto cord = q.front(); q.pop();
        int tx = cord.first;
        int ty = cord.second;
        int val = visited[tx][ty];  //현재 위치 값
        if(val == 2) break; //2라면 끊는다.
        for(int i = rdir;i>=0;i--){
            int qx = tx +dx[i];
            int qy = ty + dy[i];
            if (out(qx, qy)) {	//나갔다면
                auto resCord = restrict(qx,qy);
                qx = resCord.first;
                qy = resCord.second;
            }
            if(arr[qx][qy] == 0) continue;  //0이면 컨티뉴
            if(visited[qx][qy] == val-1){
                path.push_back({qx,qy});
                q.push({qx,qy});
                break;
            }
        }
    }

    if(visited[fx][fy] == INF){ //포탄 공격
        //cout << "potan"<<endl;
        For(i,0,8){
            int qx = fx + bombX[i];
            int qy = fy + bombY[i];
            if(out(qx,qy)){
                auto resCord = restrict(qx,qy);
                qx = resCord.first;
                qy = resCord.second;
            }
            if(arr[qx][qy] == 0 || (qx == x && qy == y)) continue;  //이미 부서진 포탑이거나 본인은 뺀다.
            doAtked(qx,qy, arr[x][y] / 2);
            arr[qx][qy] = max(arr[qx][qy] - arr[x][y]/ 2, 0);
        }

    }
    else {  //레이저 공격
        //cout <<"Laser" <<endl;
        for (auto elem: path) {
            //경로에 있는 거 뺀다.
            doAtked(elem.first, elem.second, arr[x][y] / 2);
            arr[elem.first][elem.second] = max(arr[elem.first][elem.second] - arr[x][y] / 2, 0);
        }
    }
    doAtked(fx,fy,arr[x][y]);
    arr[fx][fy] = max(arr[fx][fy]-arr[x][y],0); //목적지 공격
}

void delDead(){
    vec.erase(remove_if(vec.begin(),vec.end(),[&](tower t){
        if(t.attack == 0) return true;
        else return false;
    }),vec.end());
}


void solve() {
    int t = 1;
    while (K--) {
        sort(vec.begin(), vec.end());	//공격할 사람 선택 정렬
        //output();
        tower atk = vec[0];	//공격할 번호 선택
        sort(vec.begin(), vec.end(), selTarget);
        //output();
        tower vic = vec[0];	//피해자 선택
        fx = vic.x;
        fy = vic.y;
        BFS(atk.x, atk.y,t);    //레이저, 포탄 공격 실행
        t++;
        delDead();  //포탑 부서짐
        recover();
        if(vec.empty() || vec.size() <= 1) break;
    }
    sort(vec.begin(),vec.end(), selTarget);
    cout << vec[0].attack << endl;

}



void input() {
    int no = 1;
    For(i, 0, N) {
        For(j, 0, M) {
            cin >> arr[i][j];
            if (arr[i][j] > 0) {	//포탑이라면
                vec.push_back(tower(no++,i, j, arr[i][j]));
            }
        }
    }
}

int main() {
    fastIO();
    cin >> N >> M >> K;
    input();
    solve();
    return 0;
}
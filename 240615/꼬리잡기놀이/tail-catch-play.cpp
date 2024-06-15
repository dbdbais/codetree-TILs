//
// Created by newbi on 2024-06-15.
//
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <cstring>
#include <algorithm>

#define endl '\n'
#define For(i,a,b) for(int i=a;i<b;i++)
#define fastIO() ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)

using namespace std;

int N,M,K,ans;
vector<int> team[6];
deque<pair<int,int>>tCord[6];
//팀의 좌표가 들어갈 곳
int arr[21][21];
int tArr[21][21];
bool visited[21][21];

const int dx[4] = {0,-1,0,1};
const int dy[4] = {1,0,-1,0};
//우 상 좌 하

bool out(int x,int y){
    return x < 1 || y < 1 || x > N || y > N;
}

void output(){
    For(i,1,N+1){
        For(j,1,N+1){
            cout << arr[i][j] <<" ";
        }
        cout << endl;
    }
    cout << endl;

    For(i,1,M+1){
        cout << i <<"th Team" << endl;
        for(int idx : team[i]){
            cout << idx <<"      ";
        }
        cout << endl;
        for(auto c : tCord[i]){
            cout<<"(" <<c.first <<","<<c.second <<") ";
        }
        cout << endl;
    }
    cout<<"ANS: " << ans << endl;
}

void input(){
    cin >> N >> M >> K;
    vector<pair<int,int>> oneVec;
    For(i,1,N+1){
        For(j,1,N+1){
            cin >> arr[i][j];
            if(arr[i][j] == 1){
                oneVec.push_back({i,j});
            }
        }
    }
    int tNo = 1;
    for(auto oc : oneVec){
        queue<pair<int,int>> q;
        memset(visited,false,sizeof(visited));
        q.push({oc.first,oc.second});
        visited[oc.first][oc.second] = true;
        team[tNo].push_back(1);
        tCord[tNo].push_back({oc.first,oc.second});
        while(q.size()){
            auto cur = q.front(); q.pop();
            int val = arr[cur.first][cur.second];
            tArr[cur.first][cur.second] = tNo;
            //팀 번호 대입

            For(i,0,4){
                int qx = cur.first + dx[i];
                int qy = cur.second + dy[i];

                if(out(qx,qy)||visited[qx][qy]||arr[qx][qy] == 0) continue;
                if(val == 1 && arr[qx][qy] != 2) continue;
                //1일경우에는 2로만 이동

                q.push({qx,qy});
                visited[qx][qy] = true;

                team[tNo].push_back(arr[qx][qy]);
                tCord[tNo].push_back({qx,qy});

                break;
            }
        }
        tNo++;
    }
}

void playMove(){
    For(t,1,M+1){
        auto cord = tCord[t].front();
        int qx,qy;
        For(i,0,4){
            qx = cord.first + dx[i];
            qy = cord.second + dy[i];
            if(out(qx,qy) || arr[qx][qy] == 0 || arr[qx][qy] == 2) continue;
            break;
        }
        //4의 위치가 들어있다.
        tCord[t].pop_back();
        //맨 뒤에꺼 뱉어냄
        tCord[t].push_front({qx,qy});
        For(i,0,team[t].size()){
            //cout << tCord[t][i].first << ","<<tCord[t][i].second<< " num : "<< team[t][i] <<endl;
            arr[tCord[t][i].first][tCord[t][i].second] = team[t][i];
        }
        //실제 모든거 대입
    }
}

void trwBall(int rnd){
    int dir = ((rnd-1) / N)%4;
    // 공을 던지는 방향 정한다.

    int num = (rnd-1) % N +1;
    //던질 위치
    int x,y;
    if(dir == 0){
        x = num;
        y = 1;
    }
    else if(dir == 1){
        x = N;
        y = num;
    }
    else if(dir == 2){
        x = N - num +1;
        y = N;
    }
    else if(dir == 3){
        x = 1;
        y = N - num+1;
    }
    //cout <<rnd << endl;
    //cout << x <<", "<< y << "dir" << dir<< endl;

    queue<pair<int,int>> q;
    q.push({x,y});
    while(q.size()){
        auto cur = q.front(); q.pop();
        int cx = cur.first;
        int cy = cur.second;

        //팀이 걸린다면
        if(arr[cx][cy] <= 3 && arr[cx][cy] > 0){
            int idx;
            int tNo = tArr[cx][cy];
            for(idx=0;idx<tCord[tNo].size();idx++){
                if(cx == tCord[tNo][idx].first && cy == tCord[tNo][idx].second ){
                    break;
                }
            }
            //cout << cx << cy << " HIT"<<endl;
            ans += (idx+1) * (idx+1);
            //점수 올리고
            stack<pair<int,int>> stk;
            stack<pair<int,int>> lstk;
            for(int three=0;three<team[tNo].size();three++){
                int curNo = team[tNo][three];
                if(curNo < 4){
                    stk.push(tCord[tNo][three]);
                }
                else{
                    lstk.push(tCord[tNo][three]);
                }
            }
            //좌표를 거꾸로 뒤집는다.
            For(i,0,team[tNo].size()){
                if(team[tNo][i] <4){
                    tCord[tNo][i] = stk.top();stk.pop();
                }
                else{
                    tCord[tNo][i] = lstk.top();lstk.pop();
                }

            }

            For(i,0,tCord[tNo].size()){
                arr[tCord[tNo][i].first][tCord[tNo][i].second] = team[tNo][i];
            }

            break;
        }
        int qx = cx + dx[dir];
        int qy = cy + dy[dir];

        if(out(qx,qy)) break;

        q.push({qx,qy});
        //다음 탐색
    }

}

void solve(){
    int rnd = 1;
    while(rnd <=K){
        //output();
        playMove();
        //output();
        trwBall(rnd);
        //output();
        rnd++;
    }
    cout << ans << endl;
}

int main(){
    fastIO();
    input();
    solve();
    //output();

    return 0;
}
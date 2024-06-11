//
// Created by newbi on 2024-06-11.
//
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <queue>
#define fastIO() ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
#define endl '\n'
#define For(i,a,b) for(int i=a;i<b;i++)

using namespace std;

int K,M,aIdx;
int ancestor[304];
int arr[5][5];
int temp[5][5];
bool visited[5][5];
bool disappear[5][5];

const int dx[4] = {0,0,-1,1};
const int dy[4] = {-1,1,0,0};

bool out(int x,int y){
    return x <0 || y < 0 || x >= 5 || y >= 5;
}

struct rotCord{
    int x;
    int y;
    int val;
    int rotNum;

    bool operator<(rotCord r){
        if(val == r.val){
            if(rotNum == r.rotNum){
                if(y == r.y){
                    return x < r.x;
                }
                else{
                    return y < r.y;
                }
            }
            else{
                return rotNum < r.rotNum;
            }
        }
        else{
            return val > r.val;
        }
    }
    void print(){
        cout << x << ", "<< y <<" Val : "<< val <<" rotNum : "<< rotNum <<endl;
    }
};

vector<rotCord> vec;
//회전 좌표가 들어갈 수

void print(){
    cout << "ARR" << endl;
    For(i,0,5){
        For(j,0,5){
            cout << arr[i][j]<< " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "DIS" << endl;
    For(i,0,5){
        For(j,0,5){
            cout << disappear[i][j]<< " ";
        }
        cout << endl;
    }
    cout << endl;
}

int BFS(int x,int y,bool real){
    int val;
    vector<pair<int,int>> ways;
    if(real){
        val = arr[x][y];
    }
    else{
        val = temp[x][y];
    }

    queue<pair<int,int>> q;
    q.push({x,y});
    ways.push_back({x,y});
    visited[x][y] = true;
    int cnt =1;
    while(q.size()){
        auto cord = q.front(); q.pop();
        int curX = cord.first;
        int curY = cord.second;
        For(i,0,4){
            int qx = curX + dx[i];
            int qy = curY + dy[i];

            if(out(qx,qy) || visited[qx][qy]) continue;

            if(real){
                if(arr[qx][qy] != val) continue;
                ways.push_back({qx,qy});
            }
            else{
                if(temp[qx][qy] != val) continue;
            }

            visited[qx][qy] = true;
            cnt++;
            q.push({qx,qy});
        }
    }
    if(real && cnt >= 3){
        for(auto elem : ways){
            disappear[elem.first][elem.second] = true;
        }
    }
    return cnt;
}


//몇개가 겹치는지 count
int getCount(){
    int totCnt = 0;
    memset(visited,false,sizeof(visited));
    For(i,0,5){
        For(j,0,5){
            if(!visited[i][j]){
                int cnt = BFS(i,j,false);
                if(cnt >=3){
                    totCnt += cnt;
                }
            }
        }
    }
    return totCnt;
}

void replace(){
    For(j,0,5){
        for(int i = 4;i>=0;i--){
            if(disappear[i][j]){
                arr[i][j] = ancestor[aIdx++];
                //유물 대입
                disappear[i][j] = false;
            }
        }
    }
}

//진짜 회전
void rRotate(rotCord r){
    int rArr[3][3];
    int rtArr[3][3];
    For(i,0,3){
        For(j,0,3){
            rArr[i][j] = arr[r.x-1+i][r.y-1+j];
        }
    }
    For(a,0, r.rotNum){
        For(i,0,3){
            For(j,0,3){
                rtArr[i][j] = rArr[2-j][i];
            }
        }
        copy(&rtArr[0][0],&rtArr[0][0]+3*3,&rArr[0][0]);
    }
    For(i,0,3){
        For(j,0,3){
            arr[r.x-1+i][r.y-1+j] = rtArr[i][j];
        }
    }
    //여기까지 돌린거고
    //print();
    //실제 반영 후
    bool found;
    int tScore =0;
    do{
        found = false;
        memset(visited,false,sizeof(visited));
        For(i,0,5){
            For(j,0,5){
                if(!visited[i][j]){
                   int cnt = BFS(i,j,true);
                   if(cnt >= 3){
                       tScore+= cnt;
                       found =true;
                   }
                }
            }
        }
        //Disappear 표시
        replace();
        //print();
    }while(found);
    cout << tScore << " ";
    //대체 해준다.

}

//회전 메서드
void rotateArr(int x,int y,int num){

    copy(&arr[0][0],&arr[0][0]+5*5,&temp[0][0]);
    //temp에 복사

    int rArr[3][3];
    int rtArr[3][3];
    For(i,0,3){
        For(j,0,3){
            rArr[i][j] = arr[x-1+i][y-1+j];
        }
    }
    //해당하는 Arr넣고

    For(a,0,num){
        For(i,0,3){
            For(j,0,3){
                rtArr[i][j] = rArr[2-j][i];
            }
        }
         copy(&rtArr[0][0],&rtArr[0][0]+3*3,&rArr[0][0]);
    }
    //a번 돌리고
    For(i,0,3){
        For(j,0,3){
            temp[x-1+i][y-1+j] = rtArr[i][j];
        }
    }
    //temp에 실제로 대입 후

    int score = getCount();
    //cnt가 0보다 크다면 순위에 넣는다.
    if(score > 0){
        vec.push_back({x,y,score,num});
    }
}

bool play(){
    For(i,1,4){
        For(j,1,4){
            For(r,1,4){
                rotateArr(i,j,r);
                //돌려서 모두 vector에 때려 넣는다.
            }
        }
    }
    sort(vec.begin(),vec.end());
    //아무 유물도 못 얻었다면

    if(vec.empty()){
        return false;
    }
    //실제로 돌리고 유물 채움 + 좌표 초기화

    rRotate(vec[0]);
    vec.clear();
    //좌표 비워준다.
    return true;
}


void input(){
    cin >> K >> M;
    For(i,0,5){
        For(j,0,5){
            cin >> arr[i][j];
        }
    }
    For(i,0,M){
        cin >> ancestor[i];
    }
}

void solve(){
    input();
    For(i,0,K){
        if(!play()){
            break;
        }
    }
}

int main(){
    fastIO();
    solve();

    return 0;
}
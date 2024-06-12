//
// Created by newbi on 2024-06-12.
//
#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#define endl '\n'
#define fastIO() ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
#define For(i,a,b) for(int i=a;i<b;i++)

using namespace std;

const int dx[] = {-1,0,1,0};
const int dy[] = {0,1,0,-1};
//위 오 아 왼

int arr[41][41];
//장애물
int kArr[41][41];
int kTemp[41][41];
// 기사 위치, 복사될 배열

queue<pair<int,int>> kOrder;

struct Knight{
    int x;
    int y;
    int h;
    int w;
    int hp;
    bool life = true;
    bool touched = false;
    int dmg = 0;

    Knight(int x,int y,int h,int w,int hp):x(x),y(y),h(h),w(w),hp(hp){

    }
    void printInfo(){
        cout << "HP: " << hp <<" Life : "<< life <<" DMG : "<< dmg <<endl;
    }
};
vector<Knight> vec;
vector<Knight> tVec;
int L,N,Q;

bool out(int x,int y){
    return x < 1 || y < 1 || x > L || y > L;
}

void tPrint(){
    for(auto elem : tVec){
        elem.printInfo();
    }
}

void print(){
    For(i,1,L+1){
        For(j,1,L+1){
            cout << kArr[i][j] <<" ";
        }
        cout << endl;
    }
    cout << endl;

    for(auto elem : vec){
        elem.printInfo();
    }
}

void input(){
    int r,c,h,w,k;
    cin >> L >> N >> Q;
    For(i,1,L+1){
        For(j,1,L+1){
            cin >> arr[i][j];
        }
    }
    For(i,0,N){
        cin >> r >> c >> h >> w >> k;
        vec.push_back(Knight(r,c,h,w,k));
        For(a,r,r+h){
            For(b,c,c+w){
                kArr[a][b] = i+1;
            }
        }
        //기사 위치 표시
    }
    int no,dir;
    For(i,0,Q){
        cin >> no >> dir;
        kOrder.push({no,dir});
    }
}

//no가 들어온다
bool DFS(int no,int dir){

    bool ret = true;

    auto curK = tVec[no-1];
    int curX = curK.x;
    int curY = curK.y;
    int curH = curK.h;
    int curW = curK.w;

    //건드린 기사 목록
    set<int> touch;

    //현재 기사위치 비우고
    For(i,curX,curX+curH){
        For(j,curY,curY+curW){
            kTemp[i][j] = 0;
        }
    }

    curX += dx[dir];
    curY += dy[dir];
    //이동할 위치

    For(i,curX,curX+curH){
        For(j,curY,curY+curW){
            //나갔거나 벽에 부딪히면 움직이면 안된다.
            if(out(i,j) || arr[i][j] == 2){
                ret = false;
                return ret;
            }
            else if(kTemp[i][j]){
                //기사가 있다면
                touch.insert(kTemp[i][j]);
                //ret = DFS(kTemp[i][j],dir);
            }

        }
    }

    for(auto t : touch){
        //접촉한 애들 도 민다.
        tVec[t-1].touched = true;
        ret = DFS(t,dir);
        if(!ret) break;
        //하나라도 false가 뜨면 break
    }
    //실제로 구조체안의 기사 정보 업데이트
    tVec[no-1].x = curX;
    tVec[no-1].y = curY;

    For(i,curX,curX+curH){
        For(j,curY,curY+curW){
            kTemp[i][j] = no;
        }
    }

    return ret;
}

void solve(){
    while(kOrder.size()){

        auto ord = kOrder.front(); kOrder.pop();
        int no = ord.first;
        int dir = ord.second;
        if(!vec[no-1].life) continue;
        //기사가 죽었다면 SKIP
        //기사 정보 복사
        tVec = vec;
        copy(&kArr[0][0],&kArr[0][0]+41*41,&kTemp[0][0]);

        //움직일 수 있다면
        if(DFS(no,dir)){
            //cout << "isOK" << endl;
            copy(&kTemp[0][0],&kTemp[0][0]+41*41,&kArr[0][0]);
            vec = tVec;
            //실제로 반영
            For(i,0,N){
                if(!vec[i].life || vec[i].touched == false) continue;
                int dmg =0;
                int curX = vec[i].x;
                int curY = vec[i].y;
                int curW = vec[i].w;
                int curH = vec[i].h;

                For(a,curX,curX+curH){
                    For(b,curY,curY+curW){
                        if(arr[a][b] == 1) dmg++;
                    }
                }

                vec[i].dmg += dmg;
                if(vec[i].hp <= vec[i].dmg){
                    vec[i].life = false;
                    //비워준다.
                    For(a,curX,curX+curH){
                        For(b,curY,curY+curW){
                           kArr[a][b] = 0;
                        }
                    }
                }
                //데미지가 더 심하면 죽음 처리
                vec[i].touched = false;
            }
        }
       //print();
    }

    int ans =0;
    For(i,0,N){
        if(vec[i].life){
            ans += vec[i].dmg;
        }
    }
    cout << ans << endl;
}

int main(){
    fastIO();
    input();
    solve();
    return 0;
}
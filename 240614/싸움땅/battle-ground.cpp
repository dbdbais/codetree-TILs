//
// Created by newbi on 2024-06-14.
//
#include <iostream>
#include <vector>
#include <algorithm>
#define fastIO() ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
#define For(i,a,b) for(int i=a;i<b;i++)
#define endl '\n'

using namespace std;

struct Player{
    int x;
    int y;
    int d;
    int s;
    int gun = 0;
    int _score = 0;

    void printInfo(){
        cout << "x : "<< x << " y : "<< y << " d : "<< d << " s: "<< s <<" gun : "<<gun <<" score : "<< _score << endl;
    }
};

const int dx[4] = {-1,0,1,0};
const int dy[4] = {0,1,0,-1};
vector<int> arr[22][22];
//총의 개수가 담길..
int pArr[22][22];
Player pVec[31];
int N, M, K;

bool out(int x,int y){
    return x < 1 || y < 1 || x >N || y > N;
}

void output(){
    cout << "GUN "<<endl;
    For(i,1,N+1){
        For(j,1,N+1){
            cout << arr[i][j].front() << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "Player "<<endl;
    For(i,1,N+1){
        For(j,1,N+1){
            cout << pArr[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    For(i,1,M+1){
        cout<<i <<"th ";
        pVec[i].printInfo();
    }
}

void input(){
    cin >> N >> M >> K;
    int tmp,x,y,d,s;
    For(i,1,N+1){
        For(j,1,N+1){
            cin >> tmp;
            arr[i][j].push_back(tmp);
        }
    }
    For(i,1,M+1){
        cin >> x >> y >> d >> s;
        pArr[x][y] = i;
        pVec[i] = {x,y,d,s};
    }
}

int backWard(int dir){
    if(dir == 0){
        return 2;
    }
    else if(dir == 1){
        return 3;
    }
    else if(dir == 2){
        return 0;
    }
    else if(dir == 3){
        return 1;
    }
}
// 싸운다.
void fight(int o, int h){
    Player op = pVec[o];
    Player hp = pVec[h];

    int oFire = op.gun + op.s;
    int hFire = hp.gun + hp.s;
    int w,l;
    if(oFire > hFire){
        w = o;
        l = h;
    }
    else if (oFire < hFire){
        w = h;
        l = o;
    }
    else{
        if(op.s > hp.s){
            w = o;
            l = h;
        }
        else{
            w = h;
            l = o;
        }
    }
    Player & winner = pVec[w];
    Player & loser = pVec[l];
    //승자랑 패자 뽑고

    winner._score += abs(oFire - hFire);
    //점수 추가하고
    pArr[winner.x][winner.y] = w;
    //최종 자리는 승자가 가져감
    //숫자 업데이트
    if(loser.gun != 0){
        arr[loser.x][loser.y].push_back(loser.gun);
        loser.gun = 0;
        //총을 버림
    }

    For(i,0,4) {
        int lx = loser.x + dx[(loser.d+i) % 4];
        int ly = loser.y + dy[(loser.d+i) % 4];

        //나갔거나 누군가 있다면
        if (out(lx, ly) || pArr[lx][ly]) {
            continue;
        }
        loser.d = (loser.d +i) %4;
        //비울 필요 없이 떠나면 됨
        loser.x = lx;
        loser.y = ly;
        pArr[lx][ly] = l;
        sort(arr[lx][ly].begin(),arr[lx][ly].end());
        int bGun = arr[lx][ly].back();
        if(bGun > loser.gun){
            arr[lx][ly].pop_back();
            arr[lx][ly].push_back(loser.gun);
            loser.gun = bGun;
        }
        break;
    }

    if(arr[winner.x][winner.y].size()){
        sort(arr[winner.x][winner.y].begin(),arr[winner.x][winner.y].end());
        int bGun = arr[winner.x][winner.y].back();
        if(bGun > winner.gun){
            arr[winner.x][winner.y].pop_back();
            arr[winner.x][winner.y].push_back(winner.gun);
            winner.gun = bGun;
        }
        //제일 좋은 총으로 업데이트
    }
}
void playerMove(){
    For(i,1,M+1){
        Player &cur = pVec[i];
        int cx = cur.x;
        int cy = cur.y;
        int cd = cur.d;
        //이전 값들
        int qx = cur.x + dx[cd];
        int qy = cur.y + dy[cd];

        if(out(qx,qy)){
            cd = backWard(cd);
            qx = cur.x + dx[cd];
            qy = cur.y + dy[cd];
            cur.d = cd;
        }
        //나갈경우 반대로 처리
        pArr[cx][cy] = 0;
        cur.x = qx;
        cur.y = qy;
        //실제 좌표 업데이트
        //다른 플레이어가 있다면 싸우고
        if(pArr[qx][qy]){
            //일단 비우고
            fight(i,pArr[qx][qy]);
        }
        //없다면 총을 집어든다.
        else{
            //총이 있다면
            if(arr[cur.x][cur.y].size()){
                sort(arr[cur.x][cur.y].begin(),arr[cur.x][cur.y].end());
                int bGun = arr[cur.x][cur.y].back();
                if(cur.gun < bGun){
                    arr[qx][qy].pop_back();
                    arr[qx][qy].push_back(cur.gun);
                 //총 내려놓고
                 cur.gun = bGun;
                 //새로운 총 든다.
                }
            }
            pArr[qx][qy] = i;
            //플레이어 배열 이동
        }


    }
}
void printScore(){
    For(i,1,M+1){
        cout << pVec[i]._score <<" ";
    }
    cout << endl;
}

void solve(){
    while(K){
        //output();
        playerMove();
        K--;
    }
    printScore();
}

int main(){
    fastIO();
    input();
    solve();
    //output();
    return 0;
}
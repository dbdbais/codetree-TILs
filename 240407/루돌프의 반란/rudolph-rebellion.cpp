//
// Created by newbi on 2024-04-07.
//
#include <iostream>
#include <vector>
#include <algorithm>
#define endl '\n'
#define fastIO() ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
#define For(i,a,b) for(int i=a;i<b;i++)
using namespace std;

const int R = -1000;
int visited[51][51];
int N,M,P,C,D;
pair<int,int> rudolph;

struct Santa{
    int no;
    int x;
    int y;
    int score = 0;
    bool life = true;
    int stun = 0;

    Santa(int no,int x,int y):no(no),x(x),y(y){}

    bool operator<(Santa s){    //번호 순 대로 정렬
        return no < s.no;
    }
};
const int sx[4] = {0,0,-1,1};
const int sy[4] = {-1,1,0,0};

const int rx[8] = {0,0,-1,1,1,1,-1,-1};
const int ry[8] = {1,-1,0,0,1,-1,1,-1};
vector<Santa> vec;

int getDistance(int x1,int y1,int x2,int y2){
    int height = abs(x1 - x2);
    int width = abs(y1 - y2);
    return height * height + width * width;
}

bool out(int x,int y){
    return x <1 || y < 1 || x >N || y > N;
}

void print(){
    For(i,1,N+1){
        For(j,1,N+1){
            if(visited[i][j] == R){
                cout <<"R ";
            }
            else{
                cout << visited[i][j] <<" ";
            }

        }
        cout << endl;
    }
    cout << endl;
}

void incScore(){
    for(Santa &s : vec){
        if(s.life){
            s.score++;
        }
    }
}

void printScore(){
    for(Santa s : vec){
        cout << s.score << " ";
    }
    cout << endl;
}

bool checkDead(){
    for(Santa s : vec){
        if(s.life) return false;
    }
    return true;
}


bool custom(Santa a,Santa b){
    int aDist = getDistance(rudolph.first,rudolph.second,a.x,a.y);
    int bDist = getDistance(rudolph.first,rudolph.second,b.x,b.y);

    if(aDist == bDist){
        if(a.x == b.x){
            return a.y > b.y;
        }
        else{
            return  a.x > b.x;
        }
    }
    else {
        return aDist < bDist;   //루돌프와 가까운 순으로 정렬
    }
}

bool wayCustom(pair<int,int> a, pair<int,int> b){
    int aDist = getDistance(rudolph.first,rudolph.second,a.first,a.second);
    int bDist = getDistance(rudolph.first,rudolph.second,b.first,b.second);

    if(aDist == bDist){
        if(a.second == b.second){   //열이 같을 때
            return a.first < b.first;    //행이 작은 걸 리턴(상 하)

        }
        else if(a.first == b.first){ //행이 같을 때
            return a.second > b.second; //열이 큰 걸 리턴 (우 좌)
        }
        else{   //상하 - 좌우간 비교
            if(a.first < b.first){ //상과 좌우간 비교
                return a.first < b.first;
            }
            else{  //하와 좌우간 비교
                return a.second > b.second; //열이 큰걸 리턴
            }
        }
    }
    return aDist < bDist;

}

void chainCrash(int x,int y,pair<int,int> dir){
    for(Santa & s : vec){
        if(!s.life) continue;
        if(s.x == x && s.y == y){
            int tmpX = s.x + dir.first;
            int tmpY =  s.y + dir.second;
            if(out(tmpX,tmpY)){
                s.life = false;
                break;
            }
            if(visited[tmpX][tmpY]>0){
                chainCrash(tmpX,tmpY,dir);
            }
            s.x =tmpX;
            s.y = tmpY;
            visited[s.x][s.y] = s.no;
            break;
        }
    }
}

void rudolphMove(int t){
    sort(vec.begin(),vec.end(),custom);

    int fx,fy,nearDist = 1e9;

    /*
    for(auto elem : vec){
        cout<<"No." << elem.no << " ";
    }
*/

    for(Santa target : vec){   //가장 가까운 샅나 찜
        if(!target.life) continue;
       // cout<<"target: " << target.no << endl;
        For(i,0,8){
            int qx = rudolph.first + rx[i];
            int qy = rudolph.second + ry[i];
            if(out(qx,qy)) continue;
            int tmpDist = getDistance(qx,qy,target.x,target.y); //이동한 위치와 타겟의 위치 측정
            if(nearDist > tmpDist ){    //가장 가까운 거리라면 갱신
                fx = qx;
                fy = qy;
                nearDist = tmpDist;
                //cout << "fx : " << fx <<"fy : " << fy << endl;
            }
        }
        break;
    }

    //충돌날 시 처리
    if(visited[fx][fy]>0) {   //누군가 방문 하고 있다면
        for(Santa & s : vec){
            if(!s.life) continue;
            if(s.x == fx && s.y== fy){  //충돌한 산타라면
                s.score += C;   //점수 C점
                visited[s.x][s.y] = 0;
                pair<int,int> diff;
                diff.first = fx - rudolph.first;
                diff.second = fy - rudolph.second;
                int tmpX =s.x + diff.first * C;
                int tmpY = s.y + diff.second * C;

                //밀려나고
                if(out(tmpX,tmpY)){   //장외 시 탈락
                    s.life = false;
                   // cout << s.no <<" dead"<<endl;
                    break;
                }
                if(visited[tmpX][tmpY]>0){ //누군가 있었다면
                    chainCrash(tmpX,tmpY,diff);
                }
                s.x = tmpX;
                s.y =tmpY;
                visited[s.x][s.y] = s.no;
                s.stun = t+2; //기절
                break;
            }
        }
    }
    visited[rudolph.first][rudolph.second] = 0;
    //이동 후
    rudolph.first = fx;
    rudolph.second = fy;
   //cout << "R Move" << rudolph.first <<rudolph.second<<endl;
    visited[rudolph.first][rudolph.second] = R;

   // print();

}

void santaMove(int t){
    sort(vec.begin(),vec.end());    //산타 번호 순 정렬하고
    for(Santa & s : vec){
        if(!s.life) continue;
        if(s.stun>t){
           // cout << s.no <<"Stunned"<<endl;
            continue;
        }
        //cout << "Santa : "<< s.no << "move" << endl;
        vector<pair<int,int>> ways;
        int curDist = getDistance(s.x,s.y,rudolph.first,rudolph.second);    //현재 거리
        For(i,0,4){
            int qx = s.x + sx[i];
            int qy = s.y + sy[i];
            if(out(qx,qy) || visited[qx][qy]>0 ) continue;
            int nextDist = getDistance(qx,qy,rudolph.first,rudolph.second); //다음의 거리
            if(nextDist < curDist){     //더 가까워진다면
                ways.push_back({qx,qy});    //거리 후보로 넣기
            }
        }
        if(ways.size()){    //후보 거리가 있다면
            sort(ways.begin(),ways.end(), wayCustom);
            visited[s.x][s.y] = 0;
            if(ways[0].first == rudolph.first && ways[0].second == rudolph.second){  //루돌프와 충돌했다면
                //cout << "crashed"<< endl;
                s.score += D;
                pair<int,int> diff;
                diff.first = s.x - rudolph.first;
                diff.second = s.y - rudolph.second;
                int tmpX = rudolph.first + diff.first * D;
                int tmpY = rudolph.second + diff.second *D;

                if(out(tmpX,tmpY)){   //장외 시 탈락
                    s.life = false;
                    //cout << s.no <<" dead"<<endl;
                    continue;
                }
                if(visited[tmpX][tmpY]>0){ //누군가 있었다면
                    chainCrash(tmpX,tmpY,diff);
                }
                s.x = tmpX;
                s.y = tmpY;
                visited[s.x][s.y] = s.no;
                s.stun = t+2; //기절
            }
            else{
                s.x = ways[0].first;
                s.y = ways[0].second;
                //cout << "MOve to "<< s.x << s.y << endl;
                visited[s.x][s.y] = s.no;
            }
        }
        else{
           // cout << "no Way to go" << endl;
        }
        //print();
    }
}

void solve(){
    int t = 0;
    while(t < M){
        rudolphMove(t);
        santaMove(t);
        incScore();
        if(checkDead()) break;
        t++;

    }
    printScore();
}


int main(){
    fastIO();
    cin >> N >> M >> P >> C >> D;
    cin >> rudolph.first >> rudolph.second;
    visited[rudolph.first][rudolph.second] = R;
    int sno,_sx,_sy;
    For(i,0,P){
        cin >> sno >> _sx >> _sy;
        vec.push_back(Santa(sno,_sx,_sy));
        visited[_sx][_sy] = sno;
    }
    //print();
    solve();

    return 0;
}
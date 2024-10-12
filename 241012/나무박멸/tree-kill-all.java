import java.util.*;
import java.io.*;
import java.awt.*;

public class Main{
	static BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
	static StringTokenizer st;
	static int N,M,K,C,answer;
	static int [][] arr;
	static int [][] dead;
	static boolean [][] tree;
	static boolean [][] wall;
	static int [] dx = {0,0,-1,1};
	static int [] dy = {-1,1,0,0};
	
	static int [] bdx = {1,1,-1,-1};
	static int [] bdy = {1,-1,1,-1};
	
	static class Cord{
		int x;
		int y;
		int dir;
		int k;
		public Cord(int x, int y, int dir,int k) {
			super();
			this.x = x;
			this.y = y;
			this.dir = dir;
			this.k = k;
		}
		
	}
	
	static boolean out(int x,int y) {
		return x < 0 || y < 0 || x >= N || y >=N;
	}
	
	static void grow() {
		for(int i=0;i<N;i++) {
			for(int j=0;j<N;j++) {
				if(tree[i][j]) {
					int cnt = 0;
					for(int a=0;a<4;a++) {
						int qx = i + dx[a];
						int qy = j + dy[a];
						
						if(out(qx,qy)|| wall[qx][qy] || !tree[qx][qy]) continue;
						cnt++;
					}
					arr[i][j] += cnt;
					//나무 증가
				}
			}
		}
	}
	
	static void breed() {
		int [][] tmpArr = new int[N][N];
		
		for(int i=0;i<N;i++) {
			for(int j=0;j<N;j++) {
				int cnt =0;
				if(tree[i][j] && dead[i][j] == 0) {
					for(int a =0; a< 4; a++) {
						int qx = i + dx[a];
						int qy = j + dy[a];
						if(out(qx,qy)||wall[qx][qy] || tree[qx][qy] || dead[qx][qy] > 0) continue;
						cnt++;
					}
					if(cnt >0) {
						int nut = arr[i][j] / cnt;
						
						for(int a =0; a< 4; a++) {
							int qx = i + dx[a];
							int qy = j + dy[a];
							if(out(qx,qy)||wall[qx][qy] || tree[qx][qy]|| dead[qx][qy] > 0) continue;
							tmpArr[qx][qy] += nut;
						}
					}
				}
			}
		}
		
		for(int i=0;i<N;i++) {
			for(int j=0;j<N;j++) {
				if(tmpArr[i][j] > 0) {
					if(!tree[i][j]) tree[i][j] = true;
					arr[i][j] += tmpArr[i][j];
				}
			}
		}
		//옮기는 작업
	}
	
	static void realToxic() {
		
		int fx = 0;
		int fy = 0;
		int cnt = 0;
		for(int i=N-1;i>=0;i--) {
			for(int j=N-1;j>=0;j--) {
				if(tree[i][j]) {
					int tcnt = toxicCnt(i,j);
					if(tcnt >= cnt) {
						fx =i;
						fy =j;
						cnt = tcnt;	//최대 값 갱신
					}
				}
			}
		}
	
		toxic(fx,fy);
		answer += cnt;
		
	}
	
	static int toxicCnt(int x,int y) {
		int cnt =0;
		boolean [][] tmpVisited = new boolean[N][N];
		
		Queue<Cord> q = new ArrayDeque<>();
		
		q.add(new Cord(x,y,0,K));
		q.add(new Cord(x,y,1,K));
		q.add(new Cord(x,y,2,K));
		q.add(new Cord(x,y,3,K));
		
		tmpVisited[x][y] = true;
		cnt += arr[x][y];
		while(!q.isEmpty()) {
			Cord cur = q.poll();
			
			int qdir = cur.dir;
			int qx = cur.x + bdx[qdir];
			int qy = cur.y + bdy[qdir];
			int qk = cur.k;
			
			
			
			if(out(qx,qy) || wall[qx][qy]|| tmpVisited[qx][qy]||!tree[qx][qy]||arr[qx][qy] == 0 || dead[qx][qy] > 0) continue;
			
			cnt += arr[qx][qy];
			tmpVisited[qx][qy] = true;
			if(qk > 1) {
				q.add(new Cord(qx,qy,qdir,qk-1));
			}
	
		}
		
		return cnt;
	}
	
	static void toxic(int x,int y) {
		boolean [][] tmpVisited = new boolean[N][N];
		
		Queue<Cord> q = new ArrayDeque<>();
		
		q.add(new Cord(x,y,0,K));
		q.add(new Cord(x,y,1,K));
		q.add(new Cord(x,y,2,K));
		q.add(new Cord(x,y,3,K));
		
		tmpVisited[x][y] = true;
		dead[x][y] = C+1;
		tree[x][y] = false;
		arr[x][y] = 0;
		while(!q.isEmpty()) {
			Cord cur = q.poll();
			
			int qdir = cur.dir;
			int qx = cur.x + bdx[qdir];
			int qy = cur.y + bdy[qdir];
			int qk = cur.k;
			
			if(out(qx,qy) || wall[qx][qy]|| tmpVisited[qx][qy] ) continue;
			
			if(arr[qx][qy] == 0) {
				dead[qx][qy] =  C+1;
				tmpVisited[qx][qy] = true;
				continue;
			}
			
			dead[qx][qy] =  C+1;;
			tree[qx][qy] = false;
			arr[qx][qy] = 0;
			tmpVisited[qx][qy] = true;
			
			if(qk > 1) {
				q.add(new Cord(qx,qy,qdir,qk-1));
			}
		}
	}
	
	static void print() {
		System.out.println("ARR");
		for(int i=0;i<N;i++) {
			for(int j=0;j<N;j++) {
				System.out.print(arr[i][j]+" ");
			}
			System.out.println();
		}
		System.out.println();
		
		System.out.println("DEAD");
		for(int i=0;i<N;i++) {
			for(int j=0;j<N;j++) {
				System.out.print(dead[i][j]+" ");
			}
			System.out.println();
		}
		System.out.println();
	}
	
	static void decrease() {
		for(int i=0;i<N;i++) {
			for(int j=0;j<N;j++) {
				if(dead[i][j]>0) {
					dead[i][j]--;
				}
			}
		}
	}
	
	static void solve() {
		
		while(M-- > 0) {
			decrease();
			//print();
			//System.out.println("GROW");
			grow();
			//print();
			//System.out.println("BREED");
			breed();
			//print();
			//System.out.println("TOXIC");
			realToxic();
			//print();			
			//System.out.println("answer :" + answer);
			
		}
		
	}
	
	public static void main(String[] args) throws IOException {
		st = new StringTokenizer(in.readLine());
		N = Integer.parseInt(st.nextToken());
		M = Integer.parseInt(st.nextToken());
		K = Integer.parseInt(st.nextToken());
		C = Integer.parseInt(st.nextToken());
		
		arr = new int[N][N];
		tree = new boolean[N][N];
		wall = new boolean[N][N];
		dead = new int [N][N];
		
		for(int i=0;i<N;i++) {
			st = new StringTokenizer(in.readLine());
			for(int j=0;j<N;j++) {
				arr[i][j] = Integer.parseInt(st.nextToken());
				if(arr[i][j] > 0) tree[i][j] = true;
				else if(arr[i][j] < 0) wall[i][j] = true;
				//벽이거나 나무가 있다면
			}
		}
		//나무 입력받고
		
		
		solve();
		System.out.println(answer);
	}
	
	
	
}
import java.awt.Point;
import java.io.*;
import java.util.*;


public class Main {
	static BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
	static StringTokenizer st;
	static int R,C,K,answer;
	
	static final int[] dx = {-1,0,1,0};
	static final int[] dy = {0,1,0,-1};
	
	static boolean [][] mVisited;
	static boolean [][] visited;
	static boolean [][] gate;
	static int [][] mark;
	static int[] score;
	
	static ArrayList<Golem> lst;
	
	static boolean out(int x,int y) {
		return x < 0 || y < 0 || x >=R+3 || y >= C;
	}
	static boolean mout(int x,int y) {
		return out(x,y) || visited[x][y];
	}
	
	static class Golem{
		int x;
		int y;
		int exit;
		boolean stuck = false;
		//0 상 우 하 좌
		public Golem(int x, int y, int exit) {
			this.x = x;
			this.y = y;
			this.exit = exit;
		}
		boolean canDown() {
			return !mout(x+2,y) && !mout(x+1,y-1) && !mout(x+1,y+1);
		}
		boolean canLeft() {
			return !mout(x,y-2) && !mout(x-1,y-1) && !mout(x+1,y-1) && !mout(x+1,y-2)&& !mout(x+2,y-1); 
		}
		boolean canRight() {
			return !mout(x,y+2) && !mout(x-1,y+1) && !mout(x+1,y+1) && !mout(x+1,y+2)&& !mout(x+2,y+1); 
		}
		boolean isStuck() {
			return x <= 2;
		}
		void goDown() {
			this.x+=1;
		}
		void goLeft() {
			this.x+=1;
			this.y-=1;
			exit = exit-1;
			if(exit < 0) exit =3;
		}
		void goRight() {
			this.x+=1;
			this.y+=1;
			exit = (exit +1) % 4;
		}
		void isDone(int num) {
			visited[x][y] = true;
			mark[x][y] = num;
			for(int i=0;i<4;i++) {
				int qx = x + dx[i];
				int qy = y + dy[i];
				visited[qx][qy] = true;
				mark[qx][qy] = num;
			}
			gate[x+dx[exit]][y+dy[exit]] = true;
			
		}
		
		@Override
		public String toString() {
			return "Golem [x=" + x + ", y=" + y + ", exit=" + exit + "]";
		}
	}
	
	static void resetMap() {
		for(int i=0;i<R+3;i++) {
			Arrays.fill(visited[i], false);
			Arrays.fill(mark[i], 0);
			Arrays.fill(gate[i], false);
		}
	}
	
	static void drop(int num) {
		Golem cur = lst.get(num);
		
		while(true) {
			if(cur.canDown()) {
				cur.goDown();
			}
			else if(cur.canLeft()) {
				cur.goLeft();
			}
			else if(cur.canRight()) {
				cur.goRight();
			}
			else if(cur.isStuck()) {
				//System.out.println("STUCK");
				cur.stuck = true;
				resetMap();
				break;
				//맵 날린다.
			}
			else {
				cur.isDone(num);
				//System.out.println(cur);
				break;
			}
		}
		
		if(!cur.stuck) {
			mVisited = new boolean [R+3][C];
			
			int mx = cur.x;
			
			Queue<Point> q = new ArrayDeque<>();
			q.add(new Point(cur.x,cur.y));
			mVisited[cur.x][cur.y] = true;
			
			while(!q.isEmpty()) {
				Point cp = q.poll();
				int cx = cp.x;
				int cy = cp.y;
				
				if(mx < cx) mx = cx;
				
				boolean isGate = gate[cx][cy];
				//게이트의 위치인지
				if(isGate) {
					for(int i=0;i<4;i++) {
						int qx = cx + dx[i];
						int qy = cy + dy[i];
						
						if(out(qx,qy)||mVisited[qx][qy]|| !visited[qx][qy]) continue;
						
						if(visited[qx][qy]) {
							mVisited[qx][qy] = true;
							q.add(new Point(qx,qy));
						}
					}
				}
				else {
					//게이트가 아니라면
					for(int i=0;i<4;i++) {
						int qx = cx + dx[i];
						int qy = cy + dy[i];
						
						if(out(qx,qy)||mVisited[qx][qy]|| !visited[qx][qy]) continue;
						//나가거나 이미 방문한 곳, 골렘의 영역이 아니라면
						int mNum = mark[cx][cy];
						if(mark[qx][qy] == mNum) {
							mVisited[qx][qy] = true;
							q.add(new Point(qx,qy));
						 }
						//나의 번호	
					}
				}
			}
			score[num] = mx-2;
		}
		
	}
	
	public static void main(String[] args) throws IOException {
		
		st = new StringTokenizer(in.readLine());
		
		R = Integer.parseInt(st.nextToken());
		C = Integer.parseInt(st.nextToken());
		K = Integer.parseInt(st.nextToken());
		
		visited = new boolean[R+3][C];
		gate = new boolean[R+3][C];
		mark = new int[R+3][C];
		score = new int[K];
		lst = new ArrayList<>();
		
		for(int i=0;i<K;i++) {
			st = new StringTokenizer(in.readLine());
			int c = Integer.parseInt(st.nextToken());
			int direc = Integer.parseInt(st.nextToken());
			lst.add(new Golem(0,c-1,direc));
			//넣고
			drop(i);
		}
		
		for(int i=0;i<K;i++) {
			answer += score[i];
			//System.out.println(score[i]+" ");
		}
		
		System.out.println(answer);
	}

	

}
import java.util.*;
import java.io.*;
import java.awt.*;

public class Main {

	static BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
	static StringTokenizer st;
	static int N,M,wSum,wRock,wAtk;
	static Point endPoint = new Point();
	static Medusa medusa = new Medusa();
	static int[][] arr;
	//맵 정보 도로는 0, 아닌 곳은 1
	static boolean[][] sight;
	static ArrayList<Integer> [][] wArr;
	// 전사 2차원 배열
	static Warrior [] wLst;
	
	static final int[] dx = {-1,1,0,0};
	static final int[] dy = {0,0,-1,1};
	
	static final int[][] fdx = {{-1,-1,-1},{1,1,1},{-1,0,1},{-1,0,1}};
	static final int[][] fdy = {{-1,0,1},{-1,0,1},{-1,-1,-1},{1,1,1}};
	//상하좌우
	
	static StringBuilder sb = new StringBuilder();
	
	static boolean out(int x,int y) {
		return x <0 || y< 0 || x >= N || y >= N ;
	}
	
	static int getShoretestPathBFS(int x,int y) {
		int [][] visited = new int[N][N];
		Queue<Point> q = new ArrayDeque<>();
		
		visited[x][y] = 1;
		
		q.add(new Point(x,y));
		
		while(!q.isEmpty()) {
			
			Point cur = q.poll();
			
			int val = visited[cur.x][cur.y];
			
			for(int i=0;i<4;i++) {
				
				int qx = cur.x + dx[i];
				int qy = cur.y + dy[i];
				
				if(out(qx,qy)||visited[qx][qy] != 0 ||arr[qx][qy] == 1) continue;
				
				visited[qx][qy] = val+1;
				
				q.add(new Point(qx,qy));
				
			}
		}
	
		
		int dest = visited[endPoint.x][endPoint.y];
		
		
		if(dest == 0) {
			return Integer.MAX_VALUE;
		}
		else {
			return dest-1;
		}
	}
	
	static int getShortestPath(int x1,int y1,int x2,int y2) {
		return Math.abs(x2-x1) + Math.abs(y2-y1);
	}
	
	//석회화 된 용사들
	static class Rock implements Comparable<Rock>{
		int dir;
		int num;
		ArrayList<Integer> lst;
		ArrayList<Point> mSight;

		public Rock(int dir, int num, ArrayList<Integer> lst, ArrayList<Point> mSight) {
			super();
			this.dir = dir;
			this.num = num;
			this.lst = lst;
			this.mSight = mSight;
		}

		@Override
		public int compareTo(Main.Rock o) {
			if(num == o.num) {
				return Integer.compare(dir, o.dir);
			}
			return Integer.compare(num, o.num)*-1;
		}

		@Override
		public String toString() {
			return "Rock [dir=" + dir + ", num=" + num + ", lst=" + lst + ", mSight=" + mSight + "]";
		}
		
		
	}
	
	static int[] getShadeDirection(int dir,Point wPoint) {
		
		int diff;
		
		if(dir <= 1) {
			//상하
			diff = wPoint.y - medusa.y;
		}
		else {
			//좌우
			diff = wPoint.x - medusa.x;
		}
		if(diff < 0) {
			return new int[] {0,1};
		}
		else if(diff > 0) {
			return new int[] {1,2};
		}
		else {
			return new int[] {1};
		}
	}
	
	
	static class Medusa{
		int x;
		int y;
		
		
		
		@Override
		public String toString() {
			return "Medusa [x=" + x + ", y=" + y + "]";
		}

		public boolean isDone() {
			return x == endPoint.x && y == endPoint.y;
		}
		
		public boolean canGo() {
			boolean [][] visited = new boolean[N][N];
			
			Queue<Point> q = new ArrayDeque<>();
			
			q.add(new Point(x,y));
			visited[x][y] = true;
			
			while(!q.isEmpty()) {
				Point cur = q.poll();
				
				for(int i=0;i<4;i++) {
					int qx = cur.x + dx[i];
					int qy = cur.y + dy[i];
					
					if(out(qx,qy)||visited[qx][qy]||arr[qx][qy]== 1) continue;
					
					if(qx == endPoint.x && qy == endPoint.y) return true;
					
					visited[qx][qy] = true;
					//방문 처리하고
					q.add(new Point(qx,qy));
					
				}
			}
			return false;
		}
		
		public void move() {
			int mnVal = Integer.MAX_VALUE;
			int dir = -1;
			for(int i=0;i<4;i++) {
				int qx = x + dx[i];
				int qy = y + dy[i];
				
				if(out(qx,qy)||arr[qx][qy] == 1) continue;
				
				int dist = getShoretestPathBFS(qx, qy);
				if(dist < mnVal) {
					dir = i;
					mnVal = dist;
				}
			}
			
			x = x + dx[dir];
			y = y + dy[dir];
			//실제로 이동
			
			if(!wArr[x][y].isEmpty()) {
				//전사가 있다면
				for(int no : wArr[x][y]) {
					wLst[no].life = false;
					//해당하는 전사 모두 죽인다.
				}
				wArr[x][y].clear();
				
			}
			
		}
		
		public Rock seeWarrior(int dir) {
			
			boolean [][] visited = new boolean[N][N];
			ArrayList<Integer> warriorLst = new ArrayList<>();
			ArrayList<Point> sight = new ArrayList<>();
			
			Queue<Point> q = new ArrayDeque<>();
			// 그냥 큐
			Queue<Point> wq = new ArrayDeque<>();
			// 전사 큐
			
			q.add(new Point(x,y));
			
			while(!q.isEmpty()) {
				Point cur = q.poll();
				
				for(int i=0;i<3;i++) {
					int qx = cur.x + fdx[dir][i];
					int qy = cur.y + fdy[dir][i];
					
					if(out(qx,qy)||visited[qx][qy]) continue;
					//나갔거나, 그림자거나, 이미 방문했다면
					
					visited[qx][qy] = true;
					
					if(!wArr[qx][qy].isEmpty()) {
						//전사가 있다면
						sight.add(new Point(qx,qy));
						for(int wNo : wArr[qx][qy]) {
							warriorLst.add(wNo);
						}
						
						wq.add(new Point(qx,qy));
						
						//전사 큐 돌린다.
						while(!wq.isEmpty()) {
							Point wcur = wq.poll();
							
							int [] way = getShadeDirection(dir, wcur);
							for(int w : way) {
								int wqx = wcur.x + fdx[dir][w];
								int wqy = wcur.y + fdy[dir][w];
								//전사 큐 위치
								if(out(wqx,wqy)||visited[wqx][wqy]) continue;
								
								visited[wqx][wqy] = true;
								wq.add(new Point(wqx,wqy));
							}
							
							
							
						}
						
					}
					else {
						q.add(new Point(qx,qy));
						sight.add(new Point(qx,qy));
					}
				}
			}
			
			return new Rock(dir,warriorLst.size(),warriorLst,sight);
			
			
		}
		
		public void paralyze() {
			
			sight = new boolean[N][N];
			
			PriorityQueue<Rock> pq = new PriorityQueue<>();
			for(int i=0;i<4;i++) {
				pq.add(seeWarrior(i));
				//전사 가장 우선순위대로 뺸다.
			}
			
			Rock bestRock = pq.poll();
			
			//System.out.println(bestRock);
			
			wRock = bestRock.num;
			
			for(int wNo : bestRock.lst) {
				wLst[wNo].stun();
			}
			//전사 1턴동안 마비시킨다.
			for(Point s : bestRock.mSight) {
				sight[s.x][s.y] = true;
			}
			//메두사의 시야 표시
			
			
		}
	}
	
	static class Warrior{
		int x;
		int y;
		int no;
		boolean stun = false;
		boolean life = true;
		public Warrior(int x, int y,int no) {
			this.x = x;
			this.y = y;
			this.no = no;
		}
		
		public void moveInArr(int dir) {
			
			Iterator<Integer> iter = wArr[x][y].iterator();
			while(iter.hasNext()) {
				int ino = iter.next();
				if(ino == no) {
					iter.remove();
					break;
				}
			}
			//System.out.println(no+"번 전사");
			//System.out.println(x+","+y +"에서");
			//이전 배열에서 삭제하고
			x = x + dx[dir];
			y = y + dy[dir];
			//실제로 움직인다.
			//System.out.println(x+","+y +"로 이동");
			wArr[x][y].add(no);
		}
		
		public boolean isNearMedusa() {
			if(x == medusa.x && y == medusa.y) {
				Iterator<Integer> iter = wArr[x][y].iterator();
				while(iter.hasNext()) {
					int ino = iter.next();
					if(ino == no) {
						iter.remove();
						break;
					}
				}
				//지우고
				return true;
			}
			else {
				return false;
			}
			
		}
		
		public void move() {
			int mnVal = getShortestPath(x, y, medusa.x, medusa.y);
			boolean canGo = false;
			int dir = -1;
			for(int i=0;i<4;i++) {
				int qx = x + dx[i];
				int qy = y + dy[i];
				
				if(out(qx,qy)||sight[qx][qy]) continue;
				//나가거나 메두사의 시선이 있다면 SKIP
				
				
				int dist = getShortestPath(qx,qy,medusa.x,medusa.y);
				if(dist < mnVal) {
					dir = i;
					mnVal = dist;
					canGo = true;
				}
			}
			if(!canGo) return;
			//움직일 수 없다면
			
			moveInArr(dir);
			wSum++;
			if(isNearMedusa()) {
				//System.out.println("1번째 스텝에"+no+"번 메두사 공격");
				life = false;
				wAtk++;
				return;
			}
			
			mnVal = getShortestPath(x, y, medusa.x, medusa.y);
			dir = -1;
			canGo = false;
			
			for(int i : new int[]{2,3,0,1}) {
				int qx = x + dx[i];
				int qy = y + dy[i];
				
				if(out(qx,qy)||sight[qx][qy]) continue;
				//나가거나 메두사의 시선이 있다면 SKIP
				
				int dist = getShortestPath(qx,qy,medusa.x,medusa.y);
				if(dist < mnVal) {
					dir = i;
					mnVal = dist;
					canGo = true;
				}
			}
			if(!canGo) return;
			//움직일 수 없다면
			
			moveInArr(dir);
			wSum++;
			//움직일 수 있다면 한번 더 움직인다.
			if(isNearMedusa()) {
				//System.out.println("2번째 스텝에"+no+"번 메두사 공격");
				life = false;
				wAtk++;
				return;
			}
			
		}
		
		public void stun() {
			this.stun = true;
		}
		
		public void died() {
			life = false;
		}
	}
	static void init() {
		wSum = 0;
		wRock = 0;
		wAtk = 0;
	}
	
	static void print() {
		
		System.out.println("Warrior");
		
		for(int i=0;i<N;i++) {
			for(int j=0;j<N;j++) {
				if(!wArr[i][j].isEmpty()) {
					System.out.print(wArr[i][j].get(0)+" ");
				}
				else {
					System.out.print("O ");
				}
				
			}
			System.out.println();
		}
		
	}
	
	static void solve() {
		//System.out.println(medusa);
		if(!medusa.canGo()) {
			System.out.println(-1);
		}
		else {
			while(true) {
				init();
				//값 초기화
				//1 메두사 이동
				medusa.move();
				//System.out.println("MEDUSA MOVE");
				//System.out.println(medusa);
				if(medusa.isDone()) break;
				//2, 메두사 시야
				//System.out.println("MEDUSA PARAL");
				medusa.paralyze();
			
				//print();
				//System.out.println("Warrior MOVE");
				
				for(Warrior w : wLst) {
					if(!w.life) continue;
					if(w.stun) {
						w.stun = false;
						continue;
					}
					w.move();
				}
				//print();
				sb.append(wSum+" "+wRock+" "+ wAtk+"\n");
				
			}
			sb.append(0);
			System.out.println(sb);
			
			
		}
		
		
		
	}
	
	

	public static void main(String[] args) throws Exception {
		st = new StringTokenizer(in.readLine());
		
		N = Integer.parseInt(st.nextToken());
		M = Integer.parseInt(st.nextToken());
		
		arr = new int[N][N];
		
		st = new StringTokenizer(in.readLine());
		medusa.x = Integer.parseInt(st.nextToken());
		medusa.y = Integer.parseInt(st.nextToken());
		
		endPoint.x = Integer.parseInt(st.nextToken());
		endPoint.y = Integer.parseInt(st.nextToken());
		
		wArr = new ArrayList[N][N];
		for(int i=0;i<N;i++) {
			for(int j=0;j<N;j++) {
				wArr[i][j] = new ArrayList<>();
			}
		}
		
		wLst = new Warrior[M];
		
		st = new StringTokenizer(in.readLine());
		for(int i=0;i<M;i++) {
			int tx = Integer.parseInt(st.nextToken());
			int ty = Integer.parseInt(st.nextToken());
			wLst[i] = new Warrior(tx, ty,i);
			wArr[tx][ty].add(i);
		}
		
		for(int i=0;i<N;i++) {
			st = new StringTokenizer(in.readLine());
			for(int j=0;j<N;j++) {
				arr[i][j] = Integer.parseInt(st.nextToken());
			}
		}
		
		solve();
		
	}
}
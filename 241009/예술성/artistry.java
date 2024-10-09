import java.awt.Point;
import java.io.*;
import java.util.*;

public class Main {
	static BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
	static StringTokenizer st;
	static int N,cx,cy,num,ans;
	static int[][] arr;
	static int[][] group;
	static int [] carr;
	static int [] cnt;
	static HashMap<Integer,Integer> map;
	static boolean [][] visited;
	
	static final int dx[] = {0,0,-1,1};
	static final int dy[] = {-1,1,0,0};
	
	static boolean out(int x,int y) {
		return x < 0 || y < 0 || x >= N || y >= N;
	}
	
	static Deque<Integer> dq;
	static Queue<Point> cord [];
	
	static void init() {
		
		for(int i=0;i<cx;i++) {
			dq.add(arr[i][cy]);
		}
		//맨 위에꺼 추가
		for(int i =N-1;i>cy;i--) {
			dq.add(arr[cx][i]);
		}
		//오른쪽 추가
		
		for(int i= N-1 ; i> cx;i--) {
			dq.add(arr[i][cy]);
		}
		//아래 추가
		
		for(int i=0;i<cy;i++) {
			dq.add(arr[cx][i]);
		}
		
	}
	
	static void crossRotate() {
		
		for(int i=0;i<cx;i++) {
			dq.addLast(dq.pollFirst());
		}
		//앞에꺼 n개 빼서 뒤로 넣어야함.
		
		for(int i=0;i<cx;i++) {
			arr[i][cy] = dq.pollFirst();
			dq.addLast(arr[i][cy]);
		}
		//맨 위에꺼 추가
		for(int i =N-1;i>cy;i--) {
			arr[cx][i] = dq.pollFirst();
			dq.addLast(arr[cx][i]);
		}
		//오른쪽 추가
		
		for(int i= N-1 ; i> cx;i--) {
			arr[i][cy] = dq.pollFirst();
			dq.addLast(arr[i][cy]);
		}
		//아래 추가
		
		for(int i=0;i<cy;i++) {
			arr[cx][i] = dq.pollFirst();
			dq.addLast(arr[cx][i]);
		}
	}
	
	static void mRotate(int x1,int y1,int len) {
		
		int [][] tarr = new int[len][len];
		
		for(int i=0;i<len;i++) {
			for(int j=0;j<len;j++) {
				tarr[i][j] = arr[i+x1][j+y1]; 
			}
		}
		//temp 배열에 옮기고
		
		for(int i=0;i<len;i++) {
			for(int j=0;j<len;j++) {
				arr[i+x1][j+y1] = tarr[len-1-j][i];
			}
		}
		
	}
	
	static int BFS(int x,int y,int num) {
		
		int cnt = 1;
		
		Queue<Point> q = new ArrayDeque<>();
		q.add(new Point(x,y));
		
		cord[num].add(new Point(x,y));
		visited[x][y] = true;
		group[x][y] = num;
		while(!q.isEmpty()) {
			Point cur = q.poll();
			int val = arr[cur.x][cur.y];
			//현재 값 value
			for(int i=0;i<4;i++) {
				int qx = cur.x + dx[i];
				int qy = cur.y + dy[i];
				
				if(out(qx,qy) || visited[qx][qy] || arr[qx][qy] != val) continue;
				
				visited[qx][qy] = true;
				group[qx][qy] = num;
				q.add(new Point(qx,qy));
				cord[num].add(new Point(qx,qy));
				cnt++;
				
			}
		}
		return cnt;
		
	}
	
	static void scan() {
		map = new HashMap<>();
		visited = new boolean[N][N];
		cord = new Queue[N*N+1];
		cnt = new int[N*N+1];
		group = new int[N][N];
		
		for(int i=1;i<=N*N;i++) {
			cord[i] = new ArrayDeque<>();
		}
		
		
		num = 0;
		for(int i=0;i<N;i++) {
			for(int j=0;j<N;j++) {
				if(visited[i][j]) continue;
				cnt[++num] = BFS(i,j,num);
				map.put(num, arr[i][j]);
				//값 매핑
			
			}
		}

	}
	
	static void print() {
		for(int i=0;i<N;i++) {
			for(int j=0;j<N;j++) {
				System.out.print(arr[i][j] + " ");
			}
			System.out.println();
		}
	
	}
	
	
	
	static int fusion(int a,int b) {
		int sm = 0;
	
		int shareLine = 0;
		
		Queue<Point> cur = cord[a];
		
		for(Point p : cur) {
			
			for(int i=0;i<4;i++) {
				int qx = p.x + dx[i];
				int qy = p.y + dy[i];
				
				if(out(qx,qy)) continue;
				
				if(group[qx][qy] == b) shareLine++;
			}
		}
		
		sm = (cnt[a] + cnt[b]) * map.get(a) * map.get(b) * shareLine;
		
		return sm;
	}
	
	static void combi(int idx, int start) {
		
		if(idx == 2) {
//			for(int i= 0;i<idx;i++) {
//				System.out.print(carr[i]+" ");
//			}
//			System.out.println();
			
			ans += fusion(carr[0], carr[1]);
			
			return;
		}
		
		for(int i=start+1;i<=num;i++) {
			
			carr[idx] = i;
			combi(idx+1,i);
		}
	}
	
	static void rotate() {
		int len = cx;
		crossRotate();
		//십자가 돌리고
		mRotate(0, 0,len);
		mRotate(0,0+cy+1,len);
		mRotate(0+cx+1, 0,len);
		mRotate(0+cx+1,0+cy+1,len);
	}
	
	static void run() {
		scan();
		//그룹 스캔 번호 순으로 스캔된다.
		combi(0,0);
		//처음후
		for(int i=0;i<3;i++) {
			rotate();
			scan();
			combi(0,0);
		}
		

	}
	
	public static void main(String[] args) throws IOException {
		N = Integer.parseInt(in.readLine());
		arr = new int[N][N];

		carr = new int[2];
		
		for(int i=0;i<N;i++) {
			st = new StringTokenizer(in.readLine());
			for(int j=0;j<N;j++) {
				arr[i][j] = Integer.parseInt(st.nextToken());
			}
		}
		dq = new ArrayDeque<>();
		cx = N/2;
		cy = N/2;
		
		init();
		//덱에 가운데 있는 거 전부 넣어줌
		
		run();
		
		System.out.println(ans);
	}

}
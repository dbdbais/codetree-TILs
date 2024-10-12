import java.awt.Point;
import java.io.*;
import java.util.*;

public class Main {

	static class Rudolph {
		int x;
		int y;

		public Rudolph(int x, int y) {
			this.x = x;
			this.y = y;
		}

		public void move() {

			ArrayList<Santa> lst = new ArrayList<>();

			for (int i = 1; i <= P; i++) {
				if (!players[i].life)
					continue;
				lst.add(players[i]);
				// 죽지 않은거면 리스트에 넣고
			}

			Collections.sort(lst);

			Santa target = lst.get(0);
			// 타겟 설정
			int tx = 0, ty = 0, dir = 0, tdist = Integer.MAX_VALUE;
			for (int i = 0; i < 8; i++) {
				int qx = x + rdx[i];
				int qy = y + rdy[i];

				if (out(qx, qy))
					continue;
				int curDist = getDistance(qx, qy, target.x, target.y);
				if (tdist > curDist) {
					tdist = curDist;
					tx = qx;
					ty = qy;
					dir = i;
				}
			}
			visited[x][y] = 0;
			// 현재 자리 비우고
			//System.out.println(tx + ", " + ty);

			x = tx;
			y = ty;

			if (visited[x][y] != 0) {
				// 산타가 있다면
				int no = visited[x][y];
				Santa victim = players[no];
				victim.score += C;
				visited[victim.x][victim.y] = 0;
				// 비워주고
				victim.x += C * rdx[dir];
				victim.y += C * rdy[dir];

				if (out(victim.x, victim.y)) {
					// 장외
					victim.life = false;
				} else {
					victim.stun = 2;
					//System.out.println(no + "STUNNED");
					// 한번 스턴 먹이고
					if (visited[victim.x][victim.y] != 0) {
						scoop(victim.x, victim.y, rdx[dir], rdx[dir]);
						// 연속 처리
					}
					visited[victim.x][victim.y] = no;
					// 방문 처리
				}

			}

			visited[x][y] = -1;

		}

		@Override
		public String toString() {
			return "Rudolph [x=" + x + ", y=" + y + "]";
		}
		
		

	}

	static void scoop(int x, int y, int diffX, int diffY) {
		int no = visited[x][y];
		Santa cur = players[no];

		visited[cur.x][cur.y] = 0;
		// 현재 위치 없애고

		cur.x += diffX;
		cur.y += diffY;

		if (out(cur.x, cur.y)) {
			cur.life = false;
			return;
		}

		if (visited[cur.x][cur.y] != 0) {
			scoop(cur.x, cur.y, diffX, diffY);
			// 다음도 똑같이 실행
		}
		visited[cur.x][cur.y] = no;
		// 나의 자리로 확정

	}
	
	static int conv(int dir) {
		if(dir == 0) return 2;
		else if(dir == 1) return 3;
		else if(dir == 2) return 0;
		else return 1;
	}

	static class Cord implements Comparable<Cord> {
		int x;
		int y;
		int dir;
		int dist;

		public Cord(int x, int y, int dir) {
			this.x = x;
			this.y = y;
			this.dir = dir;
			this.dist = getDistance(x, y, rudolph.x, rudolph.y);
		}

		@Override
		public int compareTo(Cord o) {

			if (dist == o.dist) {
				return Integer.compare(dir, o.dir);
			}
			return Integer.compare(dist, o.dist);

		}

		@Override
		public String toString() {
			return "Cord [x=" + x + ", y=" + y + ", dir=" + dir + ", dist=" + dist + "]";
		}
		
		

	}

	static int getDistance(int x1, int y1, int x2, int y2) {
		return (int) Math.pow(x1 - x2, 2) + (int) Math.pow(y1 - y2, 2);
	}

	static class Santa implements Comparable<Santa> {
		int x;
		int y;
		boolean life = true;
		int stun = 0;
		int score = 0;

		public Santa(int x, int y) {
			this.x = x;
			this.y = y;
		}

		public int getDistance() {
			return (int) Math.pow(rudolph.x - x, 2) + (int) Math.pow(rudolph.y - y, 2);
		}
		
		public void turnOver() {
			if(life) score++;
		}

		public void move() {
			if (!life)
				return;
			if(stun > 0) {
				stun--;
				return;
			}
			int rdist = getDistance();
			// 루돌프와의 거리
			//System.out.println("RDIST"+rdist);
			int num = visited[x][y];
			ArrayList<Cord> lst = new ArrayList<>();

			for (int i = 0; i < 4; i++) {
				int qx = x + dx[i];
				int qy = y + dy[i];

				if (out(qx, qy))
					continue;

				if (visited[qx][qy] != 0 && visited[qx][qy] != -1) {
					continue;
				}
				// 다른 산타면 SKIP

				lst.add(new Cord(qx, qy, i));
			}

			Collections.sort(lst);

			Cord fdest = lst.get(0);
			

			int val = fdest.dist;
			//System.out.println("val" + val);
			if (val == 0) {
				// 루돌프와 충돌
				stun = 1;

				score += D;
				visited[x][y] = 0;
				// 비워주고
				int rdir = conv(fdest.dir);
				x = rudolph.x + D * dx[rdir];
				y = rudolph.y + D * dy[rdir];

				if (out(x, y)) {
					// 장외
					life = false;
				} else {
					// 한번 스턴 먹이고
					if (visited[x][y] != 0) {
						//System.out.println("충돌발생");
						scoop(x, y, dx[rdir], dy[rdir]);
						// 연속 처리
					}
					visited[x][y] = num;
					// 방문 처리
				}

			} else if (val < rdist) {
				// 이동처리
				visited[x][y] = 0;
				x = fdest.x;
				y = fdest.y;
				visited[x][y] = num;
			}

		}

		public int compareTo(Santa o) {
			int dist = getDistance();
			int oDist = o.getDistance();

			if (dist == oDist) {
				if (x == o.x) {
					return Integer.compare(y, o.y) * -1;
				}
				return Integer.compare(x, o.x) * -1;
			}
			return Integer.compare(dist, oDist);

		}

		@Override
		public String toString() {
			return "Santa [x=" + x + ", y=" + y + ", life=" + life + ", stun=" + stun + ", score=" + score + "]";
		}
		
	}
	static boolean check() {
		for(int i=1;i<=P;i++) {
			if(players[i].life) return true;
		}
		return false;
	}
	static void play() {
		while(M-- > 0) {
			rudolph.move();
			for(int i=1;i<=P;i++) {
				players[i].move();
			}
			for(int i=1;i<=P;i++) {
				players[i].turnOver();
			}
			if(!check()) break;
	
//			System.out.println(turn++ + "번째 턴");
//			print();
//			for(int i=1;i<=P;i++) {
//				System.out.print(players[i].score+" ");
//			}
//			System.out.println();
			
		}
	}
	static void print() {
		System.out.println("VISITED");
		for(int i=0;i<N;i++) {
			for(int j=0;j<N;j++) {
				System.out.print(visited[i][j]+" ");
			}
			System.out.println();
		}
		System.out.println("SANTA");
		for(int i=1;i<=P;i++) {
			System.out.println(players[i]);
		}
		System.out.println("RUDOLPH");
		System.out.println(rudolph);
		
	}

	static BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
	static StringTokenizer st;
	static int N, M, P, C, D, rx, ry;

	static final int[] rdx = { 0, 0, 1, -1, -1, -1, 1, 1 };
	static final int[] rdy = { -1, 1, 0, 0, -1, 1, -1, 1 };

	static final int[] dx = { -1, 0, 1, 0 };
	static final int[] dy = { 0, 1, 0, -1 };
	// 상 우 하 좌
	static int[][] visited;
	static Santa[] players;
	static Rudolph rudolph;

	static boolean out(int x, int y) {
		return x < 0 || y < 0 || x >= N || y >= N;
	}
	
	public static void main(String[] args) throws IOException {

		st = new StringTokenizer(in.readLine());

		N = Integer.parseInt(st.nextToken());
		M = Integer.parseInt(st.nextToken());
		P = Integer.parseInt(st.nextToken());
		C = Integer.parseInt(st.nextToken());
		D = Integer.parseInt(st.nextToken());

		visited = new int[N][N];
		players = new Santa[P + 1];
		// 산타 할당

		st = new StringTokenizer(in.readLine());

		rx = Integer.parseInt(st.nextToken());
		ry = Integer.parseInt(st.nextToken());

		rudolph = new Rudolph(rx - 1, ry - 1);
		visited[rx-1][ry-1] = -1;
		// 루돌프 위치 표시

		int num, sx, sy;

		for (int i = 1; i <= P; i++) {
			st = new StringTokenizer(in.readLine());
			num = Integer.parseInt(st.nextToken());
			sx = Integer.parseInt(st.nextToken()) - 1;
			sy = Integer.parseInt(st.nextToken()) - 1;
			players[num] = new Santa(sx, sy);
			visited[sx][sy] = num;
			// 현재 산타의 위치 표시
		}
		play();
		
		for(int i=1;i<=P;i++) {
			System.out.print(players[i].score+" ");
		}
		
		
	}
}
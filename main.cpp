#include <iostream>
#include <stack>
#include <map>

enum Direction {
	U = 0,
	R = 1,
	D = 2,
	L = 3,
};

class Minotaur {
	int x, y;
	Direction direction;
	int k;
	int moveCounter = 0;
	std::vector<std::vector<char>> surroundings;
	std::pair<int, int> curr_relative_position;

	struct Edge {
		std::pair<int, int> curr;
		std::pair<int, int> adj;

		Edge(int x, int y, int x1, int y1) : curr({x, y}), adj({x1, y1}) {}
	};

public:
	Minotaur(int x, int y, int x1, int y1, int k) : x(x), y(y), k(k) {
		surroundings = std::vector<std::vector<char>>(2 * k + 1, std::vector(2 * k + 1, '.'));
		if (x == x1) {
			if (y < y1) {
				direction = U;
			} else {
				direction = D;
			}
		} else {
			if (x > x1) {
				direction = L;
			} else {
				direction = R;
			}
		}
	}

	void walkthrough() {
		std::stack<std::pair<int, int>> moves;
		std::stack<Edge> toExplore;
		std::map<std::pair<int, int>, bool> visited;
		//zero iteration of traversal
		moves.emplace(x, y);
		while (!moves.empty()) {
			//if we already have been there, we must have gone back from the branch,
			//check if there are any other branches
			if (visited[moves.top()]) {
				if (toExplore.empty()) {
					end();
					return;
				}
				if (toExplore.top().curr.first == x && toExplore.top().curr.second == y) {
					turn(toExplore.top().adj);
					moveStraight();
					++moveCounter;
					moves.emplace(toExplore.top().adj);
					toExplore.pop();
				}
			}
			std::pair<bool, std::pair<int, int >> straightMove = {false, {0, 0}};
			x = moves.top().first;
			y = moves.top().second;
			visited[{x, y}] = true;
			if (moveCounter % 2 == 0) {
				scan();
			}
			bool foundNextMove = false;
			if (surroundings[curr_relative_position.first][curr_relative_position.second + 1] == '_' &&
				!visited[{x, y + 1}]) {
				foundNextMove = true;
				if (direction == U) {
					straightMove = {true, {0, 1}};
				} else {
					toExplore.emplace(x, y, x, y + 1);
				}
			}
			if (surroundings[curr_relative_position.first + 1][curr_relative_position.second] == '_' &&
				!visited[{x + 1, y}]) {
				foundNextMove = true;
				if (direction == R) {
					straightMove = {true, {1, 0}};
				} else {
					toExplore.emplace(x, y, x + 1, y);
				}
			}
			if (surroundings[curr_relative_position.first][curr_relative_position.second - 1] == '_' &&
				!visited[{x, y - 1}]) {
				foundNextMove = true;
				if (direction == D) {
					straightMove = {true, {0, -1}};
				} else {
					toExplore.emplace(x, y, x, y - 1);
				}
			}
			if (surroundings[curr_relative_position.first - 1][curr_relative_position.second] == '_' &&
				!visited[{x - 1, y}]) {
				foundNextMove = true;
				if (direction == L) {
					straightMove = {true, {-1, 0}};
				} else {
					toExplore.emplace(x, y, x - 1, y);
				}
			}
			if (!foundNextMove) { //if there are nowhere to go, go back
				moves.pop();
				turn(moves.top());
			} else {
				if (straightMove.first) { //if there is a move without turning
					moves.emplace(x + straightMove.second.first, y + straightMove.second.second);
					++moveCounter;
					moveStraight();
					continue;
				}
				turn(toExplore.top().adj);
				moves.emplace(toExplore.top().adj);
				toExplore.pop();
			}
			moveStraight();

			++moveCounter;
		}
		end();
	}

private:
	Direction getDirection(const std::pair<int, int> &neighbour) const {
		if (x == neighbour.first) {
			if (y < neighbour.second) {
				return U;
			} else {
				return D;
			}
		} else {
			if (x < neighbour.first) {
				return R;
			} else {
				return L;
			}
		}
	}

	void moveStraight() {
		std::cout << 1 << '\n';
		if (direction == U) {
			++y;
			++curr_relative_position.second;
		} else if (direction == R) {
			++x;
			++curr_relative_position.first;
		} else if (direction == D) {
			--y;
			--curr_relative_position.second;
		} else if (direction == L) {
			--x;
			--curr_relative_position.first;
		}
		int temp;
		std::cin >> temp;
	}

	void turn(const std::pair<int, int> &nextCoord) {
		Direction nextDirection = getDirection(nextCoord);
		int turnValue = (static_cast<int>(nextDirection) - static_cast<int>(direction) + 4) % 4;
		if (turnValue == 1) {
			turnClockwise();
		} else if (turnValue == 3) {
			turnCounterClockwise();
		} else {
			turnClockwise();
			turnClockwise();
		}
		direction = nextDirection;
	}

	static void turnClockwise() {
		std::cout << 2 << " " << 1 << '\n';
		//direction = static_cast<Direction>((direction + 1) % 4);
		int temp;
		std::cin >> temp;
	}

	static void turnCounterClockwise() {
		std::cout << 2 << " " << 0 << '\n';
		//direction = static_cast<Direction>((2 * direction - 1) % 4);
		int temp;
		std::cin >> temp;
	}

	void scan() {
		std::cout << 3 << '\n';
		char area[2 * k + 1][2 * k + 1];
		for (int i = 2 * k; i >= 0; --i) {
			for (int j = 0; j < 2 * k + 1; ++j) {
				std::cin >> surroundings[j][i];
			}
		}
		curr_relative_position = {k, k};
	}

	static void end() {
		std::cout << 4 << '\n';
	}

};

int main() {
	int x, y, x1, y1, A, B, C, K;
	std::cin >> x >> y >> x1 >> y1 >> A >> B >> C >> K;
	Minotaur minotaur(x, y, x1, y1, K);
	minotaur.walkthrough();
}

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// 도시 정보 구조체
struct city {
	int id = 0, po = 0;
	string name;
	struct city* head = NULL;
	struct city* tail = NULL;
	struct city* next = NULL;
	int head_count = 1, head_weight = 0;
};
// 도로 정보 구조체
struct road { int id1, id2, weight; };

vector<city> vertex;		// 도시 정보들 담는 벡터
vector<road> edge;			// 간선 정보들 담는 벡터
int arr[1000000];			// id에 맞는 vertex의 index 저장
int count_edge = 0;			// 현재 만들어진 spanning tree 간선 갯수
int leader_id = 0;			// 현재 시점의 leader id

bool union_find(int a, int b, int w);		// union 함수
bool compare(road a, road b);				// edge sort 함수

int main() {
	int N, M, q;
	cin >> N >> M >> q;
	city c; road r;
	for (int i = 0; i < N; i++) {
		cin >> c.id >> c.name >> c.po;		// 도시 정보 입력
		vertex.push_back(c);				// 도시 정보를 vertex에 push_back
		arr[c.id] = i;						// 도시의 id에 vertex의 index 저장
	}
	for (int i = 0; i < N; i++) {			// head와 tail을 자기 자신으로 초기화
		vertex[i].head = &vertex[i];
		vertex[i].tail = &vertex[i];
	}
	int i1, i2;
	for (int i = 0; i < M; i++) {
		cin >> i1 >> i2 >> r.weight;		// 도로 정보 입력
		r.id1 = (i1 < i2) ? i1 : i2;		// 도로 양 끝 도시 id를 작은 순서대로 저장
		r.id2 = (i1 < i2) ? i2 : i1;
		edge.push_back(r);					// 도로 정보를 edge에 push_back
	}
	sort(edge.begin(), edge.end(), compare);// edge sort
	int i = 0;								// 현재 탐색 중인 edge의 index
	bool check = true;						// 질의 I에서 MST가 완성됐는지 확인하는 변수
	char ord;								// 질의 입력 변수

	int v1, v2, k;
	city* temp; city* temp1;
	while (q--) {
		cin >> ord;
		if (ord == 'N') {
			cin >> v1 >> k;													// v1이 속한 set의 k 번째 element 정보 출력
			temp = vertex[arr[v1]].head;									// v1이 속한 set의 leader를 저장
			if (temp->head_count - 1 < k) cout << "no exist" << endl;		// set의 전체 element 개수가 k+1보다 적으면 no exist
			else if (temp->head_count - 1 == k)								// set의 전체 element 개수가 k+1과 같다면 
				cout << temp->tail->id << " " << temp->tail->name << endl;	// 그 set의 leader의 tail 정보 출력	
			else {															// set의 개수가 더 많으면
				while (k--) temp = temp->next;								// k만큼 이동하여
				cout << temp->id << " " << temp->name << endl;				// 해당 element 정보 출력
			}
		}
		else if (ord == 'L') {
			cin >> v1;											// v1이 속한 set의 element 개수 구하기
			cout << vertex[arr[v1]].head->head_count << endl;	// v1의 leader의 head_count 출력
		}
		else if (ord == 'I') {
			if (union_find(edge[i].id1, edge[i].id2, edge[i].weight)) {							// union 성공
				temp = &vertex[arr[leader_id]];													// temp에 leader vertex 저장
				if (count_edge == N - 1) {														// MST를 완성했다면
					cout << leader_id << " " << temp->head_count								// 정보 출력 후
						<< " " << leader_id << " " << temp->head_weight << endl;
					check = false;																// MST 완성했는지 확인하는 변수 갱신
					break;																		// 하고 break
				}
				else cout << leader_id << " " << vertex[arr[leader_id]].head_count << endl;		// MST 완성하지 않았다면, 1회 union에 대한 정보 출력
			}
			else cout << "not union" << endl;													// union 실패 == cycle 존재
			i++;
		}
		else if (ord == 'F') {
			cin >> v1 >> v2;													// v1과 v2가 같은 set에 있는지 확인
			temp = vertex[arr[v1]].head; temp1 = vertex[arr[v2]].head;		// 각 노드가 속한 set의 leader를 저장
			if (temp->id == temp1->id) cout << "True " << temp->id << endl;		// 두 leader의 id가 같다면 True를 반환하고 그 id 출력
			else cout << "False " << temp->id << " " << temp1->id << endl;		// 두 leader의 id가 다르다면 False와 각각의 id 출력
		}
		else if (ord == 'W') {
			cin >> v1;											// v1이 속한 set의 전체 weight 합 출력
			cout << vertex[arr[v1]].head->head_weight << endl;	// v1이 속한 leader의 head_weight 출력
		}
		else if (ord == 'Q') break;
	}
	if (check) {													//질의 I에서 MST가 완성되지 않았다면
		while (1) {
			union_find(edge[i].id1, edge[i].id2, edge[i].weight);	// spanning tree의 간선의 갯수가 N-1이될 때까지 union
			if (count_edge == N - 1) break;
			i++;
		}
		cout << leader_id << " " << vertex[arr[leader_id]].head->head_weight << endl;
	}

	return 0;
}

//edge sort
bool compare(road a, road b) {
	if (a.weight == b.weight) {											// 1) weight 최솟값
		city* s = &vertex[arr[a.id1]]; city* t = &vertex[arr[a.id2]];
		city* u = &vertex[arr[b.id1]]; city* v = &vertex[arr[b.id2]];
		if ((s->po + t->po) == (u->po + v->po)) {						// 2) 인구 수의 최댓값
			if (a.id1 == b.id1) return a.id2 < b.id2;					// 3) id의 최솟값
			return a.id1 < b.id1;										// 위 순서대로 우선순위대로 edge를 sort
		}
		return (s->po + t->po) > (u->po + v->po);
	}
	return a.weight < b.weight;
}

// union
bool union_find(int a, int b, int w) {
	city* A_l = vertex[arr[a]].head;		// id가 a인 vertex가 속한 set의 leader의 주소를 A_l에 저장
	city* B_l = vertex[arr[b]].head;		// id가 b인 vertex가 속한 set의 leader의 주소를 B_l에 저장

	if (A_l->id == B_l->id) return false;	// a, b가 속한 set의 leader의 id가 같을 때, cycle 존재 == union 실패
	else {
		count_edge++;						// union할 거니까 spanning tree의 간선 갯수 +1
		city* temp;
		if (A_l->head_count > B_l->head_count) {			// a가 속한 set의 element 갯수가 더 많다면, 그 set의 leader가 전체 leader가 됨
			temp = A_l->tail;
			A_l->tail = B_l->tail;							// A_l의 tail을 B_l의 tail로 갱신
			temp->next = B_l;								// 원래 A_l의 next를 B_l로 갱신
			temp = temp->next;
			while (1) {										// B_l부터 b가 속한 set의 element들의 head를 갱신
				temp->head = A_l;
				if (temp->next == NULL) break;
				temp = temp->next;
			}
			leader_id = A_l->id;							// 현재 시점 총 leader의 id는 A_l의 id로 갱신
			A_l->po += B_l->po;								// A_l가 총 leader가 됐으니, 
			A_l->head_weight += (w + B_l->head_weight);		// 전체 인구 수, 가중치 합, element 갯수를 갱신
			A_l->head_count += B_l->head_count;
		}
		else if (A_l->head_count < B_l->head_count) {		// b가 속한 set의 element 개수가 더 많다면 B_l이 전체 leader가 된다.
			temp = B_l->tail;								// 위의 과정과 같다.
			B_l->tail = A_l->tail;
			temp->next = A_l;
			temp = temp->next;
			while (1) {
				temp->head = B_l;
				if (temp->next == NULL) break;
				temp = temp->next;
			}
			leader_id = B_l->id;
			B_l->po += A_l->po;
			B_l->head_weight += (w + A_l->head_weight);
			B_l->head_count += A_l->head_count;
		}
		else {												// a가 속한 set의 element 수와 b가 속한 set의 element 수가 같다면
			if (A_l->id < B_l->id) {						// A_l의 id가 더 작다면 A_l이 전체 leader가 됨
				temp = A_l->tail;							// 위의 A_l이 전체 leader가 되는 과정과 같다
				A_l->tail = B_l->tail;
				temp->next = B_l;
				temp = temp->next;
				while (1) {
					temp->head = A_l;
					if (temp->next == NULL) break;
					temp = temp->next;
				}
				leader_id = A_l->id;
				A_l->po += B_l->po;
				A_l->head_weight += (w + B_l->head_weight);
				A_l->head_count += B_l->head_count;
			}
			else {											// B_l의 id가 더 작다면 B_l가 전체 leader가 됨
				temp = B_l->tail;							// 위의 B_l이 전체 leader가 되는 과정과 같다.
				B_l->tail = A_l->tail;
				temp->next = A_l;
				temp = temp->next;
				while (1) {
					temp->head = B_l;
					if (temp->next == NULL) break;
					temp = temp->next;
				}
				leader_id = B_l->id;
				B_l->po += A_l->po;
				B_l->head_weight += (w + A_l->head_weight);
				B_l->head_count += A_l->head_count;
			}
		}
	}
	return true;
}
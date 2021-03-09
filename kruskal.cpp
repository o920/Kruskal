#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// ���� ���� ����ü
struct city {
	int id = 0, po = 0;
	string name;
	struct city* head = NULL;
	struct city* tail = NULL;
	struct city* next = NULL;
	int head_count = 1, head_weight = 0;
};
// ���� ���� ����ü
struct road { int id1, id2, weight; };

vector<city> vertex;		// ���� ������ ��� ����
vector<road> edge;			// ���� ������ ��� ����
int arr[1000000];			// id�� �´� vertex�� index ����
int count_edge = 0;			// ���� ������� spanning tree ���� ����
int leader_id = 0;			// ���� ������ leader id

bool union_find(int a, int b, int w);		// union �Լ�
bool compare(road a, road b);				// edge sort �Լ�

int main() {
	int N, M, q;
	cin >> N >> M >> q;
	city c; road r;
	for (int i = 0; i < N; i++) {
		cin >> c.id >> c.name >> c.po;		// ���� ���� �Է�
		vertex.push_back(c);				// ���� ������ vertex�� push_back
		arr[c.id] = i;						// ������ id�� vertex�� index ����
	}
	for (int i = 0; i < N; i++) {			// head�� tail�� �ڱ� �ڽ����� �ʱ�ȭ
		vertex[i].head = &vertex[i];
		vertex[i].tail = &vertex[i];
	}
	int i1, i2;
	for (int i = 0; i < M; i++) {
		cin >> i1 >> i2 >> r.weight;		// ���� ���� �Է�
		r.id1 = (i1 < i2) ? i1 : i2;		// ���� �� �� ���� id�� ���� ������� ����
		r.id2 = (i1 < i2) ? i2 : i1;
		edge.push_back(r);					// ���� ������ edge�� push_back
	}
	sort(edge.begin(), edge.end(), compare);// edge sort
	int i = 0;								// ���� Ž�� ���� edge�� index
	bool check = true;						// ���� I���� MST�� �ϼ��ƴ��� Ȯ���ϴ� ����
	char ord;								// ���� �Է� ����

	int v1, v2, k;
	city* temp; city* temp1;
	while (q--) {
		cin >> ord;
		if (ord == 'N') {
			cin >> v1 >> k;													// v1�� ���� set�� k ��° element ���� ���
			temp = vertex[arr[v1]].head;									// v1�� ���� set�� leader�� ����
			if (temp->head_count - 1 < k) cout << "no exist" << endl;		// set�� ��ü element ������ k+1���� ������ no exist
			else if (temp->head_count - 1 == k)								// set�� ��ü element ������ k+1�� ���ٸ� 
				cout << temp->tail->id << " " << temp->tail->name << endl;	// �� set�� leader�� tail ���� ���	
			else {															// set�� ������ �� ������
				while (k--) temp = temp->next;								// k��ŭ �̵��Ͽ�
				cout << temp->id << " " << temp->name << endl;				// �ش� element ���� ���
			}
		}
		else if (ord == 'L') {
			cin >> v1;											// v1�� ���� set�� element ���� ���ϱ�
			cout << vertex[arr[v1]].head->head_count << endl;	// v1�� leader�� head_count ���
		}
		else if (ord == 'I') {
			if (union_find(edge[i].id1, edge[i].id2, edge[i].weight)) {							// union ����
				temp = &vertex[arr[leader_id]];													// temp�� leader vertex ����
				if (count_edge == N - 1) {														// MST�� �ϼ��ߴٸ�
					cout << leader_id << " " << temp->head_count								// ���� ��� ��
						<< " " << leader_id << " " << temp->head_weight << endl;
					check = false;																// MST �ϼ��ߴ��� Ȯ���ϴ� ���� ����
					break;																		// �ϰ� break
				}
				else cout << leader_id << " " << vertex[arr[leader_id]].head_count << endl;		// MST �ϼ����� �ʾҴٸ�, 1ȸ union�� ���� ���� ���
			}
			else cout << "not union" << endl;													// union ���� == cycle ����
			i++;
		}
		else if (ord == 'F') {
			cin >> v1 >> v2;													// v1�� v2�� ���� set�� �ִ��� Ȯ��
			temp = vertex[arr[v1]].head; temp1 = vertex[arr[v2]].head;		// �� ��尡 ���� set�� leader�� ����
			if (temp->id == temp1->id) cout << "True " << temp->id << endl;		// �� leader�� id�� ���ٸ� True�� ��ȯ�ϰ� �� id ���
			else cout << "False " << temp->id << " " << temp1->id << endl;		// �� leader�� id�� �ٸ��ٸ� False�� ������ id ���
		}
		else if (ord == 'W') {
			cin >> v1;											// v1�� ���� set�� ��ü weight �� ���
			cout << vertex[arr[v1]].head->head_weight << endl;	// v1�� ���� leader�� head_weight ���
		}
		else if (ord == 'Q') break;
	}
	if (check) {													//���� I���� MST�� �ϼ����� �ʾҴٸ�
		while (1) {
			union_find(edge[i].id1, edge[i].id2, edge[i].weight);	// spanning tree�� ������ ������ N-1�̵� ������ union
			if (count_edge == N - 1) break;
			i++;
		}
		cout << leader_id << " " << vertex[arr[leader_id]].head->head_weight << endl;
	}

	return 0;
}

//edge sort
bool compare(road a, road b) {
	if (a.weight == b.weight) {											// 1) weight �ּڰ�
		city* s = &vertex[arr[a.id1]]; city* t = &vertex[arr[a.id2]];
		city* u = &vertex[arr[b.id1]]; city* v = &vertex[arr[b.id2]];
		if ((s->po + t->po) == (u->po + v->po)) {						// 2) �α� ���� �ִ�
			if (a.id1 == b.id1) return a.id2 < b.id2;					// 3) id�� �ּڰ�
			return a.id1 < b.id1;										// �� ������� �켱������� edge�� sort
		}
		return (s->po + t->po) > (u->po + v->po);
	}
	return a.weight < b.weight;
}

// union
bool union_find(int a, int b, int w) {
	city* A_l = vertex[arr[a]].head;		// id�� a�� vertex�� ���� set�� leader�� �ּҸ� A_l�� ����
	city* B_l = vertex[arr[b]].head;		// id�� b�� vertex�� ���� set�� leader�� �ּҸ� B_l�� ����

	if (A_l->id == B_l->id) return false;	// a, b�� ���� set�� leader�� id�� ���� ��, cycle ���� == union ����
	else {
		count_edge++;						// union�� �Ŵϱ� spanning tree�� ���� ���� +1
		city* temp;
		if (A_l->head_count > B_l->head_count) {			// a�� ���� set�� element ������ �� ���ٸ�, �� set�� leader�� ��ü leader�� ��
			temp = A_l->tail;
			A_l->tail = B_l->tail;							// A_l�� tail�� B_l�� tail�� ����
			temp->next = B_l;								// ���� A_l�� next�� B_l�� ����
			temp = temp->next;
			while (1) {										// B_l���� b�� ���� set�� element���� head�� ����
				temp->head = A_l;
				if (temp->next == NULL) break;
				temp = temp->next;
			}
			leader_id = A_l->id;							// ���� ���� �� leader�� id�� A_l�� id�� ����
			A_l->po += B_l->po;								// A_l�� �� leader�� ������, 
			A_l->head_weight += (w + B_l->head_weight);		// ��ü �α� ��, ����ġ ��, element ������ ����
			A_l->head_count += B_l->head_count;
		}
		else if (A_l->head_count < B_l->head_count) {		// b�� ���� set�� element ������ �� ���ٸ� B_l�� ��ü leader�� �ȴ�.
			temp = B_l->tail;								// ���� ������ ����.
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
		else {												// a�� ���� set�� element ���� b�� ���� set�� element ���� ���ٸ�
			if (A_l->id < B_l->id) {						// A_l�� id�� �� �۴ٸ� A_l�� ��ü leader�� ��
				temp = A_l->tail;							// ���� A_l�� ��ü leader�� �Ǵ� ������ ����
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
			else {											// B_l�� id�� �� �۴ٸ� B_l�� ��ü leader�� ��
				temp = B_l->tail;							// ���� B_l�� ��ü leader�� �Ǵ� ������ ����.
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
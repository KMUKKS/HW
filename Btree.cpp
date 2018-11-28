#include<iostream>
#include <cstdio>
#define M			4		//분기M(차수)가 4일 때 (원하는 분기만큼 가능)
#define MAX_STACK	1000

class Btree {		// Btree 구조체: 키의 개수가 M-1개 이고 차수가 M
public:

	int n;
	int key[M - 1];
	class Btree *branch[M];

};

class ParentBtree {		// Btree 구조체 : 분할을 위해 사용되는 키의 개수가 M개 이고 차수가 M+1
public:

	int n;
	int key[M];
	class Btree *branch[M + 1];

};

class SplitBtree {		// Btree 구조체 :재분배를 위해 사용되는 키의 개수가 2*(M-1)개 이고 차수가 2 * (M - 1) + 1
public:

	int n;
	int key[2 * (M - 1)];
	class Btree *branch[2 * (M - 1) + 1];

};

class Btrees {		//Btree 구조체 : btree 노드 포인터를 가지고 있음
public:

	int n;
	class Btree *p;
};

void initStack();			//Stack 초기화

Btrees stack[MAX_STACK];	//노드들을 담을 Stack

int top = -1;				//스택 top부분 초기화

Btree *root = NULL;			//Btree의 root는 NULL로 초기화

Btrees pop();				//Stack에서 노드를 pop 해주는 함수

void push(Btrees node);		//Stack에 노드를 push 해주는 함수

int btree_insert(int);		//Btree에 노드를 넣는 함수

void initStack() {		//Stack 초기화 함수
	top = -1;
}

Btrees pop() {		//Stack에서 노드를 pop 해주는 함수
	Btrees node;
	node = stack[top];
	top--;
	return node;
}

void push(Btrees node) {   //Stack에 노드를 push 해주는 함수
	top++;
	stack[top] = node;
}

int btree_insert(int data) {			//Btree에 노드를 넣는 함수
	int i, j, N, mid;
	int found = 0, finished = 0;		//삽입할 공간을 찾았는지 저장하는 found 변수와 삽입했는지 저장하는 finished 변수 선언

	Btree *curr, *p, *newnode;			//분할 시 사용될 curr, p, newnode 선언
	Btrees node;
	ParentBtree tempNode;						//오버플로우 해결하는 변수

	initStack();	

	if (root == NULL) {				//루트 노드가 NULL이라면

		root = (Btree *)malloc(sizeof(Btree));		
		root->n = 1;								//루트 키 개수 1로 할당
		root->key[0] = data;						//루트의 0번째 키를 할당
		for (i = 0; i <= 1; i++)					
			root->branch[i] = NULL;
		return 1;
	}

	curr = root;		//root노드를 curr로 복사
	found = 0;

	//루트 노드가 NULL이 아니라면 들어갈 위치를 찾는다.
	do {
		p = NULL;			
		N = curr->n;		//curr의 키 개수를 변수 N에 저장

		for (i = 0; i < N; i++) {
			if (curr->key[i] == data) {	//data가 이미 있다면 
				found = 1;				//찾았으므로 found = 1
				break;
			}
			else if (curr->key[i] > data) {	//data보다 curr의 key[i]가 더 크다면
				p = curr->branch[i];			//p는 curr의 branch[i]로 들어간다
				break;
			}
		}

		if (curr->key[i - 1] < data && p == NULL)	//data가 curr의 key들 중 제일 클 경우
			p = curr->branch[i];		//curr의 마지막 branch로 들어간다.

		if (p != NULL) {		//들어갈 곳을 찾았을 경우
			node.p = curr;
			push(node);			//p를 stack에 넣어준다.
			curr = p;			//p를 curr로 복사해준다.
		}

	} while (found == 0 && p != NULL);	//삽입 위치를 아직 못 찾았다면

	if (found == 1) return 0;		//삽입했다면

	p = NULL;
	finished = 0;

	//삽입 노드가 비어있다면 삽입, 꽉 차있다면 분할
	do {

		if (curr->n < M - 1) {					//curr이 아직 꽉 차지 않았다면

			for (i = 0; i < curr->n; i++) {
				if (curr->key[i] > data)		//curr의 key[i]가 data보다 크다면
					break;
			}

			for (j = curr->n; j > i; j--) {		//curr의 키와 branch를 옮겨준다.
				curr->key[j] = curr->key[j - 1];
				curr->branch[j + 1] = curr->branch[j];
			}

			curr->key[i] = data;		//curr의 key[i]에 data 삽입
			curr->branch[i + 1] = p;	//curr의 branch[i+1]은 p가 됨
			(curr->n)++;				//curr의 키 개수 증가
			return 1;
		}

		else {		//삽입 노드가 꽉 차있다면 분할한다

					//먼저 오버플로우 된 노드를 한 사이즈 큰 tempNode노드에 다 복사한다.

			tempNode.n = curr->n + 1;				
			tempNode.branch[0] = curr->branch[0];	

			for (i = 0; i < curr->n; i++) {	

				if (curr->key[i] > data) {		//curr의 key가 data보다 클 경우
					tempNode.key[i] = data;		
					tempNode.branch[i + 1] = p;	
					break;
				}
				else {
					tempNode.key[i] = curr->key[i];				//curr의 key[i]가 tempNode의 key[i]로 들어감
					tempNode.branch[i + 1] = curr->branch[i + 1];	//key[i]의 오른쪽 branch도 복사됨
				}
			}

			if (i < M - 1) {		//i가 M - 1보다 작다면
				for (j = curr->n; j > i; j--) {
					tempNode.key[j] = curr->key[j - 1];		//curr의 key[j-1] 값은 tempNode의 key[j]에 들어간다.
					tempNode.branch[j + 1] = curr->branch[j];		//curr의 branch[j]는 tempNode의 branch[j+1]에 들어간다.
				}
			}
			else {		//tempNode의 남은 공간에 data 삽입!
				tempNode.key[i] = data;
				tempNode.branch[i + 1] = p;
			}


			//이제 분할!

			mid = tempNode.n / 2;		//tempNode의 키개수/2를 mid 변수에 저장
			data = tempNode.key[mid];	//data는 tempNode의 중간 값
			curr->n = mid;			//tempNode의 키개수/2는 curr의 키 개수

			curr->branch[i] = tempNode.branch[0];	//tempNode의 branch[0]은 curr->branch[i]

			for (i = 0; i < mid; i++) {	//tempNode의 mid 기준 왼쪽 키와 branch 모두 curr로 이동
				curr->key[i] = tempNode.key[i];
				curr->branch[i + 1] = tempNode.branch[i + 1];
			}

			newnode = (Btree *)malloc(sizeof(Btree));		//
			newnode->n = M - 1 - mid;						//							
			newnode->branch[0] = tempNode.branch[mid + 1];	//

			j = 0;

			//tempNode의 mid 기준 오른쪽 값 들은 모두 newnode로 
			for (i = mid + 1; i < tempNode.n; i++) {
				newnode->key[j] = tempNode.key[i];
				newnode->branch[++j] = tempNode.branch[i + 1];
			}

			//여기서 newnode는 오른쪽 분할 노드
			p = newnode;
			if (top > -1) {		//stack이 비어있지 않다면
				node = pop();
				curr = node.p;
			}
			else {				//stack이 비어있다면 newnode 왼쪽에 root 오른쪽에 p를 연결해주며 한 단계 올린다.
				newnode = (Btree *)malloc(sizeof(Btree));		
				newnode->n = 1;									//newnode 키개수는 1
				newnode->branch[0] = root;						
				newnode->key[0] = data;							
				newnode->branch[1] = p;							
				root = newnode;									//root는 newnode
				return 1;
			}
		}

	} while (1);

}


int btree_delete(int data) {   // 삭제 함수
	int i, j, k, N, mid;
	int found = 0, finished = 0;		
	Btree *curr, *p, *tmp;				//키 재분배와 노드합병 시 사용될 curr, p, tmp 선언
	Btrees node;						
	SplitBtree splitNode;					//재분배를 위해 사용되는 splitNode 생성

	initStack();						

	if (root == NULL)					//root가 NULL이라면
	{
		printf("No Number Deleted.\n");
		return 0;
	}

	curr = root;						//curr에 root 복사
	found = 0;

	//루트 노드가 NULL이 아니라면 들어갈 branch 위치를 찾는다.
	//삭제할 노드를 찾는다.
	do {

		p = NULL;			
		N = curr->n;		

		for (i = 0; i < N; i++) {
			if (curr->key[i] == data) {		//삭제할 데이터가 있다면
				k = i;						
				found = 1;					
				break;
			}
			else if (curr->key[i] > data) {		//curr의 key[i]보다 data가 작다면
				p = curr->branch[i];			//p는 curr의 branch[i]로 들어간다.
				k = i;							//k에 위치 저장
				break;
			}
		}

		if (i - 1 >= 0 && found == 0) {		//들어갈 곳을 아직 못 찾았을 경우

			if (curr->key[i - 1] < data) {		//data가 curr의 모든 key보다 클 경우
				p = curr->branch[i];			//p는 curr의 가장 오른쪽 branch
				k = i - 1;						//k에 위치 저장
			}

		}

		//항상 부모는 curr 자식은 p
		//부모를 Stack에 넣어준다.

		if (p != NULL) {		//branch로 들어왔을 경우

			//새로운 노드 Btrees의 p에는 부모노드를 저장하고 n에는 부모노드의 어느 키 오른쪽 브랜치로 들어왔는지 저장한다.

			node.p = curr;		
			node.n = k;			
			push(node);			//초기화한 Btrees 노드 구조체를 Stack에 push
			curr = p;			//자식 노드가 부모 노드가 된다.
		}
	} while (found == 0 && p != NULL);		//p가 NULL이 아니라면 


	if (found == 0)		//삭제했다면
		return 0;

	if (curr->branch[k + 1] != NULL) {		//삭제할 노드의 오른쪽에 자식이 있다면
		tmp = curr;							
		node.p = curr;						
		node.n = k;							
		push(node);							//마지막으로 삭제할노드도 Stack에 push

		curr = curr->branch[k + 1];			//curr을 curr의 오른쪽 branch로 저장해 내려간다.

		//삭제할 키의 후행키를 찾는다.
		while (curr->branch[0] != NULL) {		//curr의 0번째 branch가 NULL이 아니라면 
			node.p = curr;						
			node.n = 0;							
			push(node);							
			curr = curr->branch[0];				//curr을 curr의 0번째 branch로 저장해 내려간다.
		}

		tmp->key[k] = curr->key[0];				//삭제할 키 위치에 후행키를 복사한다.
		data = curr->key[0];					//삭제할 데이터에 후행키를 복사한다.					 
	}

	finished = 0;			//아직 삭제하지 못했음


	do {

		j = 0;
		for (i = 0; i < curr->n; i++) {		//삭제할 키가 있는 리프노드 curr의 키 개수만큼 반복문 실행
			if (curr->key[i] != data) {			//curr의 key가 삭제할 data와 같지 않다면
				curr->key[j] = curr->key[i];	//curr의 key[i]를 curr의 key[j]에 복사
				curr->branch[j + 1] = curr->branch[i + 1];		//curr의 branch[i+1]을 curr의 branch[j+1]에 복사!
				j++;			
			}
		}
		(curr->n)--;		//지웠다면 curr의 키 개수는 1 감소


		//키를 지운 뒤에도 curr의 키 개수가 0보다 크다면 끝
		if (curr->n > 0)
			return 1;

		//키를 지운 뒤에 Btree의 level이 감소된 경우
		else if (top == -1) {
			tmp = root;			//root를 tmp에 복사
			root = root->branch[0];			//root의 0번째 branch를 root로 설정
			if (tmp)		//tmp를 지워주는 역할
				free(tmp);
			return 1;
		}

		//키를 지운 뒤에 node가 언더플로라면 bestSibling과 재분배
		else if (stack[top].p->branch[stack[top].n]->n > 1 || stack[top].p->branch[stack[top].n + 1]->n > 1) {
			//형제노드의 키 개수가 1보다 크다면 재분배가 가능하다.

			if (stack[top].p->branch[stack[top].n]->n > 1) {	//왼쪽 형제의 키 개수가 1보다 크다면

				tmp = stack[top].p->branch[stack[top].n];		//tmp는 왼쪽 형제 노드가 된다.

				j = 0;
				for (i = 0; i < tmp->n; i++) {			
					splitNode.branch[j] = tmp->branch[i];		//splitNode에 tmp의 모든 branch를 복사한다.
					splitNode.key[j] = tmp->key[i];				//splitNode에 tmp의 모든 key를 복사한다.
					j++;
				}

				splitNode.branch[j] = tmp->branch[i];			//tmp의 마지막 branch도 splitNode에 복사한다.
				splitNode.key[j] = stack[top].p->key[stack[top].n];	//부모 노드에서 tmp의 키들보다 큰 데이터를 가져와 splitNode에 복사한다. 
				j++;		//splitNode의 인덱스 표시 역할의 j 증가

				for (i = 0; i < curr->n; i++) {				//부모의 오른쪽 노드에도 남은 키들이 있으면 splitNode에 복사한다.
					splitNode.branch[j] = curr->branch[i];
					splitNode.key[j] = curr->key[i];
				}
				splitNode.branch[j] = curr->branch[i];
				splitNode.n = j;	//splitNode에 키 개수 초기화

				mid = j / 2;	//부모노드로 올라갈 중간값 선언
				for (i = 0; i < mid; i++) {		//splitNode에서 중간값을 정했으니 중간값 기준 왼쪽은 tmp로
					tmp->branch[i] = splitNode.branch[i];
					tmp->key[i] = splitNode.key[i];
				}
				tmp->branch[i] = splitNode.branch[i];

				tmp->n = mid;		//왼쪽 자식인 tmp의 키 개수 수정

				stack[top].p->key[stack[top].n] = splitNode.key[mid];	//부모에 중간값 올라간다.

				for (i = mid + 1; i < splitNode.n; i++) {		//splitNode에서 중간값을 정했으니 중간값 기준 오른쪽은 curr로
					curr->branch[i - mid - 1] = splitNode.branch[i];
					curr->key[i - mid - 1] = splitNode.key[i];
				}
				curr->branch[mid] = splitNode.branch[i];		//curr의 가장 오른쪽 branch 할당
				curr->n = splitNode.n - mid - 1;		//오른쪽 자식인 curr의 키 개수 수정

			}
			else {		//오른쪽 형제의 키 개수가 1보다 크다면

				tmp = stack[top].p->branch[stack[top].n + 1];		//tmp는 오른쪽 형제 노드가 된다.

				j = 0;
				for (i = 0; i < curr->n; i++) {				//tmp의 키 개수 만큼 반복문
					splitNode.branch[j] = curr->branch[i];		//splitNode에 curr의 모든 branch를 복사한다.
					splitNode.key[j] = curr->key[i];				//splitNode에 curr의 모든 key를 복사한다.
					j++;
				}

				splitNode.branch[j] = curr->branch[i];			//curr의 마지막 branch도 splitNode에 복사한다.
				splitNode.key[j] = stack[top].p->key[stack[top].n];		//부모 노드에서 curr의 키들보다 큰 데이터를 가져와 splitNode에 복사한다.
				j++;		//splitNode의 인덱스 표시 역할의 j 증가

				for (i = 0; i < tmp->n; i++) {			//부모의 왼쪽 노드에도 남은 키들이 있으면 splitNode에 복사한다.
					splitNode.branch[j] = tmp->branch[i];
					splitNode.key[j] = tmp->key[i];
					j++;
				}

				splitNode.branch[j] = tmp->branch[i];
				splitNode.n = j;		//splitNode에 키 개수 초기화

				mid = j / 2;		//부모노드로 올라갈 중간값 선언
				for (i = 0; i < mid; i++) {		//splitNode에서 중간값을 정했으니 중간값 기준 왼쪽은 curr로!
					curr->branch[i] = splitNode.branch[i];
					curr->key[i] = splitNode.key[i];
				}
				curr->branch[i] = splitNode.branch[i];

				curr->n = mid;		//오른쪽 자식인 curr의 키 개수 수정

				stack[top].p->key[stack[top].n] = splitNode.key[mid];	//부모에 중간값 올라간다.

				for (i = mid + 1; i < splitNode.n; i++) {		//splitNode에서 중간값을 정했으니 중간값 기준 왼쪽은 tmp로!
					tmp->branch[i - mid - 1] = splitNode.branch[i];
					tmp->key[i - mid - 1] = splitNode.key[i];
				}
				tmp->branch[i - mid - 1] = splitNode.branch[i];	//tmp의 가장 오른쪽 branch 할당
				tmp->n = splitNode.n - mid - 1;		//왼쪽 자신인 tmp의 키 개수 수정

			}
			return 1;
		}


		//위의 조건문에 들어가지 않는다면 bestsibling과 합병
		else {
			if (stack[top].p->branch[stack[top].n]->n > 0) {		 //부모의 왼쪽 자식이 0보다 크다면
				tmp = stack[top].p->branch[stack[top].n];				//tmp는 부모의 왼쪽 자식이 된다.				
				tmp->key[tmp->n] = stack[top].p->key[stack[top].n];		//tmp의 첫번째로 비어있는 키 위치에 부모의 데이터 추가						
				(tmp->n)++;												
				j = tmp->n;												

				for (i = 0; i < curr->n; i++) {					//부모의 오른쪽 자식(curr)에 아직 키가 남아있다면 부모의 왼쪽 자식(tmp)에 다 옮겨준다.
					tmp->branch[j + i] = curr->branch[i];
					tmp->key[j + i] = curr->key[i];
				}

				tmp->branch[j + i] = curr->branch[i];		//부모의 오른쪽 자식(curr)의 가장 오른쪽 branch도 부모의 왼쪽 자식으로(tmp) 옮긴다.

				tmp->n = tmp->n + curr->n;					//부모의 왼쪽 자식(tmp)의 키 개수는 부모의 오른쪽 자식(curr)의 키 개수와 합한 값이 된다.

				if (curr)				//키를 지웠으니 노드 지워준다.
					free(curr);

				data = stack[top].p->key[stack[top].n];		//data 초기화
				node = pop();								//Stack POP 실행
				curr = node.p;								//부모 노드는 curr이 된다.
			}

			else {
				tmp = stack[top].p->branch[stack[top].n + 1];		//tmp는 부모의 오른쪽 자식이 된다.
				curr->key[curr->n] = stack[top].p->key[stack[top].n];	//curr의 첫번째로 비어있는 키 위치에 부모의 데이터 추가
				(curr->n)++;			//curr의 키 개수 증가
				j = curr->n;			//j는 curr의 키 개수

				for (i = 0; i < tmp->n; i++) {		//부모의 왼쪽 자식(temp)에 아직 키가 남아있다면 부모의 오른쪽 자식(curr)에 다 옮겨준다.
					curr->branch[j + i] = tmp->branch[i];
					curr->key[j + i] = tmp->key[i];
				}

				curr->branch[j + i] = tmp->branch[i];		//부모의 왼쪽 자식(tmp)의 가장 오른쪽 branch도 부모의 오른쪽 자식으로(curr) 옮긴다.
				curr->n = curr->n + tmp->n;				//부모의 오른쪽 자식(curr)의 키 개수는 부모의 왼쪽 자식(tmp)의 키 개수와 합한 값이 된다.

				if (tmp)			//키를 지웠으니 노드 지워준다.
					free(tmp);

				data = stack[top].p->key[stack[top].n];		//data 초기화
				node = pop();								//Stack POP 실행
				curr = node.p;								//부모 노드는 curr이 된다.
			}

		}
	} while (1);
}

void btree_inorder(struct Btree *root)   // Btree를 인오더형식으로 출력
{
	int i, j;
	if (root != NULL) {
		for (i = 0; i < root->n; i++) {				//root의 키 개수만큼
			btree_inorder(root->branch[i]);		//root의 각 branch로 들어간다.

			printf("%3d", root->key[i]);			//root의 key 출력
			printf(" ");
		}
		btree_inorder(root->branch[i]);	//root의 각 branch로 들어간다.
	}
}



void main()
{
	//#define M 3, M 4 삽입
	printf("삽입 후 인오더 출력 (단, 분기 m = 3일때) \n");
	printf("\n");
	btree_insert(30);
	btree_insert(20);
	btree_insert(62);
	btree_insert(110);
	btree_insert(140);
	btree_insert(15);
	btree_insert(65);
	btree_insert(136);
	btree_insert(150);
	btree_insert(120);

	btree_insert(40);
	btree_insert(132);
	btree_insert(19);
	btree_insert(128);
	btree_insert(138);
	btree_insert(100);
	btree_insert(16);
	btree_insert(145);
	btree_insert(70);
	btree_insert(42);

	btree_insert(69);
	btree_insert(43);
	btree_insert(26);
	btree_insert(60);
	btree_insert(130);
	btree_insert(50);
	btree_insert(18);
	btree_insert(7);
	btree_insert(36);
	btree_insert(58);

	btree_insert(22);
	btree_insert(41);
	btree_insert(59);
	btree_insert(57);
	btree_insert(54);
	btree_insert(33);
	btree_insert(75);
	btree_insert(124);
	btree_insert(122);
	btree_insert(123);	btree_inorder(root); printf("\n");

	printf("\n");
	printf("삭제(40부터 삭제시작)");
	printf("\n");

	//삭제
	btree_delete(40);	btree_inorder(root); printf("\n");
	btree_delete(132);	btree_inorder(root); printf("\n");
	btree_delete(19);	btree_inorder(root); printf("\n");
	btree_delete(128);	btree_inorder(root); printf("\n");
	btree_delete(138);	btree_inorder(root); printf("\n");
	btree_delete(100);	btree_inorder(root); printf("\n");
	btree_delete(16);	btree_inorder(root); printf("\n");
	btree_delete(145);	btree_inorder(root); printf("\n");
	btree_delete(70);	btree_inorder(root); printf("\n");
	btree_delete(42);	btree_inorder(root); printf("\n");

	btree_delete(22);	btree_inorder(root); printf("\n");
	btree_delete(41);	btree_inorder(root); printf("\n");
	btree_delete(62);	btree_inorder(root); printf("\n");
	btree_delete(110);	btree_inorder(root); printf("\n");
	btree_delete(140);	btree_inorder(root); printf("\n");
	btree_delete(15);	btree_inorder(root); printf("\n");
	btree_delete(65);	btree_inorder(root); printf("\n");
	btree_delete(124);	btree_inorder(root); printf("\n");
	btree_delete(122);	btree_inorder(root); printf("\n");
	btree_delete(123);	btree_inorder(root); printf("\n");

	btree_delete(30);	btree_inorder(root); printf("\n");
	btree_delete(20);	btree_inorder(root); printf("\n");
	btree_delete(59);	btree_inorder(root); printf("\n");
	btree_delete(57);	btree_inorder(root); printf("\n");
	btree_delete(54);	btree_inorder(root); printf("\n");
	btree_delete(33);	btree_inorder(root); printf("\n");
	btree_delete(75);	btree_inorder(root); printf("\n");
	btree_delete(136);	btree_inorder(root); printf("\n");
	btree_delete(150);	btree_inorder(root); printf("\n");
	btree_delete(120);	btree_inorder(root); printf("\n");

	btree_delete(69);	btree_inorder(root); printf("\n");
	btree_delete(43);	btree_inorder(root); printf("\n");
	btree_delete(26);	btree_inorder(root); printf("\n");
	btree_delete(60);	btree_inorder(root); printf("\n");
	btree_delete(130);	btree_inorder(root); printf("\n");
	btree_delete(50);	btree_inorder(root); printf("\n");
	btree_delete(18);	btree_inorder(root); printf("\n");
	btree_delete(7);	btree_inorder(root); printf("\n");
	btree_delete(36);	btree_inorder(root); printf("\n");
	btree_delete(58);	btree_inorder(root); printf("\n");
	btree_delete(58);	btree_inorder(root); printf("\n");

}

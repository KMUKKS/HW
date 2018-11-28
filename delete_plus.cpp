#include<iostream>
#include <cstdio>
#define M			4		//�б�M(����)�� 4�� �� (���ϴ� �б⸸ŭ ����)
#define MAX_STACK	1000

class Btree {		// Btree ����ü: Ű�� ������ M-1�� �̰� ������ M
public:

	int n;
	int key[M - 1];
	class Btree *branch[M];

};

class ParentBtree {		// Btree ����ü : ������ ���� ���Ǵ� Ű�� ������ M�� �̰� ������ M+1
public:

	int n;
	int key[M];
	class Btree *branch[M + 1];

};

class SplitBtree {		// Btree ����ü :��й踦 ���� ���Ǵ� Ű�� ������ 2*(M-1)�� �̰� ������ 2 * (M - 1) + 1
public:

	int n;
	int key[2 * (M - 1)];
	class Btree *branch[2 * (M - 1) + 1];

};

class Btrees {		//Btree ����ü : btree ��� �����͸� ������ ����
public:

	int n;
	class Btree *p;
};

void initStack();			//Stack �ʱ�ȭ

Btrees stack[MAX_STACK];	//������ ���� Stack

int top = -1;				//���� top�κ� �ʱ�ȭ

Btree *root = NULL;			//Btree�� root�� NULL�� �ʱ�ȭ

Btrees pop();				//Stack���� ��带 pop ���ִ� �Լ�

void push(Btrees node);		//Stack�� ��带 push ���ִ� �Լ�

int btree_insert(int);		//Btree�� ��带 �ִ� �Լ�

void initStack() {		//Stack �ʱ�ȭ �Լ�
	top = -1;
}

Btrees pop() {		//Stack���� ��带 pop ���ִ� �Լ�
	Btrees node;
	node = stack[top];
	top--;
	return node;
}

void push(Btrees node) {   //Stack�� ��带 push ���ִ� �Լ�
	top++;
	stack[top] = node;
}

int btree_insert(int data) {			//Btree�� ��带 �ִ� �Լ�
	int i, j, N, mid;
	int found = 0, finished = 0;		//������ ������ ã�Ҵ��� �����ϴ� found ������ �����ߴ��� �����ϴ� finished ���� ����

	Btree *curr, *p, *newnode;			//���� �� ���� curr, p, newnode ����
	Btrees node;
	ParentBtree tempNode;						//�����÷ο� �ذ��ϴ� ����

	initStack();	

	if (root == NULL) {				//��Ʈ ��尡 NULL�̶��

		root = (Btree *)malloc(sizeof(Btree));		
		root->n = 1;								//��Ʈ Ű ���� 1�� �Ҵ�
		root->key[0] = data;						//��Ʈ�� 0��° Ű�� �Ҵ�
		for (i = 0; i <= 1; i++)					
			root->branch[i] = NULL;
		return 1;
	}

	curr = root;		//root��带 curr�� ����
	found = 0;

	//��Ʈ ��尡 NULL�� �ƴ϶�� �� ��ġ�� ã�´�.
	do {
		p = NULL;			
		N = curr->n;		//curr�� Ű ������ ���� N�� ����

		for (i = 0; i < N; i++) {
			if (curr->key[i] == data) {	//data�� �̹� �ִٸ� 
				found = 1;				//ã�����Ƿ� found = 1
				break;
			}
			else if (curr->key[i] > data) {	//data���� curr�� key[i]�� �� ũ�ٸ�
				p = curr->branch[i];			//p�� curr�� branch[i]�� ����
				break;
			}
		}

		if (curr->key[i - 1] < data && p == NULL)	//data�� curr�� key�� �� ���� Ŭ ���
			p = curr->branch[i];		//curr�� ������ branch�� ����.

		if (p != NULL) {		//�� ���� ã���� ���
			node.p = curr;
			push(node);			//p�� stack�� �־��ش�.
			curr = p;			//p�� curr�� �������ش�.
		}

	} while (found == 0 && p != NULL);	//���� ��ġ�� ���� �� ã�Ҵٸ�

	if (found == 1) return 0;		//�����ߴٸ�

	p = NULL;
	finished = 0;

	//���� ��尡 ����ִٸ� ����, �� ���ִٸ� ����
	do {

		if (curr->n < M - 1) {					//curr�� ���� �� ���� �ʾҴٸ�

			for (i = 0; i < curr->n; i++) {
				if (curr->key[i] > data)		//curr�� key[i]�� data���� ũ�ٸ�
					break;
			}

			for (j = curr->n; j > i; j--) {		//curr�� Ű�� branch�� �Ű��ش�.
				curr->key[j] = curr->key[j - 1];
				curr->branch[j + 1] = curr->branch[j];
			}

			curr->key[i] = data;		//curr�� key[i]�� data ����
			curr->branch[i + 1] = p;	//curr�� branch[i+1]�� p�� ��
			(curr->n)++;				//curr�� Ű ���� ����
			return 1;
		}

		else {		//���� ��尡 �� ���ִٸ� �����Ѵ�

					//���� �����÷ο� �� ��带 �� ������ ū tempNode��忡 �� �����Ѵ�.

			tempNode.n = curr->n + 1;				
			tempNode.branch[0] = curr->branch[0];	

			for (i = 0; i < curr->n; i++) {	

				if (curr->key[i] > data) {		//curr�� key�� data���� Ŭ ���
					tempNode.key[i] = data;		
					tempNode.branch[i + 1] = p;	
					break;
				}
				else {
					tempNode.key[i] = curr->key[i];				//curr�� key[i]�� tempNode�� key[i]�� ��
					tempNode.branch[i + 1] = curr->branch[i + 1];	//key[i]�� ������ branch�� �����
				}
			}

			if (i < M - 1) {		//i�� M - 1���� �۴ٸ�
				for (j = curr->n; j > i; j--) {
					tempNode.key[j] = curr->key[j - 1];		//curr�� key[j-1] ���� tempNode�� key[j]�� ����.
					tempNode.branch[j + 1] = curr->branch[j];		//curr�� branch[j]�� tempNode�� branch[j+1]�� ����.
				}
			}
			else {		//tempNode�� ���� ������ data ����!
				tempNode.key[i] = data;
				tempNode.branch[i + 1] = p;
			}


			//���� ����!

			mid = tempNode.n / 2;		//tempNode�� Ű����/2�� mid ������ ����
			data = tempNode.key[mid];	//data�� tempNode�� �߰� ��
			curr->n = mid;			//tempNode�� Ű����/2�� curr�� Ű ����

			curr->branch[i] = tempNode.branch[0];	//tempNode�� branch[0]�� curr->branch[i]

			for (i = 0; i < mid; i++) {	//tempNode�� mid ���� ���� Ű�� branch ��� curr�� �̵�
				curr->key[i] = tempNode.key[i];
				curr->branch[i + 1] = tempNode.branch[i + 1];
			}

			newnode = (Btree *)malloc(sizeof(Btree));		//
			newnode->n = M - 1 - mid;						//							
			newnode->branch[0] = tempNode.branch[mid + 1];	//

			j = 0;

			//tempNode�� mid ���� ������ �� ���� ��� newnode�� 
			for (i = mid + 1; i < tempNode.n; i++) {
				newnode->key[j] = tempNode.key[i];
				newnode->branch[++j] = tempNode.branch[i + 1];
			}

			//���⼭ newnode�� ������ ���� ���
			p = newnode;
			if (top > -1) {		//stack�� ������� �ʴٸ�
				node = pop();
				curr = node.p;
			}
			else {				//stack�� ����ִٸ� newnode ���ʿ� root �����ʿ� p�� �������ָ� �� �ܰ� �ø���.
				newnode = (Btree *)malloc(sizeof(Btree));		
				newnode->n = 1;									//newnode Ű������ 1
				newnode->branch[0] = root;						
				newnode->key[0] = data;							
				newnode->branch[1] = p;							
				root = newnode;									//root�� newnode
				return 1;
			}
		}

	} while (1);

}


int btree_delete(int data) {   // ���� �Լ�
	int i, j, k, N, mid;
	int found = 0, finished = 0;		
	Btree *curr, *p, *tmp;				//Ű ��й�� ����պ� �� ���� curr, p, tmp ����
	Btrees node;						
	SplitBtree splitNode;					//��й踦 ���� ���Ǵ� splitNode ����

	initStack();						

	if (root == NULL)					//root�� NULL�̶��
	{
		printf("No Number Deleted.\n");
		return 0;
	}

	curr = root;						//curr�� root ����
	found = 0;

	//��Ʈ ��尡 NULL�� �ƴ϶�� �� branch ��ġ�� ã�´�.
	//������ ��带 ã�´�.
	do {

		p = NULL;			
		N = curr->n;		

		for (i = 0; i < N; i++) {
			if (curr->key[i] == data) {		//������ �����Ͱ� �ִٸ�
				k = i;						
				found = 1;					
				break;
			}
			else if (curr->key[i] > data) {		//curr�� key[i]���� data�� �۴ٸ�
				p = curr->branch[i];			//p�� curr�� branch[i]�� ����.
				k = i;							//k�� ��ġ ����
				break;
			}
		}

		if (i - 1 >= 0 && found == 0) {		//�� ���� ���� �� ã���� ���

			if (curr->key[i - 1] < data) {		//data�� curr�� ��� key���� Ŭ ���
				p = curr->branch[i];			//p�� curr�� ���� ������ branch
				k = i - 1;						//k�� ��ġ ����
			}

		}

		//�׻� �θ�� curr �ڽ��� p
		//�θ� Stack�� �־��ش�.

		if (p != NULL) {		//branch�� ������ ���

			//���ο� ��� Btrees�� p���� �θ��带 �����ϰ� n���� �θ����� ��� Ű ������ �귣ġ�� ���Դ��� �����Ѵ�.

			node.p = curr;		
			node.n = k;			
			push(node);			//�ʱ�ȭ�� Btrees ��� ����ü�� Stack�� push
			curr = p;			//�ڽ� ��尡 �θ� ��尡 �ȴ�.
		}
	} while (found == 0 && p != NULL);		//p�� NULL�� �ƴ϶�� 


	if (found == 0)		//�����ߴٸ�
		return 0;

	if (curr->branch[k + 1] != NULL) {		//������ ����� �����ʿ� �ڽ��� �ִٸ�
		tmp = curr;							
		node.p = curr;						
		node.n = k;							
		push(node);							//���������� �����ҳ�嵵 Stack�� push

		curr = curr->branch[k + 1];			//curr�� curr�� ������ branch�� ������ ��������.

		//������ Ű�� ����Ű�� ã�´�.
		while (curr->branch[0] != NULL) {		//curr�� 0��° branch�� NULL�� �ƴ϶�� 
			node.p = curr;						
			node.n = 0;							
			push(node);							
			curr = curr->branch[0];				//curr�� curr�� 0��° branch�� ������ ��������.
		}

		tmp->key[k] = curr->key[0];				//������ Ű ��ġ�� ����Ű�� �����Ѵ�.
		data = curr->key[0];					//������ �����Ϳ� ����Ű�� �����Ѵ�.					 
	}

	finished = 0;			//���� �������� ������


	do {

		j = 0;
		for (i = 0; i < curr->n; i++) {		//������ Ű�� �ִ� ������� curr�� Ű ������ŭ �ݺ��� ����
			if (curr->key[i] != data) {			//curr�� key�� ������ data�� ���� �ʴٸ�
				curr->key[j] = curr->key[i];	//curr�� key[i]�� curr�� key[j]�� ����
				curr->branch[j + 1] = curr->branch[i + 1];		//curr�� branch[i+1]�� curr�� branch[j+1]�� ����!
				j++;			
			}
		}
		(curr->n)--;		//�����ٸ� curr�� Ű ������ 1 ����


		//Ű�� ���� �ڿ��� curr�� Ű ������ 0���� ũ�ٸ� ��
		if (curr->n > 0)
			return 1;

		//Ű�� ���� �ڿ� Btree�� level�� ���ҵ� ���
		else if (top == -1) {
			tmp = root;			//root�� tmp�� ����
			root = root->branch[0];			//root�� 0��° branch�� root�� ����
			if (tmp)		//tmp�� �����ִ� ����
				free(tmp);
			return 1;
		}

		//Ű�� ���� �ڿ� node�� ����÷ζ�� bestSibling�� ��й�
		else if (stack[top].p->branch[stack[top].n]->n > 1 || stack[top].p->branch[stack[top].n + 1]->n > 1) {
			//��������� Ű ������ 1���� ũ�ٸ� ��й谡 �����ϴ�.

			if (stack[top].p->branch[stack[top].n]->n > 1) {	//���� ������ Ű ������ 1���� ũ�ٸ�

				tmp = stack[top].p->branch[stack[top].n];		//tmp�� ���� ���� ��尡 �ȴ�.

				j = 0;
				for (i = 0; i < tmp->n; i++) {			
					splitNode.branch[j] = tmp->branch[i];		//splitNode�� tmp�� ��� branch�� �����Ѵ�.
					splitNode.key[j] = tmp->key[i];				//splitNode�� tmp�� ��� key�� �����Ѵ�.
					j++;
				}

				splitNode.branch[j] = tmp->branch[i];			//tmp�� ������ branch�� splitNode�� �����Ѵ�.
				splitNode.key[j] = stack[top].p->key[stack[top].n];	//�θ� ��忡�� tmp�� Ű�麸�� ū �����͸� ������ splitNode�� �����Ѵ�. 
				j++;		//splitNode�� �ε��� ǥ�� ������ j ����

				for (i = 0; i < curr->n; i++) {				//�θ��� ������ ��忡�� ���� Ű���� ������ splitNode�� �����Ѵ�.
					splitNode.branch[j] = curr->branch[i];
					splitNode.key[j] = curr->key[i];
				}
				splitNode.branch[j] = curr->branch[i];
				splitNode.n = j;	//splitNode�� Ű ���� �ʱ�ȭ

				mid = j / 2;	//�θ���� �ö� �߰��� ����
				for (i = 0; i < mid; i++) {		//splitNode���� �߰����� �������� �߰��� ���� ������ tmp��
					tmp->branch[i] = splitNode.branch[i];
					tmp->key[i] = splitNode.key[i];
				}
				tmp->branch[i] = splitNode.branch[i];

				tmp->n = mid;		//���� �ڽ��� tmp�� Ű ���� ����

				stack[top].p->key[stack[top].n] = splitNode.key[mid];	//�θ� �߰��� �ö󰣴�.

				for (i = mid + 1; i < splitNode.n; i++) {		//splitNode���� �߰����� �������� �߰��� ���� �������� curr��
					curr->branch[i - mid - 1] = splitNode.branch[i];
					curr->key[i - mid - 1] = splitNode.key[i];
				}
				curr->branch[mid] = splitNode.branch[i];		//curr�� ���� ������ branch �Ҵ�
				curr->n = splitNode.n - mid - 1;		//������ �ڽ��� curr�� Ű ���� ����

			}
			else {		//������ ������ Ű ������ 1���� ũ�ٸ�

				tmp = stack[top].p->branch[stack[top].n + 1];		//tmp�� ������ ���� ��尡 �ȴ�.

				j = 0;
				for (i = 0; i < curr->n; i++) {				//tmp�� Ű ���� ��ŭ �ݺ���
					splitNode.branch[j] = curr->branch[i];		//splitNode�� curr�� ��� branch�� �����Ѵ�.
					splitNode.key[j] = curr->key[i];				//splitNode�� curr�� ��� key�� �����Ѵ�.
					j++;
				}

				splitNode.branch[j] = curr->branch[i];			//curr�� ������ branch�� splitNode�� �����Ѵ�.
				splitNode.key[j] = stack[top].p->key[stack[top].n];		//�θ� ��忡�� curr�� Ű�麸�� ū �����͸� ������ splitNode�� �����Ѵ�.
				j++;		//splitNode�� �ε��� ǥ�� ������ j ����

				for (i = 0; i < tmp->n; i++) {			//�θ��� ���� ��忡�� ���� Ű���� ������ splitNode�� �����Ѵ�.
					splitNode.branch[j] = tmp->branch[i];
					splitNode.key[j] = tmp->key[i];
					j++;
				}

				splitNode.branch[j] = tmp->branch[i];
				splitNode.n = j;		//splitNode�� Ű ���� �ʱ�ȭ

				mid = j / 2;		//�θ���� �ö� �߰��� ����
				for (i = 0; i < mid; i++) {		//splitNode���� �߰����� �������� �߰��� ���� ������ curr��!
					curr->branch[i] = splitNode.branch[i];
					curr->key[i] = splitNode.key[i];
				}
				curr->branch[i] = splitNode.branch[i];

				curr->n = mid;		//������ �ڽ��� curr�� Ű ���� ����

				stack[top].p->key[stack[top].n] = splitNode.key[mid];	//�θ� �߰��� �ö󰣴�.

				for (i = mid + 1; i < splitNode.n; i++) {		//splitNode���� �߰����� �������� �߰��� ���� ������ tmp��!
					tmp->branch[i - mid - 1] = splitNode.branch[i];
					tmp->key[i - mid - 1] = splitNode.key[i];
				}
				tmp->branch[i - mid - 1] = splitNode.branch[i];	//tmp�� ���� ������ branch �Ҵ�
				tmp->n = splitNode.n - mid - 1;		//���� �ڽ��� tmp�� Ű ���� ����

			}
			return 1;
		}


		//���� ���ǹ��� ���� �ʴ´ٸ� bestsibling�� �պ�
		else {
			if (stack[top].p->branch[stack[top].n]->n > 0) {		 //�θ��� ���� �ڽ��� 0���� ũ�ٸ�
				tmp = stack[top].p->branch[stack[top].n];				//tmp�� �θ��� ���� �ڽ��� �ȴ�.				
				tmp->key[tmp->n] = stack[top].p->key[stack[top].n];		//tmp�� ù��°�� ����ִ� Ű ��ġ�� �θ��� ������ �߰�						
				(tmp->n)++;												
				j = tmp->n;												

				for (i = 0; i < curr->n; i++) {					//�θ��� ������ �ڽ�(curr)�� ���� Ű�� �����ִٸ� �θ��� ���� �ڽ�(tmp)�� �� �Ű��ش�.
					tmp->branch[j + i] = curr->branch[i];
					tmp->key[j + i] = curr->key[i];
				}

				tmp->branch[j + i] = curr->branch[i];		//�θ��� ������ �ڽ�(curr)�� ���� ������ branch�� �θ��� ���� �ڽ�����(tmp) �ű��.

				tmp->n = tmp->n + curr->n;					//�θ��� ���� �ڽ�(tmp)�� Ű ������ �θ��� ������ �ڽ�(curr)�� Ű ������ ���� ���� �ȴ�.

				if (curr)				//Ű�� �������� ��� �����ش�.
					free(curr);

				data = stack[top].p->key[stack[top].n];		//data �ʱ�ȭ
				node = pop();								//Stack POP ����
				curr = node.p;								//�θ� ���� curr�� �ȴ�.
			}

			else {
				tmp = stack[top].p->branch[stack[top].n + 1];		//tmp�� �θ��� ������ �ڽ��� �ȴ�.
				curr->key[curr->n] = stack[top].p->key[stack[top].n];	//curr�� ù��°�� ����ִ� Ű ��ġ�� �θ��� ������ �߰�
				(curr->n)++;			//curr�� Ű ���� ����
				j = curr->n;			//j�� curr�� Ű ����

				for (i = 0; i < tmp->n; i++) {		//�θ��� ���� �ڽ�(temp)�� ���� Ű�� �����ִٸ� �θ��� ������ �ڽ�(curr)�� �� �Ű��ش�.
					curr->branch[j + i] = tmp->branch[i];
					curr->key[j + i] = tmp->key[i];
				}

				curr->branch[j + i] = tmp->branch[i];		//�θ��� ���� �ڽ�(tmp)�� ���� ������ branch�� �θ��� ������ �ڽ�����(curr) �ű��.
				curr->n = curr->n + tmp->n;				//�θ��� ������ �ڽ�(curr)�� Ű ������ �θ��� ���� �ڽ�(tmp)�� Ű ������ ���� ���� �ȴ�.

				if (tmp)			//Ű�� �������� ��� �����ش�.
					free(tmp);

				data = stack[top].p->key[stack[top].n];		//data �ʱ�ȭ
				node = pop();								//Stack POP ����
				curr = node.p;								//�θ� ���� curr�� �ȴ�.
			}

		}
	} while (1);
}

void btree_inorder(struct Btree *root)   // Btree�� �ο����������� ���
{
	int i, j;
	if (root != NULL) {
		for (i = 0; i < root->n; i++) {				//root�� Ű ������ŭ
			btree_inorder(root->branch[i]);		//root�� �� branch�� ����.

			printf("%3d", root->key[i]);			//root�� key ���
			printf(" ");
		}
		btree_inorder(root->branch[i]);	//root�� �� branch�� ����.
	}
}



void main()
{
	//#define M 3, M 4 ����
	printf("���� �� �ο��� ��� (��, �б� m = 3�϶�) \n");
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
	printf("����(40���� ��������)");
	printf("\n");

	//����
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
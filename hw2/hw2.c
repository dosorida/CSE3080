#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct listNode {
    struct listNode* prev;
    int data;
    struct listNode* next;
};

typedef struct listNode* listPointer;

listPointer first;
int listLength = 0;

void insert(listPointer list, int data);
void delete(listPointer node, int data);
void print_forward(listPointer list);
void print_reverse(listPointer list);
void free_list(listPointer list);

int main(int argc, char* argv[]) {
    // If the number of command-line arguments is not 1 (zero or more than 1)
    if((argc == 1) || (argc > 2)) {
        printf("usage: ./hw2 input_filename\n");
        exit(1);
    }

    // 파일 열기
    FILE *fp;
	fp = fopen(argv[1], "r");

	// If the input file does not exist
	if(fp == NULL) {
		printf("The input file does not exist.\n");
		exit(1);
	}

    int read;
    char command[10];
    int data = 0;
    
    // 빈 리스트 생성, 초기화
    listPointer list;
    list = malloc(sizeof(struct listNode));
    list->prev = list;
    list->next = list;
    list->data = 0;
    first = list;

    while(1) {
        // 명령문 한 줄씩 읽어오기
        read = fscanf(fp, "%s %d", command, &data);
        
        // input 파일 다 읽으면 루프 종료
        if(read == EOF) break;

        // 원소 삽입
        else if(strcmp(command,"INSERT") == 0) {
            insert(first, data);
        }

        // 원소 삭제
        else if(strcmp(command,"DELETE") == 0) {
            delete(first, data);
        }

        // 오름차순으로 데이터 출력
        else if (strcmp(command,"ASCEND") == 0) {
            print_forward(first);
        }

        // 내림차순으로 데이터 출력
        else if(strcmp(command,"DESCEND") == 0) {
            print_reverse(first);
        }
    }

    // 파일 닫기
    fclose(fp);

    // 메모리 할당 해제
    free_list(first);

    return 0;
}


void insert(listPointer list, int data) {
    int duplicated = 0;
    listPointer curr = list;

    // list에 원소가 없는 경우
    if(listLength == 0) {
        list->data = data;
        listLength++;
    }
    // list에 원소가 있는 경우
    else {
        // 데이터가 중복되는지 검사
        while(1) {
            if(curr->data == data) {
                duplicated = 1;
                break;
            }
            curr = curr->next;
            if(curr == list) break;
        }

        // 데이터가 중복되면 원소 삽입 없이 함수 종료
        if(duplicated == 1) return;

        // 데이터가 중복되지 않는 경우 리스트의 적절한 위치에 노드 삽입
        else {
            // 새로운 노드 생성
            listPointer new;
            new = malloc(sizeof(struct listNode));
            new->data = data;

            curr = list;

            // list에 원소가 하나 있는 경우
            if (listLength == 1) {
                new->prev = curr;
                new->next = curr;
                curr->prev= new;
                curr->next = new;
            }

            // list에 원소가 여러 개 있는 경우
            else {
                while (1) {
                    if (data > curr->data) {
			    	    curr = curr->next;
                        if (curr == list) break;
		    	    }
			        else break;
                }
                new->prev = curr->prev;
                new->next = curr;
                curr->prev->next = new;
                curr->prev = new;
		    }

            listLength++;

            // data를 list의 제일 왼쪽에 삽입하는 경우 first 재설정
            if(data < list->data) first = new;
        }   
    }
}

void delete(listPointer list, int data) {
    listPointer curr = list;
    listPointer deleted;

    // list가 비어 있으면 함수 종료
    if (listLength == 0) return;

    // list의 원소가 하나일 경우
    else if (listLength == 1) {
        free(curr);
        listLength = 0;
    }

    // list의 원소가 여러 개일 경우
    else{
        // list에서 일치하는 data 찾기
        while(1) {
            // 찾으면 리스트에서 노드 삭제
            if (data == curr->data) {
                deleted = curr;
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
                free(deleted);
                listLength--;
                break;
            }
            curr = curr->next;
            if(curr == list) break;
        }
        // list의 제일 왼쪽 node를 삭제하는 경우 first 재설정
        if(data == list->data) first = list->next;
    }
}

void print_forward(listPointer list) {
    listPointer curr;
    FILE *outfile;
    
    outfile = fopen("hw2_result.txt", "a");
    if(list) {
        curr = list;
        while(1) {
            fprintf(outfile, "%d ", curr->data);
            printf("%d ", curr->data);
            curr = curr->next;
            if(curr == list) break;
        }
    }
    fprintf(outfile, "\n");
    printf("\n");
    fclose(outfile); 
}

void print_reverse(listPointer list) {
  listPointer curr;
  FILE *outfile; 
 
  outfile = fopen("hw2_result.txt", "a");
  if(list) {
    curr = list->prev;

    while(curr != list) {
        fprintf(outfile, "%d ", curr->data);
        printf("%d ", curr->data);
        curr = curr->prev;
        }
        fprintf(outfile, "%d ", curr->data);
        printf("%d ", curr->data);
    }
    fprintf(outfile, "\n");
    printf("\n");
    fclose(outfile);
}

void free_list(listPointer list) {
    listPointer curr = list;
    listPointer deleted;
    for (int i = 0; i < listLength - 1; i++) {
        deleted = curr;
        free(deleted);
        curr = curr->next;
    }
    free(curr);
}
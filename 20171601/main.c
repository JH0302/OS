#include "main.h"

struct list List[10];
struct bitmap *Bitmap[10];
struct hash Hash[10];

struct hash_item
{
	struct hash_elem elem;
	int data;
};

bool less_func(const struct list_elem *a, const struct list_elem *b, void *aux){
	struct list_item *tempa = list_entry(a, struct list_item, elem);
	struct list_item *tempb = list_entry(b, struct list_item, elem);

	if(tempa->data >= tempb->data) return 0;
	else return 1;
}

unsigned hash_hash (const struct hash_elem *a, void *aux) {
	struct hash_item *temp = hash_entry(a, struct hash_item, elem);
	return hash_int(temp->data);
}
bool hash_less (const struct hash_elem *a, const struct hash_elem *b, void *aux){
	struct hash_item *tempa = hash_entry(a, struct hash_item, elem);
	struct hash_item *tempb = hash_entry(b, struct hash_item, elem);
	if(tempa->data >= tempb->data) return 0;
	else return 1;
}
void destructor(struct hash_elem *a, void *aux){
	struct hash_item *temp = hash_entry(a, struct hash_item, elem);
	temp=NULL;
}
void square (struct hash_elem *a, void *aux){
	struct hash_item *temp = hash_entry(a, struct hash_item, elem);
	temp->data = (temp->data)*(temp->data);
}
void triple(struct hash_elem *a, void *aux) {
	struct hash_item *temp = hash_entry(a, struct hash_item, elem);
	temp->data = (temp->data)*(temp->data)*(temp->data);
}

int main() {
	char command[100];
	char command_temp[100]; 
	char *ptr;
	char token[20][100];
	int TOKEN_SIZE = 0, count=0;
	int index;

	while(1) {
		fgets(command, sizeof(command), stdin);
		command[strlen(command)-1] = '\0';
		strcpy(command_temp, command);
		
		ptr = strtok(command_temp, " ");

		if(!ptr) continue;
		
		TOKEN_SIZE = 0;

		while(1) {//TOKEN 갯수 세기
			ptr = strtok(NULL, " ");
			TOKEN_SIZE++;
			if(!ptr) break;
		}
		
		//TOKEN 분리
		strcpy(command_temp, command);

		ptr = strtok(command_temp, " ");

		if(!ptr) continue;

		strcpy(token[0], ptr);

		for(int i=1; i<TOKEN_SIZE; i++) {
			ptr = strtok(NULL, " ");
			if (ptr == NULL) break;
			strcpy(token[i], ptr);
		}

		if(strcmp(token[0],"quit")==0) return 0;
		
		if(strcmp(token[0],"create")==0){
			if(strcmp(token[1],"list")==0) {
				if(count <= 10) {
					list_init(&List[count++]);
				}
			}
			else if(strcmp(token[1],"bitmap")==0){
				if(count<=10) {
					int len = atoi(token[3]);
					Bitmap[count++] = bitmap_create(len);	
				}
			}
			else if(strcmp(token[1],"hashtable")==0) {
				if(count<=10) {
					hash_init(&Hash[count++], hash_hash, hash_less, NULL);
				}
			}
		}
		else if(strcmp(token[0],"dumpdata")==0){
			index = token[1][strlen(token[1])-1] - '0';
			token[1][strlen(token[1])-1] = '\0';
			
			if(strcmp(token[1],"list") == 0) {
				if(list_empty(&List[index]) != true) {
					struct list_elem *e;
					for(e = list_begin(&List[index]); e != list_end(&List[index]); e=list_next(e)) {
						struct list_item *temp = list_entry(e, struct list_item, elem);
						printf("%d ",temp->data);
					}
					printf("\n");
				}
			}
			else if(strcmp(token[1],"bm")==0) {
				if(Bitmap[index] != NULL) {
					for(int i=0; i<bitmap_size(Bitmap[index]);i++) {
						if(bitmap_test(Bitmap[index], i)) printf("1");
						else printf("0");
					}
					printf("\n");
				}
			}
			else if(strcmp(token[1],"hash")==0) {
			
				struct hash_iterator it;
				struct hash_item *temp;
				int cnt = 0;
				
				hash_first(&it, &Hash[index]);
				while(hash_next(&it)){
					temp = hash_entry(hash_cur(&it), struct hash_item, elem);
					printf("%d ", temp->data);
					cnt++;
				}
				if(cnt != 0) printf("\n");
			}
		}
		else if(strcmp(token[0],"delete")==0){
			index = token[1][strlen(token[1])-1] - '0';
			token[1][strlen(token[1])-1] = '\0';
			
			if(strcmp(token[1],"list")==0) {
				while(list_empty(&List[index]) == false) {
					struct list_elem *e = list_pop_front(&List[index]);	
					free(e);		
				}
			}
			else if(strcmp(token[1],"bm")==0) bitmap_destroy(Bitmap[index]);

			else if(strcmp(token[1],"hash")==0) hash_destroy(&Hash[index], destructor);
		}

	//list instruction
		else if(strcmp(token[0],"list_push_back")==0){
			index = token[1][4]-'0';
			struct list_elem *e = (struct list_elem*)malloc(sizeof(struct list_elem));
			struct list_item *temp = list_entry(e, struct list_item, elem);
			temp->data = atoi(token[2]); 

			list_push_back(&List[index], e);
		}
		else if(strcmp(token[0],"list_push_front")==0){
			index = token[1][4]-'0';
			struct list_elem *e = (struct list_elem *)malloc(sizeof(struct list_elem));
			struct list_item *temp = list_entry(e, struct list_item, elem);
			temp->data = atoi(token[2]);

			list_push_front(&List[index], e);
		}
		else if(strcmp(token[0],"list_pop_back")==0){
			index = token[1][4]-'0';
			list_pop_back(&List[index]);
		
		}
		else if(strcmp(token[0],"list_pop_front")==0){
			index = token[1][4]-'0';
			list_pop_front(&List[index]);
		}
		else if(strcmp(token[0],"list_front")==0){
			index = token[1][4]-'0';
			struct list_elem *e = list_front(&List[index]);
			struct list_item *temp = list_entry(e, struct list_item, elem);
			
			if(temp != NULL) printf("%d\n", temp->data);
		}
		else if(strcmp(token[0],"list_back")==0){
			index = token[1][4]-'0';
			struct list_elem *e = list_back(&List[index]);
			struct list_item *temp = list_entry(e, struct list_item, elem);

			if(temp !=NULL) printf("%d\n", temp->data);
		}
		else if(strcmp(token[0],"list_insert")==0){
			index = token[1][4]-'0';
			struct list_elem *e = (struct list_elem *)malloc(sizeof(struct list_elem));
			struct list_item *temp = list_entry(e, struct list_item, elem);
			temp->data = atoi(token[3]);

			if((unsigned)list_size(&List[index])-1<atoi(token[2]) || list_empty(&List[index])) {
				list_push_back(&List[index], e);
				continue;
			}

			if(atoi(token[2])==0) {
				list_push_front(&List[index],e);
				continue;
			}

			int cur=0;
			struct list_elem* node;
			for(node = list_begin(&List[index]); node!=list_end(&List[index]); node=list_next(node)) {
				if(cur == atoi(token[2])) {
					list_insert(node,e);
					break;
				}
				cur++;
			}	
		}
		else if(strcmp(token[0],"list_insert_ordered")==0){
			index = token[1][4]-'0';
			struct list_elem *e = (struct list_elem*)malloc(sizeof(struct list_elem));
			struct list_item *temp = list_entry(e, struct list_item, elem);
			temp->data = atoi(token[2]);

			list_insert_ordered(&List[index], e,less_func, NULL);
		
		}
		else if(strcmp(token[0],"list_empty")==0){
			index = token[1][4]-'0';
			if (list_empty(&List[index]) == true) printf("true\n");
			else printf("false\n");
		}
		else if(strcmp(token[0],"list_size")==0){
			index = token[1][4]-'0';
			int size = (int)list_size(&List[index]);
			printf("%d\n",size);
		}
		else if(strcmp(token[0],"list_max")==0){
			index = token[1][4]-'0';
			struct list_elem *e = list_max(&List[index], less_func, NULL); 
			struct list_item *temp = list_entry(e,struct list_item, elem);
			printf("%d\n",temp->data);
		}
		else if(strcmp(token[0],"list_min")==0){
			index = token[1][4]-'0';
			struct list_elem *e = list_min(&List[index], less_func, NULL);
			struct list_item *temp = list_entry(e, struct list_item, elem);
			printf("%d\n",temp->data);
		}
		else if(strcmp(token[0],"list_remove")==0){
			index = token[1][4] - '0';
			struct list_elem *e = list_begin(&List[index]);
			
			for(int k=0;k<atoi(token[2]);k++) {
				e = list_next(e);
			}
			list_remove(e);
		}
		else if(strcmp(token[0],"list_reverse")==0){
			index = token[1][4]-'0';
			list_reverse(&List[index]);
		}
		else if(strcmp(token[0],"list_sort")==0){
			index = token[1][4]-'0';
			list_sort(&List[index],less_func, NULL);
		}
		else if(strcmp(token[0],"list_splice")==0){
			int idx1, idx2;
			int before, first, last;
			
			idx1 = token[1][4]-'0';
			idx2 = token[3][4]-'0';

			before = atoi(token[2]);
			first = atoi(token[4]);
			last = atoi(token[5]);

			struct list_elem *ebefore = list_begin(&List[idx1]);
			struct list_elem *efirst = list_begin(&List[idx2]);
			struct list_elem *elast = list_begin(&List[idx2]);
			
			for(int k=0;k<before;k++) {
				ebefore = list_next(ebefore);
			}

			for(int k=0;k<first;k++) {
				efirst = list_next(efirst);
			}

			
			for(int k=0;k<last;k++) {
				elast = list_next(elast);
			}

			list_splice(ebefore, efirst, elast);
		}
		else if(strcmp(token[0],"list_swap")==0){
			index = token[1][4] - '0';
			struct list_elem *a = list_begin(&List[index]);
			struct list_elem *b = list_begin(&List[index]);
			for(int k=0;k<atoi(token[2]);k++) {
				a = list_next(a);
			}
			for(int k=0;k<atoi(token[3]);k++) {
				b = list_next(b);
			}
			list_swap(a, b);
		}
		else if(strcmp(token[0],"list_unique")==0){
			int idx1 = token[1][4]-'0';

			if(TOKEN_SIZE == 2) list_unique(&List[idx1], NULL, less_func, NULL);

			else if(TOKEN_SIZE == 3){
				int idx2 = token[2][4]-'0';
				list_unique(&List[idx1], &List[idx2], less_func, NULL);
			}
		}
		else if(strcmp(token[0],"list_shuffle")==0){
			index = token[1][4]-'0';
			list_shuffle(&List[index]);
		}
			
	//bitmap instruction
		else if(strcmp(token[0],"bitmap_mark")==0){
			index = token[1][2]-'0';
			int bit = atoi(token[2]);
			bitmap_mark(Bitmap[index], bit);
		}
		else if(strcmp(token[0],"bitmap_all")==0){
			index = token[1][2]-'0';
			int start = atoi(token[2]);
			int cnt = atoi(token[3]);

			if(bitmap_all(Bitmap[index], start, cnt)) printf("true\n");
			else printf("false\n");
		}
		else if(strcmp(token[0],"bitmap_any")==0){
			index = token[1][2]-'0';
			int start = atoi(token[2]);
			int cnt = atoi(token[3]);

			if(bitmap_any(Bitmap[index], start, cnt)) printf("true\n");
			else printf("false\n");
		}
		else if(strcmp(token[0],"bitmap_contains")==0){
			index = token[1][2]-'0';
			int start = atoi(token[2]);
			int cnt = atoi(token[3]);
			bool value = false;

			if (strcmp(token[4], "false") != 0) value = true;

			if((bitmap_contains(Bitmap[index], start, cnt, value))==true) printf("true\n");
			else printf("false\n");

		}
		else if(strcmp(token[0],"bitmap_count")==0){
			index = token[1][2]-'0';
			int start = atoi(token[2]);
			int cnt = atoi(token[3]);
			bool value = false;

			if(strcmp(token[4],"false")!= 0) value = true;
			
			printf("%d\n", (int)bitmap_count(Bitmap[index], start, cnt, value));
		}
		else if(strcmp(token[0],"bitmap_dump")==0){
			index = token[1][2]-'0';
			bitmap_dump(Bitmap[index]);
		}
		else if(strcmp(token[0],"bitmap_expand")==0){
			index = token[1][2]-'0';
			Bitmap[index] = bitmap_expand(Bitmap[index], atoi(token[2]));
		}
		else if(strcmp(token[0],"bitmap_flip")==0){
			index = token[1][2]-'0';
			int cnt = atoi(token[2]);
			bitmap_flip(Bitmap[index], cnt);
		}
		else if(strcmp(token[0],"bitmap_none")==0){
			index = token[1][2]-'0';
			int start = atoi(token[2]);
			int cnt = atoi(token[3]);
			
			if(bitmap_none(Bitmap[index], start, cnt) == true) printf("true\n");
			else printf("false\n");
		}
		else if(strcmp(token[0],"bitmap_reset")==0){
			index = token[1][2]-'0';

			bitmap_reset(Bitmap[index], (unsigned)atoi(token[2]));
		
		}
		else if(strcmp(token[0],"bitmap_scan")==0){
			index = token[1][2]-'0';
			int start = atoi(token[2]);
			int cnt = atoi(token[3]);
			bool value = false;

			if(strcmp(token[4],"false")!=0) value = true;
			printf("%u\n",(unsigned)bitmap_scan(Bitmap[index], start, cnt, value));

		}
		else if(strcmp(token[0],"bitmap_scan_and_flip")==0){
			index = token[1][2]-'0';
			int start = atoi(token[2]);
			int cnt = atoi(token[3]);
			bool value = false;

			if(strcmp(token[4],"false")!=0) value = true;

			printf("%u\n",(unsigned)bitmap_scan_and_flip(Bitmap[index], start, cnt, value));
		}
		else if(strcmp(token[0],"bitmap_set")==0){
			index = token[1][2]-'0';
			int idx = atoi(token[2]);
			bool value = false;

			if(strcmp(token[3],"false")!=0) value = true;

			bitmap_set(Bitmap[index], idx, value);
		}
		else if(strcmp(token[0],"bitmap_set_all")==0){
			index = token[1][2]-'0';
			bool value = false;
			if(strcmp(token[2],"false")!=0) value = true;
			bitmap_set_all(Bitmap[index], value);
		}
		else if(strcmp(token[0],"bitmap_set_multiple")==0){
			index = token[1][2]-'0';
			int start = atoi(token[2]);
			int cnt = atoi(token[3]);
			bool value = false;

			if(strcmp(token[4],"false")!=0) value = true;

			bitmap_set_multiple(Bitmap[index], start, cnt, value);
		}
		else if(strcmp(token[0],"bitmap_size")==0){
			index = token[1][2]-'0';
			printf("%u\n", (unsigned)bitmap_size(Bitmap[index]));	
		}
		else if(strcmp(token[0],"bitmap_test")==0){
			index = token[1][2]-'0';

			if(bitmap_test(Bitmap[index], atoi(token[2])) == true) printf("true\n");
			else printf("false\n");
		}

	//hash instruction
		else if(strcmp(token[0],"hash_apply")==0){
			index = token[1][4]-'0';

			if(strcmp(token[2], "triple")==0) hash_apply(&Hash[index], triple);
			else if(strcmp(token[2], "square")==0) hash_apply(&Hash[index], square);
		}
		else if(strcmp(token[0],"hash_delete")==0){
			index = token[1][4]-'0';

			struct hash_elem *e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item *temp = hash_entry(e, struct hash_item, elem);
			temp->data = atoi(token[2]);

			struct hash_elem *node = hash_delete(&Hash[index], e);
		}
		else if(strcmp(token[0],"hash_clear")==0){
			index = token[1][4]-'0';
			hash_clear(&Hash[index], destructor);
		}
		else if(strcmp(token[0],"hash_size")==0){
			index = token[1][4]-'0';
			printf("%u\n", (unsigned) hash_size(&Hash[index]));
		}
		else if(strcmp(token[0],"hash_empty")==0){
			index = token[1][4]-'0';
			if(hash_empty(&Hash[index])==true) printf("true\n");
			else printf("false\n");
		}
		else if(strcmp(token[0],"hash_find")==0){
			index = token[1][4]-'0';

			struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item* temp = hash_entry(e, struct hash_item, elem);
			temp->data = atoi(token[2]);
			
			struct hash_elem* node = hash_find(&Hash[index], e);

			if(node != NULL){
				printf("%d\n",atoi(token[2]));
			}	
		}
		else if(strcmp(token[0],"hash_insert")==0){
			index = token[1][4]-'0';

			struct hash_item* temp = (struct hash_item*)malloc(sizeof(struct hash_item));
			temp->data = atoi(token[2]);
			hash_insert(&Hash[index], &temp->elem);
		}
		else if(strcmp(token[0],"hash_replace")==0){
			index = token[1][4]-'0';
			struct hash_elem *e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item *temp = hash_entry(e, struct hash_item, elem);
			temp->data = atoi(token[2]);

			hash_replace(&Hash[index], e);
		}
	}
	return 0;
}

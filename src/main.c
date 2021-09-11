#include<stdio.h>
#include<stdlib.h>
#include<stdio_ext.h>
#include<string.h>
#include"../libfdr/fields.h"
#include"../libfdr/jrb.h"
#include"../libfdr/jval.h"
#define MAX 100
#define INFINITY_VALUE 10000000
//trong dpe trai
typedef struct element{
    char soHieu[10];
}element;
static element bangKiHieu[MAX];

int sodinh;
int convert(char* convert){// chuyen tu ki tu nha ga sang so
    for(int i=0;i<MAX;i++){
        if(strcmp(convert,bangKiHieu[i].soHieu)==0)
        return i;
    }
    printf("Chua co thong tin nguoi nay\n");
    return -1;//Khong xuat hien trong bang ki hieu
}

void readFileAddBangKiHieu(char* file){
    IS is;
    int cong_tac = 0;
    int i = 0;
    is = new_inputstruct(file);
    while(get_line(is)>0){
        if(strcmp(is->fields[0], "11") == 0){
            sodinh = atoi(is->fields[0]);
        }else{
            strcpy(bangKiHieu[i].soHieu, is->fields[0]);
            i++;
        }
    }
}

JRB readFileAddTree(JRB root, char* file){
    IS is;
    int cong_tac = 0;
    int i = 0;
    is = new_inputstruct(file);
    while(get_line(is)>0){
        if(strcmp(is->fields[0], "11") != 0){
            for(int i = 1; i < is->NF; i++){
                addEdge(root, convert(is->fields[0]), convert(is->fields[i]), 1);
            }
        }
    }
    return root;
}

JRB renderTree(JRB root){
    JRB tmp1, tmp2, tmp3;

    jrb_traverse(tmp1,root){
        printf("%s ", bangKiHieu[tmp1->key.i].soHieu);
        tmp2 = (JRB)tmp1->val.v;
        jrb_traverse(tmp3, tmp2){
            printf("%s ", bangKiHieu[tmp3->key.i].soHieu);
        }
        printf("\n");
    }
    return root;
}

JRB case4(JRB root, char* people, char* string){
    JRB tmp1, tmp2, tmp3;

    jrb_traverse(tmp1,root){
        if(strcmp(bangKiHieu[tmp1->key.i].soHieu, people) == 0){
        tmp2 = (JRB)tmp1->val.v;
        printf("%s", string);
        jrb_traverse(tmp3, tmp2){
            printf("%s ", bangKiHieu[tmp3->key.i].soHieu);
        }
        printf("\n");
        }
    }
    return root;
}

void case5(JRB root){
    JRB tmp1, tmp2, tmp3;
    int output[MAX];
    int list[MAX];
    int max = 0;
    jrb_traverse(tmp1, root){
        list[tmp1->key.i] = outdegree(root, tmp1->key.i, output);
        if(list[tmp1->key.i] > max){
            max = list[tmp1->key.i];
        }
    }
    printf("Nhung nguoi tiep xuc voi nhieu nguoi nhat: ");
    for(int i = 0; i<MAX; i++){
        if(list[i] == max)
        printf("%s\n", bangKiHieu[i].soHieu);
    }   
}

void checkTiepXuc(JRB root, char* people1, char* people2){
    int people1_int, people2_int;
    people1_int = convert(people1);
    people2_int = convert(people2);
    int tmp = getEdgeValue(root, people1_int, people2_int);
    if(tmp == 1){
        printf("Yes\n");
    }else{
        printf("No\n");
    }
}


void listF1vsF2(JRB root, char* people){
    JRB tmp1, tmp2, tmp3;
    int i = 0;
    int arrayF1[MAX] = {0};
    jrb_traverse(tmp1,root){
        if(strcmp(bangKiHieu[tmp1->key.i].soHieu, people) == 0){
        tmp2 = (JRB)tmp1->val.v;
        printf("Danh sach F1: ");
        jrb_traverse(tmp3, tmp2){
            printf("%s ", bangKiHieu[tmp3->key.i].soHieu);
            arrayF1[i] = tmp3->key.i;
            i++;
        }
        printf("\n");
        }
    }
    for(int j = 0; j<i; j++){
        case4(root, bangKiHieu[arrayF1[j]].soHieu, "Danh sach F2: ");
    }
}

int handlerString(int* stringConverted, char* string){
    char* token;
    int j = 0;
    token = strtok(string, "- ");
    while( token != NULL ) {
        if(convert(token)!=-1){
            stringConverted[j] = convert(token);
            j++;
        }else{
            printf("Vui long nhap lai\n");
        }
        token = strtok(NULL, "- ");
    }
    return j;
}

JRB addNew(JRB root, char* people, char* string){
    int j;
    int stringConverted[100] = {0};
    j = handlerString(stringConverted, string);
    for(int m = 0; m < j; m++){
        addEdge(root, convert(people), stringConverted[m], 1);
        addEdge(root, stringConverted[m], convert(people), 1);
    }
    return root;
}

void renderToFile(FILE* ptr, JRB root){
    fprintf(ptr, "%d\n", sodinh);
    JRB tmp1, tmp2, tmp3;

    jrb_traverse(tmp1,root){
        fprintf(ptr, "%s ", bangKiHieu[tmp1->key.i].soHieu);
        tmp2 = (JRB)tmp1->val.v;
        jrb_traverse(tmp3, tmp2){
            fprintf(ptr, "%s ", bangKiHieu[tmp3->key.i].soHieu);
        }
        fprintf(ptr, "\n");
    }
    printf("Cap nhat file thanh cong\n");
}

void main(){
    JRB root;
    int choice;
    int start, end; 
    int pass[100] = {0};
    char input[10];
    char people1[10];
    char people2[10];
    char people[10];
    char string[10];
    int f1[100];
    FILE* ptr;
    root = make_jrb();
    for(int i = 0; i<MAX; i++){
        strcpy(bangKiHieu[i].soHieu," ");
    }
    while (1)
    {
        printf("Enter: ");scanf("%d", &choice);__fpurge(stdin);
        switch (choice)
    {
        case 1:
            readFileAddBangKiHieu("tiepXucGan.txt");
            root = readFileAddTree(root, "tiepXucGan.txt");
            break;
    
        case 2: 
            renderTree(root);
            break;
        case 3: 
            printf("People 1: ");scanf("%s", people1);__fpurge(stdin);
            printf("People 2: ");scanf("%s", people2);__fpurge(stdin);
            checkTiepXuc(root, people1, people2);
            break;
        case 4:
            printf("People: ");scanf("%s", people);__fpurge(stdin);
            case4(root, people, "");
            break;
        case 5: 
            case5(root);
            break;
        case 6: 
            printf("Input F0: ");scanf("%s", people);__fpurge(stdin);
            listF1vsF2(root, people);
            break;
        case 7: 
            printf("Input new people: ");scanf("%s", people);__fpurge(stdin);
            if(convert(people) == -1){
                strcpy(bangKiHieu[sodinh].soHieu, people);
                sodinh++;
                printf("Tiep xuc gan voi: ");scanf("%s", string);__fpurge(stdin);
                root = addNew(root, people, string);
            }else{
                printf("Nguoi nay da co thong tin tu truoc\nVui long chon chuc nang 8\n");
            }
            renderTree(root);
            break;
        case 8: 
            printf("People 1: ");scanf("%s", people1);__fpurge(stdin);
            if(convert(people1) == -1) break;
            printf("People 2: ");scanf("%s", people2);__fpurge(stdin);
            if(convert(people2) == -1) break;
            addEdge(root, convert(people1), convert(people2), 1);
            break;
        case 9:
            ptr = fopen("tiepXucGan.txt", "w");
            renderToFile(ptr, root);
            fclose(ptr);
            break;
        case 10: 
            exit(0);
            break;
        default:
            printf("Khong co lua chon nay\n");
            break;
    }
    }
    
}
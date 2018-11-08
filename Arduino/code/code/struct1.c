#include<stdio.h>
typedef struct stu{
	char name[10];
	int sex;
}student;
int main(){
 student s1[2];
 int i;
 
 for(i=0;i<2;i++){
 	scanf("%s%d",&s1[i].name,&s1[i].sex);
 }

 for(i=0;i<2;i++){
 	printf("%s %d  \n",s1[i].name,s1[i].sex);
 }
 

}
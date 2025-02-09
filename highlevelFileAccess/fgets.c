#include <stdio.h>

int main(void){
	char buff[100];

	printf("Enter a character: ");

	fgets(buff, sizeof(buff), stdin);

	printf("\nYou entered: %s ",buff);

	return 0;
}

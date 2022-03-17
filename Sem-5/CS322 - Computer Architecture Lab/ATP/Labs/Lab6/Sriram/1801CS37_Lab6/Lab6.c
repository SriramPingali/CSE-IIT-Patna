#include <stdio.h>
#include <string.h>

int check_sub(char* name, char* subs)
{
	int j = 0;
	for (int i = 0; i < strlen(name); i++)
	{
		if(name[i] == subs[j])
		{
			j++;
			if(j == 3)
			{
				printf("Gryffindor\n");
				return(0);
			}
		}
	}
	printf("Slytherin\n");
	return(0);
}

int main()
{
	char name[10];
	char subs[3] = "ram";
	scanf("%s", name);
	check_sub(name, subs);
}
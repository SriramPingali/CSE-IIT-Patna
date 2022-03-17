/*Author - Nischal A; Roll Number - 1801CS33
B.Tech 3rd Year, Computer Science and Engineering
Indian Institute of Technology Patna (IIT-P)
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>
#define maxsize 100

struct code
{
	char pc[8];
	char ins[8];
	struct code *next;
};

struct data
{
	int pc;
	int value;
	int addr;
	struct data *next;
};

struct code *head_c = NULL;
struct data *head_d = NULL;

/*Used a simple array for memory implementation*/
int memory[maxsize];

void copyString(char *str1, char *str2)
{
	int i;
	for(i=0;i<8;i++)
	{
		str1[i] = str2[i];
	}
	str1[i] = '\0';
}
void dectohex(long decimalnum, char *res)
{
	int neg = 0;
	long neg_const = 16777216;
	if (decimalnum == 0)
	{
		res[0] = '0';
		res[1] = '\0';
		return;
	}
	else if(decimalnum < 0)
	{
		decimalnum = neg_const + decimalnum;
		neg = 1;
	}
    long quotient, remainder;
    int i, j;
    j = 0;
    char hexadecimalnum[100];
    quotient = decimalnum;
    while (quotient != 0)
    {
        remainder = quotient % 16;
        if (remainder < 10)
            hexadecimalnum[j++] = 48 + remainder;
        else
            hexadecimalnum[j++] = 55 + remainder;
        quotient = quotient / 16;

    }
    int k = 0;
    for (i = j-1; i >= 0; i--)
    {
        res[k] = hexadecimalnum[i];
        k++;
    }
    res[k] = '\0';
}

void addCode(char *pc_p, char *ins_p)
{
	if(head_c == NULL)
	{
		head_c = (struct code*)(malloc(sizeof(struct code)));
		copyString(head_c->pc, pc_p);
		copyString(head_c->ins, ins_p);
		head_c->next = NULL;
	}
	else
	{
		struct code* temp = (struct code*)(malloc(sizeof(struct code)));
		struct code* last = head_c;
		copyString(temp->pc, pc_p);
		copyString(temp->ins, ins_p);
		temp->next = NULL;
		/* Traverse till the end of LL from head*/
		while(last->next!= NULL)
		{
			last = last->next;
		}
		last->next = temp;
	}
	return;
}
void displayCodeLL()
{
	if (head_c == NULL)
	{
		return;
	}
	struct code* last = head_c;
	printf("\nData in the Code LL\n");
	while(1)
	{
		printf("%ld\t%ld\n", strlen(last->pc), strlen(last->ins));
		last = last->next;
		if(last == NULL)
			break;
	}
}
void addData(int pc_p, int data_s, int addr_s)
{
	if(head_d == NULL)
	{
		head_d = (struct data*)(malloc(sizeof(struct data)));
		head_d->pc = pc_p;
		head_d->value = data_s;
		head_d->addr = addr_s;
		head_d->next = NULL;
	}
	else
	{
		struct data* temp = (struct data*)(malloc(sizeof(struct data)));
		struct data* last = head_d;
		temp->pc = pc_p;
		temp->value = data_s;
		temp->addr = addr_s;
		temp->next = NULL;
		/* Traverse till the end of LL from head*/
		while(last->next!= NULL)
		{
			last = last->next;
		}
		last->next = temp;
	}
	return;
}
/*Special Processing for "data" instruction*/
int findAddress(int pc_s)
{
	if (head_d == NULL)
	{
		return 0;
	}
	struct data* last = head_d;
	int addrs;
	while(1)
	{
		if (last->pc == pc_s)
		{
			addrs = last->addr;
			break;
		}
		last = last->next;
		if(last == NULL)
			break;
	}
	return addrs;
}
void displayDataLL()
{
	if (head_d == NULL)
	{
		return;
	}
	struct data* last = head_d;
	printf("\nData in the Memory_Data LL\n");
	while(1)
	{
		printf("%d\t%d\t%d\n", last->pc, last->value, last->addr);
		last = last->next;
		if(last == NULL)
			break;
	}
}
/*Sign Aware Processing of the Operand*/
int processOperand(char *operand_s)
{
	char *end;
	long ret;
	ret = strtol(operand_s, &end, 16);
	if (operand_s[0] == 'F')
	{
		ret = ret - 16777216;
	}
	return ret;	
}
/*Function for showing the memory*/
void showmemory(int sp)
{
	int i;
	printf("Printing Memory\n");
	printf("Address\tValue\n");
	for(i = maxsize-1; i>=sp;i--)
		printf("%d\t%d\n", i, memory[i]);
}
/*Main Function for EMULATION of the OBJECT FILE*/
void emulate(char **op, char **oc, int ctr)
{
	int pc;
	int a,b, flag; /* Two registers A and B*/
	/* TODO Implement Stack and Memory*/
	int sp = maxsize-1;
	char res_pc[32];
	int run_time = 0;
	/* first pass - assing all data locations from below*/
	for(pc = 0; pc<ctr;pc++)
	{
		if (strcmp(oc[pc], "13") == 0)
		{
			memory[sp] = processOperand(op[pc]);
			addData(pc, memory[sp], sp);
			sp--;
		}
	}
	displayDataLL();
	for(pc=0;pc<ctr;)
	{
		char pcfinal[8];
		int pc_pad, k;
		flag = 0;
		dectohex(pc, res_pc);
		pc_pad = 8 - strlen(res_pc);
		printf("PC = ");
		for(k=0;k<pc_pad;k++)
		{
			printf("0");
		}		
		printf("%s\t A = %d\t B = %d\t SP = %d\n", res_pc, a, b, sp);
		if (strcmp(oc[pc], "00") == 0)
		{
			/* convert hex to decimal*/
			int operand = processOperand(op[pc]);
			if (operand < ctr && operand >= 0)
			{
				/* checking if data*/
				if (strcmp(oc[operand], "13") == 0)
				{
					operand = findAddress(operand);
				}
			}
			b = a;
			a = operand;
		}
		else if(strcmp(oc[pc], "01") == 0)
		{
			int operand = processOperand(op[pc]);
			a = a + operand;
		}
		else if(strcmp(oc[pc], "02") == 0)
		{
			int offset = processOperand(op[pc]);
			b = a;
			a = memory[sp + offset+1];
		}
		else if(strcmp(oc[pc], "03") == 0)
		{
			int offset = processOperand(op[pc]);
			memory[sp + offset+1] = a;
			a = b;
		}
		else if(strcmp(oc[pc], "04") == 0)
		{
			int offset = processOperand(op[pc]);
			a = memory[a + offset+1];
		}
		else if(strcmp(oc[pc], "05") == 0)
		{
			int offset = processOperand(op[pc]);
			memory[a + offset+1] = b;
		}
		else if(strcmp(oc[pc], "06") == 0)
		{
			a = b + a;
		}
		else if(strcmp(oc[pc], "07") == 0)
		{
			a = b - a;
		}
		else if(strcmp(oc[pc], "08") == 0)
		{
			a = b << a;
		}
		else if(strcmp(oc[pc], "09") == 0)
		{
			a = b >> a;
		}
		else if(strcmp(oc[pc], "0A") == 0)
		{
			int operand = processOperand(op[pc]);
			sp = sp + operand;
		}
		else if(strcmp(oc[pc], "0B") == 0)
		{
			if (a>=maxsize)
			{
				pc++;
				flag = 1;
			}
			sp = a;
			a = b;
		}
		else if(strcmp(oc[pc], "0C") == 0)
		{
			b = a;
			a = sp;
		}
		else if(strcmp(oc[pc], "0D") == 0)
		{
			/* For CALL instruction*/
			int operand = processOperand(op[pc]);
			b = a;
			a = pc;
			pc = pc + operand+1;
			flag = 1;		
		}
		else if(strcmp(oc[pc], "0E") == 0)
		{
			/* For return instruction*/
			pc = a;
			a = b;
		}
		else if(strcmp(oc[pc], "0F") == 0)
		{
			int operand = processOperand(op[pc]);
			if (a == 0)
			{
				pc = pc + operand+1;
				flag = 1;	
			}		
		}
		else if(strcmp(oc[pc], "10") == 0)
		{
			int operand = processOperand(op[pc]);
			if (a < 0)
			{
				pc = pc + operand+1;
				flag = 1;	
			}		
		}
		else if(strcmp(oc[pc], "11") == 0)
		{
			int operand = processOperand(op[pc]);
			pc = pc + operand+1;
			flag = 1;			
		}
		else if(strcmp(oc[pc], "12") == 0)
		{
			/* Condition for HALT*/
			break;
		}
		/* Incrementing in normal case*/
		if (flag == 0)
		pc++;
		run_time++;
	}
	showmemory(sp);
	printf("Total number of lines executed are %d\n", (run_time+1));
}
/*Main Functions initiates Emulate Process*/
int main(int argc, char *argv[])
{
    FILE *fptr;
    char *file_name = argv[1];
    /*Reading the Binary File*/
	fptr = fopen(file_name,"rb");
	fseek(fptr, 0, SEEK_END); /* seek to end of file*/
	int size = ftell(fptr); /* get current file pointer*/
	fseek(fptr, 0, SEEK_SET); /* seek back to beginning of file*/
	printf("Size of the binary file is %d\n", size);
	/* Reading file contents*/
	char buffer[size];
	char pc[8];
	char ins[8];
	fread(buffer,sizeof(buffer),1,fptr);
	/* Printing in the format required*/
	int i, ctr, j, k;
	ctr = size/8;
	char *pcs[ctr];
	char *inss[ctr];
	char *oc[ctr];
	char *op[ctr];
	/*for(i=0;i<ctr;i++)
	{
		pcs[i] = (char*)(malloc(8*sizeof(char)));
		char pc[8];
		for(j = 0; j < 8; j++)
		{
			pc[j] = (char)buffer[(i*16)+j];
		}
		pc[j] = '\0';
		strcpy(pcs[i], pc);
	}*/
	for(i=0;i<ctr;i++)
	{
		inss[i] = (char*)(malloc(8*sizeof(char)));
		char ins[8];
		for(j = 0; j < 8; j++)
		{
			ins[j] = (char)buffer[(i*8)+j];
		}
		strcpy(inss[i], ins);	
	}
	/* Printing the pc and ins*/
	for(i=0;i<ctr;i++)
	{
		oc[i] = (char*)(malloc(2*sizeof(char)));
		op[i] = (char*)(malloc(6*sizeof(char)));
		for(k=0;k<6;k++)
			op[i][k] = inss[i][k];
		for(k=0;k<2;k++)
			oc[i][k] = inss[i][k+6];
		printf("%s\t%s\n", op[i], oc[i]);
	}
	/*Call to main emulator*/
	emulate(op, oc, ctr);
	return 0;
}
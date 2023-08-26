/* example inputs :
    halt
    3.14 set pi
    2 set ratio
    r 2 pow pi * unhalt
   ( stop the main function from calling pop() once a new line character is reached, creat variable pi with the value 3.14,
   creat variable ratio with the value 2, pow(r, 2) * pi, let the main function call pop() once a new line character is
   reached )

   3.14 set pi pi 4 / duplicate sin cose / (calculates tan(p/4)) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXOP 100
#define NUMBER '0'
#define COMMAND 'c'

void push(double);
double pop(void);
int getop(char []); /* get next operand */
int isoperat(char); /* check whether if the input is an operator or not */
void getco(char []); /* get the command */
void execute(char []); /* execute the command */
void savevar(char name[], double value); /* store variable, the name in the string varnames and the value in varvalues */
double readvar(char name[]); /* return the value of the variable called string name */

int point = 0; /* refer to halt() and unhalt() functions */

int main()
{
	int type;
	char s[MAXOP];
	double op2;
	extern int sp;

	while ((type = getop(s)) != EOF)
	{
		switch (type)
		{
			case NUMBER:
				push(atof(s));
				break;
			case COMMAND:
				getco(s);
				execute(s);
				break;
			case '+':
				push(pop() + pop());
				break;
			case '-':
				op2 = pop();
				push(pop() - op2);
				break;
			case '*':
				push(pop() * pop());
				break;
			case '/': case '%':
				if ((op2 = pop()) != 0.0)
				{
					if (type == '/')
						push(pop() / op2);
					else
						push((int) pop() % (int) op2);
				}

				else
					printf("error: zero divisior\n");
				break;
			case '\n':
				if (!point) /*halt sets point to one and unhalt sets it to 0 */
                    printf("\t%.8g\n", pop());
				break;
			default:
				printf("error: unkown operator %s\n", s);
				break;
		}
	}

	return 0;
}

#define MAXVAL 100

int sp = 0;
double val[MAXVAL];

void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack is full, can't push %g\n", f);
}

double pop()
{
	if (sp > 0)
		return val[--sp];
	else {
		printf("error: stack is empty\n");
		return 0.0;
	}
}

/* print the top value at the stack without popping */
void print()
{
	extern char varnames[];

	if (sp > 0)
	{
		printf("\t%.8g\n", val[sp-1]);
		savevar("lp", val[sp-1]);
	}
	else
		printf("error: stack is empty, can't print the top value\n");
}

/* duplicate the top value into the stack */
void duplicate()
{
	val[sp] = val[sp-1];
	sp++;
}

/* swap the top two values with each other */
void swap()
{
	double temp;

	temp = val[sp-1];
	val[sp-1] = val[sp-2];
	val[sp-2] = temp;
}

/* clear the stack */
void clear()
{
	int i;

	for (i = 0; i < sp; i++)
		val[i] = 0.0;
	sp = 0;
}

#define MAXBUF 100

int bufp = 0;
char buf[MAXBUF];

char getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
	if (bufp < MAXBUF)
		buf[bufp++] = c;
	else
		printf("error: too many characters, can't unread %c\n", c);
}

#include <ctype.h>

int getop(char s[])
{
	int i, c, t;

	while ((s[0] = c = getch()) == ' '  || c == '\t')
		;
	s[1] = '\0';

	if (c == '-' || c == '+') /* check if the first non character is a plusr or minus sign and if so save the next character in t */
	{
		t = getch();
		ungetch(t);
	}

	if (!isdigit(c) && c != '.' && !((c == '-' || c == '+') && isdigit(t))) /* if it isn't a part of a digit execute the body */
	{
		if (isoperat(c) || c == EOF) /* if it's an opperator return it to the caller function and if not, it's a command */
			return c;
		else
		{
			ungetch(c);
			return COMMAND;
		}
	}


	if (c == '-' || c == '+')
		c = '0';

	i = 0;

	if (isdigit(c))
		while (isdigit(s[++i] = c = getch()))
			;
	if (c == '.')
		while (isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';
	if (c != EOF)
		ungetch(c);
	return NUMBER;
}

int isoperat(char c)
{
	switch (c)
	{
		case '+': case '-': case '*': case '/': case '%': case '\n':
			return 1;
		default:
			return 0;
	}
}

void getco(char s[])
{
	int c, i;

	for (i = 0; (c = getch()) != ' ' && c != '\t' && c != '\n' && c != EOF; i++)
		s[i] = c;
	s[i] = '\0';
	ungetch(c);
}

void reverse(char s[])
{
	int i, j, c;

	for (i = 0, j = strlen(s)-1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

void ungetco(char s[])
{
	int c, i;

	reverse(s);

	for (i = 0; s[i] != '\0'; i++)
		ungetch(s[i]);
}

int reco(char s[], char t[]);

void halt(void)
{
	point = 1;
}

#include <math.h>

void sinc()
{
	push(sin(pop()));
}

/* math functions */
void cosc()
{
	push(cos(pop()));
}

void atan2c()
{
	double x = pop();

	if (x != 0.0)
		push(atan2(pop(), x));
	else
		printf("error: zero divisor\n");
}

void expc()
{
	push(exp(pop()));
}

void logc()
{
	double x;

	if ((x = pop()) > 0)
		push(log(x));
	else
		printf("error: can't calculate log of %.8g\n", x);
}

void log10c()
{
	double x;

	if ((x = pop()) > 0)
		push(log10(x));
	else
		printf("error: can't calculate log10 of %.8g\n", x);
}

void powc()
{
	double y = pop();

	push(pow(pop(), y));
}

void sqrtc()
{
	double x;

	if ((x = pop()) >= 0)
		push(sqrt(x));
	else
		printf("error: can't calculate square root of negative number %.8g\n", x);
}

void fabsc()
{
	push(fabs(pop()));
}

void unhalt(void)
{
	point = 0;
}

#include <string.h>

void setvar(double value);

char command_list[] = "print\tduplicate\tswap\tclear\thalt\tunhalt\tsin\tcos\tatan2\texp\tlog\tlog10\tpow\tsqrt\tfabs\tset\t"; /* list of commands available to the user, each commend must end in a tab character for the reco() function to work */

void execute(char s[])
{
	extern char varnames[];

	switch (reco(command_list, s))
	{
		case 0:
			print();
			break;
		case 6:
			duplicate();
			break;
		case 16:
			swap();
			break;
		case 21:
			clear();
			break;
		case 27:
			halt();
			break;
		case 32:
			unhalt();
			break;
		case 39:
			sinc();
			break;
		case 43:
			cosc();
			break;
		case 47:
			atan2c();
			break;
		case 53:
			expc();
			break;
		case 57:
			logc();
			break;
		case 61:
			log10c();
			break;
		case 67:
			powc();
			break;
		case 71:
			sqrtc();
			break;
		case 76:
			fabsc();
			break;
		case 81:
			setvar(pop());
			break;
		case -1:
			if (reco(varnames, s) != -1)
				push(readvar(s));
			else
				printf("error: unknown command or variable %s\n", s);
			break;
	}
}

void setvar(double value)
{
	char name[40], temp[40];

	int type = getop(name);

	getco(name);

	int i = 0;

	while ((temp[i] = name[i]) != '\0')
		i++;
	ungetco(temp);

	if (type == COMMAND && (reco(command_list, name)) == -1)
	{
		getco(name);
		savevar(name, value);
	}

	else
		printf("error: your variable name can't start with a digit, operator or be the same as a command\n");
}

int reco(char s[], char t[]) /* return the index of the command or variable t in string s, if it doesn't exist return -1 */
{
	int i, j, k;

	for (i = 0; s[i] != '\0'; i++)
	{
		for (j = i, k = 0; t[k] != '\0' && t[k] == s[j]; j++, k++)
			;
		if (k > 0 && t[k] == '\0' && s[j] == '\t' && (i == 0 || s[i-1] == '\t'))
			return i;
	}

	return -1;
}

#define MAXVARLEN 1000 /* maximum character length of variables combiend */
#define MAXVARIA 500 /* maximum number of variables */

char varnames[MAXVARLEN] = "lp\t";
double varvalues[MAXVARIA] = {0.0};

void savevar(char name[], double value)
{
	int t, i, k, r;

	if ((r = reco(varnames, name)) == -1)
	{
		for (i = t = 0; varnames[i] != '\0'; i++)
		{
			if (varnames[i] == '\t') /* the number of tabs found in the current array is the index of the variable being saved in varvalues */
				t++;
		}
		if (i + strlen(name) - 2 >= MAXVARLEN) /* 1 character reserved for '\0' and one for '\t' at the end of the last variable */
			printf("error: too many variables\n");
		else
		{
			for (k = 0; name[k] != '\0'; i++, k++)
				varnames[i] = name[k];
			varnames[i++] = '\t';
			varnames[i] = '\0';
			varvalues[t] = value;
		}
	}

	else
	{
		for (t = i = 0; i < r; i++)
			if (varnames[i] == '\t')
				t++;
		varvalues[t] = value;
	}

}

double readvar(char name[])
{
	int t, i, j;

	for (t = j = 0, i = reco(varnames, name); j < i; j++)
		if (varnames[j] == '\t')
			t++;
	return varvalues[t];
}

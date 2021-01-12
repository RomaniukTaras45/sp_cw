#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "malloc.h"
#include <string>
#include "stdlib.h"
#include "ctype.h"

#define MAX_LEXEMS 1000
#define MAX_IDENT 100
#define MAX_BUF_SIZE 100
#define STACK_SIZE 200
#define MAX_LENGTH_TYPES 20

enum TypeOfLexem
{
	LBegProg,		// #
	LProgram,		// Program
	LProgramName,	// ProgramName
	LStart,			// Start
	LData,			// Variable
	LVarType,		// Longint
	LIdentifier,	// NameVar
	LEnd,			// Stop

	LScan,			// Input
	LPrint,			// Output

	LWhile,			// While
	LStartWhile,	// {
	LEndWhile,		// }

	LNewValue,		// <<
	LAdd,			// Add
	LSub,			// Sub
	LMul,			// Mul
	LDiv,			// Div
	LMod,			// Mod
	LEqu,			// ==
	LNotEqu,		// !=
	LGreate,		// Lt
	LLess,			// Gt
	LNot,			// !
	LAnd,			// &
	LOr,			// |

	LLBraket,		// (
	LRBraket,		// )
	LNumber,		// number
	LEndGroup,		// ;
	LComma,			// ,
	LEOF,			// EndFile
	LUnknown
};

//DATA
typedef struct Lexem
{
	char name[50];
	int value;
	int line;
	TypeOfLexem type;
}Lexema;

typedef struct ID
{
	char name[50];
	int value;
}Identificator;

typedef struct L
{
	Lexema LexTable[MAX_LEXEMS];			//Таблиця лексем
	int LexNum;								//

	Identificator IdTable[MAX_IDENT];		//Таблиця ідентифікаторів
	int IdNum;								//

	bool IsPresentInput,					//Ознаки присутності операторів
		IsPresentOutput,
		IsPresentMod,
		IsPresentAnd,
		IsPresentOr,
		IsPresentNot,
		IsPresentEqu,
		IsPresentGreate,
		IsPresentLess,
		IsPresentDiv;

	int numberErrors;

	std::string InputFileName;
	char OutputFileName[50];

	int bufExprPostfixForm[MAX_BUF_SIZE];	//Буфер для запису виразу в постфіксній формі
}DataType;

typedef struct Stacks
{
	int st[STACK_SIZE];
	int top;
}StackType;

typedef class stack
{
public:
	StackType S;
	void Init(StackType* s)
	{
		s->top = -1;
	}

	void Push(int i, StackType* s)
	{
		if (IsFull(s))
		{
			puts("Stack error (is full)");
			exit(0);
		}
		else
		{
			++s->top;
			s->st[s->top] = i;
		}
	}
	int Pop(StackType* s)
	{
		int i;
		if (IsEmpty(s))
		{
			puts("Stack error (is empty)");
			exit(0);
		}
		else
		{
			i = s->st[s->top];
			--s->top;
		}
		return i;
	}
	bool IsEmpty(StackType* s)
	{
		if (s->top == -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool IsFull(StackType* s)
	{
		if (s->top == 199)return true;
		else return false;
	}
	void prints(StackType s)
	{
		int i = 0;
		for (; i < 10; ++i)
		{
			printf("%d_", s.st[i]);
		}
	}
}StackT;

StackT Stack;						//Стек
StackT startStack;
StackT forStack;

DataType Data;						//Структура в якій зберігаються всі дані програми

FILE* InF, * OutF, * ErrorF;
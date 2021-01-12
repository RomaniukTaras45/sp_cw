void BeginASMFile(FILE* fout)
{
	fprintf(fout, ".386\n.model flat, stdcall\noption casemap :none\n");
	fprintf(fout, "include masm32\\include\\windows.inc\ninclude masm32\\include\\kernel32.inc\ninclude masm32\\include\\masm32.inc\ninclude masm32\\include\\user32.inc\ninclude masm32\\include\\msvcrt.inc\n");
	fprintf(fout, "includelib masm32\\lib\\kernel32.lib\nincludelib masm32\\lib\\masm32.lib\nincludelib masm32\\lib\\user32.lib\nincludelib masm32\\lib\\msvcrt.lib\n");
	fprintf(fout, "\n.DATA");
}
void BeginCodeSegm(FILE* fout)
{
	fprintf(fout, "\n.CODE\n");
	fprintf(fout, "start:\n");
	fprintf(fout, "invoke AllocConsole\n");
	fprintf(fout, "invoke GetStdHandle, STD_INPUT_HANDLE\n");
	fprintf(fout, "mov hConsoleInput, EAX\n");
	fprintf(fout, "invoke GetStdHandle, STD_OUTPUT_HANDLE\n");
	fprintf(fout, "mov hConsoleOutput, EAX\n");
	fprintf(fout, "finit\n");
}

void CheckPresentInOut()			//Визначення присутності операторів Input i Output
{
	for (int i = 0; Data.LexTable[i].type != LEOF; ++i)
	{
		if (Data.LexTable[i].type == LScan) Data.IsPresentInput = true;
		if (Data.LexTable[i].type == LPrint) Data.IsPresentOutput = true;
		if (Data.LexTable[i].type == LMod) Data.IsPresentMod = true;
		if (Data.LexTable[i].type == LAnd) Data.IsPresentAnd = true;
		if (Data.LexTable[i].type == LOr) Data.IsPresentOr = true;
		if (Data.LexTable[i].type == LNot || Data.LexTable[i].type == LNotEqu) Data.IsPresentNot = true;
		if (Data.LexTable[i].type == LEqu || Data.LexTable[i].type == LNotEqu) Data.IsPresentEqu = true;
		if (Data.LexTable[i].type == LGreate) Data.IsPresentGreate = true;
		if (Data.LexTable[i].type == LLess) Data.IsPresentLess = true;
		if (Data.LexTable[i].type == LDiv) Data.IsPresentDiv = true;
		if (Data.IsPresentInput &&
			Data.IsPresentOutput &&
			Data.IsPresentAnd &&
			Data.IsPresentOr &&
			Data.IsPresentNot &&
			Data.IsPresentEqu &&
			Data.IsPresentGreate &&
			Data.IsPresentLess &&
			Data.IsPresentDiv) break;
	}
}

void PrintData(FILE* f)				//Друк сегмету даних
{
	fputs("\n;===User Data==============\n", f);
	int i;
	for (i = 0; i < Data.IdNum; ++i)
	{
		fprintf(f, "\t%s\tdd\t0%xh\n", Data.IdTable[i].name, Data.IdTable[i].value);
	}
	fputs("\n;===Addition Data===========================================\n", f);
	fputs("\thConsoleInput\tdd\t?\n", f);
	fputs("\thConsoleOutput\tdd\t?\n", f);
	fputs("\tErrorMessage\tdb\t\"Error: division by zero\", 0\n", f);
	fputs("\tInputBuf\tdb\t15 dup (?)\n", f);
	fputs("\tInMessage\tdb\t\"Input: \", 0\n", f);
	fputs("\tOutMessage\tdb\t\"Output: \",\" %d\",0\n", f);
	fputs("\tResMessage\tdb\t20 dup (?)\n", f);
	fputs("\tNumberOfCharsRead\tdd\t?\n", f);
	fputs("\tNumberOfCharsWrite\tdd\t?\n", f);
	fputs("\tmsg1310\tdw\t13,10\n", f);
	fputs("\tbuf\tdd\t0\n", f);

	fputs("\tlb1\tdd\t?\n", f); //Змінні для обробки логічних операцій
	fputs("\tlb2\tdd\t?\n", f);
}

void PrintMOD(FILE* f)			//!!!new
{
	fputs("\n;===Procedure Mod=========================\n", f);
	fputs("Mod_ PROC\n\n", f);

	fputs("\tfistp lb1\n", f);
	fputs("\tfistp lb2\n", f);
	fputs("\tfild lb1\n", f);
	fputs("\tfild lb2\n", f);
	fputs("\tfprem\n", f);

	fputs("\tret\n", f);
	fputs("Mod_ ENDP\n", f);
	fputs(";=========================================\n\n", f);
}
void PrintAND(FILE* f)
{
	fputs("\n;===Procedure And=========================\n", f);
	fputs("And_ PROC\n", f);
	fputs("\tpush eax\n", f);
	fputs("\tpush edx\n", f);
	fputs("\tpushf\n", f);
	fputs("\tfistp lb1\n", f);
	fputs("\tfistp lb2\n", f);
	fputs("\tmov eax,lb1\n", f);
	fputs("\tcmp eax,0\n", f);
	fputs("\tjnz and_t1\n", f);
	fputs("\tjz and_false\n", f);
	fputs("and_t1:\n", f);
	fputs("\tmov eax,lb2\n", f);
	fputs("\tcmp eax,0\n", f);
	fputs("\tjnz and_true\n", f);
	fputs("and_false:\n", f);
	fputs("\tfldz\n", f);
	fputs("\tjmp and_fin\n", f);
	fputs("and_true:\n", f);
	fputs("\tfld1\n", f);
	fputs("and_fin:\n\n", f);
	fputs("\tpopf\n", f);
	fputs("\tpop edx\n", f);
	fputs("\tpop eax\n\n", f);
	fputs("\tret\n", f);
	fputs("And_ ENDP\n", f);
	fputs(";=========================================\n\n", f);
}
void PrintOR(FILE* f)
{
	fputs("\n;===Procedure Or==========================\n", f);
	fputs("Or_ PROC\n", f);
	fputs("\tpush eax\n", f);
	fputs("\tpush edx\n", f);
	fputs("\tpushf\n", f);
	fputs("\tfistp lb1\n", f);
	fputs("\tfistp lb2\n", f);
	fputs("\tmov eax,lb1\n", f);
	fputs("\tcmp eax,0\n", f);
	fputs("\tjnz or_true\n", f);
	fputs("\tmov eax,lb2\n", f);
	fputs("\tcmp eax,0\n", f);
	fputs("\tjnz or_true\n", f);
	fputs("\tfldz\n", f);
	fputs("\tjmp or_fin\n", f);
	fputs("or_true:\n", f);
	fputs("\tfld1\n", f);
	fputs("or_fin:\n\n", f);
	fputs("\tpopf\n", f);
	fputs("\tpop edx\n", f);
	fputs("\tpop eax\n\n", f);
	fputs("\tret\n", f);
	fputs("Or_ ENDP\n", f);
	fputs(";=========================================\n\n", f);
}
void PrintNOT(FILE* f)
{
	fputs("\n;===Procedure Not=========================\n", f);
	fputs("Not_ PROC\n", f);
	fputs("\tpush eax\n", f);
	fputs("\tpushf\n", f);
	fputs("\tfistp lb1\n", f);
	fputs("\tmov eax,lb1\n", f);
	fputs("\tcmp eax,0\n", f);
	fputs("\tjne not_false\n", f);
	fputs("\tfld1\n", f);
	fputs("\tjmp not_fin\n", f);
	fputs("not_false:\n", f);
	fputs("\tfldz\n", f);
	fputs("not_fin:\n\n", f);
	fputs("\tpopf\n", f);
	fputs("\tpop eax\n\n", f);
	fputs("\tret\n", f);
	fputs("Not_ ENDP\n", f);
	fputs(";=========================================\n\n", f);
}
void PrintEQ(FILE* f)
{
	fputs("\n;===Procedure Eq==========================\n", f);
	fputs("Eq_ PROC\n", f);
	fputs("\tpush eax\n", f);
	fputs("\tpush edx\n", f);
	fputs("\tpushf\n", f);
	fputs("\tfistp lb1\n", f);
	fputs("\tfistp lb2\n", f);
	fputs("\tmov eax,lb1\n", f);
	fputs("\tmov edx,lb2\n", f);
	fputs("\tcmp eax,edx\n", f);
	fputs("\tjne not_eq\n", f);
	fputs("\tfld1\n", f);
	fputs("\tjmp eq_fin\n", f);
	fputs("not_eq:\n", f);
	fputs("\tfldz\n", f);
	fputs("eq_fin:\n", f);
	fputs("\tpopf\n", f);
	fputs("\tpop edx\n", f);
	fputs("\tpop eax\n\n", f);
	fputs("\tret\n", f);
	fputs("Eq_ ENDP\n", f);
	fputs(";=========================================\n\n", f);
}
void PrintGT(FILE* f)
{
	fputs("\n;===Procedure Greate======================\n", f);
	fputs("Greate_ PROC\n", f);
	fputs("\tpush eax\n", f);
	fputs("\tpush edx\n", f);
	fputs("\tpushf\n", f);
	fputs("\tfistp lb1\n", f);
	fputs("\tfistp lb2\n", f);
	fputs("\tmov eax,lb1\n", f);
	fputs("\tmov edx,lb2\n", f);
	fputs("\tcmp edx,eax\n", f);
	fputs("\tjle lov\n", f);
	fputs("\tfld1\n", f);
	fputs("\tjmp gr_fin\n", f);
	fputs("lov:\n", f);
	fputs("\tfldz\n", f);
	fputs("gr_fin:\n", f);
	fputs("\tpopf\n", f);
	fputs("\tpop edx\n", f);
	fputs("\tpop eax\n\n", f);
	fputs("\tret\n", f);
	fputs("Greate_ ENDP\n", f);
	fputs(";=========================================\n\n", f);
}
void PrintLT(FILE* f)
{
	fputs("\n;===Procedure Less========================\n", f);
	fputs("Less_ PROC\n", f);
	fputs("\tpush eax\n", f);
	fputs("\tpush edx\n", f);
	fputs("\tpushf\n", f);
	fputs("\tfistp lb1\n", f);
	fputs("\tfistp lb2\n", f);
	fputs("\tmov eax, lb1\n", f);
	fputs("\tmov edx, lb2\n", f);
	fputs("\tcmp edx,eax\n", f);
	fputs("\tjge gr\n", f);
	fputs("lo:\n", f);
	fputs("\tfld1\n", f);
	fputs("\tjmp less_fin\n", f);
	fputs("gr:\n", f);
	fputs("\tfldz\n", f);
	fputs("less_fin:\n", f);
	fputs("\tpopf\n", f);
	fputs("\tpop edx\n", f);
	fputs("\tpop eax\n\n", f);
	fputs("\tret\n", f);
	fputs("Less_ ENDP\n", f);
	fputs(";=========================================\n\n", f);
}
void PrintCheck(FILE* f)
{
	fputs("\n;===Procedure Check=======================\n", f);
	fputs("Check_ PROC\n", f);
	fputs("\tpush eax\n", f);
	fputs("\tfldz\n", f);
	fputs("\tfcomp\n", f);
	fputs("\tfstsw ax\n", f);
	fputs("\tsahf\n", f);
	fputs("\tjne end_check\n", f);
	fputs("invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0\n", f);
	fputs("\tinvoke WriteConsoleA, hConsoleOutput, ADDR ErrorMessage, SIZEOF ErrorMessage, ADDR NumberOfCharsWrite, 0\n", f);
	fputs("\tjmp exit_label\n", f);
	fputs("end_check:\n", f);
	fputs("\tpop eax\n", f);
	fputs("\tret\n", f);
	fputs("Check_ ENDP\n", f);
	fputs(";=========================================\n\n", f);
}

void PrintInput(FILE* f)
{
	fputs("\n;===Procedure Input==========================================================================\n", f);
	fputs("Input PROC\n", f);
	fputs("invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0\n", f);
	fputs("invoke WriteConsoleA,hConsoleOutput,ADDR InMessage,SIZEOF InMessage,ADDR NumberOfCharsWrite,0\n", f);
	fputs("invoke ReadConsoleA,hConsoleInput,ADDR InputBuf,11,ADDR NumberOfCharsRead,0\n", f);
	fputs("invoke  crt_atoi, addr InputBuf\n", f);
	fputs("mov dword ptr buf, eax\n", f);
	fputs("ret\n", f);
	fputs("Input ENDP\n", f);
	fputs(";============================================================================================\n\n", f);


}
void PrintOutput(FILE* f)
{
	fputs("\n;===Procedure Output=========================================================================\n", f);
	fputs("Output PROC\n", f);
	fputs("invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0\n", f);
	fputs("invoke wsprintf, addr ResMessage, addr OutMessage, dword ptr buf\n", f);
	fputs("invoke WriteConsoleA,hConsoleOutput,ADDR ResMessage,eax,ADDR NumberOfCharsWrite,0\n", f);
	fputs("ret\n", f);
	fputs("Output ENDP\n", f);
	fputs(";============================================================================================\n\n", f);


}

void PrintEnding(FILE* f)
{
	fputs("exit_label:\n", f);
	fputs("invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0\n", f);
	fputs("invoke ReadConsoleA,hConsoleInput,ADDR InputBuf,1,ADDR NumberOfCharsRead,0\n", f);

	fputs("invoke ExitProcess, 0\n", f);
	if (Data.IsPresentInput) PrintInput(f);
	if (Data.IsPresentOutput) PrintOutput(f);
	if (Data.IsPresentMod) PrintMOD(f);
	if (Data.IsPresentAnd) PrintAND(f);
	if (Data.IsPresentOr) PrintOR(f);
	if (Data.IsPresentNot) PrintNOT(f);
	if (Data.IsPresentEqu) PrintEQ(f);
	if (Data.IsPresentGreate) PrintGT(f);
	if (Data.IsPresentLess) PrintLT(f);
	if (Data.IsPresentDiv) PrintCheck(f);
	fputs("end start\n", f);
}

bool Prioritet(TypeOfLexem t, StackType s)
{
	bool r;
	r = (
		((t == LDiv) && (Data.LexTable[s.st[s.top]].type == LAdd)) ||
		((t == LDiv) && (Data.LexTable[s.st[s.top]].type == LSub)) ||
		((t == LDiv) && (Data.LexTable[s.st[s.top]].type == LOr)) ||
		((t == LDiv) && (Data.LexTable[s.st[s.top]].type == LAnd)) ||
		((t == LDiv) && (Data.LexTable[s.st[s.top]].type == LEqu)) ||
		((t == LDiv) && (Data.LexTable[s.st[s.top]].type == LNotEqu)) ||
		((t == LDiv) && (Data.LexTable[s.st[s.top]].type == LLess)) ||
		((t == LDiv) && (Data.LexTable[s.st[s.top]].type == LGreate)) ||

		((t == LMul) && (Data.LexTable[s.st[s.top]].type == LAdd)) ||
		((t == LMul) && (Data.LexTable[s.st[s.top]].type == LSub)) ||
		((t == LMul) && (Data.LexTable[s.st[s.top]].type == LOr)) ||
		((t == LMul) && (Data.LexTable[s.st[s.top]].type == LAnd)) ||
		((t == LMul) && (Data.LexTable[s.st[s.top]].type == LEqu)) ||
		((t == LMul) && (Data.LexTable[s.st[s.top]].type == LNotEqu)) ||
		((t == LMul) && (Data.LexTable[s.st[s.top]].type == LLess)) ||
		((t == LMul) && (Data.LexTable[s.st[s.top]].type == LGreate)) ||

		((t == LMod) && (Data.LexTable[s.st[s.top]].type == LAdd)) ||
		((t == LMod) && (Data.LexTable[s.st[s.top]].type == LSub)) ||
		((t == LMod) && (Data.LexTable[s.st[s.top]].type == LOr)) ||
		((t == LMod) && (Data.LexTable[s.st[s.top]].type == LAnd)) ||
		((t == LMod) && (Data.LexTable[s.st[s.top]].type == LEqu)) ||
		((t == LMod) && (Data.LexTable[s.st[s.top]].type == LNotEqu)) ||
		((t == LMod) && (Data.LexTable[s.st[s.top]].type == LLess)) ||
		((t == LMod) && (Data.LexTable[s.st[s.top]].type == LGreate)) ||

		((t == LNot) && (Data.LexTable[s.st[s.top]].type == LAdd)) ||
		((t == LNot) && (Data.LexTable[s.st[s.top]].type == LSub)) ||
		((t == LNot) && (Data.LexTable[s.st[s.top]].type == LDiv)) ||
		((t == LNot) && (Data.LexTable[s.st[s.top]].type == LMul)) ||
		((t == LNot) && (Data.LexTable[s.st[s.top]].type == LMod)) ||
		((t == LNot) && (Data.LexTable[s.st[s.top]].type == LOr)) ||
		((t == LNot) && (Data.LexTable[s.st[s.top]].type == LAnd)) ||
		((t == LNot) && (Data.LexTable[s.st[s.top]].type == LEqu)) ||
		((t == LNot) && (Data.LexTable[s.st[s.top]].type == LNotEqu)) ||
		((t == LNot) && (Data.LexTable[s.st[s.top]].type == LLess)) ||
		((t == LNot) && (Data.LexTable[s.st[s.top]].type == LGreate))
		);
	return r;
}

int ConvertToPostfixForm(int i) //Формує в масиві послідовність номерів лексем яка відповідає постфіксній формі
{
	Stack.Init(&Stack.S);
	int n, z;
	n = 0;
	for (; ((Data.LexTable[i + n].type != LEndGroup) && (Data.LexTable[i + n].type != LStartWhile)); ++n);		//Встановлення коректності та довжини вхідного масиву
	int k;
	k = i + n;
	for (z = 0; i < k; ++i)
	{
		TypeOfLexem in;
		in = Data.LexTable[i].type;

		if ((in == LIdentifier) || (in == LNumber) /*|| (in == LNot)*/)		//!!!!!!
		{
			Data.bufExprPostfixForm[z] = i;
			++z;
		}
		else if (IsOperation(in))
		{
			if (Stack.IsEmpty(&Stack.S) || Prioritet(in, Stack.S))
			{
				Stack.Push(i, &Stack.S);
			}
			else
			{
				if (Data.LexTable[Stack.S.st[Stack.S.top]].type != LLBraket)
				{
					do
					{
						Data.bufExprPostfixForm[z] = Stack.Pop(&Stack.S);
						++z;
					} while ((!(Prioritet(in, Stack.S))) && (!(Stack.IsEmpty(&Stack.S))) && (Data.LexTable[Stack.S.st[Stack.S.top]].type != LLBraket));
				}
				Stack.Push(i, &Stack.S);
			}
		}
		if (in == LLBraket)
		{
			Stack.Push(i, &Stack.S);
			Data.bufExprPostfixForm[z] = i;
			++z;
		}
		if (in == LRBraket)
		{
			for (; (Data.LexTable[Stack.S.st[Stack.S.top]].type != LLBraket);)
			{
				Data.bufExprPostfixForm[z] = Stack.Pop(&Stack.S);
				++z;
			}
			Stack.Pop(&Stack.S);
			Data.bufExprPostfixForm[z] = i;
			++z;
		}
	}
	for (; !(Stack.IsEmpty(&Stack.S));)
	{
		Data.bufExprPostfixForm[z] = Stack.Pop(&Stack.S);
		++z;
	}
	Data.bufExprPostfixForm[z] = 3000;
	z++;
	return k;
}

void GenASMCode(const char* str, FILE* f)
{
	int n;
	for (n = 0; Data.bufExprPostfixForm[n] != 3000; ++n)
	{
		//puts("pf");
		Stack.Init(&Stack.S);

		if ((!IsOperation(Data.LexTable[Data.bufExprPostfixForm[n]].type)) && (Data.LexTable[Data.bufExprPostfixForm[n]].type != LNot) && (Data.LexTable[Data.bufExprPostfixForm[n - 1]].type != LNot))
		{
			if/*(*/ (Data.LexTable[Data.bufExprPostfixForm[n]].type == LIdentifier)/*&&(tokensTable[bufExprPostfixForm[n]].type!=ltNewValue))*/
			{
				fprintf(f, "\tfild %s\n", Data.LexTable[Data.bufExprPostfixForm[n]].name);
			}
			else if (Data.LexTable[Data.bufExprPostfixForm[n]].type == LNumber)
			{
				char buf[9];
				sprintf(&buf[0], "%d", abs(Data.LexTable[Data.bufExprPostfixForm[n]].value));
				buf[8] = '\0';
				fprintf(f, "\tmov dword ptr buf,%s\n", buf);
				fputs("\tfild buf\n", f);
				if (Data.LexTable[Data.bufExprPostfixForm[n]].value < 0)
				{
					fputs("\tFLDZ\n", f);
					fputs("\tFSUBR\n", f);
				}
			}
			else if ((Data.LexTable[Data.bufExprPostfixForm[n]].type == LLBraket) || (Data.LexTable[Data.bufExprPostfixForm[n]].type == LRBraket))
			{
				continue;
			}
		}
		else
		{
			switch (Data.LexTable[Data.bufExprPostfixForm[n]].type)
			{
			case LAdd: fputs("\tfadd\n", f);
				break;
			case LSub: fputs("\tfsub\n", f);
				break;
			case LDiv: fputs("\tcall Check_\n", f);
				fputs("\tfdiv\n", f);
				break;
			case LMod: fputs("\tcall Mod_\n", f);
				break;
			case LMul: fputs("\tfmul\n", f);
				break;
			case LAnd: fputs("\tcall And_\n", f);
				break;
			case LOr: fputs("\tcall Or_\n", f);
				break;
			case LNot: fputs("\tcall Not_\n", f);
				break;

			case LEqu: fputs("\tcall Eq_\n", f); break;
			case LNotEqu:
				fputs("\tcall Eq_\n", f);
				fputs("\tcall Not_\n", f);
				break;
			case LLess: fputs("\tcall Less_\n", f); break;
			case LGreate: fputs("\tcall Greate_\n", f); break;
			}
		}
	}
	fprintf(f, "\tfistp %s\n", str);
}

void PrintCode(FILE* f)
{
	int lab = 0;
	int ifLabelIndex = 0;
	Lexema l;								//Поточна лексема
	int i = 0;

	do									//Пошук початку коду
	{
		++i;
	} while (Data.LexTable[i].type != LVarType);
	++i;
	if (Data.LexTable[i].type == LVarType)
	{
		do
		{
			i++;
		} while (Data.LexTable[i].type != LEndGroup);
		i++;
	}

	for (;; ++i)
	{
		l.type = Data.LexTable[i].type;
		strncpy(l.name, Data.LexTable[i].name, 50);
		l.value = Data.LexTable[i].value;

		if (l.type == LEOF)
		{
			break;
		}

		if (l.type == LEndWhile)
		{
			int lab = startStack.Pop(&startStack.S);
			fprintf(f, "\tjmp whileStart%d\n", lab);
			fprintf(f, "whileEnd%d:\n", lab);
		}

		if (l.type == LWhile)
		{
			int bufi;
			bufi = i;
			i = ConvertToPostfixForm(i + 1);//Генерація постфіксного виразу			
			if (i < 0)
			{
				i = -i;
				puts("IE\n");
				continue;
			}
			//Генерація асемблерного коду з постфіксного виразу
			fprintf(f, "whileStart%d:\n", ++lab);
			startStack.Push(lab, &startStack.S);
			GenASMCode("buf", f);
			fputs("\tmov eax, dword ptr buf\n", f);
			fputs("\tcmp eax, 1\n", f);
			fprintf(f, "\tjne whileEnd%d\n", lab);
		}

		if (l.type == LPrint)
		{
			i = ConvertToPostfixForm(i + 1);
			GenASMCode("buf", f);
			fputs("\tcall Output\n", f);
		}
		if (l.type == LScan)
		{
			fputs("\tcall Input\n", f);
			fprintf(f, "\tfild buf\n");
			fprintf(f, "\tfistp %s\n", Data.LexTable[i + 2].name);
			i += 4;
		}

		if (l.type == LNewValue)
		{
			int bufi;
			bufi = i;
			i = ConvertToPostfixForm(i + 1);//Генерація постфіксного виразу			
			if (i < 0)
			{
				i = -i;
				puts("IE\n");
				continue;
			}
			//Генерація асемблерного коду з постфіксного виразу
			GenASMCode(Data.LexTable[bufi - 1].name, f);
		}
	}
}

void CodeGenerator(FILE* f)
{
	BeginASMFile(f);
	CheckPresentInOut();
	PrintData(f);
	BeginCodeSegm(f);

	PrintCode(f);

	PrintEnding(f);
}
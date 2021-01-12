#pragma once
bool IsOperation(TypeOfLexem t)
{
	bool r;
	r = ((t == LAdd) || (t == LSub) || (t == LMul) || (t == LDiv) || (t == LMod) || (t == LNot) ||
		(t == LAnd) || (t == LOr) || (t == LEqu) || (t == LNotEqu) || (t == LLess) || (t == LGreate));
	return r;
}
int IsExpression(int i, FILE* ef)
{
	int nom, error = 0;
	nom = i;
	if ((Data.LexTable[nom].type != LLBraket) && (Data.LexTable[nom].type != LIdentifier) && (Data.LexTable[nom].type != LNumber))
	{
		fprintf(ef, "line %d: \tExpression must begin from identifier, number or '('!\n", Data.LexTable[nom].line);
		error++;
	}
	for (; (Data.LexTable[nom].type != LEndGroup); nom++)
	{
		
		if (Data.LexTable[nom].type == LRBraket)///////////////////////
			if (!(IsOperation(Data.LexTable[nom + 1].type)) && (Data.LexTable[nom + 1].type != LEndGroup) && (Data.LexTable[nom + 1].type != LRBraket) && (Data.LexTable[nom + 1].type != LStartWhile))
			{
				fprintf(ef, "line %d: \tAfter ')' must be ')', operation or ';'!\n", Data.LexTable[nom].line);
				error++;
			}
		if (Data.LexTable[nom].type == LLBraket)
		{
			if ((Data.LexTable[nom + 1].type != LIdentifier) && (Data.LexTable[nom + 1].type != LLBraket) &&
				(Data.LexTable[nom + 1].type != LNumber) && (Data.LexTable[nom + 1].type != LNot))
			{
				fprintf(ef, "line %d: \tAfter '(' must be '(' or identifier!\n", Data.LexTable[nom].line);
				error++;
			}
		}
		if (IsOperation(Data.LexTable[nom].type) && Data.LexTable[nom].type != LNot)
		{
			if ((Data.LexTable[nom + 1].type != LIdentifier) && (Data.LexTable[nom + 1].type != LLBraket) && (Data.LexTable[nom + 1].type != LNumber) && (Data.LexTable[nom + 1].type != LNot))
			{
				fprintf(ef, "line %d: \tAfter operation must be '(' or idetifier!\n", Data.LexTable[nom].line);
				error++;
			}
		}
		
		if ((Data.LexTable[nom].type == LIdentifier) || (Data.LexTable[nom].type == LNumber))
		{/////////////////////////////////
			if (!(IsOperation(Data.LexTable[nom + 1].type)) && (Data.LexTable[nom + 1].type != LRBraket) && (Data.LexTable[nom + 1].type != LEndGroup))
			{
				fprintf(ef, "line %d: \tAfter identifier must be ')' or ';' or operation!\n", Data.LexTable[nom].line);
				error++;
			}
		}
	}
	return error;
}

int Balans(int nom, TypeOfLexem ends, TypeOfLexem ltStart, TypeOfLexem ltFinish)
{
	StackT ss;
	int j = 0, i;
	i = nom;
	ss.Init(&ss.S);
	for (; Data.LexTable[i].type != ends; i++)
	{
		if (Data.LexTable[i].type == ltStart)
		{
			ss.Push(i, &ss.S);
		}
		if (Data.LexTable[i].type == ltFinish)
		{
			if (ss.IsEmpty(&ss.S))
			{
				j = 1;	//  Too much ')';
				break;
			}
			else
			{
				ss.Pop(&ss.S);
			}
		}
	}
	if (!(ss.IsEmpty(&ss.S)))
	{
		j = 2;	// ')' expected;
	}
	return j;
}

int FindError()
{
	int label = 0;
	int i = 0, j = 1, temp = 0, ValNum = 0;
	FILE* ef;
	int while_num = 0, STARTBLOK_ENDBLOK_num = 0;//, r1, r2;
	int Err_num = 0;

	ef = fopen("errors.txt", "w");
	fputs("List of bugs in the program\n", ef);
	fprintf(ef, "=================================================================================================\n");
	//Data.IdNum = 1;


	//перевірка чи першим словом у програмі є program
	if (Data.LexTable[0].type != LBegProg)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'#' expected! (program must begin from the '#')\n", Data.LexTable[0].line);
	}
	if (Data.LexTable[1].type != LProgram)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'Program' expected!\n", Data.LexTable[0].line);
	}
	if (Data.LexTable[2].type != LProgramName)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'ProgramName' expected!\n", Data.LexTable[0].line);
	}
	if (Data.LexTable[3].type != LEndGroup)
	{
		Err_num++;
		fprintf(ef, "line %d: \t';' expected!\n", Data.LexTable[0].line);
	}

	if (Data.LexTable[Data.LexNum - 2].type != LEnd)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'Stop' expected!\n", Data.LexTable[1].line);
	}
	if (Data.LexTable[Data.LexNum - 1].type != LEOF)
	{
		Err_num++;
		fprintf(ef, "line %d: \tEnd of file expected!\n", Data.LexTable[0].line);
	}
	if (Data.LexTable[4].type != LData)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'Variable' expected!\n", Data.LexTable[i].line);
	}

	//перевірка, чи  другим словом в програмі є var
	if (Data.LexTable[5].type != LVarType)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'Longint' expected!\n", Data.LexTable[3].line);
	}
	if (Data.LexTable[5].type == LVarType)
	{
		i = 6;
		if (Data.LexTable[i].type != LIdentifier)  	//перевірка, чи після var йдуть ідентифікатори
		{
			Err_num++;
			fprintf(ef, "%d - line %d: \tIdentifier expected!\n", Err_num, Data.LexTable[i].line);
		}
		else
		{
			do
			{
				if ((Data.LexTable[i].type == LIdentifier) && (Data.LexTable[i + 1].type == LComma))
				{
					strcpy(Data.IdTable[Data.IdNum].name, Data.LexTable[i].name);
					for (int i = 0; i < Data.IdNum; i++)
					{
						if (strcmp(Data.IdTable[i].name, Data.IdTable[Data.IdNum].name) == 0)
						{
							Err_num++;
							fprintf(ef, "%d - line %d: \tThe identifier must be specified only once!\n", Err_num, Data.LexTable[Data.IdNum].line);
						}
					}

					Data.IdNum++;
					i = i + 2;
				}
			} while ((Data.LexTable[i].type == LIdentifier) && (Data.LexTable[i + 1].type == LComma));
			if ((Data.LexTable[i].type == LIdentifier) && (Data.LexTable[i + 1].type == LEndGroup))
			{
				strcpy(Data.IdTable[Data.IdNum].name, Data.LexTable[i].name);
				for (int i = 0; i < Data.IdNum; i++)
				{
					if (strcmp(Data.IdTable[i].name, Data.IdTable[Data.IdNum].name) == 0)
					{
						Err_num++;
						fprintf(ef, "%d - line %d: \tThe identifier must be specified only once!\n", Err_num, Data.LexTable[Data.IdNum].line);
					}
				}
				Data.IdNum++;
				i = i + 2;
				goto label1;
			}
			if (Data.LexTable[i].type != LEndGroup)
			{
				if (Data.LexTable[i].type == LComma)
				{
					Err_num++;
					fprintf(ef, "line %d: \tToo much commas!\n", Data.LexTable[i].line);
				}
				else
				{
					Err_num++;
					fprintf(ef, "line %d: \t';' expected!\n", Data.LexTable[i].line);
				}
			}
			else
			{
				Err_num++;
				fprintf(ef, "line %d: \tToo much commas or identifier expected!\n", Data.LexTable[i].line);
			}
		}
		i++;
	label1:;
	}
	if (Data.LexTable[i].type != LStart)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'Start' expected!\n", Data.LexTable[1].line);
	}

	for (j = 0; j < Data.LexNum; j++) {
		int s = 0, e = 0;
		if (Data.LexTable[j].type == LStart)
		{
			s++;
		}
		else if (Data.LexTable[j].type == LEnd)
		{
			e++;
		}
		if (s > 1)
		{
			Err_num++;
			fprintf(ef, "line %d: \tToo much 'Start'!\n", Data.LexTable[j].line);
		}
		if (e > 1)
		{
			Err_num++;
			fprintf(ef, "line %d: \tToo much 'Stop'!\n", Data.LexTable[j].line);
		}
	}



	for (j = 0;; j++)
	{
		//int BraketErr;
		if (Data.LexTable[j].type == LUnknown)        //Пошук невідомих слів(не ідентифікаторів)
		{
			Err_num++;
			fprintf(ef, "line %d: \tUnknown identifier!(%s)\n", Data.LexTable[j].line, Data.LexTable[j].name);
		}
		if ((Data.LexTable[j].type == LIdentifier) && (j > i))
		{
			bool available = 0;
			for (int i = 0; i < Data.IdNum; i++)
			{
				if (strcmp(Data.IdTable[i].name, Data.LexTable[j].name) == 0)
				{
					available = 1;
				}
			}
			if (!available)
			{
				Err_num++;
				fprintf(ef, "line %d: \tUnknown identifier!(%s)\n", Data.LexTable[j].line, Data.LexTable[j].name);
			}
		}
		if (Data.LexTable[j].type == LNewValue)
		{
			int buf;

			if (Data.LexTable[j - 1].type == LIdentifier)
			{
				buf = IsExpression((j + 1), ef);
			}
			else
			{
				buf = 1;
			}
			Err_num = Err_num + buf;
		}
		if (Data.LexTable[j].type == LPrint)
		{
			int buf, brak;
			if (Data.LexTable[j + 1].type != LLBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'(' expected!\n", Data.LexTable[j + 1].line);
			}
			buf = IsExpression((j + 1), ef);
			Err_num = Err_num + buf;
			brak = Balans(j, LEndGroup, LLBraket, LRBraket);
			if (brak == 1)
			{
				Err_num++;
				fprintf(ef, "line %d: \tToo much ')'!\n", Data.LexTable[j].line);
			}
			if (brak == 2)
			{
				Err_num++;
				fprintf(ef, "line %d: \t')' expected!\n", Data.LexTable[j].line);
			}
		}
		if (Data.LexTable[j].type == LScan)
		{
			if (Data.LexTable[j + 1].type != LLBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'(' expected!\n", Data.LexTable[j + 1].line);
			}
			if (Data.LexTable[j + 2].type != LIdentifier)
			{
				Err_num++;
				fprintf(ef, "line %d: \tIdentifier expected!\n", Data.LexTable[j + 2].line);
			}
			if (Data.LexTable[j + 3].type != LRBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t')' expected!\n", Data.LexTable[j + 3].line);
			}
			if (Data.LexTable[j + 4].type != LEndGroup)
			{
				Err_num++;
				fprintf(ef, "line %d: \t';' expected!\n", Data.LexTable[j + 4].line);
			}

		}
		if (Data.LexTable[j].type == LWhile)				//перевірка for
		{
			int buf;
			if (Data.LexTable[j + 1].type != LLBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'(' expected after 'While'!\n", Data.LexTable[j + 1].line);
			}
			buf = IsExpression((j + 1), ef);
			int brak = Balans(i, LEOF, LWhile, LStartWhile);
			if (brak == 1)
			{
				Err_num++;
				fprintf(ef, "line %d: \tToo much '{'!\n", Data.LexTable[j].line);
			}
			if (brak == 2)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'{' expected!\n", Data.LexTable[j].line);
			}

			brak = Balans(i, LEOF, LWhile, LEndWhile);
			if (brak == 1)
			{
				Err_num++;
				fprintf(ef, "line %d: \tToo much '}'!\n", Data.LexTable[j].line);
			}
			if (brak == 2)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'}' expected!\n", Data.LexTable[j].line);
			}
		}
		if (Data.LexTable[j].type == LEOF) break;
	}
	if (Err_num == 0) fprintf(ef, "No errors found.");
	return Err_num;
}
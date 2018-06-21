#include "Interpreter.h"
#include<iostream>
#include<stdlib.h>
#include<vector>
using namespace std;


int  splitString(const string & strSrc, const std::string& strDelims, vector<string>& strDest)
{
	typedef std::string::size_type ST;
	string delims = strDelims;
	std::string STR;
	if (delims.empty()) delims = "/n/r";


	ST pos = 0, LEN = strSrc.size();
	while (pos < LEN) {
		STR = "";
		while ((delims.find(strSrc[pos]) != std::string::npos) && (pos < LEN)) ++pos;
		if (pos == LEN) return strDest.size();
		while ((delims.find(strSrc[pos]) == std::string::npos) && (pos < LEN)) STR += strSrc[pos++];
		//std::cout << "[" << STR << "]";  
		if (!STR.empty()) strDest.push_back(STR);
	}
	return strDest.size();
}

void Interpreter::init()
{
	cout << "Welcome to MiniSQL!" << endl;
	cout << "MiniSQL->";
	fflush(stdin);
}

miniStatement miniInterpreter(char* in)
{
	miniStatement SQL;
	vector<string> split;
	int i = 0;
	CatalogManage cm;
	string d = " (),	\n";
	splitString(in, d, split);
	if (split[0] == "select")
	{
		int i = split[split.size() - 1].find(";");
		if (i != split[split.size() - 1].length() - 1)
		{
			SQL.flag = ERROR;
			return SQL;
		}
		else
		{
			split[split.size()-1] = split[split.size() - 1].erase(split[split.size() - 1].length() - 1, 1);
			if (split[1] == "*")
			{
				if (split[2] == "from")
				{
					if (split[4] == "where")
					{
						if (cm.isTable(split[3]))
						{
							SQL.flag = SELECT;
							SQL.MiniSelect.tableName = split[3];
							SQL.MiniSelect.conditionNum = 0;
							for (int j = 5; j < split.size(); j = j + 4)
							{
								SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].attributeName = split[j];
								SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].oprt = split[j + 1];
								if (strstr(split[j + 2].c_str(), "'"))
								{
									SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].type = CHAR;
									split[j + 2] = split[j + 2].erase(0, 1);
									split[j + 2] = split[j + 2].erase(split[j + 2].length() - 1, 1);
									SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].stringValues = split[j + 2];
									SQL.MiniSelect.conditionNum++;
								}
								else if (strstr(split[j + 2].c_str(), "."))
								{
									SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].type = FLOAT;
									SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].stringValues = split[j + 2];
									SQL.MiniSelect.conditionNum++;
								}
								else
								{
									SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].type = INT;
									SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].stringValues = split[j + 2];
									SQL.MiniSelect.conditionNum++;
								}
							}
							return SQL;
						}
						else
						{
							SQL.flag = ERROR;
							cout << "error" << endl;
							return SQL;
						}
					}
					else
					{
						if (split.size() == 4)
						{
							if (cm.isTable(split[3]))
							{
								SQL.flag = SELECT;
								SQL.MiniSelect.tableName = split[3];
								SQL.MiniSelect.conditionNum = 0;
								return SQL;
							}
							else
							{
								SQL.flag = ERROR;
								return SQL;
							}
						}
						else
						{
							SQL.flag = ERROR;
							return SQL;
						}
					}
				}
				else
				{
					SQL.flag = ERROR;
					return SQL;
				}
			}
			else
			{
				int posFrom = -1;
				// ��ѡ����ȫ�����Ե�ʱ�� ������*�����
				for (int j = 0; j < split.size(); j++)
				{
					if (split[j] == "from")
						posFrom = j;
				}
				if (posFrom == -1 || posFrom == 1)
				{
					SQL.flag = ERROR;
					return SQL;
				}
				if (cm.isTable(split[posFrom + 1]))
				{
					SQL.MiniSelect.tableName = split[posFrom + 1];
					SQL.MiniSelect.conditionNum = 0;
					for (int j = posFrom + 3; j < split.size(); j = j + 4)
					{
						SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].attributeName = split[j];
						SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].oprt = split[j + 1];
						if (strstr(split[j + 2].c_str(), "'"))
						{
							SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].type = CHAR;
							split[j + 2] = split[j + 2].erase(0, 1);
							split[j + 2] = split[j + 2].erase(split[j + 2].length() - 1, 1);
							SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].stringValues = split[j + 2];
							SQL.MiniSelect.conditionNum++;
						}
						else if (strstr(split[j + 2].c_str(), "."))
						{
							SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].type = FLOAT;
							SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].stringValues = split[j + 2];
							SQL.MiniSelect.conditionNum++;
						}
						else
						{
							SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].type = INT;
							SQL.MiniSelect.cond[SQL.MiniSelect.conditionNum].stringValues = split[j + 2];
							SQL.MiniSelect.conditionNum++;
						}
					}
				}
				else
				{
					SQL.flag = ERROR;
					return SQL;
				}
				SQL.MiniSelect.attributeNum = 0;
				for (int j = 1; j < posFrom; j++)
				{
					if (cm.isAttribute(split[posFrom + 1], split[j]))
					{
						SQL.flag = SELECT;
						SQL.MiniSelect.attr[SQL.MiniSelect.attributeNum].name = split[j];
						SQL.MiniSelect.attributeNum++;
						
					}
					else
					{
						SQL.flag = ERROR;
						return SQL;
					}
				}
				return SQL;
			}
		}	
	}

	else if (split[0] == "delete")
	{
		if (split[1] == "from")
		{
			if (split[3] == "where")
			{
				int i = split[split.size()-1].find(";");
				if (i != split[split.size() - 1].length()-1)
				{
					SQL.flag = ERROR;					
					return SQL;
				}
				else
				{
					split[split.size()-1] = split[split.size() - 1].erase(split[split.size() - 1].length() - 1, 1);
					if (cm.isTable(split[2]))
					{
						SQL.flag = DELETE;
						if (split.size() == 3)
						{
							SQL.MiniDelete.tableName = split[3];
							SQL.MiniDelete.conditionNum = 0;
							return SQL;
						}
						else
						{
							SQL.MiniDelete.tableName = split[3];
							SQL.MiniDelete.conditionNum = 0;
							for (int j = 4; j < split.size(); j = j + 4)
							{
								SQL.MiniDelete.cond[SQL.MiniDelete.conditionNum].attributeName = split[j];
								SQL.MiniDelete.cond[SQL.MiniDelete.conditionNum].oprt = split[j + 1];
								if (strstr(split[j + 2].c_str(), "'"))
								{
									SQL.MiniDelete.cond[SQL.MiniDelete.conditionNum].type = CHAR;
									split[j + 2] = split[j + 2].erase(0, 1);
									split[j + 2] = split[j + 2].erase(split[j + 2].length() - 1, 1);
									SQL.MiniDelete.cond[SQL.MiniDelete.conditionNum].stringValues = split[j + 2];
									SQL.MiniDelete.conditionNum++;
								}
								else if (strstr(split[j + 2].c_str(), "."))
								{
									SQL.MiniDelete.cond[SQL.MiniDelete.conditionNum].type = FLOAT;
									SQL.MiniDelete.cond[SQL.MiniDelete.conditionNum].stringValues = split[j + 2];
									SQL.MiniDelete.conditionNum++;
								}
								else
								{
									SQL.MiniDelete.cond[SQL.MiniDelete.conditionNum].type = INT;
									SQL.MiniDelete.cond[SQL.MiniDelete.conditionNum].stringValues = split[j + 2];
									SQL.MiniDelete.conditionNum++;
								}
							}
							return SQL;
						}
						
					}
					else
					{
						SQL.flag = ERROR;
						cout << "error" << endl;
						return SQL;
					}
				}
				
			}
			else
			{
					
			}
		}
		else
		{
			SQL.flag = ERROR;
			return SQL;
		}
	}

	else if (split[0] == "insert")
	{
		if (split[1] == "into")
		{
			if (split[3] == "values")
			{
				if (cm.isTable(split[2]))
				{
					if (split[split.size() - 1] != ";")
					{
						SQL.flag = ERROR;
						return SQL;
					}
					else
					{
						SQL.flag = INSERT;
						SQL.MiniInsert.tableName = split[2];
						SQL.MiniInsert.insertNum = 0;
						for (int j = 4; j < split.size(); j++)
						{
							if (strstr(split[j].c_str(), "'"))
							{
								SQL.MiniInsert.cond[SQL.MiniInsert.insertNum].type = CHAR;
								split[j] = split[j].erase(0, 1);
								split[j] = split[j].erase(split[j].length() - 1, 1);
								SQL.MiniInsert.cond[SQL.MiniInsert.insertNum].stringValues = split[j];
								SQL.MiniInsert.insertNum++;
							}
							else if (strstr(split[j].c_str(), "."))
							{
								SQL.MiniInsert.cond[SQL.MiniInsert.insertNum].type = FLOAT;
								SQL.MiniInsert.cond[SQL.MiniInsert.insertNum].stringValues = split[j];
								SQL.MiniInsert.insertNum++;
							}
							else
							{
								SQL.MiniInsert.cond[SQL.MiniInsert.insertNum].type = INT;
								SQL.MiniInsert.cond[SQL.MiniInsert.insertNum].stringValues = split[j];
								SQL.MiniInsert.insertNum++;
							}
						}
						return SQL;
					}				
				}
				else
				{
					SQL.flag = ERROR;
					return SQL;
				}
			}
			else
			{
				SQL.flag = ERROR;
				return SQL;
			}
		}
		else
		{
			SQL.flag = ERROR;
			return SQL;
		}
	}

	else if (split[0] == "execfile")
	{
		int i = split[1].find(";");
		if (i != split[1].length() - 1)
		{
			SQL.flag = ERROR;
			return SQL;
		}
		else
		{
			SQL.flag = EXEFILE;
			split[1] = split[1].erase(split[1].length() - 1, 1);
			SQL.MiniFile.fileName = split[1];
			return SQL;
		}
	}

	else if (split[0] == "create")
	{
		if (split[1] == "table")
		{
			if (split[split.size() - 1] != ";")	//�ж���û�зֺŽ�β
			{
				SQL.flag = ERROR;
				return SQL;
			}
			else
			{
				if (cm.isTable(split[2]))	  //�������Ƿ��Ѿ�����
				{
					SQL.flag = ERROR;
					return SQL;
				}
				else
				{
					SQL.flag = CREATETABLE;
					SQL.MiniCreateTable.tableName = split[2];
					SQL.MiniCreateTable.attributeNum = 0;
					vector<string> primary;		//���ڼ�¼������������������boolֵд��
					int j = 0;
					for (int k = 3; k < split.size()-1; k++)
					{					
						if (split[k] != "primary")
						{
							primary.insert(primary.end(), split[k]);
							j++;
							SQL.MiniCreateTable.attributes[SQL.MiniCreateTable.attributeNum].name = split[k];
							if (split[k + 1] == "char")
							{
								SQL.MiniCreateTable.attributes[SQL.MiniCreateTable.attributeNum].type = CHAR;
								SQL.MiniCreateTable.attributes[SQL.MiniCreateTable.attributeNum].length = atoi(split[k + 2].c_str());
								if (split[k + 3] == "unique")
								{
									SQL.MiniCreateTable.attributes[SQL.MiniCreateTable.attributeNum].unique = true;
									k = k + 3;
								}
								else
								{
									SQL.MiniCreateTable.attributes[SQL.MiniCreateTable.attributeNum].unique = false;
									k = k + 2;
								}
								SQL.MiniCreateTable.attributeNum++;
								
							}
							else if (split[k + 1] == "int")
							{
								SQL.MiniCreateTable.attributes[SQL.MiniCreateTable.attributeNum].type = INT;
								if (split[k + 2] == "unique")
								{
									SQL.MiniCreateTable.attributes[SQL.MiniCreateTable.attributeNum].unique = true;
									k = k + 2;
								}
								else
								{
									SQL.MiniCreateTable.attributes[SQL.MiniCreateTable.attributeNum].unique = false;
									k = k + 1;
								}
								SQL.MiniCreateTable.attributeNum++;
							}
							else if (split[k + 1] == "float")
							{
								SQL.MiniCreateTable.attributes[SQL.MiniCreateTable.attributeNum].type = FLOAT;
								if (split[k + 2] == "unique")
								{
									SQL.MiniCreateTable.attributes[SQL.MiniCreateTable.attributeNum].unique = true;
									k = k + 2;
								}
								else
								{
									SQL.MiniCreateTable.attributes[SQL.MiniCreateTable.attributeNum].unique = false;
									k = k + 1;
								}
								SQL.MiniCreateTable.attributeNum++;
							}
							else
							{
								SQL.flag = ERROR;
								return SQL;
							}
						}											
						else if (split[k] == "primary")
						{
							if (split[k+1]==("key"))
							{
								int m;
								for (m = 0; m < j; m++)
								{
									if (split[k + 2] == primary[m])
									{
										SQL.MiniCreateTable.attributes[m].primary = true;
										break;
									}									
								}
								if (m > j)
								{
									SQL.flag = ERROR;
									return SQL;
								}
								k = k + 3;
							}
							else
							{
								SQL.flag = ERROR;
								return SQL;
							}
						}
					}
					return SQL;
				}
			}			
		}
		else if (split[1] == "index")
		{
			if (split[6]!=";")
			{
				SQL.flag = ERROR;
				return SQL;
			}
			else
			{
				if (split[3] == "on")
				{
					if (cm.isTable(split[4]))
					{
						if (cm.isAttribute(split[4], split[5]))
						{
							SQL.flag = CREATEINDEX;
							SQL.MiniCreateIndex.indexName = split[2];
							SQL.MiniCreateIndex.tableName = split[4];
							SQL.MiniCreateIndex.attributeName = split[5];
							return SQL;
						}
						else
						{
							SQL.flag = ERROR;
							return SQL;
						}
					}
					else
					{
						SQL.flag = ERROR;
						return SQL;
					}

				}
				else
				{
					SQL.flag = ERROR;
					return SQL;
				}
			}
			
		}
		else if (split[1] == "database")
		{
			int i = split[2].find(";");
			if (i != split[2].length() - 1)
			{
				SQL.flag = ERROR;
				return SQL;
			}
			else
			{
				SQL.flag = CREATEDATABASE;
				split[2] = split[2].erase(split[2].length() - 1, 1);
				SQL.MiniCreateDatabase.databaseName = split[2];
				return SQL;
			}
			
		}
		else
		{
			SQL.flag = ERROR;
			return SQL;
		}
	}

	else if (split[0] == "drop")
	{
		if (split[1] == "table")
		{
			int i = split[2].find(";");
			if (i != split[2].length() - 1)
			{
				SQL.flag = ERROR;
				return SQL;
			}
			else
			{
				SQL.flag = DROPTABLE;
				split[2] = split[2].erase(split[2].length() - 1, 1);
				SQL.MiniDropTable.tableName = split[2];
				return SQL;
			}
			
		}
		else if (split[1] == "index")
		{
			int i = split[2].find(";");
			if (i != split[2].length() - 1)
			{
				SQL.flag = ERROR;
				return SQL;
			}
			else
			{
				split[2] = split[2].erase(split[2].length() - 1, 1);
				SQL.flag = DROPINDEX;
				SQL.MiniDropIndex.indexName = split[2];
				return SQL;
			}
			
		}
		else if (split[1] == "database")
		{
			int i = split[2].find(";");
			if (i != split[2].length() - 1)
			{
				SQL.flag = ERROR;
				return SQL;
			}
			else
			{
				SQL.flag = DROPDATABASE;
				split[2] = split[2].erase(split[2].length() - 1, 1);
				SQL.MiniDropDatabase.databaseName = split[2];
				return SQL;
			}
			
		}
		else
		{
			SQL.flag = ERROR;
			return SQL;
		}
	}

	else if (split[0] == "quit;")
	{
		SQL.flag = QUIT;
		return SQL;
	}

	else if (split[0] == "use")
	{
		int i = split[1].find(";");
		if (i != split[1].length() - 1)
		{
			SQL.flag = ERROR;
			return SQL;
		}
		else
		{
			SQL.flag = USEDATABASE;
			split[1] = split[1].erase(split[1].length() - 1, 1);
			SQL.MiniUseDatabase.databaseName = split[1];
			return SQL;
		}
		
	}

	else
	{
		SQL.flag = ERROR;
		return SQL;
	}
}

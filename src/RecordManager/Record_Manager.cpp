/*Record Manager*/
/*��¼�Ĵ洢���Զ���Ϊ����������к�Ϊ��¼�ļ����,�����/00Ϊ��β*/
class RecordManager
{
private:
	RecordManager();
	~RecordManager();
	//mode�У���Ҫ�б�Ĵ�����ɾ������¼�Ķ�ȡ��������ɾ��
	/*����API���ԣ���Ҫ��һ�ű����Ϣ��catelog)�Լ�����/ɾ��/ѡ�����Ϣ��interpreter��*/
	bool InsertRecord(miniInsert I);
	bool DeleteRecord(miniDelete I,table t);//ֱ�Ӹ�����������ɾ�� 
	bool DeleteRecordByBlock(Block* b,miniDelete I,table t);//��������block�Լ���������ɾ�� 
	bool DeleteRecordByPos(Block* b,int pos,miniDelete I,table t);//����block�Լ�ƫ����posɾ������ĳһ��record 
	records SelectRecord(miniSelect I,table t);//select�������з��������ļ�¼ 
	records SelectRecordByBlock(Block* b,miniSelect I,table t);//����block���з��������ļ�¼ 
	records SelectRecordByPos(Block* b,int pos,miniSelect I,table t);//����block�Լ�ƫ����pos���ؾ���һ����¼ 

public:
	int getInt(Block *block, int posBegin);
	void setInt(Block *block, int posBegin,int num);
	float getFloat(Block *block, int posBegin);
	void setFloat(Block *block, int posBegin,float num);
	std::string getString(Block *block, int posBegin,int num);
	void setString(Block *block, int posBegin, std::string str);
	Block* GetBlock(string name);
	Block* GetNextBlock(Block* b);
	int FindEnd(char* data);
	int located(string name,table t);
	char* findAttri(char* data,int order);
	bool cmpAttri(char* d,condition c);
	char* FindNextRecord(char* data,int num);
	Block* Writein(string name);//find the block that can be inserted
	Block* Writeback(Block* b);
	Block* Readout(string name);
}

class records
{
public:
	records(){};
	~records(){};
	records(table t);
	insert(Block* b,int pos); 
	int attriNum;//���Եĸ��� 
	vector<attribute> attributes;//������Ϣ 
	int recordNum;//��¼�ĸ���
	vector<miniRecord> list;//��¼������
}

class miniRecord
{
public:
	condition cond[32];//��¼���� 
	int conditionNum;//������Ŀ 
	int pos;//���ڿ��ƫ�� 
	int blockNum;//���ڿ�ı�� 
}

bool RecordManager::InsertRecord(miniInsert I)
{
	Block* b;
	b=Writein(I.tableName);
    int pos=FindEnd(b->data);
    b->data[pos++]='\n';
    int i;
    for(i=0;i<I.conditionNum;i++)
    {
    	switch(I.cond[i].type)
    	case 1://int 
    		setInt(&b,pos,I.cond[i].intValue);
    		pos+=sizeof(int);
    		break;
    	case 2://float
    		setFloat(&b,pos,I.cond[i].floatValue);
    		pos+=sizeof(float);
    		break;    		
    	case 3://string
			setString(&b,pos,I.cond[i].stringValues);
			pos+=length(I.cond[i].stringValues);
			break;
		b->data[pos++]=',';
    }
    b->[pos-1]=0;
    //��ǰĬ��ÿ�鶼��д��ȥ,����������ٸ� 
    Writeback(b);
    return 1;
}

bool RecordManager::DeleteRecord(miniDelete I,table t)
{
	Block* b;
	b=Readout(I.tableName);//��ȡ��ĵ�һ��block 
	char* data,*temp;
	int i;
	int num=t.attributeNum;
	int order[num];
	for(i=0;i<num;i++)
	{
		order[i]=located(I.cond[i].attributeName,t);
	}
	while(b)//��¼δ������ 
	{
		data=b->data;
		while(data)
		{
			for(i=0;i<num;i++)
			{
				temp=findAttri(data,order[i]);
				if(!cmpAttri(data,I.cond[i]))
				break;
			}
			if(i==num)//�������� 
			{
				/*���ո�*/
				FillBlack(data);
			}
			else
			data=FindNextRecord(data,t.attributeNum);
		}
		b=getNextBlock(b);
	}		
}

bool DeleteRecordByBlock(Block* b,miniDelete I,table t)
{
	char* data,*temp;
	int i;
	int num=t.attributeNum;
	int order[num];
	for(i=0;i<num;i++)
	{
		order[i]=located(I.cond[i].attributeName,t);
	}
	data=b->data;
	while(data)
	{
		for(i=0;i<num;i++)
		{
			temp=findAttri(data,order[i]);
			if(!cmpAttri(data,I.cond[i]))
			break;
		}
		if(i==num)//�������� 
		{
			/*���ո�*/
			FillBlack(data);
		}
		else
		data=FindNextRecord(data,t.attributeNum);
	}		
}

bool DeleteRecordByPos(Block* b,int pos,miniDelete I,table t)
{
	char* data,*temp;
	int i;
	int num=t.attributeNum;
	int order[num];
	for(i=0;i<num;i++)
	{
		order[i]=located(I.cond[i].attributeName,t);
	}
	data=b->data[pos];
	for(i=0;i<num;i++)
	{
		temp=findAttri(data,order[i]);
		if(!cmpAttri(data,I.cond[i]))
		break;
	}
	if(i==num)//�������� 
	{
		/*���ո�*/
		FillBlack(data);
	}
	else
	data=FindNextRecord(data,t.attributeNum);	
}

records SelectRecord(miniSelect I,table t)
{
	records record(t);
	Block* b;
	b=Readout(I.tableName);//��ȡ��ĵ�һ��block 
	char* data,*temp;
	int i;
	int num=t.attributeNum;
	int order[num];
	for(i=0;i<num;i++)
	{
		order[i]=located(I.cond[i].attributeName,t);
	}
	while(b)//��¼δ������ 
	{
		data=b->data;
		while(data)
		{
			for(i=0;i<num;i++)
			{
				temp=findAttri(data,order[i]);
				if(!cmpAttri(data,I.cond[i]))
				break;
			}
			if(i==num)//�������� 
			{
				record.insert(b,pos);
			}
			else
			data=FindNextRecord(data,t.attributeNum);
		}
		b=getNextBlock(b);
	}
	return record;
}
records SelectRecordByBlock(Block* b,miniSelect I,table t)
{
	records record(t);
	char* data,*temp;
	int i;
	int num=t.attributeNum;
	int order[num];
	for(i=0;i<num;i++)
	{
		order[i]=located(I.cond[i].attributeName,t);
	}
	data=b->data;
	while(data)
	{
		for(i=0;i<num;i++)
		{
			temp=findAttri(data,order[i]);
			if(!cmpAttri(data,I.cond[i]))
			break;
		}
		if(i==num)//�������� 
		{
			record.insert(b,pos);
		}
		else
		data=FindNextRecord(data,t.attributeNum);
	}
	return record;	
}

records SelectRecordByPos(Block* b,int pos,miniSelect I,table t)
{
	records record(t);
	char* data,*temp;
	int i;
	int num=t.attributeNum;
	int order[num];
	for(i=0;i<num;i++)
	{
		order[i]=located(I.cond[i].attributeName,t);
	}
	data=b->data;
	for(i=0;i<num;i++)
	{
		temp=findAttri(data,order[i]);
		if(!cmpAttri(data,I.cond[i]))
		break;
	}
	if(i==num)//�������� 
	{
		record.insert(b,pos);
	}
	return record;
}


Block* RecordManager::GetBlock(string name)
{
	BufferManager bm;
	Block b;
	b=bm.GetBlock(name,0);
	return b;
}

Block* RecordManager::GetNextBlock(Block* b)
{
	BufferManager bm;
	Block* next;
	if(!next=bm.GetBlock(b->FileName,b->offset+1))
	next=bm.GetNewBlock(b->FileName,b->offset+1);
	if(b->IsAccessed)
	{
		SetAccessed(next);
		b->IsAccessed=false;
	}
	if(b->IsWritten)
	{
		SetWritten(next);
		b->IsWritten=false;
	}
	if(b->IsLocked)
	{
		SetLock(next);
		UnLock(b);
	}
	return next;
}

int RecordManager::FindEnd(char* data)
{
	int i=0;
	while(data[i]!=0)i++;
	return i;
}

int RecordManager::located(string name,table t)
{
	int i;
	for(i=1;i<=t.attributeNum;i++)
	{
		if(t.attributes[i].name==name)
		break; 
	}
	return i;
}

char* RecordManager::findAttri(char* data,int order)
{
	int i=1;
	char* temp=data;
	while(i<order)
	{
		if(*temp==',')i++;
		temp++;
	}
	return temp;
}

bool RecordManager::cmpAttri(char* d,condition c)
{
	switch c.type:
		case 1:if(*(int*)d==c.intValue)return true;
			else return false;
		case 2:if(*(float*)d==c.floatValue)return true;
			else return false;
		case 3:if(*d==c.stringVlalues)return true;
			else return false;//�˴������� 
}

char* RecordManager::FindNextRecord(char* data,int num)
{
	int i=1;
	char* temp=data;
	while(i<=num)
	{
		if(*temp==',')i++;
		temp++;
	}
	return temp;	
}

Block* RecordManager::Writein(string name);//find the block that can be inserted
{
	BufferManager bm;
	CatelogManager cm;
	Block* b=GetBlock(name,0);
	char* temp;
	table t;
	t=cm.GetTable(name);
	int LenR=0,LenB,i;
	for(i=0;i<t.attributeNum;i++)
	{
		LenR+=t.attributes[i].Length;
	}
	while(1)
	{ 
		LenB=0;
		temp=b->data;
		while(temp[0]!=0)
		{
			temp++;
			LenB++;
		}
		if(LenB+LenR<=4096)break;
		else b=GetNextBlock(b); 
	}
	bm.SetWritten(b);
	bm.SetAccessed(b);
	bm.SetLock(b);
	return b;
}

Block* RecordManager::Writeback(Block* b)
{
	b->IsAccessed=false;
	b->IsWritten=false;
	b->IsLocked=false;
}

Block* RecordManager::Readout(string name)
{
	BufferManager bm;
	Block* b;
	b=bm.GetBlock(name,0);
	bm.SetAccessed(b);
	return b;
}

int RecordManager::getInt(Block *block, int posBegin)
{
	int *info = (int *)(block->data + posBegin);
	return *info;
}

void RecordManager::setInt(Block *block, int posBegin, int num)
{
	int *info = &num;
	for (int i = 0; i < sizeof(int); i++)
	{
		block->data[posBegin + i] = *((char*)info + i);
	}
}

float RecordManager::getFloat(Block *block, int posBegin)
{
	float *info = (float *)(block->data + posBegin);
	return *info;
}

void RecordManager::setFloat(Block *block, int posBegin, float num)
{
	float *info = &num;
	for (int i = 0; i < sizeof(float); i++)
	{
		block->data[posBegin + i] = *((char*)info + i);
	}
}

std::string RecordManager::getString(Block *block, int posBegin,int num)
{
	char* str = (char*)block->data + posBegin;
	std::string theString(str, num);
	return theString;
}

void RecordManager::setString(Block *block, int posBegin, std::string str)
{
	strcpy((char *)(block->data + posBegin), str.c_str());
}

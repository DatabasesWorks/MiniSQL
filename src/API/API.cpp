/*API*/
class API
{
public:
	API();
	~API();
	//mode�У���Ҫ�б�Ĵ�����ɾ������¼�Ķ�ȡ��������ɾ��
	bool Insert(miniInsert I);
	bool Create(miniCreate I);
	bool Drop(miniDrop I);
	bool Select(miniSelect I);
	bool Delete(miniDelete I);
	bool CreateIndex();//��ȱ 
	bool DropIndex();//��ȱ 
private:
	
}



bool API::Insert(miniInsert I)
{
	RecordManger rm;
	rm.Insert(mininInsert I);
	/*ȱ�ٶ�index��insert����Ҫ�ȼ��index�Ƿ���ڣ�Ȼ����insert*/
}

bool API::CreateTable(_Instruction I)
{
	BufferManager bm;
	bm.GetNewBlock(I.tableName,0);
	/*ȱ��catelog�Ĵ���*/
}

bool API::Drop(_Instruction I)
{
	BufferManager bm;
	bm.DeleteFileBlock(I.tableName);
	/*ȱ��catelog��ɾ��*/
}

char* API::Select(miniSelect I)
{
	CatelogManager cm; 
	RecordManager rm;
	int i;
	table t;
	t=cm.getTable(I.tableName);
	records r;
	int num=t.attributeNum;
	/*������Ҫ�ж��Ƿ����index
	.
	.
	.*/
	/*��û��index����ֱ��ͨ��rm����select*/
	r=rm.SelectRecord(I);
	/*��ӡ
	.
	.
	.*/
}
bool API::Delete(_Instruction I)
{
	CatelogManager cm;
	Block* b;
	b=Readout(I.tableName);//��ȡ��ĵ�һ��block 
	char* data,*temp;
	int i;
	table t;
	t=cm.getTable(I.tableName);
	int num=t.attributeNum;
	/*������Ҫ�ж��Ƿ����index
	.
	.
	.*/
	/*��û��index����ֱ��ͨ��rm����delect*/
	r=rm.DeleteRecord(I);
}

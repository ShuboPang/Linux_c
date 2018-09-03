#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//sqlite3���ݿ���Ҫ��ͷ�ļ�
#include <sqlite3.h>
#define MAX_COMMAND 512

int flag=0;

void actionSql(char *sql,sqlite3 *db)
{
	char *zErrMsg=NULL;
	int rc;
	//����api�����sql���
	rc=sqlite3_exec(db,sql,NULL,NULL,&zErrMsg);
	if(rc!=SQLITE_OK)
	{
		printf("SQL error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		printf("action successfully!\n");
	}
}

static int callback(void* NotUsed,int argc,
	char** argv,char** azColName)
{
	int i;
	if(0 == flag)
	{
		for(i=0;i<argc;i++)
		{
			printf("%s \t",azColName[i]);
		}
		printf("\n");
		flag = 1;
	}
	for(i=0;i<argc;i++)
	{
		printf("%s \t",argv[i]);
	}
	printf("\n");
	return 0;
}


int main(int argc,char** argv)
{
	//����һ��sqlite3�����ݿ�ʵ��ָ��
	sqlite3 *db;
	char *zErrMsg=NULL;
	//����ֵ���������ڽ��պ�������ֵ���жϺ����Ƿ�ִ�гɹ�
	int rc;
	char sql[MAX_COMMAND];

	//��/�������ݿ�
	rc=sqlite3_open("test.db",&db);
	if(rc)
	{
		printf("Can't open database:%s\n",sqlite3_errmsg(db));
		return 1;
	}
	printf("open database successfully\n");
	//��䴴�����sql��䵽�ַ�����
	memset(sql,0,MAX_COMMAND);
	strcpy(sql,"create table company("\
	"ID int primary key not null,"\
	"name text not null,"\
	"age int not null,"\
	"address char(50),"\
	"salary real);");
	actionSql(sql,db);

	//�������¼��sql��䵽�ַ�����
	memset(sql,0,MAX_COMMAND);
	strcpy(sql,
	"insert into COMPANY values(1,'Lucy',26,'Jilin',5000.00);");
	actionSql(sql,db);
	memset(sql,0,MAX_COMMAND);
	strcpy(sql,
	"insert into COMPANY values(2,'Jim',28,'Changchun',4100.00);");
	actionSql(sql,db);
	memset(sql,0,MAX_COMMAND);
	strcpy(sql,
	"insert into COMPANY values(3,'Kim',25,'Baicheng',2300.00);");
	actionSql(sql,db);
	memset(sql,0,MAX_COMMAND);
	strcpy(sql,
	"insert into COMPANY values(4,'Allen',26,'Shenyang',5500.00);");
	actionSql(sql,db);


	//�鿴����select
	memset(sql,0,MAX_COMMAND);
	strcpy(sql,"select * from company;");

	rc=sqlite3_exec(db,sql,callback,NULL,&zErrMsg);
	if(rc!=SQLITE_OK)
	{
		printf("SQL error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		printf("action successfully!\n");
	}
	flag=0;


	//�ر����ݿ�
	sqlite3_close(db);
	return 0;
}

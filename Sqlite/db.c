#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//sqlite3数据库需要的头文件
#include <sqlite3.h>
#define MAX_COMMAND 512

int flag=0;

void actionSql(char *sql,sqlite3 *db)
{
	char *zErrMsg=NULL;
	int rc;
	//调用api，完成sql语句
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
	//定义一个sqlite3的数据库实例指针
	sqlite3 *db;
	char *zErrMsg=NULL;
	//返回值变量，用于接收函数返回值，判断函数是否执行成功
	int rc;
	char sql[MAX_COMMAND];

	//打开/创建数据库
	rc=sqlite3_open("test.db",&db);
	if(rc)
	{
		printf("Can't open database:%s\n",sqlite3_errmsg(db));
		return 1;
	}
	printf("open database successfully\n");
	//填充创建表的sql语句到字符串中
	memset(sql,0,MAX_COMMAND);
	strcpy(sql,"create table company("\
	"ID int primary key not null,"\
	"name text not null,"\
	"age int not null,"\
	"address char(50),"\
	"salary real);");
	actionSql(sql,db);

	//填充插入记录的sql语句到字符串中
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


	//查看数据select
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


	//关闭数据库
	sqlite3_close(db);
	return 0;
}

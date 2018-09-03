#include "login.h"
#include <stdio.h>
#include<string.h>
#include "queue.h"
#include<pthread.h>

#define ERROR_TIME 3

enum logCommand
{
	logCommand_add,
	logCommand_del,
	logCommand_chg,
	logCommand_lis,
	logCommand_max,
};

typedef struct
{
	enum logCommand commandNo;
	char commandStr[LOGIN_MIN_SIZE];
	void(*p)(char pCommandBlock[LOGIN_MIN_SIZE][LOGIN_MIN_SIZE]);
}LoginCommand;

const char db_path[]={"userLog.db"};
int found=0;
int maxId = 0;

static SqQueue *g_pLogQueue;
const LoginCommand loginCom[] = {
	{logCommand_add,"add",NULL},
	{logCommand_del,"del",NULL},
	{logCommand_chg,"chg",NULL},
	{logCommand_lis,"lis",NULL},
};

static int zFind(void *notUse,int argc,char** argv,char** azColName)
{
	found=1;
	return 0;
}

static int zMaxId(void *notUse, int argc, char** argv, char** azColName)
{
	maxId = atoi(argv[0]);
	return 0;
}

void logComAdd(char pCommandBlock[LOGIN_MIN_SIZE][LOGIN_MIN_SIZE])
{
	//insert into user
	//select * from User order by uId desc limit 1;
	sqlite3 *db;
	char * zerrmsg;
	int rc;
	char tmpSql[LOGIN_MAX_SIZE];
	rc = sqlite3_open(db_path, &db);
	if (rc != 0)
	{
		printf("can't open %s \n", db_path);
		return ;
	}
	//obtain exist Max uId
	memset(tmpSql, 0, LOGIN_MAX_SIZE);
	strcpy(tmpSql, "select * from User order by uId desc limit 1;");
	sqlite3_exec(db, tmpSql, zMaxId, NULL, &zerrmsg);
	if (1 == found)
	{
		found = 0;
		sqlite3_close(db);
		return ;
	}
	//add user record
	memset(tmpSql, 0, LOGIN_MAX_SIZE);
	sprintf(tmpSql, "insert into User values(%d,'%s','%s','%s');",maxId+1, pCommandBlock[1], pCommandBlock[2], pCommandBlock[3]);
	sqlite3_exec(db, tmpSql, NULL, NULL, &zerrmsg);
	sqlite3_close(db);
}

int checkUser()
{
	int i;
	sqlite3 *db;
	char * zerrmsg;
	
	Account tmpAccount;
	char tmpSql[LOGIN_MAX_SIZE];

	for(i=0;i<ERROR_TIME;i++)
	{
		printf("please input user name:");
		gets(tmpAccount.userName);
		printf("please input password:");
		gets(tmpAccount.passWord);

		//�����ݿ���user���ڣ������û�������û���������
		int rc;
		rc=sqlite3_open(db_path,&db);
		if(rc!=0)
		{
			printf("can't open %s \n",db_path);
			return 1;
		}
		memset(tmpSql,0,LOGIN_MAX_SIZE);
		sprintf(tmpSql,"select * from User where uName='%s' and uPassword = '%s';",tmpAccount.userName,tmpAccount.passWord);	
		sqlite3_exec(db,tmpSql,zFind,NULL,&zerrmsg);
		if(1==found)
		{
			found=0;
			sqlite3_close(db);
			return 0;
		}
		sqlite3_close(db);
		printf("your user name or passwd error");
		printf("please input again!\n");
	}
	printf("your input three times,system quit!");
	
	return 1;
}

void str_depart1(char *str, char c[LOGIN_MIN_SIZE][LOGIN_MIN_SIZE])
{
	int len, k, m = 0, n = 0;
	len = strlen(str);
	for (k = 0; k<len; k++)
	{
		if (str[k] != ' ')
		{
			c[m][n++] = str[k];
		}
		else
		{
			if (n != 0)
			{
				m++;
				n = 0;
			}
		}
	}
}

void logThread()
{
	ElemType message;
	char a[LOGIN_MIN_SIZE][LOGIN_MIN_SIZE] = { 0 };
	int i = 0;
	while (1)
	{
		//check queue empty or not 
		if(is_EmptyQueue(g_pLogQueue))
		{
			sleep(2);
			continue;
		}
		//out queue 
		memset(&message, 0, sizeof(message));
		out_CirQueue(g_pLogQueue, &message);
		//parse and act
		str_depart1(message.command, a);
		printf("////////////////////////////////\n");
		for (i = 0;i<logCommand_max; i++)
		{
			if (!strcmp(a[0], loginCom[i].commandStr))
			{
				printf("*********************************\n");
				loginCom[i].p(a);
			}
		}
	
	}
}


void accountConfig()
{
	char tmpCommand[LOGIN_MAX_SIZE] = { 0 };
	ElemType message;
	int ret, i = 0;
	//create son pthread
	pthread_t id;
	pthread_attr_t attr;
	//线程属性初始化
	pthread_attr_init(&attr);
	//设置绑定属性
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	//创建线程
	ret = pthread_create(&id, &attr, (void*)logThread, NULL);
	if (-1 == ret)
	{
		printf("thread create failure,Account control quit!\n");
		return;
	}
	//创建队列
	g_pLogQueue = init_CirQueue();
	if (NULL == g_pLogQueue)
	{
		printf("queue create failure,Account control quit!\n");
		goto quit;
	}
	setbuf(stdin, NULL);
	while (1)
	{
		printf(">>");
		
		memset(tmpCommand, 0, LOGIN_MAX_SIZE);
		gets(tmpCommand);
		if (!strcmp(tmpCommand, "q"))
		{
			break;
		}
		//入队
		message.commandID = i++;
		memset(message.command, 0, sizeof(message.command));
		strcpy(message.command, tmpCommand);
		while (1)
		{
			if (RET_OK == in_CirQueue(g_pLogQueue, message))
			{
				break;
			}
			sleep(2);
		}
	}
	uinit_CirQueue(g_pLogQueue);
quit:
	pthread_cancel(id);
	pthread_join(id, NULL);
}
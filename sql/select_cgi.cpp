#include "sql.h"

//脚本从子进程中读取参数
int ReadQuery_String(char buf[])
{
	char *method = NULL;
	char *arg_string = NULL;
	char *content_len = NULL;
	
	method = getenv("METHOD");
	if(method == NULL)
	{
		std::cout<<"get mothod error"<<std::endl;
		return -5;
	}
	//method 不为空，get方法参数从环境变量QUERY_STRING
	if(method && strcasecmp(method, "Get") == 0)
	{
		arg_string = getenv("QUERY_STRING");
		if(!arg_string)
		{
			return -1;
		}
		strcpy(buf,arg_string);
	}
	//post 方法，先读取content_len，再读取参数
	else if(method && strcasecmp(method,"POST") == 0)
	{
		content_len = getenv("CONTENT_LENGTH");
		if(!content_len)
		{
			return -2;
		}
		int nums = atoi(content_len);
		int i=0;
		char c='\0';
		for(;i < nums;++i)
		{
			//读取参数
			read(0,&c,1);//子进程的input[0]重定向到0
			buf[i] = c;
		}
		buf[i] = '\0';
	}
	else
	{
		return -3;
	}
	return 0;
}
int main()
{
	sql obj("root","","127.0.1","class",3306);
	obj.connect();
	char buf[1024];
	ReadQuery_String(buf);
	obj.select();
	return 0;
}

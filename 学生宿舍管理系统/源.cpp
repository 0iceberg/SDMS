#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

#define FILE_ACCOUNT "account"  //用户文件
#define FILE_STUDENT "student"  //学生文件
#define ACCOUNT sizeof(account) //-sizeof(account *)//账号结构体大小
#define STUDENT sizeof(student) //学生结构体大小
#define MAX 20					//账户密码最大值
#define ADMIN_OPTION 7			//管理员选项总数
#define USER_OPTION 5			//普通用户选项总数
#define STUDENT_OPTION 8        //学生结构体内容总数
#define SEEK_STUDENT 7			//寻找函数内容选项总数
#define UP 'w'					//向上
#define DOWN 's'				//向下
#define ENTER 13				//回车
#define ESC 27				    //ESC
#define getch _getch

typedef struct account
{
	char	 number[MAX];  //账号
	char	 password[MAX];//密码
	int		 jurisdiction; //权限
	account* next;
}account;

typedef struct student
{
	int			  bednum;			   //床位号
	char		  name[MAX];		   //姓名
	char		  boss_name[MAX];	   //寝室长姓名
	char		  bossPH_num[MAX];     //寝室长电话
	char		  phone_num[MAX];	   //电话号
	unsigned long student_num;		   //学号
	unsigned long boss_num;			   //寝室长学号
	unsigned long dormitory_no;		   //宿舍号
	student*	  next;
}student;

typedef struct _XY
{
	int x=0; int special_x=0;
	int y=0; int special_y=0;
}XY;

//------------------------------------打印函数------------------------------------------

XY Print_login(
	int		mode
);//打印登录界面

XY Print_Admin(
);//打印管理员界面

XY Print_User(
);//打印普通用户界面

XY print_account(
	account* head
);//打印统计用户界面

XY print_student(
	int		 mode,
	student* head
);//打印学生详情

XY print_seek(
);//打印查找界面

//------------------------------------操作函数------------------------------------------

void gotoxy(
	int		x,
	int		y
);//光标跳转

FILE* File_access(
	const char* FileName,
	int			mode,
	errno_t*	Err
);//文件访问

int cursor_move(
	XY		Interface,
	int		max
);//选项光标移动

void quit(
	XY		 Interface
);//简易退出框

//------------------------------------账号操作函数---------------------------------------

int connect_account(
	account**	head
);//生成用户链表

int delete_accout(
	XY		 Interface_account,
	account* head
);//删除用户

int reset_password(
	XY			Interface_account,
	account*	head,
	int			jurisdiction,
	char*	    number
);//重置用户密码

//------------------------------------学生操作函数---------------------------------------

int student_registration(
	XY		 Interface_student
);//学生注册

int connect_student(
	student** head
);//生成学生链表

student* student_Seek(
	XY		 Interface_student,
	student* head
);//学生信息-寻找

void student_Change(
	XY		 Interface_student,
	student* user
);//学生信息-更改

void student_Delete(
	student* user,
	student* head
);//学生信息-更改

int student_file(
	student* head
);//学生信息-文件

int dormitory_seek(
	student* head
);//查询宿舍信息

//------------------------------------界面函数-------------------------------------------

int Sign_in(
	int*   jurisdiction
);//登录

int Register(
);//注册

void Admin_interface(
);//管理员界面

void User_interface(
	char *user_number
);//普通用户界面

int student_Menu(
	XY		 Interface_student,
	student* head
);//学生信息-菜单

int seek_student(
	XY		 Interface_seek,
	student* head
);//查询学生住宿信息

//--------------------------------------------------------------------------------------
char user_number[MAX];
int main()
{
	system("color 9f");
	int judge;//判断登录是否成功
	int mode;//账户权限模式
	for (;;) //程序主循环
	{
		do
		{

			system("cls");
			judge = Sign_in(&mode);
		} while (!judge);
		if (mode)//拥有管理员权限
			Admin_interface();
		else//普通用户
		{
			User_interface(user_number);
		}
	}

}

void gotoxy(int x, int y)//光标跳转
{
	COORD pos;
	HANDLE houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(houtput, pos);//跳转

	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	SetConsoleCursorInfo(houtput, &cursor);//光标隐藏
}

XY Print_login(int mode)//打印登录界面
{
	int x = 15, y = 10;
	XY Interface; Interface.x = x; Interface.y = y;
	switch (mode)
	{
		case 0://初始化界面
			{			
			gotoxy(x, y);
			printf(" _______________________________"); y++;
			gotoxy(x, y);
			printf("|                               |"); y++;
			gotoxy(x, y);
			printf("|     账号:                     |"); y++;
			gotoxy(x, y);
			printf("|     密码:                     |"); y++;
			gotoxy(x, y);
			printf("|        [1]登录  [2]注册       |"); y++;
			gotoxy(x, y);
			printf("|_______________________________|"); y++;
			break;
			}
		case 1://登录
			{
			y += 2;//跳转到中间行
			gotoxy(x, y);
			printf("|     账号:                     |"); y++;
			gotoxy(x, y);
			printf("|     密码:                     |"); y++;
			gotoxy(x, y);
			printf("|                               |"); y++;
				break;
			}
		case 2://注册
		{
			y += 3;//跳转到中间行
			gotoxy(x, y);
			printf("|     密码:                     |"); y++;
			gotoxy(x, y);
			printf("| 再次输入:                     |");
			break;
		}
		case 3://管理员
		{
			y += 2;//跳转到中间行
			gotoxy(x, y);
			printf("|       已进入管理员注册        |"); y++;
			gotoxy(x, y);
			printf("|     密码:                     |"); y++;
			gotoxy(x, y);
			printf("| 再次输入:                     |");
			break;
		}

	}
	return Interface;
}

int Sign_in(int* jurisdiction)//登录
{
	account user;//登录用户
	char test_number[MAX], test_password[MAX];
	Print_login(0);//打印初始化界面
	char mode;
	do{
		mode = getch();
	} while (mode != '1' && mode != '2');//输入操作
	if (mode == '2')
	{
		*jurisdiction=Register();
		return 1;
	}//注册
	//登录
	errno_t err=0;
	FILE* fp = File_access(FILE_ACCOUNT, 2,&err);//打开用户文件
	if (err) return 0;
	XY Interface = Print_login(1);
	Interface.x += 11; Interface.y += 2;//坐标初始化
	gotoxy(Interface.x, Interface.y);//输入账号
	scanf_s("%s", test_number, 20); Interface.y++;
	gotoxy(Interface.x, Interface.y);//输入密码
	scanf_s("%s", test_password, 20); Interface.y++;
	for (;!feof(fp);)
	{
		fread(&user, ACCOUNT, 1, fp);
		if (!strcmp(test_number, user.number))//匹配账号
		{
			if (!strcmp(test_password, user.password))//匹配密码
			{
				gotoxy(Interface.x, Interface.y);
				printf("登录成功按任意键继续");
				getch();
				if(!user.jurisdiction)
				for (int i = 0; i < MAX; i++)
				{
					user_number[i] = user.number[i];
				}
				*jurisdiction = user.jurisdiction;//返回用户权限
				fclose(fp);
				return 1;
			}
			else
			{
				gotoxy(Interface.x, Interface.y);
				printf("密码错误按任意键继续");
				getch();
				fclose(fp);
				return 0;
			}
		}

	}
	gotoxy(Interface.x, Interface.y);//循环到文件尾
	printf("账号错误按任意键继续");
	getch();
	fclose(fp);
	return 0;
}

int Register()//注册
{
	account user;//注册用户
	char testing[20];//检测密码
	char ADM[20] = "10086110";//注册管理员密码
	XY Interface=Print_login(2);
	Interface.x += 11; Interface.y += 2;//坐标初始化
	gotoxy(Interface.x, Interface.y);//输入账号
	scanf_s("%s", user.number, 20); Interface.y++;
	do {
		gotoxy(Interface.x, Interface.y);//输入密码
		scanf_s("%s", user.password, 20); Interface.y++;

		if (!strcmp(user.password, ADM))//输入管理员密码
		{
			Print_login(3);//打印新界面
			user.jurisdiction = 1; Interface.y--;
			continue;
		}
		else
			if(user.jurisdiction!=1)//防止权限覆盖
				user.jurisdiction = 0;//普通用户权限

		gotoxy(Interface.x, Interface.y);//输入重复密码
		scanf_s("%s", testing, 20);
		if (strcmp(user.password, testing))//判定输入是否正确
		{
			gotoxy(Interface.x, Interface.y);
			printf("输入错误按任意键继续"); Interface.y--;
			getch();
			Print_login(2);
		}
	} while (strcmp(user.password, testing));
	errno_t err;
	FILE *fp=File_access(FILE_ACCOUNT, 1,&err);
	fwrite(&user, ACCOUNT, 1, fp);
	fclose(fp);
	return user.jurisdiction;//返回权限
}


/*
存在巨大漏洞还未修复 XD
写入修复完成  【未测试】
*/
FILE *File_access(const char* FileName,int mode,errno_t *Err)//文件访问
{
	FILE* fp=NULL;	
	errno_t err;
	switch (mode)
	{
		case 1://写入
		{
			err = fopen_s(&fp, FileName, "rb");//判断文件是否存在
			*Err = err;
			if (err == 2)//新建设置文件
			{
				err = fopen_s(&fp, FileName, "wb");
				*Err = err;
				if (err)
				{
					gotoxy(0, 0);
					printf("新建%s文件失败[%d]\n", FileName, err);
					system("pause");
					return NULL;
				}//新建设置文件失败
			}
			else if (err == 0)//文件存在
			{
				fclose(fp);
				err = fopen_s(&fp, FileName, "ab");
				*Err = err;
				if (err)
				{
					gotoxy(0, 0);
					printf("写入%s文件失败[%d]\n", FileName, err);
					system("pause");
					return NULL;
				}//写入账户文件失败
			}
			else if (err)//出现其他问题
			{
				gotoxy(0, 0);
				printf("文件检测失败[%d]\n", err);
				printf("建议：删除 %s 文件\n", FileName);
				system("pause");
				return NULL;
			}
			break;
		}
		case 2://读取
		{
			err = fopen_s(&fp, FileName, "rb");
			*Err = err;
			if (err == 2)
			{
				gotoxy(0, 0);
				printf("检测%s文件失败,请查看跟目录中是否存在%s文件\n", FileName,FileName);
				system("pause");
				return NULL;
			}
			else if (err)//读取出现问题
			{
				gotoxy(0, 0);
				printf("读取%s文件失败[%d]\n", FileName, err);
				system("pause");
				return NULL;
			}
			break;
		}
		case 3://复写
		{
			err = fopen_s(&fp, FileName, "wb");
			*Err = err;
			if (err)
			{
				gotoxy(0, 0);
				printf("新建%s文件失败[%d]\n", FileName, err);
				system("pause");
				return NULL;
			}//新建设置文件失败
			break;
		}
	}
		return fp;
}

XY Print_User()//打印普通用户界面
{
	int x = 15, y = 10;
	XY Interface; Interface.x = x; Interface.y = y;
	gotoxy(x, y);
	printf(" _______________________________________________"); y++;
	gotoxy(x, y);
	printf("|                                               |"); y++;
	gotoxy(x, y);
	printf("|             更改用户密码                      |"); y++;
	gotoxy(x, y);
	printf("|             浏览住宿入住信息                  |"); y++;
	gotoxy(x, y);
	printf("|             查询学生住宿信息                  |"); y++;
	gotoxy(x, y);
	printf("|             查询空闲床位                      |"); y++;
	gotoxy(x, y);
	printf("|             退出                              |"); y++;
	gotoxy(x, y);
	printf("|_______________________________________________|"); y++;
	return Interface;
}

void User_interface(char *user_number)//普通用户界面
{
	char* a = user_number;
	account* head_account;
	student* head_student;
	for (int quit = 1; quit;)
	{
		system("cls");
		XY Interface_user = Print_User();
		int mode = cursor_move(Interface_user, USER_OPTION);
		system("cls");
		switch (mode)
		{
			case 1:
			{
				system("cls");
				connect_account(&head_account);//生成用户链表
				reset_password(Interface_user, head_account, 2, user_number);
				break;
			}
			case 2:
			{
				if (!connect_student(&head_student))
					break;
				for (int i = 1; i;)
				{
					system("cls");
					i = dormitory_seek(head_student);
				}
				break;
			}
			case 3:
			{
				if (!connect_student(&head_student))
					break;
				for (int i = 1; i;)
				{
					system("cls");
					XY Interface_seek = print_seek();
					i = seek_student(Interface_seek, head_student);
				}
				break;
			}
			case USER_OPTION://退出
			{
				quit = 0;
				break;
			}
		}
	}
}

int cursor_move(XY Interface, int max)//选项光标移动
{
	int judge = 1;//位置判断
	Interface.x += 3;int y=Interface.y += 1;//位置初始化
	for (char mode;;)
	{
		gotoxy(Interface.x, Interface.y + judge);
		printf("->");
		do {
			mode = getch();
		} while (mode != UP && mode != DOWN && mode != ENTER);
		gotoxy(Interface.x, Interface.y+judge);
		printf("  ");
		if (mode == UP) judge--;
		if (mode == DOWN) judge++;
		if (mode == ENTER) return judge;//确认选项
		if (judge > max) judge = 1;//超出最大选项
		if (judge < 1) judge = max;//超出最小选项
	}
	return 1;
}

XY Print_Admin()//打印管理员界面
{
	int x = 15, y = 10;
	XY Interface; Interface.x = x; Interface.y = y;
	gotoxy(x, y);
	printf(" _______________________________________________"); y++;
	gotoxy(x, y);
	printf("|                                               |"); y++;
	gotoxy(x, y);
	printf("|             创建用户账号                      |"); y++;
	gotoxy(x, y);
	printf("|             删除用户账号                      |"); y++;
	gotoxy(x, y);
	printf("|             重置用户密码                      |"); y++;
	gotoxy(x, y);
	printf("|             输入学生住宿信息                  |"); y++;
	gotoxy(x, y);
	printf("|             更新学生住宿信息                  |"); y++;
	gotoxy(x, y);
	printf("|             查询住宿信息                      |"); y++;
	gotoxy(x, y);
	printf("|             退出                              |"); y++;
	gotoxy(x, y);
	printf("|_______________________________________________|"); y++;
	return Interface;
}

void Admin_interface()//管理员界面
{
	account* head_account;
	student* head_student;
	for (int quit=1;quit;)
	{
		system("cls");
		connect_account(&head_account);//生成用户链表
		XY Interface_admin=Print_Admin();
		int mode = cursor_move(Interface_admin, ADMIN_OPTION);
		system("cls");
		switch (mode)
		{
			case 1://创建账户
			{
				Print_login(0);
				Register();
				break;
			}
			case 2://删除账户
			{
				for (int i = 1; i;)
				{
					system("cls");
					if (!connect_account(&head_account))
						break;
					XY Interface_account = print_account(head_account);
					i = delete_accout(Interface_account, head_account);
				}
				break;
			}
			case 3://重置用户密码
			{
				for (int i = 1; i;)
				{
					system("cls");
					if (!connect_account(&head_account))
						break;
					XY Interface_account = print_account(head_account);
					i = reset_password(Interface_account, head_account, 1,NULL);
				}
				break;
			}
			case 4://输入学生住宿信息
			{
				for (int i = 1; i;)
				{
					system("cls");
					XY Interface_student = print_student(0, NULL);
					i = student_registration(Interface_student);
				}
				break;
			}
			case 5://更新学生住宿信息
			{
				for (int i = 1; i;)
				{
					system("cls");
					if (!connect_student(&head_student))
						break;
					XY Interface_student = print_student(1, head_student);
					i = student_Menu(Interface_student,head_student);
				}
				break;
			}
			case 6://查询住宿信息
			{
				if (!connect_student(&head_student))
					break;
				for (int i = 1; i;)
				{
					system("cls");
					XY Interface_seek = print_seek();
					i=seek_student(Interface_seek, head_student);
				}
				break;
			}
			case ADMIN_OPTION://退出
			{
				quit = 0;
				break;
			}
		}
	}
}

XY print_account(account* head)//打印统计用户界面
{
	int x = 15, y = 10;
	XY Interface_account;
	gotoxy(x, y);
	printf(" ________________________________________________________________"); y++;
	for (; head; head = head->next)
	{
		gotoxy(x, y);
		printf("|                                                                |"); y++;
		gotoxy(x, y);
		printf("| 账号:%-20s   密码:%-20s    权限%d |", head->number, head->password, head->jurisdiction); y++;
	}
	gotoxy(x, y);
	printf("|________________________________________________________________|"); y++;
	Interface_account.x = x; Interface_account.y = y;
	return Interface_account;
}

int delete_accout(XY Interface_account,account *head)//删除用户
{
	gotoxy(Interface_account.x, Interface_account.y);
	printf("|                                                                |"); Interface_account.y++;
	gotoxy(Interface_account.x, Interface_account.y); int y;//定位该行
	printf("| 删除账户:                                                      |"); y=Interface_account.y++;
	gotoxy(Interface_account.x, Interface_account.y);
	printf("|              [ENTER]输入     [ESC]退出                         |"); Interface_account.y++;
	gotoxy(Interface_account.x, Interface_account.y);
	printf("|________________________________________________________________|");
	char delete_number[MAX];
	while(1)
	{
		char t = getch();
		if (t == ESC)
			return 0;
		else if (t == ENTER)
		{
			account* p1 = head;
			account* p2 = head;
			if (head == NULL)//用户链表为空
				return 0;
			int judge=0;//判断
			gotoxy(Interface_account.x, y);
			printf("| 删除账户:                                                      |");
			gotoxy(Interface_account.x,y+1);
			printf("|                                                                |");
			gotoxy(Interface_account.x + 11, y);
			scanf_s("%s", delete_number, MAX);
			for (;p2;)
			{
				if (p2 == NULL) return 1;
				if(!strcmp(p2->number,delete_number))
				{
					if (p2 == head)//删除用户在头节点
					{
						head = head->next;
						judge = 1;
						break;
					}
					else if (p2->next != NULL)//删除用户处于正常位置
					{
						p1->next = p2->next;
						judge = 1;
						break;
					}
					else if (p2->next == NULL)//删除用户处于尾端
					{
						p1->next = NULL;
						judge = 1;
						break;
					}

				}
				p1 = p2;
				p2 = p2->next;
			}
			if (judge)
			{
				errno_t err;
				FILE* fp = File_access(FILE_ACCOUNT, 3, &err);
				if (err) return 0;//判断文件是否报错
				for (p1 = head; p1; p1 = p1->next)//文件写入
				{
					fwrite(p1, ACCOUNT, 1, fp);
				}
				fclose(fp);
				gotoxy(Interface_account.x, y);
				printf("| 删除账户:删除成功                                              |"); y++;
				gotoxy(Interface_account.x+1, y);
				system("pause");
				return 1;
			}
			else 
			{
				gotoxy(Interface_account.x, y);
				printf("| 删除账户:删除失败                                              |"); y++;
				gotoxy(Interface_account.x+1, y);
				system("pause");
				return 1;
			}

		}
	}

}
/*
接下来这个函数可能会很乱
请耐心阅读
*/
int reset_password(XY Interface_account, account* head,int jurisdiction,char *number)//重置用户密码
{
	switch (jurisdiction)
	{
		case 1:
		{
			print_account(head);
			gotoxy(Interface_account.x, Interface_account.y);
			printf("|                                                                |"); Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y); int number_y;//定位账户行
			printf("| 重置账户:                                                      |"); number_y = Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y); int password_y;//定位账户行
			printf("|   新密码:                                                      |"); password_y = Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y);
			printf("|              [ENTER]输入     [ESC]退出                         |"); Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y);
			printf("|________________________________________________________________|");
			char reset_number[MAX];
			while (1)
			{
				char t = getch();
				if (t == ESC)
					return 0;
				else if (t == ENTER)
				{
					account* p1 = head;
					if (head == NULL)//用户链表为空
						return 0;
					int judge = 0;//判断
					gotoxy(Interface_account.x, number_y);
					printf("| 重置账户:                                                      |");
					gotoxy(Interface_account.x, password_y);
					printf("|   新密码:                                                      |");
					gotoxy(Interface_account.x, password_y+1);
					printf("|                                                                |");
					gotoxy(Interface_account.x + 11, number_y);
					scanf_s("%s", reset_number, MAX);
					for (; p1; p1 = p1->next)//寻找用户
					{
						if (!strcmp(p1->number, reset_number))
							break;
					}
					if (p1 == NULL)//没找到需要重置的账户
					{
						gotoxy(Interface_account.x, number_y);
						printf("| 重置账户:该用户不存在                                          |");
						gotoxy(Interface_account.x + 1, password_y);
						system("pause");
						return 1;
					}
					gotoxy(Interface_account.x+11, password_y);
					scanf_s("%s", p1->password, MAX);//写入新密码

					errno_t err;
					FILE* fp = File_access(FILE_ACCOUNT, 3, &err);
					if (err) return 0;//判断文件是否报错
					for (p1 = head; p1; p1 = p1->next)//文件写入
					{
						fwrite(p1, ACCOUNT, 1, fp);
					}
					fclose(fp);
					gotoxy(Interface_account.x, password_y);
					printf("|   新密码:修改成功                                              |");
					gotoxy(Interface_account.x + 1, number_y);
					system("pause");
					return 1;
				}

			}
			break;
		}
		case 2:
		{
			account* p1 = head;
			for (; p1; p1 = p1->next)//寻找用户
			{
				if (!strcmp(p1->number, number))
					break;
			}if (p1 == NULL)break;
			gotoxy(Interface_account.x, Interface_account.y);
			printf(" __________________________________________"); Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y);
			printf("|                                          |"); Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y);
			printf("| 当前用户:%-20s            |",p1->number); Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y);
			printf("| 当前密码:%-20s            |", p1->password); Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y);
			printf("|         [ENTER]更改   [ESC]退出          |"); Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y);
			printf("|__________________________________________|"); Interface_account.y--;
			char t;
			do {
				t = getch();
			} while (t != ESC && t != ENTER);
			if (t == ESC)
				return 0;
			gotoxy(Interface_account.x, Interface_account.y);
			printf("| 新密码:                                  |");
			gotoxy(Interface_account.x+9, Interface_account.y--);
			scanf_s("%s", p1->password, MAX);
			errno_t err;
			FILE* fp = File_access(FILE_ACCOUNT, 3, &err);
			if (err) return 0;//判断文件是否报错
			gotoxy(Interface_account.x, Interface_account.y);
			printf("| 当前密码:%-20s            |", p1->password); Interface_account.y++;
			for (p1 = head; p1; p1 = p1->next)//文件写入
			{
				fwrite(p1, ACCOUNT, 1, fp);
			}
			fclose(fp);
			gotoxy(Interface_account.x, Interface_account.y);
			printf("| 新密码:修改成功 ");
			system("pause");
			return 1;
			break;
		}
	}
	return 0;

}

void quit(XY Interface)//简易退出框
{
	gotoxy(Interface.x, Interface.y);
	printf(" __________________________________"); Interface.y++;
	gotoxy(Interface.x, Interface.y);
	printf("|                                  |"); Interface.y++;
	gotoxy(Interface.x, Interface.y);
	printf("|           [ESC]结束浏览          |"); Interface.y++;
	gotoxy(Interface.x, Interface.y);
	printf("|__________________________________|");
	while (getch() != ESC);
}

XY print_seek()//打印查找界面
{
	int x = 15, y = 10;
	XY Interface;
	Interface.x = x; Interface.y = y;
	gotoxy(x, y);
	printf(" ____________________________________"); y++;
	gotoxy(x, y);
	printf("|                                    |"); y++;
	gotoxy(x, y);
	printf("|           按宿舍号查询             |"); y++;
	gotoxy(x, y);
	printf("|           按寝室长学号查询         |"); y++;
	gotoxy(x, y);
	printf("|           按寝室长电话查询         |"); y++;
	gotoxy(x, y);
	printf("|           按本人学号查询           |"); y++;
	gotoxy(x, y);
	printf("|           按本人电话查询           |"); y++;
	gotoxy(x, y);
	printf("|           按本人姓名查询           |"); y++;
	gotoxy(x, y);
	printf("|           退出                     |"); y++;
	gotoxy(x, y);
	printf("|____________________________________|"); y++;
	gotoxy(x, y);
	printf("|                                    |"); y++;
	gotoxy(x, y);
	printf("|              请选择                |"); Interface.special_y = y++;
	gotoxy(x, y);
	printf("|____________________________________|"); y++;
	Interface.special_x = x;
	return Interface;
}

int dormitory_seek(student* head)//查询宿舍信息
{
	int x = 15, y = 10;
	unsigned long user_long;
	XY Interface_seek;
	gotoxy(x,y);
	printf(" __________________________________");y++;
	gotoxy(x, y);
	printf("|                                  |");y++;
	gotoxy(x, y);
	printf("| 宿舍号:                          |");y++;
	gotoxy(x, y);
	printf("|      [ENTER]输入 [ESC]退出       |");y++;
	gotoxy(x, y);
	printf("|__________________________________|"); y--;
	char t;
	do {
		t = getch();
	} while (t != ESC && t != ENTER);
	if (t == ESC) 
		return 0;
	static int j = 9;
	j = 9;
	gotoxy(x, y);
	printf("|                                  |"); y--;
	gotoxy(x+9, y);
	scanf_s("%d", &user_long);
	scanf_s("%*[^\n]%*c");
	system("cls");
	for (int i=1;i<=8;i++)
	{
		for (student* p2 = head; p2; p2=p2->next)
		{
			if (p2->dormitory_no == user_long)
			{
				if (p2->bednum == i)
				{
					Interface_seek=print_student(4, p2);
					break;
				}
			}
			if (p2->next == NULL)
			{
				gotoxy(x - 10+j,y - 4);
				printf("%d床位[空]  ", i); j += 9;
			}
		}

	}
	quit(Interface_seek);
	print_student(5, NULL);
}

int seek_student(XY	Interface_seek, student* head)//查询学生住宿信息
{
	int mode = cursor_move(Interface_seek, SEEK_STUDENT);
	unsigned long user_long;
	char user_char[MAX];
	student* p1;
	switch (mode)
	{
		case 1://按宿舍号查找
		{
			gotoxy(Interface_seek.special_x, Interface_seek.special_y);
			printf("|       宿舍号:                      |"); 
			gotoxy(Interface_seek.special_x+16, Interface_seek.special_y);
			scanf_s("%d", &user_long);
			system("cls");
			for (p1 = head; p1; p1 = p1->next)
			{
				if (p1->dormitory_no == user_long)
				Interface_seek = print_student(4, p1);
			}
			print_student(5, NULL);
			quit(Interface_seek);
			break;
		}
		case 2://按寝室长学号查找
		{
			gotoxy(Interface_seek.special_x, Interface_seek.special_y);
			printf("|   寝室长学号:                      |");
			gotoxy(Interface_seek.special_x + 16, Interface_seek.special_y);
			scanf_s("%d", &user_long);
			system("cls");
			for (p1 = head; p1; p1 = p1->next)
			{
				if (p1->boss_num == user_long)
					Interface_seek = print_student(4, p1);
			}
			print_student(5, NULL);
			quit(Interface_seek);
			break;
		}
		case 3://按寝室长电话查找
		{
			gotoxy(Interface_seek.special_x, Interface_seek.special_y);
			printf("|   寝室长电话:                      |");
			gotoxy(Interface_seek.special_x + 16, Interface_seek.special_y);
			scanf_s("%s", user_char,MAX);
			system("cls");
			for (p1 = head; p1; p1 = p1->next)
			{
				if (!strcmp(p1->bossPH_num,user_char))
					Interface_seek = print_student(4, p1);
			}
			print_student(5, NULL);
			quit(Interface_seek);
			break;
		}
		case 4://按本人学号查找
		{
			gotoxy(Interface_seek.special_x, Interface_seek.special_y);
			printf("|   本人学号:                        |");
			gotoxy(Interface_seek.special_x + 16, Interface_seek.special_y);
			scanf_s("%d", &user_long);
			system("cls");
			for (p1 = head; p1; p1 = p1->next)
			{
				if (p1->student_num == user_long)
					Interface_seek = print_student(4, p1);
			}
			print_student(5, NULL);
			quit(Interface_seek);
			break;
		}
		case 5://按本人电话查找
		{
			gotoxy(Interface_seek.special_x, Interface_seek.special_y);
			printf("|   本人电话:                        |");
			gotoxy(Interface_seek.special_x + 16, Interface_seek.special_y);
			scanf_s("%s", user_char, MAX);
			system("cls");
			for (p1 = head; p1; p1 = p1->next)
			{
				if (!strcmp(p1->phone_num, user_char))
					Interface_seek = print_student(4, p1);
			}
			print_student(5, NULL);
			quit(Interface_seek);
			break;
		}
		case 6://按本人姓名查找
		{
			gotoxy(Interface_seek.special_x, Interface_seek.special_y);
			printf("|   本人姓名:                        |");
			gotoxy(Interface_seek.special_x + 16, Interface_seek.special_y);
			scanf_s("%s", user_char, MAX);
			system("cls");
			for (p1 = head; p1; p1 = p1->next)
			{
				if (!strcmp(p1->name, user_char))
					Interface_seek = print_student(4, p1);
			}
			print_student(5, NULL);
			quit(Interface_seek);
			break;
		}
		case SEEK_STUDENT:
		{
			return 0;
			break;
		}
	}
	return 1;
}
XY print_student(int mode, student* head)//打印学生详情
{
	int x = 15, y = 10;
	static int sx = x-5, sy = y, si = 1, location_y = y;
	XY Interface;
	switch (mode)
	{
		case 0://注册学生信息
		{
			gotoxy(x, y);
			printf(" ________________________________"); y++;
			gotoxy(x, y);
			printf("|                                |"); y++;
			gotoxy(x, y);
			printf("|       姓名:                    |");Interface.y = y++;
			gotoxy(x, y);
			printf("|       学号:                    |"); y++;
			gotoxy(x, y);
			printf("|     电话号:                    |"); y++;
			gotoxy(x, y);
			printf("|     宿舍号:                    |"); y++;
			gotoxy(x, y);
			printf("|     床位号:                    |"); y++;
			gotoxy(x, y);
			printf("| 寝室长姓名:                    |"); y++;
			gotoxy(x, y);
			printf("| 寝室长学号:                    |"); y++;
			gotoxy(x, y);
			printf("| 寝室长电话:                    |"); y++;
			gotoxy(x, y);
			printf("|________________________________|"); y++;
			gotoxy(x, y);
			printf("|                                |"); y++;
			gotoxy(x, y);
			printf("|     [ENTER]注册 [ESC]退出      |"); Interface.special_y = y++;
			gotoxy(x, y);
			printf("|________________________________|"); y++;
			Interface.x = x + 13;
			Interface.special_x = x + 2;
			break;
		}
		case 1://粗略打印学生信息
		{
			gotoxy(x, y);
			printf(" _________________________________________________________________________"); y++;
			for (; head; head = head->next)
			{
				gotoxy(x, y);
				printf("|                                                                         |"); y++;
				gotoxy(x, y);
				printf("| 姓名:%-20s   学号:%-10d   电话:%-11s   ...... |", head->name, head->student_num, head->phone_num); y++;
			}
			gotoxy(x, y);
			printf("|_________________________________________________________________________|"); Interface.y = ++y;
			Interface.x = x;
			break;
		}
		case 2://详细打印全体学生信息
		{
			x -= 5;
			int initial_x = x, initial_y = y;//记录初始坐标
			for (int i=1; head; head = head->next,i++)//i为一横显示计数
			{
				y = initial_y;
				gotoxy(x, y);
				printf(" __________________________________   "); y++;
				gotoxy(x, y);
				printf("|                                  |   "); y++;
				gotoxy(x, y);
				printf("|         姓名:%-20s|   ", head->name); y++;
				gotoxy(x, y);
				printf("|         学号:%-20d|   ", head->student_num); y++;
				gotoxy(x, y);
				printf("|       电话号:%-20s|   ", head->phone_num); y++;
				gotoxy(x, y);
				printf("|       宿舍号:%-20d|   ", head->dormitory_no); y++;
				gotoxy(x, y);
				printf("|       床位号:%-20d|   ", head->bednum); y++;
				gotoxy(x, y);
				printf("|   寝室长姓名:%-20s|   ", head->boss_name); y++;
				gotoxy(x, y);
				printf("|   寝室长学号:%-20d|   ", head->boss_num); y++;
				gotoxy(x, y);
				printf("|   寝室长电话:%-20s|   ", head->bossPH_num); y++;
				gotoxy(x, y);
				printf("|__________________________________|"); y++;
				x += 36;//横移
				if (i == 3)
				{
					i = 0;
					x = initial_x;
					initial_y += 13;
				}
			}
			Interface.x = x, Interface.y = y;
			if(initial_y-2!=y) Interface.y = y-4;//判断是否显示完一横
			break;
		}
		case 3://详细打印单个学生信息
		{
			Interface.x = x; Interface.y = y;
			gotoxy(x, y);
			printf(" ____________________________________   "); y++;
			gotoxy(x, y);
			printf("|                                    |   "); y++;
			gotoxy(x, y);
			printf("|           姓名:%-20s|   ", head->name); y++;
			gotoxy(x, y);
			printf("|           学号:%-20d|   ", head->student_num); y++;
			gotoxy(x, y);
			printf("|         电话号:%-20s|   ", head->phone_num); y++;
			gotoxy(x, y);
			printf("|         宿舍号:%-20d|   ", head->dormitory_no); y++;
			gotoxy(x, y);
			printf("|         床位号:%-20d|   ", head->bednum); y++;
			gotoxy(x, y);
			printf("|     寝室长姓名:%-20s|   ", head->boss_name); y++;
			gotoxy(x, y);
			printf("|     寝室长学号:%-20d|   ", head->boss_num); y++;
			gotoxy(x, y);
			printf("|     寝室长电话:%-20s|   ", head->bossPH_num); y++;
			gotoxy(x, y);
			printf("|____________________________________|"); y++;
			Interface.special_x = x; Interface.special_y = y;
			break;
		}
		case 4://间断打印学生详细信息
		{
			x -= 5;
			sy = location_y;
			gotoxy(sx, sy);
			printf(" __________________________________   \n"); sy++;
			gotoxy(sx, sy);
			printf("|                                  |   \n"); sy++;
			gotoxy(sx, sy);
			printf("|         姓名:%-20s|   \n", head->name); sy++;
			gotoxy(sx, sy);
			printf("|         学号:%-20d|   \n", head->student_num); sy++;
			gotoxy(sx, sy);
			printf("|       电话号:%-20s|   \n", head->phone_num); sy++;
			gotoxy(sx, sy);
			printf("|       宿舍号:%-20d|   \n", head->dormitory_no); sy++;
			gotoxy(sx, sy);
			printf("|       床位号:%-20d|   \n", head->bednum); sy++;
			gotoxy(sx, sy);
			printf("|   寝室长姓名:%-20s|   \n", head->boss_name); sy++;
			gotoxy(sx, sy);
			printf("|   寝室长学号:%-20d|   \n", head->boss_num); sy++;
			gotoxy(sx, sy);
			printf("|   寝室长电话:%-20s|   \n", head->bossPH_num); sy++;
			gotoxy(sx, sy);
			printf("|__________________________________|\n"); sy++;
			sx += 36;
			if (si == 3)
			{
				si = 0;
				sx = x; location_y += 13;
			}si++;
			Interface.x = sx, Interface.y = sy;
			if (location_y - 2 != sy) Interface.y = sy - 4;//判断是否显示完一横
			break;
		}
		case 5://间断打印复位
		{
			si = 1; sx = x-5; sy = y; location_y = y;
			break;
		}

	}
	return Interface;
}

int student_registration(XY Interface_student)//学生注册
{
	while (1)
	{
		char t = getch();
		if (t == ESC)//函数退出
			return 0;
		else if (t == ENTER)
		{
			errno_t err;
			FILE* fp = File_access(FILE_STUDENT, 1, &err);
			if (err) return 0;//报错处理
			student user;
			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("                               ");

			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("请输入姓名      ");
			gotoxy(Interface_student.x, Interface_student.y);
			scanf_s("%s", user.name, 20);Interface_student.y++;
			scanf_s("%*[^\n]%*c");

			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("请输入学号      ");
			gotoxy(Interface_student.x, Interface_student.y);
			scanf_s("%d", &user.student_num); Interface_student.y++;
			scanf_s("%*[^\n]%*c");

			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("请输入电话号     ");
			gotoxy(Interface_student.x, Interface_student.y);
			scanf_s("%s",user.phone_num,20); Interface_student.y++;
			scanf_s("%*[^\n]%*c");

			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("请输入宿舍号      ");
			gotoxy(Interface_student.x, Interface_student.y);
			scanf_s("%d", &user.dormitory_no); Interface_student.y++;
			scanf_s("%*[^\n]%*c");

			do {
				gotoxy(Interface_student.special_x, Interface_student.special_y);
				printf("请输入床位号[1-8]");
				gotoxy(Interface_student.x, Interface_student.y);
				scanf_s("%d", &user.bednum);
				scanf_s("%*[^\n]%*c");
				if (user.bednum <= 0 || user.bednum > 8)//床位号输入异常
				{
					gotoxy(Interface_student.x, Interface_student.y);
					printf("                    ");
					scanf_s("%*[^\n]%*c");
				}
			} while (user.bednum <=0|| user.bednum >8); Interface_student.y++;

			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("请输入寝室长姓名      ");
			gotoxy(Interface_student.x, Interface_student.y);
			scanf_s("%s", user.boss_name,20); Interface_student.y++;
			scanf_s("%*[^\n]%*c");

			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("请输入寝室长学号      ");
			gotoxy(Interface_student.x, Interface_student.y);
			scanf_s("%d",&user.boss_num); Interface_student.y++;
			scanf_s("%*[^\n]%*c");

			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("请输入寝室长电话      ");
			gotoxy(Interface_student.x, Interface_student.y);
			scanf_s("%s", user.bossPH_num,20); Interface_student.y++;
			scanf_s("%*[^\n]%*c");

			fwrite(&user, STUDENT, 1, fp);
			fclose(fp);
			gotoxy(Interface_student.special_x-1, Interface_student.special_y);
			printf("学生注册成功");
			system("pause");
			return 1;
		}
	}
}

int student_Menu(XY Interface_student, student* head)//学生信息-菜单
{
	int x = Interface_student.x, y = Interface_student.y;
	gotoxy(x,y);
	printf("|                                                                         |"); y++;
	gotoxy(x, y);
	printf("|             [1]显示详情   [2]更改学生信息   [3]删除学生信息             |"); y++;
	gotoxy(x, y);
	printf("|                           [ESC]退出                                     |"); y++;
	gotoxy(x, y);
	printf("|_________________________________________________________________________|");
	char mode;
	do {
		mode = getch();
	} while (mode != '1' && mode != '2' && mode != '3' && mode != ESC);
	switch (mode)
	{
		case '1'://显示详细学生信息
		{
			system("cls");
			Interface_student=print_student(2, head);
			gotoxy(Interface_student.x, Interface_student.y);
			printf(" __________________________________"); Interface_student.y++;
			gotoxy(Interface_student.x, Interface_student.y);
			printf("|                                  |"); Interface_student.y++;
			gotoxy(Interface_student.x, Interface_student.y);
			printf("|           [ESC]结束浏览          |"); Interface_student.y++;
			gotoxy(Interface_student.x, Interface_student.y);
			printf("|__________________________________|");
			while (getch() != ESC);
			break;
		}
		case '2':
		{
			student* user=student_Seek(Interface_student,head);
			system("cls");
			if (!user)//查找失败
				break;
			student_Change(Interface_student, user);
			student_file(head);
			break;
		}
		case '3':
		{
			student* user = student_Seek(Interface_student, head);
			system("cls");
			if (!user)//查找失败
				break;
			student_Delete(user, head);
			break;
		}
		case ESC:
		{
			return 0;
			break;
		}
	}
	return 1;
}

void student_Change(XY Interface_student, student* user)//学生信息-更改
{
	Interface_student = print_student(3, user);
	while (1)
	{
		gotoxy(Interface_student.special_x, Interface_student.special_y);
		printf("|                                    |"); Interface_student.special_y++;
		gotoxy(Interface_student.special_x, Interface_student.special_y);
		printf("|          请选择需更改信息          |"); Interface_student.special_y++;
		gotoxy(Interface_student.special_x, Interface_student.special_y);
		printf("|____________________________________|"); Interface_student.special_y--;
		int mode = cursor_move(Interface_student, STUDENT_OPTION);
		switch (mode)
		{
			case 1:
			{
				gotoxy(Interface_student.special_x, Interface_student.special_y);
				printf("|       新姓名:                      |");
				gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
				scanf_s("%s", user->name, 20);
				break;
			}
			case 2:
			{
				gotoxy(Interface_student.special_x, Interface_student.special_y);
				printf("|       新学号:                      |");
				gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
				scanf_s("%d", &(user->student_num));
				break;
			}
			case 3:
			{
				gotoxy(Interface_student.special_x, Interface_student.special_y);
				printf("|       新电话:                      |");
				gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
				scanf_s("%s", user->phone_num, 20);
				break;
			}
			case 4:
			{
				gotoxy(Interface_student.special_x, Interface_student.special_y);
				printf("|       新宿舍:                      |");
				gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
				scanf_s("%d", &(user->dormitory_no));
				break;
			}
			case 5:
			{
				do {
					gotoxy(Interface_student.special_x, Interface_student.special_y);
					printf("|      新床位:                       |");
					gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
					scanf_s("%d", &(user->bednum));
					if (user->bednum <= 0 || user->bednum > 8)//床位号输入异常
					{
						gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
						printf("                    ");
					}
				} while (user->bednum <= 0 || user->bednum > 8);
				break;
			}
			case 6:
			{
				gotoxy(Interface_student.special_x, Interface_student.special_y);
				printf("| 新寝室长姓名:                      |");
				gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
				scanf_s("%s", user->boss_name, 20);
				break;
			}
			case 7:
			{
				gotoxy(Interface_student.special_x, Interface_student.special_y);
				printf("| 新寝室长学号:                      |");
				gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
				scanf_s("%d", &(user->boss_num));
				break;
			}
			case 8:
			{
				gotoxy(Interface_student.special_x, Interface_student.special_y);
				printf("| 新寝室长电话:                      |");
				gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
				scanf_s("%s", user->bossPH_num,20);
				break;
			}
		}
		print_student(3, user);
		gotoxy(Interface_student.special_x, Interface_student.special_y);
		printf("|     [ENTER]继续更改  [ESC]退出     |"); Interface_student.special_y--;
		char t;
		do {
			t = getch();
		} while (t != ENTER && t != ESC);
		if (t == ESC) break;
	}
}

void student_Delete(student* user, student* head)//学生信息-更改
{
	if (head)
	{
		XY Interface_student=print_student(3, user);
		gotoxy(Interface_student.special_x, Interface_student.special_y);
		printf("|                                    |"); Interface_student.special_y++;
		gotoxy(Interface_student.special_x, Interface_student.special_y);
		printf("|             确定删除吗             |"); Interface_student.special_y++;
		gotoxy(Interface_student.special_x, Interface_student.special_y);
		printf("|       [ENTER]确定  [ESC]取消       |"); Interface_student.special_y++;
		gotoxy(Interface_student.special_x, Interface_student.special_y);
		printf("|____________________________________|");
		char t;
		do {
			t = getch();
		} while (t != ENTER && t != ESC);
		if (t == ESC) { ; }
		else if (t == ENTER)
		{
			student* p1;
			for (p1 = head; p1; p1 = p1->next)
			{
				if (p1 == user) break;
				else if (p1->next == user) break;
			}
			if (p1 == head) head = head->next;
			else if (p1->next == user && user->next != NULL) p1->next = user->next;
			else if (p1->next == user && user->next == NULL) p1->next = NULL;
			student_file(head);
		}
	}
}

student* student_Seek(XY Interface_student, student* head)//学生信息-寻找
{
	gotoxy(Interface_student.x, Interface_student.y);
	printf("|                                                                         |"); Interface_student.y++;
	gotoxy(Interface_student.x, Interface_student.y);
	printf("| 学生学号:                                                               |"); Interface_student.y++;
	gotoxy(Interface_student.x, Interface_student.y);
	printf("|                      [ENTER]输入     [ESC]退出                          |"); Interface_student.y++;
	gotoxy(Interface_student.x, Interface_student.y);
	printf("|_________________________________________________________________________|");
	char t;
	do {
		t = getch();
	} while (t != ENTER && t != ESC);
	if (t == ESC)
		return NULL;//退出
	else if (t == ENTER)
	{
		
		gotoxy(Interface_student.x, --Interface_student.y);
		printf("|                                                                         |"); Interface_student.y--;
		gotoxy(Interface_student.x + 12, Interface_student.y);
		unsigned long student_num;
		scanf_s("%d", &student_num);
		for (; head; head = head->next)
		{
			if (head->student_num == student_num)
			{
				return head;
			}
		}
		gotoxy(Interface_student.x + 12, Interface_student.y);
		printf("此学号不存在");
		system("pause");
		return NULL;
	}
	return NULL;
}

int student_file(student* head)//学生信息-文件
{
	errno_t err;
	FILE *fp=File_access(FILE_STUDENT, 3, &err);
	if (err) return 0;//报错
	for(;head;head=head->next)
	fwrite(head, STUDENT, 1, fp);
	fclose(fp);
	return 1;
}

/*
下面两个函数本来想写一起
奈何技术有限全是bug
万般无奈之下只能分开
*/
int connect_account(account** head)//生成账户链表
{
	FILE* fp; errno_t err;//文件读取变量
	account* p1, * p2;
	*head = p1 =p2= (account*)malloc(ACCOUNT);
	p1->next = NULL;
	fp = File_access(FILE_ACCOUNT, 2, &err);
	if (err) return 0;//报错处理
	//printf("1234%d", ftell(fp));
	for (; !feof(fp);)
	{
		fread(p1, ACCOUNT, 1, fp);
		if(p1->jurisdiction != 1 && p1->jurisdiction != 0)
			break;
		p2 = (account*)malloc(ACCOUNT);
		p1->next = p2;
		p2 = p1;
		p1 = p1->next;
	}fclose(fp);
	if ((*head)->jurisdiction != 0 && (*head)->jurisdiction != 1)
	{
		*head = NULL;
	}
	p2->next = NULL;
	return 1;
}

int connect_student(student** head)//生成学生链表
{
	FILE* fp; errno_t err;//文件读取变量
	student* p1, * p2;
	*head = p1 = p2 = (student*)malloc(STUDENT);
	p1->next = NULL;
	fp = File_access(FILE_STUDENT, 2, &err);
	if (err) return 0;//报错处理
	//printf("1234%d", ftell(fp));
	for (; !feof(fp);)
	{
		fread(p1, STUDENT, 1, fp);
		if (p1->bednum <=0 || p1->bednum >8)
			break;
		p2 = (student*)malloc(STUDENT);
		p1->next = p2;
		p2 = p1;
		p1 = p1->next;
	}fclose(fp);
	if ((*head)->bednum <= 0 || (*head)->bednum > 8)
	{
		*head = NULL;
	}
	p2->next = NULL;
	return 1;
}

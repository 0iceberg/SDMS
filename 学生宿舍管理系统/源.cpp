#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

#define FILE_ACCOUNT "account"  //�û��ļ�
#define FILE_STUDENT "student"  //ѧ���ļ�
#define ACCOUNT sizeof(account) //-sizeof(account *)//�˺Žṹ���С
#define STUDENT sizeof(student) //ѧ���ṹ���С
#define MAX 20					//�˻��������ֵ
#define ADMIN_OPTION 7			//����Աѡ������
#define USER_OPTION 5			//��ͨ�û�ѡ������
#define STUDENT_OPTION 8        //ѧ���ṹ����������
#define SEEK_STUDENT 7			//Ѱ�Һ�������ѡ������
#define UP 'w'					//����
#define DOWN 's'				//����
#define ENTER 13				//�س�
#define ESC 27				    //ESC
#define getch _getch

typedef struct account
{
	char	 number[MAX];  //�˺�
	char	 password[MAX];//����
	int		 jurisdiction; //Ȩ��
	account* next;
}account;

typedef struct student
{
	int			  bednum;			   //��λ��
	char		  name[MAX];		   //����
	char		  boss_name[MAX];	   //���ҳ�����
	char		  bossPH_num[MAX];     //���ҳ��绰
	char		  phone_num[MAX];	   //�绰��
	unsigned long student_num;		   //ѧ��
	unsigned long boss_num;			   //���ҳ�ѧ��
	unsigned long dormitory_no;		   //�����
	student*	  next;
}student;

typedef struct _XY
{
	int x=0; int special_x=0;
	int y=0; int special_y=0;
}XY;

//------------------------------------��ӡ����------------------------------------------

XY Print_login(
	int		mode
);//��ӡ��¼����

XY Print_Admin(
);//��ӡ����Ա����

XY Print_User(
);//��ӡ��ͨ�û�����

XY print_account(
	account* head
);//��ӡͳ���û�����

XY print_student(
	int		 mode,
	student* head
);//��ӡѧ������

XY print_seek(
);//��ӡ���ҽ���

//------------------------------------��������------------------------------------------

void gotoxy(
	int		x,
	int		y
);//�����ת

FILE* File_access(
	const char* FileName,
	int			mode,
	errno_t*	Err
);//�ļ�����

int cursor_move(
	XY		Interface,
	int		max
);//ѡ�����ƶ�

void quit(
	XY		 Interface
);//�����˳���

//------------------------------------�˺Ų�������---------------------------------------

int connect_account(
	account**	head
);//�����û�����

int delete_accout(
	XY		 Interface_account,
	account* head
);//ɾ���û�

int reset_password(
	XY			Interface_account,
	account*	head,
	int			jurisdiction,
	char*	    number
);//�����û�����

//------------------------------------ѧ����������---------------------------------------

int student_registration(
	XY		 Interface_student
);//ѧ��ע��

int connect_student(
	student** head
);//����ѧ������

student* student_Seek(
	XY		 Interface_student,
	student* head
);//ѧ����Ϣ-Ѱ��

void student_Change(
	XY		 Interface_student,
	student* user
);//ѧ����Ϣ-����

void student_Delete(
	student* user,
	student* head
);//ѧ����Ϣ-����

int student_file(
	student* head
);//ѧ����Ϣ-�ļ�

int dormitory_seek(
	student* head
);//��ѯ������Ϣ

//------------------------------------���溯��-------------------------------------------

int Sign_in(
	int*   jurisdiction
);//��¼

int Register(
);//ע��

void Admin_interface(
);//����Ա����

void User_interface(
	char *user_number
);//��ͨ�û�����

int student_Menu(
	XY		 Interface_student,
	student* head
);//ѧ����Ϣ-�˵�

int seek_student(
	XY		 Interface_seek,
	student* head
);//��ѯѧ��ס����Ϣ

//--------------------------------------------------------------------------------------
char user_number[MAX];
int main()
{
	system("color 9f");
	int judge;//�жϵ�¼�Ƿ�ɹ�
	int mode;//�˻�Ȩ��ģʽ
	for (;;) //������ѭ��
	{
		do
		{

			system("cls");
			judge = Sign_in(&mode);
		} while (!judge);
		if (mode)//ӵ�й���ԱȨ��
			Admin_interface();
		else//��ͨ�û�
		{
			User_interface(user_number);
		}
	}

}

void gotoxy(int x, int y)//�����ת
{
	COORD pos;
	HANDLE houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(houtput, pos);//��ת

	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	SetConsoleCursorInfo(houtput, &cursor);//�������
}

XY Print_login(int mode)//��ӡ��¼����
{
	int x = 15, y = 10;
	XY Interface; Interface.x = x; Interface.y = y;
	switch (mode)
	{
		case 0://��ʼ������
			{			
			gotoxy(x, y);
			printf(" _______________________________"); y++;
			gotoxy(x, y);
			printf("|                               |"); y++;
			gotoxy(x, y);
			printf("|     �˺�:                     |"); y++;
			gotoxy(x, y);
			printf("|     ����:                     |"); y++;
			gotoxy(x, y);
			printf("|        [1]��¼  [2]ע��       |"); y++;
			gotoxy(x, y);
			printf("|_______________________________|"); y++;
			break;
			}
		case 1://��¼
			{
			y += 2;//��ת���м���
			gotoxy(x, y);
			printf("|     �˺�:                     |"); y++;
			gotoxy(x, y);
			printf("|     ����:                     |"); y++;
			gotoxy(x, y);
			printf("|                               |"); y++;
				break;
			}
		case 2://ע��
		{
			y += 3;//��ת���м���
			gotoxy(x, y);
			printf("|     ����:                     |"); y++;
			gotoxy(x, y);
			printf("| �ٴ�����:                     |");
			break;
		}
		case 3://����Ա
		{
			y += 2;//��ת���м���
			gotoxy(x, y);
			printf("|       �ѽ������Աע��        |"); y++;
			gotoxy(x, y);
			printf("|     ����:                     |"); y++;
			gotoxy(x, y);
			printf("| �ٴ�����:                     |");
			break;
		}

	}
	return Interface;
}

int Sign_in(int* jurisdiction)//��¼
{
	account user;//��¼�û�
	char test_number[MAX], test_password[MAX];
	Print_login(0);//��ӡ��ʼ������
	char mode;
	do{
		mode = getch();
	} while (mode != '1' && mode != '2');//�������
	if (mode == '2')
	{
		*jurisdiction=Register();
		return 1;
	}//ע��
	//��¼
	errno_t err=0;
	FILE* fp = File_access(FILE_ACCOUNT, 2,&err);//���û��ļ�
	if (err) return 0;
	XY Interface = Print_login(1);
	Interface.x += 11; Interface.y += 2;//�����ʼ��
	gotoxy(Interface.x, Interface.y);//�����˺�
	scanf_s("%s", test_number, 20); Interface.y++;
	gotoxy(Interface.x, Interface.y);//��������
	scanf_s("%s", test_password, 20); Interface.y++;
	for (;!feof(fp);)
	{
		fread(&user, ACCOUNT, 1, fp);
		if (!strcmp(test_number, user.number))//ƥ���˺�
		{
			if (!strcmp(test_password, user.password))//ƥ������
			{
				gotoxy(Interface.x, Interface.y);
				printf("��¼�ɹ������������");
				getch();
				if(!user.jurisdiction)
				for (int i = 0; i < MAX; i++)
				{
					user_number[i] = user.number[i];
				}
				*jurisdiction = user.jurisdiction;//�����û�Ȩ��
				fclose(fp);
				return 1;
			}
			else
			{
				gotoxy(Interface.x, Interface.y);
				printf("����������������");
				getch();
				fclose(fp);
				return 0;
			}
		}

	}
	gotoxy(Interface.x, Interface.y);//ѭ�����ļ�β
	printf("�˺Ŵ������������");
	getch();
	fclose(fp);
	return 0;
}

int Register()//ע��
{
	account user;//ע���û�
	char testing[20];//�������
	char ADM[20] = "10086110";//ע�����Ա����
	XY Interface=Print_login(2);
	Interface.x += 11; Interface.y += 2;//�����ʼ��
	gotoxy(Interface.x, Interface.y);//�����˺�
	scanf_s("%s", user.number, 20); Interface.y++;
	do {
		gotoxy(Interface.x, Interface.y);//��������
		scanf_s("%s", user.password, 20); Interface.y++;

		if (!strcmp(user.password, ADM))//�������Ա����
		{
			Print_login(3);//��ӡ�½���
			user.jurisdiction = 1; Interface.y--;
			continue;
		}
		else
			if(user.jurisdiction!=1)//��ֹȨ�޸���
				user.jurisdiction = 0;//��ͨ�û�Ȩ��

		gotoxy(Interface.x, Interface.y);//�����ظ�����
		scanf_s("%s", testing, 20);
		if (strcmp(user.password, testing))//�ж������Ƿ���ȷ
		{
			gotoxy(Interface.x, Interface.y);
			printf("����������������"); Interface.y--;
			getch();
			Print_login(2);
		}
	} while (strcmp(user.password, testing));
	errno_t err;
	FILE *fp=File_access(FILE_ACCOUNT, 1,&err);
	fwrite(&user, ACCOUNT, 1, fp);
	fclose(fp);
	return user.jurisdiction;//����Ȩ��
}


/*
���ھ޴�©����δ�޸� XD
д���޸����  ��δ���ԡ�
*/
FILE *File_access(const char* FileName,int mode,errno_t *Err)//�ļ�����
{
	FILE* fp=NULL;	
	errno_t err;
	switch (mode)
	{
		case 1://д��
		{
			err = fopen_s(&fp, FileName, "rb");//�ж��ļ��Ƿ����
			*Err = err;
			if (err == 2)//�½������ļ�
			{
				err = fopen_s(&fp, FileName, "wb");
				*Err = err;
				if (err)
				{
					gotoxy(0, 0);
					printf("�½�%s�ļ�ʧ��[%d]\n", FileName, err);
					system("pause");
					return NULL;
				}//�½������ļ�ʧ��
			}
			else if (err == 0)//�ļ�����
			{
				fclose(fp);
				err = fopen_s(&fp, FileName, "ab");
				*Err = err;
				if (err)
				{
					gotoxy(0, 0);
					printf("д��%s�ļ�ʧ��[%d]\n", FileName, err);
					system("pause");
					return NULL;
				}//д���˻��ļ�ʧ��
			}
			else if (err)//������������
			{
				gotoxy(0, 0);
				printf("�ļ����ʧ��[%d]\n", err);
				printf("���飺ɾ�� %s �ļ�\n", FileName);
				system("pause");
				return NULL;
			}
			break;
		}
		case 2://��ȡ
		{
			err = fopen_s(&fp, FileName, "rb");
			*Err = err;
			if (err == 2)
			{
				gotoxy(0, 0);
				printf("���%s�ļ�ʧ��,��鿴��Ŀ¼���Ƿ����%s�ļ�\n", FileName,FileName);
				system("pause");
				return NULL;
			}
			else if (err)//��ȡ��������
			{
				gotoxy(0, 0);
				printf("��ȡ%s�ļ�ʧ��[%d]\n", FileName, err);
				system("pause");
				return NULL;
			}
			break;
		}
		case 3://��д
		{
			err = fopen_s(&fp, FileName, "wb");
			*Err = err;
			if (err)
			{
				gotoxy(0, 0);
				printf("�½�%s�ļ�ʧ��[%d]\n", FileName, err);
				system("pause");
				return NULL;
			}//�½������ļ�ʧ��
			break;
		}
	}
		return fp;
}

XY Print_User()//��ӡ��ͨ�û�����
{
	int x = 15, y = 10;
	XY Interface; Interface.x = x; Interface.y = y;
	gotoxy(x, y);
	printf(" _______________________________________________"); y++;
	gotoxy(x, y);
	printf("|                                               |"); y++;
	gotoxy(x, y);
	printf("|             �����û�����                      |"); y++;
	gotoxy(x, y);
	printf("|             ���ס����ס��Ϣ                  |"); y++;
	gotoxy(x, y);
	printf("|             ��ѯѧ��ס����Ϣ                  |"); y++;
	gotoxy(x, y);
	printf("|             ��ѯ���д�λ                      |"); y++;
	gotoxy(x, y);
	printf("|             �˳�                              |"); y++;
	gotoxy(x, y);
	printf("|_______________________________________________|"); y++;
	return Interface;
}

void User_interface(char *user_number)//��ͨ�û�����
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
				connect_account(&head_account);//�����û�����
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
			case USER_OPTION://�˳�
			{
				quit = 0;
				break;
			}
		}
	}
}

int cursor_move(XY Interface, int max)//ѡ�����ƶ�
{
	int judge = 1;//λ���ж�
	Interface.x += 3;int y=Interface.y += 1;//λ�ó�ʼ��
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
		if (mode == ENTER) return judge;//ȷ��ѡ��
		if (judge > max) judge = 1;//�������ѡ��
		if (judge < 1) judge = max;//������Сѡ��
	}
	return 1;
}

XY Print_Admin()//��ӡ����Ա����
{
	int x = 15, y = 10;
	XY Interface; Interface.x = x; Interface.y = y;
	gotoxy(x, y);
	printf(" _______________________________________________"); y++;
	gotoxy(x, y);
	printf("|                                               |"); y++;
	gotoxy(x, y);
	printf("|             �����û��˺�                      |"); y++;
	gotoxy(x, y);
	printf("|             ɾ���û��˺�                      |"); y++;
	gotoxy(x, y);
	printf("|             �����û�����                      |"); y++;
	gotoxy(x, y);
	printf("|             ����ѧ��ס����Ϣ                  |"); y++;
	gotoxy(x, y);
	printf("|             ����ѧ��ס����Ϣ                  |"); y++;
	gotoxy(x, y);
	printf("|             ��ѯס����Ϣ                      |"); y++;
	gotoxy(x, y);
	printf("|             �˳�                              |"); y++;
	gotoxy(x, y);
	printf("|_______________________________________________|"); y++;
	return Interface;
}

void Admin_interface()//����Ա����
{
	account* head_account;
	student* head_student;
	for (int quit=1;quit;)
	{
		system("cls");
		connect_account(&head_account);//�����û�����
		XY Interface_admin=Print_Admin();
		int mode = cursor_move(Interface_admin, ADMIN_OPTION);
		system("cls");
		switch (mode)
		{
			case 1://�����˻�
			{
				Print_login(0);
				Register();
				break;
			}
			case 2://ɾ���˻�
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
			case 3://�����û�����
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
			case 4://����ѧ��ס����Ϣ
			{
				for (int i = 1; i;)
				{
					system("cls");
					XY Interface_student = print_student(0, NULL);
					i = student_registration(Interface_student);
				}
				break;
			}
			case 5://����ѧ��ס����Ϣ
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
			case 6://��ѯס����Ϣ
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
			case ADMIN_OPTION://�˳�
			{
				quit = 0;
				break;
			}
		}
	}
}

XY print_account(account* head)//��ӡͳ���û�����
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
		printf("| �˺�:%-20s   ����:%-20s    Ȩ��%d |", head->number, head->password, head->jurisdiction); y++;
	}
	gotoxy(x, y);
	printf("|________________________________________________________________|"); y++;
	Interface_account.x = x; Interface_account.y = y;
	return Interface_account;
}

int delete_accout(XY Interface_account,account *head)//ɾ���û�
{
	gotoxy(Interface_account.x, Interface_account.y);
	printf("|                                                                |"); Interface_account.y++;
	gotoxy(Interface_account.x, Interface_account.y); int y;//��λ����
	printf("| ɾ���˻�:                                                      |"); y=Interface_account.y++;
	gotoxy(Interface_account.x, Interface_account.y);
	printf("|              [ENTER]����     [ESC]�˳�                         |"); Interface_account.y++;
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
			if (head == NULL)//�û�����Ϊ��
				return 0;
			int judge=0;//�ж�
			gotoxy(Interface_account.x, y);
			printf("| ɾ���˻�:                                                      |");
			gotoxy(Interface_account.x,y+1);
			printf("|                                                                |");
			gotoxy(Interface_account.x + 11, y);
			scanf_s("%s", delete_number, MAX);
			for (;p2;)
			{
				if (p2 == NULL) return 1;
				if(!strcmp(p2->number,delete_number))
				{
					if (p2 == head)//ɾ���û���ͷ�ڵ�
					{
						head = head->next;
						judge = 1;
						break;
					}
					else if (p2->next != NULL)//ɾ���û���������λ��
					{
						p1->next = p2->next;
						judge = 1;
						break;
					}
					else if (p2->next == NULL)//ɾ���û�����β��
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
				if (err) return 0;//�ж��ļ��Ƿ񱨴�
				for (p1 = head; p1; p1 = p1->next)//�ļ�д��
				{
					fwrite(p1, ACCOUNT, 1, fp);
				}
				fclose(fp);
				gotoxy(Interface_account.x, y);
				printf("| ɾ���˻�:ɾ���ɹ�                                              |"); y++;
				gotoxy(Interface_account.x+1, y);
				system("pause");
				return 1;
			}
			else 
			{
				gotoxy(Interface_account.x, y);
				printf("| ɾ���˻�:ɾ��ʧ��                                              |"); y++;
				gotoxy(Interface_account.x+1, y);
				system("pause");
				return 1;
			}

		}
	}

}
/*
����������������ܻ����
�������Ķ�
*/
int reset_password(XY Interface_account, account* head,int jurisdiction,char *number)//�����û�����
{
	switch (jurisdiction)
	{
		case 1:
		{
			print_account(head);
			gotoxy(Interface_account.x, Interface_account.y);
			printf("|                                                                |"); Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y); int number_y;//��λ�˻���
			printf("| �����˻�:                                                      |"); number_y = Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y); int password_y;//��λ�˻���
			printf("|   ������:                                                      |"); password_y = Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y);
			printf("|              [ENTER]����     [ESC]�˳�                         |"); Interface_account.y++;
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
					if (head == NULL)//�û�����Ϊ��
						return 0;
					int judge = 0;//�ж�
					gotoxy(Interface_account.x, number_y);
					printf("| �����˻�:                                                      |");
					gotoxy(Interface_account.x, password_y);
					printf("|   ������:                                                      |");
					gotoxy(Interface_account.x, password_y+1);
					printf("|                                                                |");
					gotoxy(Interface_account.x + 11, number_y);
					scanf_s("%s", reset_number, MAX);
					for (; p1; p1 = p1->next)//Ѱ���û�
					{
						if (!strcmp(p1->number, reset_number))
							break;
					}
					if (p1 == NULL)//û�ҵ���Ҫ���õ��˻�
					{
						gotoxy(Interface_account.x, number_y);
						printf("| �����˻�:���û�������                                          |");
						gotoxy(Interface_account.x + 1, password_y);
						system("pause");
						return 1;
					}
					gotoxy(Interface_account.x+11, password_y);
					scanf_s("%s", p1->password, MAX);//д��������

					errno_t err;
					FILE* fp = File_access(FILE_ACCOUNT, 3, &err);
					if (err) return 0;//�ж��ļ��Ƿ񱨴�
					for (p1 = head; p1; p1 = p1->next)//�ļ�д��
					{
						fwrite(p1, ACCOUNT, 1, fp);
					}
					fclose(fp);
					gotoxy(Interface_account.x, password_y);
					printf("|   ������:�޸ĳɹ�                                              |");
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
			for (; p1; p1 = p1->next)//Ѱ���û�
			{
				if (!strcmp(p1->number, number))
					break;
			}if (p1 == NULL)break;
			gotoxy(Interface_account.x, Interface_account.y);
			printf(" __________________________________________"); Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y);
			printf("|                                          |"); Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y);
			printf("| ��ǰ�û�:%-20s            |",p1->number); Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y);
			printf("| ��ǰ����:%-20s            |", p1->password); Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y);
			printf("|         [ENTER]����   [ESC]�˳�          |"); Interface_account.y++;
			gotoxy(Interface_account.x, Interface_account.y);
			printf("|__________________________________________|"); Interface_account.y--;
			char t;
			do {
				t = getch();
			} while (t != ESC && t != ENTER);
			if (t == ESC)
				return 0;
			gotoxy(Interface_account.x, Interface_account.y);
			printf("| ������:                                  |");
			gotoxy(Interface_account.x+9, Interface_account.y--);
			scanf_s("%s", p1->password, MAX);
			errno_t err;
			FILE* fp = File_access(FILE_ACCOUNT, 3, &err);
			if (err) return 0;//�ж��ļ��Ƿ񱨴�
			gotoxy(Interface_account.x, Interface_account.y);
			printf("| ��ǰ����:%-20s            |", p1->password); Interface_account.y++;
			for (p1 = head; p1; p1 = p1->next)//�ļ�д��
			{
				fwrite(p1, ACCOUNT, 1, fp);
			}
			fclose(fp);
			gotoxy(Interface_account.x, Interface_account.y);
			printf("| ������:�޸ĳɹ� ");
			system("pause");
			return 1;
			break;
		}
	}
	return 0;

}

void quit(XY Interface)//�����˳���
{
	gotoxy(Interface.x, Interface.y);
	printf(" __________________________________"); Interface.y++;
	gotoxy(Interface.x, Interface.y);
	printf("|                                  |"); Interface.y++;
	gotoxy(Interface.x, Interface.y);
	printf("|           [ESC]�������          |"); Interface.y++;
	gotoxy(Interface.x, Interface.y);
	printf("|__________________________________|");
	while (getch() != ESC);
}

XY print_seek()//��ӡ���ҽ���
{
	int x = 15, y = 10;
	XY Interface;
	Interface.x = x; Interface.y = y;
	gotoxy(x, y);
	printf(" ____________________________________"); y++;
	gotoxy(x, y);
	printf("|                                    |"); y++;
	gotoxy(x, y);
	printf("|           ������Ų�ѯ             |"); y++;
	gotoxy(x, y);
	printf("|           �����ҳ�ѧ�Ų�ѯ         |"); y++;
	gotoxy(x, y);
	printf("|           �����ҳ��绰��ѯ         |"); y++;
	gotoxy(x, y);
	printf("|           ������ѧ�Ų�ѯ           |"); y++;
	gotoxy(x, y);
	printf("|           �����˵绰��ѯ           |"); y++;
	gotoxy(x, y);
	printf("|           ������������ѯ           |"); y++;
	gotoxy(x, y);
	printf("|           �˳�                     |"); y++;
	gotoxy(x, y);
	printf("|____________________________________|"); y++;
	gotoxy(x, y);
	printf("|                                    |"); y++;
	gotoxy(x, y);
	printf("|              ��ѡ��                |"); Interface.special_y = y++;
	gotoxy(x, y);
	printf("|____________________________________|"); y++;
	Interface.special_x = x;
	return Interface;
}

int dormitory_seek(student* head)//��ѯ������Ϣ
{
	int x = 15, y = 10;
	unsigned long user_long;
	XY Interface_seek;
	gotoxy(x,y);
	printf(" __________________________________");y++;
	gotoxy(x, y);
	printf("|                                  |");y++;
	gotoxy(x, y);
	printf("| �����:                          |");y++;
	gotoxy(x, y);
	printf("|      [ENTER]���� [ESC]�˳�       |");y++;
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
				printf("%d��λ[��]  ", i); j += 9;
			}
		}

	}
	quit(Interface_seek);
	print_student(5, NULL);
}

int seek_student(XY	Interface_seek, student* head)//��ѯѧ��ס����Ϣ
{
	int mode = cursor_move(Interface_seek, SEEK_STUDENT);
	unsigned long user_long;
	char user_char[MAX];
	student* p1;
	switch (mode)
	{
		case 1://������Ų���
		{
			gotoxy(Interface_seek.special_x, Interface_seek.special_y);
			printf("|       �����:                      |"); 
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
		case 2://�����ҳ�ѧ�Ų���
		{
			gotoxy(Interface_seek.special_x, Interface_seek.special_y);
			printf("|   ���ҳ�ѧ��:                      |");
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
		case 3://�����ҳ��绰����
		{
			gotoxy(Interface_seek.special_x, Interface_seek.special_y);
			printf("|   ���ҳ��绰:                      |");
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
		case 4://������ѧ�Ų���
		{
			gotoxy(Interface_seek.special_x, Interface_seek.special_y);
			printf("|   ����ѧ��:                        |");
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
		case 5://�����˵绰����
		{
			gotoxy(Interface_seek.special_x, Interface_seek.special_y);
			printf("|   ���˵绰:                        |");
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
		case 6://��������������
		{
			gotoxy(Interface_seek.special_x, Interface_seek.special_y);
			printf("|   ��������:                        |");
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
XY print_student(int mode, student* head)//��ӡѧ������
{
	int x = 15, y = 10;
	static int sx = x-5, sy = y, si = 1, location_y = y;
	XY Interface;
	switch (mode)
	{
		case 0://ע��ѧ����Ϣ
		{
			gotoxy(x, y);
			printf(" ________________________________"); y++;
			gotoxy(x, y);
			printf("|                                |"); y++;
			gotoxy(x, y);
			printf("|       ����:                    |");Interface.y = y++;
			gotoxy(x, y);
			printf("|       ѧ��:                    |"); y++;
			gotoxy(x, y);
			printf("|     �绰��:                    |"); y++;
			gotoxy(x, y);
			printf("|     �����:                    |"); y++;
			gotoxy(x, y);
			printf("|     ��λ��:                    |"); y++;
			gotoxy(x, y);
			printf("| ���ҳ�����:                    |"); y++;
			gotoxy(x, y);
			printf("| ���ҳ�ѧ��:                    |"); y++;
			gotoxy(x, y);
			printf("| ���ҳ��绰:                    |"); y++;
			gotoxy(x, y);
			printf("|________________________________|"); y++;
			gotoxy(x, y);
			printf("|                                |"); y++;
			gotoxy(x, y);
			printf("|     [ENTER]ע�� [ESC]�˳�      |"); Interface.special_y = y++;
			gotoxy(x, y);
			printf("|________________________________|"); y++;
			Interface.x = x + 13;
			Interface.special_x = x + 2;
			break;
		}
		case 1://���Դ�ӡѧ����Ϣ
		{
			gotoxy(x, y);
			printf(" _________________________________________________________________________"); y++;
			for (; head; head = head->next)
			{
				gotoxy(x, y);
				printf("|                                                                         |"); y++;
				gotoxy(x, y);
				printf("| ����:%-20s   ѧ��:%-10d   �绰:%-11s   ...... |", head->name, head->student_num, head->phone_num); y++;
			}
			gotoxy(x, y);
			printf("|_________________________________________________________________________|"); Interface.y = ++y;
			Interface.x = x;
			break;
		}
		case 2://��ϸ��ӡȫ��ѧ����Ϣ
		{
			x -= 5;
			int initial_x = x, initial_y = y;//��¼��ʼ����
			for (int i=1; head; head = head->next,i++)//iΪһ����ʾ����
			{
				y = initial_y;
				gotoxy(x, y);
				printf(" __________________________________   "); y++;
				gotoxy(x, y);
				printf("|                                  |   "); y++;
				gotoxy(x, y);
				printf("|         ����:%-20s|   ", head->name); y++;
				gotoxy(x, y);
				printf("|         ѧ��:%-20d|   ", head->student_num); y++;
				gotoxy(x, y);
				printf("|       �绰��:%-20s|   ", head->phone_num); y++;
				gotoxy(x, y);
				printf("|       �����:%-20d|   ", head->dormitory_no); y++;
				gotoxy(x, y);
				printf("|       ��λ��:%-20d|   ", head->bednum); y++;
				gotoxy(x, y);
				printf("|   ���ҳ�����:%-20s|   ", head->boss_name); y++;
				gotoxy(x, y);
				printf("|   ���ҳ�ѧ��:%-20d|   ", head->boss_num); y++;
				gotoxy(x, y);
				printf("|   ���ҳ��绰:%-20s|   ", head->bossPH_num); y++;
				gotoxy(x, y);
				printf("|__________________________________|"); y++;
				x += 36;//����
				if (i == 3)
				{
					i = 0;
					x = initial_x;
					initial_y += 13;
				}
			}
			Interface.x = x, Interface.y = y;
			if(initial_y-2!=y) Interface.y = y-4;//�ж��Ƿ���ʾ��һ��
			break;
		}
		case 3://��ϸ��ӡ����ѧ����Ϣ
		{
			Interface.x = x; Interface.y = y;
			gotoxy(x, y);
			printf(" ____________________________________   "); y++;
			gotoxy(x, y);
			printf("|                                    |   "); y++;
			gotoxy(x, y);
			printf("|           ����:%-20s|   ", head->name); y++;
			gotoxy(x, y);
			printf("|           ѧ��:%-20d|   ", head->student_num); y++;
			gotoxy(x, y);
			printf("|         �绰��:%-20s|   ", head->phone_num); y++;
			gotoxy(x, y);
			printf("|         �����:%-20d|   ", head->dormitory_no); y++;
			gotoxy(x, y);
			printf("|         ��λ��:%-20d|   ", head->bednum); y++;
			gotoxy(x, y);
			printf("|     ���ҳ�����:%-20s|   ", head->boss_name); y++;
			gotoxy(x, y);
			printf("|     ���ҳ�ѧ��:%-20d|   ", head->boss_num); y++;
			gotoxy(x, y);
			printf("|     ���ҳ��绰:%-20s|   ", head->bossPH_num); y++;
			gotoxy(x, y);
			printf("|____________________________________|"); y++;
			Interface.special_x = x; Interface.special_y = y;
			break;
		}
		case 4://��ϴ�ӡѧ����ϸ��Ϣ
		{
			x -= 5;
			sy = location_y;
			gotoxy(sx, sy);
			printf(" __________________________________   \n"); sy++;
			gotoxy(sx, sy);
			printf("|                                  |   \n"); sy++;
			gotoxy(sx, sy);
			printf("|         ����:%-20s|   \n", head->name); sy++;
			gotoxy(sx, sy);
			printf("|         ѧ��:%-20d|   \n", head->student_num); sy++;
			gotoxy(sx, sy);
			printf("|       �绰��:%-20s|   \n", head->phone_num); sy++;
			gotoxy(sx, sy);
			printf("|       �����:%-20d|   \n", head->dormitory_no); sy++;
			gotoxy(sx, sy);
			printf("|       ��λ��:%-20d|   \n", head->bednum); sy++;
			gotoxy(sx, sy);
			printf("|   ���ҳ�����:%-20s|   \n", head->boss_name); sy++;
			gotoxy(sx, sy);
			printf("|   ���ҳ�ѧ��:%-20d|   \n", head->boss_num); sy++;
			gotoxy(sx, sy);
			printf("|   ���ҳ��绰:%-20s|   \n", head->bossPH_num); sy++;
			gotoxy(sx, sy);
			printf("|__________________________________|\n"); sy++;
			sx += 36;
			if (si == 3)
			{
				si = 0;
				sx = x; location_y += 13;
			}si++;
			Interface.x = sx, Interface.y = sy;
			if (location_y - 2 != sy) Interface.y = sy - 4;//�ж��Ƿ���ʾ��һ��
			break;
		}
		case 5://��ϴ�ӡ��λ
		{
			si = 1; sx = x-5; sy = y; location_y = y;
			break;
		}

	}
	return Interface;
}

int student_registration(XY Interface_student)//ѧ��ע��
{
	while (1)
	{
		char t = getch();
		if (t == ESC)//�����˳�
			return 0;
		else if (t == ENTER)
		{
			errno_t err;
			FILE* fp = File_access(FILE_STUDENT, 1, &err);
			if (err) return 0;//������
			student user;
			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("                               ");

			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("����������      ");
			gotoxy(Interface_student.x, Interface_student.y);
			scanf_s("%s", user.name, 20);Interface_student.y++;
			scanf_s("%*[^\n]%*c");

			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("������ѧ��      ");
			gotoxy(Interface_student.x, Interface_student.y);
			scanf_s("%d", &user.student_num); Interface_student.y++;
			scanf_s("%*[^\n]%*c");

			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("������绰��     ");
			gotoxy(Interface_student.x, Interface_student.y);
			scanf_s("%s",user.phone_num,20); Interface_student.y++;
			scanf_s("%*[^\n]%*c");

			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("�����������      ");
			gotoxy(Interface_student.x, Interface_student.y);
			scanf_s("%d", &user.dormitory_no); Interface_student.y++;
			scanf_s("%*[^\n]%*c");

			do {
				gotoxy(Interface_student.special_x, Interface_student.special_y);
				printf("�����봲λ��[1-8]");
				gotoxy(Interface_student.x, Interface_student.y);
				scanf_s("%d", &user.bednum);
				scanf_s("%*[^\n]%*c");
				if (user.bednum <= 0 || user.bednum > 8)//��λ�������쳣
				{
					gotoxy(Interface_student.x, Interface_student.y);
					printf("                    ");
					scanf_s("%*[^\n]%*c");
				}
			} while (user.bednum <=0|| user.bednum >8); Interface_student.y++;

			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("���������ҳ�����      ");
			gotoxy(Interface_student.x, Interface_student.y);
			scanf_s("%s", user.boss_name,20); Interface_student.y++;
			scanf_s("%*[^\n]%*c");

			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("���������ҳ�ѧ��      ");
			gotoxy(Interface_student.x, Interface_student.y);
			scanf_s("%d",&user.boss_num); Interface_student.y++;
			scanf_s("%*[^\n]%*c");

			gotoxy(Interface_student.special_x, Interface_student.special_y);
			printf("���������ҳ��绰      ");
			gotoxy(Interface_student.x, Interface_student.y);
			scanf_s("%s", user.bossPH_num,20); Interface_student.y++;
			scanf_s("%*[^\n]%*c");

			fwrite(&user, STUDENT, 1, fp);
			fclose(fp);
			gotoxy(Interface_student.special_x-1, Interface_student.special_y);
			printf("ѧ��ע��ɹ�");
			system("pause");
			return 1;
		}
	}
}

int student_Menu(XY Interface_student, student* head)//ѧ����Ϣ-�˵�
{
	int x = Interface_student.x, y = Interface_student.y;
	gotoxy(x,y);
	printf("|                                                                         |"); y++;
	gotoxy(x, y);
	printf("|             [1]��ʾ����   [2]����ѧ����Ϣ   [3]ɾ��ѧ����Ϣ             |"); y++;
	gotoxy(x, y);
	printf("|                           [ESC]�˳�                                     |"); y++;
	gotoxy(x, y);
	printf("|_________________________________________________________________________|");
	char mode;
	do {
		mode = getch();
	} while (mode != '1' && mode != '2' && mode != '3' && mode != ESC);
	switch (mode)
	{
		case '1'://��ʾ��ϸѧ����Ϣ
		{
			system("cls");
			Interface_student=print_student(2, head);
			gotoxy(Interface_student.x, Interface_student.y);
			printf(" __________________________________"); Interface_student.y++;
			gotoxy(Interface_student.x, Interface_student.y);
			printf("|                                  |"); Interface_student.y++;
			gotoxy(Interface_student.x, Interface_student.y);
			printf("|           [ESC]�������          |"); Interface_student.y++;
			gotoxy(Interface_student.x, Interface_student.y);
			printf("|__________________________________|");
			while (getch() != ESC);
			break;
		}
		case '2':
		{
			student* user=student_Seek(Interface_student,head);
			system("cls");
			if (!user)//����ʧ��
				break;
			student_Change(Interface_student, user);
			student_file(head);
			break;
		}
		case '3':
		{
			student* user = student_Seek(Interface_student, head);
			system("cls");
			if (!user)//����ʧ��
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

void student_Change(XY Interface_student, student* user)//ѧ����Ϣ-����
{
	Interface_student = print_student(3, user);
	while (1)
	{
		gotoxy(Interface_student.special_x, Interface_student.special_y);
		printf("|                                    |"); Interface_student.special_y++;
		gotoxy(Interface_student.special_x, Interface_student.special_y);
		printf("|          ��ѡ���������Ϣ          |"); Interface_student.special_y++;
		gotoxy(Interface_student.special_x, Interface_student.special_y);
		printf("|____________________________________|"); Interface_student.special_y--;
		int mode = cursor_move(Interface_student, STUDENT_OPTION);
		switch (mode)
		{
			case 1:
			{
				gotoxy(Interface_student.special_x, Interface_student.special_y);
				printf("|       ������:                      |");
				gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
				scanf_s("%s", user->name, 20);
				break;
			}
			case 2:
			{
				gotoxy(Interface_student.special_x, Interface_student.special_y);
				printf("|       ��ѧ��:                      |");
				gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
				scanf_s("%d", &(user->student_num));
				break;
			}
			case 3:
			{
				gotoxy(Interface_student.special_x, Interface_student.special_y);
				printf("|       �µ绰:                      |");
				gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
				scanf_s("%s", user->phone_num, 20);
				break;
			}
			case 4:
			{
				gotoxy(Interface_student.special_x, Interface_student.special_y);
				printf("|       ������:                      |");
				gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
				scanf_s("%d", &(user->dormitory_no));
				break;
			}
			case 5:
			{
				do {
					gotoxy(Interface_student.special_x, Interface_student.special_y);
					printf("|      �´�λ:                       |");
					gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
					scanf_s("%d", &(user->bednum));
					if (user->bednum <= 0 || user->bednum > 8)//��λ�������쳣
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
				printf("| �����ҳ�����:                      |");
				gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
				scanf_s("%s", user->boss_name, 20);
				break;
			}
			case 7:
			{
				gotoxy(Interface_student.special_x, Interface_student.special_y);
				printf("| �����ҳ�ѧ��:                      |");
				gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
				scanf_s("%d", &(user->boss_num));
				break;
			}
			case 8:
			{
				gotoxy(Interface_student.special_x, Interface_student.special_y);
				printf("| �����ҳ��绰:                      |");
				gotoxy(Interface_student.special_x + 16, Interface_student.special_y);
				scanf_s("%s", user->bossPH_num,20);
				break;
			}
		}
		print_student(3, user);
		gotoxy(Interface_student.special_x, Interface_student.special_y);
		printf("|     [ENTER]��������  [ESC]�˳�     |"); Interface_student.special_y--;
		char t;
		do {
			t = getch();
		} while (t != ENTER && t != ESC);
		if (t == ESC) break;
	}
}

void student_Delete(student* user, student* head)//ѧ����Ϣ-����
{
	if (head)
	{
		XY Interface_student=print_student(3, user);
		gotoxy(Interface_student.special_x, Interface_student.special_y);
		printf("|                                    |"); Interface_student.special_y++;
		gotoxy(Interface_student.special_x, Interface_student.special_y);
		printf("|             ȷ��ɾ����             |"); Interface_student.special_y++;
		gotoxy(Interface_student.special_x, Interface_student.special_y);
		printf("|       [ENTER]ȷ��  [ESC]ȡ��       |"); Interface_student.special_y++;
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

student* student_Seek(XY Interface_student, student* head)//ѧ����Ϣ-Ѱ��
{
	gotoxy(Interface_student.x, Interface_student.y);
	printf("|                                                                         |"); Interface_student.y++;
	gotoxy(Interface_student.x, Interface_student.y);
	printf("| ѧ��ѧ��:                                                               |"); Interface_student.y++;
	gotoxy(Interface_student.x, Interface_student.y);
	printf("|                      [ENTER]����     [ESC]�˳�                          |"); Interface_student.y++;
	gotoxy(Interface_student.x, Interface_student.y);
	printf("|_________________________________________________________________________|");
	char t;
	do {
		t = getch();
	} while (t != ENTER && t != ESC);
	if (t == ESC)
		return NULL;//�˳�
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
		printf("��ѧ�Ų�����");
		system("pause");
		return NULL;
	}
	return NULL;
}

int student_file(student* head)//ѧ����Ϣ-�ļ�
{
	errno_t err;
	FILE *fp=File_access(FILE_STUDENT, 3, &err);
	if (err) return 0;//����
	for(;head;head=head->next)
	fwrite(head, STUDENT, 1, fp);
	fclose(fp);
	return 1;
}

/*
������������������дһ��
�κμ�������ȫ��bug
�������֮��ֻ�ֿܷ�
*/
int connect_account(account** head)//�����˻�����
{
	FILE* fp; errno_t err;//�ļ���ȡ����
	account* p1, * p2;
	*head = p1 =p2= (account*)malloc(ACCOUNT);
	p1->next = NULL;
	fp = File_access(FILE_ACCOUNT, 2, &err);
	if (err) return 0;//������
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

int connect_student(student** head)//����ѧ������
{
	FILE* fp; errno_t err;//�ļ���ȡ����
	student* p1, * p2;
	*head = p1 = p2 = (student*)malloc(STUDENT);
	p1->next = NULL;
	fp = File_access(FILE_STUDENT, 2, &err);
	if (err) return 0;//������
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

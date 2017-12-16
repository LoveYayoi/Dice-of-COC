/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/
/*
* Dice of COC(VC++) 
* Written by w4123
* Version 1.1.1
* Commercial use is prohibited.
*/
#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����

using namespace std;

int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
static string FILENAME;

/* 
* ����Ӧ�õ�ApiVer��Appid������󽫲������
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}
/* 
* ���� Ӧ��AuthCode����Q��ȡӦ����Ϣ��������ܸ�Ӧ�ã���������������������AuthCode��
* ��Ҫ�ڱ��������������κδ��룬���ⷢ���쳣���������ִ�г�ʼ����������Startup�¼���ִ�У�Type=1001����
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}

CQEVENT(int32_t, __eventEnable, 0)() {
	/*FILENAME = CQ_getAppDirectory(ac);
	FILENAME += "config.ini";
	fstream i(FILENAME.c_str());
	if (!i) {
		i.open(FILENAME.c_str(), ios::in);
		i<<"[��������]\nDiceCount="
	}*/

	return 0;
}

//Change the number to a string
string num2str(long long i)
{
	stringstream ss;
	ss << i;
	return ss.str();
}

int RollDice(string Dice,string &strAns) {
	if (Dice.find("d") != Dice.npos ) {//Find "d"
		int dPosition = Dice.find("d");
		int DiceCnt = 0;//Count of dice
		int DiceType=0;//The biggest number of the dice
		if (dPosition == 0) {
			DiceCnt = 1;//The 1 before d/D can be omitted
		}
		else {
			int tmp = 0;
			while (tmp!=dPosition) {
				DiceCnt = DiceCnt * 10 + (Dice[tmp] - 48);
				tmp++;//Get the count of the dice
			}
		}
		int tmp = dPosition + 1;
		while (tmp != Dice.length()) {
			DiceType = DiceType * 10 + (Dice[tmp] - 48);
			tmp++;//Get the biggest number of the dice
		}
		srand((unsigned)rand()+(unsigned)time(NULL));//Generate a new key
		int sum = 0;
		for (int i = 1; i <= DiceCnt; i++) {
			int tmpRes = rand() % (DiceType - 1) + 1;//Get the random number and add it to the output string
			strAns += num2str(tmpRes);
			sum += tmpRes;
			strAns += '+';
		}
		return sum;
	}
	else if(Dice.find("D") != Dice.npos) {
			int dPosition = Dice.find("D");
			int DiceCnt = 0;
			int DiceType = 0;
			if (dPosition == 0) {
				DiceCnt = 1;
			}
			else {
				int tmp = 0;
				while (tmp != dPosition) {
					DiceCnt = DiceCnt * 10 + (Dice[tmp] - 48);
					tmp++;
				}
			}
			int tmp = dPosition + 1;
			while (tmp != Dice.length()) {
				DiceType = DiceType * 10 + (Dice[tmp] - 48);
				tmp++;
			}
			srand((unsigned)rand() + (unsigned)time(NULL));
			int sum = 0;
			for (int i = 1; i <= DiceCnt; i++) {
				int tmpRes = rand() % (DiceType - 1) + 1;
				strAns += num2str(tmpRes);
				sum += tmpRes;
				strAns += '+';
			}
			return sum;
		
	}
	else {
		//Add a number
		strAns += Dice;
		strAns += '+';
		int intTmpAns = 0;
		for (int Cnt = 0; Cnt !=Dice.length();Cnt++) {
			intTmpAns = intTmpAns * 10 + (Dice[Cnt] - 48);
		}//Convert the string to an integer
		return intTmpAns;
	}
}
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
	if (msg[0] == '.'&&msg[1] == 'r'&&msg[2] == 'd') {
		string strSecondInput;
		int intCustomMsg = 3;
		while (msg[intCustomMsg] == ' ')intCustomMsg++;
		while (msg[intCustomMsg] != ' '&&msg[intCustomMsg] != '\0') {
			strSecondInput += msg[intCustomMsg];
			intCustomMsg++;
		}
		if (strSecondInput == "��������") {
			string strMAns = num2str(fromQQ);
			string strNoUse;
			strMAns += "����������: ";
			strMAns += '\n';
			strMAns += "����STR=3D6*5=";
			strMAns += num2str(RollDice("3D6",strNoUse) * 5);
			strMAns += '\n';
			strMAns += "����CON=3D6*5=";
			strMAns += num2str(RollDice("3D6", strNoUse) * 5);
			strMAns += '\n';
			strMAns += "����SIZ=3D6*5=";
			strMAns += num2str(RollDice("3D6", strNoUse) * 5);
			strMAns += '\n';
			strMAns += "����DEX=3D6*5=";
			strMAns += num2str(RollDice("3D6", strNoUse) * 5);
			strMAns += '\n';
			strMAns += "��òAPP=3D6*5=";
			strMAns += num2str(RollDice("3D6", strNoUse) * 5);
			strMAns += '\n';
			strMAns += "����INT=(2D6+6)*5=";
			strMAns += num2str((RollDice("2D6", strNoUse) + 6) * 5);
			strMAns += '\n';
			strMAns += "��־POW=(2D6+6)*5=";
			strMAns += num2str((RollDice("2D6", strNoUse) + 6) * 5);
			strMAns += '\n';
			strMAns += "����EDU=(2D6+6)*5=";
			strMAns += num2str((RollDice("2D6", strNoUse) + 6) * 5);
			strMAns += '\n';
			strMAns += "����LUCK=3D6*5=";
			strMAns += num2str(RollDice("3D6", strNoUse) * 5);
			CQ_sendGroupMsg(ac,fromGroup,strMAns.c_str());
		}
		else if (strSecondInput == "������������") {
			string strMAns = num2str(fromQQ);
			string strNoUse;
			strMAns += "����������: ";
			strMAns += '\n';
			strMAns += "����STR=3D6=";
			strMAns += num2str(RollDice("3D6", strNoUse));
			strMAns += '\n';
			strMAns += "����CON=3D6=";
			strMAns += num2str(RollDice("3D6", strNoUse));
			strMAns += '\n';
			strMAns += "����SIZ=3D6=";
			strMAns += num2str(RollDice("3D6", strNoUse));
			strMAns += '\n';
			strMAns += "����DEX=3D6=";
			strMAns += num2str(RollDice("3D6", strNoUse));
			strMAns += '\n';
			strMAns += "��òAPP=3D6=";
			strMAns += num2str(RollDice("3D6", strNoUse));
			strMAns += '\n';
			strMAns += "����INT=2D6+6=";
			strMAns += num2str(RollDice("2D6", strNoUse) + 6);
			strMAns += '\n';
			strMAns += "��־POW=2D6+6=";
			strMAns += num2str(RollDice("2D6", strNoUse) + 6);
			strMAns += '\n';
			strMAns += "����EDU=3D6+3=";
			strMAns += num2str(RollDice("3D6", strNoUse) + 3);
			CQ_sendGroupMsg(ac, fromGroup, strMAns.c_str());
		}
		else {
			int intReadMsgCnt = 3;
			int intTmp;
			string strAns;//Output string
			strAns += num2str(fromQQ);//Add the QQ number at the front(Next version:Nickname)
			strAns += "������";
			strAns += ": ";

			while (msg[intReadMsgCnt] == ' ')intReadMsgCnt++;//Skip the space
			for (intTmp = intReadMsgCnt; msg[intTmp] != ' '&&msg[intTmp] != '\0'; intTmp++) {
				strAns += msg[intTmp];
			}
			strAns += '=';
			string strSeparateDice;
			int mainsum = 0;//Result
			string secondaryRes;
			for (; msg[intReadMsgCnt] != ' '&&msg[intReadMsgCnt] != '\0'; intReadMsgCnt++) {
				if (msg[intReadMsgCnt] == '+') {
					int ttmpRes = RollDice(strSeparateDice, strAns);
					mainsum += ttmpRes;
					secondaryRes += num2str(ttmpRes);
					secondaryRes += '+';
					strSeparateDice = "";
					//Reset
				}
				else {
					strSeparateDice += msg[intReadMsgCnt];
				}
			}
			int ttmpRes = RollDice(strSeparateDice, strAns);
			mainsum += ttmpRes;
			secondaryRes += num2str(ttmpRes);
			//Call the function of rolling dice again because of the last die
			while (msg[intReadMsgCnt] == ' ')intReadMsgCnt++;
			string strReason;//Get the reason from the third part of the input
			while (msg[intReadMsgCnt] != '\0') {
				strReason += msg[intReadMsgCnt];
				intReadMsgCnt++;
			}
			if (strReason != "") {
				strAns = strReason + strAns;
				strAns = "����" + strAns;//Add the reason at the front of the output
			}
			strAns[strAns.length() - 1] = '=';//There will be an unnecessary "+" in the end.Use "=" to replace it.
			strAns += secondaryRes;
			strAns += '=';
			strAns += num2str(mainsum);//Merge all the results to the string "strAns"
			CQ_sendGroupMsg(ac, fromGroup, strAns.c_str());//Change it to a C type string and output
		}
		return EVENT_BLOCK;
	}
	return EVENT_IGNORE;
}

CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, int32_t font) {
	if (msg[0] == '.'&&msg[1] == 'r'&&msg[2]=='d') {
		string strSecondInput;
		int intCustomMsg = 3;
		while (msg[intCustomMsg] == ' ')intCustomMsg++;
		while (msg[intCustomMsg] != ' '&&msg[intCustomMsg] != '\0') {
			strSecondInput += msg[intCustomMsg];
			intCustomMsg++;
		}
		if (strSecondInput == "��������") {
			string strMAns = num2str(fromQQ);
			string strNoUse;
			strMAns += "����������: ";
			strMAns += '\n';
			strMAns += "����STR=3D6*5=";
			strMAns += num2str(RollDice("3D6", strNoUse) * 5);
			strMAns += '\n';
			strMAns += "����CON=3D6*5=";
			strMAns += num2str(RollDice("3D6", strNoUse) * 5);
			strMAns += '\n';
			strMAns += "����SIZ=3D6*5=";
			strMAns += num2str(RollDice("3D6", strNoUse) * 5);
			strMAns += '\n';
			strMAns += "����DEX=3D6*5=";
			strMAns += num2str(RollDice("3D6", strNoUse) * 5);
			strMAns += '\n';
			strMAns += "��òAPP=3D6*5=";
			strMAns += num2str(RollDice("3D6", strNoUse) * 5);
			strMAns += '\n';
			strMAns += "����INT=(2D6+6)*5=";
			strMAns += num2str((RollDice("2D6", strNoUse) + 6) * 5);
			strMAns += '\n';
			strMAns += "��־POW=(2D6+6)*5=";
			strMAns += num2str((RollDice("2D6", strNoUse) + 6) * 5);
			strMAns += '\n';
			strMAns += "����EDU=(2D6+6)*5=";
			strMAns += num2str((RollDice("2D6", strNoUse) + 6) * 5);
			strMAns += '\n';
			strMAns += "����LUCK=3D6*5=";
			strMAns += num2str(RollDice("3D6", strNoUse) * 5);
			CQ_sendPrivateMsg(ac, fromQQ, strMAns.c_str());
		}
		else if (strSecondInput == "������������") {
			string strMAns = num2str(fromQQ);
			string strNoUse;
			strMAns += "����������: ";
			strMAns += '\n';
			strMAns += "����STR=3D6=";
			strMAns += num2str(RollDice("3D6", strNoUse));
			strMAns += '\n';
			strMAns += "����CON=3D6=";
			strMAns += num2str(RollDice("3D6", strNoUse));
			strMAns += '\n';
			strMAns += "����SIZ=3D6=";
			strMAns += num2str(RollDice("3D6", strNoUse));
			strMAns += '\n';
			strMAns += "����DEX=3D6=";
			strMAns += num2str(RollDice("3D6", strNoUse));
			strMAns += '\n';
			strMAns += "��òAPP=3D6=";
			strMAns += num2str(RollDice("3D6", strNoUse));
			strMAns += '\n';
			strMAns += "����INT=2D6+6=";
			strMAns += num2str(RollDice("2D6", strNoUse) + 6);
			strMAns += '\n';
			strMAns += "��־POW=2D6+6=";
			strMAns += num2str(RollDice("2D6", strNoUse) + 6);
			strMAns += '\n';
			strMAns += "����EDU=3D6+3=";
			strMAns += num2str(RollDice("3D6", strNoUse) + 3);
			CQ_sendPrivateMsg(ac, fromQQ, strMAns.c_str());
		}
		else {
			int intReadMsgCnt = 3;
			int intTmp;
			string strAns;//Output string
			strAns += num2str(fromQQ);//Add the QQ number at the front(Next version:Nickname)
			strAns += "������";
			strAns += ": ";

			while (msg[intReadMsgCnt] == ' ')intReadMsgCnt++;//Skip the space
			for (intTmp = intReadMsgCnt; msg[intTmp] != ' '&&msg[intTmp] != '\0'; intTmp++) {
				strAns += msg[intTmp];//Copy the Dice in the input to the output string
			}
			strAns += '=';
			string strSeparateDice;//Separate each die
			int mainsum = 0;//The result number
			string secondaryRes;//The direct result of each part of the input
			for (; msg[intReadMsgCnt] != ' '&&msg[intReadMsgCnt] != '\0'; intReadMsgCnt++) {
				if (msg[intReadMsgCnt] == '+') {
					int ttmpRes = RollDice(strSeparateDice, strAns);
					//Call the function of rolling dice
					mainsum += ttmpRes;
					secondaryRes += num2str(ttmpRes);
					secondaryRes += '+';
					strSeparateDice = "";
					//Reset
				}
				else {
					strSeparateDice += msg[intReadMsgCnt];
				}
			}
			int ttmpRes = RollDice(strSeparateDice, strAns);
			mainsum += ttmpRes;
			secondaryRes += num2str(ttmpRes);
			//Call the function of rolling dice again because of the last die
			while (msg[intReadMsgCnt] == ' ')intReadMsgCnt++;
			string strReason;//Get the reason from the third part of the input
			while (msg[intReadMsgCnt] != '\0') {
				strReason += msg[intReadMsgCnt];
				intReadMsgCnt++;
			}
			if (strReason != "") {
				strAns = strReason + strAns;
				strAns = "����" + strAns;//Add the reason at the front of the output
			}
			strAns[strAns.length() - 1] = '=';//There will be an unnecessary "+" in the end.Use "=" to replace it.
			strAns += secondaryRes;
			strAns += '=';
			strAns += num2str(mainsum);//Merge all the results to the string "strAns"
			CQ_sendPrivateMsg(ac, fromQQ, strAns.c_str());//Change it to a C type string and output
		}
		return EVENT_BLOCK;
	}
	return EVENT_IGNORE;
}

CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(nullptr, "ʹ�÷���:\n\t��ͨ����: .rd �������� ԭ��(��ѡ)\n\r�����������������߰��������ɣ�����Ϊ.rd �����������ɺ�.rd �߰���������", "Help", MB_OK+MB_ICONINFORMATION);
	return 0;
}
/*\n\t��Ӧ�ó���Ŀ¼�µ�config.ini�п��������Զ�������,����Ϊ.rd �Զ�����������\n\t��config.ini��Ĭ��*/

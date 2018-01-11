#include <windows.h>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include "CQSDK/CQAPI_EX.h"
#include "CQSDK/CQEVE_ALL.h"
#include "APPINFO.h"
#include "RollDice.h"

using namespace std;
using namespace CQ;

map<pair<long long, long long>, string> GroupName;
map<pair<long long, long long>, string> DiscussName;
set<long long> DisabledGroup;
set<long long> DisabledDiscuss;

static string strFileLoc;
static string strTooManyTimesNotice = "����ʧ��!���������������������10��!";
//Help Message
static string strHlpMsg = 
R"(�����г��˱������˵�ȫ������(ʹ��".help ����"��ȡ���������ϸ��Ϣ):
	.o/r/d/rd ��������/���� [ԭ��]   ��ͨ����
	.h/rh ��������/���� [ԭ��]     ����
	.sn [����]                         ����/ɾ������
	.dn		                        ɾ������
	.on [QQ��]                       ��������
	.off [QQ��]                       �ر�����
	.st [QQ��]                         �鿴���ӿ���״̬
	.me Ⱥ��(��˽��) ����	   �Ե������ӽ���������
	.sc ����Sanֵ ��ǰSanֵ     �Զ�Sancheck,����Sanֵ�ĸ�ʽ��"Sancheck�ɹ��۵ĵ���/Sancheckʧ�ܿ۵ĵ���"
	.help [����]	                  ��ȡ����
�����е�����:
	.o/r/d/rd/h/rh COC6       COC6����������
	.o/r/d/rd/h/rh COC7       COC7����������
	.o/r/d/rd/h/rh DND        DNDӢ������
	.o/r/d/rd/h/rh Bx         D100������(xΪ����������,��.o B2,B��x֮���޿ո�)
	.o/r/d/rd/h/rh Px         D100�ͷ���(xΪ�ͷ�������,��.o B2,P��x֮���޿ո�)
ע:ǿ�ҽ�����������ʱ��Ҫʡ�Կո�,��Ȼ�����������ĳ�̶ֳ���ʶ�𲻼ӿո������,��Ҳ��һ�����ʻᷢ������
�汾:1.5.2
���ⷴ��/����Ⱥ624807593)";

static string strNormalDiceMsg = R"(��ͨ����:
	.o/r/rd �������� [ԭ��]
���Ǳ�������������Ĺ���,�����������п���ʶ�����Ӽ�����,����֧��ʹ��#���ж�������,ʹ�÷���Ϊ����#��������
��:.o 1d3#1d6-2d2+3��������1d3�ε�1d6-2d2+3)";

static string strHiddenDiceMsg = R"(����:
	.h/rh �������� [ԭ��]
����ֻ��Ⱥ/������������Ч,�����˽�ĸ�ָ������,����Ⱥ/���������з�������֪ͨ,����֧�ֵ�������������ͨ������ͬ)";

static string strSNMsg = R"(����/ɾ������:
	.sn [����]
������ֻ��Ⱥ/������������Ч,ͨ������������Ըı�����˶���ĳƺ�,��ʹ�ô��������ù�������Ὣ�串��,�������Ϊ������dn������ͬΪɾ������,�ƺ����ȼ�Ϊsn���õĳƺ�>Ⱥ��Ƭ>�ǳ�)";

static string strDNMsg = R"(ɾ������:
	.dn
������ֻ��Ⱥ/������������Ч,ͨ�������������ɾ��sn�������õĳƺ�,�ָ����������õĳƺ�,�ƺ����ȼ�Ϊsn���õĳƺ�>Ⱥ��Ƭ>�ǳ�)";

static string strONMsg = R"(��������:
	.on [QQ��]
��������Ϊ����Ⱥ���ж�������˵������׼����,���Դ�����ֻ��Ⱥ/������������Ч,����,��Ⱥ�б�������Ҫ����Ա/Ⱥ��Ȩ�ޡ�����QQ����ֻ������ӦQQ�ŵĻ�����,������������л�������Ч)";

static string strOFFMsg = R"(�ر�����:
	.on [QQ��]
��������Ϊ����Ⱥ���ж�������˵������׼����,���Դ�����ֻ��Ⱥ/������������Ч,����,��Ⱥ�б�������Ҫ����Ա/Ⱥ��Ȩ�ޡ�����QQ����ֻ�رն�ӦQQ�ŵĻ�����,������������л�������Ч)";

static string strSTMsg = R"(�鿴���ӿ���״̬:
	.st [QQ��]
��������Ϊ����Ⱥ���ж�������˵������׼����,���Դ�����ֻ��Ⱥ/������������Ч������QQ����ֻ�鿴��ӦQQ�ŵĻ�����״̬,������������л�������Ч)";

static string strMEMsg = R"(�Ե������ӽ���������:
	Ⱥ��(��˽��) ����
��Ⱥ/����������ʹ�õ�ʱ������Ϊ.me ����,�����˻��Ե��������ӽ��ظ�������һ������������Ϸ�����,Ϊ��ֹ���������ƻ���Ϸ��,Ҳ����ʹ��˽������.me Ⱥ�� ����,�����˻�ֱ�ӽ��˶������͵�ָ����Ⱥ��(˽��me���֧�ֶ�������))";

static string strSCMsg = R"(Sancheck:
	.sc ����Sanֵ ��ǰSanֵ
������Ϊ�Զ�Sancheck����,��������Sanֵ�ĸ�ʽ��"Sancheck�ɹ��۵ĵ���/Sancheckʧ�ܿ۵ĵ���",��:.sc 1/1d6 60
���������Ͷ��1d100��San�Ƚ�,����ɹ�Sanֵ��۳�"/"ǰ��ĵ���,����۳�"/"����ĵ���,������������Լ���ǰSanֵ���")";

static string strHMsg = R"(ʲô?һ�������㻹Ҫ����?)";

//Error Message
static string strErrMsg = "�������!����.help��.��������ð���!";

long long Str2Int(const string a) {
	stringstream ConvertStream;
	long long b;
	ConvertStream << a;
	ConvertStream >> b;
	return b;
}

EVE_Startup(startUp) {
	strFileLoc = getAppDirectory();
	ifstream ifstreamGroupName(strFileLoc + "GroupName.config");
	if (ifstreamGroupName) {
		long long QQ;
		long long Group;
		string name;
		pair<long long, long long> pairQQGroup;
		while (ifstreamGroupName >> QQ) {
			ifstreamGroupName >> Group >> name;
			pairQQGroup.first = QQ;
			pairQQGroup.second = Group;
			GroupName[pairQQGroup] = name;
		}
	}
	ifstreamGroupName.close();
	ifstream ifstreamDiscussName(strFileLoc + "DiscussName.config");
	if (ifstreamDiscussName) {
		long long QQ;
		long long Discuss;
		string name;
		pair<long long, long long> pairQQDiscuss;
		while (ifstreamDiscussName >> QQ) {
			ifstreamDiscussName >> Discuss >> name;
			pairQQDiscuss.first = QQ;
			pairQQDiscuss.second = Discuss;
			DiscussName[pairQQDiscuss] = name;
		}
	}
	ifstreamDiscussName.close();
	ifstream ifstreamDisabledGroup(strFileLoc + "DisabledGroup.config");
	if (ifstreamDisabledGroup) {
		long long Group;
		while (ifstreamDisabledGroup >> Group) {
			DisabledGroup.insert(Group);
		}
	}
	ifstreamDisabledGroup.close();
	ifstream ifstreamDisabledDiscuss(strFileLoc + "DisabledDiscuss.config");
	if (ifstreamDisabledDiscuss) {
		long long Discuss;
		while (ifstreamDisabledDiscuss >> Discuss) {
			DisabledDiscuss.insert(Discuss);
		}
	}
	ifstreamDisabledDiscuss.close();
	return 0;
}

///<summary>
///The function is used for receiving and dealing with group message
///Notice:groupMsg is the NAME of the Function
///</summary>
///<param name="eve">Main class of the message</param>
EVE_GroupMsg_EX(groupMsg) {

	pair<long long, long long> pairQQGroup;
	pairQQGroup.first = eve.fromQQ;
	pairQQGroup.second = eve.fromGroup;
	
	//Skip the space
	while (eve.message[0] == ' ')eve.message.erase(eve.message.begin());
	while (eve.message[eve.message.length()-1] == ' ')eve.message.erase(eve.message.end()-1);
	if (eve.message.find("��") == 0) {
		eve.message.erase(eve.message.begin());
		eve.message[0] = '.';
	}
	
	//Save the first input
	string strFirstInput = eve.message.substr(0, eve.message.find(" "));

	//Use the string to save the second part of the Input
	string strSecondInput;

	//Replace the "��" in the front to "."
	if (strFirstInput[0] != '.')return;

	if (eve.message == "." || strFirstInput[1] == '.' || strFirstInput.find("��") == 1)return;
	for (int k = 0; k != strFirstInput.length(); k++) {
		if (isdigit(strFirstInput[k])) {
			strSecondInput += strFirstInput.substr(k);
			strFirstInput = strFirstInput.substr(0, k);
			break;
		}
		if ((strFirstInput[k] == 'd'||strFirstInput[k] == 'D')&&(isdigit(strFirstInput[k+1]))) {
			strSecondInput += strFirstInput.substr(k);
			strFirstInput = strFirstInput.substr(0, k);
			break;
		}
		if (!isprint(strFirstInput[k])) {
			strSecondInput += strFirstInput.substr(k);
			strFirstInput = strFirstInput.substr(0, k);
			break;
		}
	}
	if (strFirstInput == ".")strFirstInput = ".rd";
	//Save the GroupNick. if the GroupNick is empty, save the Nickname
	string strNickName = GroupName.count(pairQQGroup) ? GroupName[pairQQGroup] : getGroupMemberInfo(eve.fromGroup, eve.fromQQ).GroupNickname.empty() ? getStrangerInfo(eve.fromQQ).nick : getGroupMemberInfo(eve.fromGroup, eve.fromQQ).GroupNickname;
	
	//Tolower the first input
	for (auto &i : strFirstInput)if (isupper(i))i = tolower(i);
	
	//Judge if it is a command
	if ((strFirstInput==".o" || strFirstInput==".h" || strFirstInput == ".rd"|| strFirstInput==".r"||strFirstInput==".rh" || strFirstInput == ".d")&&!DisabledGroup.count(eve.fromGroup)) {
		
		//Save the group name
		string strGroupName = getGroupList()[eve.fromGroup];

		//Use it to save the HiddenDice Notice
		string strHiddenNotice = strNickName + "������һ�ΰ���";

		//Use it to save the count of the Msg have read
		int intMsgCnt = eve.message.find(" ") == string::npos ? eve.message.length() : eve.message.find(" ");

		//Skip the space
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		
		//Save the Reason
		string strReason;
		
		//Get the Second Input
		strSecondInput += eve.message.substr(intMsgCnt, eve.message.find(" ", intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(" ", intMsgCnt) - intMsgCnt);
		string strSecondInputCopy = strSecondInput;
		//Toupper the second input
		for (auto &i : strSecondInput)if (islower(i))i = toupper(i);

		//Make the intMsgCnt jump to the space after the second input
		intMsgCnt = eve.message.find(" ", intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(" ", intMsgCnt);

		//Check if it is a "hidden dice"
		bool isHidden = false;
		if (strFirstInput==".h"||strFirstInput == ".rh")isHidden = true;
		
		//Save the number or dice before # to get the time of rolling dice
		int intRollDiceTime = 0;
		if (strSecondInput.find("#") != string::npos) {
			if (MainRoll(intRollDiceTime, strSecondInput.substr(0, strSecondInput.find("#"))) == -1 || intRollDiceTime <= 0) {
				sendGroupMsg(eve.fromGroup, strErrMsg);
				eve.message_block();
				return;
			}
		}
		else {

			//"#" not found, so only roll for one time
			intRollDiceTime = 1;
		}
			
		//If the input contains # and the input before # is dice, send a roll-dice-time notice
		if (strSecondInput.find("#") != string::npos && (strSecondInput.substr(0, strSecondInput.find("#")).find("d") != string::npos || strSecondInput.substr(0, strSecondInput.find("#")).find("D") != string::npos)) {
			string strRollDiceTurnNotice;
			strRollDiceTurnNotice += "��������: " + strSecondInput.substr(0, strSecondInput.find("#")) + "=" + to_string(intRollDiceTime) + "��";
			if (isHidden)sendPrivateMsg(eve.fromQQ, strRollDiceTurnNotice);else
			sendGroupMsg(eve.fromGroup, strRollDiceTurnNotice);
		}
		if (intRollDiceTime > 10) {
			sendGroupMsg(eve.fromGroup, strTooManyTimesNotice);
			eve.message_block();
			return;
		}

		//Save the count of erasing characters
		int erase_cnt = strSecondInput.find("#") == string::npos ? -1 : strSecondInput.find("#");

		//Erase the input # and the char before it
		for (int i = 0; i <= erase_cnt; ++i) strSecondInput.erase(strSecondInput.begin());
		for (int i = 0; i <= erase_cnt; ++i) strSecondInputCopy.erase(strSecondInputCopy.begin());
		//Roll the dice
		for (int w = 0; w != intRollDiceTime; ++w) {
			if (strSecondInput == "��������"|| strSecondInput == "��������"|| strSecondInput == "��������"|| strSecondInput == "�߰���������" || strSecondInput == "�߰���������" || strSecondInput == "�߰���������" || strSecondInput == "COC" || strSecondInput == "COC7") {
				//COC7

				//Save the result of COC7
				string strMAns = strNickName;

				//Call the COC7 roll dice function
				COC7(strMAns);

				//If is hidden, send a private msg instead of a group one
				if (isHidden) {
					strMAns = "��Ⱥ" + strGroupName + "��" + strMAns;
					sendPrivateMsg(eve.fromQQ, strMAns);
				}
				else
				sendGroupMsg(eve.fromGroup, strMAns);
			}
			else if (strSecondInput == "������������"|| strSecondInput == "������������" || strSecondInput == "������������" || strSecondInput == "COC6") {
				//COC6

				//Save the result of COC6
				string strMAns = strNickName;

				//Call the COC6 roll dice function
				COC6(strMAns);

				//If is hidden, send a private msg instead of a group one
				if (isHidden) {
					strMAns = "��Ⱥ" + strGroupName + "��" + strMAns;
					sendPrivateMsg(eve.fromQQ, strMAns);
				}
				else
				sendGroupMsg(eve.fromGroup, strMAns);
			}
			else if (strSecondInput=="DND"||strSecondInput=="Ӣ������"||strSecondInput=="Ӣ������"||strSecondInput=="Ӣ������") {
				//DND
				
				//Save the result of DND
				string strMAns = strNickName + "��Ӣ������:\n";

				//Call the DND roll dice function
				DND(strMAns);

				//If is hidden, send a private msg instead of a group one
				if (isHidden) {
					strMAns = "��Ⱥ" + strGroupName + "��" + strMAns;
					sendPrivateMsg(eve.fromQQ, strMAns);
				}
				else
					sendGroupMsg(eve.fromGroup, strMAns);
			}
			else if (strSecondInput[0]=='B') {
				string strBonusNum = strSecondInput.substr(1);
				for (const auto i : strBonusNum) {
					if (!isdigit(i)) {
						sendGroupMsg(eve.fromGroup, strErrMsg);
						eve.message_block();
						return;
					}
				}
				int intBonusNum = Str2Int(strBonusNum);
				if (intBonusNum <= 0) {
					sendGroupMsg(eve.fromGroup, "��������������!");
					eve.message_block();
					return;
				}
				int D100res = 0;
				MainRoll(D100res, "D100");
				int MinVal = 10;
				string strAns = strNickName + "������:\nD100=" + to_string(D100res) + "\n������:";
				for (int i = 1; i <= intBonusNum; i++) {
					Sleep(1);
					int D10res = -!!(D100res % 10);
					MainRoll(D10res, "D10");
					if (D10res < MinVal)MinVal = D10res;
					strAns += to_string(D10res) + " ";
				}
				strAns[strAns.length() - 1] = '\n';
				strAns += "���Ϊ:" + to_string((D100res / 10 < MinVal ? D100res / 10 : MinVal) * 10 + D100res % 10);
				if (intMsgCnt != eve.message.length())strReason = eve.message.substr(intMsgCnt);
				//If the reason is available, add the reason in the front of the output
				if (!strReason.empty()) {
					strAns = "����" + strReason + strAns;
				}
				if (isHidden) {
					strAns = "��Ⱥ" + strGroupName + "��" + strAns;
					sendPrivateMsg(eve.fromQQ, strAns);
				}
				else
					sendGroupMsg(eve.fromGroup, strAns);
				
			}
			else if (strSecondInput[0] == 'P') {
				string strBonusNum = strSecondInput.substr(1);
				for (const auto i : strBonusNum) {
					if (!isdigit(i)) {
						sendGroupMsg(eve.fromGroup, strErrMsg);
						eve.message_block();
						return;
					}
				}
				int intBonusNum = Str2Int(strBonusNum);
				if (intBonusNum <= 0) {
					sendGroupMsg(eve.fromGroup, "�ͷ�����������!");
					eve.message_block();
					return;
				}
				int D100res = 0;
				MainRoll(D100res, "D100");
				int MaxVal = 0;
				string strAns = strNickName + "������:\nD100=" + to_string(D100res) + "\n�ͷ���:";
				for (int i = 1; i <= intBonusNum; i++) {
					Sleep(1);
					int D10res = -!!(D100res % 10);
					MainRoll(D10res, "D10");
					if (D10res > MaxVal)MaxVal = D10res;
					strAns += to_string(D10res) + " ";
				}
				strAns[strAns.length() - 1] = '\n';
				strAns += "���Ϊ:" + to_string((D100res / 10 > MaxVal ? D100res / 10 : MaxVal) * 10 + D100res % 10);
				if (intMsgCnt != eve.message.length())strReason = eve.message.substr(intMsgCnt);
				//If the reason is available, add the reason in the front of the output
				if (!strReason.empty()) {
					strAns = "����" + strReason + strAns;
				}
				if (isHidden) {
					strAns = "��Ⱥ" + strGroupName + "��" + strAns;
					sendPrivateMsg(eve.fromQQ, strAns);
				}
				else
					sendGroupMsg(eve.fromGroup, strAns);
			}
			else {
				
				for (int k = 0; k != strSecondInput.length(); k++) {
					if (!isprint(strSecondInput[k]) || (!isdigit(strSecondInput[k])&& strSecondInput[k]!='-'&&strSecondInput[k]!='+' && strSecondInput[k] != 'D') || (strSecondInput[k] == 'D' && !isdigit(strSecondInput[k + 1]))) {
						strReason += strSecondInputCopy.substr(k);
						strSecondInput = strSecondInput.substr(0, k);
						break;
					}
				}
				
				//Default Dice
				if (strSecondInput == "")strSecondInput = "D100";

				//Output string, save the main answer
				string strAns = strNickName + "������: " + strSecondInput + "=";

				//Save the final result
				int intMainSum = 0;

				//Save the second result
				string strSecondRes;

				//Call the function of rolling dice
				int intRollRes = MainRoll(intMainSum, strSecondInput, strAns, strSecondRes);

				//If the return value is -1, the input is illegal
				if (intRollRes == -1) {
					sendGroupMsg(eve.fromGroup, strErrMsg);
					eve.message_block();
					return;
				}

				//Skip the space
				while (eve.message[intMsgCnt] == ' '&&intMsgCnt != eve.message.length())intMsgCnt++;



				//Get the reason from the third part of the input
				if (intMsgCnt != eve.message.length())strReason += eve.message.substr(intMsgCnt);
				
				//If the reason is available, add the reason in the front of the output
				if (!strReason.empty()) {
					strAns = "����" + strReason + strAns;
				}

				//Avoid Repeat Result
				if (strAns.substr(strAns.rfind("=", strAns.length() - 2) + 1, strAns.length() - strAns.rfind("=", strAns.length() - 2) - 2) != strSecondRes) {
					strAns += strSecondRes;
					if (strSecondRes != to_string(intMainSum)) {
						strAns += '=';
						strAns += to_string(intMainSum);
					}
				}
				else {

					//Avoid unnecessary character
					strAns.erase(strAns.end() - 1);
				}

				//If is hidden, send a private msg instead of a group one
				if (isHidden) {
					strAns = "��Ⱥ" + strGroupName + "��" + strAns;
					sendPrivateMsg(eve.fromQQ, strAns);
				}
				else
				sendGroupMsg(eve.fromGroup, strAns);
			}

			//Avoid being frozen
			Sleep(50);
		}

		//If is hidden, send a hidden dice notice
		if (isHidden) {
			sendGroupMsg(eve.fromGroup, strHiddenNotice);
		}
		eve.message_block();
		return;
	}
	else if ((strFirstInput==".sn"|| strFirstInput == ".set"||strFirstInput == ".setname"|| strFirstInput == ".nickname"|| strFirstInput == ".nn") && !DisabledGroup.count(eve.fromGroup)) {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string name = eve.message.substr(intMsgCnt);
		if (!name.empty()) {
			GroupName[pairQQGroup] = name;
			sendGroupMsg(eve.fromGroup, "�ѽ�" + strNickName + "�����Ƹ���Ϊ" + name);
		}
		else {
			if (GroupName.count(pairQQGroup)){
				GroupName.erase(pairQQGroup);
				sendGroupMsg(eve.fromGroup, "�ѽ�" + strNickName + "������ɾ��");
			}
			else sendGroupMsg(eve.fromGroup, strErrMsg);
		}
		eve.message_block();
		return;
	}
	else if ((strFirstInput == ".deletename" || strFirstInput == ".delete" || strFirstInput == ".del" || strFirstInput == ".dn") && !DisabledGroup.count(eve.fromGroup)) {
		if (GroupName.count(pairQQGroup)) {
			GroupName.erase(pairQQGroup);
			sendGroupMsg(eve.fromGroup, "�ѽ�" + strNickName + "������ɾ��");
		}
		else sendGroupMsg(eve.fromGroup, strErrMsg);
		eve.message_block();
		return;
	}
	else if (strFirstInput==".on") {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string Number = eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		if (Number == "" || Number == to_string(getLoginQQ())) {
			if (getGroupMemberInfo(eve.fromGroup, eve.fromQQ).permissions >= 2) {
				if (DisabledGroup.count(eve.fromGroup)) {
					DisabledGroup.erase(eve.fromGroup);
					sendGroupMsg(eve.fromGroup, "�ɹ�������������!");
				}
				else sendGroupMsg(eve.fromGroup, "���������Ѿ����ڿ���״̬!");
			}
			else sendGroupMsg(eve.fromGroup, "���ʱ��ܾ�,��û���㹻��Ȩ��!");
		}
		eve.message_block();
		return;
	}
	else if (strFirstInput == ".off") {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string Number = eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		if (Number == "" || Number == to_string(getLoginQQ())) {
			if (getGroupMemberInfo(eve.fromGroup, eve.fromQQ).permissions >= 2) {
				if (!DisabledGroup.count(eve.fromGroup)) {
					DisabledGroup.insert(eve.fromGroup);
					sendGroupMsg(eve.fromGroup, "�ɹ��رձ�������!");
				}
				else sendGroupMsg(eve.fromGroup, "���������Ѿ����ڹر�״̬!");
			}
			else sendGroupMsg(eve.fromGroup, "���ʱ��ܾ�,��û���㹻��Ȩ��!");
		}
		eve.message_block();
		return;
	}
	else if (strFirstInput == ".status"||strFirstInput==".st") {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string Number = eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		if (Number == "" || Number == to_string(getLoginQQ())) {
			if(DisabledGroup.count(eve.fromGroup)) sendGroupMsg(eve.fromGroup, "�����˴��ڹر�״̬!");
			else sendGroupMsg(eve.fromGroup, "�����˴��ڿ���״̬!");
		}
		eve.message_block();
		return;
	}
	else if ((strFirstInput == ".����" || strFirstInput == ".help") && !DisabledGroup.count(eve.fromGroup)) {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string Help = eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		for (auto &i : Help)i = tolower(i);	
		if (Help[0] == '.')Help.erase(Help.begin());
		if (Help == "") 
			sendGroupMsg(eve.fromGroup, strHlpMsg);
		else if (Help == "o" || Help == "r"|| Help == "rd") 
			sendGroupMsg(eve.fromGroup, strNormalDiceMsg);
		else if (Help == "h" || Help == "rh") 
			sendGroupMsg(eve.fromGroup, strHiddenDiceMsg);
		else if (Help == "sn" || Help == "set" ||Help=="setname"||Help=="nn"||Help=="nickname") 
			sendGroupMsg(eve.fromGroup, strSNMsg);
		else if (Help == "dn" || Help == "del" || Help == "delete" || Help == "deletename")
			sendGroupMsg(eve.fromGroup, strDNMsg);
		else if (Help == "on")
			sendGroupMsg(eve.fromGroup, strONMsg);
		else if (Help == "off")
			sendGroupMsg(eve.fromGroup, strOFFMsg);
		else if (Help == "st" || Help == "status")
			sendGroupMsg(eve.fromGroup, strSTMsg);
		else if (Help == "me")
			sendGroupMsg(eve.fromGroup, strMEMsg);
		else if (Help == "help"|| Help == "����")
			sendGroupMsg(eve.fromGroup, strHMsg);
		else if (Help == "sc")
			sendGroupMsg(eve.fromGroup, strSCMsg);
		else 
			sendGroupMsg(eve.fromGroup, "δ�ҵ���Ӧ�İ����ı�");
		eve.message_block();
		return;
	}
	else if (strFirstInput == ".me" && !DisabledGroup.count(eve.fromGroup)) {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string Action = eve.message.substr(intMsgCnt);
		if (Action != "")sendGroupMsg(eve.fromGroup, strNickName + Action);
		eve.message_block();
		return;
	}
	else if (strFirstInput == ".sc" && !DisabledGroup.count(eve.fromGroup)) {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string SanCost = eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		intMsgCnt = eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt);
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string San= eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		if (San == "" || SanCost == "" || SanCost.find("/")==string::npos) {
			sendGroupMsg(eve.fromGroup, strErrMsg);
			eve.message_block();
			return;
		}
		for (auto i : San)
			if (!isdigit(i)) {
				sendGroupMsg(eve.fromGroup, strErrMsg);
				eve.message_block();
				return;
			}
		int intSan = 0;
		stringstream toint;
		toint << San;
		toint >> intSan;
		if (San.length() >= 3 || intSan == 0) {
			sendGroupMsg(eve.fromGroup, "Sanֵ���벻��ȷ!");
			eve.message_block();
			return;
		}
		string strAns = strNickName + "��Sancheck:\n1D100=";
		int D100res = 0;
		MainRoll(D100res, "D100");
		strAns += to_string(D100res);
		if (D100res <= intSan) {
			strAns += " �ɹ�\n���Sanֵ����" + SanCost.substr(0, SanCost.find("/"));
			int intReduceSan = 0;
			int Rollres=MainRoll(intReduceSan, SanCost.substr(0, SanCost.find("/")));
			if (Rollres == -1) {
				sendGroupMsg(eve.fromGroup, strErrMsg);
				eve.message_block();
				return;
			}
			if(SanCost.substr(0, SanCost.find("/")).find("d") != string::npos|| SanCost.substr(0, SanCost.find("/")).find("D") != string::npos)strAns += "=" + to_string(intReduceSan);
			strAns += +"��,��ǰʣ��" + to_string(intSan - intReduceSan) + "��";
		}
		else {
			strAns += " ʧ��\n���Sanֵ����" + SanCost.substr(SanCost.find("/")+1);
			int intReduceSan = 0;
			int Rollres=MainRoll(intReduceSan,SanCost.substr(SanCost.find("/")+1));
			if (Rollres == -1) {
				sendGroupMsg(eve.fromGroup, strErrMsg);
				eve.message_block();
				return;
			}
			if (SanCost.substr(SanCost.find("/") + 1).find("d") != string::npos || SanCost.substr(SanCost.find("/") + 1).find("D") != string::npos)strAns += "=" + to_string(intReduceSan);
			strAns += +"��,��ǰʣ��" + to_string(intSan - intReduceSan) + "��";
		}
		sendGroupMsg(eve.fromGroup, strAns);
		eve.message_block();
		return;
	}
	if (eve.message[0] == '.')sendGroupMsg(eve.fromGroup, "�����������!�Ƿ���д�˿ո�?����.help��ȡ����");
	eve.message_ignore();
	return;
}

///<summary>
///The function is used for receiving and dealing with private message
///Notice:privateMsg is the NAME of the Function
///</summary>
///<param name="eve">Main class of the message</param>
EVE_PrivateMsg_EX(privateMsg)
{//Skip the space
	while (eve.message[0] == ' ')eve.message.erase(eve.message.begin());
	while (eve.message[eve.message.length() - 1] == ' ')eve.message.erase(eve.message.end() - 1);
	if (eve.message.find("��") == 0) {
		eve.message.erase(eve.message.begin());
		eve.message[0] = '.';
	}

	//Save the first input
	string strFirstInput = eve.message.substr(0, eve.message.find(" "));

	//Use the string to save the second part of the Input
	string strSecondInput;

	//Replace the "��" in the front to "."
	if (strFirstInput[0] != '.')return;

	if (eve.message == "." || strFirstInput[1] == '.' || strFirstInput.find("��") == 1)return;

	for (int k = 0; k != strFirstInput.length(); k++) {
		if (isdigit(strFirstInput[k])) {
			strSecondInput += strFirstInput.substr(k);
			strFirstInput = strFirstInput.substr(0, k);
			break;
		}
		if ((strFirstInput[k] == 'd' || strFirstInput[k] == 'D') && (isdigit(strFirstInput[k + 1]))) {
			strSecondInput += strFirstInput.substr(k);
			strFirstInput = strFirstInput.substr(0, k);
			break;
		}
		if (!isprint(strFirstInput[k])) {
			strSecondInput += strFirstInput.substr(k);
			strFirstInput = strFirstInput.substr(0, k);
			break;
		}
	}
	if (strFirstInput == ".")strFirstInput = ".rd";

	//Save the Nickname
	string strNickName = getStrangerInfo(eve.fromQQ).nick;

	//Tolower the first input
	for (auto &i : strFirstInput)if (isupper(i))i = tolower(i);

	//Judge if it is a command
	if (strFirstInput == ".o" || strFirstInput == ".rd" || strFirstInput == ".r" || strFirstInput == ".d") {
		//Use it to save the count of the Msg have read
		int intMsgCnt = eve.message.find(" ") == string::npos ? eve.message.length() : eve.message.find(" ");

		//Skip the space
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;

		//Save the Reason
		string strReason;

		//Get the Second Input
		strSecondInput += eve.message.substr(intMsgCnt, eve.message.find(" ", intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(" ", intMsgCnt) - intMsgCnt);
		string strSecondInputCopy = strSecondInput;
		//Toupper the second input
		for (auto &i : strSecondInput)if (islower(i))i = toupper(i);

		//Make the intMsgCnt jump to the space after the second input
		intMsgCnt = eve.message.find(" ", intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(" ", intMsgCnt);

		//Save the number or dice before # to get the time of rolling dice
		int intRollDiceTime = 0;
		if (strSecondInput.find("#") != string::npos) {
			if (MainRoll(intRollDiceTime, strSecondInput.substr(0, strSecondInput.find("#"))) == -1 || intRollDiceTime <= 0) {
				sendPrivateMsg(eve.fromQQ, strErrMsg);
				eve.message_block();
				return;
			}
		}
		else {

			//"#" not found, so only roll for one time
			intRollDiceTime = 1;
		}

		//If the input contains # and the input before # is dice, send a roll-dice-time notice
		if (strSecondInput.find("#") != string::npos && (strSecondInput.substr(0, strSecondInput.find("#")).find("d") != string::npos || strSecondInput.substr(0, strSecondInput.find("#")).find("D") != string::npos)) {
			string strRollDiceTurnNotice;
			strRollDiceTurnNotice += "��������: " + strSecondInput.substr(0, strSecondInput.find("#")) + "=" + to_string(intRollDiceTime) + "��";
			sendPrivateMsg(eve.fromQQ, strRollDiceTurnNotice);
		}
		if (intRollDiceTime > 10) {
			sendPrivateMsg(eve.fromQQ, strTooManyTimesNotice);
			eve.message_block();
			return;
		}
		//Save the count of erasing characters
		int erase_cnt = strSecondInput.find("#") == string::npos ? -1 : strSecondInput.find("#");

		//Erase the input # and the char before it
		for (int i = 0; i <= erase_cnt; ++i) strSecondInput.erase(strSecondInput.begin());
		for (int i = 0; i <= erase_cnt; ++i) strSecondInputCopy.erase(strSecondInputCopy.begin());
		//Roll the dice
		for (int w = 0; w != intRollDiceTime; ++w) {
			if (strSecondInput == "��������" || strSecondInput == "��������" || strSecondInput == "��������" || strSecondInput == "�߰���������" || strSecondInput == "�߰���������" || strSecondInput == "�߰���������" || strSecondInput == "COC" || strSecondInput == "COC7") {
				//COC7

				//Save the result of COC7
				string strMAns = strNickName;

				//Call the COC7 roll dice function
				COC7(strMAns);

				sendPrivateMsg(eve.fromQQ, strMAns);
			}
			else if (strSecondInput == "������������" || strSecondInput == "������������" || strSecondInput == "������������" || strSecondInput == "COC6") {
				//COC6

				//Save the result of COC6
				string strMAns = strNickName;

				//Call the COC6 roll dice function
				COC6(strMAns);

				sendPrivateMsg(eve.fromQQ, strMAns);

			}
			else if (strSecondInput == "DND" || strSecondInput == "Ӣ������" || strSecondInput == "Ӣ������" || strSecondInput == "Ӣ������") {
				//DND

				//Save the result of DND
				string strMAns = strNickName + "��Ӣ������:\n";

				//Call the DND roll dice function
				DND(strMAns);


				sendPrivateMsg(eve.fromQQ, strMAns);

			}
			else if (strSecondInput[0] == 'B') {
				string strBonusNum = strSecondInput.substr(1);
				for (const auto i : strBonusNum) {
					if (!isdigit(i)) {
						sendPrivateMsg(eve.fromQQ, strErrMsg);
						eve.message_block();
						return;
					}
				}
				int intBonusNum = Str2Int(strBonusNum);
				if (intBonusNum <= 0) {
					sendPrivateMsg(eve.fromQQ, "��������������!");
					eve.message_block();
					return;
				}
				int D100res = 0;
				MainRoll(D100res, "D100");
				int MinVal = 10;
				string strAns = strNickName + "������:\nD100=" + to_string(D100res) + "\nD10������:";
				for (int i = 1; i <= intBonusNum; i++) {
					Sleep(1);
					int D10res = -!!(D100res % 10);
					MainRoll(D10res, "D10");
					if (D10res < MinVal)MinVal = D10res;
					strAns += to_string(D10res) + " ";
				}
				strAns[strAns.length() - 1] = '\n';
				strAns += "���Ϊ:" + to_string((D100res / 10 < MinVal ? D100res / 10 : MinVal) * 10 + D100res % 10);
				if (intMsgCnt != eve.message.length())strReason = eve.message.substr(intMsgCnt);
				//If the reason is available, add the reason in the front of the output
				if (!strReason.empty()) {
					strAns = "����" + strReason + strAns;
				}
				sendPrivateMsg(eve.fromQQ, strAns);
			}
			else if (strSecondInput[0] == 'P') {
				string strBonusNum = strSecondInput.substr(1);
				for (const auto i : strBonusNum) {
					if (!isdigit(i)) {
						sendPrivateMsg(eve.fromQQ, strErrMsg);
						eve.message_block();
						return;
					}
				}
				int intBonusNum = Str2Int(strBonusNum);
				if (intBonusNum <= 0) {
					sendPrivateMsg(eve.fromQQ, "�ͷ�����������!");
					eve.message_block();
					return;
				}
				int D100res = 0;
				MainRoll(D100res, "D100");
				int MaxVal = 0;
				string strAns = strNickName + "������:\nD100=" + to_string(D100res) + "\n�ͷ���:";
				for (int i = 1; i <= intBonusNum; i++) {
					Sleep(1);
					int D10res = -!!(D100res % 10);
					MainRoll(D10res, "D10");
					if (D10res > MaxVal)MaxVal = D10res;
					strAns += to_string(D10res) + " ";
				}
				strAns[strAns.length() - 1] = '\n';
				strAns += "���Ϊ:" + to_string((D100res / 10 > MaxVal ? D100res / 10 : MaxVal) * 10 + D100res % 10);
				if (intMsgCnt != eve.message.length())strReason = eve.message.substr(intMsgCnt);
				//If the reason is available, add the reason in the front of the output
				if (!strReason.empty()) {
					strAns = "����" + strReason + strAns;
				}
				sendPrivateMsg(eve.fromQQ, strAns);
			}
			else {

				for (int k = 0; k != strSecondInput.length(); k++) {
					if (!isprint(strSecondInput[k]) || (!isdigit(strSecondInput[k]) && strSecondInput[k] != '-'&&strSecondInput[k] != '+' && strSecondInput[k] != 'D') || (strSecondInput[k] == 'D' && !isdigit(strSecondInput[k + 1]))) {
						strReason += strSecondInputCopy.substr(k);
						strSecondInput = strSecondInput.substr(0, k);
						break;
					}
				}

				//Default Dice
				if (strSecondInput == "")strSecondInput = "D100";

				//Output string, save the main answer
				string strAns = strNickName + "������: " + strSecondInput + "=";

				//Save the final result
				int intMainSum = 0;

				//Save the second result
				string strSecondRes;

				//Call the function of rolling dice
				int intRollRes = MainRoll(intMainSum, strSecondInput, strAns, strSecondRes);

				//If the return value is -1, the input is illegal
				if (intRollRes == -1) {
					sendPrivateMsg(eve.fromQQ, strErrMsg);
					eve.message_block();
					return;
				}

				//Skip the space
				while (eve.message[intMsgCnt] == ' '&&intMsgCnt != eve.message.length())intMsgCnt++;



				//Get the reason from the third part of the input
				if (intMsgCnt != eve.message.length())strReason += eve.message.substr(intMsgCnt);

				//If the reason is available, add the reason in the front of the output
				if (!strReason.empty()) {
					strAns = "����" + strReason + strAns;
				}

				//Avoid Repeat Result
				if (strAns.substr(strAns.rfind("=", strAns.length() - 2) + 1, strAns.length() - strAns.rfind("=", strAns.length() - 2) - 2) != strSecondRes) {
					strAns += strSecondRes;
					if (strSecondRes != to_string(intMainSum)) {
						strAns += '=';
						strAns += to_string(intMainSum);
					}
				}
				else {

					//Avoid unnecessary character
					strAns.erase(strAns.end() - 1);
				}
				sendPrivateMsg(eve.fromQQ, strAns);
			}

			//Avoid being frozen
			Sleep(50);
		}
		eve.message_block();
		return;
	}
	else if (strFirstInput == ".����" || strFirstInput == ".help") {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string Help = eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		for (auto &i : Help)i = tolower(i);
		if (Help[0] == '.')Help.erase(Help.begin());
		if (Help == "")
			sendPrivateMsg(eve.fromQQ, strHlpMsg);
		else if (Help == "o" || Help == "r" || Help == "rd")
			sendPrivateMsg(eve.fromQQ, strNormalDiceMsg);
		else if (Help == "h" || Help == "rh")
			sendPrivateMsg(eve.fromQQ, strHiddenDiceMsg);
		else if (Help == "sn" || Help == "set" || Help == "setname" || Help == "nn" || Help == "nickname")
			sendPrivateMsg(eve.fromQQ, strSNMsg);
		else if (Help == "dn" || Help == "del" || Help == "delete" || Help == "deletename")
			sendPrivateMsg(eve.fromQQ, strDNMsg);
		else if (Help == "on")
			sendPrivateMsg(eve.fromQQ, strONMsg);
		else if (Help == "off")
			sendPrivateMsg(eve.fromQQ, strOFFMsg);
		else if (Help == "st" || Help == "status")
			sendPrivateMsg(eve.fromQQ, strSTMsg);
		else if (Help == "me")
			sendPrivateMsg(eve.fromQQ, strMEMsg);
		else if (Help == "help" || Help == "����")
			sendPrivateMsg(eve.fromQQ, strHMsg);
		else if (Help == "sc")
			sendPrivateMsg(eve.fromQQ, strSCMsg);
		else
			sendPrivateMsg(eve.fromQQ, "δ�ҵ���Ӧ�İ����ı�");
		eve.message_block();
		return;
	}
	else if (strFirstInput == ".me") {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string strGroupnum = eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		string Action;
		for (int q = 0; q != strGroupnum.length(); q++) {
			if (!isdigit(strGroupnum[q])) {
				Action += strGroupnum.substr(q);
				strGroupnum = strGroupnum.substr(0, q);
				break;
			}
		}
		intMsgCnt = eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt);
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		Action += eve.message.substr(intMsgCnt);
		long long llGroupnum;
		stringstream toint;
		toint << strGroupnum;
		toint >> llGroupnum;
		if (Action == "") {
			sendPrivateMsg(eve.fromQQ, strErrMsg);
		}
		else if (strGroupnum == "") {
			sendPrivateMsg(eve.fromQQ, "δ����Ⱥ��!����.help��.��������ð���!");
		}
		else {
			pair<long long, long long>pairQQGroup(eve.fromQQ, llGroupnum);
			if (!DisabledGroup.count(llGroupnum)) {
				int Sendres=sendGroupMsg(llGroupnum, (GroupName.count(pairQQGroup) ? GroupName[pairQQGroup] : getGroupMemberInfo(llGroupnum, eve.fromQQ).GroupNickname.empty() ? getStrangerInfo(eve.fromQQ).nick : getGroupMemberInfo(llGroupnum, eve.fromQQ).GroupNickname) + Action);
				if(Sendres>=0)sendPrivateMsg(eve.fromQQ, "����ִ�гɹ�!");
				else sendPrivateMsg(eve.fromQQ, "����ִ��ʧ��!�Ƿ������˴����Ⱥ��?");
			}
			else {
				sendPrivateMsg(eve.fromQQ, "����ʧ��!�ڴ�Ⱥ�л������ѱ��ر�!");
			}
		}
		eve.message_block();
		return;
	}
	else if (strFirstInput == ".sc") {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string SanCost = eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		intMsgCnt = eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt);
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string San = eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		if (San == "" || SanCost == "" || SanCost.find("/") == string::npos) {
			sendPrivateMsg(eve.fromQQ, strErrMsg);
			eve.message_block();
			return;
		}
		for (auto i : San)
			if (!isdigit(i)) {
				sendPrivateMsg(eve.fromQQ, strErrMsg);
				eve.message_block();
				return;
			}
		int intSan = 0;
		stringstream toint;
		toint << San;
		toint >> intSan;
		if (San.length() >= 3 || intSan == 0) {
			sendPrivateMsg(eve.fromQQ, "Sanֵ���벻��ȷ!");
			eve.message_block();
			return;
		}
		string strAns = strNickName + "��Sancheck:\n1D100=";
		int D100res = 0;
		MainRoll(D100res, "D100");
		strAns += to_string(D100res);
		if (D100res <= intSan) {
			strAns += " �ɹ�\n���Sanֵ����" + SanCost.substr(0, SanCost.find("/"));
			int intReduceSan = 0;
			int Rollres = MainRoll(intReduceSan, SanCost.substr(0, SanCost.find("/")));
			if (Rollres == -1) {
				sendPrivateMsg(eve.fromQQ, strErrMsg);
				eve.message_block();
				return;
			}
			if (SanCost.substr(0, SanCost.find("/")).find("d") != string::npos || SanCost.substr(0, SanCost.find("/")).find("D") != string::npos)strAns += "=" + to_string(intReduceSan);
			strAns += +"��,��ǰʣ��" + to_string(intSan - intReduceSan) + "��";
		}
		else {
			strAns += " ʧ��\n���Sanֵ����" + SanCost.substr(SanCost.find("/")+1);
			int intReduceSan = 0;
			int Rollres = MainRoll(intReduceSan, SanCost.substr(SanCost.find("/")+1));
			if (Rollres == -1) {
				sendPrivateMsg(eve.fromQQ, strErrMsg);
				eve.message_block();
				return;
			}
			if (SanCost.substr(SanCost.find("/") + 1).find("d") != string::npos || SanCost.substr(SanCost.find("/") + 1).find("D") != string::npos)strAns += "=" + to_string(intReduceSan);
			strAns += +"��,��ǰʣ��" + to_string(intSan - intReduceSan) + "��";
		}
		sendPrivateMsg(eve.fromQQ, strAns);
		eve.message_block();
		return;
	}

	if(eve.message[0]=='.')sendPrivateMsg(eve.fromQQ, "�����������!�Ƿ���д�˿ո�?����.help��ȡ����");
	eve.message_ignore();
	return;
}
///<summary>
///The function is used for receiving and dealing with discuss message
///Notice:discussMsg is the NAME of the Function
///</summary>
///<param name="eve">Main class of the message</param>
EVE_DiscussMsg_EX(discussMsg) {
	pair<long long, long long> pairQQDiscuss;
	pairQQDiscuss.first = eve.fromQQ;
	pairQQDiscuss.second = eve.fromDiscuss;

	//Skip the space
	while (eve.message[0] == ' ')eve.message.erase(eve.message.begin());
	while (eve.message[eve.message.length() - 1] == ' ')eve.message.erase(eve.message.end() - 1);
	if (eve.message.find("��") == 0) {
		eve.message.erase(eve.message.begin());
		eve.message[0] = '.';
	}

	//Save the first input
	string strFirstInput = eve.message.substr(0, eve.message.find(" "));

	//Use the string to save the second part of the Input
	string strSecondInput;

	//Replace the "��" in the front to "."
	if (strFirstInput[0] != '.')return;
	if (eve.message == "." || strFirstInput[1] == '.' || strFirstInput.find("��") == 1)return;
	for (int k = 0; k != strFirstInput.length(); k++) {
		if (isdigit(strFirstInput[k])) {
			strSecondInput += strFirstInput.substr(k);
			strFirstInput = strFirstInput.substr(0, k);
			break;
		}
		if ((strFirstInput[k] == 'd' || strFirstInput[k] == 'D') && (isdigit(strFirstInput[k + 1]))) {
			strSecondInput += strFirstInput.substr(k);
			strFirstInput = strFirstInput.substr(0, k);
			break;
		}
		if (!isprint(strFirstInput[k])) {
			strSecondInput += strFirstInput.substr(k);
			strFirstInput = strFirstInput.substr(0, k);
			break;
		}
	}

	if (strFirstInput == ".")strFirstInput = ".rd";

	//Save the DiscussNick. if the DiscussNick is empty, save the Nickname
	string strNickName = DiscussName.count(pairQQDiscuss) ? DiscussName[pairQQDiscuss] : getStrangerInfo(eve.fromQQ).nick;

	//Tolower the first input
	for (auto &i : strFirstInput)if (isupper(i))i = tolower(i);

	//Judge if it is a command
	if ((strFirstInput == ".o" || strFirstInput == ".h" || strFirstInput == ".rd" || strFirstInput == ".r" || strFirstInput == ".rh" || strFirstInput == ".d") && !DisabledDiscuss.count(eve.fromDiscuss)) {

		//Save the Discuss name
		string strDiscussName = to_string(eve.fromDiscuss);

		//Use it to save the HiddenDice Notice
		string strHiddenNotice = strNickName + "������һ�ΰ���";

		//Use it to save the count of the Msg have read
		int intMsgCnt = eve.message.find(" ") == string::npos ? eve.message.length() : eve.message.find(" ");

		//Skip the space
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;

		//Save the Reason
		string strReason;

		//Get the Second Input
		strSecondInput += eve.message.substr(intMsgCnt, eve.message.find(" ", intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(" ", intMsgCnt) - intMsgCnt);
		string strSecondInputCopy = strSecondInput;
		//Toupper the second input
		for (auto &i : strSecondInput)if (islower(i))i = toupper(i);

		//Make the intMsgCnt jump to the space after the second input
		intMsgCnt = eve.message.find(" ", intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(" ", intMsgCnt);

		//Check if it is a "hidden dice"
		bool isHidden = false;
		if (strFirstInput == ".h" || strFirstInput == ".rh")isHidden = true;

		//Save the number or dice before # to get the time of rolling dice
		int intRollDiceTime = 0;
		if (strSecondInput.find("#") != string::npos) {
			if (MainRoll(intRollDiceTime, strSecondInput.substr(0, strSecondInput.find("#"))) == -1 || intRollDiceTime <= 0) {
				sendDiscussMsg(eve.fromDiscuss, strErrMsg);
				eve.message_block();
				return;
			}
		}
		else {

			//"#" not found, so only roll for one time
			intRollDiceTime = 1;
		}

		//If the input contains # and the input before # is dice, send a roll-dice-time notice
		if (strSecondInput.find("#") != string::npos && (strSecondInput.substr(0, strSecondInput.find("#")).find("d") != string::npos || strSecondInput.substr(0, strSecondInput.find("#")).find("D") != string::npos)) {
			string strRollDiceTurnNotice;
			strRollDiceTurnNotice += "��������: " + strSecondInput.substr(0, strSecondInput.find("#")) + "=" + to_string(intRollDiceTime) + "��";
			if (isHidden)sendPrivateMsg(eve.fromQQ, strRollDiceTurnNotice); else
				sendDiscussMsg(eve.fromDiscuss, strRollDiceTurnNotice);
		}
		if (intRollDiceTime > 10) {
			sendDiscussMsg(eve.fromDiscuss, strTooManyTimesNotice);
			eve.message_block();
			return;
		}
		//Save the count of erasing characters
		int erase_cnt = strSecondInput.find("#") == string::npos ? -1 : strSecondInput.find("#");

		//Erase the input # and the char before it
		for (int i = 0; i <= erase_cnt; ++i) strSecondInput.erase(strSecondInput.begin());
		for (int i = 0; i <= erase_cnt; ++i) strSecondInputCopy.erase(strSecondInputCopy.begin());
		//Roll the dice
		for (int w = 0; w != intRollDiceTime; ++w) {
			if (strSecondInput == "��������" || strSecondInput == "��������" || strSecondInput == "��������" || strSecondInput == "�߰���������" || strSecondInput == "�߰���������" || strSecondInput == "�߰���������" || strSecondInput == "COC" || strSecondInput == "COC7") {
				//COC7

				//Save the result of COC7
				string strMAns = strNickName;

				//Call the COC7 roll dice function
				COC7(strMAns);

				//If is hidden, send a private msg instead of a Discuss one
				if (isHidden) {
					strMAns = "�ڶ�������" + strDiscussName + "��" + strMAns;
					sendPrivateMsg(eve.fromQQ, strMAns);
				}
				else
					sendDiscussMsg(eve.fromDiscuss, strMAns);
			}
			else if (strSecondInput == "������������" || strSecondInput == "������������" || strSecondInput == "������������" || strSecondInput == "COC6") {
				//COC6

				//Save the result of COC6
				string strMAns = strNickName;

				//Call the COC6 roll dice function
				COC6(strMAns);

				//If is hidden, send a private msg instead of a Discuss one
				if (isHidden) {
					strMAns = "�ڶ�������" + strDiscussName + "��" + strMAns;
					sendPrivateMsg(eve.fromQQ, strMAns);
				}
				else
					sendDiscussMsg(eve.fromDiscuss, strMAns);
			}
			else if (strSecondInput == "DND" || strSecondInput == "Ӣ������" || strSecondInput == "Ӣ������" || strSecondInput == "Ӣ������") {
				//DND

				//Save the result of DND
				string strMAns = strNickName + "��Ӣ������:\n";

				//Call the DND roll dice function
				DND(strMAns);

				//If is hidden, send a private msg instead of a Discuss one
				if (isHidden) {
					strMAns = "�ڶ�������" + strDiscussName + "��" + strMAns;
					sendPrivateMsg(eve.fromQQ, strMAns);
				}
				else
					sendDiscussMsg(eve.fromDiscuss, strMAns);
			}
			else if (strSecondInput[0] == 'B') {
				string strBonusNum = strSecondInput.substr(1);
				for (const auto i : strBonusNum) {
					if (!isdigit(i)) {
						sendDiscussMsg(eve.fromDiscuss, strErrMsg);
						eve.message_block();
						return;
					}
				}
				int intBonusNum = Str2Int(strBonusNum);
				if (intBonusNum <= 0) {
					sendDiscussMsg(eve.fromDiscuss, "��������������!");
					eve.message_block();
					return;
				}
				int D100res = 0;
				MainRoll(D100res, "D100");
				int MinVal = 10;
				string strAns = strNickName + "������:\nD100=" + to_string(D100res) + "\n������:";
				for (int i = 1; i <= intBonusNum; i++) {
					Sleep(1);
					int D10res = -!!(D100res % 10);
					MainRoll(D10res, "D10");
					if (D10res < MinVal)MinVal = D10res;
					strAns += to_string(D10res) + " ";
				}
				strAns[strAns.length() - 1] = '\n';
				strAns += "���Ϊ:" + to_string((D100res / 10 < MinVal ? D100res / 10 : MinVal) * 10 + D100res % 10);
				if (intMsgCnt != eve.message.length())strReason = eve.message.substr(intMsgCnt);
				//If the reason is available, add the reason in the front of the output
				if (!strReason.empty()) {
					strAns = "����" + strReason + strAns;
				}
				if (isHidden) {
					strAns = "�ڶ�������" + strDiscussName + "��" + strAns;
					sendPrivateMsg(eve.fromQQ, strAns);
				}
				else
					sendDiscussMsg(eve.fromDiscuss, strAns);
			}
			else if (strSecondInput[0] == 'P') {
				string strBonusNum = strSecondInput.substr(1);
				for (const auto i : strBonusNum) {
					if (!isdigit(i)) {
						sendDiscussMsg(eve.fromDiscuss, strErrMsg);
						eve.message_block();
						return;
					}
				}
				int intBonusNum = Str2Int(strBonusNum);
				if (intBonusNum <= 0) {
					sendDiscussMsg(eve.fromDiscuss, "�ͷ�����������!");
					eve.message_block();
					return;
				}
				int D100res = 0;
				MainRoll(D100res, "D100");
				int MaxVal = 0;
				string strAns = strNickName + "������:\nD100=" + to_string(D100res) + "\n�ͷ���:";
				for (int i = 1; i <= intBonusNum; i++) {
					Sleep(1);
					int D10res = -!!(D100res % 10);
					MainRoll(D10res, "D10");
					if (D10res > MaxVal)MaxVal = D10res;
					strAns += to_string(D10res) + " ";
				}
				strAns[strAns.length() - 1] = '\n';
				strAns += "���Ϊ:" + to_string((D100res / 10 > MaxVal ? D100res / 10 : MaxVal) * 10 + D100res % 10);
				while (eve.message[intMsgCnt] == ' '&&intMsgCnt != eve.message.length())intMsgCnt++;
				if (intMsgCnt != eve.message.length())strReason = eve.message.substr(intMsgCnt);
				//If the reason is available, add the reason in the front of the output
				if (!strReason.empty()) {
					strAns = "����" + strReason + strAns;
				}
				if (isHidden) {
					strAns = "�ڶ�������" + strDiscussName + "��" + strAns;
					sendPrivateMsg(eve.fromQQ, strAns);
				}
				else
					sendDiscussMsg(eve.fromDiscuss, strAns);
			}
			else {

				for (int k = 0; k != strSecondInput.length(); k++) {
					if (!isprint(strSecondInput[k]) || (!isdigit(strSecondInput[k]) && strSecondInput[k] != '-'&&strSecondInput[k] != '+' && strSecondInput[k] != 'D') || (strSecondInput[k] == 'D' && !isdigit(strSecondInput[k + 1]))) {
						strReason += strSecondInputCopy.substr(k);
						strSecondInput = strSecondInput.substr(0, k);
						break;
					}
				}

				//Default Dice
				if (strSecondInput == "")strSecondInput = "D100";

				//Output string, save the main answer
				string strAns = strNickName + "������: " + strSecondInput + "=";

				//Save the final result
				int intMainSum = 0;

				//Save the second result
				string strSecondRes;

				//Call the function of rolling dice
				int intRollRes = MainRoll(intMainSum, strSecondInput, strAns, strSecondRes);

				//If the return value is -1, the input is illegal
				if (intRollRes == -1) {
					sendDiscussMsg(eve.fromDiscuss, strErrMsg);
					eve.message_block();
					return;
				}

				//Skip the space
				while (eve.message[intMsgCnt] == ' '&&intMsgCnt != eve.message.length())intMsgCnt++;



				//Get the reason from the third part of the input
				if (intMsgCnt != eve.message.length())strReason += eve.message.substr(intMsgCnt);

				//If the reason is available, add the reason in the front of the output
				if (!strReason.empty()) {
					strAns = "����" + strReason + strAns;
				}

				//Avoid Repeat Result
				if (strAns.substr(strAns.rfind("=", strAns.length() - 2) + 1, strAns.length() - strAns.rfind("=", strAns.length() - 2) - 2) != strSecondRes) {
					strAns += strSecondRes;
					if (strSecondRes != to_string(intMainSum)) {
						strAns += '=';
						strAns += to_string(intMainSum);
					}
				}
				else {

					//Avoid unnecessary character
					strAns.erase(strAns.end() - 1);
				}

				//If is hidden, send a private msg instead of a Discuss one
				if (isHidden) {
					strAns = "�ڶ�������" + strDiscussName + "��" + strAns;
					sendPrivateMsg(eve.fromQQ, strAns);
				}
				else
					sendDiscussMsg(eve.fromDiscuss, strAns);
			}

			//Avoid being frozen
			Sleep(50);
		}

		//If is hidden, send a hidden dice notice
		if (isHidden) {
			sendDiscussMsg(eve.fromDiscuss, strHiddenNotice);
		}
		eve.message_block();
		return;
	}
	else if ((strFirstInput == ".sn" || strFirstInput == ".set" || strFirstInput == ".setname" || strFirstInput == ".nickname" || strFirstInput == ".nn") && !DisabledDiscuss.count(eve.fromDiscuss)) {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string name = eve.message.substr(intMsgCnt);
		if (!name.empty()) {
			DiscussName[pairQQDiscuss] = name;
			sendDiscussMsg(eve.fromDiscuss, "�ѽ�" + strNickName + "�����Ƹ���Ϊ" + name);
		}
		else {
			if (DiscussName.count(pairQQDiscuss)) {
				DiscussName.erase(pairQQDiscuss);
				sendDiscussMsg(eve.fromDiscuss, "�ѽ�" + strNickName + "������ɾ��");
			}
			else sendDiscussMsg(eve.fromDiscuss, strErrMsg);
		}
		eve.message_block();
		return;
	}
	else if ((strFirstInput == ".deletename" || strFirstInput == ".delete" || strFirstInput == ".del" || strFirstInput == ".dn") && !DisabledDiscuss.count(eve.fromDiscuss)) {
		if (DiscussName.count(pairQQDiscuss)) {
			DiscussName.erase(pairQQDiscuss);
			sendDiscussMsg(eve.fromDiscuss, "�ѽ�" + strNickName + "������ɾ��");
		}
		else sendDiscussMsg(eve.fromDiscuss, strErrMsg);
		eve.message_block();
		return;
	}
	else if (strFirstInput == ".on") {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string Number = eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		if (Number == "" || Number == to_string(getLoginQQ())) {
			if (DisabledDiscuss.count(eve.fromDiscuss)) {
				DisabledDiscuss.erase(eve.fromDiscuss);
				sendDiscussMsg(eve.fromDiscuss, "�ɹ�������������!");
			}
			else sendDiscussMsg(eve.fromDiscuss, "���������Ѿ����ڿ���״̬!");
		}
		eve.message_block();
		return;
	}
	else if (strFirstInput == ".off") {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string Number = eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		if (Number == "" || Number == to_string(getLoginQQ())) {
			if (!DisabledDiscuss.count(eve.fromDiscuss)) {
				DisabledDiscuss.insert(eve.fromDiscuss);
				sendDiscussMsg(eve.fromDiscuss, "�ɹ��رձ�������!");
			}
			else sendDiscussMsg(eve.fromDiscuss, "���������Ѿ����ڹر�״̬!");
		}
		eve.message_block();
		return;
	}
	else if (strFirstInput == ".status" || strFirstInput == ".st") {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string Number = eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		if (Number == "" || Number == to_string(getLoginQQ())) {
			if (DisabledDiscuss.count(eve.fromDiscuss)) sendDiscussMsg(eve.fromDiscuss, "�����˴��ڹر�״̬!");
			else sendDiscussMsg(eve.fromDiscuss, "�����˴��ڿ���״̬!");
		}
		eve.message_block();
		return;
	}
	else if ((strFirstInput == ".����" || strFirstInput == ".help") && !DisabledDiscuss.count(eve.fromDiscuss)) {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string Help = eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		for (auto &i : Help)i = tolower(i);
		if (Help[0] == '.')Help.erase(Help.begin());
		if (Help == "")
			sendDiscussMsg(eve.fromDiscuss, strHlpMsg);
		else if (Help == "o" || Help == "r" || Help == "rd")
			sendDiscussMsg(eve.fromDiscuss, strNormalDiceMsg);
		else if (Help == "h" || Help == "rh")
			sendDiscussMsg(eve.fromDiscuss, strHiddenDiceMsg);
		else if (Help == "sn" || Help == "set" || Help == "setname" || Help == "nn" || Help == "nickname")
			sendDiscussMsg(eve.fromDiscuss, strSNMsg);
		else if (Help == "dn" || Help == "del" || Help == "delete" || Help == "deletename")
			sendDiscussMsg(eve.fromDiscuss, strDNMsg);
		else if (Help == "on")
			sendDiscussMsg(eve.fromDiscuss, strONMsg);
		else if (Help == "off")
			sendDiscussMsg(eve.fromDiscuss, strOFFMsg);
		else if (Help == "st" || Help == "status")
			sendDiscussMsg(eve.fromDiscuss, strSTMsg);
		else if (Help == "me")
			sendDiscussMsg(eve.fromDiscuss, strMEMsg);
		else if (Help == "sc")
			sendDiscussMsg(eve.fromDiscuss, strSCMsg);
		else if (Help == "help" || Help == "����")
			sendDiscussMsg(eve.fromDiscuss, strHMsg);
		else
			sendDiscussMsg(eve.fromDiscuss, "δ�ҵ���Ӧ�İ����ı�");
		eve.message_block();
		return;
	}
	else if (strFirstInput == ".me" && !DisabledDiscuss.count(eve.fromDiscuss)) {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string Action = eve.message.substr(intMsgCnt);
		if (Action != "")sendDiscussMsg(eve.fromDiscuss, strNickName + Action);
		eve.message_block();
		return;
	}
	else if (strFirstInput == ".sc" && !DisabledDiscuss.count(eve.fromDiscuss)) {
		int intMsgCnt = strFirstInput.length();
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string SanCost = eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		intMsgCnt = eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt);
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		string San = eve.message.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(' ', intMsgCnt) - intMsgCnt);
		if (San == "" || SanCost == ""||SanCost.find("/")==string::npos) {
			sendDiscussMsg(eve.fromDiscuss, strErrMsg);
			eve.message_block();
			return;
		}
		for (auto i : San)
			if (!isdigit(i)) {
				sendDiscussMsg(eve.fromDiscuss, strErrMsg);
				eve.message_block();
				return;
			}
		int intSan = 0;
		stringstream toint;
		toint << San;
		toint >> intSan;
		if (San.length() >= 3 || intSan == 0) {
			sendDiscussMsg(eve.fromDiscuss, "Sanֵ���벻��ȷ!");
			eve.message_block();
			return;
		}
		string strAns = strNickName + "��Sancheck:\n1D100=";
		int D100res = 0;
		MainRoll(D100res, "D100");
		strAns += to_string(D100res);
		if (D100res <= intSan) {
			strAns += " �ɹ�\n���Sanֵ����" + SanCost.substr(0, SanCost.find("/"));
			int intReduceSan = 0;
			int Rollres = MainRoll(intReduceSan, SanCost.substr(0, SanCost.find("/")));
			if (Rollres == -1) {
				sendDiscussMsg(eve.fromDiscuss, strErrMsg);
				eve.message_block();
				return;
			}
			if (SanCost.substr(0, SanCost.find("/")).find("d") != string::npos || SanCost.substr(0, SanCost.find("/")).find("D") != string::npos)strAns += "=" + to_string(intReduceSan);
			strAns += +"��,��ǰʣ��" + to_string(intSan - intReduceSan) + "��";
		}
		else {
			strAns += " ʧ��\n���Sanֵ����" + SanCost.substr(SanCost.find("/")+1);
			int intReduceSan = 0;
			int Rollres = MainRoll(intReduceSan, SanCost.substr(SanCost.find("/")+1));
			if (Rollres == -1) {
				sendDiscussMsg(eve.fromDiscuss, strErrMsg);
				eve.message_block();
				return;
			}
			if (SanCost.substr(SanCost.find("/") + 1).find("d") != string::npos || SanCost.substr(SanCost.find("/") + 1).find("D") != string::npos)strAns += "=" + to_string(intReduceSan);
			strAns += +"��,��ǰʣ��" + to_string(intSan - intReduceSan) + "��";
		}
		sendDiscussMsg(eve.fromDiscuss, strAns);
		eve.message_block();
		return;
	}
	if (eve.message[0] == '.')sendDiscussMsg(eve.fromDiscuss, "�����������!�Ƿ���д�˿ո�?����.help��ȡ����");
	eve.message_ignore();
	return;
}

EVE_Menu(__menuA)
{
	MessageBoxA(nullptr, strHlpMsg.c_str(), "Help", MB_OK | MB_ICONINFORMATION);
	return 0;
}

EVE_Exit(eventexit) {
	ofstream ofstreamGroupName(strFileLoc + "GroupName.config", ios::in|ios::trunc);
	for (auto it = GroupName.begin(); it != GroupName.end(); it++) {
		if (!(it->second).empty())
			ofstreamGroupName << it->first.first << " " << it->first.second << " " << it->second << endl;
	}
	ofstreamGroupName.close();
	ofstream ofstreamDiscussName(strFileLoc + "DiscussName.config", ios::in | ios::trunc);
	for (auto it = DiscussName.begin(); it != DiscussName.end(); it++) {
		if (!(it->second).empty())
			ofstreamDiscussName << it->first.first << " " << it->first.second << " " << it->second << endl;
	}
	ofstreamDiscussName.close();

	ofstream ofstreamDisabledGroup(strFileLoc + "DisabledGroup.config", ios::in | ios::trunc);
	for (auto it = DisabledGroup.begin(); it != DisabledGroup.end(); it++) {
		ofstreamDisabledGroup << *it << endl;
	}
	ofstreamDisabledGroup.close();

	ofstream ofstreamDisabledDiscuss(strFileLoc + "DisabledDiscuss.config", ios::in | ios::trunc);
	for (auto it = DisabledDiscuss.begin(); it != DisabledDiscuss.end(); it++) {
		ofstreamDisabledDiscuss << *it << endl;
	}
	ofstreamDisabledDiscuss.close();
	return 0;
}

MUST_AppInfo_RETURN(CQAPPID)

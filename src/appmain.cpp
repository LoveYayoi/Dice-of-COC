#include <string>

#include "CQSDK/CQconstant.h"
#include "CQSDK/CQAPI_EX.h"
#include "CQSDK/CQEVE_GroupMsg.h"
#include "CQSDK/CQEVE_PrivateMsg.h"
#include "CQSDK/CQLogger.h"
#include "CQSDK/CQTools.h"
#include "APPINFO.h"
#include "RollDice.h"
#include <windows.h>

using namespace std;
using namespace CQ;

static string hlpmsg = "ʹ�÷���:\n\t��ͨ����: .o(��.rd) �������� ԭ��(��ѡ)\n\t�����������COC�����߰�����������DNDӢ�����ɣ�����Ϊ.o(��.rd) COC6��.o(��.rd) COC7��.o(��.rd) DND\n\t������ʹ��.h����.o(��.rd)\n\tֱ������.o(��.rd)Ϊ����d100Ͷ��\n\t�汾:1.3.5\n\t���ⷴ������ϵQQ1840686745";
static string ErrMsg = "�������!����.o help��.o ��������ð���!";

EVE_GroupMsg_EX(groupMsg) {
	while (eve.message[0] == ' ')eve.message.erase(eve.message.begin());
	string strFirstInput = eve.message.substr(0, eve.message.find(" "));
	//Replace the "��" in the front to "."
	if (strFirstInput.find("��") == 0) {
		strFirstInput.erase(strFirstInput.begin());
		strFirstInput[0] = '.';
	}
	for (auto &i : strFirstInput)if (isupper(i))i = tolower(i);
	if (strFirstInput==".o" || strFirstInput==".h" || strFirstInput == ".rd") {
		//Check if it is a "hidden dice"
		bool isHidden = false;
		if (strFirstInput==".h")isHidden = true;
		//Save the GroupNick. if the GroupNick is empty, save the Nickname
		string strNickName = getGroupMemberInfo(eve.fromGroup, eve.fromQQ).GroupNickname == "" ? getStrangerInfo(eve.fromQQ).nick : getGroupMemberInfo(eve.fromGroup, eve.fromQQ).GroupNickname;
		//Save the group name
		string strGroupName = getGroupList()[eve.fromGroup];
		//Use it to save the HiddenDice Notice
		string strHiddenNotice = strNickName + "������һ�ΰ���";
		//Use the string to save the second part of the Input
		string strSecondInput;
		//Use it to save the count of the Msg have read
		int intMsgCnt = eve.message.find(" ") == string::npos ? eve.message.length() : eve.message.find(" ");
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;//Skip the space
		//Get the Second Input
		strSecondInput = eve.message.substr(intMsgCnt, eve.message.find(" ", intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(" ", intMsgCnt) - intMsgCnt);
		if (strSecondInput == "")strSecondInput = "D100";
		for (auto &i : strSecondInput)if (islower(i))i=toupper(i);
		//Make the intMsgCnt jump to the space after the second input
		intMsgCnt = eve.message.find(" ", intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(" ", intMsgCnt);
		//Save the number or dice before # to get the time of rolling dice
		int intRollDiceTime = 0;
		if (strSecondInput.find("#") != string::npos) {
			if (MainRoll(intRollDiceTime, strSecondInput.substr(0, strSecondInput.find("#"))) == -1 || intRollDiceTime <= 0) {
				sendGroupMsg(eve.fromGroup, ErrMsg);
				eve.message_block();
				return;
			}
		}
		else {
			intRollDiceTime = 1;
		}
			
		//If the input contains # and the input before # is dice, send a roll-dice-time notice
		if (strSecondInput.find("#") != string::npos && (strSecondInput.substr(0, strSecondInput.find("#")).find("d") != string::npos || strSecondInput.substr(0, strSecondInput.find("#")).find("D") != string::npos)) {
			string strRollDiceTurnNotice;
			strRollDiceTurnNotice += "��������: " + strSecondInput.substr(0, strSecondInput.find("#")) + "=" + to_string(intRollDiceTime) + "��";
			if (isHidden)sendPrivateMsg(eve.fromQQ, strRollDiceTurnNotice);else
			sendGroupMsg(eve.fromGroup, strRollDiceTurnNotice);
		}
		//Erase the input # and the char before it
		int erase_cnt = strSecondInput.find("#") == string::npos ? -1 : strSecondInput.find("#");
		for (int i = 0; i <= erase_cnt; ++i) strSecondInput.erase(strSecondInput.begin());
		for (int w = 0; w != intRollDiceTime; ++w) {
			if (strSecondInput == "��������"|| strSecondInput == "��������"|| strSecondInput == "��������"|| strSecondInput == "�߰���������" || strSecondInput == "�߰���������" || strSecondInput == "�߰���������" || strSecondInput == "COC" || strSecondInput == "COC7") {
				string strMAns = strNickName;
				COC7(strMAns);
				if (isHidden) {
					strMAns = "��Ⱥ" + strGroupName + "��" + strMAns;
					sendPrivateMsg(eve.fromQQ, strMAns);
				}
				else
				sendGroupMsg(eve.fromGroup, strMAns);
			}
			else if (strSecondInput == "������������"|| strSecondInput == "������������" || strSecondInput == "������������" || strSecondInput == "COC6") {
				string strMAns = strNickName;
				COC6(strMAns);
				if (isHidden) {
					strMAns = "��Ⱥ" + strGroupName + "��" + strMAns;
					sendPrivateMsg(eve.fromQQ, strMAns);
				}
				else
				sendGroupMsg(eve.fromGroup, strMAns);
			}
			else if (strSecondInput == "����" || strSecondInput == "help") {
				sendGroupMsg(eve.fromGroup, hlpmsg);
			}
			else if (strSecondInput=="DND"||strSecondInput=="Ӣ������"||strSecondInput=="Ӣ������"||strSecondInput=="Ӣ������") {
				string strMAns = strNickName + "��Ӣ������:\n";
				DND(strMAns);
				sendGroupMsg(eve.fromGroup, strMAns);
			}
			else {
				string strAns;//Output string
				strAns = strNickName;
				strAns += "������: " + strSecondInput + "=";
				int mainsum = 0;//Result
				string secondaryRes;
				int intRollRes = MainRoll(mainsum, strSecondInput, strAns, secondaryRes);
				if (intRollRes == -1) {
					sendGroupMsg(eve.fromGroup, ErrMsg);
					eve.message_block();
					return;
				}
				while (eve.message[intMsgCnt] == ' '&&intMsgCnt != eve.message.length())intMsgCnt++;
				string strReason;//Get the reason from the third part of the input
				if (intMsgCnt != eve.message.length())strReason = eve.message.substr(intMsgCnt);
				bool OnlySpace = true;
				for (const auto i : strReason) {
					if (i != ' ') {
						OnlySpace = false;
						break;
					}
				}
				if (!OnlySpace) {
					//Add the reason at the front of the output
					strAns = "����" + strReason + strAns;
				}
				if (strAns.substr(strAns.rfind("=", strAns.length() - 2) + 1, strAns.length() - strAns.rfind("=", strAns.length() - 2) - 2) != secondaryRes) {
					strAns += secondaryRes;
					if (secondaryRes != to_string(mainsum)) {
						strAns += '=';
						strAns += to_string(mainsum);
					}
				}
				else {
					strAns.erase(strAns.end() - 1);
				}
				//Merge all the results to the string "strAns"
				if (isHidden) {
					strAns = "��Ⱥ" + strGroupName + "��" + strAns;
					sendPrivateMsg(eve.fromQQ, strAns);
				}
				else
				sendGroupMsg(eve.fromGroup, strAns);//Output
			}
			Sleep(50);
		}
		if (isHidden) {
			sendGroupMsg(eve.fromGroup, strHiddenNotice);
		}
		eve.message_block();
		return;
	}
	eve.message_ignore();
	return;
}
EVE_PrivateMsg_EX(privateMsg)
{
	logger m("main");
	while (eve.message[0] == ' ')eve.message.erase(eve.message.begin());
	m.Debug("1");
	string strFirstInput = eve.message.substr(0, eve.message.find(" "));
	m.Debug("1");
	//Replace the "��" in the front to "."
	if (strFirstInput.find("��") == 0) {
		strFirstInput.erase(strFirstInput.begin());
		strFirstInput[0] = '.';
	}
	m.Debug("1");
	for (auto &i : strFirstInput)if (isupper(i))i = tolower(i);
	m.Debug("1");
	if (strFirstInput==".o"|| strFirstInput == ".rd") {
		m.Debug("1");
		string strNickName = getStrangerInfo(eve.fromQQ).nick;
		string strSecondInput;
		int intMsgCnt = eve.message.find(" ") == string::npos ? eve.message.length() : eve.message.find(" ");
		while (eve.message[intMsgCnt] == ' ')intMsgCnt++;
		m.Debug("1");
		strSecondInput = eve.message.substr(intMsgCnt, eve.message.find(" ", intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(" ", intMsgCnt) - intMsgCnt);
		if (strSecondInput == "")strSecondInput = "D100";
		m.Debug("1");
		for (auto &i : strSecondInput)if (islower(i))i=toupper(i);
		intMsgCnt = eve.message.find(" ", intMsgCnt) == string::npos ? eve.message.length() : eve.message.find(" ", intMsgCnt);
		int intRollDiceTime = 0;
		m.Debug("1");
		if (strSecondInput.find("#") != string::npos) {
			if (MainRoll(intRollDiceTime, strSecondInput.substr(0, strSecondInput.find("#"))) == -1 || intRollDiceTime <= 0) {
				sendPrivateMsg(eve.fromQQ, ErrMsg);
				eve.message_block();
				return;
			}
		}
		else {
			intRollDiceTime = 1;
		}
		if (strSecondInput.find("#")!=string::npos&&(strSecondInput.substr(0, strSecondInput.find("#")).find("d") != string::npos || strSecondInput.substr(0, strSecondInput.find("#")).find("D") != string::npos)) {
			string strRollDiceTurnNotice;
			strRollDiceTurnNotice += "��������: " + strSecondInput.substr(0, strSecondInput.find("#")) + "=" + to_string(intRollDiceTime) + "��";
			sendPrivateMsg(eve.fromQQ, strRollDiceTurnNotice);
		}
		int erase_cnt = strSecondInput.find("#") == string::npos ? -1 : strSecondInput.find("#");
		for (int i = 0; i <= erase_cnt; ++i) strSecondInput.erase(strSecondInput.begin());
		for (int w = 0; w != intRollDiceTime; ++w) {
			if (strSecondInput == "��������" || strSecondInput == "��������" || strSecondInput == "��������" || strSecondInput == "�߰���������" || strSecondInput == "�߰���������" || strSecondInput == "�߰���������") {
				string strMAns = strNickName;
				COC7(strMAns);
				sendPrivateMsg(eve.fromQQ, strMAns);
			}
			else if (strSecondInput == "������������" || strSecondInput == "������������" || strSecondInput == "������������") {
				string strMAns = strNickName;
				COC6(strMAns);
				sendPrivateMsg(eve.fromQQ, strMAns);
			}
			else if (strSecondInput == "����" || strSecondInput == "help") {
				sendPrivateMsg(eve.fromQQ, hlpmsg);
			}
			else if (strSecondInput == "DND" || strSecondInput == "Ӣ������" || strSecondInput == "Ӣ������" || strSecondInput == "Ӣ������") {
				string strMAns = strNickName + "��Ӣ������:\n";
				DND(strMAns);
				sendPrivateMsg(eve.fromQQ, strMAns);
			}
			else {
				string strAns;//Output string
				strAns = strNickName;
				strAns += "������: " + strSecondInput + "=";
				int mainsum = 0;//Result
				string secondaryRes;
				int intRollRes = MainRoll(mainsum, strSecondInput, strAns,secondaryRes );
				if (intRollRes == -1) {
					sendPrivateMsg(eve.fromQQ, ErrMsg);
					eve.message_block();
					return;
				}
				while (eve.message[intMsgCnt] == ' '&&intMsgCnt != eve.message.length())intMsgCnt++;
				string strReason;//Get the reason from the third part of the input
				if (intMsgCnt != eve.message.length())strReason = eve.message.substr(intMsgCnt);
				bool OnlySpace = true;
				for (const auto i : strReason) {
					if (i != ' ') {
						OnlySpace = false;
						break;
					}
				}
				if (!OnlySpace) {
					//Add the reason at the front of the output
					strAns = "����" + strReason + strAns;
				}
				if (strAns.substr(strAns.rfind("=", strAns.length() - 2) + 1, strAns.length() - strAns.rfind("=", strAns.length() - 2) - 2) != secondaryRes) {
					strAns += secondaryRes;
					if (secondaryRes != to_string(mainsum)) {
						strAns += '=';
						strAns += to_string(mainsum);
					}
				}
				else {
					strAns.erase(strAns.end() - 1);
				}//Merge all the results to the string "strAns"
				sendPrivateMsg(eve.fromQQ, strAns);//Output
			}
			Sleep(50);
		}
		eve.message_block();
		return;
	}
	eve.message_ignore();
	return;

}

EVE_Menu(__menuA)
{
	MessageBoxA(nullptr, hlpmsg.c_str(), "Help", MB_OK | MB_ICONINFORMATION);
	return 0;
}

MUST_AppInfo_RETURN(CQAPPID)

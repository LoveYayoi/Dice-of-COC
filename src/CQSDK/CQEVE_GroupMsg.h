#pragma once
//#include <exception>
#include "CQEVE.h"//����ɾ������...
#include "CQEVEMsg.h"

#include <vector>
/*
Ⱥ��Ϣ(Type=2)
���ӳ�����ڿ�Q���̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)

subType �����ͣ�Ŀǰ�̶�Ϊ1
msgId ��ϢID
fromGroup ��ԴȺ��
fromQQ ��ԴQQ��
fromAnonymous ��Դ������
msg ��Ϣ����
font ����

�����Ϣ����������,isAnonymous() ���� true, ��ʹ�� getFromAnonymousInfo() ��ȡ ��������Ϣ

���ӳ�����ڿ�Q���̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)
�������ʹ���»��߿�ͷ��Ҫ�ĳ�˫�»���
���ط���ֵ,��Ϣ��������,������Ȳ�������
*/
#define EVE_GroupMsg_EX(Name) 																	\
	void Name(CQ::EVEGroupMsg & eve);															\
	EVE_GroupMsg(Name)																		\
	{																							\
		CQ::EVEGroupMsg tep(subType, msgId, fromGroup, fromQQ, fromAnonymous, msg, font);	\
		Name(tep);																				\
		return tep._EVEret;																		\
	}																							\
	void Name(CQ::EVEGroupMsg & eve)

namespace CQ{
	class GroupMemberInfo;

	// Ⱥ������Ϣ
	struct AnonymousInfo
	{
		long long AID;
		std::string AnonymousName;
		//std::vector<unsigned char> Token;

		AnonymousInfo(const char* msg);
		AnonymousInfo() = default;
	};

	//Ⱥ�¼�
    struct EVEGroupMsg :public EVEMsg
    {
	private:
		AnonymousInfo* fromAnonymousInfo;
	public:
        //Ⱥ��
        long long fromGroup;
        //�����õ�����
        const char* fromAnonymousToken;
		EVEGroupMsg(int subType, int msgId, long long fromGroup, long long fromQQ, const char* fromAnonymous, const char* msg, int Font);

		virtual ~EVEGroupMsg();
		
		bool isAnonymous() const;

		// ͨ�� EVEMsg �̳�
		 int sendMsg(const char *) const override;
		 int sendMsg(std::string) const override;
		 msg sendMsg() const override;

		//��ȡ��������Ϣ
		AnonymousInfo&getFromAnonymousInfo() /*throw(std::exception_ptr)*/;

		//��ȺԱ�Ƴ�
		bool setGroupKick(bool refusedAddAgain = false);
		//��ȺԱ����
		//�Զ��ж��Ƿ�������
		bool setGroupBan(long long banTime = 60);
		//��Ⱥ����Ա
		bool setGroupAdmin(bool isAdmin);
		//��Ⱥ��Աר��ͷ��
		bool setGroupSpecialTitle(std::string UniqueRank, long long ExpireTime = -1);

		//��ȫȺ����
		bool setGroupWholeBan(bool WholeBan = true);
		//��Ⱥ��������
		bool setGroupAnonymous(bool Anonymous);
		//��Ⱥ��Ա��Ƭ
		bool setGroupCard(std::string NewGroupNickname);
		//��Ⱥ�˳�
		bool setGroupLeave(bool Dismiss);
		//ȡȺ��Ա��Ϣ (֧�ֻ���)
		GroupMemberInfo getGroupMemberInfo(bool DisableCache = false);
		//ȡȺ��Ա�б�
		std::vector<GroupMemberInfo> getGroupMemberList();

	};
}

#pragma once

#include "CQconstant.h"
namespace CQ {
	// �¼�����
	struct EVE
	{
		//���ر�����Ϣ
		void message_block();
		

		//������Ϣ���κζ���
		//���֮ǰ��������Ϣ,���ｫ���·��б�����Ϣ
		void message_ignore();

		int _EVEret = ��Ϣ_����;

		virtual ~EVE() {}
	};
}
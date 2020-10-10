#pragma once

#include "Ticket.h"

//!!!!!����������!!!!!!!

//����� ���� ������ ��� ���������� ��������� � ������������� ��� � ��������� ����

	//���������� ������ ������������ �������������� ���������

struct Evaluator {
	Rational* list;   //������������ � �������� ���������� �����
	unsigned list_size;
	const Rational* data;
	struct OpersConfig opers_config;
};

//�����������
void* init_Evaluator(struct Evaluator *eval, unsigned size, const Rational*_data, struct OpersConfig *_opers_config);
//����������, ��������� ����������� ���������� list
void destroy_Evaluator(Rational *list);

//���������������� ���������� ���������. ���������. 
//��� ������� �� ���� ���������� INF, ��� ������������� ������������� ���������� - (-1)
Rational Evaluator_evaluate(struct Evaluator eval);

//������ ������� �������������� ���������
Rational Evaluator_evaluate_honestly(struct Evaluator eval);
#pragma once

#include "Ticket.h"



//Permutator ���������� ��������� ���������� � ��������


struct Permutator {
	struct OpersConfig *opers_config;
	unsigned _WORKING_OPERATORS;
};

//��� ������� �������� ������������ ��� 5 ������
const unsigned NORMAL_EVALUATION = OPERATORS_COUNT;
//���� ����� ������� ����������� ������� ��� ������� �������������� � OPERATORS_SIZE  = NO_DIVISION
const unsigned NO_DIVISION = 4u;
//���� ����� ������� ������ �� ������ � �������
const unsigned ONLY_SUM = 3u;



/*������������� ������� ���������� � ���� [ 1, 2, 3, 4, 5, 6...]
�������� ������������� ����������� ���������������� ��������
(������ ��� �������� ����������� �������� ����� begin).
��� ����� ����� ������������� ������������� ������������������
*/


//����� ������� ������� ���������� ������������� �������. 
void Permutator_init_opers(struct Permutator p);

//������ �������������� �������
void Permutator_reinit_signs(struct Permutator p);
void Permutator_reinit_pos(struct Permutator p);

//......................................................
//��������� "a + b + c" � "(a + b) + c" ��������� ���� �����. ��������� ��� ������������ ��. ���� � �����������.

//��������� ��� ������ ������� �� �������� ������� ������ ������� 
bool Permutator_is_doubled(const struct Permutator p);

//c������ ��� ��������� ������������ ������ �������, �.� ��� �������� �� 0 �� OPERATOR_COUNT
bool Permutator_are_signs_valid(const struct Permutator p);

//���������� � opers.sign ��������� ������������, �� ��������� �� ����������
void Permutator_next_sign_configuration(struct Permutator p);

//�������� ������� ������� �� ����������
bool Permutator_are_poses_valid(const struct Permutator p);

//���������� � opers ���������� ��������� ������������ ������� ��� �������� �� �������!
void Permutator_next_operators_permutation(struct Permutator p);

//�������� ��������� ������(�����������) ����������(�������������) ������������ ������� ����������. ���������� true, ���� �������.
bool Permutator_min_unique_pos(struct Permutator p);
//������������ ������� �����.������ �� ��������� ��������� �������.(��� �������� �� �������)
void Permutator_last_pos_configuration(struct Permutator p);

//�������� �������� � opers ��������� ������������ � ��������� �� �������!
bool Permuator_next_operators_configuration(struct Permutator p);
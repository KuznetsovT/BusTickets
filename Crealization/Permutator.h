#pragma once

#include "Ticket.h"



//Permutator ���������� ��������� ���������� � ��������


struct Permutator {
	struct OpersConfig opers_config;
	unsigned _WORKING_OPERATORS;
};

//��� ������� �������� ������������ ��� 5 ������
extern const unsigned NORMAL_EVALUATION;
//���� ����� ������� ����������� ������� ��� ������� �������������� � OPERATORS_SIZE  = NO_DIVISION
extern const unsigned NO_DIVISION;
//���� ����� ������� ������ �� ������ � �������
extern const unsigned ONLY_SUM;



/*������������� ������� ���������� � ���� [ 1, 2, 3, 4, 5, 6...]
�������� ������������� ����������� ���������������� ��������
(������ ��� �������� ����������� �������� ����� begin).
��� ����� ����� ������������� ������������� ������������������
*/


//����� ������� ������� ���������� ������������� �������. 
void Permutator_init_opers(struct OpersConfig op);

//������ �������������� �������
void Permutator_reinit_signs(struct OpersConfig conf);
void Permutator_reinit_pos(struct OpersConfig conf);

//......................................................
//��������� "a + b + c" � "(a + b) + c" ��������� ���� �����. ��������� ��� ������������ ��. ���� � �����������.

//��������� ��� ������ ������� �� �������� ������� ������ ������� 
bool Permutator_is_doubled(const struct OpersConfig conf);

//c������ ��� ��������� ������������ ������ �������, �.� ��� �������� �� 0 �� OPERATOR_COUNT
bool Permutator_are_signs_valid(const struct Permutator p);

//���������� � opers.sign ��������� ������������, �� ��������� �� ����������
void Permutator_next_sign_configuration(struct Permutator p);

//�������� ������� ������� �� ����������
bool Permutator_are_poses_valid(const struct OpersConfig conf);

//���������� � opers ���������� ��������� ������������ ������� ��� �������� �� �������!
void Permutator_next_operators_permutation(struct OpersConfig conf);

//�������� ��������� ������(�����������) ����������(�������������) ������������ ������� ����������. ���������� true, ���� �������.
bool Permutator_min_unique_pos(struct OpersConfig conf);
//������������ ������� �����.������ �� ��������� ��������� �������.(��� �������� �� �������)
void Permutator_last_pos_configuration(struct OpersConfig conf);

//�������� �������� � opers ��������� ������������ � ��������� �� �������!
bool Permuator_next_operators_configuration(struct OpersConfig conf);

//������� ����������� ������
void Permutator_print_opers(struct OpersConfig conf);

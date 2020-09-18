#pragma once

#include <string>


#include <ostream>

#include "Rational.h"



/*
������ �� ������� ����������� �������� ����������� � ���������:

	��� ����� n �����, ��������� ����� ���� +-/*() ����� �������� goal.
	������������ ����� �����������.
	
	����������� ����� �� 6 ����� �� 0 �� 10 �������� 100.
	������:  9 9 9 9 9 9 = 100  -> (9/9+9)*(9/9+9) = 100

������ ������ ������ ����� ��������� �������� �������� �������.
	� ��� ������ "a + b" ������� "a b +", �� ���� �������� ��� ����� ���� ���������

������ ����������� ������������ ����� goal, 
	������� ��� ������� �������� ������������� ���� 
	�������� ������ invalid_argument.

���������� ��� n = 1 �������� invalid_argument 
*/



class TicketsSolver {
private:
	//��� - �������� ������� ��� ���� ����������, �������� { return a+b; }
	template<class T> using func = T(*)(const T&, const T&);
	//��������� ��� ����������� ����������� ������� � str
	struct str_token;
public:
	typedef const func<str_token> FLAG[];

	//����������� ������ ���� ���� ����� ������� ��������� � �������� �������� �������
	const static FLAG REVERSED_NOTATION;

	//����������� ������ ���� ���� ����� ������� ��������� � ���������� �������. ����� ��-���������.
	const static FLAG NORMAL_NOTATION;

	//����������� ������ TicketsSolver. �������� ��������! goal >= 0 � n > 1 !
	TicketsSolver(size_t n, /*unsigned*/ Rational goal, const unsigned *data);
	
	virtual ~TicketsSolver();
	
	//���������� ������������, ���������� ������ ��������� �������, ���� ������� ���, ���������� ������ ������
	std::string first_solution(FLAG notation = NORMAL_NOTATION) noexcept;
	//���������� ��������� �������, ������ ������������ ������������, ���� ������� ���, ���������� ������ ������
	std::string next_solution(FLAG notation = NORMAL_NOTATION) noexcept;
	
	void all_solutions(std::ostream& out, FLAG notation = NORMAL_NOTATION)noexcept;

private:
	/*
	* ����� �� ����������� ������� �����
	* ��� �������� ���������� ������ �� 5 ����������
	* ����������, ��� goal >= 0, ������� ������������� �������� ������ ��� ��������� �� �����.
	*/

	typedef unsigned OPERATORS;
	static const OPERATORS
		PLUS = 0u,    //id: 0
		MINUS = 1u,
		MINUS_PLUS = 2u,  //����������� ����: -(a) + b
		MULTIPLE = 3u,
		DIVIDE = 4u;     //id: 4
	static const unsigned OPERATORS_COUNT = 5u;
	//��������� ��� op < MULTIPLE
	static bool IS_SUMMARY(const OPERATORS op) noexcept;
	//��������� ��� op >= MULTIPLE
	static bool IS_MULTIPLE(const OPERATORS op) noexcept;


	/*
	*����� � ��� ������ ���� (n-1) ����������.
	* � ������� ��������� ������ ���� id ������� �������� � sign � �������, ������� �������� � pos.
	* ������� - ��� ����� �����, ����� �������� ���� �������� ���������, ��������:
	* "a b � + ..." - ������� ��������� "+" ����� 2,
	*��� ��� �� ����� ����� �������� �����, � ��������� ���������� � ����
	*/

	//��������� ����������� ��������
	struct token {
		OPERATORS sign;
		size_t pos;
	};

	const size_t size, opers_size; //opers_size = size-1
	Rational goal;
	Rational* data;

	//������ ������� ����������� ����������� ������ � ���������
	token* opers;

	//������ ������ data � ������� ����� ��������
	void init_data(const unsigned* int_data);

	//����� ������� ������� ���������� ������������� �������
	void init_opers() noexcept;

	//��� ������� �������� ��������� ��������������� �������
	void reinit_signs(size_t begin, size_t end) noexcept;
	void reinit_pos(size_t begin, size_t end, const size_t min_value = 0u) noexcept;


	//c������ ��� ��������� ������������ ������ �������, �.� ��� �������� �� 0 �� OPERATOR_COUNT
	bool are_signs_valid() const noexcept;
	//���������� � opers.sign ��������� ������������, �� ��������� �� ����������
	void next_sign_configuration() noexcept;

	//�������� ������� ������� �� ����������
	bool are_poses_valid()const noexcept;

	//���������� � signs ��������� ������������ ������� � ��������� �� �������!
	void next_operators_permutation() noexcept;

	//��������� ��� ������ ������� �� �������� ������� ������ �������
	virtual bool is_doubled() const noexcept;

	//��������������� ������� ��� is_doubled
	bool CheckForSamePos(const token* i, const token* j) const noexcept;
	bool CheckForNeighbourPos(const token* i, const token* j) const noexcept;


///////////////////////////////////////////////////////////////////////////////////////////////
	//����� ���� ������ ��� ���������� ��������� � ������������� ��� � ��������� ����
	
	

	//���������������� ��������� ��� ���� ������ ��� ����������
	static const func<Rational> rational_lib[];


	//eval_list - ��� ���� �� ������� ����� ����������� ��� ������ ��� ����������
	Rational* eval_list;
	//������� ���������� ������ ���������� ���� ���������� ������� �� 0 ���������� INF
	Rational evaluate() const noexcept; //���� ������������� ��������� ������������� - ���������� -1

	//�������� �� data � eval_list
	void init_list() const noexcept;
	
	//���������� ����� eval_list
	void move(Rational *a, Rational* const _begin) const noexcept;

///////////////////////////////////////////////////////////////////////////////////////////////

	//���������� ���������� �������������
	//�������� ��������� ������ ����� �� ��� � ��� ��������� �����.

	//� ������� ��������� ���� ���� id
	typedef unsigned str_id;

	static const str_id
		NUM = 0u,
		SUM = 1u,
		MULT = 2u,
		DIV = 3u,
		EXPR = 4u;

	struct str_token {
		std::string str;
		str_id id = NUM;
	};

	//������ �� ������� ����� ����������� ��������� �������� ������
	str_token* str_eval_list;

	//����� ����������� ��������� ������������� ����� ������������
	std::string str_evaluate(const FLAG notation = NORMAL_NOTATION) const noexcept;

	//������ ��������� ������������� �����
	void init_str_list() const noexcept;

	//���������� ����� eval_list
	void move(str_token* a, str_token* const _begin) const noexcept;
};




///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


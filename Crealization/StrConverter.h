#pragma once

#include "Ticket.h"


//����� ���� ������ ��� ���������� ��������� � ������������� ��� � ��������� ����

	//���������� ������ ������������ �������������� ���������

struct StrConverter {
	struct str_token* list;
	unsigned list_size;
	const struct str_token* str_data;
	struct OpersConfig opers_config;
};





//������ ������ �� ��������� �������������� ��������
struct str_token* static_StrConverter_init_str_data(const unsigned* data, unsigned size);

//�����������
void* init_Str_Converter(struct StrConverter* sconv, unsigned size, const struct str_token*str_data, struct OpersConfig _opers_config);

//����������, ��������� ����������� ���������� list
void destroy_StrConverter(struct str_token* str_list);

//����������� ��������������� ��������� � ������ � ���������� �������
char* StrConverter_normal_convert(struct StrConverter sconv);

//����������� ��������������� ��������� � ������ � �������� �������
char* StrConverter_reversed_convert(struct StrConverter sconv);

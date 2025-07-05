#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Constants.h"


//����������� ��� "�����"
struct Dish
{
	char* name = (char*)malloc(STRING_SIZE * sizeof(char));//�������� �����
	unsigned int weight;//��������� (�)
	float callories; //������������ (����)
	float protein; //����� (�)
	float fats; //���� (�)
	float carbs; //�������� (�)
	bool IsGluten; //�����������
	bool IsLactose;  //�����������


};

struct TypeOfDish
{
	char* typeName = (char*)malloc(STRING_SIZE * sizeof(char)); //�������� �����
	struct Dish* dishes = (struct Dish*)malloc(sizeof(struct Dish)); //�����
	int count; //���������� ���� � ���� ����
};

struct NutritionTarget
{
	float calories; //�������
	float protein; //�����
	float fats; //����
	float carbs; //��������
};

enum MealType
{
	FIRST_BREAKFAST,
	SECOND_BREAKFAST,
	LUNCH,
	SUPPER,
	DINNER
};

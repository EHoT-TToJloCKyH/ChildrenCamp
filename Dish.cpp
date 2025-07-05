#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Constants.h"


//структурный тип "блюдо"
struct Dish
{
	char* name = (char*)malloc(STRING_SIZE * sizeof(char));//название блюда
	unsigned int weight;//граммовка (г)
	float callories; //калорийность (ккал)
	float protein; //белки (г)
	float fats; //жиры (г)
	float carbs; //углеводы (г)
	bool IsGluten; //глютенность
	bool IsLactose;  //лактозность


};

struct TypeOfDish
{
	char* typeName = (char*)malloc(STRING_SIZE * sizeof(char)); //название блюда
	struct Dish* dishes = (struct Dish*)malloc(sizeof(struct Dish)); //блюда
	int count; //количество блюд в этом типе
};

struct NutritionTarget
{
	float calories; //калории
	float protein; //белки
	float fats; //жиры
	float carbs; //углеводы
};

enum MealType
{
	FIRST_BREAKFAST,
	SECOND_BREAKFAST,
	LUNCH,
	SUPPER,
	DINNER
};

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "FunctionsFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "Constants.h"
#include "Dish.cpp"

/// <summary>
/// Чтение блюд из файла и запись в имеющуюся структуру данных
/// </summary>
/// <param name="filename">Имя файла</param>
/// <param name="types">Типы блюд</param>
/// <param name="typeCount">Кол-во типов блюд</param>
void readDishesFromFile(const char* filename, struct TypeOfDish** types, int* typeCount)
{
	setlocale(LC_ALL, "rus");
	FILE* file = fopen(filename, "r");
	if(file == NULL)
	{
		printf("Ошибка открытия файла\n");
		return;
	}

	char line[STRING_SIZE * 3];
	*typeCount = 0;
	*types = (struct TypeOfDish*)malloc(MAX_TYPES * sizeof(struct TypeOfDish));

	while (fgets(line, sizeof(line), file))
	{
		//удаляем символ новой строки
		line[strcspn(line, "\n")] = '\0';

		//проверяем, является ли строка названием типа блюда (не содержит ';')
		if (strchr(line, ';') == NULL)
		{
			//это новый тип блюда
			if (*typeCount >= MAX_TYPES)
			{
				printf("Too many dish types\n");
				break;
			}

			(*types)[*typeCount].typeName = (char*)malloc(STRING_SIZE * sizeof(char));
			strcpy((*types)[*typeCount].typeName, line);
			(*types)[*typeCount].dishes = (struct Dish*)malloc(MAX_DISHES * sizeof(struct Dish));
			(*types)[*typeCount].count = 0;
			(*typeCount)++;
		}
		else
		{
			// Это блюдо, парсим его данные
			if (*typeCount == 0)
			{
				printf("Error: Dish found before any type\n");
				continue;
			}

			struct TypeOfDish* currentType = &(*types)[*typeCount - 1];
			if (currentType->count >= MAX_DISHES)
			{
				printf("Too many dishes in type %s\n", currentType->typeName);
				continue;
			}

			struct Dish* currentDish = &currentType->dishes[currentType->count];
			currentDish->name = (char*)malloc(STRING_SIZE * sizeof(char));

			char* token = strtok(line, ";");
			if (token) strcpy(currentDish->name, token);

			token = strtok(NULL, ";");
			if (token) currentDish->weight = atoi(token);

			token = strtok(NULL, ";");
			if (token) currentDish->callories = atof(token);

			token = strtok(NULL, ";");
			if (token) currentDish->protein = atof(token);

			token = strtok(NULL, ";");
			if (token) currentDish->fats = atof(token);

			token = strtok(NULL, ";");
			if (token) currentDish->carbs = atof(token);

			token = strtok(NULL, ";");
			if (token) currentDish->IsGluten = (strcmp(token, "true") == 0);

			token = strtok(NULL, ";");
			if (token) currentDish->IsLactose = (strcmp(token, "true") == 0);

			currentType->count++;
		}
	}

	fclose(file);
}
/// <summary>
/// Функция для вывода всех блюд, записанных из файла
/// </summary>
/// <param name="types">Типы блюд</param>
/// <param name="typeCount">Кол-во типов блюд</param>
void printAllDishes(struct TypeOfDish* types, int typeCount)
{
	
	printf("\n=== ALL DISHES ===\n");
	for (int i = 0; i < typeCount; i++)
	{
		printf("\n[%s]\n", types[i].typeName);
		for (int j = 0; j < types[i].count; j++)
		{
			struct Dish d = types[i].dishes[j];
			printf("  %s\n", d.name);
			printf("    Weight: %d g\n", d.weight);
			printf("    Calories: %.2f kcal\n", d.callories);
			printf("    Proteins: %.2f g\n", d.protein);
			printf("    Fats: %.2f g\n", d.fats);
			printf("    Carbs: %.2f g\n", d.carbs);
			printf("    Gluten: %s\n", d.IsGluten ? "yes" : "no");
			printf("    Lactose: %s\n", d.IsLactose ? "yes" : "no");
			printf("\n");
		}
	}
	printf("=================\n");
}
/// <summary>
/// Функция для освобождения памяти от блюд, записанных из файла
/// </summary>
/// <param name="types">Типы блюд</param>
/// <param name="typeCount">Кол-во типов блюд</param>
void freeDishData(struct TypeOfDish* types, int typeCount)
{
	for (int i = 0; i < typeCount; i++)
	{
		free(types[i].typeName);
		for (int j = 0; j < types[i].count; j++)
		{
			free(types[i].dishes[j].name);
		}
		free(types[i].dishes);
	}
	free(types);
}

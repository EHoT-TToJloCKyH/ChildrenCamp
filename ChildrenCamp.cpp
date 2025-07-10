#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "Constants.h"
#include "Dish.cpp"
#include "FunctionsFile.h"
#include <time.h>
#include <math.h>

/*
* ДОБАВИТЬ:
* - АЛЛЕРГИИ
* - НА НЕДЕЛЮ для повторов
* - вывод в txt(если захочется, то в word;D)
*/

/// <summary>
/// Типы меню
/// </summary>
enum MenuType
{
	StartMenu,
	AgeMenu,
	AllergyMenu,
	DietaryModeMenu
};

/// <summary>
/// Возраста
/// </summary>
enum Age
{
	Young7_10,
	Middle11_13,
	Elder14_17
};

/// <summary>
/// Наличие аллергий
/// </summary>
enum Allergy
{
	No,
	Gluten,
	Lactose,
	GlutenAndLactose
};

/// <summary>
/// Режим питания в лагере(полный/неполный)
/// </summary>
enum DietaryMode
{
	Full,
	NotFull
};

/// <summary>
/// Функция, проверяющая строку на числовое значение
/// </summary>
/// <param name="input">Проверяемая строка</param>
/// <returns></returns>
bool isIntNumber(char* input) {
	for (int i = 0; input[i] != '\0'; i++)
	{
		if (isalpha(input[i]))
			return false;
		if (input[i] == '.' || input[i] == ',')
		{
			return false;
		}
		if (i >= 1)
		{
			if (input[i] == '-' || input[i] == '+')
				return false;
		}
	}

	return true;
}


/// <summary>
/// Вывод приветствия
/// </summary>
void Greet()
{
	printf(HELLO_STR);
}
/// <summary>
/// Вывод стартового меню
/// </summary>
void ShowStartMenu()
{
		printf("Что вы хотите сделать?\n");
		printf("1 - Составить меню\n");
		printf("2 - Выход из программы\n");
		//шиза
		//шиза
		//шиза
}
/// <summary>
/// Вывод меню выбора возрастной категории
/// </summary>
void ShowAgeMenu()
{
	printf("Выберите возрастную категорию детей:\n");
	printf("1 - 7-10 лет\n");
	printf("2 - 11-13 лет\n");
	printf("3 - 14-17 лет\n");
	printf("4 - Выход из программы\n");
}
/// <summary>
/// Вывод меню выбора наличия аллергии
/// </summary>
void ShowAllergyMenu()
{
	printf("Известно ли о наличии аллергий/запретов у детей на какие-либо продукты питания?\n");
	printf("1 - Нет\n");
	printf("2 - На глютен\n");
	printf("3 - На лактозу\n");
	printf("4 - На глютен и лактозу\n");

}
/// <summary>
/// Вывод меню типа дня
/// </summary>
void ShowDietaryModeMenu()
{
	printf("Выберите тип дня лагеря\n");
	printf("1 - Полный(круглосуточно)\n");
	printf("2 - Неполный(до обеда)\n");
}

/// <summary>
/// Получение от пользователя пункта какого-либо меню с обработкой некорректного ввода
/// </summary>
/// <param name="menuNumber">Номер выбранного меню</param>
/// <returns>Цифра выбранного пункта меню</returns>
int GetChoice(int menuNumber)
{
	char* input = (char*)malloc(STRING_SIZE * sizeof(char));
	do
	{
		switch(menuNumber)
		{
		case StartMenu: //начальное меню
			ShowStartMenu();
			break;
		case AgeMenu: //меню возраста
			ShowAgeMenu();
			break;
		case AllergyMenu: //меню аллергий
			ShowAllergyMenu();
			break;
		case DietaryModeMenu: //режим лагеря
			ShowDietaryModeMenu();
			break;
		}
		scanf("%s", input);
		if (!isIntNumber(input))
			printf("Некорректный ввод, попробуйте ещё\n");
	} while (!isIntNumber(input));

	return atoi(input);
}

/// <summary>
/// Функция для проверки, соответствует ли меню целевым значениям калорий (с погрешностью 25%)
/// </summary>
/// <param name="menu">Меню</param>
/// <param name="dishCount">Кол-во блюд</param>
/// <param name="target">Целевые значения калорий</param>
/// <returns>true, если проходит по калориям, иначе false</returns>
bool isMenuValid(const struct Dish* menu, int dishCount, const struct NutritionTarget* target) {
	float totalCalories = 0; //общее число калорий

	for (int i = 0; i < dishCount; i++) {
		totalCalories += menu[i].callories; 
	}

	float tolerance = 0.25f; //25% погрешность
	//true, если проходит по калориям, иначе false
	return fabs(totalCalories - target->calories) <= target->calories * tolerance;
}

/// <summary>
/// Функция для составления меню
/// </summary>
/// <param name="types">Типы блюд</param>
/// <param name="typeCount">Кол-во типов</param>
/// <param name="">Тип приёма пищи</param>
/// <param name="target">Целевые значения калорий выбранного приёма пищи</param>
/// <param name="menu">Меню</param>
/// <param name="menuCount">Кол-во созданных меню</param>
/// <param name="sumTarget">Сумма калорий</param>
void CreateMenu(struct TypeOfDish* types, int typeCount, enum MealType mealType,
	const struct NutritionTarget* target, struct Dish* menu, int* menuCount, struct NutritionTarget* sumTarget) {
	srand((unsigned)time(NULL));
	srand((unsigned)rand());
	int attempts = 0; //попытки
	bool isFoundValidMenu = false; //найдено ли подходящее меню

	//определяем категории блюд для каждого типа приема пищи
	const char* requiredCategories[5][6] = {
		//первый завтрак
		{"Горячее блюдо", "Холодное блюдо", "Горячий напиток", "Хлеб", "Фрукты"},
		//второй завтрак
		{"Перекус", "Фруктобулочки", NULL, NULL, NULL},
		//обед
		{"Обед_Первое_блюдо", "Обед_Второе_блюдо", "Обед_Гарнир", "Обед_Напиток", "Обед_Овощи/Салат/Закуска", "Обед_Сладкое"},
		//полдник
		{"Перекус_Полдник", "Фруктобулочки_Полдник", NULL, NULL, NULL},
		//ужин
		{"Ужин_Горячее_блюдо", "Ужин_Овощи/Салат", "Ужин_Напиток", "Хлеб", NULL}
	};
	//количество обязательных категорий для каждого приёма пищи
	int requiredCounts[5] = { 5, 2, 6, 2, 4 }; 

	while (attempts < MAX_ATTEMPTS && !isFoundValidMenu) {
		*menuCount = 0; //счётчик меню
		float currentCalories = 0; //текущее кол-во калорий

		//выбираем блюда из требуемых категорий
		for (int i = 0; i < requiredCounts[mealType]; i++) {
			const char* category = requiredCategories[mealType][i];
			if (category == NULL) continue;

			//находим тип блюда
			int typeIndex = -1;
			for (int j = 0; j < typeCount; j++) {
				if (strcmp(types[j].typeName, category) == 0) {
					typeIndex = j;
					break;
				}
			}

			if (typeIndex == -1 || types[typeIndex].count == 0) continue;

			//выбираем случайное блюдо из этой категории
			int dishIndex = rand() % types[typeIndex].count;
			struct Dish* dish = &types[typeIndex].dishes[dishIndex];

			//добавляем блюдо в меню
			menu[*menuCount] = *dish;
			(*menuCount)++;

			currentCalories += dish->callories;
			
		}

		//проверяем, соответствует ли меню требованиям по калориям
		if (isMenuValid(menu, *menuCount, target)) {
			isFoundValidMenu = true;
		}
		else
		{
			attempts++;
		}
	}

	if (!isFoundValidMenu) {
		*menuCount = 0;
		printf("Не удалось составить меню, соответствующее требованиям за %d попыток\n", MAX_ATTEMPTS);
	}
	else { //засчитываем в сумму целевых параметров
		(*sumTarget).calories += target->calories;
		(*sumTarget).protein += target->protein;
		(*sumTarget).fats += target->fats;
		(*sumTarget).carbs += target->carbs;
	}

	
}

/// <summary>
/// Функция для вывода меню для выбранного приёма пищи
/// </summary>
/// <param name="menuCount"></param>
/// <param name="menu"></param>
/// <param name="target"></param>
/// <param name="MenuMessage">Сообщение, всплывающее перед выводом меню</param>
/// <param name="sumParams"></param>
void PrintMenu(int* menuCount, struct Dish* menu, const struct NutritionTarget* target,const char* MenuMessage, struct NutritionTarget* sumParams)
{
	if (*menuCount > 0)
	{
		printf(MenuMessage); //выводим сообщение для указанного типа меню
		//общие КБЖУ
		float totalCalories = 0, totalProtein = 0, totalFats = 0, totalCarbs = 0;

		//считаем и выводим общее КБЖУ получившегося меню
		for (int i = 0; i < *menuCount; i++) {
			printf("- %s (%.2f ккал, белки: %.2f г, жиры: %.2f г, углеводы: %.2f г)\n",
				menu[i].name, menu[i].callories, menu[i].protein, menu[i].fats, menu[i].carbs);

			totalCalories += menu[i].callories;
			totalProtein += menu[i].protein;
			totalFats += menu[i].fats;
			totalCarbs += menu[i].carbs;
		}

		//суммируем получившееся КБЖУ
		sumParams->calories += totalCalories;
		sumParams->protein += totalProtein;
		sumParams->fats += totalFats;
		sumParams->carbs += totalCarbs;

		//выводим получившееся КБЖУ с меню на этот приём пищи
		printf("\nИтого: %.2f ккал, белки: %.2f г, жиры: %.2f г, углеводы: %.2f г\n",
			totalCalories, totalProtein, totalFats, totalCarbs);
		//выводим целевое КБЖУ с меню на этот приём пищи
		printf("Цель: %.2f ккал, белки: %.2f г, жиры: %.2f г, углеводы: %.2f г\n",
			(*target).calories, (*target).protein, (*target).fats, (*target).carbs);
	}
	printf("=================================================================================\n");
	menuCount = 0;
}
int main(void)
{
	int choice; //выбранный юзером пункт меню
	enum Age TargetAge; //целевой возраст
	enum Allergy IsAllergy; //есть ли аллергия
	enum DietMode DietaryMode; //тип дня лагеря
	struct TypeOfDish* types = NULL; //типы блюд
	int typeCount = 0; //счётчик типов блюд
	float requiredCalories, requiredProtein, requiredFats, requiredCarbs; //требуемые КБЖУ
	//целевые показатели каждого приёма пищи
	struct NutritionTarget firstBreakfastTarget, secondBreakfastTarget, lunchTarget, supperTarget, dinnerTarget;
	struct Dish menu[10]; //меню
	int menuCount = 0; //счётчик меню
	//итоговая и целевая сумма параметров меню
	struct NutritionTarget* SumParams = (struct NutritionTarget*)malloc(sizeof(struct NutritionTarget)); SumParams->calories = 0; SumParams->protein = 0; SumParams->fats = 0; SumParams->carbs = 0;
	struct NutritionTarget* SumTarget = (struct NutritionTarget*)malloc(sizeof(struct NutritionTarget)); SumTarget->calories = 0; SumTarget->protein = 0; SumTarget->fats = 0; SumTarget->carbs = 0;

	setlocale(LC_ALL, "rus");

	//приветствие
	Greet();

	//меню
	do
	{
		choice = GetChoice(StartMenu);
		switch (choice) //составить меню или выйти
		{
		case 1: //составить меню
			do
			{
				choice = GetChoice(AgeMenu);
				switch (choice) //какой возраст
				{
				case 1: //7-10
					TargetAge = Young7_10;
					break;
				case 2: //11-13
					TargetAge = Middle11_13;
					break;
				case 3: //14-17
					TargetAge = Elder14_17;
					break;
				default:
					printf("Некорректный ввод, попробуйте ещё\n");
					break;
				}
			} while (choice != 1 && choice != 2 && choice != 3);
			do
			{


				choice = GetChoice(AllergyMenu);
				switch (choice) //есть ли аллергии
				{
				case 1: //нет
					IsAllergy = No;
					break;
				case 2: //глютен
					IsAllergy = Gluten;
					break;
				case 3: //лактоза
					IsAllergy = Lactose;
					break;
				case 4: //глютен и лактоза
					IsAllergy = GlutenAndLactose;
					break;
				default:
					printf("Некорректный ввод, попробуйте ещё\n");
					break;
				}
			} while (choice != 1 && choice != 2);
			do
			{
				choice = GetChoice(DietaryModeMenu);
				switch (choice) //какой режим лагеря
				{
				case 1: //полный режим лагеря
					DietaryMode = Full;
					if (TargetAge == Young7_10)
					{
						//расчёт необходимого КБЖУ по полученным данным
						requiredCalories = TARGET_CALLORIES7_10;
						requiredProtein = TARGET_PROTEIN7_10;
						requiredFats = TARGET_FATS7_10;
						requiredCarbs = TARGET_CARBS7_10;
					}
					else if (TargetAge == Middle11_13)
					{
						//расчёт необходимого КБЖУ по полученным данным
						requiredCalories = TARGET_CALLORIES11_13;
						requiredProtein = TARGET_PROTEIN11_13;
						requiredFats = TARGET_FATS11_13;
						requiredCarbs = TARGET_CARBS11_13;
					}
					else
					{
						//расчёт необходимого КБЖУ по полученным данным
						requiredCalories = TARGET_CALLORIES14_17;
						requiredProtein = TARGET_PROTEIN14_17;
						requiredFats = TARGET_FATS14_17;
						requiredCarbs = TARGET_CARBS14_17;
					}
					break;
				case 2: //неполный режим лагеря
					DietaryMode = NotFull;
					if (TargetAge == Young7_10)
					{
						//расчёт необходимого КБЖУ по полученным данным
						requiredCalories = TARGET_CALLORIES7_10 * PERCENT_OF_CAL_DUR_NOT_FULL;
						requiredFats = TARGET_FATS7_10 * PERCENT_OF_CAL_DUR_NOT_FULL;
						requiredProtein = TARGET_PROTEIN7_10 * PERCENT_OF_CAL_DUR_NOT_FULL;
						requiredCarbs = TARGET_CARBS7_10 * PERCENT_OF_CAL_DUR_NOT_FULL;
					}
					else if (TargetAge == Middle11_13)
					{
						//расчёт необходимого КБЖУ по полученным данным
						requiredCalories = TARGET_CALLORIES11_13 * PERCENT_OF_CAL_DUR_NOT_FULL;
						requiredFats = TARGET_FATS11_13 * PERCENT_OF_CAL_DUR_NOT_FULL;
						requiredProtein = TARGET_PROTEIN11_13 * PERCENT_OF_CAL_DUR_NOT_FULL;
						requiredCarbs = TARGET_CARBS11_13 * PERCENT_OF_CAL_DUR_NOT_FULL;
					}
					else
					{
						//расчёт необходимого КБЖУ по полученным данным
						requiredCalories = TARGET_CALLORIES14_17 * PERCENT_OF_CAL_DUR_NOT_FULL;
						requiredFats = TARGET_FATS14_17 * PERCENT_OF_CAL_DUR_NOT_FULL;
						requiredProtein = TARGET_PROTEIN14_17 * PERCENT_OF_CAL_DUR_NOT_FULL;
						requiredCarbs = TARGET_CARBS14_17 * PERCENT_OF_CAL_DUR_NOT_FULL;
					}
					break;
				default:
					printf("Некорректный ввод, попробуйте ещё\n");
					break;
				}
			} while (choice != 1 && choice != 2);

			//расчёт калорий, белков, жиров и углеводов на каждый приём пищи с учётом раннее введённых параметров
			//первый завтрак
			firstBreakfastTarget.calories = requiredCalories * FIR_BREAKFAST_CAL_PERCENT;
			firstBreakfastTarget.protein = requiredProtein * FIR_BREAKFAST_CAL_PERCENT;
			firstBreakfastTarget.fats = requiredFats * FIR_BREAKFAST_CAL_PERCENT;
			firstBreakfastTarget.carbs = requiredCarbs * FIR_BREAKFAST_CAL_PERCENT;
			//второй завтрак
			secondBreakfastTarget.calories = requiredCalories * SEC_BREAKFAST_CAL_PERCENT;
			secondBreakfastTarget.protein = requiredProtein * SEC_BREAKFAST_CAL_PERCENT;
			secondBreakfastTarget.fats = requiredFats * SEC_BREAKFAST_CAL_PERCENT;
			secondBreakfastTarget.carbs = requiredCarbs * SEC_BREAKFAST_CAL_PERCENT;
			//обед
			lunchTarget.calories = requiredCalories * LUNCH_CAL_PERCENT;
			lunchTarget.protein = requiredProtein * LUNCH_CAL_PERCENT;
			lunchTarget.fats = requiredFats * LUNCH_CAL_PERCENT;
			lunchTarget.carbs = requiredCarbs * LUNCH_CAL_PERCENT;
			
			
			//получение списка блюд из файла по категориям
			readDishesFromFile("DishBase.txt", &types, &typeCount);
			//создаём меню первого завтрака
			CreateMenu(types, typeCount, FIRST_BREAKFAST, &firstBreakfastTarget, menu, &menuCount,SumTarget);
			//выводим меню первого завтрака
			PrintMenu(&menuCount, menu, &firstBreakfastTarget, STR_MENU_FIR_BREAKFAST, SumParams);
			//создаём меню второго завтрака
			CreateMenu(types, typeCount, SECOND_BREAKFAST, &secondBreakfastTarget, menu, &menuCount, SumTarget);
			//выводим меню второго завтрака
			PrintMenu(&menuCount, menu, &secondBreakfastTarget, STR_MENU_SEC_BREAKFAST, SumParams);
			//создаём меню обеда
			CreateMenu(types, typeCount, LUNCH, &lunchTarget, menu, &menuCount, SumTarget);
			//выводим меню обеда
			PrintMenu(&menuCount, menu, &lunchTarget, STR_MENU_LUNCH, SumParams);
			
			if (DietaryMode == Full) //если полный день
			{
				//полдник
				supperTarget.calories = requiredCalories * SUPPER_CAL_PERCENT;
				supperTarget.protein = requiredProtein * SUPPER_CAL_PERCENT;
				supperTarget.fats = requiredFats * SUPPER_CAL_PERCENT;
				supperTarget.carbs = requiredCarbs * SUPPER_CAL_PERCENT;
				//ужин
				dinnerTarget.calories = requiredCalories * DINNER_CAL_PERCENT;
				dinnerTarget.protein = requiredProtein * DINNER_CAL_PERCENT;
				dinnerTarget.fats = requiredFats * DINNER_CAL_PERCENT;
				dinnerTarget.carbs = requiredCarbs * DINNER_CAL_PERCENT;

				//создаём меню полдника
				CreateMenu(types, typeCount, SUPPER, &supperTarget, menu, &menuCount, SumTarget);
				//выводим меню полдника
				PrintMenu(&menuCount, menu, &supperTarget, STR_MENU_SUPPER, SumParams);
				//создаём меню ужина
				CreateMenu(types, typeCount, DINNER, &dinnerTarget, menu, &menuCount, SumTarget);
				//выводим меню ужина
				PrintMenu(&menuCount, menu, &dinnerTarget, STR_MENU_DINNER, SumParams);
			}
			//выводим общий итог и цель для сравнения
			printf("=================================================================================\n");
			printf("\nОбщий итог: %.2f ккал, белки: %.2f г, жиры: %.2f г, углеводы: %.2f г\n", SumParams->calories, SumParams->protein, SumParams->fats, SumParams->carbs);

			printf("Общая цель: %.2f ккал, белки: %.2f г, жиры: %.2f г, углеводы: %.2f г\n", SumTarget->calories, SumTarget->protein, SumTarget->fats, SumTarget->carbs);

			//освобождаем память
			for (int i = 0; i < typeCount; i++) {
				for (int j = 0; j < types[i].count; j++) {
					free(types[i].dishes[j].name);
				}
				free(types[i].dishes);
				free(types[i].typeName);
			}
			free(types);
			system("pause");
			return 0;
			//printAllDishes(types, typeCount);
			break;
		case 2: //выйти
			printf("Завершение работы программы...\n");
			return 0;
		default:
			printf("Некорректный ввод, попробуйте ещё\n");
			continue;
		}
	} while (choice != 1 && choice != 2);
	return 0;
}
#pragma once
#include <SFML/Graphics.hpp>
#include<iostream>
#include<string>
#include<string.h>
#include "Table.h"
#include<vector>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include<Windows.h>
#include<conio.h>

using namespace std;
using namespace sf;

bool menu(RenderWindow& window);

class Button
{
public:
	Button(string, int xR, int yR, int xT, int yT);
	void DrawButton(RenderWindow& window);

	RectangleShape rectangle;
	Text getText();
private:

	Text text;
	Font font;
};

class Card
{
public:
	enum suit { PEAKS, CROSS, DIAMONDS, HEARTS }; // Пики, Бубны, Черви, Крести   // Новый вариант Пики, крести, бубны, черви

	enum rank { SIX = 6, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };



	Card(rank r, suit s, bool Face, string F);
	void Flip(); // перевернуть карту 
	int GetValue(); // возвращает значение карты
	int GetSuit(); // возвращает значение масти
	void DrawCard(RenderWindow& window, int x, int y);


	String File;  // файл с расширением
	Image image; // сфмл изображение
	Texture texture; // сфмл текстура
	Sprite sprite; // сфмл спрайт

	Image imageFaceDown;
	Texture textureFaceDown;
	Sprite spriteFaceDown;

private:
	rank m_Rank; // ранг карты
	suit m_Suit; // масть карты
	bool m_IsFaceUp; // Лицом вверх

};

class Hand
{
public:
	Hand(); // Создает изначальный объем вектора
	~Hand();
	virtual void Add(Card* pCard); // Добавляет карты в руку
	void Clear(); // Очищает руку от карт
	void DeleteCard(int aIndex); // Удаляет одну карту из руки
	void DealInputTable(RenderWindow& window, Hand& aTable, int aIndex);	// Добавляет карту на стол
	void SortHand();	// Сортирует карты в руке
	void SeeHand();
	int SizeArray();

	void DrawHand(RenderWindow& window, bool aPlayer);

	int RankCard(int aIndex);	// Возвращает значение карты под указанным индексом
	int SuitCard(int aIndex);	// Возвращает масть карты под указанным индексом

protected:
	vector<Card*> m_Cards; // Вектор карт
};


class GenericPlayer : public Hand
{
public:
	GenericPlayer(const string& name = "");
	virtual ~GenericPlayer();

	enum actionModel { FIRST_MOVE, ATTACK, PROTECTION };

	bool NeedCard(); // Правда, если в руке меньше 6 карт
	int LowCard(int aTrumpSuit); // Возвращает значение карты минимального козыря в руке
	bool NoCards();	// Возвращает истину, если в руки нет карт



protected:
	string m_Name;

};

class Table : public Hand
{
public:
	Table();
	virtual ~Table();
	void DealOutputTable(Hand& aHand);		// Вытаскивает карту в руку игрока
	void AdditionalOutputTable(GenericPlayer& aGenericPlayer);		// Вытаскивает карты в руки до тех пор, пока не будет пустоты на столе
	int SizeTable();		// Возвращает количество карт на столе

	void DrawTable(RenderWindow& window, int size);


	Image Table_image;
	Texture Table_texture;
	Sprite Table_map;
};

class Player : public GenericPlayer
{
public:
	Player(/*const string& name = ""*/);
	~Player();
	void ChoosingName();	// Тестовая функция выбора имени игроку
	int ChoosingCard(RenderWindow& window, Table& aTable, int movement, int aTrumpSuit, Button& aButton, Button& aButtonExit);	// Возвращает индекс карты
	//char AddMoreCard(Table& aTable); // Позволяет подкинуть карту или нажать биту

	void Wins();		// Объявляет победу игрока
	void Lose();		// Объявляет проигрыш игрока
	void Draw();		// Объявляет ничью

	//Button m_Button("Бита", 1350, 730);
};

class Computer : public GenericPlayer {
public:
	Computer(const string& name = "Computer");
	virtual ~Computer();
	int OpponentAction(RenderWindow& window, GenericPlayer& aGenericPlayer, int aIndex, int aTrumpSuit, int aActionModel, Table& aTable);		// Действие соперника

};

class Deck : public Hand
{
public:
	Deck();
	virtual ~Deck();
	void Populate();
	void Shuffle();
	void Deal(Hand& aHand); // Добавить в руку карту из колоды
	void AdditionalCard(GenericPlayer& aGenericPlayer); // Добавляет в руку карту, пока не заполнит ее до 6
	void TrumpSuitCard();	// Определяет козырь
	int GetTrumpSuit();		// Возращает козырную масть 
	void DrawDeck(RenderWindow& window);
	void DrawSuit(RenderWindow& window);

	String File;  // файл с расширением
	Image image_TrumpSuit; // сфмл изображение
	Texture texture_TrumpSuit; // сфмл текстура
	Sprite sprite_TrumpSuit; // сфмл спрайт

private:
	int m_Count;
	int m_TrumpSuit;
};

class Game
{
public:
	Game();
	bool  Play(); // Партия
	void DrawBackground(RenderWindow& window);		// Отрисовка фона
	void gameRunning(Game& aGame);
private:
	Deck m_Deck;
	Computer m_Computer;
	Player m_Player;
	Table m_Table;
};
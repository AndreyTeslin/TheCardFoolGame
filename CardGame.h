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
	enum suit { PEAKS, CROSS, DIAMONDS, HEARTS }; // ����, �����, �����, ������   // ����� ������� ����, ������, �����, �����

	enum rank { SIX = 6, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };



	Card(rank r, suit s, bool Face, string F);
	void Flip(); // ����������� ����� 
	int GetValue(); // ���������� �������� �����
	int GetSuit(); // ���������� �������� �����
	void DrawCard(RenderWindow& window, int x, int y);


	String File;  // ���� � �����������
	Image image; // ���� �����������
	Texture texture; // ���� ��������
	Sprite sprite; // ���� ������

	Image imageFaceDown;
	Texture textureFaceDown;
	Sprite spriteFaceDown;

private:
	rank m_Rank; // ���� �����
	suit m_Suit; // ����� �����
	bool m_IsFaceUp; // ����� �����

};

class Hand
{
public:
	Hand(); // ������� ����������� ����� �������
	~Hand();
	virtual void Add(Card* pCard); // ��������� ����� � ����
	void Clear(); // ������� ���� �� ����
	void DeleteCard(int aIndex); // ������� ���� ����� �� ����
	void DealInputTable(RenderWindow& window, Hand& aTable, int aIndex);	// ��������� ����� �� ����
	void SortHand();	// ��������� ����� � ����
	void SeeHand();
	int SizeArray();

	void DrawHand(RenderWindow& window, bool aPlayer);

	int RankCard(int aIndex);	// ���������� �������� ����� ��� ��������� ��������
	int SuitCard(int aIndex);	// ���������� ����� ����� ��� ��������� ��������

protected:
	vector<Card*> m_Cards; // ������ ����
};


class GenericPlayer : public Hand
{
public:
	GenericPlayer(const string& name = "");
	virtual ~GenericPlayer();

	enum actionModel { FIRST_MOVE, ATTACK, PROTECTION };

	bool NeedCard(); // ������, ���� � ���� ������ 6 ����
	int LowCard(int aTrumpSuit); // ���������� �������� ����� ������������ ������ � ����
	bool NoCards();	// ���������� ������, ���� � ���� ��� ����



protected:
	string m_Name;

};

class Table : public Hand
{
public:
	Table();
	virtual ~Table();
	void DealOutputTable(Hand& aHand);		// ����������� ����� � ���� ������
	void AdditionalOutputTable(GenericPlayer& aGenericPlayer);		// ����������� ����� � ���� �� ��� ���, ���� �� ����� ������� �� �����
	int SizeTable();		// ���������� ���������� ���� �� �����

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
	void ChoosingName();	// �������� ������� ������ ����� ������
	int ChoosingCard(RenderWindow& window, Table& aTable, int movement, int aTrumpSuit, Button& aButton, Button& aButtonExit);	// ���������� ������ �����
	//char AddMoreCard(Table& aTable); // ��������� ��������� ����� ��� ������ ����

	void Wins();		// ��������� ������ ������
	void Lose();		// ��������� �������� ������
	void Draw();		// ��������� �����

	//Button m_Button("����", 1350, 730);
};

class Computer : public GenericPlayer {
public:
	Computer(const string& name = "Computer");
	virtual ~Computer();
	int OpponentAction(RenderWindow& window, GenericPlayer& aGenericPlayer, int aIndex, int aTrumpSuit, int aActionModel, Table& aTable);		// �������� ���������

};

class Deck : public Hand
{
public:
	Deck();
	virtual ~Deck();
	void Populate();
	void Shuffle();
	void Deal(Hand& aHand); // �������� � ���� ����� �� ������
	void AdditionalCard(GenericPlayer& aGenericPlayer); // ��������� � ���� �����, ���� �� �������� �� �� 6
	void TrumpSuitCard();	// ���������� ������
	int GetTrumpSuit();		// ��������� �������� ����� 
	void DrawDeck(RenderWindow& window);
	void DrawSuit(RenderWindow& window);

	String File;  // ���� � �����������
	Image image_TrumpSuit; // ���� �����������
	Texture texture_TrumpSuit; // ���� ��������
	Sprite sprite_TrumpSuit; // ���� ������

private:
	int m_Count;
	int m_TrumpSuit;
};

class Game
{
public:
	Game();
	bool  Play(); // ������
	void DrawBackground(RenderWindow& window);		// ��������� ����
	void gameRunning(Game& aGame);
private:
	Deck m_Deck;
	Computer m_Computer;
	Player m_Player;
	Table m_Table;
};
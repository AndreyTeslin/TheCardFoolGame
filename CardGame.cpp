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
#include "CardGame.h"




using namespace std;
using namespace sf;


Button::Button(string txt, int xR, int yR, int xT, int yT)
{
	rectangle.setSize(Vector2f(200, 100));
	rectangle.setFillColor(Color(116, 71, 48));
	rectangle.setOutlineThickness(2);
	rectangle.setOutlineColor(Color(0, 0, 0));
	rectangle.setPosition(xR, yR);

	font.loadFromFile("CyrilicOld.ttf");

	text.setFont(font);
	text.setString(txt);
	text.setCharacterSize(40);
	text.setPosition(xT, yT);
}

void Button::DrawButton(RenderWindow& window)
{
	window.draw(rectangle);
	window.draw(text);
}

Text Button::getText()
{
	return text;
}



Card::Card(rank r, suit s, bool Face, string F) :
	m_Rank(r),
	m_Suit(s),
	m_IsFaceUp(Face)
{
	File = F; // имя файлы + расширение
	image.loadFromFile("images/" + File + ".png");
	//image.createMaskFromColor(Color(241, 249, 238));
	texture.loadFromImage(image);
	sprite.setTexture(texture);

	// Спрайт карты рубашкой вверх, лицом вниз
	imageFaceDown.loadFromFile("images/38.png");
	textureFaceDown.loadFromImage(imageFaceDown);
	spriteFaceDown.setTexture(textureFaceDown);


}

void Card::DrawCard(RenderWindow& window, int x, int y)
{
	if (m_IsFaceUp == true)
	{
		sprite.setPosition(x, y);
		window.draw(sprite);
	}
	else
	{
		spriteFaceDown.setPosition(x, y);
		window.draw(spriteFaceDown);
	}
}

void Card::Flip()
{
	m_IsFaceUp = !m_IsFaceUp;
}

int Card::GetValue()
{
	return m_Rank;
}

int Card::GetSuit()
{
	return static_cast<unsigned int>(m_Suit);
}

Hand::Hand()
{
	m_Cards.reserve(6);
}

Hand::~Hand()
{
	Clear();
}

void Hand::DrawHand(RenderWindow& window, bool aPlayer)
{
	SortHand();
	int count = 0;
	for (int i = 0; i < m_Cards.size(); i++)
	{
		if (aPlayer == true)
		{
			m_Cards[i]->DrawCard(window, 350 + count, 665);
		}
		else
		{
			m_Cards[i]->Flip();
			m_Cards[i]->spriteFaceDown.setRotation(0);
			m_Cards[i]->DrawCard(window, 350 + count, 20);
			m_Cards[i]->Flip();
		}

		if (m_Cards.size() <= 6)
		{
			count += 159;
		}
		else
		{
			//count = 0;
			count += (955 / m_Cards.size());
		}

	}
}

int Hand::SizeArray()
{
	return m_Cards.size();
}

int Hand::RankCard(int aIndex)
{
	return m_Cards[aIndex]->GetValue();
}

int Hand::SuitCard(int aIndex)
{
	return m_Cards[aIndex]->GetSuit();
}

void Hand::SeeHand()
{
	for (int i = 0; i < m_Cards.size(); i++)
	{
		cout << m_Cards[i]->GetValue() << " " << m_Cards[i]->GetSuit() << endl;
	}
}
void Hand::DealInputTable(RenderWindow& window, Hand& aTable, int aIndex)
{
	if ((!(m_Cards.empty())) && aIndex != -1)
	{
		aTable.Add(m_Cards[aIndex]);
		m_Cards.erase(m_Cards.begin() + aIndex);

	}
}

void Hand::SortHand()
{
	if (m_Cards.size() != 0)
	{
		for (int i = 0; i < m_Cards.size() - 1; i++)
		{
			for (int j = i + 1; j < m_Cards.size(); j++)
			{
				if ((m_Cards[i]->GetValue() > m_Cards[j]->GetValue())) // Тут я добавил сортировку и по масти, но возможно это неверно, поэтому придется убрать
				{
					Card* temp = m_Cards[i];
					m_Cards[i] = m_Cards[j];
					m_Cards[j] = temp;
				}
			}
		}
	}
}


void Hand::DeleteCard(int aIndex)
{
	m_Cards.erase(m_Cards.begin() + aIndex);
}
void Hand::Add(Card* pCard)
{
	m_Cards.push_back(pCard);
}

void Hand::Clear()
{
	vector<Card*>::iterator pIter;
	for (pIter = m_Cards.begin(); pIter < m_Cards.end(); pIter++)
	{
		delete* pIter;
		*pIter = 0;
	}
	m_Cards.clear();
}


GenericPlayer::GenericPlayer(const string& name) :
	m_Name(name)
{}
GenericPlayer::~GenericPlayer()
{}



bool GenericPlayer::NeedCard()
{
	return (m_Cards.size() < 6);	// Тут было != 
}



int GenericPlayer::LowCard(int aTrumpSuit)
{
	int lowTrumpCard = 0;
	SortHand();
	for (int i = 0; i < m_Cards.size(); i++)
	{
		if (m_Cards[i]->GetSuit() == aTrumpSuit)
		{
			lowTrumpCard = i;
			return m_Cards[lowTrumpCard]->GetValue();
		}
	}
	//if(m_Cards[lowTrumpCard]->GetSuit() == aTrumpSuit)
	//{
	//	return m_Cards[lowTrumpCard]->GetValue();
	//}
	//else
	//{
	return 0;
	//}
}


bool GenericPlayer::NoCards()
{
	return m_Cards.empty();
}

Table::Table()
{
	m_Cards.reserve(12);
	Table_image.loadFromFile("images/37.png");
	Table_texture.loadFromImage(Table_image);
	Table_map.setTexture(Table_texture);
}

Table::~Table()
{

}

int Table::SizeTable()
{
	return m_Cards.size();
}

void Table::DrawTable(RenderWindow& window, int size)
{
	int count = 0;
	int width = 21;
	int height = 27;
	for (int i = 0; i < m_Cards.size() - size; i++)
	{
		if (i == 0 || i == 2 || i == 4 || i == 6 || i == 8 || i == 10)
		{
			m_Cards[i]->DrawCard(window, 350 + count, 343);
		}
		else
		{
			m_Cards[i]->DrawCard(window, 350 + count + width, 343 - height);
			count += 170;
		}

		//count += 159;
	}
	if (size != 0)
	{
		for (int i = size; i > 0; --i)
		{
			count += 170;
			m_Cards[m_Cards.size() - size]->DrawCard(window, 350 + count, 343);

		}
	}

}

void Table::DealOutputTable(Hand& aHand)
{
	if (!(m_Cards.empty()))
	{
		aHand.Add(m_Cards[m_Cards.size() - 1]);
		m_Cards.pop_back();
	}
}

void Table::AdditionalOutputTable(GenericPlayer& aGenericPlayer)
{
	while (m_Cards.size() != 0)
	{
		DealOutputTable(aGenericPlayer);
	}
}



Player::Player(/*const string& name*/)
{
	//GenericPlayer(name)
}

Player::~Player()
{}

void Player::Wins()
{
	cout << "Вы победили!" << endl;
}

void Player::Lose()
{
	cout << "Вы проиграли!" << endl;
}

void Player::Draw()
{
	cout << "У вас ничья!" << endl;
}

int Player::ChoosingCard(RenderWindow& window, Table& aTable, int aActionModel, int aTrumpSuit, Button& aButton, Button& aButtonExit)		// Важно в конце отредактировать индексы для пользователя ( Добавить вычитание единицы и поменять условия циклов)
{
	int choice = -1;
	Event event;
	bool cardChosen = false;

	switch (aActionModel)
	{

	case FIRST_MOVE:
		while (window.pollEvent(event))
		{
			if (event.type == Event::MouseMoved)
			{
				Vector2i mousePos = Mouse::getPosition(window);
				if (aButton.rectangle.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					aButton.rectangle.setFillColor(Color(164, 102, 68));
				}
				else
				{
					aButton.rectangle.setFillColor(Color(116, 71, 48));
				}

				if (aButtonExit.rectangle.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					aButtonExit.rectangle.setFillColor(Color(164, 102, 68));
				}
				else
				{
					aButtonExit.rectangle.setFillColor(Color(116, 71, 48));
				}

				for (int i = 0; i < m_Cards.size(); i++)
				{
					if (m_Cards[i]->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
					{
						m_Cards[i]->sprite.setColor(Color{ 195, 195, 195 });
					}
					else
					{
						m_Cards[i]->sprite.setColor(Color::White);
					}
				}
			}

			if (event.type == Event::MouseButtonPressed)
			{
				Vector2i mousePos = Mouse::getPosition(window);

				if (aButtonExit.rectangle.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					window.close();
					return 100;
				}

				for (int i = 0; i < m_Cards.size(); i++)
				{
					if (m_Cards[i]->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
					{
						m_Cards[i]->sprite.setColor(Color::White);
						choice = i;
						cardChosen = true;
						break;

					}
				}

				if (cardChosen) break;

			}
		}
		return choice;

	case ATTACK:
		while (window.pollEvent(event))
		{
			if (event.type == Event::MouseMoved)
			{
				Vector2i mousePos = Mouse::getPosition(window);
				if (aButton.rectangle.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					aButton.rectangle.setFillColor(Color(164, 102, 68));
				}
				else
				{
					aButton.rectangle.setFillColor(Color(116, 71, 48));
				}

				if (aButtonExit.rectangle.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					aButtonExit.rectangle.setFillColor(Color(164, 102, 68));
				}
				else
				{
					aButtonExit.rectangle.setFillColor(Color(116, 71, 48));
				}

				for (int i = 0; i < m_Cards.size(); i++)
				{
					if (m_Cards[i]->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
					{
						m_Cards[i]->sprite.setColor(Color{ 195, 195, 195 });
					}
					else
					{
						m_Cards[i]->sprite.setColor(Color::White);
					}
				}
			}

			if (event.type == Event::MouseButtonPressed)
			{
				Vector2i mousePos = Mouse::getPosition(window);

				if (aButton.rectangle.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					return 99;
				}
				if (aButtonExit.rectangle.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					window.close();
					return 100;
				}

				for (int i = 0; i < m_Cards.size(); i++)
				{
					for (int j = 0; j < aTable.SizeArray(); j++)
					{
						if (m_Cards[i]->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && (m_Cards[i]->GetValue() == aTable.RankCard(j)))
						{
							m_Cards[i]->sprite.setColor(Color::White);
							choice = i;
							cardChosen = true;
							break;

						}
					}
				}

				if (cardChosen) break;

			}
		}
		return choice;

	case PROTECTION:


		while (window.pollEvent(event))
		{
			if (event.type == Event::MouseMoved)
			{
				Vector2i mousePos = Mouse::getPosition(window);
				if (aButton.rectangle.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					aButton.rectangle.setFillColor(Color(164, 102, 68));
				}
				else
				{
					aButton.rectangle.setFillColor(Color(116, 71, 48));
				}

				if (aButtonExit.rectangle.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					aButtonExit.rectangle.setFillColor(Color(164, 102, 68));
				}
				else
				{
					aButtonExit.rectangle.setFillColor(Color(116, 71, 48));
				}

				for (int i = 0; i < m_Cards.size(); i++)
				{
					if (m_Cards[i]->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
					{
						m_Cards[i]->sprite.setColor(Color{ 195, 195, 195 });
					}
					else
					{
						m_Cards[i]->sprite.setColor(Color::White);
					}
				}
			}

			if (event.type == Event::MouseButtonPressed)
			{
				Vector2i mousePos = Mouse::getPosition(window);

				if (aButton.rectangle.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					return 99;
				}

				if (aButtonExit.rectangle.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					window.close();
					return 100;
				}

				for (int i = 0; i < m_Cards.size(); i++)
				{
					if (m_Cards[i]->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && (m_Cards[i]->GetValue() > aTable.RankCard(aTable.SizeArray() - 1)) && (m_Cards[i]->GetSuit() == aTable.SuitCard(aTable.SizeArray() - 1)))
					{
						m_Cards[i]->sprite.setColor(Color::White);
						choice = i;
						cardChosen = true;
						break;
					}
					else if ((m_Cards[i]->GetSuit() == aTrumpSuit) && (m_Cards[i]->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y)))
					{
						m_Cards[i]->sprite.setColor(Color::White);
						choice = i;
						cardChosen = true;
						break;
					}
				}

				if (cardChosen) break;

			}
		}
		return choice;

		/*cout << "Введите номер карты (0 - взять): ";
		do
		{
			cin >> choice;
		} while ((choice != 0) && (choice < 1 || choice > m_Cards.size()));

		while (choice != 0)
		{
			for (int i = 0; i < aTable.SizeTable(); i++)
			{
				if (m_Cards[choice - 1]->GetSuit() == aTable.SuitCard(i) && m_Cards[choice -1]->GetValue() > aTable.RankCard(i))
				{
					return choice - 1;
				}
				else if (m_Cards[choice - 1]->GetSuit() == aTrumpSuit)
				{
					return choice - 1;
				}
				cin >> choice;
			}
			if (choice == 0)
			{
				return choice;
			}
		}*/

	}
}

void Player::ChoosingName()
{
	cout << "Введите имя: ";
	getline(cin, m_Name);

}



Computer::Computer(const string& name) :
	GenericPlayer(name)
{}

Computer::~Computer()
{}

int Computer::OpponentAction(RenderWindow& window, GenericPlayer& aGenericPlayer, int aIndex, int aTrumpSuit, int aActionModel, Table& aTable)
{
	unsigned int index = 25;
	switch (aActionModel)
	{
	case FIRST_MOVE:
		if (index == 25)
		{
			for (int i = 0; i < m_Cards.size(); i++)
			{
				if (m_Cards[i]->GetSuit() != aTrumpSuit)
				{
					index = i;
					return index;
				}
			}
		}
		if (index == 25)
		{
			for (int i = 0; i < m_Cards.size(); i++)
			{
				if (m_Cards[i]->GetSuit() == aTrumpSuit)
				{
					index = i;
					return index;
				}
			}
		}

	case PROTECTION:
		if (index == 25)                         // Покрыть обычной картой
		{
			for (int i = 0; i < m_Cards.size(); i++)
			{


				if ((m_Cards[i]->GetSuit() == aTable.SuitCard(aTable.SizeTable() - 1) && (m_Cards[i]->GetValue() > aTable.RankCard(aTable.SizeTable() - 1))))
				{
					index = i;
					return index;
				}
			}
		}
		if (index == 25)                   // Покрыть козырной картой
		{
			for (int i = 0; i < m_Cards.size(); i++)
			{
				if (m_Cards[i]->GetSuit() == aTrumpSuit && aTable.SuitCard(aTable.SizeTable() - 1) != aTrumpSuit)
				{
					index = i;
					return index;
				}
				if (m_Cards[i]->GetSuit() == aTrumpSuit && aTable.SuitCard(aTable.SizeTable() - 1) == aTrumpSuit && (m_Cards[i]->GetValue() > aTable.RankCard(aTable.SizeTable() - 1)))
				{
					index = i;
					return index;
				}
			}
		}
		if (true)                                  // Забрать
		{
			return 99;
		}
	case ATTACK:
		if (index == 25)		// Походить не козырной картой
		{
			for (int i = 0; i < m_Cards.size(); i++)
			{
				for (int j = 0; j < aTable.SizeTable(); j++)
				{
					if (m_Cards[i]->GetValue() == aTable.RankCard(j) && m_Cards[i]->GetSuit() != aTrumpSuit)
					{
						index = i;
						return index;
					}
				}
			}
		}
		if (aTable.SizeTable() > 6 || m_Cards.size() < 3)			// Положить на стол козырную карту
		{
			for (int i = 0; i < m_Cards.size(); i++)
			{
				for (int j = 0; j < aTable.SizeTable(); j++)
				{
					if (m_Cards[i]->GetValue() == aTable.RankCard(j) && m_Cards[i]->GetSuit() == aTrumpSuit)
					{
						index = i;
						return index;
					}
				}
			}
		}
		if (index == 25)		// Бита
		{
			return 99;
		}

	}
}



Deck::Deck()
{
	m_Cards.reserve(36);
	Populate();
	m_Count = 36;

	image_TrumpSuit.loadFromFile("images/39.png");
	texture_TrumpSuit.loadFromImage(image_TrumpSuit);
	sprite_TrumpSuit.setTexture(texture_TrumpSuit);
}

Deck::~Deck()
{}

void Deck::DrawSuit(RenderWindow& window)
{
	if (m_TrumpSuit == 0)
	{
		sprite_TrumpSuit.setTextureRect(IntRect(0, 0, 50, 50));
		sprite_TrumpSuit.setPosition(140, 420);
		//sprite_TrumpSuit.setScale(1.5f, 1.5f);
		window.draw(sprite_TrumpSuit);
	}
	else if (m_TrumpSuit == 1)
	{
		sprite_TrumpSuit.setTextureRect(IntRect(50, 50, 50, 50));
		sprite_TrumpSuit.setPosition(140, 420);
		//sprite_TrumpSuit.setScale(1.5f, 1.5f);
		window.draw(sprite_TrumpSuit);
	}
	else if (m_TrumpSuit == 2)
	{
		sprite_TrumpSuit.setTextureRect(IntRect(0, 50, 50, 50));
		sprite_TrumpSuit.setPosition(140, 420);
		//sprite_TrumpSuit.setScale(1.5f, 1.5f);
		window.draw(sprite_TrumpSuit);
	}
	else
	{
		sprite_TrumpSuit.setTextureRect(IntRect(50, 0, 50, 50));
		sprite_TrumpSuit.setPosition(140, 420);
		//sprite_TrumpSuit.setScale(1.5f, 1.5f);
		window.draw(sprite_TrumpSuit);
	}
}

void Deck::DrawDeck(RenderWindow& window)
{
	int cnt = 0;
	for (int i = 0; i < m_Cards.size(); i++)
	{
		if (i == 0)
		{
			m_Cards[i]->DrawCard(window, 80, 343);
		}
		else
		{
			m_Cards[i]->Flip();
			m_Cards[i]->spriteFaceDown.setRotation(90);
			m_Cards[i]->DrawCard(window, 250 + cnt, 400 + cnt);
			//m_Cards[i]->spriteFaceDown.setRotation(90);
			m_Cards[i]->Flip();
			cnt += 1;
		}
	}
	if (m_Cards.size() == 0)
	{
		DrawSuit(window);
	}
}

void Deck::Populate()
{
	Clear();
	int count = 0;
	string str = " ";
	for (int s = Card::PEAKS; s <= Card::HEARTS; s++)
	{
		for (int r = Card::SIX; r <= Card::ACE; r++)
		{
			count++;
			Add(new Card(static_cast<Card::rank>(r), static_cast<Card::suit>(s), true, to_string(count)));
		}
	}
}

void Deck::TrumpSuitCard()
{
	m_Cards.insert(m_Cards.begin(), m_Cards[m_Cards.size() - 1]);
	m_Cards.pop_back();
	m_TrumpSuit = m_Cards[0]->GetSuit();
}

int Deck::GetTrumpSuit()
{
	return m_TrumpSuit;
}

void Deck::Shuffle()
{
	srand(static_cast<unsigned int>(time(0)));
	random_shuffle(m_Cards.begin(), m_Cards.end());
}

void Deck::Deal(Hand& aHand)
{
	if (!(m_Cards.empty()))
	{
		aHand.Add(m_Cards[m_Cards.size() - 1]);
		m_Cards.pop_back();
	}
}

void Deck::AdditionalCard(GenericPlayer& aGenericPlayer)
{
	while (aGenericPlayer.NeedCard())
	{
		if (m_Cards.size() == 0)
		{
			break;
		}
		Deal(aGenericPlayer);

	}
	//SortHand();
	//aGenericPlayer.DrawHand(window, aPlayer);
}


Game::Game()
{
	m_Deck.Populate();
	m_Deck.Shuffle();
}

void Game::DrawBackground(RenderWindow& window)
{
	for (int i = 0; i < HEIGHT_TABLE; i++)		// Отрисовка заднего фона (стола)
	{
		for (int j = 0; j < WIDTH_TABLE; j++)
		{
			if (TileTable[i][j] == ' ') m_Table.Table_map.setTextureRect(IntRect(0, 0, 100, 100));

			m_Table.Table_map.setPosition(j * 100, i * 100);
			window.draw(m_Table.Table_map);
		}
	}
}

bool Game::Play()
{
	RenderWindow window(VideoMode(1600, 900), "The Fool Card Game"); // Создание окна -> объект окна -> размер окна -> заголовок окна
	bool menuChoice = menu(window);
	Font font;	// Шрифт
	font.loadFromFile("CyrilicOld.ttf");
	Text text("", font, 20);
	text.setFillColor(Color::White);

	Button m_Button_Bita("Бита", 1350, 730, 1405, 750);		// Кнока ( бита / взять )
	Button m_Button_Take("Взять", 1350, 730, 1395, 750);	// Кнопка ( взять )
	Button m_Button_Game_Exit("Выход", 1350, 70, 1395, 90);
	Button m_Button_Game_Again("Ещё раз", 1350, 200, 1375, 220);

	//Button m_Button("Бита", 1350, 730);

	m_Deck.AdditionalCard(m_Player);
	m_Deck.AdditionalCard(m_Computer);

	m_Deck.TrumpSuitCard();					// Определяем козырь

	int TrumpSuit = m_Deck.GetTrumpSuit();	// Козырная масть
	char turn = 'C';						// Очередь (P - Player/ C - computer)
	int computerMove = 0;
	int playerMove = 99;	// Индекс выбранной карты
	bool raund = false;		// Конец раунда


	if (m_Player.LowCard(TrumpSuit) < m_Computer.LowCard(TrumpSuit))		// Ход игрока
	{
		turn = 'P';
	}
	else if (m_Player.LowCard(TrumpSuit) > m_Computer.LowCard(TrumpSuit))	// Ход компьютера
	{
		turn = 'C';
	}
	else
	{
		srand(static_cast<unsigned int>(time(0)));
		int choice = (rand() % 2) + 1;
		if (choice == 1)
		{
			turn = 'P';
		}
		else
		{
			turn = 'C';
		}
	}



	while (window.isOpen()) // Пока открыто окно
	{

		//Event event; // событие
		//while (window.pollEvent(event)) // состояние 
		//{
		//	if (event.type == Event::Closed) // если событые закрывает окно
		//		window.close(); // закрывает окно 
		//}
		while (!(m_Player.NoCards()) && !(m_Computer.NoCards()))
		{

			if (turn == 'P')
			{

				do
				{
					// Информация первого хода
					window.clear(); // очистка текущего окна
					text.setString("Начало игрока");
					text.setCharacterSize(40);
					text.setPosition(20, 10);
					DrawBackground(window);

					if (m_Table.SizeArray() == 0)
					{
						playerMove = m_Player.ChoosingCard(window, m_Table, GenericPlayer::FIRST_MOVE, TrumpSuit, m_Button_Bita, m_Button_Game_Exit);	 	// Выбрали карту ( 1 ход )
						if (playerMove == 100)
						{
							break;
						}
						m_Player.DealInputTable(window, m_Table, playerMove);	// Положили карту на стол
					}
					else
					{
						playerMove = m_Player.ChoosingCard(window, m_Table, GenericPlayer::ATTACK, TrumpSuit, m_Button_Bita, m_Button_Game_Exit);
						if (playerMove == 100)
						{
							break;
						}

						if (playerMove != 99)// Выбрали карту ( 1 ход )   Если бита
						{
							m_Player.DealInputTable(window, m_Table, playerMove);	// Положили карту на стол
						}
					}

					window.draw(text);		// Отрисовка текста ( Ход )
					m_Deck.DrawDeck(window);		// Отрисовка стопки колоды
					m_Player.DrawHand(window, true);		// Отрисовка карт на руке игрока
					m_Computer.DrawHand(window, false);		// Отрисовка карт на руке компьютера
					m_Table.DrawTable(window, 0);
					m_Button_Bita.DrawButton(window);
					m_Button_Game_Exit.DrawButton(window);
					window.display(); // Демонстрация

					if (playerMove != -1 && playerMove != 99)
					{
						computerMove = m_Computer.OpponentAction(window, m_Player, playerMove, TrumpSuit, GenericPlayer::PROTECTION, m_Table);	// Ходит компьютер ( 1 ход )
					}
					if (computerMove != 99 && playerMove != -1 && playerMove != 99)		// Случай, если компьютер смог отбиться от первой атаки
					{
						m_Computer.DealInputTable(window, m_Table, computerMove);
					}

					if (playerMove == 99)		// Если бита
					{
						raund = true;			// Раунд завершен 
						m_Table.Clear();		// Очищаем стол от карт
						turn = 'C';				// Следующий ход начинает компьютер
						//break;
					}

					if (m_Player.NoCards() || m_Computer.NoCards())		// Если у кого-то из игроков закончились карты, то ход заканчивается
					{
						raund = true;			// Раунд завершен 
						m_Table.Clear();		// Очищаем стол от карт
						turn = 'C';
						//break;
					}

					if (computerMove == 99)		// Если компьютер не смог отбиться от следующих атак
					{
						int size = 0;

						while (playerMove != 99)		// Игрок может докинуть ему карт
						{
							playerMove = m_Player.ChoosingCard(window, m_Table, GenericPlayer::ATTACK, TrumpSuit, m_Button_Bita, m_Button_Game_Exit);
							if (playerMove == 100)
							{
								break;
							}
							if (playerMove != -1)
							{
								size++;
							}
							if (playerMove == 99)
							{
								raund = true;			// Раунд завершен 
								break;
							}
							if (playerMove != 100) // -1
							{
								m_Player.DealInputTable(window, m_Table, playerMove);

							}
							window.clear();
							DrawBackground(window);			// Отрисовка фона
							window.draw(text);		// Отрисовка текста ( Ход )
							m_Deck.DrawDeck(window);		// Отрисовка стопки колоды
							m_Player.DrawHand(window, true);		// Отрисовка карт на руке игрока
							m_Computer.DrawHand(window, false);		// Отрисовка карт на руке компьютера
							m_Table.DrawTable(window, size);
							m_Button_Bita.DrawButton(window);
							m_Button_Game_Exit.DrawButton(window);
							window.display(); // Демонстрация
						}
						if (playerMove == 100)
						{
							break;
						}
						m_Table.AdditionalOutputTable(m_Computer);
						//break;
					}

					if (raund == true)		// Если раунд завершен, то пополняем руки картами
					{
						m_Deck.AdditionalCard(m_Player);
						m_Deck.AdditionalCard(m_Computer);
						computerMove = 25;
						playerMove = -1;
						raund = false;
					}

				} while (turn == 'P');
				if (playerMove == 100)
				{
					break;
				}

			}
			else
			{
				// Информация первого хода
				window.clear(); // очистка текущего окна
				text.setString("Начало соперника");
				text.setCharacterSize(40);
				text.setPosition(20, 10);
				DrawBackground(window);
				window.draw(text);		// Отрисовка текста ( Ход )
				m_Deck.DrawDeck(window);		// Отрисовка стопки колоды
				m_Player.DrawHand(window, true);		// Отрисовка карт на руке игрока
				m_Computer.DrawHand(window, false);		// Отрисовка карт на руке компьютера
				m_Table.DrawTable(window, 0);
				m_Button_Take.DrawButton(window);
				m_Button_Game_Exit.DrawButton(window);
				window.display(); // Демонстрация
				if (m_Table.SizeArray() == 0)
				{
					computerMove = m_Computer.OpponentAction(window, m_Player, playerMove, TrumpSuit, GenericPlayer::FIRST_MOVE, m_Table);
					m_Computer.DealInputTable(window, m_Table, computerMove);
				}
				else if (m_Table.SizeArray() != 0 && playerMove != -1)
				{
					computerMove = m_Computer.OpponentAction(window, m_Player, playerMove, TrumpSuit, GenericPlayer::ATTACK, m_Table);
					if (computerMove != 99)
					{
						m_Computer.DealInputTable(window, m_Table, computerMove);
					}
				}

				if (computerMove == 99)
				{
					raund = true;
					m_Table.Clear();
					turn = 'P';
					//break;
				}

				if (m_Player.NoCards() || m_Computer.NoCards())
				{
					raund = true;
					m_Table.Clear();
					turn = 'P';
					//break;
				}

				playerMove = m_Player.ChoosingCard(window, m_Table, GenericPlayer::PROTECTION, TrumpSuit, m_Button_Take, m_Button_Game_Exit);
				if (playerMove == 100)
				{
					break;
				}

				if (playerMove != 99 && playerMove != -1)		// Если игрок не взял карты 
				{
					m_Player.DealInputTable(window, m_Table, playerMove);
				}

				if (playerMove == 99)
				{
					int size = 0;

					while (computerMove != 99)
					{
						computerMove = m_Computer.OpponentAction(window, m_Player, playerMove, TrumpSuit, GenericPlayer::ATTACK, m_Table);
						if (computerMove != 99)
						{
							size++;
						}
						if (computerMove == 99)
						{
							raund = true;
							break;
						}
						if (computerMove != 100)
						{
							m_Computer.DealInputTable(window, m_Table, computerMove);
						}
						window.clear(); // очистка текущего окна
						DrawBackground(window);
						window.draw(text);		// Отрисовка текста ( Ход )
						m_Deck.DrawDeck(window);		// Отрисовка стопки колоды
						m_Player.DrawHand(window, true);		// Отрисовка карт на руке игрока
						m_Computer.DrawHand(window, false);		// Отрисовка карт на руке компьютера
						m_Table.DrawTable(window, size);
						m_Button_Take.DrawButton(window);
						m_Button_Game_Exit.DrawButton(window);
						window.display(); // Демонстрация
					}
					m_Table.AdditionalOutputTable(m_Player);
					//break;
				}

				if (raund == true)		// Если раунд завершен, то пополняем руки картами
				{
					m_Deck.AdditionalCard(m_Player);
					m_Deck.AdditionalCard(m_Computer);
					raund = false;
					playerMove = -1;
					computerMove = 25;
				}

			}
		}
		if (m_Player.NoCards())
		{
			text.setString("Вы выйграли");
			text.setCharacterSize(60);
			text.setPosition(635, 370);
		}
		else
		{
			text.setString("Вы проиграли");
			text.setCharacterSize(60);
			text.setPosition(635, 370);
		}

		if(playerMove != 100)
		{
			int menuNum = 0;
			bool isMenu = 1;
			while (isMenu)
			{

				menuNum = 0;
				m_Button_Game_Exit.rectangle.setFillColor(Color(116, 71, 48));
				m_Button_Game_Again.rectangle.setFillColor(Color(116, 71, 48));

				if (IntRect(1350, 70, 200, 100).contains(Mouse::getPosition(window)))
				{
					m_Button_Game_Exit.rectangle.setFillColor(Color(164, 102, 68));
					menuNum = 1;
				}
				if (IntRect(1350, 200, 200, 100).contains(Mouse::getPosition(window)))
				{
					m_Button_Game_Again.rectangle.setFillColor(Color(164, 102, 68));
					menuNum = 2;
				}

				if (Mouse::isButtonPressed(Mouse::Left))
				{
					if (menuNum == 1)
					{
						window.close();
						isMenu = false;
					}
					if (menuNum == 2)
					{
						isMenu = false;//если нажали первую кнопку, то выходим из меню 
					}

				}

				window.clear(); // очистка текущего окна
				DrawBackground(window);
				window.draw(text);		// Отрисовка текста ( Ход )
				m_Deck.DrawDeck(window);		// Отрисовка стопки колоды
				m_Player.DrawHand(window, true);		// Отрисовка карт на руке игрока
				m_Computer.DrawHand(window, false);		// Отрисовка карт на руке компьютера
				m_Table.DrawTable(window, 0);
				m_Button_Game_Exit.DrawButton(window);
				m_Button_Game_Again.DrawButton(window);
				//m_Button_Take.DrawButton(window);
				window.display(); // Демонстрация
			}
			if (menuNum == 2)
			{
				m_Player.Clear();
				m_Computer.Clear();
				m_Table.Clear();
				m_Deck.Populate();
				m_Deck.Shuffle();
				return true;
			}
			else
			{
				return false;
			}
		}
		return false;
	}
	if (!menuChoice)
	{
		return false;
	}
	window.close();
}


bool menu(RenderWindow& window)
{
	Button ButtonNewGame("Новая игра", 700, 325, 707, 347);		// Кнока ( бита / взять )
	Button ButtonExit("Выход", 700, 475, 744, 495);	// Кнопка ( взять )

	bool isMenu = 1;
	int menuNum = 0;

	while (isMenu)
	{
		Event event; // событие
		while (window.pollEvent(event)) // состояние 
		{
			if (event.type == Event::Closed) // если событые закрывает окно
				window.close(); // закрывает окно 
		}
		ButtonNewGame.rectangle.setFillColor(Color(116, 71, 48));
		ButtonExit.rectangle.setFillColor(Color(116, 71, 48));
		menuNum = 0;
		window.clear(Color(23, 80, 31));

		if (IntRect(650, 325, 200, 100).contains(Mouse::getPosition(window)))
		{
			ButtonNewGame.rectangle.setFillColor(Color(164, 102, 68));
			menuNum = 1;
		}
		if (IntRect(650, 475, 200, 100).contains(Mouse::getPosition(window)))
		{
			ButtonExit.rectangle.setFillColor(Color(164, 102, 68));
			menuNum = 2;
		}

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1)
			{
				isMenu = true;//если нажали первую кнопку, то выходим из меню 
				return isMenu;
			}
			if (menuNum == 2)
			{
				window.close();
				isMenu = false;
				return isMenu;
			}
		}

		ButtonNewGame.DrawButton(window);
		ButtonExit.DrawButton(window);

		window.display();
	}


}

void Game::gameRunning(Game& aGame)
{
	if (aGame.Play())
	{
		gameRunning(aGame);
	}
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	setlocale(LC_ALL, "ru");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	Game m_Game;
	m_Game.gameRunning(m_Game);;
	return 0;

}

//int main()
//{
//	setlocale(LC_ALL, "ru");
//	SetConsoleOutputCP(1251);
//	SetConsoleCP(1251);
//	Game m_Game;
//	m_Game.gameRunning(m_Game);;
//	return 0;
//
//}






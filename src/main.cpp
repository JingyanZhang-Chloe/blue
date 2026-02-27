#include <SFML/Graphics.hpp>
#include "dialogue.hpp"
#include "SFML/Audio/Music.hpp"
#include <iostream>
#include <sstream>
#include <random>

std::random_device rd;       // seed source
std::mt19937 gen(rd());      // Mersenne Twister engine
using namespace std;

class Game {
public:
	Game();
	void run();

private:
	void handleEvents();
	void update();
	void render();
	void applyEffect(DialogueEffect effect);

	sf::RenderWindow window;

	sf::Texture characterTexture;
	sf::Sprite characterSprite;

	sf::Text optionText;

	sf::Texture cakeTexture;
	sf::Sprite cakeSprite;
	sf::Clock cakeClock;
	float moveInterval = 1.0f;

	sf::Texture letterTexture;
	sf::Sprite letterSprite;

	sf::Font font;
	sf::Text text;
	sf::RectangleShape bubble;

	sf::Music birthdayMusic;

	Dialogue dialogue;
	int lastDialogueIndex;

	bool showCake;
	bool showLetter;
};

Game::Game()
: window( sf::VideoMode( { 1000, 1000 } ), "BLUE DUCK", sf::Style::Default, sf::State::Windowed)
, optionText(font)
, characterSprite(characterTexture)
, cakeSprite(cakeTexture)
, letterSprite(letterTexture)
, text(font)
, dialogue(0)
, lastDialogueIndex(-1)
, showCake(false)
, showLetter(false) {
	if (!characterTexture.loadFromFile("character.png")) {
		throw std::runtime_error("Failed to load character.png");
	}

	characterTexture.setSmooth(false);
	characterSprite = sf::Sprite(characterTexture);

	sf::Vector2u window_size = window.getSize();
	sf::Vector2u character_size = characterTexture.getSize();

	float scale_x = static_cast<float>(window_size.x) / static_cast<float>(character_size.x);
	float scale_y = static_cast<float>(window_size.y) / static_cast<float>(character_size.y);

	sf::Vector2f scale_factor(scale_x * 0.5f, scale_y * 0.5f);
	characterSprite.setScale(scale_factor);

	sf::FloatRect bounds = characterSprite.getLocalBounds();
	characterSprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
	characterSprite.setPosition({static_cast<float>(window_size.x) / 2.f, static_cast<float>(window_size.y) / 2.f});

	// font
	if (!font.openFromFile("font.ttf")) {
		throw std::runtime_error("Failed to load font.ttf");
	}

	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Black);
	text.setPosition({static_cast<float>(window_size.x) / 2.f, static_cast<float>(window_size.y) / 8.f});


	// optionText
	optionText.setCharacterSize(25);
	optionText.setFillColor(sf::Color::Black);
	optionText.setPosition({static_cast<float>(window_size.x) / 4.f, static_cast<float>(window_size.y) / 1.2f});

	// bubble
	bubble.setFillColor(sf::Color(200, 200, 255));

	//sf::Vector2f bubbleSize = bubble.getSize();

	//float shift_left = (bubbleSize.x - textBounds.size.x) / 2.f;
	//float shift_down = (bubbleSize.y - textBounds.size.y) / 2.f;
	//text.setPosition({bubble.getPosition().x + shift_left, bubble.getPosition().y + shift_down});

	// cake
	characterTexture.setSmooth(false);
	if (!cakeTexture.loadFromFile("cake.png")) {
		throw std::runtime_error("Failed to load cake.jpg");
	}
	sf::Vector2u cake_size = cakeTexture.getSize();

	float cake_scale_x = static_cast<float>(window_size.x) / static_cast<float>(cake_size.x);
	float cake_scale_y = static_cast<float>(window_size.y) / static_cast<float>(cake_size.y);
	sf::Vector2f cake_scale_factor(cake_scale_x * 0.5f, cake_scale_y * 0.5f);

	cakeSprite = sf::Sprite(cakeTexture);
	cakeSprite.setScale({cake_scale_factor});
	cakeSprite.setPosition(characterSprite.getPosition());

	// lettre
	if (!letterTexture.loadFromFile("letter.png")) {
		throw std::runtime_error("Failed to load letter.png");
	}
	letterSprite = sf::Sprite(letterTexture);

	sf::Vector2u letter_size = letterTexture.getSize();
	float letter_scale_x = static_cast<float>(window_size.x) / static_cast<float>(letter_size.x);
	float letter_scale_y = static_cast<float>(window_size.y) / static_cast<float>(letter_size.y);
	sf::Vector2f letter_scale_factor(0.3f, 0.3f);

	letterSprite.setScale(letter_scale_factor);

	sf::FloatRect letter_bounds = letterSprite.getLocalBounds();
	letterSprite.setOrigin({letter_bounds.size.x / 2.f, letter_bounds.size.y / 2.f});

	letterSprite.setPosition(characterSprite.getPosition());

	// music
	if (!birthdayMusic.openFromFile("happy_birthday.mp3")) {
		throw std::runtime_error("Failed to load music");
	}
	birthdayMusic.setLooping(true);
}

void Game::applyEffect(DialogueEffect effect) {
	showCake = false;
	showLetter = false;

	// stop music unless current node wants music
	if (birthdayMusic.getStatus() == sf::SoundSource::Status::Playing)
		birthdayMusic.stop();

	switch (effect) {
		case DialogueEffect::ShowCake:
			showCake = true;
		break;
		case DialogueEffect::ShowLetter:
			showLetter = true;
		break;
		case DialogueEffect::PlaySong:
			birthdayMusic.play();
		break;
		case DialogueEffect::None:
			default:
				break;
	}
}

void Game::handleEvents() {
	while ( const std::optional event = window.pollEvent() )
	{
		if ( event->is<sf::Event::Closed>() )
			window.close();

		// if one click key 1, 2, 3, jumps to the 0th, 1st, 2nd dialogue
		int chosen = -1;
		if ( event->is<sf::Event::KeyPressed>() ) {
			const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
			switch (keyEvent->code) {
				case sf::Keyboard::Key::Num1:
					chosen = 0;
				break;
				case sf::Keyboard::Key::Num2:
					chosen = 1;
				break;
				case sf::Keyboard::Key::Num3:
					chosen = 2;
				break;
				case sf::Keyboard::Key::Num4:
					chosen = 3;
				break;
				default:
					chosen = -1;
			}
		}
		dialogue.choose_option(chosen);
	}
}


std::string wrap_string(
	const std::string& current_string,
	sf::Font& font,
	float max_width)
{
	sf::Text text(font);

	std::stringstream words(current_string);
	std::string wrapped;
	std::string current_line;
	std::string word;

	while (words >> word)
	{
		std::string testLine = current_line + word + " ";
		text.setString(testLine);

		if (text.getLocalBounds().size.x > max_width)
		{
			wrapped += current_line + "\n";
			current_line = word + " ";
		}
		else
		{
			current_line = testLine;
		}
	}

	wrapped += current_line;
	return wrapped;
}


void Game::update() {
	int current_index = dialogue.current_node_index;
	if (lastDialogueIndex != current_index) {
		lastDialogueIndex = current_index;
		const DialogueNode& current_dialogue = dialogue.get_current_dialogue();

		// text update
		std::string current_string = wrap_string(current_dialogue.character_text, font, static_cast<float>(window.getSize().x));
		text.setString(current_string);
		sf::FloatRect textBounds = text.getLocalBounds();
		text.setOrigin({textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f});

		bubble.setSize({textBounds.size.x + 20, textBounds.size.y + 20});
		sf::FloatRect bubbleBounds = bubble.getLocalBounds();
		bubble.setOrigin({bubbleBounds.position.x + bubbleBounds.size.x / 2.f, bubbleBounds.position.y + bubbleBounds.size.y / 2.f});
		bubble.setPosition(text.getPosition());

		// build options
		std::string option_string = std::string("");

		for (size_t i = 0; i < current_dialogue.options.size(); ++i)
		{
			const Option& option = current_dialogue.options[i];

			option_string += std::to_string(i + 1);
			option_string += ") ";
			option_string += option.text;
			option_string += "\n";
		}

		optionText.setString(option_string);

		// apply effect
		applyEffect(current_dialogue.effect);
	}
}

void Game::render() {
	window.clear(sf::Color::White);
	window.draw(bubble);
	window.draw(text);
	window.draw(optionText);
	window.draw(characterSprite);

	if (showCake)
	{
		if (cakeClock.getElapsedTime().asSeconds() > moveInterval)
		{
			float maxX = static_cast<float>(window.getSize().x) - cakeSprite.getGlobalBounds().size.x;
			float maxY = static_cast<float>(window.getSize().y) - cakeSprite.getGlobalBounds().size.y;

			std::uniform_real_distribution<float> distX(0.f, maxX);
			std::uniform_real_distribution<float> distY(0.f, maxY);

			float randomX = distX(gen);
			float randomY = distY(gen);

			cakeSprite.setPosition({randomX, randomY});
			cakeClock.restart();
		}

		window.draw(cakeSprite);
	}

	if (showLetter) {
		window.draw(letterSprite);
	}

	window.display();
}

void Game::run() {
	while ( window.isOpen() )
	{
		handleEvents();
		update();
		render();
	}
}


int main()
{
	Game game;
	game.run();
}

// including the relevant libraries required
// sfml and tgui are the GUI libraries
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <iostream>
// fstream is used to read and write to files
#include <fstream>
// curl is the library that calls, sends and recieves data from the api
#include "curl/curl.h"
#include <string.h>
#include "TGUI/AllWidgets.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"


using namespace std;

// class that contains all global variables that are used multiple times within the program
class globals {
// setting the variables below to public
public:
	// setting buttonFloat and answer as global variables so they can be used in a function and in the main program
	float buttonFloat;
	float answer;
	float finalCorrectAnswer;
	float finalUserAnswer;
};
// creating a object for the class globals to use alter in the code
globals globalobj;

// string array containing currencies so a random currecny can be selected for conversion from the base currency
string currencies[160] = { "AED","AFN","ALL","AMD","ANG","AOA","ARS","AUD","AWG","AZN","BAM","BBD","BDT","BGN","BHD","BIF","BMD","BND","BOB","BRL","BSD","BTN","BWP","BYN","BZD","CAD","CDF","CHF", "CLP","CNY","COP","CRC","CUC","CUP","CVE","CZK","DJF","DKK","DOP","DZD","EGP","ERN","ETB","EUR","FJD","FKP","FOK","GBP","GEL","GGP","GHS","GIP","GMD","GNF","GTQ","GYD","HKD","HNL","HRK","HTG","HUF","IDR","ILS","IMP","INR","IQD","IRR","ISK","JMD","JOD","JPY","KES","KGS","KHR","KID","KMF","KRW","KWD","KYD","KZT","LAK","LBP","LKR","LRD","LSL","LYD","MAD","MDL","MGA","MKD","MMK","MNT","MOP","MRU","MUR","MVR","MWK","MXN""MYR","MZN","NAD","NGN","NIO","NOK","NPR","NZD","OMR","PAB","PEN","PGK","PHP","PKR","PLN","PYG","QAR","RON","RSD","RUB","RWF","SAR","SBD","SCR","SDG","SEK","SGD","SHP","SLL","SOS","SRD","SSP","STN","SYP","SZL","THB","TJS","TMT","TND","TOP","TRY","TTD","TVD","TWD","TZS","UAH","UGX","USD","UYU","UZS","VES","VND","VUV","WST","XAF","XCD","XDR","XOF","XPF","YER","ZAR","ZMW" };
// string array of all currencies so a random base code can be selected for conversion
string BaseCodes[160] = { "AED","AFN","ALL","AMD","ANG","AOA","ARS","AUD","AWG","AZN","BAM","BBD","BDT","BGN","BHD","BIF","BMD","BND","BOB","BRL","BSD","BTN","BWP","BYN","BZD","CAD","CDF","CHF", "CLP","CNY","COP","CRC","CUC","CUP","CVE","CZK","DJF","DKK","DOP","DZD","EGP","ERN","ETB","EUR","FJD","FKP","FOK","GBP","GEL","GGP","GHS","GIP","GMD","GNF","GTQ","GYD","HKD","HNL","HRK","HTG","HUF","IDR","ILS","IMP","INR","IQD","IRR","ISK","JMD","JOD","JPY","KES","KGS","KHR","KID","KMF","KRW","KWD","KYD","KZT","LAK","LBP","LKR","LRD","LSL","LYD","MAD","MDL","MGA","MKD","MMK","MNT","MOP","MRU","MUR","MVR","MWK","MXN""MYR","MZN","NAD","NGN","NIO","NOK","NPR","NZD","OMR","PAB","PEN","PGK","PHP","PKR","PLN","PYG","QAR","RON","RSD","RUB","RWF","SAR","SBD","SCR","SDG","SEK","SGD","SHP","SLL","SOS","SRD","SSP","STN","SYP","SZL","THB","TJS","TMT","TND","TOP","TRY","TTD","TVD","TWD","TZS","UAH","UGX","USD","UYU","UZS","VES","VND","VUV","WST","XAF","XCD","XDR","XOF","XPF","YER","ZAR","ZMW" };
// float array for the numbers involved within the conversion question
float ConvNums[30] = { 10.00,27.89,38.50,46.72,51.00,64.00,70.12,83.45,90.68,23.41,29.70,32.18,110.10,104.71,160.00,125.34,139.21,145.50,15.00,120.00,167.56,171.00,185.00,193.00,198.00,200.00,250.00,225.00,210.00,43.60 };
// finding the length of each array by taking the end position value from the start position value
int currencylen = end(currencies) - begin(currencies);
int BaseCodelen = end(BaseCodes) - begin(BaseCodes);
int ConvNumlen = end(ConvNums) - begin(ConvNums);


// making a global varaibale to store the number of lives the player has
int lives = 3;

// function to call back api results for writing.
// this was taken from https://stackoverflow.com/questions/9786150/save-curl-content-result-into-a-string-in-c
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	//appending the contents and size by number of members
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}
// function to append a string to a file
void fileappend(string toAppend) {
	// creating an ofstream object so the file can be opened
	ofstream file;
	// opening the file PreviousRates.txt and using std::ios_base::app to go to the end of the file before writing to it
	file.open("PreviousRates.txt", std::ios_base::app);
	//writing to the end of the file/ appending
	file << toAppend << endl;
}
// making a function to get the text ffrom the editbox on the gui
void textGetter(tgui::EditBox::Ptr textBox) {
	// setting a tgui string called textboxstr
	tgui::String textboxstr;
	// setting textboxstr as the input to the editbox
	textboxstr = textBox->getText();
	// converting the tgui sttring to a std string of utf8
	string strUtf8(textboxstr);
	
	// outputting the textbox input
	cout << "\nyou answered " << strUtf8;

	
	// converting the textbox input to a float to check it for calculations
	globalobj.buttonFloat = stof(strUtf8);

	
}

// making a function that will return a different random number each time it is called
int RandNum(int Size) {
	int maximum;
	maximum = Size; //set the upper bound to generate the random number
	// seeding the number generator using rand and providing the unsigned integer time(0) as the seed
	srand(time(0));
	// to get the random number rand() is used and maximum is the upper bound and uses modulus (%) to provide a remainder which gives a value between 0 and maximum
	int Rnum = rand() % maximum;
	// re randomising the number to make it less likely there will be the same numbers in a row
	int returner = rand() % Rnum + 1;

	return returner;
}
// a function to round a float to 2 decimal places although it does not remove all 0s past 2 decimal places
float round2dp(float roundnum)
{
	//multiplying by 100 then adding 0.5 to see if number increases
	//type casting to int to remove the numbers after decimal point
	float value = (int)((roundnum * 100) + 0.5)/ 100.0;
	// dividing by a hundred to get the new rounded number in type float
	return value ;
}

int previousSearches() {
	fstream file;
	file.open("PreviousRates.txt",std::ios_base::in);
	if (file.is_open()){
		while (!file.eof()) {
			string search;
			file >> search;
			cout << search << "\n";
		}
	}
	return 0;
};
// class that calls the game over window
class gameOver {
// setting the gameover function as public
public:
	int Gameover() {

		// creating a game over window of size 500 by 500
		sf::RenderWindow GameOverWindow(sf::VideoMode(500, 500), "Game Over!");
		// creating a game over string and a youre fired string
		string GameOver = "Game Over!";
		string firedstr = "You've Been Fired";

		// loading a font called break it and storing it as the sfml font variable gameoverfont
		sf::Font GameOverFont;
		// checking for load error and returning a failure causing the program to exit, font taken from dafont.com
		if (!GameOverFont.loadFromFile("BREAK IT.ttf")) {
			return EXIT_FAILURE;
		}

		// creating an sfml text variable called gameovertext
		sf::Text GameOverText;
		// setting the font as the gameoverfont
		GameOverText.setFont(GameOverFont);
		// setting the text string as the gameover string
		GameOverText.setString(GameOver);
		// setting the text colour as black
		GameOverText.setFillColor(sf::Color::Black);
		// setting the text size to 100
		GameOverText.setCharacterSize(100);
		// setting the text position at 50,120
		GameOverText.setPosition(50, 120);

		// creating a sfml text variable to display the youre fired string
		sf::Text fired;
		// setting the font as gameoverfont
		fired.setFont(GameOverFont);
		// setting the text string as the fired string
		fired.setString(firedstr);
		// setting the text colour as red
		fired.setFillColor(sf::Color::Red);
		// setting text size as 60
		fired.setCharacterSize(60);
		// setting the text position at 70,240
		fired.setPosition(70, 240);

		// creating a sound buffer to play a sound
		sf::SoundBuffer buffer;
		// loading the sound file and storing it in the sound buffer, sound from https://youtube.com/shorts/54BevebGR-k?feature=share
		if (!buffer.loadFromFile("get out luke bennett.wav")) {
			// handle error if sound file not loaded
			cout << "sound file could not be loaded";
		}

		// setting the sound up
		sf::Sound GetOutSound;
		// setting the sound buffer for the sound as the buffer created previously
		GetOutSound.setBuffer(buffer);
		// playing the sound
		GetOutSound.play();
		// setting the sound to loop
		GetOutSound.setLoop(true);

		// running the program for as long as it is open
		while (GameOverWindow.isOpen())
		{
			// creating an event variable
			sf::Event GameOverevent;
			// processing the event given while window is open
			while (GameOverWindow.pollEvent(GameOverevent))
			{
				// checks if the window is closed
				if (GameOverevent.type == sf::Event::Closed) {
					// stops the sound and closes the window upon the window close button being pressed
					GetOutSound.stop();
					GameOverWindow.close();
				}
			}

			// clearing the game over window so it is blank then setting the background colour to white
			GameOverWindow.clear(sf::Color::White);
			// drawing the two sfml text variables to the game over window
			GameOverWindow.draw(GameOverText);
			GameOverWindow.draw(fired);
			// displaying the game over window
			GameOverWindow.display();
		}
		return 0;
		// destructing the class once the function ends
		gameOver::~gameOver();
	}
	
};



// main program
int main()
{
	// setting the color of the text and background on the shell
	system("color 3F");
	// running the loop while lives are more than 0
	while (lives > 0) {
		string searchquestion;
		cout << "\nwould you like to see previous searches(y/n)";
		cin >> searchquestion;
		if (searchquestion == "y") {
			previousSearches();
		}

		// finding the basecode for the exchange rate using a random number position
		int BaseCodeposition = RandNum(BaseCodelen);
		// setting the extracted value from the currencies array as a string to be used for the api call
		string outp = { BaseCodes[BaseCodeposition] };
		// setting a character variable with length 4 this is to hold the currency to search for
		char initial[4];
		// copying the value of outp to initial after a conversion from string to char
		strcpy_s(initial, outp.c_str());
		// a character variable that hold the url
		char result[100] = "https://exchangerate-api.p.rapidapi.com/rapid/latest/";
		// adding the requested currency to the url so the results for the desired base code are called
		strcat_s(result, initial);
		// setting a string to hold the api results outputted
		string currencystr;
		// initialising curl so a request can be made
		CURL* hnd = curl_easy_init();
		// setting up curl so that a custom request can be made to the api and the "GET" shows that data is being retrieved
		curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
		// setting the url that the requested daat will be gotten from
		curl_easy_setopt(hnd, CURLOPT_URL, result);
		// defining headers as a curl linked list and setting the value of headers to NULL
		struct curl_slist* headers = NULL;
		// appending the api host to headers
		headers = curl_slist_append(headers, "x-rapidapi-host: exchangerate-api.p.rapidapi.com");
		// appending the api key to headers
		headers = curl_slist_append(headers, "x-rapidapi-key: 96b90b1d7bmshfd92fe00bddff38p1b2eb5jsnfcdb111409dd");
		// providing the information required for a successful data retrieval from the specified api
		curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
		// using the function WriteCallback to call back the api data so it can be written to a string
		curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
		// writing the api data that has been called back into a string
		curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &currencystr);
		CURLcode ret = curl_easy_perform(hnd);
		// creating two string varaibles to take on the currency requested and the substring for the value of the requested currency
		string currencysubstr, currency;
		// creating a size variable so that it can be used to search through the string to find the currency searched
		size_t pos;
		// setting the position of the currency in the array as a random number
		int currencyPosition = RandNum(currencylen);
		// extracting the currency from the currencies array using the random number positional value
		currency = { currencies[currencyPosition] };
		// creating a failsafe so that if both currencies are the same it will add 1 to the currencyposition to change the currency value
		if (currency == initial) {
			currency = { currencies[currencyPosition + 1] };
		}
		// finding the contents of str2 with currencystr which contains all data recieved from the api
		pos = currencystr.find(currency);
		// finding and saving the substring using the parameters pos which is the position of the desired search and the size of the amount to be saved after the subvstring found. 11 covers both the currency and the conversion value
		currencysubstr = currencystr.substr(pos);


		// initialising the strings selectedcurrency and baseCode
		string selectedcurrency, baseCode;
		// initialising a size variable to store the positional value of a substring found
		size_t basepos;
		// finding basecode substring within currencystr
		basepos = currencystr.find("base_code");
		// setting baseCode as the substring position found in basepos and 15 characters after that
		baseCode = currencystr.substr(basepos, 15);
		// removing all speech marks from baseCode from the beginning to the end
		baseCode.erase(remove(baseCode.begin(), baseCode.end(), '"'), baseCode.end());



		// setting size variable to store position of the ,
		size_t commapos = currencysubstr.find(",");
		// setting selectedcurrency as the substring result of currencystr from start to position of the first comma
		selectedcurrency = currencysubstr.substr(0, commapos);
		// initialising the string currencyOutput
		string currencyOutput;
		// setting currencyOutput as baseCode + selectedcurrency with a line break in between
		currencyOutput = baseCode + "\n" + selectedcurrency;

		// removing all speech marks from currencyOutput
		currencyOutput.erase(remove(currencyOutput.begin(), currencyOutput.end(), '"'), currencyOutput.end());
		// initialising size variable ratepos and string variable answerstr
		size_t ratepos;
		string answerstr;
		// setting ratepos as the position of the colon within selectedcurrency
		ratepos = selectedcurrency.find(":");
		// adding 1 to ratepos so it removes the colon 
		ratepos += 1;
		// storing the substring from ratepos to the end of selectedcurrency as answerstr
		answerstr = selectedcurrency.substr(ratepos);
		// setting a string to be appended to the text file
		string append = baseCode + " Currency " + selectedcurrency;
		// appending the string variables append and answerstr to the text file
		fileappend(append);
		fileappend(answerstr);

		
		// converting answerstr to a float for calculations further on
		globalobj.answer = stof(answerstr);
		// setting the random number position for the convnum array
		int Convnumposition = RandNum(ConvNumlen);
		// extracting the float value from the convnum array
		float num = { ConvNums[Convnumposition] };
		// setting the float to 2dp using the round2dp function
		float num2dp = round2dp(num);
		// converting the 2dp number to a string called numq
		string numq = to_string(num2dp);


		// initialising size variable speechpos and string variable currencynoRate
		size_t speechpos;
		string currencynoRate;
		// storing position of speech mark within selected currency
		speechpos = selectedcurrency.find('"');
		// storing the currency value without a rate by creating a substring from the start to the speech mark position(speechpos)
		currencynoRate = selectedcurrency.substr(0, speechpos);

		// initialising string variable numberstr
		string numberstr;
		// setting size variable decimalpos as the position of the decimal point in string numq
		size_t decimalpos = numq.find(".");
		// storing the substring of numq as the start of numq to the decimal position + 3 so the output is to 2 decimal places
		numberstr = numq.substr(0, decimalpos + 3);
		// setting the question string to be outputted onto the gui inside a speech bubble
		string question = "\nhello i have " + numberstr + "\nof " + baseCode + " and\ni would like \nthis converting to " + "\n" + currencynoRate;

		// setting a float variable as the converted numberstr from string to float as the number to use in later calculations
		float numcalc = stof(numberstr);
		// setting the correct answer as answer * numcalc
		float correctAnswer = (numcalc * globalobj.answer);
		// using round2dp function to round correctAnswer to 2dp and store it as correctAnswer2dp
		float correctAnswer2dp = round2dp(correctAnswer);
		// converting correctAnswer2dp to a string
		string correctAnswerStr = to_string(correctAnswer2dp);
		// initialising string variable correctsubstr
		string CorrectSubStr;
		// setting sizevariable correctdecimalpos as the position of the decimal point within correctAnswerStr
		size_t Correctdecimalpos = correctAnswerStr.find(".");
		// setting correctsubstr as the substring from the start to correctdecimalpos + 3 so that the outputted number otputs as a 2dp number
		CorrectSubStr = correctAnswerStr.substr(0, Correctdecimalpos + 3);
		// converting the string correctsubstr to a float called finalCorrectAnswer
		globalobj.finalCorrectAnswer = stof(CorrectSubStr);


		// creating a window for 2d objects ,images and text to be drawn to 
		sf::RenderWindow window(sf::VideoMode(1920, 1080), "Bureau De Change Game!");
		// setting the gui as the sfml window
		tgui::Gui gui{ window };
		// setting a pre built theme to be used for tgui objects
		tgui::Theme theme{ "C:/TGUI/themes/Black.txt" };

		// initialising an sfml font
		sf::Font font;
		// loading the font and putting an error check in place if the file cant be loaded  font file from dafont.com
		if (!font.loadFromFile("aAhaWow.ttf"))
		{
			// handle error if font cant be found
			cout << "font file could not be found";
			return EXIT_FAILURE;
		}
		// setting the background image as a texture
		sf::Texture background;
		// loading background image file
		if (!background.loadFromFile("officebg.png"))
			return EXIT_FAILURE;
		// creating a sprite for the background
		sf::Sprite bgSprite;
		// setting the background sprites texture as the background image
		bgSprite.setTexture(background);
		// setting position to 0,0 so it will cover the entire window
		bgSprite.setPosition(0, 0);
		// scaling the background to fit the whole screen
		bgSprite.setScale(1, 1);

		// setting a sfml text variable questiontext so the question can be displayed onto the window
		sf::Text questiontext;
		// setting the font as the font defined earlier
		questiontext.setFont(font);
		// setting the string to be displayed as the question
		questiontext.setString(question);
		// setting the text colour as black so it contrasts the pink speech bubble
		questiontext.setFillColor(sf::Color::Black);
		// setting the text size to 20
		questiontext.setCharacterSize(20);
		// setting the position of the text so it will fit within the speech bubble
		questiontext.setPosition(600, 100);

		// creating another sfml text variable to store the information needed to calculate the conversion
		sf::Text text;
		// setting the texts font as the font defined earlier in the code
		text.setFont(font);
		// setting the text as the string currecnyOutput
		text.setString(currencyOutput);
		// setting the text colour to black
		text.setFillColor(sf::Color::Black);
		// setting the text size to 50
		text.setCharacterSize(50);
		// setting the text position within the canvas the text is drawn to
		text.setPosition(50, 100);

		// creating a string to store the number of lives left
		string livesStr = "Lives Left :" + to_string(lives);

		// creating a tgui label to display the lives left, auto is used to the variabvles data type is taken from the intialiser so in this case tgui::Label
		auto livesLabel = tgui::Label::create();
		// setting the size of the label
		livesLabel->setSize(400, 100);
		// setting the size of the text within the label to 50
		livesLabel->setTextSize(50);
		// setting the position of the label so it is displayed in the top right corner
		livesLabel->setPosition(1585, 0);
		// setting the labels text as the lives string
		livesLabel->setText(livesStr);
		// adding the label to the gui
		gui.add(livesLabel);

		// creating a textbox for the user to input their answer, again uses auto so the type is deduced from the intialiser
		auto textbox = tgui::EditBox::create();
		// setting the textbox size as 450 by 90
		textbox->setSize(450, 90);
		// setting the theme for the textbox by retrieving the pre defined editbox renderer from the tgui theme file
		textbox->setRenderer(theme.getRenderer("EditBox"));
		// setting the text size within the textbox to 18
		textbox->setTextSize(18);
		// setting the position of the textbox so it is just left of the lives left label
		textbox->setPosition(1200, 70);
		// setting the default text that is displayed in the text box when there is no user interaction with the text box
		textbox->setDefaultText("Type answer here to 2 decimal places");
		// adding the textbox to the gui
		gui.add(textbox);

		// creating a button to retrieve the text inputted into the textbox and to close the window to show whether the user is correct or incorrect
		auto getTextButton = tgui::Button::create("Submit Answer");
		// setting the size of the button as 450,30 so it is the same width as textbox
		getTextButton->setSize(450, 30);
		// setting the theme of the button as the pre defined button renderer from the tgui theme file
		getTextButton->setRenderer(theme.getRenderer("Button"));
		// setting the text size to 18
		getTextButton->setTextSize(18);
		// setting the buttons position so it is just below the textbox
		getTextButton->setPosition(1200, 170);
		// adding the button to the gui
		gui.add(getTextButton);
		// using the onpress function so that it carries out the textgetter function and so that it also clears the window
		getTextButton->onPress(&textGetter, textbox);
		// use of the & symbol is to connect a signal handler which will be called when the signal is sent so in this case for it to close the window and to carry out the textgetter function
		getTextButton->onPress([&] { window.close(); });




		// setting the woman image as a texture
		sf::Texture WomanImg;
		// loading the woman image file and returning an error if this fails
		if (!WomanImg.loadFromFile("woman.png"))
			return EXIT_FAILURE;
		// creating a sprite for the woman image
		sf::Sprite imageSprite;
		// setting the sprites texture as the image taken from the file
		imageSprite.setTexture(WomanImg);
		// setting the position of the sprite within the canvas which is set later in the code
		imageSprite.setPosition(100, 100);
		// setting the sprites scale so it is of reasonable size within the window
		imageSprite.setScale(0.75, 0.75);

		// creating a canvas ,of size 870 by 800, to have sfml objects be drawn onto, this one will have the woman image sprite and the question
		auto canvas = tgui::CanvasSFML::create({ 870, 800 });
		// setting the canvas position at 320,90 on the main window
		canvas->setPosition(320, 90);
		// clearing the canvas and leaving it transparent so that it will not affect the background
		canvas->clear(tgui::Color::Transparent);
		// drawing the woman image sprite and the question text to the canvas
		canvas->draw(imageSprite);
		canvas->draw(questiontext);
		// displaying the canvas
		canvas->display();
		// adding the canvas to the gui
		gui.add(canvas);

		// creating a canvas to display the base code and rate string of size 700 by 300
		auto textcanv = tgui::CanvasSFML::create({ 700,300 });
		// setting the position as 0,0 so it will be displayed in the top left corner
		textcanv->setPosition(0, 0);
		// clearing the canvas and setting it to transparent so that it will not affect the background
		textcanv->clear(tgui::Color::Transparent);
		// drawing the sfml text to the text canvas
		textcanv->draw(text);
		// displaying the text canvas
		textcanv->display();
		// adding the text canvas to the gui
		gui.add(textcanv);

		// creating a texture called deskimg
		sf::Texture deskImg;
		// loading the desk image file and storing it in the texture deskimg
		// handling error if there is failure
		if (!deskImg.loadFromFile("Desk.png"))
			return EXIT_FAILURE;
		// making a desk sprite
		sf::Sprite desk;
		// setting the texture of the desk sprite as the deskImg texture
		desk.setTexture(deskImg);
		// setting the position to 0,0 within the desk canvas
		desk.setPosition(0, 0);
		// setting the scale of the desk sprite so it fits the window reasonably and proportionatly
		desk.setScale(0.8, 0.8);

		// creating a sfml canvas to draw the desk sprite to
		auto deskCanv = tgui::CanvasSFML::create({ 1400,600 });
		// setting the canvas position to 250,600
		deskCanv->setPosition(250, 600);
		// clearing the canvas as transparent so it doesnt affect the background
		deskCanv->clear(tgui::Color::Transparent);
		// drawing the desk sprite to the canvas
		deskCanv->draw(desk);
		// displaying the canvas and its contents
		deskCanv->display();
		// adding the canvas to the gui
		gui.add(deskCanv);

		// running the program for as long as the window is open
		while (window.isOpen())
		{
			// initialising a sfml event variable to check if any events have occurred during the previous iteration
			sf::Event event;

			// window will keep checking for events while window is open and processing these events
			while (window.pollEvent(event))
			{
				// setting the event for the gui window
				gui.handleEvent(event);

				// if the close button is pressed close the window
				if (event.type == sf::Event::Closed)
					window.close();

			}

			// clearing the window before drawing to it to ensure the window is blank
			window.clear(tgui::Color::Cyan);
			// drawing the background before anything else
			window.draw(bgSprite);
			// drawing the gui onto the sfml window
			gui.draw();
			// displaying the window
			window.display();
		}

		

		// initialising the float useranswer as the buttonfloat which is the users input into the textbox
		
		float useranswer = (globalobj.buttonFloat);
		// rounding useranswer to 2 decimal places
		float useranswer2dp = round2dp(useranswer);
		// converting the rounded user answer to a string
		string useranswerStr = to_string(useranswer2dp);
		// creating a string to hold the user answer substring
		string UserSubStr;
		// creating a size variable to hold the position of the decimal point
		size_t Ansdecimalpos = useranswerStr.find(".");
		// setting the user answer sub string as the beginning of the string to the decimal point position + 3 so it will remove all the 0s past the 2dp mark
		UserSubStr = useranswerStr.substr(0, Ansdecimalpos + 3);
		// converting the the substring back to a float
		globalobj.finalUserAnswer = stof(UserSubStr);

		// checking to see if users answer is the same as the correct answer
		if (globalobj.finalUserAnswer == globalobj.finalCorrectAnswer) {
			// closes the window so it can be redrawn with a new question
			window.close();
			// outputting that the answer is correct
			cout << "\ncorrect";
			// outputting the lives count
			cout << "\nyou have " << lives << " lives remaining";
		}
		// if the answers dont match
		else {
			// outputting incorrect
			cout << "\nincorrect";
			// decrementing the lives
			lives -= 1;
			// outputting the correct answer
			cout << " the correct answer was " << globalobj.finalCorrectAnswer;
			// outputting lives count
			cout << "\nyou have " << lives << " lives remaining";
		}

	}
	// creating an object for the class gameOver
	gameOver gameoverObj;
	// calling the function Gameover using the gameOver class object
	gameoverObj.Gameover();

	
}


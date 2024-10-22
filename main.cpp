#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <regex>
#include <chrono>
#include <thread>


const std::string ANSI_COLOR_RESET = "\033[0m";
const std::string ANSI_COLOR_GREEN = "\033[1;32m";
const std::string ANSI_COLOR_RED = "\033[1;31m";
const std::string ANSI_COLOR_YELLOW = "\033[1;33m";
const std::string ANSI_COLOR_BLUE = "\033[1;34m";


std::vector<std::string> split(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}


std::map<std::string, std::vector<std::string>> buildMarkovModel(const std::vector<std::string> &words, int order) {
    std::map<std::string, std::vector<std::string>> markovModel;
    for (size_t i = 0; i < words.size() - order; ++i) {
        std::string key;
        for (int j = 0; j < order; ++j) {
            key += words[i + j] + " ";
        }
        key.pop_back(); // Remove the trailing space
        markovModel[key].push_back(words[i + order]);
    }
    return markovModel;
}


std::string generateResponse(const std::map<std::string, std::vector<std::string>> &markovModel, const std::string &startPhrase) {
    std::string response = startPhrase;
    std::string currentPhrase = startPhrase;

    for (int i = 0; i < 100; ++i) { // Limit the length of the response
        auto it = markovModel.find(currentPhrase);
        if (it == markovModel.end() || it->second.empty()) {
            break;
        }
        const std::vector<std::string> &nextWords = it->second;
        std::string nextWord = nextWords[rand() % nextWords.size()];
        response += " " + nextWord;

        std::vector<std::string> currentWords = split(currentPhrase, ' ');
        currentWords.erase(currentWords.begin());
        currentWords.push_back(nextWord);
        currentPhrase = "";
        for (const std::string &word : currentWords) {
            currentPhrase += word + " ";
        }
        currentPhrase.pop_back(); // Remove the trailing space

        if (nextWord.back() == '.' || nextWord.back() == '?' || nextWord.back() == '!') {
            break; // Stop the response after a period, question mark, or exclamation mark
        }
    }
    return response;
}


std::vector<std::string> readDataFile(const std::string &fileName) {
    std::ifstream file(fileName);
    std::vector<std::string> words;
    std::string line, word;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::transform(line.begin(), line.end(), line.begin(), ::tolower); // Convert to lowercase
            std::vector<std::string> lineWords = split(line, ' ');
            words.insert(words.end(), lineWords.begin(), lineWords.end());
        }
        file.close();
    } else {
        std::cerr << ANSI_COLOR_RED << "Unable to open file: " << fileName << ANSI_COLOR_RESET << std::endl;
    }
    return words;
}


std::map<std::string, std::string> readDefinitionsFile(const std::string &fileName) {
    std::ifstream file(fileName);
    std::map<std::string, std::string> definitions;
    std::string line, word, definition;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::size_t pos = line.find(" - ");
            if (pos != std::string::npos) {
                word = line.substr(0, pos);
                definition = line.substr(pos + 3);
                definitions[word] = definition;
            }
        }
        file.close();
    } else {
        std::cerr << ANSI_COLOR_RED << "Unable to open file: " << fileName << ANSI_COLOR_RESET << std::endl;
    }
    return definitions;
}


std::set<std::string> loadInappropriateWords(const std::string &fileName) {
    std::ifstream file(fileName);
    std::set<std::string> inappropriateWords;
    std::string word;

    if (file.is_open()) {
        while (file >> word) {
            inappropriateWords.insert(word);
        }
        file.close();
    } else {
        std::cerr << ANSI_COLOR_RED << "Unable to open file: " << fileName << ANSI_COLOR_RESET << std::endl;
    }
    return inappropriateWords;
}


bool containsInappropriateWord(const std::string &question, const std::set<std::string> &inappropriateWords) {
    std::vector<std::string> words = split(question, ' ');
    for (const auto &word : words) {
        if (inappropriateWords.find(word) != inappropriateWords.end()) {
            return true;
        }
    }
    return false;
}


bool isPositiveSentiment(const std::string &question) {
    std::string positiveKeywords[] = {"happy", "good", "great", "love", "like"};
    for (const auto &keyword : positiveKeywords) {
        if (question.find(keyword) != std::string::npos) {
            return true;
        }
    }
    return false;
}


void printWithAnimation(const std::string &text, int delay) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}


void printGreen(const std::string &text) {
    std::cout << ANSI_COLOR_GREEN;
    printWithAnimation(text, 20);
    std::cout << ANSI_COLOR_RESET;
}


void printRed(const std::string &text) {
    std::cout << ANSI_COLOR_RED;
    printWithAnimation(text, 20);
    std::cout << ANSI_COLOR_RESET;
}


void printYellow(const std::string &text) {
    std::cout << ANSI_COLOR_YELLOW;
    printWithAnimation(text, 20);
    std::cout << ANSI_COLOR_RESET;
}


void printBlue(const std::string &text) {
    std::cout << ANSI_COLOR_BLUE;
    printWithAnimation(text, 20);
    std::cout << ANSI_COLOR_RESET;
}


bool isDefinitionQuestion(const std::string &question) {
    std::vector<std::string> questionWords = split(question, ' ');
    if (questionWords.size() > 2) {
        std::string firstWord = questionWords[0];
        std::string secondWord = questionWords[1];
        std::transform(firstWord.begin(), firstWord.end(), firstWord.begin(), ::tolower);
        std::transform(secondWord.begin(), secondWord.end(), secondWord.begin(), ::tolower);
        return (firstWord == "what" && secondWord == "is") || (firstWord == "who" && secondWord == "is");
    }
    return false;
}


std::string extractKeyword(const std::string &question) {
    std::vector<std::string> questionWords = split(question, ' ');
    if (questionWords.size() > 2) {
        return questionWords[2]; // Assume the keyword is the third word in the question
    }
    return "";
}

int main() {
    srand(time(nullptr)); // Initialize the random number generator

    std::cout <<
        "     /$$      /$$  /$$$$$$  /$$$$$$        \n"
        "    | $$$    /$$$ /$$__  $$|_  $$_/  /$$   \n"
        "    | $$$$  /$$$$| $$  \\ $$  | $$   | $$   \n"
        "    | $$ $$/$$ $$| $$$$$$$$  | $$ /$$$$$$$$\n"
        "    | $$  $$$| $$| $$__  $$  | $$|__  $$__/\n"
        "    | $$\\  $ | $$| $$  | $$  | $$   | $$   \n"
        "    | $$ \\/  | $$| $$  | $$ /$$$$$$ |__/   \n"
        "    |__/     |__/|__/  |__/|______/         \n";

    std::cout << "\n";
    std::cout << "           MADE BY: " << ANSI_COLOR_BLUE << "PENCILBLUE0" << ANSI_COLOR_RESET << std::endl;
    std::cout << "\n";
    std::cout << "V.1.0\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "==================================================\n";
    std::cout << "" << std::endl;

    std::string name;
    std::cout << "--Hello, what is your name?" << std::endl;
    std::cin >> name;
    std::cout << "--Hello, " << name << ". You can ask your questions now (type 'exit' to quit)." << std::endl;

    std::cin.ignore(); // Ignore any newline characters left in the input buffer after reading the user's name

    
    std::vector<std::string> words = readDataFile("data.txt");

    
    std::map<std::string, std::string> definitions = readDefinitionsFile("data.txt");

   
    std::set<std::string> inappropriateWords = loadInappropriateWords("inappropriate_word.txt");

   
    std::vector<std::string> questionPrompts = {
        "--What would you like to know?",
        "--I'm here to help. What would you like to know?",
        "--Feel free to ask anything!",
        "--What's on your mind today?",
        "--How can I assist you?"
    };

    
    std::map<std::string, std::vector<std::string>> markovModel = buildMarkovModel(words, 3);

    while (true) {
        std::string question;
        
        std::cout << questionPrompts[rand() % questionPrompts.size()] << std::endl;
        std::getline(std::cin, question);

        if (question == "exit") {
            break;
        }

        
        if (containsInappropriateWord(question, inappropriateWords)) {
            printRed("--Sorry, I can't respond to that.");
            std::cout << std::endl;
            continue;
        }

        
        if (isDefinitionQuestion(question)) {
            std::string keyword = extractKeyword(question);
            auto it = definitions.find(keyword);
            if (it != definitions.end()) {
                printGreen("--Response:");
                std::cout << " ";
                printWithAnimation(it->second, 30);
                std::cout << std::endl;
            } else {
                printRed("--Sorry, I couldn't find an explanation for that in my database.");
                std::cout << std::endl;
            }
            continue;
        }

        
        bool positiveSentiment = isPositiveSentiment(question);

       
        std::string response = generateResponse(markovModel, question);

        
        if (positiveSentiment) {
            printGreen("--Response:");
        } else {
            printYellow("--Response:");
        }
        std::cout << " ";
        printWithAnimation(response, 30);
        std::cout << std::endl;
        std::cout << std::endl;

        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    printBlue("--Goodbye!");
    std::cout << std::endl;

    return 0;
}

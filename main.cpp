#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <set>

// ANSI escape codes for colored output
const std::string ANSI_COLOR_RESET = "\033[0m";
const std::string ANSI_COLOR_GREEN = "\033[1;32m";
const std::string ANSI_COLOR_RED = "\033[1;31m";
const std::string ANSI_COLOR_YELLOW = "\033[1;33m";
const std::string ANSI_COLOR_BLUE = "\033[1;34m";

// Function to split a string into words
std::vector<std::string> split(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to build the Markov model of higher order (N-grams)
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

// Function to generate a response using a higher-order Markov model
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

// Function to read the text file and extract words
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

// Function to load inappropriate words from a file
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

// Function to check for inappropriate words
bool containsInappropriateWord(const std::string &question, const std::set<std::string> &inappropriateWords) {
    std::vector<std::string> words = split(question, ' ');
    for (const auto &word : words) {
        if (inappropriateWords.find(word) != inappropriateWords.end()) {
            return true;
        }
    }
    return false;
}

// Function to perform sentiment analysis
bool isPositiveSentiment(const std::string &question) {
    // Simplified sentiment analysis based on keywords or patterns
    std::string positiveKeywords[] = {"happy", "good", "great", "love", "like"};
    for (const auto &keyword : positiveKeywords) {
        if (question.find(keyword) != std::string::npos) {
            return true;
        }
    }
    return false;
}

// Function to print text in green color
void printGreen(const std::string &text) {
    std::cout << ANSI_COLOR_GREEN << text << ANSI_COLOR_RESET; // Print in green
}

// Function to print text in red color
void printRed(const std::string &text) {
    std::cout << ANSI_COLOR_RED << text << ANSI_COLOR_RESET; // Print in red
}

// Function to print text in yellow color
void printYellow(const std::string &text) {
    std::cout << ANSI_COLOR_YELLOW << text << ANSI_COLOR_RESET; // Print in yellow
}

// Function to print text in blue color
void printBlue(const std::string &text) {
    std::cout << ANSI_COLOR_BLUE << text << ANSI_COLOR_RESET; // Print in blue
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

    // Read the data file
    std::vector<std::string> words = readDataFile("data.txt");

    // Load inappropriate words
    std::set<std::string> inappropriateWords = loadInappropriateWords("inappropriate_word.txt");

    std::cin.ignore(); // Ignore any newline characters left in the input buffer after reading the user's name

    // Array of question prompts
    std::vector<std::string> questionPrompts = {
        "--What is your question?",
        "--I'm here to help. What would you like to know?",
        "--Feel free to ask anything!",
        "--What's on your mind today?",
        "--How can I assist you?"
    };

    std::map<std::string, std::vector<std::string>> markovModel = buildMarkovModel(words, 3); // Build a tri-gram Markov model initially

    while (true) {
        std::string question;
        // Randomly select a question prompt
        std::cout << questionPrompts[rand() % questionPrompts.size()] << std::endl;
        std::getline(std::cin, question);

        if (question == "exit") {
            break;
        }

        // Check for inappropriate words
        if (containsInappropriateWord(question, inappropriateWords)) {
            printRed("--Sorry, I can't respond to that.");
            std::cout << std::endl;
            continue;
        }

        // Perform sentiment analysis
        bool positiveSentiment = isPositiveSentiment(question);

        // Generate a response using the Markov model
        std::string response = generateResponse(markovModel, question);

        // Print the response with appropriate color
        if (positiveSentiment) {
            printGreen("--Response:");
        } else {
            printYellow("--Response:");
        }
        std::cout << " " << response << std::endl;
        std::cout << std::endl;
    }

    printBlue("--Goodbye!");
    std::cout << std::endl;

    return 0;
}

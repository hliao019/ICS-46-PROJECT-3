// WordChecker.cpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"



WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    return words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
      std::vector<std::string> suggestions;
    std::string alphabet ="ABCDEFGHIJKLMNOPQRSTUVWXYZ";



    //Swapping
    for (int i = 0; i < word.size() - 1; i++)
    {
        std::string temp = word;
        std::swap(temp[i], temp[i+1]);
        if (wordExists(temp) && (std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end()))
            suggestions.push_back(temp);
    }




    //Insert
    for (int i = 0; i <= word.size(); i++)
    {
        for (int x = 0; x < alphabet.size(); x++)
        {
            std::string temp = word;
            temp.insert(i, alphabet.substr(x,1));
            if (wordExists(temp) && (std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end()))
                suggestions.push_back(temp);
        }
    }


    //Delete
    for (int i = 0; i < word.size(); i++)
    {
        std::string temp = word;
        temp = temp.erase(i,1);
        if (wordExists(temp) && (std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end()))
            suggestions.push_back(temp);
    }


    //Replace
    for (int i = 0; i < word.size(); i++)
    {

        for (int y = 0; y < alphabet.size(); y++)
        {
            std::string temp = word;
            temp[i] = alphabet[y]; // might have to use replace
            if (wordExists(temp) && (std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end()))
                suggestions.push_back(temp);
        }

    }


    //Split
    for (int i = 1; i < word.size(); i++)
    {
        std::string temp1 = word.substr(0,i);
        std::string temp2 = word.substr(i);
        std::string temp = temp1 + " " + temp2;
        if (wordExists(temp1) && wordExists(temp2) && (std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end()))
            suggestions.push_back(temp);
    }



    return suggestions;
}


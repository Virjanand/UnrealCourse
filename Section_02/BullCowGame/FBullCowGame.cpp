#pragma once

#include "FBullCowGame.h"
#include <map>
#include <iostream>
#define TMap std::map // to make syntax Unreal friendly

FBullCowGame::FBullCowGame()
{
	Reset();
}

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const {	return bGameWon; }

void FBullCowGame::SetMyHiddenWord() // word MUST be an isogram
{
	int32 HiddenWordLength;
	std::cout << "Enter a word length between 3 and 7: ";
	std::cin >> HiddenWordLength;
	
	switch (HiddenWordLength)
	{
	case 3:
		MyHiddenWord = "fly";
		break;
	case 4:
		MyHiddenWord = "help";
		break;
	case 5:
		MyHiddenWord = "human";
		break;
	case 6:
		MyHiddenWord = "planes";
		break;
	case 7:
		MyHiddenWord = "isogram";
		break;
	default:
		MyHiddenWord = "plane";
		break;
	}
}

int32 FBullCowGame::GetMaxTries() const { 
	TMap<int32, int32> WordLengthToMaxTries{ {3,4}, {4,7}, {5,10}, {6,16}, {7, 20} };
	return WordLengthToMaxTries[MyHiddenWord.length()]; 
}

void FBullCowGame::Reset()
{ 
	MyCurrentTry = 1;
	bGameWon = false;
	return;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsIsogram(Guess)) // if the guess isn't an isogram
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowerCase(Guess)) // if the guess isn't all lowercase 
	{
		return EGuessStatus::Not_Lowercase; // TODO write function
	}
	else if (Guess.length() != MyHiddenWord.length()) // if the guess length is wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else
	{
		return EGuessStatus::OK;
	}
}

// receives a VALID guess, increments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length(); // assuming same length as guess

	// loop through all letters in the hidden word
	
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++) {
		// compare letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++) {
			// if they match then
			if (Guess[GChar] == MyHiddenWord[MHWChar]) {
				if (MHWChar == GChar) { // if they're in the same place
					BullCowCount.Bulls++; // increment bulls 
				}
				else {
					BullCowCount.Cows++; // must be a cow
				}
			}
		}
	}

	// Check if won and set win condition
	if (BullCowCount.Bulls == WordLength)
	{
		bGameWon = true;
	}
	else 
	{
		bGameWon = false;
	}

	return BullCowCount;
}

bool FBullCowGame::IsIsogram(FString Word) const
{

	// treat 0 or 1 letter words as isograms
	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen; // setup our map
	for (auto Letter : Word)	 // for all letters of the word
	{
		Letter = tolower(Letter); // handle mixed case
		
		if (LetterSeen[Letter]) {// if letter is in the map
			return false; // we do not have an isogram
		} else {		
			LetterSeen[Letter] = true; // add the letter to the map as seen
		}
	}

	return true; // for example in cases where /0 is entered
}

bool FBullCowGame::IsLowerCase(FString Word) const
{
	if (Word[0] == '\0') { return true; } // treat 0 letter words as lowercase
	
	for (auto Letter : Word) {// for all letters of the word
		if (!islower(Letter)) { // if letter is not lowercase
			return false; // we do not have all lowercase word
		}
	}
	return true;
}

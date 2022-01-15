// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    SetupGame();
    ShowPreview();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();
    ShowPreview();

    if (bGameOver)
    {
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    const FString WordListPath = FPaths::ProjectContentDir()/TEXT("WordsList/HiddenWordsList.txt");
    FFileHelper::LoadFileToStringArray(HiddenWords, *WordListPath);

    PrintLine(TEXT("Number of possible words: %i"), HiddenWords.Num());

    TArray<FString> ValidHiddenWords = GetValidWords(HiddenWords);

    PrintLine(TEXT("Number of possible valid words: %i"), ValidHiddenWords.Num());

    HiddenWord = ValidHiddenWords[FMath::RandRange(0, ValidHiddenWords.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;
}

void UBullCowCartridge::ShowPreview()
{
    PrintLine(FString::Printf(TEXT("The hidden word is: %s"), *HiddenWord)); // Debug line

    PrintLine(TEXT("Hi there!"));
    PrintLine(FString::Printf(TEXT("Try to guess the %i letters word!\n"), HiddenWord.Len()));
}

void UBullCowCartridge::EndGame()
{
    PrintLine(TEXT("\nPress Enter to play again!"));
    bGameOver = true;
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        ClearScreen();
        PrintLine(TEXT("Your guess was right! You win!"));
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(FString::Printf(TEXT("Hidden word has %i letters!"), HiddenWord.Len()));
        return;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("Your guess contains repeating letters! Use an isogram for a guess!"));
        return;
    }

    PrintLine(TEXT("Your guess was wrong and you lost a life! Try again!"));
    PrintLine(TEXT("You have %i lives!"), --Lives);

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You lost all your lives! Game over!"));
        PrintLine(TEXT("Hidden word was: %s"), *HiddenWord);
        EndGame();
    }
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len() - 1; Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& Words) const
{
    TArray<FString> ValidWords;

    for (FString Word : Words)
    {
        int32 WordLen = Word.Len();
        if (WordLen >= 4 && WordLen <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}
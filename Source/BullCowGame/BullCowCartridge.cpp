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
        ClearScreen();
        ShowPreview();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    const FString WordListPath = FPaths::ProjectContentDir()/TEXT("WordsList/HiddenWordsList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(HiddenWords, *WordListPath, [](const FString& Word)
    {
        int32 WordLen = Word.Len();
        return WordLen >= 4 && WordLen <= 8 && IsIsogram(Word);
    });

    PrintLine(TEXT("Number of possible words: %i"), HiddenWords.Num());

    HiddenWord = HiddenWords[FMath::RandRange(0, HiddenWords.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;
}

void UBullCowCartridge::ShowPreview()
{
    //PrintLine(FString::Printf(TEXT("The hidden word is: %s"), *HiddenWord)); // Debug line

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

    FBullCowCount Score = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows!"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("You have %i lives!"), --Lives);

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You lost all your lives! Game over!"));
        PrintLine(TEXT("Hidden word was: %s"), *HiddenWord);
        EndGame();
    }
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
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

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 g = 0; g < Guess.Len(); g++)
    {
        if (Guess[g] == HiddenWord[g])
        {
            Count.Bulls++;
            continue;
        }
        
        for (int32 h = 0; h < HiddenWord.Len(); h++)
        {
            if (Guess[g] == HiddenWord[h])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}
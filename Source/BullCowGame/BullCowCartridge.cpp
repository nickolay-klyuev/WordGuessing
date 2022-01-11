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
        if (Input == HiddenWord)
        {
            PrintLine(TEXT("Your guess was right! You win! \nPress Enter to play again!"));
            bGameOver = true;
        }
        else
        {
            if (Input.Len() != HiddenWord.Len())
            {
                PrintLine(FString::Printf(TEXT("Hidden word has %i letters!"), HiddenWord.Len()));
            }

            PrintLine(TEXT("Your guess was wrong and you lost a life! Try again!"));
            PrintLine(TEXT("You have %i lives!"), --Lives);

            if (Lives <= 0)
            {
                PrintLine(TEXT("You lost all your lives! Game over!\nPress Enter to play again!"));
                bGameOver = true;
            }
        }
    }
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = TEXT("sucker");
    Lives = HiddenWord.Len();
    bGameOver = false;
}

void UBullCowCartridge::ShowPreview()
{
    PrintLine(FString::Printf(TEXT("The hidden word is: %s"), *HiddenWord)); // Debug line

    PrintLine(TEXT("Hi there!"));
    PrintLine(FString::Printf(TEXT("Try to guess the %i letters word!\n"), HiddenWord.Len()));
}
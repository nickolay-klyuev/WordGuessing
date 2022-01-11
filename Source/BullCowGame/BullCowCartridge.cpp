// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    HiddenWord = TEXT("word");

    PrintLine(TEXT("Hi there!"));
    PrintLine(TEXT("Try to guess the 4 letters word!"));
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();
    
    if (Input == HiddenWord)
    {
        PrintLine(TEXT("Your guess was right! You win! Take your reward!"));
    }
    else
    {
        PrintLine(TEXT("Your guess was wrong! Try again!"));
    }
}
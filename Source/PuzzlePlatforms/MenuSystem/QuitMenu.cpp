// Fill out your copyright notice in the Description page of Project Settings.


#include "QuitMenu.h"
#include "../PuzzlePlatformsGameInstance.h"
#include "Components/Button.h"

bool UQuitMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UQuitMenu::CancelMenu);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UQuitMenu::BackToMainMenu);



	return true;
}

void UQuitMenu::CancelMenu()
{
	Teardown();
}

void UQuitMenu::BackToMainMenu()
{

	if (MenuInterface != nullptr)
	{
		Teardown();
		MenuInterface->LoadMainMenu();
	}

}

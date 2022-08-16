// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHudWidget.h"

#include "Weapon_Master.h"
void UFPSHudWidget::UpdateWeapon(AWeapon_Master* NewWeapon)
{
	Weapon = NewWeapon;

}
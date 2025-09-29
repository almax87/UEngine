// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "Components/LMAWeaponComponent.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/LMABaseWeapon.h"

ULMAWeaponComponent::ULMAWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULMAWeaponComponent::Fire()
{
	if (CanFire())
	{
		Weapon->Fire();
	}
}

void ULMAWeaponComponent::StopFire()
{
	if (Weapon)
	{
		Weapon->StopFire();
	}
}

void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
	InitAnimNotify();
}

void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULMAWeaponComponent::SpawnWeapon()
{
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
	if (Weapon)
	{
		const auto Character = Cast<ACharacter>(GetOwner());
		if (Character)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "r_Weapon_Socket");

			// Подписываемся на делегат окончания патронов
			Weapon->OnClipEmpty.AddUObject(this, &ULMAWeaponComponent::OnClipEmpty);
		}
	}
}

void ULMAWeaponComponent::InitAnimNotify()
{
	if (!ReloadMontage)
		return;

	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinish)
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character && Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
		if (Weapon)
		{
			Weapon->ChangeClip();
			UE_LOG(LogTemp, Warning, TEXT("Reload finished - can fire now"));
		}
	}
}


void ULMAWeaponComponent::OnClipEmpty()
{
	AutoReload();
}

bool ULMAWeaponComponent::CanReload() const
{
	return !AnimReloading && Weapon != nullptr;
}

bool ULMAWeaponComponent::CanFire() const
{
	return Weapon && !AnimReloading && !Weapon->IsCurrentClipEmpty();
}

void ULMAWeaponComponent::Reload()
{
	if (!CanReload())
		return;

	AutoReload();
}

void ULMAWeaponComponent::AutoReload()
{
	if (!Weapon || AnimReloading)
		return;

	// Останавливаем стрельбу перед перезарядкой
	StopFire();

	AnimReloading = true;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character && ReloadMontage)
	{
		Character->PlayAnimMontage(ReloadMontage);
		UE_LOG(LogTemp, Warning, TEXT("Starting reload animation"));
	}
	else
	{
		// Если нет анимации, сразу завершаем перезарядку
		AnimReloading = false;
		Weapon->ChangeClip();
	}
}

bool ULMAWeaponComponent::GetCurrentWeaponAmmo(FAmmoWeapon& AmmoWeapon) const
{
	if (Weapon)
	{
		AmmoWeapon = Weapon->GetCurrentAmmoWeapon();
		return true;
	}
	return false;
}

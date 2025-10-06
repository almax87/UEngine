// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class ULMAWeaponComponent;
class UAnimMontage;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALMADefaultCharacter();

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }

		UFUNCTION(BlueprintCallable, Category = "Sprint")
	bool IsSprinting() const { return bIsSprinting; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	    // Sprint variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sprint")
	float MaxStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sprint")
	float StaminaDrainRate = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sprint")
	float StaminaRegenRate = 15.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sprint")
	float SprintSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sprint")
	float NormalSpeed = 300.0f;

	// Sprint functions
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	bool CanSprint() const;

	UFUNCTION(BlueprintCallable, Category = "Sprint")
	float GetStamina() const { return CurrentStamina; }


	virtual void BeginPlay() override;


public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	ULMAWeaponComponent* WeaponComponent;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	float YRotation = -75.0f;

	float ArmLength = 1400.0f;

	float FOV = 55.0f;

	bool bIsSprinting = false;
	float CurrentStamina = 0.0f;

	void StartSprint();
	void StopSprint();

	void MoveForward(float Value);
	void MoveRight(float Value);

	void OnDeath();
	void OnHealthChanged(float NewHealth);

	void RotationPlayerOnCursor();

	    // Stamina management
	void UpdateStamina(float DeltaTime);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "DAO/SSDAOManager.h"

USSDAOObjectBase* USSDAOManager::GetDAO(TSubclassOf<USSDAOObjectBase> DAOClass)
{
	for (auto* DAOObject : DAOObjects)
	{
		if (DAOObject)
		{
			return DAOObject;
		}
	}

	auto* DAOObject = NewObject<USSDAOObjectBase>(this, DAOClass);
	DAOObjects.Add(DAOObject);

	return DAOObject;
}

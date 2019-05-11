#pragma once

#include "InventoryItem.h"
#include "VisualEffect.h"

class HealthCharge
	:
	public InventoryItem
{
public:
	HealthCharge()
		:
		InventoryItem( "ItemDescriptions/HealthCharge.txt",
			"Images/Items/HealthCharge.bmp" )
	{}

	void OnPlayerHit( InventoryEventInfo& evtInfo ) override
	{
		for( const auto& enemyBull : evtInfo.enemyBullets )
		{
			evtInfo.visualEffects.emplace_back(
				VisualEffect{ enemyBull->GetPos(),
				VisualEffect::Type::LightningDissipate } );

			remove = true;
		}

		evtInfo.enemyBullets.clear();
	}
};
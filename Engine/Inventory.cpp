#include "Inventory.h"

Inventory::Inventory()
{
	// for( int i = 0; i < size.x * size.y; ++i )
	// {
	// 	AddItem( "Hi","test desc","Images/Wall2.bmp" );
	// }
}

void Inventory::Update( const Keyboard& kbd,const Mouse& mouse,
	InventoryEventInfo& invEvtInfo )
{
	if( kbd.KeyIsPressed( 'I' ) || kbd.KeyIsPressed( VK_TAB ) )
	{
		if( active && canToggle )
		{
			active = false;
		}
		else if( !active && canToggle )
		{
			active = true;
		}

		canToggle = false;
	}
	else canToggle = true;

	for( auto& item : items )
	{
		item->OnUpdate( invEvtInfo );
	}

	if( mouse.LeftIsPressed() )
	{
		items[0]->OnActivate( invEvtInfo );
	}
	if( kbd.KeyIsPressed( VK_SHIFT ) )
	{
		items[1]->OnActivate( invEvtInfo );
	}
	if( mouse.RightIsPressed() )
	{
		items[2]->OnActivate( invEvtInfo );
	}

	if( !active ) return; // -----------------------------

	for( auto& item : items )
	{
		item->Update( mouse );
	}

	for( auto it = items.begin(); it != items.end(); )
	{
		if( ( *it )->WillRemove() )
		{
			// ShiftItems( it );
			items.erase( it );
			ReorganizeInventory();
			it = items.begin();
		}
		else
		{
			if( mouse.LeftIsPressed() )
			{
				if( ( *it )->GetRect().ContainsPoint(
					mouse.GetPos() ) && !holdingItem )
				{
					selectedItem = it;
					holdingItem = true;
				}
			}
			else if( ( ( *it )->GetRect().ContainsPoint(
				mouse.GetPos() ) ) && holdingItem )
			{
				std::iter_swap( it,selectedItem );
				holdingItem = false;
				ReorganizeInventory();
				break;
			}

			++it;
		}
	}

	if( holdingItem ) heldItemPos = mouse.GetPos();
}

void Inventory::Draw( Graphics& gfx ) const
{
	if( active )
	{
		const auto drawPos = invStart - itemPadding;

		gfx.DrawRect( drawPos.x,drawPos.y,
			invSize.x,invSize.y,
			Colors::Gray );

		DrawInvGrid( gfx );

		for( const auto& item : items )
		{
			if( item->IsSelected() )
			{
				// gfx.DrawRect( nameStart.x - itemPadding.x,
				// 	nameStart.y - itemPadding.y,
				// 	invSize.x,invSize.y,Colors::Gray );

				luckyPixel.DrawText( item->GetName(),
					nameStart,Colors::White,gfx );

				luckyPixel.DrawText( item->GetDesc(),
					descStart,Colors::White,gfx );
			}
		}

		gfx.DrawSprite( invStart.x,invStart.y -
			invInstructions.GetHeight(),
			invInstructions,
			SpriteEffect::Chroma{ Colors::Magenta } );

		if( holdingItem )
		{
			( *selectedItem )->Draw( heldItemPos,gfx );
		}
	}
}

// void Inventory::AddItem( const std::string& name,
// 	const std::string& desc,const std::string& icon )
// {
// 	AddItem( new InventoryItem( name,desc,icon ) );
// }

void Inventory::AddItem( InventoryItem* itemToAdd )
{
	items.emplace_back( itemToAdd );

	if( ( items.size() - 1 ) % size.x == 0 &&
		items.size() > 1 )
	{
		items.back()->SetPos( items[items.size() - 2]->GetPos().Y() +
			itemSize.Y() + itemPadding.Y() + invStart.X() );
	}
	else if( items.size() > 1 )
	{
		items.back()->SetPos( items[items.size() - 2]->GetPos() +
			itemSize.X() + itemPadding.X() );
	}
	else
	{
		items.back()->SetPos( invStart );
	}
}

void Inventory::ConsumeItem( const std::string& name )
{
	assert( false ); // TODO: Fix this if you ever want to use it.

	for( auto it = items.begin(); it != items.end(); ++it )
	{
		if( ( *it )->GetName() == name )
		{
			items.erase( it );
			break;
		}
	}

	// ShiftItems();
}

void Inventory::OnPlayerHit( InventoryEventInfo& evtInfo )
{
	for( auto& item : items )
	{
		item->OnPlayerHit( evtInfo );
	}
}

void Inventory::OnPlayerShoot( InventoryEventInfo& evtInfo )
{
	for( auto& item : items )
	{
		item->OnPlayerShoot( evtInfo );
	}
}

void Inventory::OnEnemyExplode( InventoryEventInfo& evtInfo )
{
	for( auto& item : items )
	{
		item->OnEnemyExplode( evtInfo );
	}
}

InventoryItem* Inventory::FindItem( const std::string& name )
{
	for( auto& item : items )
	{
		if( item->GetName() == name )
		{
			return( item.get() );
		}
	}

	return( nullptr );
}

bool Inventory::IsOpen() const
{
	return( active );
}

void Inventory::DrawInvGrid( Graphics& gfx ) const
{
	// auto curPos = invStart;
	for( int y = 0; y < size.y; ++y )
	{
		for( int x = 0; x < size.x; ++x )
		{
			if( y * size.x + x > int( items.size() ) - 1 ) return;
			items[y * size.x + x]->Draw( gfx );
			// curPos += itemSize.X();
			// curPos += itemPadding.X();
		}
		// curPos += itemSize.Y();
		// curPos += itemPadding.Y();
		// curPos.x = invStart.x;
	}
}

void Inventory::ShiftItems( std::vector<std::unique_ptr<
	InventoryItem>>::iterator spot )
{
	// Re-order items after removal.
	// for( int i = int( items.size() ) - 1; i > 0; --i )
	// {
	// 	items[i]->SetPos( items[i - 1]->GetPos() );
	// }
	for( auto it = items.end() - 1; it != spot; --it )
	{
		( *it )->SetPos( ( *( it - 1 ) )->GetPos() );
	}
}

void Inventory::ReorganizeInventory()
{
	std::vector<std::unique_ptr<InventoryItem>> oldItems;
	for( auto& item : items )
	{
		oldItems.emplace_back( item->Clone() );
	}

	items.clear();

	for( auto& item : oldItems )
	{
		AddItem( item->Clone() );
	}
}

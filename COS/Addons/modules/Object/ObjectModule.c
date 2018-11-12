class ObjectModule: EditorModule
{
    void ObjectModule()
    {   
        GetRPCManager().AddRPC( "COS_Object", "SpawnObjectPosition", this, SingeplayerExecutionType.Server );
        GetRPCManager().AddRPC( "COS_Object", "SpawnObjectInventory", this, SingeplayerExecutionType.Server );

        GetPermissionsManager().RegisterPermission( "Object.Spawn.Position" );
        GetPermissionsManager().RegisterPermission( "Object.Spawn.Inventory" );
    }

    override string GetLayoutRoot()
    {
        return "COS/Modules/Object/gui/layouts/ObjectMenu.layout";
    }
    
    void SpawnObjectPosition( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
    {
        if ( !GetPermissionsManager().HasPermission( sender, "Object.Spawn.Position" ) )
            return;

        Param3< string, vector, string > data;
        if ( !ctx.Read( data ) ) return;
        
        if( type == CallType.Server )
        {
            bool ai = false;

            if ( GetGame().IsKindOf( data.param1, "DZ_LightAI" ) ) 
            {
                ai = true;
            }

            EntityAI entity = GetGame().CreateObject( data.param1, data.param2, false, ai );

            entity.SetHealth( entity.GetMaxHealth() );

            if ( entity.IsInherited( ItemBase ) )
            {
                ItemBase oItem = ( ItemBase ) entity;
                SetupSpawnedItem( oItem, oItem.GetMaxHealth(), 1 );

                int quantity = 0;

                string text = data.param3;

                text.ToUpper();

                if (text == "MAX")
                {
                    quantity = oItem.GetQuantityMax();
                } else
                {
                    quantity = text.ToInt();
                }

                oItem.SetQuantity(quantity);
            }

            entity.PlaceOnSurface();
        }
    }

    void SpawnObjectInventory( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
    {
        if ( !GetPermissionsManager().HasPermission( sender, "Object.Spawn.Inventory" ) )
            return;

        ref Param3< string, string, ref array< ref AuthPlayer > > data;
        if ( !ctx.Read( data ) ) return;
        
        if( type == CallType.Server )
        {
            for ( int i = 0; i < data.param3.Count(); i++ )
            {
                EntityAI entity = data.param3[i].GetPlayerObject().GetInventory().CreateInInventory( data.param1 );

                entity.SetHealth( entity.GetMaxHealth() );

                if ( entity.IsInherited( ItemBase ) )
                {
                    ItemBase oItem = ( ItemBase ) entity;
                    SetupSpawnedItem( oItem, oItem.GetMaxHealth(), 1 );

                    int quantity = 0;

                    string text = data.param2;

                    text.ToUpper();

                    if (text == "MAX")
                    {
                        quantity = oItem.GetQuantityMax();
                    } else
                    {
                        quantity = text.ToInt();
                    }

                    oItem.SetQuantity(quantity);
                }
            }
        }
    }
}
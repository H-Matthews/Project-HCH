#include "core/inc/GameAssetContainer/GameAssetContainer.hpp"

bool Core::GameAssetContainer::initializeTextures()
{
    // TODO probably remove this later on and add this logic to be baked into the sprites themselves.
    // That way they are built into the binary.
    if ( !loadTexture( GameAssetType::ERROR_TEXTURE, "../../../gameAssets/textures/errorSprite.png" ) )
    {
        return false;
    }
    if ( !loadTexture( GameAssetType::PLAYER_TEXTURE, "../../../gameAssets/textures/playerSprite.png" ) )
    {
        return false;
    }
    if ( !loadTexture( GameAssetType::ENEMY_TEXTURE, "../../../gameAssets/textures/enemySprite.png" ) )
    {
        return false;
    }
    return true;
}

bool Core::GameAssetContainer::initializeFonts()
{
    // TODO probably remove this later on and add this logic to be baked into the font themselves.
    // That way they are built into the binary. Not sure how possible this is but I know you can do it with sprites.
    if ( !loadFont( GameAssetType::ERROR_FONT, "../../../gameAssets/fonts/error.ttf" ) )
    {
        return false;
    }
    if ( !loadFont( GameAssetType::ARIAL_FONT, "../../../gameAssets/fonts/arial.ttf" ) )
    {
        return false;
    }
    if ( !loadFont( GameAssetType::NASTY_FONT, "../../../gameAssets/fonts/nasty.ttf" ) )
    {
        return false;
    }
    return true;
}

const sf::Texture& Core::GameAssetContainer::getTexture( GameAssetType textureName )
{

    if ( textureMap.find( textureName ) != textureMap.end() )
    {
        return *textureMap[ textureName ].get();
    }
    // ERROR print and then exit program.
    printf( "[ERROR] Texture %d was referenced, but not loaded.\n", int( textureName ) );
    std::exit( 1 );
}

const sf::Font& Core::GameAssetContainer::getFont( GameAssetType fontName )
{

    if ( fontMap.find( fontName ) != fontMap.end() )
    {
        return *fontMap[ fontName ].get();
    }
    // ERROR print and then exit program.
    printf( "[ERROR] Font %d was referenced, but not loaded.\n", int( fontName ) );
    std::exit( 1 );
}

bool Core::GameAssetContainer::loadTexture( GameAssetType textureName, const std::string& filePath )
{
    auto tempTexture( std::make_unique< sf::Texture >() );

    if ( !tempTexture->loadFromFile( filePath ) )
    {
        return false;
    }

    textureMap.insert( { textureName, std::move( tempTexture ) } );

    return true;
}

bool Core::GameAssetContainer::loadFont( GameAssetType fontName, const std::string& filePath )
{
    auto tempFont( std::make_unique< sf::Font >() );

    if ( !tempFont->openFromFile( filePath ) )
    {
        return false;
    }

    fontMap.insert( { fontName, std::move( tempFont ) } );

    return true;
}

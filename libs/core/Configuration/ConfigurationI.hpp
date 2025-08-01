#pragma once

#include <string>
#include <bits/stdc++.h>

namespace Core
{

    enum DirectoryIDs
    {
        OUTPUT = 0,
        CONFIG,
        ASSETS,
        SIZE
    };

    /**
     * Defines our Configuration Interface
     */

    class ConfigurationI
    {
      public:
        ConfigurationI( const std::string& projectDirectory ) :
            mOutputDirPath( "" ),
            mConfigDirPath( "" ),
            mAssetDirPath( "" ),
            mAssetFontsDirPath( "" ),
            mAssetTexturesDirPath( "" ),
            mProjectDirectory( projectDirectory ),
            mDirectoryBits( 0 )
        {}

        virtual void initializeOutputDirectory() = 0;

        virtual void initializeConfigDirectory() = 0;

        virtual void initializeAssetsDirectory() = 0;

        virtual bool configure() = 0;

        inline void setDirectoryInit( DirectoryIDs directoryID );

        inline bool isInitialized();

        virtual ~ConfigurationI()
        {}

      public:
        std::string mOutputDirPath;
        std::string mConfigDirPath;

        std::string mAssetDirPath;
        std::string mAssetFontsDirPath;
        std::string mAssetTexturesDirPath;

        std::string mProjectDirectory;

      private:
        std::bitset< DirectoryIDs::SIZE > mDirectoryBits;
    };

    void ConfigurationI::setDirectoryInit( DirectoryIDs directoryID )
    {
        // Bitset will NOT be this large
        if ( directoryID == DirectoryIDs::SIZE )
            return;

        // IF the bit is NOT set, then set it
        if ( !mDirectoryBits.test( directoryID ) )
            mDirectoryBits.set( directoryID );

        return;
    }

    bool ConfigurationI::isInitialized()
    {
        for ( int i = 0; i < DirectoryIDs::SIZE; i++ )
        {
            if ( !mDirectoryBits.test( i ) )
                return false;
        }

        return true;
    }

}
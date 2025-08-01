#pragma once

#include <string>

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
        unsigned int mDirectoryBits : 3;
    };

    void ConfigurationI::setDirectoryInit( DirectoryIDs directoryID )
    {
        mDirectoryBits = mDirectoryBits | 1 << directoryID;

        return;
    }

    bool ConfigurationI::isInitialized()
    {
        // 7 is from 2^3 - 1
        return mDirectoryBits == 7;

        return true;
    }

}
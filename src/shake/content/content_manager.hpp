#ifndef CONTENT_MANAGER_HPP
#define CONTENT_MANAGER_HPP

#include <any>
#include <functional>
#include <memory>
#include <map>

#include "shake/core/contracts/contracts.hpp"
#include "shake/core/std/type_erased_map.hpp"
#include "shake/core/log.hpp"
#include "shake/core/macros/macro_debug_only.hpp"
#include "shake/core/macros/macro_non_copyable.hpp"
#include "shake/core/macros/macro_property.hpp"
#include "shake/core/macros/macro_strongly_typed_alias.hpp"
#include "shake/core/type_traits/type_id.hpp"
#include "shake/io/file.hpp"
#include "shake/io/path.hpp"

#include "shake/content/load_cube_map.hpp"
#include "shake/content/load_font.hpp"
#include "shake/content/load_material.hpp"
#include "shake/content/load_mesh.hpp"
#include "shake/content/load_program.hpp"
#include "shake/content/load_sprite.hpp"
#include "shake/content/load_texture.hpp"
#include "shake/content/load_voxel_grid.hpp"

namespace shake {
namespace content {

class ContentManager
{
private:

    //----------------------------------------------------------------
    // Because we want a user to be able to easily register new content types, 
    // we store all content loaders in a single map, 
    // and all content caches in a second map,
    // by erase their types.

    template<typename Content_T>
    using ContentLoader         = std::function<std::shared_ptr<Content_T>( ContentManager*, io::Path )>;
    using ContentLoaderRegistry = TypeErasedMap;

    template<typename Content_T>
    using ContentCache          = std::map<io::Path, std::shared_ptr<Content_T>>;
    using ContentCacheRegistry  = TypeErasedMap;

public:

    ContentManager() = default;
    NON_COPYABLE( ContentManager )

    //----------------------------------------------------------------
    void init()
    {
        load::init_font_loader();

        register_content_type<graphics::CubeMap>    ( load::load_cube_map   );
        register_content_type<graphics::Font>       ( load::load_font       );
        register_content_type<graphics::Material>   ( load::load_material   );
        register_content_type<graphics::Program>    ( load::load_program    );
        register_content_type<graphics::Texture>    ( load::load_texture    );
        //register_content_type<graphics::VoxelGrid>  ( load::load_voxel_grid );
    }

    //----------------------------------------------------------------
    // make sure to call the destroy function
    // before your contents' contexts (such as the graphics context) are destroyed,
    // otherwise you will get segmentation faults.
    void destroy()
    {
        m_content_cache_registry.clear();

        load::destroy_font_loader();
    }

    //----------------------------------------------------------------
    // Add a directory to host content from
    void host_content_directory( const io::Path& content_directory )
    {
        m_hosted_content_directories.emplace_back( content_directory );
    }

    //----------------------------------------------------------------
    void clear_hosted_content_directories()
    {
        m_hosted_content_directories.clear();
    }

    //----------------------------------------------------------------
    inline io::Path get_full_path( const io::Path& path )
    {
        for ( const auto& content_directory : m_hosted_content_directories )
        {
            const auto full_path = content_directory / path;
            if ( io::file::exists( full_path ) )
            {
                return full_path;
            }
        }
        CHECK_FAIL( "File does not exist: " + path.get_string() );
        return io::Path { "" }; // to shut up warning
    }

public:

    //----------------------------------------------------------------
    template<typename Content_T>
    void  register_content_type( const ContentLoader< Content_T >& loader_function )
    {
        m_content_loader_registry.emplace( loader_function );
        m_content_cache_registry.emplace( ContentCache<Content_T> { } );
    }

    //----------------------------------------------------------------
    // The content manager does some internal content caching for you.
    // You can however, also do some preloading and unloading of content.
    // The intended use is normally to preload() and unload() data at level transitions,
    // and use get() for everything else.

    //----------------------------------------------------------------
    // Used to load content to the content cache
    template<typename Content_T> inline
    const std::shared_ptr<Content_T>& preload( const io::Path& path )
    {
        const auto full_path = get_full_path( path ); 

        ContentCache<Content_T>& cache = m_content_cache_registry.at<ContentCache<Content_T>>();
        DEBUG_ONLY( CHECK( !map::has( cache, path ), "Map has unexpected key" ) );

        const ContentLoader<Content_T>& loader = m_content_loader_registry.at<ContentLoader<Content_T>>();
        const std::shared_ptr<Content_T> content = loader( this, full_path ); 
        cache[ path ] = content;
        return cache[ path ];
    }

    //----------------------------------------------------------------
    // Used if you want to obtain some content,
    // and it might not yet be in the cache
    template<typename Content_T>
    const std::shared_ptr<Content_T>& get_or_load( const io::Path& path )
    {
        auto& cache = m_content_cache_registry.at<ContentCache<Content_T>>();
        if ( !map::has( cache, path ) ) { preload<Content_T>( path ); }
        return cache[ path ];
    }

    //----------------------------------------------------------------
    // Used if you want to obtain some content,
    // and assert it was already preloaded
    template< typename Content_T >
    const std::shared_ptr<Content_T>& get( const io::Path& path )
    {
        auto& cache = m_content_cache_registry.at< ContentCache< Content_T> >();
        DEBUG_ONLY( CHECK( map::has( cache, path ), "Map does not have expected key" ) );
        return cache[ path ];
    }

    //----------------------------------------------------------------
    // Used to remove content from the cache
    template< typename Content_T >
    void unload( const io::Path& path )
    {
        auto& cache = m_content_cache_registry.at< ContentCache< Content_T > >();
        bool erased = map::erase_if_has( cache, path );
        LOG_IF( !erased, "Unnecessary unload of " + path.get_string() );
    }

public:
    PROPERTY_R ( std::vector<io::Path>, hosted_content_directories  )

    ContentLoaderRegistry   m_content_loader_registry;
    ContentCacheRegistry    m_content_cache_registry;
};


} // namespace content
} // namespace shake

#endif // CONTENT_MANAGER_HPP

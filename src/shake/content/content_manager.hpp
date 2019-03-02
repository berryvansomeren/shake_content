#ifndef CONTENT_MANAGER_HPP
#define CONTENT_MANAGER_HPP

#include <memory>

#include "shake/core/contracts/contracts.hpp"
#include "shake/core/log.hpp"
#include "shake/core/data_structures/map.hpp"
#include "shake/core/macro_debug_only.hpp"
#include "shake/core/types/macro_property.hpp"

#include "shake/io/file.hpp"
#include "shake/io/path.hpp"
#include "shake/content/load_cube_map.hpp"
#include "shake/content/load_font.hpp"
#include "shake/content/load_material.hpp"
#include "shake/content/load_shader.hpp"
#include "shake/content/load_sprite.hpp"
#include "shake/content/load_texture.hpp"
#include "shake/content/load_voxel_grid.hpp"

namespace shake {
namespace content {

class ContentManager
{
private:
    template<typename T>
    using SPtr = std::shared_ptr<T>;

    template<typename T>
    using Cache = std::map<io::Path, SPtr<T>>;

public:
    using Ptr = std::shared_ptr<ContentManager>;

public:

    NON_COPYABLE( ContentManager )

    static
    ContentManager& get_instance()
    {
        static ContentManager content_manager { };
        return content_manager;
    }

    void init()
    {
        load::init_font_loader();
    }

    // make sure to call the destroy function
    // before your contents' contexts (such as the graphics context) are destroyed,
    // otherwise you will get segmentation faults.
    void destroy()
    {
        // clear all cashes
        m_font_cache.clear();
        m_material_cache.clear();
        m_shader_cache.clear();
        m_sprite_cache.clear();
        m_texture_cache.clear();
        m_voxel_grid_cache.clear();

        load::destroy_font_loader();
    }

    //----------------------------------------------------------------
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

private:
    ContentManager() {}

public:

#define REGISTER_NAMED_ACCESSORS( type, name ) \
    inline const SPtr<type>& preload_##name     ( const io::Path& path ) { return preload<type>     ( path ); } \
    inline const SPtr<type>& get_or_load_##name ( const io::Path& path ) { return get_or_load<type> ( path ); } \
    inline const SPtr<type>& get_##name         ( const io::Path& path ) { return get<type>         ( path ); } \
    inline       void        unload_##name      ( const io::Path& path ) {        unload<type>      ( path ); }

    REGISTER_NAMED_ACCESSORS( graphics::CubeMap,        cube_map    )
    REGISTER_NAMED_ACCESSORS( graphics::Font,           font        )
    REGISTER_NAMED_ACCESSORS( graphics::Material,       material    )
    REGISTER_NAMED_ACCESSORS( graphics::Shader,         shader      )
    REGISTER_NAMED_ACCESSORS( graphics::Sprite,         sprite      )
    REGISTER_NAMED_ACCESSORS( graphics::Texture,        texture     )
    REGISTER_NAMED_ACCESSORS( graphics::VoxelGrid,      voxel_grid  )

#undef REGISTER_NAMED_ACCESSORS

public:
    //----------------------------------------------------------------
    // The content manager does some internal content caching for you.
    // You can however, also do some preloading and unloading of content.
    // The intended use is normally to preload() and unload() data at level transitions,
    // and use get() for everything else.

    //----------------------------------------------------------------
    // Used to load content to the content cache
    template<typename T> inline
    const SPtr<T>& preload( const io::Path& path )
    {
        auto& cache = get_cache<T>();
        DEBUG_ONLY( CHECK( !map::has( cache, path ), "Map has unexpected key" ) );
        cache[ path ] = load_t<T>( path );
        return cache[ path ];
    }

    //----------------------------------------------------------------
    // Used if you want to obtain some content,
    // and it might not yet be in the cache
    template<typename T>
    const SPtr<T>& get_or_load( const io::Path& path )
    {
        auto& cache = get_cache<T>();
        if ( !map::has( cache, path ) ) { cache[ path ] = load_t<T>( path ); }
        return cache[ path ];
    }

    //----------------------------------------------------------------
    // Used if you want to obtain some content,
    // and assert it was already preloaded
    template<typename T>
    const SPtr<T>& get( const io::Path& path )
    {
        auto& cache = get_cache<T>();
        DEBUG_ONLY( CHECK( map::has( cache, path ), "Map does not have expected key" ) );
        return cache[ path ];
    }

    //----------------------------------------------------------------
    // Used to remove content from the cache
    template<typename T>
    void unload( const io::Path& path )
    {
        auto& cache = get_cache<T>();
        bool erased = map::erase_if_has( cache, path );
        LOG_IF( !erased, "Unnecessary unload of " + path.get_string() );
    }

private:
    //----------------------------------------------------------------
    template<typename T> inline
    Cache<T>& get_cache();

    //----------------------------------------------------------------
    template<typename T> inline
    SPtr<T> load_t(const io::Path& path);

public:
    PROPERTY_W ( io::Path,                      shake_content_directory     )
    PROPERTY_R ( std::vector<io::Path>,         hosted_content_directories  )

    PROPERTY_RW( Cache<graphics::CubeMap>,      cube_map_cache      )
    PROPERTY_RW( Cache<graphics::Font>,         font_cache          )
    PROPERTY_RW( Cache<graphics::Material>,     material_cache      )
    PROPERTY_RW( Cache<graphics::Shader>,       shader_cache        )
    PROPERTY_RW( Cache<graphics::Sprite>,       sprite_cache        )
    PROPERTY_RW( Cache<graphics::Texture>,      texture_cache       )
    PROPERTY_RW( Cache<graphics::VoxelGrid>,    voxel_grid_cache    )
};

#define REGISTER_SPECIALIZED_TEMPLATE_ACCESSORS( type, name ) \
    template<> inline \
    ContentManager::Cache<type>& ContentManager::get_cache<type>() \
    { \
        return m_##name##_cache; \
    } \
    \
    template<> inline ContentManager::SPtr<type> \
    ContentManager::load_t<type>( const io::Path& path ) \
    { \
        const auto full_path = get_full_path( path ); \
        return load::load_##name( full_path ); \
    }

REGISTER_SPECIALIZED_TEMPLATE_ACCESSORS( graphics::CubeMap,     cube_map    )
REGISTER_SPECIALIZED_TEMPLATE_ACCESSORS( graphics::Font,        font        )
REGISTER_SPECIALIZED_TEMPLATE_ACCESSORS( graphics::Material,    material    )
REGISTER_SPECIALIZED_TEMPLATE_ACCESSORS( graphics::Shader,      shader      )
REGISTER_SPECIALIZED_TEMPLATE_ACCESSORS( graphics::Sprite,      sprite      )
REGISTER_SPECIALIZED_TEMPLATE_ACCESSORS( graphics::Texture,     texture     )
REGISTER_SPECIALIZED_TEMPLATE_ACCESSORS( graphics::VoxelGrid,   voxel_grid  )

#undef REGISTER_SPECIALIZED_TEMPLATE_ACCESSORS

} // namespace content
} // namespace shake

#endif // CONTENT_MANAGER_HPP

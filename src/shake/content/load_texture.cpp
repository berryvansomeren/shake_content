#include "load_texture.hpp"

#include <array>

#include <stb_image.h>

#include "shake/core/macros/macro_define_mapping.hpp"

#include "shake/io/file.hpp"
#include "shake/io/file_json.hpp"
#include "shake/io/file_reader.hpp"

#include "shake/content/content_manager.hpp"

#include "shake/graphics/material/texture_parameters.hpp"

namespace shake {
namespace content {
namespace load {

namespace
{
// Magica Voxel constants and structs

constexpr uint32_t file_id      = 542658390; // *reinterpret_cast<const uint32_t*>( "VOX " );
constexpr uint32_t file_version = 150; // MagicaVoxel 0.98

constexpr uint32_t main_id      = 1313423693; // *reinterpret_cast<const uint32_t*>( "MAIN" );
constexpr uint32_t palette_id   = 1094862674; // *reinterpret_cast<const uint32_t*>( "RGBA" );

std::array<uint32_t, 256> default_palette
{
    0x00000000, 0xffffffff, 0xffccffff, 0xff99ffff, 0xff66ffff, 0xff33ffff, 0xff00ffff, 0xffffccff, 0xffccccff, 0xff99ccff, 0xff66ccff, 0xff33ccff, 0xff00ccff, 0xffff99ff, 0xffcc99ff, 0xff9999ff,
    0xff6699ff, 0xff3399ff, 0xff0099ff, 0xffff66ff, 0xffcc66ff, 0xff9966ff, 0xff6666ff, 0xff3366ff, 0xff0066ff, 0xffff33ff, 0xffcc33ff, 0xff9933ff, 0xff6633ff, 0xff3333ff, 0xff0033ff, 0xffff00ff,
    0xffcc00ff, 0xff9900ff, 0xff6600ff, 0xff3300ff, 0xff0000ff, 0xffffffcc, 0xffccffcc, 0xff99ffcc, 0xff66ffcc, 0xff33ffcc, 0xff00ffcc, 0xffffcccc, 0xffcccccc, 0xff99cccc, 0xff66cccc, 0xff33cccc,
    0xff00cccc, 0xffff99cc, 0xffcc99cc, 0xff9999cc, 0xff6699cc, 0xff3399cc, 0xff0099cc, 0xffff66cc, 0xffcc66cc, 0xff9966cc, 0xff6666cc, 0xff3366cc, 0xff0066cc, 0xffff33cc, 0xffcc33cc, 0xff9933cc,
    0xff6633cc, 0xff3333cc, 0xff0033cc, 0xffff00cc, 0xffcc00cc, 0xff9900cc, 0xff6600cc, 0xff3300cc, 0xff0000cc, 0xffffff99, 0xffccff99, 0xff99ff99, 0xff66ff99, 0xff33ff99, 0xff00ff99, 0xffffcc99,
    0xffcccc99, 0xff99cc99, 0xff66cc99, 0xff33cc99, 0xff00cc99, 0xffff9999, 0xffcc9999, 0xff999999, 0xff669999, 0xff339999, 0xff009999, 0xffff6699, 0xffcc6699, 0xff996699, 0xff666699, 0xff336699,
    0xff006699, 0xffff3399, 0xffcc3399, 0xff993399, 0xff663399, 0xff333399, 0xff003399, 0xffff0099, 0xffcc0099, 0xff990099, 0xff660099, 0xff330099, 0xff000099, 0xffffff66, 0xffccff66, 0xff99ff66,
    0xff66ff66, 0xff33ff66, 0xff00ff66, 0xffffcc66, 0xffcccc66, 0xff99cc66, 0xff66cc66, 0xff33cc66, 0xff00cc66, 0xffff9966, 0xffcc9966, 0xff999966, 0xff669966, 0xff339966, 0xff009966, 0xffff6666,
    0xffcc6666, 0xff996666, 0xff666666, 0xff336666, 0xff006666, 0xffff3366, 0xffcc3366, 0xff993366, 0xff663366, 0xff333366, 0xff003366, 0xffff0066, 0xffcc0066, 0xff990066, 0xff660066, 0xff330066,
    0xff000066, 0xffffff33, 0xffccff33, 0xff99ff33, 0xff66ff33, 0xff33ff33, 0xff00ff33, 0xffffcc33, 0xffcccc33, 0xff99cc33, 0xff66cc33, 0xff33cc33, 0xff00cc33, 0xffff9933, 0xffcc9933, 0xff999933,
    0xff669933, 0xff339933, 0xff009933, 0xffff6633, 0xffcc6633, 0xff996633, 0xff666633, 0xff336633, 0xff006633, 0xffff3333, 0xffcc3333, 0xff993333, 0xff663333, 0xff333333, 0xff003333, 0xffff0033,
    0xffcc0033, 0xff990033, 0xff660033, 0xff330033, 0xff000033, 0xffffff00, 0xffccff00, 0xff99ff00, 0xff66ff00, 0xff33ff00, 0xff00ff00, 0xffffcc00, 0xffcccc00, 0xff99cc00, 0xff66cc00, 0xff33cc00,
    0xff00cc00, 0xffff9900, 0xffcc9900, 0xff999900, 0xff669900, 0xff339900, 0xff009900, 0xffff6600, 0xffcc6600, 0xff996600, 0xff666600, 0xff336600, 0xff006600, 0xffff3300, 0xffcc3300, 0xff993300,
    0xff663300, 0xff333300, 0xff003300, 0xffff0000, 0xffcc0000, 0xff990000, 0xff660000, 0xff330000, 0xff0000ee, 0xff0000dd, 0xff0000bb, 0xff0000aa, 0xff000088, 0xff000077, 0xff000055, 0xff000044,
    0xff000022, 0xff000011, 0xff00ee00, 0xff00dd00, 0xff00bb00, 0xff00aa00, 0xff008800, 0xff007700, 0xff005500, 0xff004400, 0xff002200, 0xff001100, 0xffee0000, 0xffdd0000, 0xffbb0000, 0xffaa0000,
    0xff880000, 0xff770000, 0xff550000, 0xff440000, 0xff220000, 0xff110000, 0xffeeeeee, 0xffdddddd, 0xffbbbbbb, 0xffaaaaaa, 0xff888888, 0xff777777, 0xff555555, 0xff444444, 0xff222222, 0xff111111
};

struct FileHeader
{
    uint32_t id;
    uint32_t version_number;
};

struct ChunkHeader
{
    uint32_t id;
    uint32_t n_bytes_content;
    uint32_t n_bytes_children;
};

struct PaletteChunk
{
    uint32_t colors[256];
};


//----------------------------------------------------------------
std::shared_ptr<graphics::Texture> load_voxel_texture( ContentManager* content_manager, const io::Path& path )
{
    // open a file reader
    auto file_reader = io::FileReader( path );
    CHECK_GE( file_reader.get_size(), 8, "Vox File is too short. It might be corrupted." );

    // parse the header
    const auto header_bytes = file_reader.read_bytes( sizeof( FileHeader) );
    const auto p_header = reinterpret_cast< const FileHeader* >( header_bytes.data() );
    CHECK_EQ( p_header->id, file_id, "Header of VOX file is not as expected." );

    // parse the main chunk specifier
    const auto main_chunk_bytes = file_reader.read_bytes( sizeof( ChunkHeader ) );
    const auto p_main_chunk = reinterpret_cast< const ChunkHeader* >( main_chunk_bytes.data() );
    CHECK_EQ( p_main_chunk->id, main_id, "Main chunk not found" );
    CHECK_GT( p_main_chunk->n_bytes_children, 0, "Main chunk does not contain children." );
    CHECK_EQ( p_main_chunk->n_bytes_content, 0, "Main chunk contains unexpected content." );

    auto palette = default_palette;

    // parse the chunks
    while ( file_reader.get_position() < file_reader.get_size() )
    {
        const auto chunk_header_bytes = file_reader.read_bytes( sizeof( ChunkHeader) );
        const auto p_chunk_header = reinterpret_cast< const ChunkHeader* >( chunk_header_bytes.data() );
        const auto chunk_id = p_chunk_header->id;
        //CHECK_EQ( p_chunk_header->n_bytes_children, 0, "Chunk contains unexpected children." );
        //CHECK_GT( p_chunk_header->n_bytes_content, 0, "Chunk doesn't contain any content." );

        // parse an optional palette chunk
        if ( chunk_id == palette_id )
        {
            CHECK_EQ( p_chunk_header->n_bytes_content, sizeof( PaletteChunk ), "Chunk is not of expected size." );
            const auto palette_chunk_bytes = file_reader.read_bytes( sizeof( PaletteChunk ) );
            const auto p_palette_chunk = reinterpret_cast< const PaletteChunk* >( palette_chunk_bytes.data() );
            auto palette_from_chunk = std::array<uint32_t, 256> { };
            std::copy( std::begin( p_palette_chunk->colors ), std::end( p_palette_chunk->colors  ), std::begin( palette_from_chunk ) );
            palette = palette_from_chunk;
        }
    }

    const auto palette_texture = std::make_shared<graphics::Texture>
    (
        reinterpret_cast<uint8_t*>( palette.data() ),
        256,
        1,
        graphics::gl::TextureFormat::RGBA,
        graphics::gl::Filter::Nearest
    );

    return palette_texture;
}

//----------------------------------------------------------------
std::shared_ptr<graphics::Texture> load_regular_texture( shake::content::ContentManager* content_manager, const io::Path& path )
{
    const auto content = io::file::json::read( path );

    // read from json
    const auto texture_path             = io::file::json::read_as<std::string>  ( content, { "texture"             } );
    const auto image_format_str         = io::file::json::read_as<std::string>  ( content, { "image_format"        } );
    const auto texture_format_str       = io::file::json::read_as<std::string>  ( content, { "texture_format"      } );
    const auto interpolation_mode_str   = io::file::json::read_as<std::string>  ( content, { "interpolation_mode"  } );
    const auto generate_mipmaps         = io::file::json::read_as<bool>         ( content, { "generate_mip_maps"   } );

    // check if texture path exists
    const auto full_texture_path = content_manager->get_full_path( io::Path( texture_path ) );
    CHECK( io::file::exists( full_texture_path ), "Texture file does not exist." );

    // get data from file in memory
    int width   { };
    int height  { };
    int bytes_per_pixel         { };
    int bytes_per_pixel_forced  { 3 };
    uint8_t* stb_image_ptr { static_cast<uint8_t*>( stbi_load
    (
        full_texture_path.c_str(),
        &width,
        &height,
        &bytes_per_pixel,
        bytes_per_pixel_forced
    ))};

    // load texture on gpu
    const auto texture = std::make_shared<graphics::Texture>
    (
        stb_image_ptr, 
        width,
        height,
        graphics::to_texture_format( texture_format_str ),
        graphics::to_filter( interpolation_mode_str )
    );

    // clear memory from cpu
    stbi_image_free( stb_image_ptr );

    return texture;
}

} // namespace anonymous

//----------------------------------------------------------------
std::shared_ptr<graphics::Texture> load_texture( ContentManager* content_manager, const io::Path& path )
{
    const auto file_extension = path.get_file_extension();

    if ( file_extension == ".vox" )
    {
        return load_voxel_texture( content_manager, path );
    }
    else if ( file_extension == ".json" )
    {
        return load_regular_texture( content_manager, path );
    }

    CHECK_FAIL( "Unrecognised texture file extension: " + file_extension );
}

} // namespace load
} // namespace content
} // namespace shake

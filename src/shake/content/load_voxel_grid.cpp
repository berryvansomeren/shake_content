//#include "load_voxel_grid.hpp"
//
//#include <array>
//#include <string>
//#include <optional>
//
//#include "shake/content/content_manager.hpp"
//
//#include "shake/core/contracts/contracts.hpp"
//
//#include "shake/io/file_reader.hpp"
//#include "shake/io/file_json.hpp"
//#include "shake/io/serialize.hpp"
//
//#include "shake/graphics/material/texture.hpp"
//
//namespace shake {
//namespace content {
//namespace load {
//
//namespace { // anonymous
//
//std::array<uint32_t, 256> default_palette
//{
//    0x00000000, 0xffffffff, 0xffccffff, 0xff99ffff, 0xff66ffff, 0xff33ffff, 0xff00ffff, 0xffffccff, 0xffccccff, 0xff99ccff, 0xff66ccff, 0xff33ccff, 0xff00ccff, 0xffff99ff, 0xffcc99ff, 0xff9999ff,
//    0xff6699ff, 0xff3399ff, 0xff0099ff, 0xffff66ff, 0xffcc66ff, 0xff9966ff, 0xff6666ff, 0xff3366ff, 0xff0066ff, 0xffff33ff, 0xffcc33ff, 0xff9933ff, 0xff6633ff, 0xff3333ff, 0xff0033ff, 0xffff00ff,
//    0xffcc00ff, 0xff9900ff, 0xff6600ff, 0xff3300ff, 0xff0000ff, 0xffffffcc, 0xffccffcc, 0xff99ffcc, 0xff66ffcc, 0xff33ffcc, 0xff00ffcc, 0xffffcccc, 0xffcccccc, 0xff99cccc, 0xff66cccc, 0xff33cccc,
//    0xff00cccc, 0xffff99cc, 0xffcc99cc, 0xff9999cc, 0xff6699cc, 0xff3399cc, 0xff0099cc, 0xffff66cc, 0xffcc66cc, 0xff9966cc, 0xff6666cc, 0xff3366cc, 0xff0066cc, 0xffff33cc, 0xffcc33cc, 0xff9933cc,
//    0xff6633cc, 0xff3333cc, 0xff0033cc, 0xffff00cc, 0xffcc00cc, 0xff9900cc, 0xff6600cc, 0xff3300cc, 0xff0000cc, 0xffffff99, 0xffccff99, 0xff99ff99, 0xff66ff99, 0xff33ff99, 0xff00ff99, 0xffffcc99,
//    0xffcccc99, 0xff99cc99, 0xff66cc99, 0xff33cc99, 0xff00cc99, 0xffff9999, 0xffcc9999, 0xff999999, 0xff669999, 0xff339999, 0xff009999, 0xffff6699, 0xffcc6699, 0xff996699, 0xff666699, 0xff336699,
//    0xff006699, 0xffff3399, 0xffcc3399, 0xff993399, 0xff663399, 0xff333399, 0xff003399, 0xffff0099, 0xffcc0099, 0xff990099, 0xff660099, 0xff330099, 0xff000099, 0xffffff66, 0xffccff66, 0xff99ff66,
//    0xff66ff66, 0xff33ff66, 0xff00ff66, 0xffffcc66, 0xffcccc66, 0xff99cc66, 0xff66cc66, 0xff33cc66, 0xff00cc66, 0xffff9966, 0xffcc9966, 0xff999966, 0xff669966, 0xff339966, 0xff009966, 0xffff6666,
//    0xffcc6666, 0xff996666, 0xff666666, 0xff336666, 0xff006666, 0xffff3366, 0xffcc3366, 0xff993366, 0xff663366, 0xff333366, 0xff003366, 0xffff0066, 0xffcc0066, 0xff990066, 0xff660066, 0xff330066,
//    0xff000066, 0xffffff33, 0xffccff33, 0xff99ff33, 0xff66ff33, 0xff33ff33, 0xff00ff33, 0xffffcc33, 0xffcccc33, 0xff99cc33, 0xff66cc33, 0xff33cc33, 0xff00cc33, 0xffff9933, 0xffcc9933, 0xff999933,
//    0xff669933, 0xff339933, 0xff009933, 0xffff6633, 0xffcc6633, 0xff996633, 0xff666633, 0xff336633, 0xff006633, 0xffff3333, 0xffcc3333, 0xff993333, 0xff663333, 0xff333333, 0xff003333, 0xffff0033,
//    0xffcc0033, 0xff990033, 0xff660033, 0xff330033, 0xff000033, 0xffffff00, 0xffccff00, 0xff99ff00, 0xff66ff00, 0xff33ff00, 0xff00ff00, 0xffffcc00, 0xffcccc00, 0xff99cc00, 0xff66cc00, 0xff33cc00,
//    0xff00cc00, 0xffff9900, 0xffcc9900, 0xff999900, 0xff669900, 0xff339900, 0xff009900, 0xffff6600, 0xffcc6600, 0xff996600, 0xff666600, 0xff336600, 0xff006600, 0xffff3300, 0xffcc3300, 0xff993300,
//    0xff663300, 0xff333300, 0xff003300, 0xffff0000, 0xffcc0000, 0xff990000, 0xff660000, 0xff330000, 0xff0000ee, 0xff0000dd, 0xff0000bb, 0xff0000aa, 0xff000088, 0xff000077, 0xff000055, 0xff000044,
//    0xff000022, 0xff000011, 0xff00ee00, 0xff00dd00, 0xff00bb00, 0xff00aa00, 0xff008800, 0xff007700, 0xff005500, 0xff004400, 0xff002200, 0xff001100, 0xffee0000, 0xffdd0000, 0xffbb0000, 0xffaa0000,
//    0xff880000, 0xff770000, 0xff550000, 0xff440000, 0xff220000, 0xff110000, 0xffeeeeee, 0xffdddddd, 0xffbbbbbb, 0xffaaaaaa, 0xff888888, 0xff777777, 0xff555555, 0xff444444, 0xff222222, 0xff111111
//};
//
//struct FileHeader
//{
//    std::string id;
//    uint32_t version;
//};
//
//struct ChunkHeader
//{
//    std::string id;
//    uint32_t n_bytes_content;
//    uint32_t n_bytes_children;
//};
//
//struct PackChunk
//{
//    uint32_t n_models;
//};
//
//struct SizeChunk
//{
//    uint32_t x_size;
//    uint32_t y_size;
//    uint32_t z_size;
//};
//
//struct VoxelChunkHeader
//{
//    uint32_t n_voxels;
//};
//
//struct VoxelData
//{
//    uint8_t x;
//    uint8_t y;
//    uint8_t z;
//    uint8_t color_index;
//};
//
//struct VoxelChunk
//{   
//    std::vector<VoxelData> voxels;
//};
//
//struct PaletteColor
//{
//    uint8_t r;
//    uint8_t g;
//    uint8_t b;
//    uint8_t a;
//};
//
//struct PaletteChunk
//{
//    std::array<PaletteColor, 256> colors;
//};
//
//struct MaterialChunk
//{
//    uint32_t            id;
//    uint32_t            type;
//    float               material_weight;
//    uint32_t            property_flags;
//    std::vector<float>  property_values;
//};
//
//struct VoxRepresentation
//{
//    std::optional<PackChunk>        pack_chunk         = {};
//    std::vector<SizeChunk>          size_chunks        = {};
//    std::vector<VoxelChunk>         voxel_chunks       = {};
//    std::optional<PaletteChunk>     palette_chunk      = {};
//    std::vector<MaterialChunk>      material_chunks    = {};
//};
//
////----------------------------------------------------------------
//inline void validate_file_header( io::FileReader& file_reader )
//{
//    const auto file_header = FileHeader
//    {
//        file_reader.read<std::string>( sizeof( uint32_t ) ),
//        file_reader.read<uint32_t>()
//    };
//    CHECK_EQ( file_header.id,       "VOX ", "Header of .vox file has unexpected id."        );
//    CHECK_EQ( file_header.version,  150,    "Header of .vox file has unexpected version."   );
//}
//
//inline void read_chunk( io::FileReader& file_reader, VoxRepresentation& vox_representation );
//
////----------------------------------------------------------------
//inline void read_main_chunk( const ChunkHeader& chunk_header, io::FileReader& file_reader, VoxRepresentation& vox_representation )
//{
//    CHECK_EQ( chunk_header.id,                  "MAIN", "Unexpected chunk header id."       );
//    CHECK_EQ( chunk_header.n_bytes_content,     0,      "Unexpected chunk size."            );
//    CHECK_GT( chunk_header.n_bytes_children,    0,      "Unexpected chunk children size."   );
//
//    const auto expected_end_position = file_reader.get_position() + chunk_header.n_bytes_children;
//
//    while ( file_reader.get_position() < expected_end_position )
//    {
//        CHECK_LT( file_reader.get_position(), expected_end_position, "Unexpected file reader position." );
//        read_chunk( file_reader, vox_representation );
//    }
//
//    CHECK_EQ( file_reader.get_position(), expected_end_position, "Unexpected file reader position." );
//};
//
////----------------------------------------------------------------
//inline void read_pack_chunk( const ChunkHeader& chunk_header, io::FileReader& file_reader, VoxRepresentation& vox_representation )
//{
//    CHECK_EQ( chunk_header.id,                  "PACK",                 "Unexpected chunk header id."       );
//    CHECK_EQ( chunk_header.n_bytes_content,     sizeof( PackChunk ),    "Chunk is not of expected size."    );
//    CHECK_EQ( chunk_header.n_bytes_children,    0,                      "Unexpected chunk children size."   );
//
//    const auto expected_end_position = file_reader.get_position() + chunk_header.n_bytes_content;
//
//    const auto pack_chunk = file_reader.read<PackChunk>();
//    vox_representation.pack_chunk = pack_chunk;
//    vox_representation.size_chunks.reserve( pack_chunk.n_models );
//    vox_representation.voxel_chunks.reserve( pack_chunk.n_models );
//
//    CHECK_EQ( file_reader.get_position(), expected_end_position, "Unexpected file reader position." );
//}
//
////----------------------------------------------------------------
//inline void read_size_chunk( const ChunkHeader& chunk_header, io::FileReader& file_reader, VoxRepresentation& vox_representation )
//{
//    CHECK_EQ( chunk_header.id,                  "SIZE",                 "Unexpected chunk header id."       );
//    CHECK_EQ( chunk_header.n_bytes_content,     sizeof( SizeChunk ),    "Chunk is not of expected size."    );
//    CHECK_EQ( chunk_header.n_bytes_children,    0,                      "Unexpected chunk children size."   );
//
//    const auto expected_end_position = file_reader.get_position() + chunk_header.n_bytes_content;
//
//    vox_representation.size_chunks.emplace_back( file_reader.read<SizeChunk>() );
//
//    CHECK_EQ( file_reader.get_position(), expected_end_position, "Unexpected file reader position." );
//}
//
////----------------------------------------------------------------
//inline void read_voxel_chunk( const ChunkHeader& chunk_header, io::FileReader& file_reader, VoxRepresentation& vox_representation )
//{
//    CHECK_EQ( chunk_header.id,                  "XYZI",                 "Unexpected chunk header id."       );
//    CHECK_EQ( chunk_header.n_bytes_children,    0,                      "Unexpected chunk children size."   );
//
//    const auto expected_end_position = file_reader.get_position() + chunk_header.n_bytes_content;
//
//    const auto voxel_chunk_header = file_reader.read<VoxelChunkHeader>();
//    CHECK_EQ( chunk_header.n_bytes_content, voxel_chunk_header.n_voxels * sizeof( VoxelData ) + sizeof( VoxelChunkHeader ), "Chunk is not of expected size." );
//
//    // parse all voxels within the chunk
//    auto voxel_chunk = VoxelChunk{};
//    voxel_chunk.voxels.reserve( voxel_chunk_header.n_voxels );
//    for ( size_t voxel_index = 0; voxel_index < voxel_chunk_header.n_voxels; ++voxel_index )
//    {
//        voxel_chunk.voxels.emplace_back( file_reader.read<VoxelData>() );
//    }
//    vox_representation.voxel_chunks.emplace_back( voxel_chunk );
//
//    CHECK_EQ( file_reader.get_position(), expected_end_position, "Unexpected file reader position." );
//}
//
////----------------------------------------------------------------
//inline void read_palette_chunk( const ChunkHeader& chunk_header, io::FileReader& file_reader, VoxRepresentation& vox_representation )
//{
//    CHECK_EQ( chunk_header.id,                  "RGBA",                 "Unexpected chunk header id."       );
//    CHECK_EQ( chunk_header.n_bytes_content,     sizeof( PaletteChunk ), "Chunk is not of expected size."    );
//    CHECK_EQ( chunk_header.n_bytes_children,    0,                      "Unexpected chunk children size."   );
//
//    const auto expected_end_position = file_reader.get_position() + chunk_header.n_bytes_content;
//    
//    auto palette_chunk = PaletteChunk{};
//    palette_chunk.colors.at( 0 ) = PaletteColor{ 0, 0, 0, 0 };
//    // Colors [0-254] are mapped to palette index [1-255], e.g
//    // Also see official .vox file format documentation
//    for ( size_t color_index = 0; color_index < 255; ++color_index )
//    {
//        palette_chunk.colors.at( color_index + 1 ) = file_reader.read<PaletteColor>();
//    }
//    vox_representation.palette_chunk = palette_chunk;
//
//    // because of the mapping mentioned above, we need to compensate for one missing color
//    file_reader.advance( sizeof( uint32_t ) ); 
//
//    CHECK_EQ( file_reader.get_position(), expected_end_position, "Unexpected file reader position." );
//}
//
////----------------------------------------------------------------
//inline void read_chunk( io::FileReader& file_reader, VoxRepresentation& vox_representation )
//{
//    const auto chunk_header = ChunkHeader
//    {
//        file_reader.read<std::string>( sizeof( uint32_t ) ),
//        file_reader.read<uint32_t>(),
//        file_reader.read<uint32_t>()
//    };
//
//         if ( chunk_header.id == "MAIN" ) { read_main_chunk     ( chunk_header, file_reader, vox_representation ); return; }
//    else if ( chunk_header.id == "PACK" ) { read_pack_chunk     ( chunk_header, file_reader, vox_representation ); return; }
//    else if ( chunk_header.id == "SIZE" ) { read_size_chunk     ( chunk_header, file_reader, vox_representation ); return; }
//    else if ( chunk_header.id == "XYZI" ) { read_voxel_chunk    ( chunk_header, file_reader, vox_representation ); return; }
//    else if ( chunk_header.id == "RGBA" ) { read_palette_chunk  ( chunk_header, file_reader, vox_representation ); return; }
//    else
//    {
//        // We don't parse the chunk
//        // But we still need to advance the file reader
//        file_reader.advance( chunk_header.n_bytes_content + chunk_header.n_bytes_children );
//    }
//}
//
//} // namespace anonymous
//
//
////----------------------------------------------------------------
//std::unique_ptr<graphics::VoxelGrid> load_voxel_grid( shake::content::ContentManager* content_manager, const io::Path& path )
//{
//    // open a file reader
//    auto file_reader = io::FileReader( path );
//    CHECK_GE( file_reader.get_size(), 8, "Vox File is too short. It might be corrupted." );
//
//    // parse the .vox file
//    validate_file_header( file_reader );
//    auto vox_representation = VoxRepresentation{};
//    read_chunk( file_reader, vox_representation );
//    
//    // transform the vox data into our own voxel model
//    auto instances = std::vector< float > { };
//
//    for ( const auto& voxel_chunk : vox_representation.voxel_chunks )
//    {
//        for ( const auto& voxel : voxel_chunk.voxels )
//        {
//            instances.emplace_back( static_cast<float>( voxel.x ) );
//            instances.emplace_back( static_cast<float>( voxel.y ) );
//            instances.emplace_back( static_cast<float>( voxel.z ) );
//            instances.emplace_back( static_cast<float>( voxel.color_index ) );
//            instances.emplace_back( static_cast<float>( 0 ) );
//        }
//    }
//
//    const auto voxel_content = io::file::json::read( content_manager->get_full_path( io::Path( "meshes/voxel.json" ) ) );
//    const auto vertices      = io::file::json::read_as<std::vector<float>>( voxel_content, {"vertices"} );
//    const auto indices       = math::integer_sequence<uint32_t>(vertices.size() / 6); // 6 floats per vertex
//
//    const auto palette_data = vox_representation.palette_chunk.has_value()
//        ? reinterpret_cast<uint8_t*>( vox_representation.palette_chunk.value().colors.data() )
//        : reinterpret_cast<uint8_t*>( default_palette.data() );
//
//    //const auto palette_data = reinterpret_cast<uint8_t*>( default_palette.data() );
//
//    auto palette_texture = std::make_shared<graphics::Texture>
//    (
//        palette_data,
//        256,
//        1,
//        graphics::gl::TextureFormat::RGBA,
//        graphics::gl::Filter::Nearest
//    );
//    
//    return std::make_unique<graphics::VoxelGrid>( vertices, indices, instances, palette_texture );
//}
//
//
//} // namespace load
//} // namespace content
//} // namespace shake
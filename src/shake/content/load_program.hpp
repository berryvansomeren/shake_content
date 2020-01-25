#ifndef LOAD_PROGRAM_HPP
#define LOAD_PROGRAM_HPP

#include <memory>

#include "shake/io/path.hpp"
#include "shake/graphics/material/program.hpp"

namespace shake {
namespace content {

class ContentManager;

namespace load {

std::shared_ptr< graphics::Program > load_program( shake::content::ContentManager* content_manager, const io::Path& path );

} // namespace load
} // namespace content
} // namespace shake

#endif // LOAD_PROGRAM_HPP

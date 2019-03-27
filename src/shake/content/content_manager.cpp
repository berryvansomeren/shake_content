#include "content_manager.hpp"

namespace shake {
namespace content {

void init_gl( const GLLoadProc& gl_load_proc )
{
    const auto load_success = gladLoadGLLoader( ( GLADloadproc ) gl_load_proc );
    if ( !load_success ) 
    {
        CHECK_FAIL( "Could not initialize OpenGl Context" );
    }
}

} // namespace content
} // namespace shake

// NOTE: DO NOT CHANGE THIS FILE. IT IS AUTOMATICALLY GENERATED.

#ifndef MBGL_SHADER_SHADERS
#define MBGL_SHADER_SHADERS

namespace mbgl {

struct shader_source {
    const char *vertex;
    const char *fragment;
};

enum {
    RASTER_SHADER,
    OUTLINE_SHADER,
    PATTERN_SHADER,
    GAUSSIAN_SHADER,
    SDF_SHADER,
    PLAIN_SHADER,
    LINE_SHADER,
    LINEPATTERN_SHADER,
    LINESDF_SHADER,
    DOT_SHADER,
    ICON_SHADER,
    SHADER_COUNT
};

extern const shader_source shaders[SHADER_COUNT];

}

#endif

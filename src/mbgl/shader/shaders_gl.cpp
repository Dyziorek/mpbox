// NOTE: DO NOT CHANGE THIS FILE. IT IS AUTOMATICALLY GENERATED.
#include <mbgl/platform/gl.hpp>
#ifndef GL_ES_VERSION_2_0
#include <mbgl/shader/shaders.hpp>

namespace mbgl {

const shader_source shaders[SHADER_COUNT] = {
    {
        R"raster_vert(#version 120
uniform mat4 u_matrix;
uniform float u_buffer;

attribute vec2 a_pos;

varying vec2 v_pos;


void main() {
    gl_Position = u_matrix * vec4(a_pos, 0, 1);
    float dimension = (4096.0 + 2.0 * u_buffer);
    v_pos = (a_pos / dimension) + (u_buffer / dimension);
}
)raster_vert",
        R"raster_frag(#version 120
uniform sampler2D u_image;
uniform float u_opacity;

varying vec2 v_pos;

uniform float u_brightness_low;
uniform float u_brightness_high;

uniform float u_saturation_factor;
uniform float u_contrast_factor;
uniform vec3 u_spin_weights;

void main() {

    vec4 color = texture2D(u_image, v_pos) * u_opacity;
    vec3 rgb = color.rgb;

    // spin
    rgb = vec3(
        dot(rgb, u_spin_weights.xyz),
        dot(rgb, u_spin_weights.zxy),
        dot(rgb, u_spin_weights.yzx));

    // saturation
    float average = (color.r + color.g + color.b) / 3.0;
    rgb += (average - rgb) * u_saturation_factor;

    // contrast
    rgb = (rgb - 0.5) * u_contrast_factor + 0.5;

    // brightness
    vec3 u_high_vec = vec3(u_brightness_low, u_brightness_low, u_brightness_low);
    vec3 u_low_vec = vec3(u_brightness_high, u_brightness_high, u_brightness_high);

    gl_FragColor = vec4(mix(u_high_vec, u_low_vec, rgb), color.a);
}
)raster_frag",
    },
    {
        R"outline_vert(#version 120
attribute vec2 a_pos;
uniform mat4 u_matrix;
uniform vec2 u_world;

varying vec2 v_pos;

void main() {
    gl_Position = u_matrix * vec4(a_pos, 0, 1);
    v_pos = (gl_Position.xy + 1.0) / 2.0 * u_world;
}
)outline_vert",
        R"outline_frag(#version 120
uniform vec4 u_color;

varying vec2 v_pos;

void main() {
    float dist = length(v_pos - gl_FragCoord.xy);
    float alpha = smoothstep(1.0, 0.0, dist);
    gl_FragColor = u_color * alpha;
}
)outline_frag",
    },
    {
        R"pattern_vert(#version 120
uniform mat4 u_matrix;
uniform mat3 u_patternmatrix_a;
uniform mat3 u_patternmatrix_b;

attribute vec2 a_pos;

varying vec2 v_pos_a;
varying vec2 v_pos_b;

void main() {
    gl_Position = u_matrix * vec4(a_pos, 0, 1);
    v_pos_a = (u_patternmatrix_a * vec3(a_pos, 1)).xy;
    v_pos_b = (u_patternmatrix_b * vec3(a_pos, 1)).xy;
}
)pattern_vert",
        R"pattern_frag(#version 120
uniform float u_opacity;
uniform vec2 u_pattern_tl_a;
uniform vec2 u_pattern_br_a;
uniform vec2 u_pattern_tl_b;
uniform vec2 u_pattern_br_b;
uniform float u_mix;

uniform sampler2D u_image;

varying vec2 v_pos_a;
varying vec2 v_pos_b;

void main() {

    vec2 imagecoord = mod(v_pos_a, 1.0);
    vec2 pos = mix(u_pattern_tl_a, u_pattern_br_a, imagecoord);
    vec4 color1 = texture2D(u_image, pos);

    vec2 imagecoord_b = mod(v_pos_b, 1.0);
    vec2 pos2 = mix(u_pattern_tl_b, u_pattern_br_b, imagecoord_b);
    vec4 color2 = texture2D(u_image, pos2);

    gl_FragColor = mix(color1, color2, u_mix) * u_opacity;
}
)pattern_frag",
    },
    {
        R"gaussian_vert(#version 120
attribute vec2 a_pos;

uniform mat4 u_matrix;
uniform vec2 u_offset;

varying vec2 v_coords[3];

void main() {
    gl_Position = u_matrix * vec4(a_pos, 0, 1);

    vec2 tex = gl_Position.xy / 2.0 + 0.5;
    v_coords[0] = tex;
    v_coords[1] = tex + u_offset * 1.1824255238063563;
    v_coords[2] = tex - u_offset * 1.1824255238063563;
}
)gaussian_vert",
        R"gaussian_frag(#version 120
uniform sampler2D u_image;

varying vec2 v_coords[3];

void main() {
    vec4 sum = vec4(0.0);
    sum += texture2D(u_image, v_coords[0]) * 0.40261994689424746;
    sum += texture2D(u_image, v_coords[1]) * 0.2986900265528763;
    sum += texture2D(u_image, v_coords[2]) * 0.2986900265528763;
    gl_FragColor = sum;
}
)gaussian_frag",
    },
    {
        R"sdf_vert(#version 120
attribute vec2 a_pos;
attribute vec2 a_offset;
attribute vec4 a_data1;
attribute vec4 a_data2;


// matrix is for the vertex position, exmatrix is for rotating and projecting
// the extrusion vector.
uniform mat4 u_matrix;
uniform mat4 u_exmatrix;
uniform float u_angle;
uniform float u_zoom;
uniform float u_flip;
uniform float u_fadedist;
uniform float u_minfadezoom;
uniform float u_maxfadezoom;
uniform float u_fadezoom;

uniform vec2 u_texsize;

varying vec2 v_tex;
varying float v_alpha;

void main() {
    vec2 a_tex = a_data1.xy;
    float a_labelminzoom = a_data1[2];
    float a_angle = a_data1[3];
    vec2 a_zoom = a_data2.st;
    float a_minzoom = a_zoom[0];
    float a_maxzoom = a_zoom[1];
    vec2 a_range = a_data2.pq;
    float a_rangeend = a_range[0];
    float a_rangestart = a_range[1];

    float rev = 0.0;

    // u_angle is angle of the map, -128..128 representing 0..2PI
    // a_angle is angle of the label, 0..256 representing 0..2PI, where 0 is horizontal text
    float rotated = mod(a_angle + u_angle, 256.0);
    // if the label rotates with the map, and if the rotated label is upside down, hide it
    if (u_flip > 0.0 && rotated >= 64.0 && rotated < 192.0) rev = 1.0;

    // If the label should be invisible, we move the vertex outside
    // of the view plane so that the triangle gets clipped. This makes it easier
    // for us to create degenerate triangle strips.
    // u_zoom is the current zoom level adjusted for the change in font size
    float z = 2.0 - step(a_minzoom, u_zoom) - (1.0 - step(a_maxzoom, u_zoom)) + rev;

    // fade out labels
    float alpha = clamp((u_fadezoom - a_labelminzoom) / u_fadedist, 0.0, 1.0);

    if (u_fadedist >= 0.0) {
        v_alpha = alpha;
    } else {
        v_alpha = 1.0 - alpha;
    }
    if (u_maxfadezoom < a_labelminzoom) {
        v_alpha = 0.0;
    }
    if (u_minfadezoom >= a_labelminzoom) {
        v_alpha = 1.0;
    }

    // if label has been faded out, clip it
    z += step(v_alpha, 0.0);

    // all the angles are 0..256 representing 0..2PI
    // hide if (angle >= a_rangeend && angle < rangestart)
    z += step(a_rangeend, u_angle) * (1.0 - step(a_rangestart, u_angle));

    gl_Position = u_matrix * vec4(a_pos, 0, 1) + u_exmatrix * vec4(a_offset / 64.0, z, 0);
    v_tex = a_tex / u_texsize;
}
)sdf_vert",
        R"sdf_frag(#version 120
uniform sampler2D u_texture;
uniform vec4 u_color;
uniform float u_buffer;
uniform float u_gamma;

varying vec2 v_tex;
varying float v_alpha;

void main() {
    float dist = texture2D(u_texture, v_tex).a;
    float alpha = smoothstep(u_buffer - u_gamma, u_buffer + u_gamma, dist) * v_alpha;
    gl_FragColor = u_color * alpha;
}
)sdf_frag",
    },
    {
        R"plain_vert(#version 120
attribute vec2 a_pos;

uniform mat4 u_matrix;

void main() {
    gl_Position = u_matrix * vec4(a_pos, 0, 1);
}
)plain_vert",
        R"plain_frag(#version 120
uniform vec4 u_color;

void main() {
    gl_FragColor = u_color;
}
)plain_frag",
    },
    {
        R"line_vert(#version 120
// floor(127 / 2) == 63.0
// the maximum allowed miter limit is 2.0 at the moment. the extrude normal is
// stored in a byte (-128..127). we scale regular normals up to length 63, but
// there are also "special" normals that have a bigger length (of up to 126 in
// this case).
// #define scale 63.0
#define scale 0.015873016

attribute vec2 a_pos;
attribute vec4 a_data;

// matrix is for the vertex position, exmatrix is for rotating and projecting
// the extrusion vector.
uniform mat4 u_matrix;
uniform mat4 u_exmatrix;

// shared
uniform float u_ratio;
uniform vec2 u_linewidth;
uniform vec4 u_color;

varying vec2 v_normal;

void main() {
    vec2 a_extrude = a_data.xy;

    // We store the texture normals in the most insignificant bit
    // transform y so that 0 => -1 and 1 => 1
    // In the texture normal, x is 0 if the normal points straight up/down and 1 if it's a round cap
    // y is 1 if the normal points up, and -1 if it points down
    vec2 normal = mod(a_pos, 2.0);
    normal.y = sign(normal.y - 0.5);
    v_normal = normal;

    // Scale the extrusion vector down to a normal and then up by the line width
    // of this vertex.
    vec4 dist = vec4(u_linewidth.s * a_extrude * scale, 0.0, 0.0);

    // Remove the texture normal bit of the position before scaling it with the
    // model/view matrix. Add the extrusion vector *after* the model/view matrix
    // because we're extruding the line in pixel space, regardless of the current
    // tile's zoom level.
    gl_Position = u_matrix * vec4(floor(a_pos * 0.5), 0.0, 1.0) + u_exmatrix * dist;
}
)line_vert",
        R"line_frag(#version 120
uniform vec2 u_linewidth;
uniform vec4 u_color;
uniform float u_blur;

varying vec2 v_normal;

void main() {
    // Calculate the distance of the pixel from the line in pixels.
    float dist = length(v_normal) * u_linewidth.s;

    // Calculate the antialiasing fade factor. This is either when fading in
    // the line in case of an offset line (v_linewidth.t) or when fading out
    // (v_linewidth.s)
    float alpha = clamp(min(dist - (u_linewidth.t - u_blur), u_linewidth.s - dist) / u_blur, 0.0, 1.0);

    gl_FragColor = u_color * alpha;
}
)line_frag",
    },
    {
        R"linepattern_vert(#version 120
// floor(127 / 2) == 63.0
// the maximum allowed miter limit is 2.0 at the moment. the extrude normal is
// stored in a byte (-128..127). we scale regular normals up to length 63, but
// there are also "special" normals that have a bigger length (of up to 126 in
// this case).
// #define scale 63.0
#define scale 0.015873016

attribute vec2 a_pos;
attribute vec4 a_data;

// matrix is for the vertex position, exmatrix is for rotating and projecting
// the extrusion vector.
uniform mat4 u_matrix;
uniform mat4 u_exmatrix;

// shared
uniform float u_ratio;
uniform vec2 u_linewidth;
uniform vec4 u_color;
uniform float u_point;

varying vec2 v_normal;
varying float v_linesofar;

void main() {
    vec2 a_extrude = a_data.xy;
    float a_linesofar = a_data.z * 128.0 + a_data.w;

    // We store the texture normals in the most insignificant bit
    // transform y so that 0 => -1 and 1 => 1
    // In the texture normal, x is 0 if the normal points straight up/down and 1 if it's a round cap
    // y is 1 if the normal points up, and -1 if it points down
    vec2 normal = mod(a_pos, 2.0);
    normal.y = sign(normal.y - 0.5);
    v_normal = normal;

    // Scale the extrusion vector down to a normal and then up by the line width
    // of this vertex.
    vec2 extrude = a_extrude * scale;
    vec2 dist = u_linewidth.s * extrude * (1.0 - u_point);

    // If the x coordinate is the maximum integer, we move the z coordinates out
    // of the view plane so that the triangle gets clipped. This makes it easier
    // for us to create degenerate triangle strips.
    float z = step(32767.0, a_pos.x);

    // When drawing points, skip every other vertex
    z += u_point * step(1.0, v_normal.y);

    // Remove the texture normal bit of the position before scaling it with the
    // model/view matrix. Add the extrusion vector *after* the model/view matrix
    // because we're extruding the line in pixel space, regardless of the current
    // tile's zoom level.
    gl_Position = u_matrix * vec4(floor(a_pos / 2.0), 0.0, 1.0) + u_exmatrix * vec4(dist, z, 0.0);
    v_linesofar = a_linesofar;// * u_ratio;


    gl_PointSize = 2.0 * u_linewidth.s - 1.0;
}
)linepattern_vert",
        R"linepattern_frag(#version 120
uniform vec2 u_linewidth;
uniform float u_point;
uniform float u_blur;

uniform vec2 u_pattern_size_a;
uniform vec2 u_pattern_tl_a;
uniform vec2 u_pattern_br_a;
uniform vec2 u_pattern_size_b;
uniform vec2 u_pattern_tl_b;
uniform vec2 u_pattern_br_b;
uniform float u_fade;
uniform float u_opacity;

uniform sampler2D u_image;

varying vec2 v_normal;
varying float v_linesofar;

void main() {
    // Calculate the distance of the pixel from the line in pixels.
    float dist = length(v_normal) * (1.0 - u_point) + u_point * length(gl_PointCoord * 2.0 - 1.0);

    dist *= u_linewidth.s;

    // Calculate the antialiasing fade factor. This is either when fading in
    // the line in case of an offset line (v_linewidth.t) or when fading out
    // (v_linewidth.s)
    float alpha = clamp(min(dist - (u_linewidth.t - u_blur), u_linewidth.s - dist) / u_blur, 0.0, 1.0);

    float x_a = mod(v_linesofar / u_pattern_size_a.x, 1.0);
    float y_a = 0.5 + (v_normal.y * u_linewidth.s / u_pattern_size_a.y);
    vec2 pos_a = mix(u_pattern_tl_a, u_pattern_br_a, vec2(x_a, y_a));
    float x_b = mod(v_linesofar / u_pattern_size_b.x, 1.0);
    float y_b = 0.5 + (v_normal.y * u_linewidth.s / u_pattern_size_b.y);
    vec2 pos_b = mix(u_pattern_tl_b, u_pattern_br_b, vec2(x_b, y_b));

    vec4 color = mix(texture2D(u_image, pos_a), texture2D(u_image, pos_b), u_fade);

    alpha *= u_opacity;

    gl_FragColor = color * alpha;
}
)linepattern_frag",
    },
    {
        R"linesdf_vert(#version 120
// floor(127 / 2) == 63.0
// the maximum allowed miter limit is 2.0 at the moment. the extrude normal is
// stored in a byte (-128..127). we scale regular normals up to length 63, but
// there are also "special" normals that have a bigger length (of up to 126 in
// this case).
// #define scale 63.0
#define scale 0.015873016

attribute vec2 a_pos;
attribute vec4 a_data;

// matrix is for the vertex position, exmatrix is for rotating and projecting
// the extrusion vector.
uniform mat4 u_matrix;
uniform mat4 u_exmatrix;

// shared
uniform float u_ratio;
uniform vec2 u_linewidth;
uniform vec2 u_patternscale_a;
uniform float u_tex_y_a;
uniform vec2 u_patternscale_b;
uniform float u_tex_y_b;

varying vec2 v_normal;
varying vec2 v_tex_a;
varying vec2 v_tex_b;

void main() {
    vec2 a_extrude = a_data.xy;
    float a_linesofar = a_data.z * 128.0 + a_data.w;

    // We store the texture normals in the most insignificant bit
    // transform y so that 0 => -1 and 1 => 1
    // In the texture normal, x is 0 if the normal points straight up/down and 1 if it's a round cap
    // y is 1 if the normal points up, and -1 if it points down
    vec2 normal = mod(a_pos, 2.0);
    normal.y = sign(normal.y - 0.5);
    v_normal = normal;

    // Scale the extrusion vector down to a normal and then up by the line width
    // of this vertex.
    vec4 dist = vec4(u_linewidth.s * a_extrude * scale, 0.0, 0.0);

    // Remove the texture normal bit of the position before scaling it with the
    // model/view matrix. Add the extrusion vector *after* the model/view matrix
    // because we're extruding the line in pixel space, regardless of the current
    // tile's zoom level.
    gl_Position = u_matrix * vec4(floor(a_pos * 0.5), 0.0, 1.0) + u_exmatrix * dist;

    v_tex_a = vec2(a_linesofar * u_patternscale_a.x, normal.y * u_patternscale_a.y + u_tex_y_a);
    v_tex_b = vec2(a_linesofar * u_patternscale_b.x, normal.y * u_patternscale_b.y + u_tex_y_b);
}
)linesdf_vert",
        R"linesdf_frag(#version 120
uniform vec2 u_linewidth;
uniform vec4 u_color;
uniform float u_blur;
uniform sampler2D u_image;
uniform float u_sdfgamma;
uniform float u_mix;

varying vec2 v_normal;
varying vec2 v_tex_a;
varying vec2 v_tex_b;

void main() {
    // Calculate the distance of the pixel from the line in pixels.
    float dist = length(v_normal) * u_linewidth.s;

    // Calculate the antialiasing fade factor. This is either when fading in
    // the line in case of an offset line (v_linewidth.t) or when fading out
    // (v_linewidth.s)
    float alpha = clamp(min(dist - (u_linewidth.t - u_blur), u_linewidth.s - dist) / u_blur, 0.0, 1.0);

    float sdfdist_a = texture2D(u_image, v_tex_a).a;
    float sdfdist_b = texture2D(u_image, v_tex_b).a;
    float sdfdist = mix(sdfdist_a, sdfdist_b, u_mix);
    alpha *= smoothstep(0.5 - u_sdfgamma, 0.5 + u_sdfgamma, sdfdist);

    gl_FragColor = u_color * alpha;
}
)linesdf_frag",
    },
    {
        R"dot_vert(#version 120
uniform mat4 u_matrix;
uniform float u_size;

attribute vec2 a_pos;

void main(void) {
    gl_Position = u_matrix * vec4(a_pos, 0, 1);
    gl_PointSize = u_size;
}
)dot_vert",
        R"dot_frag(#version 120
uniform vec4 u_color;
uniform float u_blur;

void main() {
    float dist = length(gl_PointCoord - 0.5);
    float t = smoothstep(0.5, 0.5 - u_blur, dist);

    gl_FragColor = u_color * t;
}
)dot_frag",
    },
    {
        R"icon_vert(#version 120
attribute vec2 a_pos;
attribute vec2 a_offset;
attribute vec4 a_data1;
attribute vec4 a_data2;


// matrix is for the vertex position, exmatrix is for rotating and projecting
// the extrusion vector.
uniform mat4 u_matrix;
uniform mat4 u_exmatrix;
uniform float u_angle;
uniform float u_zoom;
uniform float u_flip;
uniform float u_fadedist;
uniform float u_minfadezoom;
uniform float u_maxfadezoom;
uniform float u_fadezoom;
uniform float u_opacity;

uniform vec2 u_texsize;

varying vec2 v_tex;
varying float v_alpha;

void main() {
    vec2 a_tex = a_data1.xy;
    float a_labelminzoom = a_data1[2];
    float a_angle = a_data1[3];
    vec2 a_zoom = a_data2.st;
    float a_minzoom = a_zoom[0];
    float a_maxzoom = a_zoom[1];
    vec2 a_range = a_data2.pq;
    float a_rangeend = a_range[0];
    float a_rangestart = a_range[1];

    float a_fadedist = 10.0;
    float rev = 0.0;

    // u_angle is angle of the map, -128..128 representing 0..2PI
    // a_angle is angle of the label, 0..256 representing 0..2PI, where 0 is horizontal text
    float rotated = mod(a_angle + u_angle, 256.0);
    // if the label rotates with the map, and if the rotated label is upside down, hide it
    if (u_flip > 0.0 && rotated >= 64.0 && rotated < 192.0) rev = 1.0;

    // If the label should be invisible, we move the vertex outside
    // of the view plane so that the triangle gets clipped. This makes it easier
    // for us to create degenerate triangle strips.
    // u_zoom is the current zoom level adjusted for the change in font size
    float z = 2.0 - step(a_minzoom, u_zoom) - (1.0 - step(a_maxzoom, u_zoom)) + rev;

    // fade out labels
    float alpha = clamp((u_fadezoom - a_labelminzoom) / u_fadedist, 0.0, 1.0);

    if (u_fadedist >= 0.0) {
        v_alpha = alpha;
    } else {
        v_alpha = 1.0 - alpha;
    }
    if (u_maxfadezoom < a_labelminzoom) {
        v_alpha = 0.0;
    }
    if (u_minfadezoom >= a_labelminzoom) {
        v_alpha = 1.0;
    }

    // if label has been faded out, clip it
    z += step(v_alpha, 0.0);

    // all the angles are 0..256 representing 0..2PI
    // hide if (angle >= a_rangeend && angle < rangestart)
    z += step(a_rangeend, u_angle) * (1.0 - step(a_rangestart, u_angle));

    gl_Position = u_matrix * vec4(a_pos, 0, 1) + u_exmatrix * vec4(a_offset / 64.0, z, 0);
    v_tex = a_tex / u_texsize;

    v_alpha *= u_opacity;
}
)icon_vert",
        R"icon_frag(#version 120
uniform sampler2D u_texture;

varying vec2 v_tex;
varying float v_alpha;

void main() {
    gl_FragColor = texture2D(u_texture, v_tex) * v_alpha;
}
)icon_frag",
    },

};

}
#endif

// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

// clang-format off
const char *const kGmVertexShaderSource_ =
    "#version 330 core\n"

    "layout (location = 0) in vec2 a_Position;\n"
    "layout (location = 1) in vec2 a_TextureCoordinates;\n"

    "out vec2 v_TextureCoordinates;\n"

    "void main() {\n"
      "gl_Position = vec4(a_Position, 0.0, 1.0);\n"
      "v_TextureCoordinates = a_TextureCoordinates;\n"
    "}\n";
// clang-format on

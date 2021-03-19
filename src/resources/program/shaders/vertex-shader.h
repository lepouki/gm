// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

// clang-format off
const char *const kGmVertexShaderSource_ =
    "#version 330 core\n"

    "layout (location = 0) in vec2 a_Position;\n"

    "void main() {\n"
      "gl_Position = vec4(a_Position, 0.0, 1.0);\n"
    "}\n";
// clang-format on

// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

// clang-format off
const char *const kGmFragmentShaderSource_ =
    "#version 330 core\n"

    "in vec2 v_TextureCoordinates;\n"

    "out vec4 f_Color;\n"

    "vec2 ComplexMultiply(vec2 a, vec2 b) {\n"
      "return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);\n"
    "}\n"

    "vec2 ComplexSquare(vec2 z) {\n"
      "return ComplexMultiply(z, z);\n"
    "}\n"

    "float ComplexSquareMag(vec2 z) {\n"
      "return z.x * z.x + z.y * z.y;\n"
    "}\n"

    "vec3 HsvToRgb(vec3 hsv) {\n"
      "vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);\n"
      "vec3 p = abs(fract(hsv.xxx + K.xyz) * 6.0 - K.www);\n"
      "return hsv.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), hsv.y);\n"
    "}\n"

    "vec3 IterToRgb(int iterations) {\n"
      "int h_deg = iterations % 360;\n"
      "vec3 hsv = vec3(float(h_deg) / 360.0, 0.9, 1.0);\n"
      "return HsvToRgb(hsv);\n"
    "}\n"

    "const int kMaxIterations = 100;\n"

    "void main() {\n"
      "vec2 c = v_TextureCoordinates * 2.0 - vec2(1.5, 1.0);\n"
      "vec2 z = c;\n"

      "int i = 0;\n"
      "for (; (i < kMaxIterations) && (ComplexSquareMag(z) < 16.0); ++i) {\n"
        "z = ComplexSquare(z) + c;"
      "}\n"

      "if (i == kMaxIterations) {\n"
        "f_Color = vec4(0.0);\n"
      "} else {\n"
        "f_Color = vec4(IterToRgb(i), 1.0);\n"
      "}\n"
    "}\n";
// clang-format on

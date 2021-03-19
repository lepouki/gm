// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

/**
 * Indicates that a parameter is used as output.
 */
#define GM_OUT_PARAM

#ifndef NDEBUG
#  define GM_DEBUG
#else
#  define GM_RELEASE
#endif

#define GM_STRINGIFY_HELPER_(x) #x
#define GM_STRINGIFY(x) GM_STRINGIFY_HELPER_(x)

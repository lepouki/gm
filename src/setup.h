// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

/**
 * Indicates that a parameter is used as output.
 */
#define GM_OUT_PARAM

/**
 * Indicates that a variable may be unused.
 */
#define GM_MAYBE_UNUSED

#ifndef NDEBUG
/**
 * More explicit than `!NDEBUG`.
 */
#  define GM_DEBUG
#endif

// Copyright 2022 Arturo Avila (@ADPenrose)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

/* This is a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
#define XXX KC_NO

#define LAYOUT( \
         k01, k02, k03, k04, k05, k06, k07,            k68, k69, k6a, k6b, k6c, k6d, k6e, k6f, kb8, \
         k12, k13, k14, k15, k16, k17,                 k78, k79, k7a, k7b, k7c, k7d, k7e, k7f, kb9, \
         k21, k23, k24, k25, k26, k27,                 k88, k89, k8a, k8b, k8c, k8d, k8e, k8f, kba, \
         k31, k32, k33, k34, k35, k36, k37,            k98, k99, k9a, k9b, k9c, k9d, k9e, k9f, \
    k20, k40, k50, k41, k51,                                     ka8, ka9, kaa, kab, kad, kae, kaf  \
) { \
    { XXX, k01, k02, k03, k04, k05, k06, k07 }, \
    { XXX, XXX, k12, k13, k14, k15, k16, k17 }, \
    { k20, k21, XXX, k23, k24, k25, k26, k27 }, \
    { XXX, k31, k32, k33, k34, k35, k36, k37 }, \
    { k40, k41, XXX, XXX, XXX, XXX, XXX, XXX }, \
    { k50, k51, XXX, XXX, XXX, XXX, XXX, XXX }, \
    \
    { k68, k69, k6a, k6b, k6c, k6d, k6e, k6f }, \
    { k78, k79, k7a, k7b, k7c, k7d, k7e, k7f }, \
    { k88, k89, k8a, k8b, k8c, k8d, k8e, k8f }, \
    { k98, k99, k9a, k9b, k9c, k9d, k9e, k9f }, \
    { ka8, ka9, kaa, kab, XXX, kad, kae, kaf }, \
    { kb8, kb9, kba, XXX, XXX, XXX, XXX, XXX } \
}

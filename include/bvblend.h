/*
 * bvblend.h
 *
 * Copyright (C) 2011 Texas Instruments, Inc.
 *
 * This file is part of BLTsville, an open application programming interface
 * (API) for accessing 2-D software or hardware implementations.
 *
 * This work is licensed under the Creative Commons Attribution-NoDerivs 3.0
 * Unported License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nd/3.0/ or send a letter to
 * Creative Commons, 444 Castro Street, Suite 900, Mountain View, California,
 * 94041, USA.
 */

/*
 * This file defines the types of shared blends available.
 *
 * To extend the list of blends, create a file containing additional
 * enumerations to be added to enum bvblend below.  Then #define
 * BVBLEND_EXTERNAL_INCLUDE as the name of that file before including
 * this file in your project.
 */

#ifndef BVBLEND_H
#define BVBLEND_H

/*
 * bvblend - specifies the type of blending operation to perform; only valid
 *	     when BVFLAG_BLEND is set in the bvbltparams.flags field.
 */
#define BVBLENDDEF_FORMAT_SHIFT 28
#define BVBLENDDEF_FORMAT_CLASSIC	(0x0 << BVBLENDDEF_FORMAT_SHIFT)
#define BVBLENDDEF_FORMAT_ESSENTIAL	(0x1 << BVBLENDDEF_FORMAT_SHIFT)
#define BVBLENDDEF_FORMAT_MASK (0xF << BVBLENDDEF_FORMAT_SHIFT)

/*
 * Definitions for BVBLDNDDEF_FORMAT_PORTERDUFF, use the following equations:
 *   Cd = C1 x K1 + C2 x K2
 *   Ad = A1 x K3 + A2 x K4
 *
 * where:
 *   Cd: destination color
 *   C1: source 1 color
 *   C2: source 2 color
 *   Ad: destination alpha
 *   A1: source 1 alpha
 *   A2: source 2 alpha
 *   K*: one of the constants defined using the bitfields below.
 */
#define BVBLENDDEF_ALL_ZERO 0x00
#define BVBLENDDEF_ALL_C1 0
#define BVBLENDDEF_ALL_A1 1
#define BVBLENDDEF_ALL_C2 2
#define BVBLENDDEF_ALL_A2 3
#define BVBLENDDEF_ALL_ONE_MINUS_C1 (0 << 2)
#define BVBLENDDEF_ALL_ONE_MINUS_A1 (1 << 2)
#define BVBLENDDEF_ALL_ONE_MINUS_C2 (2 << 2)
#define BVBLENDDEF_ALL_ONE_MINUS_A2 (3 << 2)
#define BVBLENDDEF_ALL_ONE  0x3F

#define BVBLENDDEF_K1_SHIFT 18
#define BVBLENDDEF_K2_SHIFT 12
#define BVBLENDDEF_K3_SHIFT 6
#define BVBLENDDEF_K4_SHIFT 0

/*
 * The following definitions are be used to modify the enumerations.
 */
#define BVBLENDDEF_REMOTE	0x00800000	/* mask surface provides alpha
						   for source 1 */

/* These enable global alpha and define the type of globalalpha */
#define BVBLENDDEF_GLOBAL_SHIFT 22
#define BVBLENDDEF_GLOBAL_UCHAR	 (0 << BVBLENDDEF_GLOBAL_SHIFT)
#define BVBLENDDEF_GLOBAL_USHORT (1 << BVBLENDDEF_GLOBAL_SHIFT)
#define BVBLENDDEF_GLOBAL_ULONG	 (2 << BVBLENDDEF_GLOBAL_SHIFT)
#define BVBLENDDEF_GLOBAL_FLOAT	 (3 << BVBLENDDEF_GLOBAL_SHIFT)
#define BVBLENDDEF_GLOBAL_MASK (3 << BVBLENDDEF_GLOBAL_SHIFT)

/*
 * For BVBLENDDEF_FORMAT_CLASSIC, the specific blend operation is constructed
 * using the definitions above.
 *
 * Note that the equations assume the surfaces are premultiplied.  So
 * if the surface formats indicate that they are not premultiplied, the
 * alpha multiplication of each color is done prior to using the surface
 * values in the equations.
 *
 * For example, BVBLEND_SRC1OVER specifies the equations:
 *   Cd = C1 x 1 + C2 x (1 - A1)
 *   Ad = A1 x 1 + A2 x (1 - A1)
 * If the format of surface 1 is non-premultiplied, the equations
 * are modified to include the multiplication explicitly:
 *   Cd = C1 x A1 x 1 + C2 x (1 - A1)
 *   Ad = A1 x 1 + A2 x (1 - A1)
 * Likewise, if the format of surface 2 is non-premultiplied, the
 * equations are modified for this:
 *   Cd = C1 x 1 + C2 x A2 x (1 - A1)
 *   Ad = A1 x 1 + A2 x (1 - A1)
 *
 * For convenience, the Porter-Duff versions are defined below.  These utilize
 * the local alpha if the formats include it.  To use global or remote alpha,
 * these enumerations need to be modified.  For example, to include the global
 * alpha in the Porter-Duff SRC1OVER blend, the blend could be defined like
 * this:
 *   params.op.blend = BVBLEND_SRC1OVER + BVBLENDDEF_GLOBAL_UCHAR;
 * To include the remote alpha, the blend could be defined like this:
 *   params.op.blend = BVBLEND_SRC1OVER + BVBLENDDEF_REMOTE;
 * And to include both:
 *   params.op.blend = BVBLEND_SRC1OVER +
 *		       BVBLENDDEF_GLOBAL_UCHAR +
 *		       BVBLENDDEF_REMOTE;
 *
 * Note that if the source color formats include local alphas, the local
 * alphas, global alpha, and remote alpha will be used together:
 *
 * For an interface like DirectFB, where the source and destination blending
 * functions are set separately, the blend value must be created manually from
 * the arguments passed to those functions
 * (e.g. SetSrcBlendFunction()/SetDstBlendFunction()).
 */

enum bvblend {
  /* Porter-Duff blending equations */
	BVBLEND_CLEAR = BVBLENDDEF_FORMAT_CLASSIC |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K1_SHIFT) |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K2_SHIFT) |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K3_SHIFT) |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K4_SHIFT),
	BVBLEND_SRC1 =	BVBLENDDEF_FORMAT_CLASSIC |
			(BVBLENDDEF_ALL_ONE << BVBLENDDEF_K1_SHIFT) |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K2_SHIFT) |
			(BVBLENDDEF_ALL_ONE << BVBLENDDEF_K3_SHIFT) |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K4_SHIFT),
	BVBLEND_SRC2 =	BVBLENDDEF_FORMAT_CLASSIC |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K1_SHIFT) |
			(BVBLENDDEF_ALL_ONE << BVBLENDDEF_K2_SHIFT) |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K3_SHIFT) |
			(BVBLENDDEF_ALL_ONE << BVBLENDDEF_K4_SHIFT),
	BVBLEND_SRC1OVER = BVBLENDDEF_FORMAT_CLASSIC |
			(BVBLENDDEF_ALL_ONE << BVBLENDDEF_K1_SHIFT) |
			(BVBLENDDEF_ALL_ONE_MINUS_A1 << BVBLENDDEF_K2_SHIFT) |
			(BVBLENDDEF_ALL_ONE << BVBLENDDEF_K3_SHIFT) |
			(BVBLENDDEF_ALL_ONE_MINUS_A1 << BVBLENDDEF_K4_SHIFT),
	BVBLEND_SRC2OVER = BVBLENDDEF_FORMAT_CLASSIC |
			(BVBLENDDEF_ALL_ONE_MINUS_A2 << BVBLENDDEF_K1_SHIFT) |
			(BVBLENDDEF_ALL_ONE << BVBLENDDEF_K2_SHIFT) |
			(BVBLENDDEF_ALL_ONE_MINUS_A2 << BVBLENDDEF_K3_SHIFT) |
			(BVBLENDDEF_ALL_ONE << BVBLENDDEF_K4_SHIFT),
	BVBLEND_SRC1IN = BVBLENDDEF_FORMAT_CLASSIC |
			(BVBLENDDEF_ALL_A2 << BVBLENDDEF_K1_SHIFT) |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K2_SHIFT) |
			(BVBLENDDEF_ALL_A2 << BVBLENDDEF_K3_SHIFT) |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K4_SHIFT),
	BVBLEND_SRC2IN = BVBLENDDEF_FORMAT_CLASSIC |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K1_SHIFT) |
			(BVBLENDDEF_ALL_A1 << BVBLENDDEF_K2_SHIFT) |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K3_SHIFT) |
			(BVBLENDDEF_ALL_A1 << BVBLENDDEF_K4_SHIFT),
	BVBLEND_SRC1OUT = BVBLENDDEF_FORMAT_CLASSIC |
			(BVBLENDDEF_ALL_ONE_MINUS_A2 << BVBLENDDEF_K1_SHIFT) |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K2_SHIFT) |
			(BVBLENDDEF_ALL_ONE_MINUS_A2 << BVBLENDDEF_K3_SHIFT) |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K4_SHIFT),
	BVBLEND_SRC2OUT = BVBLENDDEF_FORMAT_CLASSIC |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K1_SHIFT) |
			(BVBLENDDEF_ALL_ONE_MINUS_A1 << BVBLENDDEF_K2_SHIFT) |
			(BVBLENDDEF_ALL_ZERO << BVBLENDDEF_K3_SHIFT) |
			(BVBLENDDEF_ALL_ONE_MINUS_A1 << BVBLENDDEF_K4_SHIFT),
	BVBLEND_SRC1ATOP = BVBLENDDEF_FORMAT_CLASSIC |
			(BVBLENDDEF_ALL_A2 << BVBLENDDEF_K1_SHIFT) |
			(BVBLENDDEF_ALL_ONE_MINUS_A1 << BVBLENDDEF_K2_SHIFT) |
			(BVBLENDDEF_ALL_A2 << BVBLENDDEF_K3_SHIFT) |
			(BVBLENDDEF_ALL_ONE_MINUS_A1 << BVBLENDDEF_K4_SHIFT),
	BVBLEND_SRC2ATOP = BVBLENDDEF_FORMAT_CLASSIC |
			(BVBLENDDEF_ALL_ONE_MINUS_A2 << BVBLENDDEF_K1_SHIFT) |
			(BVBLENDDEF_ALL_A1 << BVBLENDDEF_K2_SHIFT) |
			(BVBLENDDEF_ALL_ONE_MINUS_A2 << BVBLENDDEF_K3_SHIFT) |
			(BVBLENDDEF_ALL_A1 << BVBLENDDEF_K4_SHIFT),
	BVBLEND_XOR = BVBLENDDEF_FORMAT_CLASSIC |
			(BVBLENDDEF_ALL_ONE_MINUS_A2 << BVBLENDDEF_K1_SHIFT) |
			(BVBLENDDEF_ALL_ONE_MINUS_A1 << BVBLENDDEF_K2_SHIFT) |
			(BVBLENDDEF_ALL_ONE_MINUS_A2 << BVBLENDDEF_K3_SHIFT) |
			(BVBLENDDEF_ALL_ONE_MINUS_A1 << BVBLENDDEF_K4_SHIFT),
	BVBLEND_PLUS = BVBLENDDEF_FORMAT_CLASSIC |
			(BVBLENDDEF_ALL_ONE << BVBLENDDEF_K1_SHIFT) |
			(BVBLENDDEF_ALL_ONE << BVBLENDDEF_K2_SHIFT) |
			(BVBLENDDEF_ALL_ONE << BVBLENDDEF_K3_SHIFT) |
			(BVBLENDDEF_ALL_ONE << BVBLENDDEF_K4_SHIFT),

	BVBLEND_NORMAL = BVBLENDDEF_FORMAT_ESSENTIAL + 0,
	BVBLEND_LIGHTEN = BVBLENDDEF_FORMAT_ESSENTIAL + 1,
	BVBLEND_DARKEN = BVBLENDDEF_FORMAT_ESSENTIAL + 2,
	BVBLEND_MULTIPLY = BVBLENDDEF_FORMAT_ESSENTIAL + 3,
	BVBLEND_AVERAGE = BVBLENDDEF_FORMAT_ESSENTIAL + 4,
	BVBLEND_ADD = BVBLENDDEF_FORMAT_ESSENTIAL + 5,
	BVBLEND_LINEAR_DODGE = BVBLEND_ADD,
	BVBLEND_SUBTRACT = BVBLENDDEF_FORMAT_ESSENTIAL + 6,
	BVBLEND_LINEAR_BURN = BVBLEND_SUBTRACT,
	BVBLEND_DIFFERENCE = BVBLENDDEF_FORMAT_ESSENTIAL + 7,
	BVBLEND_NEGATE = BVBLENDDEF_FORMAT_ESSENTIAL + 8,
	BVBLEND_SCREEN = BVBLENDDEF_FORMAT_ESSENTIAL + 9,
	BVBLEND_EXCLUSION = BVBLENDDEF_FORMAT_ESSENTIAL + 10,
	BVBLEND_OVERLAY = BVBLENDDEF_FORMAT_ESSENTIAL + 11,
	BVBLEND_SOFT_LIGHT = BVBLENDDEF_FORMAT_ESSENTIAL + 12,
	BVBLEND_HARD_LIGHT = BVBLENDDEF_FORMAT_ESSENTIAL + 13,
	BVBLEND_COLOR_DODGE = BVBLENDDEF_FORMAT_ESSENTIAL + 14,
	BVBLEND_COLOR_BURN = BVBLENDDEF_FORMAT_ESSENTIAL + 15,
	BVBLEND_LINEAR_LIGHT = BVBLENDDEF_FORMAT_ESSENTIAL + 16,
	BVBLEND_VIVID_LIGHT = BVBLENDDEF_FORMAT_ESSENTIAL + 17,
	BVBLEND_PIN_LIGHT = BVBLENDDEF_FORMAT_ESSENTIAL + 18,
	BVBLEND_HARD_MIX = BVBLENDDEF_FORMAT_ESSENTIAL + 19,
	BVBLEND_REFLECT = BVBLENDDEF_FORMAT_ESSENTIAL + 20,
	BVBLEND_GLOW = BVBLENDDEF_FORMAT_ESSENTIAL + 21,
	BVBLEND_PHOENIX = BVBLENDDEF_FORMAT_ESSENTIAL + 22,
#ifdef BVBLEND_EXTERNAL_INCLUDE
#define BVBLEND_EXTERNAL_INCLUDE
#endif
};

#endif /* BVBLEND_H */

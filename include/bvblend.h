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
#define BVBLENDDEF_FORMAT_MASK (0xF << BVBLENDDEF_FORMAT_SHIFT)

#define BVBLENDDEF_FORMAT_CLASSIC	(0x0 << BVBLENDDEF_FORMAT_SHIFT)
#define BVBLENDDEF_FORMAT_ESSENTIAL	(0x1 << BVBLENDDEF_FORMAT_SHIFT)

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
#define BVBLENDDEF_REMOTE	0x08000000	/* mask surface provides alpha
						   for source 1 */

/* Bit 26 reserved */

/* These enable global alpha and define the type of globalalpha */
#define BVBLENDDEF_GLOBAL_SHIFT 24
#define BVBLENDDEF_GLOBAL_MASK	(3 << BVBLENDDEF_GLOBAL_SHIFT)

#define BVBLENDDEF_GLOBAL_NONE	(0 << BVBLENDDEF_GLOBAL_SHIFT)
#define BVBLENDDEF_GLOBAL_UCHAR	(1 << BVBLENDDEF_GLOBAL_SHIFT)
/* 2 reserved */
#define BVBLENDDEF_GLOBAL_FLOAT	(3 << BVBLENDDEF_GLOBAL_SHIFT)

union bvalpha {
	unsigned char size8;	/* btwn 0 (0.0) and 255 (1.0) */
	float fp;		/* btwn 0.0 and 1.0 */
};

/*
 * For BVBLENDDEF_FORMAT_CLASSIC, the specific blend operation is constructed
 * using the definitions above.  Following the format identifier, the
 * remaining 28 bits are divided into four groups of 6 bits plus 4 modifier
 * bits.
 *
 * Starting with the following equations:
 *  Cd = K1 x C1 + K2 x C2
 *  Ad = K3 x A1 + K4 x A2
 *
 * The four 6-bit fields are used to specify the values for the four constant
 * (K) values.
 *
 * These 6-bit fields are further divided into 3 groups of 2 bits each.  The
 * rightmost 2 groups specify parameters used as indicated by the leftmost
 * group.
 *
 * Bits n through n+1 specify the second parameter:
 * 00b = 1-Cs
 * 01b = 1-As
 * 10b = 1-Cd
 * 11b = 1-Ad
 *
 * Bits n+2 through n+3 specify the first parameter:
 * 00b = Cs
 * 01b = As
 * 10b = Cd
 * 11b = Ad
 *
 * Bits n+4 through n+5 indicate how the two fields are used:
 * 00b = only the first parameter is used (special case for both 00b)
 * 01b = minimum of two fields
 * 10b = maximum of two fields
 * 11b = only the second parameter is used (special case for both 11b)
 *
 * Put together, these can define portions of the blend equations that can be
 * put together in a variety of ways:
 *
 * 00 00 00: 0
 * 00 00 11: C1
 * 00 01 11: A1
 * 00 10 11: C2
 * 00 11 11: A2
 * 11 00 00: 1-C1
 * 11 00 01: 1-A1
 * 11 00 10: 1-C2
 * 11 00 11: 1-A2
 * 11 11 11: 1
 * 01 01 11: min(A1,1-A2)
 *
 * Putting these together into the proper constants, the blending equations
 * can be built for DirectFB or Porter-Duff:
 *
 * For Porter-Duff, the equations can be more specifically defined.  For
 * convenience, these are defined below.  These utilize the local alpha as
 * indicated.  To use global or remote alpha, these enumerations need to be
 * modified.  For example, to include the global alpha in the Porter-Duff
 * SRC1OVER blend, the blend could be defined like this:
 *   params.op.blend = BVBLEND_SRC1OVER + BVBLENDDEF_GLOBAL_UCHAR;
 *
 * To include the remote alpha, the blend could be defined like this:
 *   params.op.blend = BVBLEND_SRC1OVER + BVBLENDDEF_REMOTE;
 *
 * And to include both:
 *   params.op.blend = BVBLEND_SRC1OVER +
 *		       BVBLENDDEF_GLOBAL_UCHAR +
 *		       BVBLENDDEF_REMOTE;
 *
 * Note that if the source color formats include local alphas, the local
 * alphas, global alpha, and remote alpha will be used together.
 *
 * Note also that the equations assume the surfaces are premultiplied.  So
 * if the surface formats indicate that they are not premultiplied, the
 * alpha multiplication of each color is done prior to using the surface
 * values in the equations.
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
 * For DirectFB, the SetSrcBlendFunction() and SetDstBlendFunction() can
 * specify 121 combinations of blends (11 x 11). It's impractical to
 * specify these combinations individually.  INstead, the settings indicated
 * by each call should be bitwise OR'd to make the proper single value used in
 * BLTsville.
 *
 * binary value                                 <- SetSrcBlendFunction()
 *           [--K1--] [--K2--] [--K3--] [--K4--]
 * 00000000 00 00 00 xx xx xx 00 00 00 xx xx xx <- DSBF_ZERO
 * 00000000 11 11 11 xx xx xx 11 11 11 xx xx xx <- DSBF_ONE
 * 00000000 00 00 11 xx xx xx 00 01 11 xx xx xx <- DSBF_SRCCOLOR
 * 00000000 11 00 00 xx xx xx 11 00 01 xx xx xx <- DSBF_INVSRCCOLOR
 * 00000000 00 01 11 xx xx xx 00 01 11 xx xx xx <- DSBF_SRCALPHA
 * 00000000 11 00 01 xx xx xx 11 00 01 xx xx xx <- DSBF_INVSRCALPHA
 * 00000000 00 11 11 xx xx xx 00 11 11 xx xx xx <- DSBF_DESTALPHA
 * 00000000 11 00 11 xx xx xx 11 00 11 xx xx xx <- DSBF_INVDESTALPHA
 * 00000000 00 10 11 xx xx xx 00 11 11 xx xx xx <- DSBF_DESTCOLOR
 * 00000000 11 00 10 xx xx xx 11 00 11 xx xx xx <- DSBF_INVDESTCOLOR
 * 00000000 01 01 11 xx xx xx 11 11 11 xx xx xx <- DSBF_SRCALPHASAT
 *
 * binary value                                 <- SetDstBlendFunction()
 * 00000000 xx xx xx 00 00 00 xx xx xx 00 00 00 <- DSBF_ZERO
 * 00000000 xx xx xx 11 11 11 xx xx xx 11 11 11 <- DSBF_ONE
 * etc.
 *
 */

/*
 * For FORMAT_ESSENTIAL, the variety of well-known blending functions from
 * popular image manipulation programs are specified.
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

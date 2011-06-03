/*
 * bltsville.h
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

#ifndef BLTSVILLE_H
#define BLTSVILLE_H

#include <ocd.h>
#include <bverror.h>
#include <bvblend.h>
#include <bvfilter.h>

/*
 * bvrect - This structure is used to specify rectangles in BLTsville.
 */
struct bvrect {
	int left;
	int top;
	unsigned int width;
	unsigned int height;
};


/*
 * bvsurfdesc - This structure specifies the way a buffer should be used in a
 * 2-D context.
 */

struct bvsurfgeom {
	unsigned int structsize;	/* used to identify struct ver */
	enum ocdformat format;		/* color format of surface */
	unsigned int width;		/* width of the surface in pixels */
	unsigned int height;		/* height of the surface in lines */
	int orientation;		/* angle of the surface in degrees
					   (multiple of 90 only) */
	long virtstride;		/* distance from one pixel to the
					   pixel immediately below it in
					   virtual space */
	enum ocdformat paletteformat;	/* format of palette */
	void *palette;			/* array of palette entries of
					   paletteformat; only valid when
					   format includes BVFMTDEF_LUT;
					   number of entries is 2^bpp. */
};

/*
 * bvbuffermap - This is a private structure used by BLTsville
 * implementations to manage resources associated with a buffer.  A pointer
 * to this is returned from bv_map() and used in subsequent bv_blt() and
 * bv_unmap() calls.
 */
struct bvbuffermap;

/*
 * bvbufferdesc - This structure is used to specify the buffer parameters
 * in a call to bv_map().
 */
struct bvbufferdesc {
	unsigned int structsize;	/* used to identify struct ver */
	void *virtaddr;			/* virtual ptr to start of buffer */
	unsigned long length;		/* length of the buffer in bytes */
};

/*
 * BVFLAG_* - These define the type of BLT to be performed and are placed in
 * the bvparams.flags element.
 */
#define BVFLAG_OP_SHIFT	0
/* 0 reserved */
#define BVFLAG_ROP	(0x1 << BVFLAG_OP_SHIFT) /* ROP4 spec'd in rop */
#define BVFLAG_BLEND	(0x2 << BVFLAG_OP_SHIFT) /* blend spec'd in blend */
/* 3 reserved */
#define BVFLAG_FILTER	(0x4 << BVFLAG_OP_SHIFT) /* filter spec'd in filter */
/* 5-F reserved */
#define BVFLAG_OP_MASK	(0xF << BVFLAG_OP_SHIFT)

#define BVFLAG_KEY_SRC		0x00000010 /* source color key - value spec'd
					      by pcolorkey; Mutually exclusive
					      with BVFLAG_KEY_DST */
#define BVFLAG_KEY_DST		0x00000020 /* dest color key - value spec'd
					      by pcolorkey; Mutually exclusive
					      with BVFLAG_KEY_SRC */
#define BVFLAG_CLIP		0x00000040 /* clipping rectangle spec'd by
					      cliprect */
#define BVFLAG_SRCMASK		0x00000080 /* when scaling a masked copy, mask
					      at the source instead of the
					      (default) destination */

#define BVFLAG_ASYNC		0x00000100 /* call should return once queued */

#define BVFLAG_TILE_SRC1	0x00000200 /* source 1 is tiled */
#define BVFLAG_TILE_SRC2	0x00000400 /* source 2 is tiled */
#define BVFLAG_TILE_MASK	0x00000800 /* mask is tiled */

#define BVFLAG_BATCH_SHIFT	12
#define BVFLAG_BATCH_BEGIN	(1 << BVFLAG_BATCH_SHIFT) /* begin batch */
#define BVFLAG_BATCH_CONTINUE	(2 << BVFLAG_BATCH_SHIFT) /* continue batch */
#define BVFLAG_BATCH_END	(3 << BVFLAG_BATCH_SHIFT) /* end batch */
#define BVFLAG_BATCH_MASK	(3 << BVFLAG_BATCH_SHIFT)

#define BVFLAG_HORZ_FLIP_SRC1	0x00004000 /* flip src1 horizontally */
#define BVFLAG_VERT_FLIP_SRC1	0x00008000 /* flip src1 vertically */
#define BVFLAG_HORZ_FLIP_SRC2	0x00010000 /* flip src2 horizontally */
#define BVFLAG_VERT_FLIP_SRC2	0x00020000 /* flip src2 vertically */
#define BVFLAG_HORZ_FLIP_MASK	0x00040000 /* flip mask horizontally */
#define BVFLAG_VERT_FLIP_MASK	0x00080000 /* flip mask vertically */

#define BVFLAG_SCALE_RETURN	0x00100000 /* return scale type used */
#define BVFLAG_DITHER_RETURN	0x00200000 /* return dither type used */
/**** Bits 31-16 reserved ****/

/*
 * BVIMPL_* - BLTsville implementations may be combined under managers to
 * allow clients to take advantage of multiple implementations without doing
 * so explicitly.  The BVIMPL_* definition are placed into the
 * bvparams.implementation member by the client to override the manager's
 * choice of implementation.
 */
#define BVIMPL_ANY		0
#define BVIMPL_FIRST_HW		(1 << 31) /* Continues to the right */
#define BVIMPL_FIRST_CPU	(1 << 0)  /* Continues to the left */


/*
 * bvscalemode - This specifies the type of scaling to perform.
 */
#define BVSCALEDEF_VENDOR_SHIFT 24
#define BVSCALEDEF_VENDOR_ALL (0 << BVSCALEDEF_VENDOR_SHIFT)
#define BVSCALEDEF_VENDOR_TI  (1 << BVSCALEDEF_VENDOR_SHIFT)
/* 0xF0-0xFE reserved */
#define BVSCALEDEF_VENDOR_GENERIC (0xFF << BVSCALEDEF_VENDOR_SHIFT)
#define BVSCALEDEF_VENDOR_MASK (0xFF << BVSCALEDEF_VENDOR_SHIFT)

/***** VENDOR_GENERIC definitions *****/
/**** Bits 23-22 indicate classification ****/
#define BVSCALEDEF_CLASS_SHIFT	22
#define BVSCALEDEF_IMPLICIT	(0 << BVSCALEDEF_CLASS_SHIFT)
#define BVSCALEDEF_EXPLICIT	(1 << BVSCALEDEF_CLASS_SHIFT)
/* 2-3 reserved */

/**** IMPLICIT definitions ****/
/*** Bits 21-16 indicate the quality (speed) desired ***/
#define BVSCALEDEF_QUALITY_SHIFT 16
#define BVSCALEDEF_FASTEST	(0x00 << BVSCALEDEF_QUALITY_SHIFT)
#define BVSCALEDEF_GOOD		(0x15 << BVSCALEDEF_QUALITY_SHIFT)
#define BVSCALEDEF_BETTER	(0x2A << BVSCALEDEF_QUALITY_SHIFT)
#define BVSCALEDEF_BEST		(0x3F << BVSCALEDEF_QUALITY_SHIFT)
/* Bits 15-12 are reserved */
/*** Bits 11-8 indicate the desired technique ***/
#define BVSCALEDEF_TECHNIQUE_SHIFT 8
#define BVSCALEDEF_DONT_CARE	(0x0 << BVSCALEDEF_TECHNIQUE_SHIFT)
#define BVSCALEDEF_NOT_NEAREST_NEIGHBOR	(0x1 << BVSCALEDEF_TECHNIQUE_SHIFT)
#define BVSCALEDEF_POINT_SAMPLE	(0x2 << BVSCALEDEF_TECHNIQUE_SHIFT)
#define BVSCALEDEF_INTERPOLATED	(0x3 << BVSCALEDEF_TECHNIQUE_SHIFT)
/* Bits 7-2 reserved */
/*** Bits 1-0 indicate the type of image ***/
#define BVSCALEDEF_TYPE_SHIFT 0
/* 0 don't know */
#define BVSCALEDEF_PHOTO	(1 << BVSCALEDEF_TYPE_SHIFT)
#define BVSCALEDEF_DRAWING	(2 << BVSCALEDEF_TYPE_SHIFT)
/* 3 reserved */

/**** EXPLICIT definitions ****/
/* Bits 21-16 reserved */
#define BVSCALEDEF_HORZ_SHIFT	8
#define BVSCALEDEF_VERT_SHIFT	0
#define BVSCALEDEF_NEAREST_NEIGHBOR	0x00
#define BVSCALEDEF_LINEAR		0x01
#define BVSCALEDEF_CUBIC		0x02
#define	BVSCALEDEF_3_TAP		0x03
/* 0x04 reserved */
#define BVSCALEDEF_5_TAP		0x05
/* 0x06 reserved */
#define BVSCALEDEF_7_TAP		0x07
/* 0x08 reserved */
#define BVSCALEDEF_9_TAP		0x09
/* 0x0A-0xFF reserved */
#define BVSCALEDEF_HORZ_MASK	(0xFF << BVSCALEDEF_HORZ_SHIFT)
#define BVSCALEDEF_VERT_MASK	(0xFF << BVSCALEDEF_VERT_SHIFT)

enum bvscalemode {
	BVSCALE_FASTEST =	BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_FASTEST |
				BVSCALEDEF_DONT_CARE,
	BVSCALE_FASTEST_NOT_NEAREST_NEIGHBOR = BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_FASTEST |
				BVSCALEDEF_NOT_NEAREST_NEIGHBOR,
	BVSCALE_FASTEST_POINT_SAMPLE = BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_FASTEST |
				BVSCALEDEF_POINT_SAMPLE,
	BVSCALE_FASTEST_INTERPOLATED = BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_FASTEST |
				BVSCALEDEF_INTERPOLATED,
	BVSCALE_FASTEST_PHOTO =	BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_FASTEST |
				BVSCALEDEF_PHOTO,
	BVSCALE_FASTEST_DRAWING = BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_FASTEST |
				BVSCALEDEF_DRAWING,
	BVSCALE_GOOD =		BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_GOOD |
				BVSCALEDEF_DONT_CARE,
	BVSCALE_GOOD_POINT_SAMPLE = BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_GOOD |
				BVSCALEDEF_POINT_SAMPLE,
	BVSCALE_GOOD_INTERPOLATED = BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_GOOD |
				BVSCALEDEF_INTERPOLATED,
	BVSCALE_GOOD_PHOTO =	BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_GOOD |
				BVSCALEDEF_PHOTO,
	BVSCALE_GOOD_DRAWING =	BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_GOOD |
				BVSCALEDEF_DRAWING,
	BVSCALE_BETTER =	BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_BETTER |
				BVSCALEDEF_DONT_CARE,
	BVSCALE_BETTER_POINT_SAMPLE = BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_BETTER |
				BVSCALEDEF_POINT_SAMPLE,
	BVSCALE_BETTER_INTERPOLATED = BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_BETTER |
				BVSCALEDEF_INTERPOLATED,
	BVSCALE_BETTER_PHOTO =	BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_BETTER |
				BVSCALEDEF_PHOTO,
	BVSCALE_BETTER_DRAWING = BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_BETTER |
				BVSCALEDEF_DRAWING,
	BVSCALE_BEST =		BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_BEST |
				BVSCALEDEF_DONT_CARE,
	BVSCALE_BEST_POINT_SAMPLE = BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_BEST |
				BVSCALEDEF_POINT_SAMPLE,
	BVSCALE_BEST_INTERPOLATED = BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_BEST |
				BVSCALEDEF_INTERPOLATED,
	BVSCALE_BEST_PHOTO =	BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_BEST |
				BVSCALEDEF_PHOTO,
	BVSCALE_BEST_DRAWING =	BVSCALEDEF_VENDOR_ALL |
				BVSCALEDEF_IMPLICIT |
				BVSCALEDEF_BEST |
				BVSCALEDEF_DRAWING,

	BVSCALE_NEAREST_NEIGHBOR = BVSCALEDEF_VENDOR_GENERIC |
		BVSCALEDEF_EXPLICIT |
		(BVSCALEDEF_NEAREST_NEIGHBOR << BVSCALEDEF_HORZ_SHIFT) |
		(BVSCALEDEF_NEAREST_NEIGHBOR << BVSCALEDEF_VERT_SHIFT),
	BVSCALE_BILINEAR = BVSCALEDEF_VENDOR_GENERIC |
		BVSCALEDEF_EXPLICIT |
		(BVSCALEDEF_LINEAR << BVSCALEDEF_HORZ_SHIFT) |
		(BVSCALEDEF_LINEAR << BVSCALEDEF_VERT_SHIFT),
	BVSCALE_BICUBIC = BVSCALEDEF_VENDOR_GENERIC |
		BVSCALEDEF_EXPLICIT |
		(BVSCALEDEF_CUBIC << BVSCALEDEF_HORZ_SHIFT) |
		(BVSCALEDEF_CUBIC << BVSCALEDEF_VERT_SHIFT),
	BVSCALE_3x3_TAP = BVSCALEDEF_VENDOR_GENERIC |
		BVSCALEDEF_EXPLICIT |
		(BVSCALEDEF_3_TAP << BVSCALEDEF_HORZ_SHIFT) |
		(BVSCALEDEF_3_TAP << BVSCALEDEF_VERT_SHIFT),
	BVSCALE_5x5_TAP = BVSCALEDEF_VENDOR_GENERIC |
		BVSCALEDEF_EXPLICIT |
		(BVSCALEDEF_5_TAP << BVSCALEDEF_HORZ_SHIFT) |
		(BVSCALEDEF_5_TAP << BVSCALEDEF_VERT_SHIFT),
	BVSCALE_7x7_TAP = BVSCALEDEF_VENDOR_GENERIC |
		BVSCALEDEF_EXPLICIT |
		(BVSCALEDEF_7_TAP << BVSCALEDEF_HORZ_SHIFT) |
		(BVSCALEDEF_7_TAP << BVSCALEDEF_VERT_SHIFT),
	BVSCALE_9x9_TAP = BVSCALEDEF_VENDOR_GENERIC |
		BVSCALEDEF_EXPLICIT |
		(BVSCALEDEF_9_TAP << BVSCALEDEF_HORZ_SHIFT) |
		(BVSCALEDEF_9_TAP << BVSCALEDEF_VERT_SHIFT),
#ifdef BVSCALE_EXTERNAL_INCLUDE
#include BVSCALE_EXTERNAL_INCLUDE
#endif
};


/*
 * bvdithermode - This defines the type of dithering to use.
 */
#define BVDITHERDEF_VENDOR_SHIFT 24
#define BVDITHERDEF_VENDOR_ALL (0 << BVDITHERDEF_VENDOR_SHIFT)
#define BVDITHERDEF_VENDOR_TI  (1 << BVDITHERDEF_VENDOR_SHIFT)
/* 0xF0-0xFE reserved */
#define BVDITHERDEF_VENDOR_GENERIC (0xFF << BVDITHERDEF_VENDOR_SHIFT)
#define BVDITHERDEF_VENDOR_MASK (0xFF << BVDITHERDEF_VENDOR_SHIFT)

/***** VENDOR_GENERIC definitions *****/
/* Bits 23-18 reserved */
/**** Bits 17-16 indicate the type of image - 0 = don't know ****/
#define BVDITHERDEF_TYPE_SHIFT 16
#define BVDITHERDEF_PHOTO	(0x01 << BVDITHERDEF_TYPE_SHIFT)
#define BVDITHERDEF_DRAWING	(0x02 << BVDITHERDEF_TYPE_SHIFT)
/**** Bits 15-8 indicate the desired technique ****/
#define BVDITHERDEF_TECHNIQUE_SHIFT 8
#define BVDITHERDEF_DONT_CARE	(0x00 << BVDITHERDEF_TECHNIQUE_SHIFT)
#define BVDITHERDEF_RANDOM	(0x01 << BVDITHERDEF_TECHNIQUE_SHIFT)
#define BVDITHERDEF_ORDERED	(0x02 << BVDITHERDEF_TECHNIQUE_SHIFT)
#define BVDITHERDEF_DIFFUSED	(0x04 << BVDITHERDEF_TECHNIQUE_SHIFT)
#define BVDITHERDEF_ON		(0xFF << BVDITHERDEF_TECHNIQUE_SHIFT)
/**** Bits 7-0 indicate the quality (speed) desired ****/
#define BVDITHERDEF_QUALITY_SHIFT 0
#define BVDITHERDEF_FASTEST	(0x00 << BVDITHERDEF_QUALITY_SHIFT)
#define BVDITHERDEF_GOOD	(0x55 << BVDITHERDEF_QUALITY_SHIFT)
#define BVDITHERDEF_BETTER	(0xAA << BVDITHERDEF_QUALITY_SHIFT)
#define BVDITHERDEF_BEST	(0xFF << BVDITHERDEF_QUALITY_SHIFT)

enum bvdithermode {
	BVDITHER_FASTEST =	BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_FASTEST |
				BVDITHERDEF_DONT_CARE,
	BVDITHER_FASTEST_ON =	BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_FASTEST |
				BVDITHERDEF_ON,
	BVDITHER_FASTEST_RANDOM = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_FASTEST |
				BVDITHERDEF_RANDOM,
	BVDITHER_FASTEST_ORDERED = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_FASTEST |
				BVDITHERDEF_ORDERED,
	BVDITHER_FASTEST_DIFFUSED = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_FASTEST |
				BVDITHERDEF_DIFFUSED,
	BVDITHER_FASTEST_PHOTO = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_FASTEST |
				BVDITHERDEF_PHOTO,
	BVDITHER_FASTEST_DRAWING = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_FASTEST |
				BVDITHERDEF_DRAWING,
	BVDITHER_GOOD =		BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_GOOD |
				BVDITHERDEF_ON,
	BVDITHER_GOOD_RANDOM = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_GOOD |
				BVDITHERDEF_RANDOM,
	BVDITHER_GOOD_ORDERED = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_GOOD |
				BVDITHERDEF_ORDERED,
	BVDITHER_GOOD_DIFFUSED = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_GOOD |
				BVDITHERDEF_DIFFUSED,
	BVDITHER_GOOD_PHOTO =	BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_GOOD |
				BVDITHERDEF_PHOTO,
	BVDITHER_GOOD_DRAWING = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_GOOD |
				BVDITHERDEF_DRAWING,
	BVDITHER_BETTER =	BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_BETTER |
				BVDITHERDEF_ON,
	BVDITHER_BETTER_RANDOM = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_BETTER |
				BVDITHERDEF_RANDOM,
	BVDITHER_BETTER_ORDERED = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_BETTER |
				BVDITHERDEF_ORDERED,
	BVDITHER_BETTER_DIFFUSED = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_BETTER |
				BVDITHERDEF_DIFFUSED,
	BVDITHER_BETTER_PHOTO =	BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_BETTER |
				BVDITHERDEF_PHOTO,
	BVDITHER_BETTER_DRAWING = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_BETTER |
				BVDITHERDEF_DRAWING,
	BVDITHER_BEST =		BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_BEST |
				BVDITHERDEF_ON,
	BVDITHER_BEST_RANDOM = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_BEST |
				BVDITHERDEF_RANDOM,
	BVDITHER_BEST_ORDERED = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_BEST |
				BVDITHERDEF_ORDERED,
	BVDITHER_BEST_DIFFUSED = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_BEST |
				BVDITHERDEF_DIFFUSED,
	BVDITHER_BEST_PHOTO = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_BEST |
				BVDITHERDEF_PHOTO,
	BVDITHER_BEST_DRAWING = BVDITHERDEF_VENDOR_ALL |
				BVDITHERDEF_BEST |
				BVDITHERDEF_DRAWING,

	BVDITHER_NONE =		BVDITHERDEF_VENDOR_GENERIC + 0,
	BVDITHER_ORDERED_2x2 =	BVDITHERDEF_VENDOR_GENERIC + 4,
	BVDITHER_ORDERED_4x4 =	BVDITHERDEF_VENDOR_GENERIC + 16,
	BVDITHER_ORDERED_2x2_4x4 = BVDITHERDEF_VENDOR_GENERIC + 4 + 16,	/* 2x2 for 6->8, 4x4 for 5->8 */
#ifdef BVDITHER_EXTERNAL_INCLUDE
#include BVDITHER_EXTERNAL_INCLUDE
#endif
};


/*
 * BVTILE_* flags - These specify parameters used when tiling.
 */
#define BVTILE_LEFT_SHIFT    0
#define BVTILE_TOP_SHIFT     (BVTILE_LEFT_SHIFT + 2)
#define BVTILE_RIGHT_SHIFT   (BVTILE_TOP_SHIFT + 2)
#define BVTILE_BOTTOM_SHIFT  (BVTILE_RIGHT_SHIFT + 2)
#define BVTILE_LEFT_REPEAT   (0 << BVTILE_LEFT_SHIFT)	/* ...012301230123 */
#define BVTILE_TOP_REPEAT    (0 << BVTILE_TOP_SHIFT)	/* ...012301230123 */
#define BVTILE_RIGHT_REPEAT  (0 << BVTILE_RIGHT_SHIFT)	/* 012301230123... */
#define BVTILE_BOTTOM_REPEAT (0 << BVTILE_BOTTOM_SHIFT)	/* 012301230123... */
#define BVTILE_LEFT_MIRROR   (1 << BVTILE_LEFT_SHIFT)	/* ...012332100123 */
#define BVTILE_TOP_MIRROR    (1 << BVTILE_TOP_SHIFT)	/* ...012332100123 */
#define BVTILE_RIGHT_MIRROR  (1 << BVTILE_RIGHT_SHIFT)	/* 012332100123... */
#define BVTILE_BOTTOM_MIRROR (1 << BVTILE_BOTTOM_SHIFT)	/* 012332100123... */

/*
 * bvtileparams - This structure provides additional parameters needed when
 * tiling.  This structure replaces the bvbuffermap in bvbltparams when the
 * associated BVFLAG_TILE_* flag is set in bvbltparams.flags.
 */
struct bvtileparams {
	unsigned int structsize; /* used to ID structure ver */
	unsigned long flags;	 /* tile flags */
	void *virtaddr;		 /* pointer to the brush */
	int dstleft;		 /* horizontal offset */
	int dsttop;		 /* vertical offset */
	unsigned int srcwidth;	 /* w/dst width to spec horz scale */
	unsigned int srcheight;	 /* w/dst height to spec vert scale */
};

/*
 * BVBATCH_* - These flags specify the parameters that change between
 * batched BLTs, when BVFLAG_CONTINUE or BVFLAG_END set.
 */
#define BVBATCH_OP		0x00000001 /* type of operation changed */
#define BVBATCH_KEY		0x00000002 /* color key changed */
#define BVBATCH_MISCFLAGS	0x00000004 /* other flags changed */
#define BVBATCH_ALPHA		0x00000008 /* global alpha changed */
#define BVBATCH_DITHER		0x00000010 /* dither changed */
#define BVBATCH_SCALE		0x00000020 /* scaling type changed */
/* Bits 6-7 reserved */
#define BVBATCH_DST		0x00000100 /* destination surface changed */
#define BVBATCH_SRC1		0x00000200 /* source 1 surface changed */
#define BVBATCH_SRC2		0x00000400 /* source 2 surface changed */
#define BVBATCH_MASK		0x00000800 /* mask surface changed */
#define BVBATCH_DSTRECT_ORIGIN	0x00001000 /* dest rect origin changed */
#define BVBATCH_DSTRECT_SIZE	0x00002000 /* dest rect dimensions changed */
#define BVBATCH_SRC1RECT_ORIGIN	0x00004000 /* src 1 rect origin changed */
#define BVBATCH_SRC1RECT_SIZE	0x00008000 /* src 1 rect dimensions changed */
#define BVBATCH_SRC2RECT_ORIGIN	0x00010000 /* src 2 rect origin changed */
#define BVBATCH_SRC2RECT_SIZE	0x00020000 /* src 2 rect dimensions changed */
#define BVBATCH_MASKRECT_ORIGIN	0x00040000 /* mask rect origin changed */
#define BVBATCH_MASKRECT_SIZE	0x00080000 /* mask rect dimensions changed */
#define BVBATCH_CLIPRECT_ORIGIN	0x00100000 /* Clip rect origin changed */
#define BVBATCH_CLIPRECT_SIZE	0x00200000 /* Clip rect dimensions changed */
#define BVBATCH_CLIPRECT	(BVBATCH_CLIPRECT_ORIGIN | \
				 BVBATCH_CLIPRECT_SIZE)	/* clip rect... */
							/* ...changed */
#define BVBATCH_TILE_SRC1	0x00400000 /* tile params for src 1 changed */
#define BVBATCH_TILE_SRC2	0x00800000 /* tile params for src 2 changed */
#define BVBATCH_TILE_MASK	0x00100000 /* tile params for mask changed */
/* Bits 30-21 reserved */
#define BVBATCH_ENDNOP		0x80000000 /* just end batch, don't do BLT */

/*
 * bvcallbackerror - This structure is passed into the callback function
 * if an error occurs.
 */
struct bvcallbackerror {
	unsigned int structsize;	/* used to ID structure ver */
	enum bverror error;		/* error during async BLT */
	char* errdesc;			/* 0-terminated ASCII string
					   with extended error info (not
					   for end users) */
};

/*
 * bvbatch - an implementation-specific container for batch information;
 * not used by client; forward declaration here
 */
struct bvbatch;

/*
 * bvbltparams - This structure is passed into bv_blt() to specify the
 * parameters for a BLT.
 */
struct bvbltparams {
	unsigned int structsize;	/* (i) used to ID structure ver */
	char *errdesc;			/* (o) 0-terminated ASCII string
					       with extended error info (not
					       for end users) */

	unsigned long implementation;	/* (i) override manager choice */

	unsigned long flags;		/* (i) see BVFLAG_* above */

	union {				/* (i) params for ops */
		unsigned short rop;		/* when BVFLAG_ROP set */
		enum bvblend blend;		/* when BVFLAG_BLEND set */
		struct bvfilterop *filter_ptr;	/* when BVFLAG_FILTER set */
	} op;

	void *colorkey_ptr;		/* (i) pointer to color key pixel
					       matching non-SUBSAMPLE format
					       of the keyed surface when
					       BVFLAG_KEY_* is set */

	union {				/* (i) global alpha when BVFLAG_BLEND
					       set in flags and
					       BVBLENDDEF_GLOBAL is set in
					       blend */
		unsigned char size8;	/* btwn 0 (0.0) and 255 (1.0) */
		unsigned short size16;	/* btwn 0 (0.0) and 65535 (1.0) */
		unsigned long size32;	/* btwn 0 (0.0) and 4^32-1 (0.0) */
		float fp;		/* btwn 0.0 and 1.0 */
	} globalalpha;

	enum bvscalemode scalemode;	/* (i/o) type of scaling */
	enum bvdithermode dithermode;	/* (i/o) type of dither */

	struct bvbuffermap *dstmap;	/* (i) dest buff from bv_map() */
	struct bvsurfgeom *dstgeom;	/* (i) dest surf fmt and geometry */
	struct bvrect dstrect;		/* (i) rect into which data written */

	union {				/* (i) src1 buffer */
		struct bvbuffermap *map;	 /* src1 buff from bv_map() */
		struct bvtileparams *tileparams; /* tile params when
						    BVFLAG_TILE_SRC1 set */
	} src1;
	struct bvsurfgeom *src1geom;	/* (i) src1 surf fmt and geometry */
	struct bvrect src1rect;		/* (i) rect from which data is read */

	union {				/* (i) src2 buffer */
		struct bvbuffermap *map;	 /* src2 buffer from bv_map() */
		struct bvtileparams *tileparams; /* tile params when
						    BVFLAG_TILE_SRC2 set */
	} src2;
	struct bvsurfgeom *src2geom;	/* (i) src2 surf fmt and geometry */
	struct bvrect src2rect;		/* (i) rect from which data is read */

	union {				/* (i) mask buffer */
		struct bvbuffermap *map;	 /* mask buffer from bv_map() */
		struct bvtileparams *tileparams; /* tile params when
						   BFFLAG_TILE_MASK is set */
	} mask;
	struct bvsurfgeom *maskgeom;	/* (i) mask surf fmt and geometry */
	struct bvrect maskrect;		/* (i) rect from which data is read */

	struct bvrect cliprect;		/* (i) dest clipping rect when
					       BVFLAG_CLIP flag set */

	unsigned long batchflags;	/* (i) flags used to indicate params
					       changed between batch BLTs */
	struct bvbatch *batch;		/* (i/o) handle for associated batch;
						 returned when
						 BVFLAG_BATCH_BEGIN set;
						 provided to subsequent BLTs
						 with BVFLAG_BATCH_CONTINUE */

	void (*callbackfn)(struct bvcallbackerror* err,
			   unsigned long handle); /* (i) callback function when
						         BVFLAG_ASYNC is set -
						         err is 0 when no
						         error; handle contains
						         callbackdata below */
	unsigned long callbackdata;	/* (i) callback data */
};

/*
 * bv_*() - These are the API calls for BLTsville.  The client needs to
 * import these from the shared library.
 */
typedef enum bverror (*BVFN_MAP)(struct bvbuffermap **buffermap,
				 struct bvbufferdesc *bufferdesc);
typedef enum bverror (*BVFN_BLT)(struct bvbltparams *bltparms);
typedef enum bverror (*BVFN_UNMAP)(struct bvbuffermap *buffermap);

#endif /* BLTSVILLE_H */

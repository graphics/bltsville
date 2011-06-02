/*
 * bverror.h
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

#ifndef BVERROR_H
#define BVERROR_H

/*
 * bverror - These are error codes returned by BLTsville functions.
 */
#define BVERRDEF_VENDOR_SHIFT	24
#define BVERRDEF_VENDOR_ALL	(0x00 << BVERRDEF_VENDOR_SHIFT)
#define BVERRDEF_VENDOR_TI	(0x01 << BVERRDEF_VENDOR_SHIFT)
/* 0xF0-0xFF reserved */
#define BVERRDEF_VENDOR_MASK	(0xFF << BVERRDEF_VENDOR_SHIFT)

enum bverror {
	BVERR_NONE = 0,		/* no error */

	BVERR_UNK =		/* unknown error */
		BVERRDEF_VENDOR_ALL + 1,
	BVERR_OOM =		/* memory allocation failure */
		BVERRDEF_VENDOR_ALL + 2,
	BVERR_RSRC =		/* required resource unavailable */
		BVERRDEF_VENDOR_ALL + 3,

	BVERR_VIRTPTR =		/* virtptr is bad */
		BVERRDEF_VENDOR_ALL + 1000,

	BVERR_BUFFERDESC_VERS =	/* bvbufferdesc.structsize too small */
		BVERRDEF_VENDOR_ALL + 10000,
	BVERR_MEMORY_UNMAPPABLE = /* can't map bvbufferdesc.virtptr */
		BVERRDEF_VENDOR_ALL + 11000,
	BVERR_BUFFERDESC_LEN =	/* bvbufferdesc.length not supported */
		BVERRDEF_VENDOR_ALL + 12000,

	BVERR_BLTPARAMS_VERS =	/* bvbltparams.structsize too small */
		BVERRDEF_VENDOR_ALL + 20000,
	BVERR_FLAGS =		/* bvbltparams.flags unsupported */
		BVERRDEF_VENDOR_ALL + 21000,
	BVERR_OP =		/* unsupported operation */
		BVERRDEF_VENDOR_ALL + 21100,
	BVERR_KEY =		/* type of color key not supported */
		BVERRDEF_VENDOR_ALL + 21200,
	BVERR_FLIP =		/* flipping not supported */
		BVERRDEF_VENDOR_ALL + 21300,

	BVERR_SRC1_TILE =	/* src1 tiling not supported */
		BVERRDEF_VENDOR_ALL + 21400,
	BVERR_SRC2_TILE =	/* src2 tiling not supported */
		BVERRDEF_VENDOR_ALL + 21410,
	BVERR_MASK_TILE =	/* mask tiling not supported */
		BVERRDEF_VENDOR_ALL + 21420,

	BVERR_ROP =		/* ROP code not supported */
		BVERRDEF_VENDOR_ALL + 22000,
	BVERR_BLEND =		/* blend not supported */
		BVERRDEF_VENDOR_ALL + 22100,
	BVERR_FILTER =		/* filter type not supported */
		BVERRDEF_VENDOR_ALL + 22200,
	BVERR_FILTER_PARAMS_VERS = /* filter parameter structsize too small */
		BVERRDEF_VENDOR_ALL + 22210,
	BVERR_FILTER_PARAMS =	/* filter parameters not supported */
		BVERRDEF_VENDOR_ALL + 22220,

	BVERR_DST_HANDLE =	/* invalid bvbltparams.dstmap */
		BVERRDEF_VENDOR_ALL + 23000,
	BVERR_DST_VERS =	/* dstgeom.structsize too small */
		BVERRDEF_VENDOR_ALL + 24000,
	BVERR_DST_FORMAT =	/* bltparams.dstgeom.format not supported */
		BVERRDEF_VENDOR_ALL + 24100,
	BVERR_DST_RECT =	/* bvbltparams.dstrect not supported */
		BVERRDEF_VENDOR_ALL + 24200,
	BVERR_DST_STRIDE =	/* bltparams.dstgeom.stride not supported */
		BVERRDEF_VENDOR_ALL + 24300,

	BVERR_SRC1_HANDLE =	/* invalid bvbltparams.src1.map */
		BVERRDEF_VENDOR_ALL + 25000,

	BVERR_SRC1_TILE_VERS =	/* mask.tileparams.structsize too small */
		BVERRDEF_VENDOR_ALL + 25100,
	BVERR_SRC1_TILE_FLAGS =	/* tileparams.flags not supported */
		BVERRDEF_VENDOR_ALL + 25200,
	BVERR_SRC1_TILE_ORIGIN = /* tileparams.left or .top not supported */
		BVERRDEF_VENDOR_ALL + 25300,
	BVERR_SRC1_TILE_SIZE =	/* tileparams.width or .height not supported */
		BVERRDEF_VENDOR_ALL + 25400,

	BVERR_SRC1_VERS =	/* src1geom.structsize too small */
		BVERRDEF_VENDOR_ALL + 26000,
	BVERR_SRC1_FORMAT =	/* bltparams.src1geom.format not supported */
		BVERRDEF_VENDOR_ALL + 26100,
	BVERR_SRC1_STRIDE =	/* bltparams.src1geom.stride not supported */
		BVERRDEF_VENDOR_ALL + 26200,
	BVERR_SRC1_RECT =	/* bvbltparams.src1rect not supported */
		BVERRDEF_VENDOR_ALL + 26300,
	BVERR_SRC1_HORZSCALE =	/* horz scale for src1->dst not supported */
		BVERRDEF_VENDOR_ALL + 26310,
	BVERR_SRC1_VERTSCALE =	/* vert scale for src1->dst not supported */
		BVERRDEF_VENDOR_ALL + 26320,
	BVERR_SRC1_ROT = 	/* src1->dst rotation angle not supported */
		BVERRDEF_VENDOR_ALL + 26340,

	BVERR_SRC2_HANDLE =	/* invalid bvbltparams.src2.map */
		BVERRDEF_VENDOR_ALL + 27000,

	BVERR_SRC2_TILE_VERS =	/* mask.tileparams.structsize too small */
		BVERRDEF_VENDOR_ALL + 27100,
	BVERR_SRC2_TILE_FLAGS =	/* tileparams.flags not supported */
		BVERRDEF_VENDOR_ALL + 27200,
	BVERR_SRC2_TILE_ORIGIN = /* tileparams.left or .top not supported */
		BVERRDEF_VENDOR_ALL + 27300,
	BVERR_SRC2_TILE_SIZE =	/* tileparams.width or .height not supported */
		BVERRDEF_VENDOR_ALL + 27400,

	BVERR_SRC2_VERS =	/* src2geom.structsize too small*/
		BVERRDEF_VENDOR_ALL + 28000,
	BVERR_SRC2_FORMAT =	/* bltparams.src2geom.format not supported */
		BVERRDEF_VENDOR_ALL + 28100,
	BVERR_SRC2_STRIDE =	/* bltparams.src2geom.stride not supported */
		BVERRDEF_VENDOR_ALL + 28200,
	BVERR_SRC2_RECT =	/* bvbltparams.src2rect not supported */
		BVERRDEF_VENDOR_ALL + 28300,
	BVERR_SRC2_HORZSCALE =	/* horz scale for src2->dst not supported */
		BVERRDEF_VENDOR_ALL + 28310,
	BVERR_SRC2_VERTSCALE =	/* vert scale for src2->dst not supported */
		BVERRDEF_VENDOR_ALL + 28320,
	BVERR_SRC2_ROT = 	/* src2->dst rotation angle not supported */
		BVERRDEF_VENDOR_ALL + 28330,

	BVERR_MASK_HANDLE =	/* invalid bvbltparams.mask.map */
		BVERRDEF_VENDOR_ALL + 29000,

	BVERR_MASK_TILE_VERS =	/* mask.tileparams.structsize too small */
		BVERRDEF_VENDOR_ALL + 29100,
	BVERR_MASK_TILE_FLAGS =	/* tileparams.flags not supported */
		BVERRDEF_VENDOR_ALL + 29200,
	BVERR_MASK_TILE_ORIGIN = /* tileparams.left or .top not supported */
		BVERRDEF_VENDOR_ALL + 29300,
	BVERR_MASK_TILE_SIZE =	/* tileparams.width or .height not supported */
		BVERRDEF_VENDOR_ALL + 29400,

	BVERR_MASK_VERS =	/* maskgeom.structsize too small */
		BVERRDEF_VENDOR_ALL + 30000,
	BVERR_MASK_FORMAT =	/* bltparams.maskgeom.format not supported */
		BVERRDEF_VENDOR_ALL + 30100,
	BVERR_MASK_STRIDE =	/* bltparams.src1geom.stride not supported */
		BVERRDEF_VENDOR_ALL + 30200,
	BVERR_MASK_RECT =	/* bvbltparams.maskrect not supported */
		BVERRDEF_VENDOR_ALL + 30300,
	BVERR_MASK_HORZSCALE =	/* horz scale for mask->dst not supported */
		BVERRDEF_VENDOR_ALL + 30310,
	BVERR_MASK_VERTSCALE =	/* vert scale for mask->dst not supported */
		BVERRDEF_VENDOR_ALL + 30320,
	BVERR_MASK_ROT = 	/* mask->dst rotation angle not supported */
		BVERRDEF_VENDOR_ALL + 30330,

	BVERR_CLIP_RECT =	/* bvbltparams.cliprect not supported */
		BVERRDEF_VENDOR_ALL + 31000,

	BVERR_DITHER_MODE =	/* bvbltparams.dithermode not supported */
		BVERRDEF_VENDOR_ALL + 32000,
	BVERR_SCALE_MODE =	/* bvbltparams.scalemode not supported */
		BVERRDEF_VENDOR_ALL + 33000,

	BVERR_BATCH_FLAGS =	/* bvbltparams.batchflags not supported */
		BVERRDEF_VENDOR_ALL + 34000,
	BVERR_BATCH_HANDLE =	/* bvbltparams.batchhandle not valid */
		BVERRDEF_VENDOR_ALL + 35000,

	BVERR_OP_FAILED =	/* async operation failed to start */
		BVERRDEF_VENDOR_ALL + 50000,
	BVERR_OP_INCOMPLETE =	/* async operation failed mid-way */
		BVERRDEF_VENDOR_ALL + 50001,
	BVERR_MEMORY_ERROR =	/* async operation triggered memory error */
		BVERRDEF_VENDOR_ALL + 52000,

#ifdef BVERR_EXTERNAL_INCLUDE
#include BVERR_EXTERNAL_INCLUDE
#endif
};

#endif /* BVERROR_H */
